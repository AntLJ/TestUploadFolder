/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "StdAfx.h"
#include "CircleFitEvaluator.h"
#include "CurveInfoUtility.h"
#include <iomanip>

namespace curve_info
{
	namespace circle_fit_evaluator
	{
		using namespace utility;
		using namespace spatial_reference;

		ResultsPtr CircleFitEvaluator::evaluate()
		{
			ATLASSERT( m_pTargetPoint );
			IPointPtr ipPoint2 = m_pTargetPoint;	///< 評価対象点が、３点のうちの２点目

			const double aDACT2 = m_DACTofTargetPoint;

			// 「３点の間隔」のそれぞれについて
			BOOST_FOREACH (const long aDistanceOf3P, m_rDistanceOf3PointsArray)
			{
				HRESULT hr;

				// 第１点と第３点の位置を決定
				IPointPtr ipPoint1 = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				IPointPtr ipPoint3 = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				double aDACT1 = aDACT2 - aDistanceOf3P / m_DegreeToMeterCoef;		///< ３点の内の第１点の、対象形状からの道のり
				double aDACT3 = aDACT2 + aDistanceOf3P / m_DegreeToMeterCoef;		///< ３点の内の第３点の、対象形状の始点からの道のり
				hr = m_pPolycurve->QueryPoint( esriNoExtension, aDACT1, VARIANT_FALSE, ipPoint1 );
				ATLASSERT( hr == S_OK );
				hr = m_pPolycurve->QueryPoint( esriNoExtension, aDACT3, VARIANT_FALSE, ipPoint3 );
				ATLASSERT( hr == S_OK );
				ATLASSERT( ipPoint1 && ipPoint3 );
				setSpatialReference( ipPoint1, m_pSpRef );
				setSpatialReference( ipPoint3, m_pSpRef );

				// 第１点または第３点が対象形状の範囲外ならスキップ
				// TODO: ここで僅かに範囲外に出て評価されない場合が多い。
				if (aDACT1 <= 0 || m_pPolycurve->Length <= aDACT3)
					{ continue; }

				// ３点を通る円弧を求める
				ICircularArcPtr ipCA = newGeometry< ICircularArcPtr >( CLSID_CircularArc, m_pSpRef );
				IConstructCircularArcPtr ipCCA( ipCA );
				hr = ipCCA->ConstructThreePoints( ipPoint1, ipPoint2, ipPoint3, VARIANT_FALSE );
				ATLASSERT( hr == S_OK );
				setSpatialReference( ipCA, m_pSpRef );
				// 円弧を完全な円にする
				IPointPtr ipCenterPoint( ipCA->GetCenterPoint() );
				// 中心点が得られなければスキップ
					// ３点が一直線上にある時、ベジエ曲線群は線分となり、ICircularArc.GetCenterPoint()が失敗する。
					// （だったらIConstructCircularArcPtr.ConstructThreePoints()で失敗すべきな気もするが…。）
					// TODO: 「セグメント群が線分（＝一切曲がっていない）か」はもっと早い段階でチェックすべきか？
				if (! ipCenterPoint)
					{ continue; }

				hr = ipCCA->ConstructCircle( ipCenterPoint, ipCA->GetRadius(), VARIANT_FALSE );
				ATLASSERT( hr == S_OK );
				setSpatialReference( ipCA, m_pSpRef );
				ATLASSERT( ipCA->IsClosed == VARIANT_TRUE );	// 円になった事の確認
				// 注：円の始点は0時方向の点になる。 http://edndoc.esri.com/arcobjects/9.1/componenthelp/esrigeometry/IConstructCircularArc_ConstructCircle.htm

				// 作った円が小さすぎるなら評価対象外とする（Bug9737）
				// 「円弧が対象形状にフィットするかを評価する点の間隔」が円周の半分以上なら、この円は評価対象外
				if ( ipCA->GetLength() <= m_FittingPointInterval * 2 )
					{ continue; }

				// 円の始点から第２点までの道のりを求める
				double aDACCofP2 = 0;						///< 円の始点から第２点までの道のり
				{
					double aDFCCofP2 = 0;						// 使用しない
					VARIANT_BOOL aRightSide = VARIANT_FALSE;	// 使用しない
					hr = ipCA->QueryPointAndDistance( esriNoExtension, ipPoint2, VARIANT_FALSE, IPointPtr( CLSID_Point ), &aDACCofP2, &aDFCCofP2, &aRightSide );
					ATLASSERT( hr == S_OK );
				}

				// 円周上を一定間隔で進み、ベジエ曲線群との距離をチェックしていく
				long aFitCount = 0;
				aFitCount += fitCount( ipCA, aDACCofP2, aDistanceOf3P, true );		// 順方向（円の終点側）
				aFitCount += fitCount( ipCA, aDACCofP2, aDistanceOf3P, false );		// 逆方向（円の始点側）

				// 結果を記録
				addResult( ResultPtr( new Result(
					m_pTargetPoint,
					aDistanceOf3P,
					ipCA->GetRadius() * m_DegreeToMeterCoef,
					aFitCount
				) ) );
			}

			return m_pResults;
		}

		long CircleFitEvaluator::fitCount(
			const ICircularArcPtr cpCA,
			const double cDACCofTargetPoint,
			const long cDistanceOf3P,
			const bool cIsForward
		)
		{
			HRESULT hr;

			long aCircleIndex = 0;											///< 円周上の等間隔な点群のインデックス
			double aDistanceFromP2 = 0;										///< 第２点から円周上の点までの道のり
			double aDistanceFromP2Max = M_PI * cpCA->GetRadius() / 2;		///< aDistanceFromP2の最大値（円周の1/4）
			long aFitCount = 0;
			for (; aDistanceFromP2 < aDistanceFromP2Max; ++aCircleIndex)
			{
				aDistanceFromP2 = m_FittingPointInterval * aCircleIndex;
				
				// 最初の点は順方向の場合のみ評価する
				if (aDistanceFromP2 == 0 && ! cIsForward)
					{ continue; }

				// 円周上の点を決定
				IPointPtr ipCirclePoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				double aDAC = cIsForward ?						///< 円の始点からipPointまでの道のり
					cDACCofTargetPoint + aDistanceFromP2 :
					cDACCofTargetPoint - aDistanceFromP2;
				hr = cpCA->QueryPoint( esriExtendEmbedded, aDAC, VARIANT_FALSE, ipCirclePoint );
				ATLASSERT( hr == S_OK );
				setSpatialReference( ipCirclePoint, m_pSpRef );

				// 対象形状までの距離を求める
				IProximityOperatorPtr ipPOpe( m_pPolycurve );
				IPointPtr ipCurvePoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
				hr = ipPOpe->QueryNearestPoint( ipCirclePoint, esriNoExtension, ipCurvePoint );
				ATLASSERT( hr == S_OK );
				setSpatialReference( m_pPolycurve, m_pSpRef );
				setSpatialReference( ipCurvePoint, m_pSpRef );
				setSpatialReference( ipCirclePoint, m_pSpRef );

				bool aFit = false;	///< フィットしたか否か

				// 「円周上の点に最も近い対象形状上の点」が対象形状の端点である場合はフィットしなかったと見なす
				if (ipCurvePoint->_Compare( m_pPolycurve->GetFromPoint() ) != 0 && ipCurvePoint->_Compare( m_pPolycurve->GetToPoint() ) != 0)
				{
					// 端点でなければ、距離で判定
					// TODO: 厳密にはcrd_cnvで計算すべき？
					double aDistanceToTargetGeom = distance(
						ipCirclePoint->GetX(),	ipCirclePoint->GetY(),
						ipCurvePoint->GetX(),	ipCurvePoint->GetY()
					);

					aFit = ( aDistanceToTargetGeom < m_FittingThreshold );
				}

				// 必要ならフィット評価点の情報をログに出力
				if (m_LogFitEvaluatingProcess)
				{
					m_rLogStream << utility::log_format::pointRecord( ipCirclePoint->GetX(), ipCirclePoint->GetY(), aFit ? "フィット評価点（フィットした）" : "フィット評価点（フィットしなかった）" );
				}

				// 距離が閾値未満ならカウント加算
				if (aFit)
				{
					++aFitCount;
				}
				// 距離が閾値以上ならその先はフィットしないと見なす（高速化のため）
				// TODO: 少し先までは様子を見てもいいかも。10点見て再フィットしなければ終了とか。
				else
				{
					break;
				}
			}

			return aFitCount;
		}

		void CircleFitEvaluator::addResult( const ResultPtr cpResult )
		{
			// 評価結果群コンテナがまだ生成されていなければ生成する
			if (! m_pResults)
			{
				m_pResults = ResultsPtr( new Results() );
			}

			m_pResults->push_back( cpResult );
		}

		const ResultPtr bestFitResult( const Results& crResults, std::ostream* cpLogStream /*= NULL*/ )
		{
			ResultsRanking aResultsRanking;
			std::copy( crResults.begin(), crResults.end(), std::inserter( aResultsRanking, aResultsRanking.begin() ) );

			// 必要なら評価過程をログに出力
			if (cpLogStream)
			{
				outputResults( aResultsRanking, *cpLogStream );
			}

			return *aResultsRanking.begin();
		}

		void outputResults( const ResultsRanking& crResults, std::ostream& co )
		{
			BOOST_FOREACH (const ResultPtr pResult, crResults)
			{
				ATLASSERT( pResult );
				const Result& rResult = *pResult;
				std::stringstream ss;
				ss  << std::setprecision( 20 )
					<< "３点の間隔=" << rResult.m_DistanceOf3Points << ", "
					<< "曲率半径=" << rResult.m_ROC << ", "
					<< "フィット数=" << rResult.m_FitCount << ", "
					<< "スコア=" << rResult.score();
				co << utility::log_format::pointRecord( rResult.m_pTargetPoint->GetX(), rResult.m_pTargetPoint->GetY(), ss.str() );
			}
		}

	} // namespace circle_fit_evaluator
} // namespace curve_info

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
#include "AngleAccumulating.h"
#include "CurveInfoUtility.h"
#include <boost/utility.hpp>	// for prior/next

namespace curve_info
{
	/// 角度の累積に関する概念
	namespace angle_accumulating
	{
		using namespace utility;
		using namespace spatial_reference;

		void LRAccumulating::update( double cAngle, SegmentIt citCurrentSegment )
		{
			// 状況確認
			bool aIsStraight = (fabs( cAngle ) < m_StraightAngle);
			curve_direction::ECode aDirection =
				aIsStraight ? curve_direction::straight :
				(0 < cAngle) ? curve_direction::left : curve_direction::right;

			// 左右に曲がった状態から、それまでとは違う方向（直線も含む）になったら終了セグメントを記録
			if (m_LeftAccumulating.isCurved() && aDirection != curve_direction::left)
			{
				if (! m_LeftAccumulating.hasEnded())
				{
					m_LeftAccumulating.endAt( boost::prior( citCurrentSegment ) );
				}
			}
			if (m_RightAccumulating.isCurved() && aDirection != curve_direction::right)
			{
				if (! m_RightAccumulating.hasEnded())
				{
					m_RightAccumulating.endAt( boost::prior( citCurrentSegment ) );
				}
			}

			if (! aIsStraight)
			{
				// 左に曲がっている場合
				if	(aDirection == curve_direction::left)
				{
					update( cAngle, citCurrentSegment, curve_direction::left );
				}
				// 右に曲がっている場合
				else
				{
					update( cAngle, citCurrentSegment, curve_direction::right );
				}
			}
		}

		void LRAccumulating::update( const double cAngle, const SegmentIt citCurrentSegment, const curve_direction::ECode cDirection )
		{
			HRESULT hr;

			// 左右どちらかなのを前提とする
			if (cDirection != curve_direction::right && cDirection != curve_direction::left)
			{
				ATLASSERT(0);
				return;
			}

			Accumulating& rCurrentSideAcc = (cDirection == curve_direction::left) ? m_LeftAccumulating : m_RightAccumulating;
			Accumulating& rAnotherSideAcc = (cDirection == curve_direction::left) ? m_RightAccumulating : m_LeftAccumulating;

			// 累積を開始していなければ開始する
			if (! rCurrentSideAcc.hasBegun())
			{
				rCurrentSideAcc.beginAt( boost::prior( citCurrentSegment ) );
			}

			// 角度を累積
			rCurrentSideAcc.addAngle( cAngle );
			m_CurrentDirection = cDirection;

			// 累積角度が「方向を決定する角度」を超えたらカーブの方向を決定する（超えていないなら終了）
			if (rCurrentSideAcc.m_Angle <= m_CurveCertifyAngle)
				{ return; }

			// 曲がっている方向の終了セグメントを初期化（記録されていても、なかった事にする）
			rCurrentSideAcc.resetEnd();
			// 逆方向の開始セグメントを初期化（記録されていても、なかった事にする）
			rAnotherSideAcc.resetBegin();

			// 既に逆方向のカーブだと認定されているなら、変曲点を通り過ぎたと見なす（認定されていなければ修了）
			if (rAnotherSideAcc.m_Angle <= m_CurveCertifyAngle)
				{ return; }

			// 変曲点の位置を決定
			// 一方の終了セグメントから、他方の開始セグメントまでの間にあるセグメント群の中点を変曲点と見なす
			SegmentIt itEndSegment = rAnotherSideAcc.m_EndSegment;
			SegmentIt itBeginSegment = rCurrentSideAcc.m_BeginSegment;
			IPointCollectionPtr ipPointCol = newGeometry< IPointCollectionPtr >( CLSID_Polyline, m_pSpRef );
			double aSegmentsLength = 0;		///< セグメント群の長さ（度）
#ifdef _DEBUG
			long aPointIndex = 0;
#endif
			for (SegmentIt itSegment = itEndSegment; itSegment != boost::next( itBeginSegment ); ++itSegment)
			{
#ifdef _DEBUG
				if (aPointIndex == 0)
				{
//								std::cout << std::setprecision( 20 ) << (*itSegment)->m_pFromPoint->GetX() << "\t" << (*itSegment)->m_pFromPoint->GetY() << "\t" << aPointIndex << std::endl;
					++aPointIndex;
				}
//							std::cout << std::setprecision( 20 ) << (*itSegment)->m_pToPoint->GetX() << "\t" << (*itSegment)->m_pToPoint->GetY() << "\t" << aPointIndex << std::endl;
				++aPointIndex;
#endif
				if (itSegment == itEndSegment)
				{
					hr = ipPointCol->AddPoint( (*itSegment)->m_pFromPoint );
					ATLASSERT( hr == S_OK );
				}
				hr = ipPointCol->AddPoint( (*itSegment)->m_pToPoint );
				ATLASSERT( hr == S_OK );

				aSegmentsLength += (*itSegment)->length();

				ATLASSERT( itSegment != m_rSingleRoad.end() );	// EndとBeginで挟まれた領域があるはず
			}
			ICurvePtr ipCurve( ipPointCol );
			ATLASSERT( ipCurve );
			m_pInflectionPoint = newGeometry< IPointPtr >( CLSID_Point, m_pSpRef );
			hr = ipCurve->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, m_pInflectionPoint );
			ATLASSERT( hr == S_OK );
			ATLASSERT( m_pInflectionPoint );
			setSpatialReference( m_pInflectionPoint, m_pSpRef );

			// 変曲点セグメントを求める
			double aSegmentsLengthHalf = aSegmentsLength / 2;	///< セグメント群の長さの半分（度）
			double aPosOnSegments = 0;							///< セグメント群の始点からの道のり（度）
			for (SegmentIt itSegment = itEndSegment; itSegment != boost::next( itBeginSegment ); ++itSegment)
			{
				aPosOnSegments += (*itSegment)->length();
				if (aSegmentsLengthHalf <= aPosOnSegments)
				{
					m_itInflectionSegment = itSegment;
					break;
				}
			}

			// 変曲点に関する処理が終わったら、それより前のカーブの情報は忘れる
			rAnotherSideAcc.reset();
		}

	} // namespace angle_accumulating
} // namespace curve_info

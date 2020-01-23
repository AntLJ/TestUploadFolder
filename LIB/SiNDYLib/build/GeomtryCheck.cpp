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

/**
 * @file GeometryCheck.cpp
 * @brief <b>CGeometryクラスチェック関数実装ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "EnumSegment.h"
#include "GeometryRule.h"
#include "ErrorObject.h"
#include "util.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "AheMeshUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR FAILED_TO_SIMPLIFY                = _T("Simplifyに失敗しました");
	LPCTSTR FAILED_TOINVOKE_PUT_ISKNOWNSIMPLE = _T("put_IsKnownSimpleに失敗しました");
	LPCTSTR AREA_CANNOT_BE_EDITED             = _T("編集可能エリアではありません");
	LPCTSTR AREA_IS_LOCKED                    = _T("ロックエリアです");
	LPCTSTR LIGHTNING_BOLT_SHAPE              = _T("稲妻形状[%.1lf度-%.1lf度（リミット：30度-30度）]");
	LPCTSTR PULSE_SHAPE_TURN                  = _T("パルス/鋭角ターン[%.1lf度（リミット：%.1lf度～%.1lf度）]");
	LPCTSTR FAILED_TO_QUERY_WKSENVELOPE       = _T("ISegment::QueryWKSEnvelopeに失敗");
	LPCTSTR SLIVER_SEGMENT                    = _T("微小セグメント[位置：%d 距離：%.3lfm 制限：%.3lfm以上]");
#else
	LPCTSTR FAILED_TO_SIMPLIFY                = _T("Failed to simplify.");
	LPCTSTR FAILED_TOINVOKE_PUT_ISKNOWNSIMPLE = _T("Failed to invoke put_IsKnownSimple.");
	LPCTSTR AREA_CANNOT_BE_EDITED             = _T("This is not an area that can be edited.");
	LPCTSTR AREA_IS_LOCKED                    = _T("This area is locked.");
	LPCTSTR LIGHTNING_BOLT_SHAPE              = _T("Lightning bolt shape [%.1lf degrees -%.1lf degrees (limit: 30 degrees -30degrees)])");
	LPCTSTR PULSE_SHAPE_TURN                  = _T("Pulse/sharp turn [%.1lf degrees (limit: %.1lf degrees to %.1lf degrees）]");
	LPCTSTR FAILED_TO_QUERY_WKSENVELOPE       = _T("Failed to ISegment::QueryWKSEnvelope.");
	LPCTSTR SLIVER_SEGMENT                    = _T("Sliver segment [Position: %d, Distance: %.3lfm, Limit: %.3lfm or more]");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
using namespace errorcode;

long GetExteriorRingCount( IGeometry* ipGeom )
{
	long lRet = 0;
	IPolygonPtr ipPoly( ipGeom );
	if( ipPoly != NULL )
		ipPoly->get_ExteriorRingCount( &lRet );

	return lRet;
}

sindyErrCode CGeometry::CheckLogic( const CBasicLogicRule& cRule, CErrorInfos& cErrInfos, IEnvelope* ipQuery/* = NULL*/ ) const
{
	// [CHECK] 形状が存在するかどうか
	if( ! IsEmpty() )
	{
		sindyErrCode emErr = sindyErr_NoErr;
		switch( GetGeometryType() )
		{
			case esriGeometryPoint:
			case esriGeometryPolyline:
			{
				IPolylinePtr ipPolyline( (IGeometry*)(*this) );
				if( ipPolyline != NULL )
					ipPolyline->SimplifyNetwork(); // TODO: 勝手に形状を変えるのはいかがなものか

				// [CHECK] パート数（ポリライン、ポイント）
				if( 1 < GetPartCount() )
					cErrInfos.push_back( CErrorInfo( sindyErr_GeometryPartTooMach, sindyErrLevel_ERR ) );

				// [CHECK] ループチェック
				if( cRule.DoCheckRoop() )
				{
					VARIANT_BOOL vbIsClosed = VARIANT_FALSE;
					if( SUCCEEDED( ((IPolylinePtr)(IGeometry*)(*this))->get_IsClosed( &vbIsClosed ) ) && vbIsClosed )
					{
						cErrInfos.push_back( CErrorInfo( sindyErr_GeometryRooped, sindyErrLevel_ERR ) );
					}
				}
				break;
			}
			case esriGeometryPolygon:
			{
				ITopologicalOperator3Ptr ipTopo( (IGeometry*)(*this) );
				if( ipTopo != NULL )
				{
					if( SUCCEEDED( ipTopo->put_IsKnownSimple( VARIANT_FALSE ) ) )
					{
						// [CHECK] シンプルチェック（この方法ではポリゴンのみしか実装されていない）
						//         Simplifyをかける前に行わないと、ポリゴンの始終点のうち片方のみが移動されてる場合に検出できない
						VARIANT_BOOL vaIsSimple = VARIANT_FALSE;
						esriNonSimpleReasonEnum emReason = esriNonSimpleOK;
						if( SUCCEEDED( ipTopo->get_IsSimpleEx( &emReason, &vaIsSimple ) ) )
						{
							if( ! vaIsSimple )
							{
								switch( emReason )
								{
									case esriNonSimpleOK:                 emErr = sindyErr_NoErr;                               break;
									case esriNonSimpleRingOrientation:    emErr = sindyErr_GeometryNonSimpleRingOrientation;    break;
									case esriNonSimpleSegmentOrientation: emErr = sindyErr_GeometryNonSimpleSegmentOrientation; break;
									case esriNonSimpleShortSegments:      emErr = sindyErr_GeometryNonSimpleShortSegments;      break;
									case esriNonSimpleSelfIntersections:  emErr = sindyErr_GeometryNonSimpleSelfIntersections;  break;
									case esriNonSimpleUnclosedRing:       emErr = sindyErr_GeometryNonSimpleUnclosedRing;       break;
									case esriNonSimpleEmptyPart:          emErr = sindyErr_GeometryNonSimpleEmptyPart;          break;
									default:                              emErr = sindyErr_GeometryNonSimple;                   break;
								}
								if( sindyErr_NoErr != emErr )
									cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR ) );
							}
						}
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_GeometryNonSimple, sindyErrLevel_ERR ) );

						if( SUCCEEDED( ipTopo->Simplify() ) ) // TODO: 勝手に形状を変えるのはいかがなものか
						{
							// [CHECK] エクステリア数（ポリライン、ポイント）
							if( 1 < GetExteriorRingCount((IGeometry*)*this) ) // <- GetExteriorRingCountはメンバとするか、Ahe系関数とすること
								cErrInfos.push_back( CErrorInfo( sindyErr_GeometryExteriorTooMach, sindyErrLevel_ERR ) );
						}
						else
							cErrInfos.push_back( CErrorInfo( sindyErr_GeometryFunctionFailed, sindyErrLevel_ERR, NULL, -1, -1, FAILED_TO_SIMPLIFY ) );
					}
					else
						cErrInfos.push_back( CErrorInfo( sindyErr_GeometryFunctionFailed, sindyErrLevel_ERR, NULL, -1, -1, FAILED_TOINVOKE_PUT_ISKNOWNSIMPLE ) );
				}
				break;
			}
			default:
				cErrInfos.push_back( CErrorInfo( sindyErr_GeometryNotSupported, sindyErrLevel_ERR ) );
				break;
		}

		// [CHECK] 編集可能エリアかどうか
		emErr = g_cRule.IsEditableArea( cRule.GetTableName(), (IGeometry*)(*this) );
		if( ! ( emErr == sindyErr_NoErr || emErr == sindyErr_NoLogin ) )
			cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, -1, AREA_CANNOT_BE_EDITED ) );

		// [CHECK] ロックエリアかどうか
		emErr = g_cRule.IsLockArea( cRule.GetConnectUserName(), cRule.GetTableName(), (IGeometry*)(*this) );
		if( ! ( sindyErr_NoErr == emErr || sindyErr_NoLogin == emErr )  )
			cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, -1, AREA_IS_LOCKED ) );

		// セグメント単位でのチェック
		CheckSegments( cRule, cErrInfos, ipQuery );

		// ポイント単位でのチェック
		CheckPoints( cRule, cErrInfos, ipQuery );
	}
	else
		cErrInfos.push_back( CErrorInfo( sindyErr_GeometryNotFound, sindyErrLevel_ERR ) );

	return cErrInfos.GetDefaultErrCode();
}

// セグメント系チェック
sindyErrCode CGeometry::CheckSegments(const CBasicLogicRule& cRule, CErrorInfos& cErrInfos, IEnvelope* ipQuery/* = NULL*/ ) const
{
	using namespace sindy::schema;

	// セグメント系チェックはポイント系以外
	esriGeometryType emType = GetGeometryType();
	if( esriGeometryPoint == emType || esriGeometryMultipoint == emType )
		return cErrInfos.GetDefaultErrCode();

	for( const_part_iterator itPart = part_begin(); itPart != part_end(); ++itPart )
	{
		CGeometry cPart( *itPart );
		double dPrevDeg = -1.0;	// 前回の角度（稲妻形状チェック用）
		const_segment_iterator it = cPart.segment_begin(ipQuery), it2 = cPart.segment_begin(ipQuery); ++it2;
		for( const_segment_iterator itEnd = cPart.segment_end(); it != itEnd; ++it, ++it2 )
		{
			_ISegmentPtr ipSegment;
			// itが最終セグメントの場合は現在のパートの最初を比較対象とする
			// ただし、ipQueryが与えられていることを考慮し、IsLastInPartの
			// 時に限定する。
			if( it2 == itEnd )
			{
				if( it.IsLastInPart() ) // ipQueryにより最終セグメントがこない場合もある
				{
					// [Bug 5937] 鋭角ターンにならないはずなのに警告が出る
					// ライン系は最終セグメントのチェックしなくていいはず。
					if ( emType == esriGeometryPolygon || emType == esriGeometryRing ) {
						// セグメントが一つしかない場合は飛ばす
						long lSegCount = 0;
						((ISegmentCollectionPtr)*itPart)->get_SegmentCount( &lSegCount );
						if( 1 < lSegCount )
							((ISegmentCollectionPtr)*itPart)->get_Segment( 0, &ipSegment );
					}
				}
			}
			else
				ipSegment = *it2;

			if( ipSegment )
			{
				// [CHECK] セグメントがなす角度チェック
				double dDeg = it.degree( (ILinePtr)ipSegment );
				if( cRule.GetMinDeg() <= dDeg && dDeg <= cRule.GetMaxDeg() )
				{
					// TODO 以下、鉄道ラインを特別扱いした処理になっているため、形状チェックをフィーチャクラス別にするなどして対応するべき
					// [SJ鉄道全路線対応] 鉄道ラインでは稲妻形状を作成できるようにするため、チェックは行わない
					if( esriGeometryPolyline == emType && lstrcmpi( sj::railway_link::kTableName, cRule.GetTableName() ) != 0)
					{
						// [CHECK] 稲妻チェック（ポリラインのみ）
						if( dPrevDeg <= 30 && dDeg <= 30 )
							cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentsAngleOverFlow, sindyErrLevel_ERR, NULL, -1, -1, LIGHTNING_BOLT_SHAPE, dPrevDeg, dDeg ) ); // 角度エラー
					}
				}
				else
					cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentsAngleOverFlow, sindyErrLevel_WARN, NULL, -1, -1, PULSE_SHAPE_TURN, dDeg, cRule.GetMinDeg(), cRule.GetMaxDeg() ) ); // 角度エラー// bug 8463 #11で警告に変更
				dPrevDeg = dDeg;
			}

			if( /*bForRoadLink &&*/ it2.IsFirstInPart() )
			{
				// [CHECK] 開始・終了のなす角とセグメントの距離チェック
				// かいてね
			}

			// [CHECK] 微小セグメントチェック
			// TODO : 微小セグメントの自動修復の機能が…必要っす。
			if( cRule.GetMinLen() > 0 )
			{
/*				static std::map<IUnknown*, std::map<double, double> > g_mapMeter;	// それぞれの空間参照での距離変換マップ

				double dMinLen = -1.0;	// 空間参照毎のリミット距離
				ISpatialReferencePtr ipSpRef;
				it->get_SpatialReference( &ipSpRef );
				bool bFind = false;
				std::map<IUnknown*, std::map<double, double> >::iterator itmapSpRef = g_mapMeter.find( (IUnknown*)(IUnknownPtr)ipSpRef );
				if( g_mapMeter.end() != itMapSpRef )
				{
					std::map<double, double>::iterator itmapCnv = itmapSpRef.find( cRule.GetMinLen() );
					if( itMapSpRef.end() != itMapCnv )
					{
						dMinLen = itMapCnv.second();
						bFind = true;
					}
				}
				if( ! bFind )
				{

				}*/
				WKSEnvelope env = {0};
				SUCCEEDED_IF( it->QueryWKSEnvelope( &env ), sindyErr_GeometryFunctionFailed, _T("%s"), FAILED_TO_QUERY_WKSENVELOPE )
				{
					if( cRule.GetMinLen() > AheGETDIST( env.XMin, env.YMin, env.XMax, env.YMax ) )
						cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentTooShort, sindyErrLevel_ERR, NULL, -1, -1, SLIVER_SEGMENT, it.index(), AheGETDIST( env.XMin, env.YMin, env.XMax, env.YMax ), cRule.GetMinLen() ) );	// 微小セグメントエラー
				}
			}
		}
		// [CHECK] 隣り合わないセグメント同士の接触判定
		if( cRule.DoCheckOneselfTouches() )
		{
			// 再度使用するため、初期化
			it.Reset(); ++it;
			for( const_segment_iterator itEnd = cPart.segment_end(); it != itEnd; ++it )
			{
				// 自己接触判定は、seg1に隣り合わないseg2が、seg1に接触している場合ににエラーとするものである（自己交差はOK）。
				// 現在のアルゴリズムではセグメント端点が重なっている場合もエラーとなってしまう。
				// AheIsOnselfCrossではOKだが、そのために膨大なコードを書いている。恐らくエラーとなっても問題ないと思われる
				// ので、セグメント同士の距離を測り、0の場合にPolylineに変換してIRelationalOperator::Touchesで判断するように
				// する。
				// 今のセグメントから数えて次の次から
				// ループをまわしてチェック
				IProximityOperatorPtr ipProx( *it );
				IRelationalOperatorPtr ipRelSeg;
				// it2初期化
				HRESULT hr = ((IEnumSegment*)it2)->SetAt( it.part(), it.index() + 2 );
				if ( SUCCEEDED(hr) )
				{
					++it2;
					// インデックス付の時に、LastInPartが正しく動作しない時があるので。
					if ( it.index() + 2 == it2.index() )
					{
						for( ; it2 != cPart.segment_end(); ++it2 )
						{
							double dDistance = -1;
							if( SUCCEEDED( ipProx->ReturnDistance( *it2, &dDistance ) ) )
							{
								if( 0.0 == dDistance ) // 0判定してよい？？
								{
									if( ! ipRelSeg ) // TODO: 円弧を考えなくて良いのであれば、IRelationalOperatorを使わなくてもFROM、TOのポイントからの距離をみるだけでOKでは？？
										ipRelSeg = ToPolyline( *it );
									// 距離が0の場合はPolylineに変換してテスト
									VARIANT_BOOL vbTouches = VARIANT_FALSE;
									if( SUCCEEDED( ipRelSeg->Touches( ToPolyline( *it2 ), &vbTouches ) ) && vbTouches )
									{
										cErrInfos.push_back( CErrorInfo( sindyErr_GeometryOneselfTouches, sindyErrLevel_ERR ) );
									}
								}
							}
						}
					}
				}
			}
		}

		// [CHECK] 最低セグメント数
		if( emType == esriGeometryPolyline ||
			emType == esriGeometryPolygon )
		{
			long lCount = 0;
			ISegmentCollectionPtr ipSegCol( (IGeometry*)*itPart );
			if( ipSegCol )
				ipSegCol->get_SegmentCount( &lCount );

			if( ( emType == esriGeometryPolyline && 1 > lCount ) ||
				( emType == esriGeometryPolygon  && 3 > lCount ) )
				cErrInfos.push_back( CErrorInfo( sindyErr_GeometrySegmentCountIsNotEnough, sindyErrLevel_ERR ) );	// セグメントが足りない
		}
	}
	return cErrInfos.GetDefaultErrCode();
}

// ポイント系チェック
sindyErrCode CGeometry::CheckPoints( const CBasicLogicRule&/* cRule*/, CErrorInfos&/* cErrInfos*/, IEnvelope* /*ipQuery = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// * @li 最低ポイント数チェック（必須）
	// * @li 重複構成点チェック（必須）

	return emErr;
}

} // sindy

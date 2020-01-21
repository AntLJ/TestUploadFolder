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
 * @file
 * @brief <b>CGeometryBaseクラスチェック関数実装ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "GeometryBase.h"
#include "EnumSegment.h"
#include "GeometryRule.h"
#include "ErrorObject2.h"
#include "sindymacroutil.h"
#include "GlobalFunctions.h"
#include "GlobalRules.h"
#include "RuleWrapper.h"
#include "GlobalRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR FAIL_SIMPLIFY[]                       = _T("Simplifyに失敗しました");
const static _TCHAR FAIL_PUT_ISKWNONSIMPLE[]              = _T("put_IsKnownSimpleに失敗しました");
const static _TCHAR NOT_EDITABLE_AREA[]                   = _T("編集可能エリアではありません");
const static _TCHAR THIS_IS_LOCKAREA[]                    = _T("ロックエリアです");
const static _TCHAR  LIGHTNING_SHAPE[]                    = _T("稲妻形状[%.1lf度-%.1lf度（リミット：30度-30度）]");
const static _TCHAR  PULSE_SHARP_ANGLE_TURN[]             = _T("パルス/鋭角ターン[%.1lf度（リミット：%.1lf度～%.1lf度）]");
const static _TCHAR  FAIL_ISEGMENT_QUERYWKS_ENV[]         = _T("ISegment::QueryWKSEnvelopeに失敗");
const static _TCHAR  SMALL_SEGMENT[]                      = _T("微小セグメント[位置：%d 距離：%.3lfm 制限：%.3lfm以上]");
const static _TCHAR  PART_SHAPE_NOT_EXIST[]               = _T("パート形状が存在しない");
const static _TCHAR  GEOMETRYTYPE_POINT_MORE_THAN_POINT[] = _T("[ジオメトリタイプ]ポイント：点数が多い（%ld点）");
const static _TCHAR  GEOMETRYTYPE_POLYLINE_ONLY_POINT[]   = _T("[ジオメトリタイプ]ポリライン [パート]%ld：点数が少ない（%ld点）");
const static _TCHAR  GEOMETRYTYPE_POLYGON_ONLY_POINT[]    = _T("[ジオメトリタイプ]ポリゴン [パート]%ld：点数が少ない（%ld点）");
#else
const static _TCHAR  FAIL_SIMPLIFY[]                      = _T("Simplify has failed");
const static _TCHAR  FAIL_PUT_ISKWNONSIMPLE[]             = _T("put_IsKnownSimple has failed");
const static _TCHAR  NOT_EDITABLE_AREA[]                  = _T("Not an editable area");
const static _TCHAR  THIS_IS_LOCKAREA[]                   = _T("Lock area");
const static _TCHAR  LIGHTNING_SHAPE[]                    = _T("Lightning shape [%.1lf degree - %.1lf degree (limit: 30 degree - 30 degree)]");
const static _TCHAR  PULSE_SHARP_ANGLE_TURN[]             = _T("Pulse/sharp angle turn [%.1lf degree (limit:%.1lf degree %.1");
const static _TCHAR  FAIL_ISEGMENT_QUERYWKS_ENV[]         = _T("ISegment::QueryWKSEnvelope has failed");
const static _TCHAR  SMALL_SEGMENT[]                      = _T("Small segment [position:%d distance:%.3lfm limit:%.3lfm or more]");
const static _TCHAR  PART_SHAPE_NOT_EXIST[]               = _T("Part shape does not exist");
const static _TCHAR  GEOMETRYTYPE_POINT_MORE_THAN_POINT[] = _T("[Geometry type] point: more than one point (%d points)");
const static _TCHAR  GEOMETRYTYPE_POLYLINE_ONLY_POINT[]   = _T("[Geometry type] polyline [part]%ld: only one point (%d points)");
const static _TCHAR  GEOMETRYTYPE_POLYGON_ONLY_POINT[]    = _T("[Geometry type] polygon [part]%ld: two points or less (%d points)");
#endif	// SINDY_FOR_ENGLISH

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

sindyErrCode CGeometryBase::CheckLogic( const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery/* = NULL*/ ) const
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
					cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryPartTooMach, sindyErrLevel_ERR ) ) );

				// [CHECK] ループチェック
				if( cRule.DoCheckRoop() )
				{
					VARIANT_BOOL vbIsClosed = VARIANT_FALSE;
					if( SUCCEEDED( ((IPolylinePtr)(IGeometry*)(*this))->get_IsClosed( &vbIsClosed ) ) && vbIsClosed )
					{
						cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryRooped, sindyErrLevel_ERR ) ) );
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
									cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( emErr, sindyErrLevel_ERR ) ) );
							}
						}
						else
							cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNonSimple, sindyErrLevel_ERR ) ) );

						if( SUCCEEDED( ipTopo->Simplify() ) ) // TODO: 勝手に形状を変えるのはいかがなものか
						{
							// [CHECK] エクステリア数（ポリライン、ポイント）
							if( 1 < GetExteriorRingCount((IGeometry*)*this) ) // <- GetExteriorRingCountはメンバとするか、Ahe系関数とすること
								cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryExteriorTooMach, sindyErrLevel_ERR ) ) );
						}
						else
							cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryFunctionFailed, sindyErrLevel_ERR, NULL, -1, -1, FAIL_SIMPLIFY ) ) );
					}
					else
						cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryFunctionFailed, sindyErrLevel_ERR, NULL, -1, -1, FAIL_PUT_ISKWNONSIMPLE ) ) );
				}
				break;
			}
			default:
				cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNotSupported, sindyErrLevel_ERR ) ) );
				break;
		}
		if( g_cRule && g_cRule->IsLogined() )
		{
			// [CHECK] 編集可能エリアかどうか
			if( ! g_cRule->IsEditableArea( cRule.GetTableName(), IGeometryPtr((IGeometry*)(*this)) ) )
				cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_RuleNotAllowedEditByEditArea, sindyErrLevel_ERR, NULL, -1, -1, NOT_EDITABLE_AREA ) ) );

			// [CHECK] ロックエリアかどうか
			if( g_cRule->IsLockArea( cRule.GetConnectUserName(), cRule.GetTableName(), IGeometryPtr((IGeometry*)(*this)) ) )
				cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_RuleNotAllowedEditByLockArea, sindyErrLevel_ERR, NULL, -1, -1, THIS_IS_LOCKAREA ) ) );
		}
		// セグメント単位でのチェック
		CheckSegments( cRule, cErrInfos, ipQuery );

		// ポイント単位でのチェック
		CheckPoints( cRule, cErrInfos, ipQuery );
	}
	else
		cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNotFound, sindyErrLevel_ERR ) ) );

	return cErrInfos.GetDefaultErrCode();
}

// セグメント系チェック
sindyErrCode CGeometryBase::CheckSegments(const CBasicLogicRule& cRule, CErrorInfosBase& cErrInfos, IEnvelope* ipQuery/* = NULL*/ ) const
{
	// セグメント系チェックはポイント系以外
	esriGeometryType emType = GetGeometryType();
	if( esriGeometryPoint != emType && esriGeometryMultipoint != emType )
	{
		for( const_part_iterator itPart = part_begin(); itPart != part_end(); ++itPart )
		{
			CGeometryBase cPart( *itPart );
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
						if( esriGeometryPolyline == emType )
						{
							// [CHECK] 稲妻チェック（ポリラインのみ）
							if( dPrevDeg <= 30 && dDeg <= 30 )
								cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometrySegmentsAngleOverFlow, sindyErrLevel_WARN, NULL, -1, -1, LIGHTNING_SHAPE, dPrevDeg, dDeg ) ) ); // 角度エラー
						}
					}
					else
						cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometrySegmentsAngleOverFlow, sindyErrLevel_WARN, NULL, -1, -1, PULSE_SHARP_ANGLE_TURN, dDeg, cRule.GetMinDeg(), cRule.GetMaxDeg() ) ) ); // 角度エラー

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
					SUCCEEDED_IF( it->QueryWKSEnvelope( &env ), sindyErr_GeometryFunctionFailed, _T("%s"), FAIL_ISEGMENT_QUERYWKS_ENV )
					{
						if( cRule.GetMinLen() > g_cnv.GetDist( env.XMin, env.YMin, env.XMax, env.YMax ) )
							cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometrySegmentTooShort, sindyErrLevel_ERR, NULL, -1, -1, SMALL_SEGMENT, it.index(), g_cnv.GetDist( env.XMin, env.YMin, env.XMax, env.YMax ), cRule.GetMinLen() ) ) );	// 微小セグメントエラー
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
											cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryOneselfTouches, sindyErrLevel_ERR ) ) );
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
				   cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometrySegmentCountIsNotEnough, sindyErrLevel_ERR ) ) );	// セグメントが足りない
			}
		}
	}
	return cErrInfos.GetDefaultErrCode();
}

// ポイント系チェック
sindyErrCode CGeometryBase::CheckPoints( const CBasicLogicRule&/* cRule*/, CErrorInfosBase& cErrInfos, IEnvelope* /*ipQuery = NULL*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// * @li 最低ポイント数チェック（必須）
	// * @li 重複構成点チェック（必須）

	// [CHECK] NULL形状
	long lPointCount = GetPointCount();
	if( 0 >= lPointCount )
	{
		cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNotFound, sindyErrLevel_ERR ) ) ); // 形状が存在しない
		return sindyErr_GeometryNotFound;
	}

	struct FUNC {
		static void CheckPartPoints( long lPart, const CGeometryBase& cGeom, CErrorInfosBase& cErrInfos )
		{
			long lPointCount = cGeom.GetPointCount();
			if( 0 >= lPointCount )
			{
				cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNotFound, sindyErrLevel_ERR, IGeometryPtr(), -1, -1, _T("%s"), PART_SHAPE_NOT_EXIST ) ) ); // パート形状が存在しない
				return;
			}

			CString strErr;
			switch( cGeom.GetGeometryType() )
			{
			case esriGeometryPoint:
				if( 1 != lPointCount )
					strErr.Format(GEOMETRYTYPE_POINT_MORE_THAN_POINT, lPointCount );
				break;
			case esriGeometryPolyline:
			case esriGeometryPath:
				if( 2 > lPointCount )
					strErr.Format(GEOMETRYTYPE_POLYLINE_ONLY_POINT, lPart, lPointCount );
				break;
			case esriGeometryPolygon:
			case esriGeometryRing:
				if( 4 > lPointCount )
					strErr.Format(GEOMETRYTYPE_POLYGON_ONLY_POINT, lPart, lPointCount );
				break;
			default:
				break; // 他の形状のことはしらない
			}
			if( ! strErr.IsEmpty() )
				cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNonSimple, sindyErrLevel_ERR, cGeom.Clone(), -1, -1, _T("%s"), strErr ) ) );
		}
	};

	switch( GetGeometryType() )
	{
	case esriGeometryPoint:
	case esriGeometryMultipoint:
		FUNC::CheckPartPoints( 0, *this, cErrInfos );
		break;
	default:
		for( const_part_iterator itPart = part_begin(); itPart != part_end(); ++itPart )
		{
			CGeometryBase cPart( *itPart );
			FUNC::CheckPartPoints( itPart.index(), cPart, cErrInfos );
		}
		break;
	}
	return cErrInfos.GetDefaultErrCode();
}

} // sindy

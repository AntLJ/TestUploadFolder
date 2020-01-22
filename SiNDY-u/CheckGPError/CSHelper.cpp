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

#include "stdafx.h"
#include "CSHelper.h"

#include <AddrLib/CSAddrPoint.h>

#include <SiNDYLib/ErrorObject.h>
#include <ArcHelperEx/GlobalFunctions.h>

using namespace addr;

// CSポイント検索
EResultStat CCSHelper::findPoint( CLRec& cRec, bool bAddMode )
{
	CString strWhere;
	strWhere.Format( _T("%s = '%s'"), cs_addr_point::kAddrCode, cRec.m_strAddrCodeNew );

	IQueryFilterPtr ipQuery( AheInitQueryFilter(NULL, NULL, strWhere) );

	long lCSCount = m_cCSPointFC.FeatureCount( ipQuery );
	IFeatureCursorPtr ipFeatureCursor( m_cCSPointFC.Search(ipQuery, VARIANT_FALSE) );
 	if( lCSCount > 0 && ipFeatureCursor )
	{
		// フィーチャ取得
		IFeaturePtr	ipFeature;
		ipFeatureCursor->NextFeature( &ipFeature );

		CCSAddrPoint cCSPoint( ipFeature, sindyTableType::cs_addr_point, false, m_cCSPointFC.GetFieldMap(), m_cCSPointFC.GetNameString() );
		
		// 座標取得・メッシュＸＹに変換
		CMeshXY cMXY;
		gf::ConvPoint2MXY( cCSPoint.GetShapeCopy(), &cMXY );

		if( lCSCount > 1 )
		{
			// CSポイントが複数ある
			cRec.m_cMXY_Ref = cMXY;
			return kW_DUPLI_CS;
		}
		else
		{
			// 情報ソースの取得
			CComVariant	vaValue;
			cCSPoint.GetValueByFieldName(ipc_table::kSource, vaValue );
			cRec.m_strSource = (vaValue.vt == VT_BSTR)? CString(vaValue.bstrVal) : _T("");

			// [Bug 6180]同一16桁の行政界が存在する？(CSポイント下4桁[0000]のとき)
			if( "0000" == cRec.m_strAddrCodeNew.Mid(16, 4) )
			{
				strWhere.Format( _T("citycode='%s' and addrcode='%s' and gaikufugo=%d"), 
								 cRec.m_strAddrCodeNew.Left(5), cRec.m_strAddrCodeNew.Mid(5, 6), _tstol(cRec.m_strAddrCodeNew.Mid(11, 5)) );
				if( m_cCityAdminFC.FeatureCount(AheInitQueryFilter(NULL, NULL, strWhere)) > 0 )
				{
					cRec.m_cMXY_Ref = cMXY;
					return kW_CS_SAME_ADMIN;
				}		
			}

			if( bAddMode )	// 付与モード
			{	
				cRec.m_cMXY = cMXY;
				return kO_CS_ADDED;
			}
			else	// チェックモード
			{
				if( cRec.m_cMXY == cMXY )
				{
					return kO_CS_OK;
				}
				else
				{
					cRec.m_lEditPointID = cCSPoint.GetOID();
					cRec.m_cMXY_Ref = cMXY;
					return kW_CS_MOVED;
				}
			}
		}

	}
	else
		return kO_NOT_FOUND;
}

// CSポイント移動
EResultStat CCSHelper::movePoint( const CLRec& cRec )
{
	CCSAddrPoint cCSPoint( m_cCSPointFC.GetFeature(cRec.m_lEditPointID), sindyTableType::cs_addr_point, false, m_cCSPointFC.GetFieldMap(), m_cCSPointFC.GetNameString() );
	if( cCSPoint )
	{		
		IPointPtr ipPoint( gf::CreateMXYPoint(cRec.m_cMXY) );
		if( ipPoint )
		{
			// 属性値の設定
			cCSPoint.SetValueByFieldName( ipc_table::kOperator, CComVariant(gf::GetCurAccount()) );			// 最終更新者
			cCSPoint.SetValueByFieldName( ipc_table::kPurpose, CComVariant(cRec.m_lPurpose) );				// 作業目的
			cCSPoint.SetValueByFieldName( ipc_table::kUpdateType, CComVariant(3L) );						// 最終更新内容(形状変更)
			cCSPoint.SetValueByFieldName( ipc_table::kProgModifyDate, CComVariant(gf::GetCurDateStr()) );	// 最終プログラム更新日時
			cCSPoint.SetValueByFieldName( ipc_table::kModifyProgName, CComVariant(PROGRAM_NAME) );	// 最終更新プログラム名
			cCSPoint.SetValueByFieldName( ipc_table::kUserClaim, CComVariant(1L) );							// ユーザクレームフラグ
			cCSPoint.SetValueByFieldName( ipc_table::kSource, CComVariant(cRec.m_strClaimNo) );				// 情報ソース
			cCSPoint.SetShape( ipPoint );																	// 形状

			CErrorObjects cErrObjs;
			if( sindyErr_NoErr == cCSPoint.Store( cErrObjs ) )
				return kW_CS_MOVED; // CSポイント移動
		}
	}
	return kE_CS_MOVED_FAIL; // CSポイント移動失敗
}

// CSポイント追加
EResultStat CCSHelper::insertPoint( CLRec& cRec )
{
	// フィーチャの作成
	CRow cRow;
	CCSAddrPoint cCSPoint;
	cCSPoint.SetObject( m_cCSPointFC.CreateRow(cRow), sindyTableType::cs_addr_point, true, m_cCSPointFC.GetFieldMap(), m_cCSPointFC.GetNameString() );
	if( cCSPoint )
	{
		IPointPtr ipPoint( gf::CreateMXYPoint(cRec.m_cMXY) );
		if( ipPoint )
		{
			// 属性値の設定
			cCSPoint.SetValueByFieldName( ipc_table::kOperator, CComVariant(gf::GetCurAccount()) );			// 最終更新者
			cCSPoint.SetValueByFieldName( ipc_table::kPurpose, CComVariant(cRec.m_lPurpose) );				// 作業目的
			cCSPoint.SetValueByFieldName( ipc_table::kUpdateType, CComVariant(3L) );						// 最終更新内容(形状変更)
			cCSPoint.SetValueByFieldName( ipc_table::kProgModifyDate, CComVariant(gf::GetCurDateStr()) );	// 最終プログラム更新日時
			cCSPoint.SetValueByFieldName( ipc_table::kModifyProgName, CComVariant(PROGRAM_NAME) );	// 最終更新プログラム名
			cCSPoint.SetValueByFieldName( ipc_table::kUserClaim, CComVariant(1L) );							// ユーザクレームフラグ
			cCSPoint.SetValueByFieldName( ipc_table::kSource, CComVariant(cRec.m_strClaimNo) );				// 情報ソース
			cCSPoint.SetValueByFieldName( cs_addr_point::kAddrCode, CComVariant(cRec.m_strAddrCodeNew) );	// 住所コード
			cCSPoint.SetShape( ipPoint );																	// 形状

			CErrorObjects cErrObjs;
			if( sindyErr_NoErr == cCSPoint.Store(cErrObjs) )
			{
				cRec.m_lEditPointID = cCSPoint.GetOID();
				return kW_CS_INSERT; // CSポイント追加
			}
		}
	}

	return kE_CS_INSERT_FAIL; // CSポイント追加失敗
}

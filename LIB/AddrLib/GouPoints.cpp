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
#include "GouPoints.h"

#include <ArcHelperEx/GlobalFunctions.h>

namespace addr
{
using namespace sindy;
using namespace errorcode;

// 行政界以下にある号ポイントを取得(住所コード指定Ver)
errorcode::sindyErrCode CGouPoints::SelectByAddrCode( const _TCHAR* lpcszAddrCode )
{
	clear();	// 検索前に中身を一度クリア

	// 都市地図行政界から該当住所コードを持つオブジェクトを取得する
	CString strWhere = GetQueryStr( lpcszAddrCode );
	if( !strWhere.IsEmpty() )
	{
		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		ipQuery->put_WhereClause( _bstr_t(strWhere) );
		return Select(ipQuery);
	}
	else
		return sindyErr_NoErr;
}

// 行政界以下にある号ポイントを取得(Where句指定Ver)
errorcode::sindyErrCode CGouPoints::SelectByWhereClause( const _TCHAR* lpcszWhere )
{
	clear();	// 検索前に中身を一度クリア

	if( lpcszWhere )
	{
		IQueryFilterPtr ipQuery(CLSID_QueryFilter);
		ipQuery->put_WhereClause( _bstr_t(lpcszWhere) );
		return Select(ipQuery);
	}
	else
		return sindyErr_NoErr;
}

///////////////////////////////////////////////
//	private
///////////////////////////////////////////////

// クエリ作成
CString CGouPoints::GetQueryStr( const _TCHAR* lpcszAddrCode )
{
	long lCodeLen = _tcslen( lpcszAddrCode );
	if( lCodeLen >= 5 )
	{
		CString strWhere;
		strWhere.Format( _T("%s = '%s'"), city_admin::kCityCode, CString(lpcszAddrCode).Left(5) );

		switch( lCodeLen )
		{
		case 5:
			break;
		case 8:
			strWhere.AppendFormat( _T(" and %s like '%s%%'"), city_admin::kAddrCode, CString(lpcszAddrCode).Mid(5, 3) );
			break;
		case 11:
			strWhere.AppendFormat( _T(" and %s = '%s'"), city_admin::kAddrCode, CString(lpcszAddrCode).Mid(5, 6) );
			break;
		case 16:
			strWhere.AppendFormat( _T(" and %s = '%s' and %s = %d"), 
								   city_admin::kAddrCode, CString(lpcszAddrCode).Mid(5, 6), city_admin::kGaikuFugo, _tstol(CString(lpcszAddrCode).Mid(11, 5)) );
			break;
		default:
			return _T("");
		}

		return strWhere;
	}
	else
		return _T("");
}

// 形状で検索する
errorcode::sindyErrCode CGouPoints::Select( IQueryFilter* ipFilter, bool bRecycling, bool bCreateCache, CErrorObjects* pErrs, bool bInit )
{
	_ICursorPtr ipCursor( Search(ipFilter, bRecycling) );
	if( ! ipCursor )
		return sindyErr_ArgIsNull;

	_IRowPtr ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) )
	{
		if( ! ipRow )
			break;

		long lOID = -1;
		ipRow->get_OID( &lOID );

		// 同じフィーチャであったら登録はしない
		if( ! bInit )
		{
			const_rows_iterator it = find( sindyTableType::city_admin, lOID );
			if( it != end(sindyTableType::city_admin) )
				continue;
		}

		CSPRow cRow( new CCityAdmin(ipRow, sindyTableType::city_admin, false, GetFieldMap(), GetNameString() ) );

		// キャッシュ作成
		if( bRecycling || bCreateCache )
			cRow->CreateCache( bRecycling );

		// コンテナに格納
		push_back( cRow );
	}

	// 号ポイントが明示的に指定されていなければ、行政界と同じオーナ・ワークスペースの号ポイントを取得
	if( m_cGouFC == NULL )
		m_cGouFC.SetObject( OpenSameOwnerTable(gou_point::kTableName) );

	// 行政界内の号ポイントを取得する
	for( const_rows_iterator itAdmin = begin(GetTableType()), itAdminEnd = end(GetTableType()); itAdmin != itAdminEnd; ++itAdmin )
	{
		// 号ポイント→行政界の紐付け用にもう一つインスタンス用意
		CSPCityAdmin cAdmin( new CCityAdmin(**itAdmin, sindyTableType::city_admin, false, GetFieldMap(), GetNameString()) );
		IFeatureCursorPtr ipFeatureCursor( m_cGouFC.Search(AheInitSpatialFilter(NULL, (*itAdmin)->GetShapeCopy()), false) );
		
		if( ipFeatureCursor )
		{
			IFeaturePtr ipFeature;
			while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
			{
				// 家形と号ポイントもここでリンクする？？要検討

				// 行政界と号ポイントを紐付け
				CSPGouPoint pGou( new CGouPoint(ipFeature, sindyTableType::gou_point, false, m_cGouFC.GetFieldMap(), m_cGouFC.GetNameString()) );
				pGou->SetCityAdmin( cAdmin );	// 号ポイントから見た行政界への紐付けもここで実施(初期状態のものにする)
				(*itAdmin)->push_back( pGou );
			}
		}
	}

	return sindyErr_NoErr;
}

}	// addr

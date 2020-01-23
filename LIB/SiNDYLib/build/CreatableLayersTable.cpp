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
 * @file CreatableLayersTable.cpp
 * @brief <b>CCreatableLayersTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "CreatableLayersTable.h"
#include "ListString.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR LAYER_CANNOT_BE_CREATED = _T("作成不可能なレイヤです：%s");
#else
	LPCTSTR LAYER_CANNOT_BE_CREATED = _T("Layer that cannot be created: %s");
#endif // SINDY_FOR_ENGLISH
}

namespace sindy {
namespace creatablelayers {
using namespace sindy::errorcode;

// 渡されたオブジェクトが属性編集可能かどうかをチェックする
sindyeEditableType CCreatableLayersTable::IsCreatable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	sindyeEditableType emType = sindyeEditableNoDefine;	// 返り値

	const CCreatableLayers* pRule = Find( lPurposeID, lLayerNameID );
	if( pRule != NULL )
		emType = sindyeEditableEditable;	// 作成可能
	else {
		_sntprintf( lpszMessage, nLen, LAYER_CANNOT_BE_CREATED, cRow.GetTableName() );
		emType = sindyeEditableNoEditable;	// 作成可能レイヤではない
	}

	return emType;
}

// 作業目的番号とレイヤ名に対応するレコードをキャッシュから探して返す
const CCreatableLayers* CCreatableLayersTable::Find( long lPurposeID, long lLayerNameID ) const
{
	std::map<long, std::map<long, CCreatableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		std::map<long, CCreatableLayers>::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
		if( itByLayerNameID != itByPurposeID->second.end() )
			return &(itByLayerNameID->second);
	}
	return NULL;
}

// 作業目的番号とレイヤ名に対応するレコードをデータベースから探して返す
_IRowPtr CCreatableLayersTable::GetRow( long lPurposeID, long lLayerNameID ) const
{
	CString strWhere;
	strWhere.Format(_T("%s = %d AND %s = %d"), schema::creatablelayers::kPurposeID, lPurposeID, schema::creatablelayers::kLayerNameID, lLayerNameID );
	return CTable::GetRow( AheInitQueryFilter( NULL, NULL, strWhere ) );
}

// 作業目的番号とレイヤ名に対応するレコードを返す
CCreatableLayers* CCreatableLayersTable::Find( long lPurposeID, long lLayerNameID )
{
	std::map<long, std::map<long, CCreatableLayers> >::iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		std::map<long, CCreatableLayers>::iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
		if( itByLayerNameID != itByPurposeID->second.end() )
			return &(itByLayerNameID->second);
	}
	return NULL;
}

// 作成可能レイヤの一覧を返す
sindyErrCode CCreatableLayersTable::GetCreatableLayers( long lPurposeID, CListString& listLayerName ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	std::map<long, std::map<long, CCreatableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		// LAYERNAMESテーブル取得
		const layernames::CLayerNamesTable& cLayers = g_cRule.GetLayerNamesTable();
		TCHAR strLayerName[64];
		for( std::map<long,CCreatableLayers>::const_iterator it = itByPurposeID->second.begin(); it != itByPurposeID->second.end(); ++it )
		{
			LOGASSERTERR_IF( NULL != cLayers.Find( it->first, 64, strLayerName ), sindyErr_RuleNotFound )
			{
				listLayerName.push_back( strLayerName );
			}
		}
	}
	else
		emErr = sindyErr_RuleNotFound;

	return emErr;
}

// テーブルを全てキャッシュする
long CCreatableLayersTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CCreatableLayersTable::CreateTableCache() : Started.\n") );

	CCreatableLayers cCreatableLayers;
	_ICursorPtr ipCursor;
	if( 0 > lPurposeID )
		ipCursor = Search( NULL, bDetatch );
	else
		ipCursor = Search( AheInitQueryFilter( NULL, _T("*"), _T("PURPOSE_ID=%d"), lPurposeID ), bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// キャッシュ前に共有オブジェクトが新規に作成されてしまうことを防ぐため、
	// 作業目的IDインデックスとレイヤ名IDインデックスを前もって取得
	long lPurposeIDIndex = GetFieldMap()->FindField( schema::creatablelayers::kPurposeID );
	long lLayerNameIDIndex = spFields->FindField( schema::creatablelayers::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cCreatableLayers.Release() ) ) )
	{
		if( cCreatableLayers == NULL ) break;

		// 作業目的名IDとレイヤ名IDをキーにして格納
		long lPurposeID = cCreatableLayers.GetLongValue( lPurposeIDIndex, -1 );
		long lLayerNameID = cCreatableLayers.GetLongValue( lLayerNameIDIndex, -1 );

		m_mapCache[ lPurposeID ][ lLayerNameID ] = cCreatableLayers;
		m_mapCache[ lPurposeID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CCreatableLayersTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();

}

// キャッシュをクリアする
void CCreatableLayersTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

// レイヤを追加する
CCreatableLayers* CCreatableLayersTable::AddLayer( long lPurposeID, long lLayerNameID )
{
	CCreatableLayers* pRow = NULL;
	LOGASSERTE_IF( 0 <= lPurposeID && 0 <= lLayerNameID, sindyErr_ArgLimitOver )
	{
		LOGASSERTE_IF( NULL == Find( lPurposeID, lLayerNameID ), sindyErr_ObjectAlreadyExist )
		{
			CCreatableLayers& cRow = m_mapCache[lPurposeID][lLayerNameID];
			pRow = &cRow;
			if( CreateRow( cRow ) != NULL )	// 作成
			{
				// PurposeIDとLayerNameIDをセット
				LOGASSERTE_IF( sindyErr_NoErr == cRow.SetPurposeID( lPurposeID ) &&
					sindyErr_NoErr == cRow.SetLayerNameID( lLayerNameID ), sindyErr_AOPutValueFailed );

				// キャッシュ作成済みならキャッシュを作成する
				CSPFieldMap spFieldMap = GetFieldMap();
				CSPTableNameString spNameString = GetNameString();
				if( m_bCached ) cRow.CreateCache( false, spFieldMap, spNameString );	// Detatchするかどうかはどうやって決める？？
			}
		}
	}

	return pRow;
}

sindyErrCode CCreatableLayersTable::DeleteLayer( long lPurposeID, long lLayerNameID )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( 0 <= lPurposeID && 0 <= lLayerNameID, sindyErr_ArgLimitOver )
	{
		CCreatableLayers* pRow = NULL;
		LOGASSERTEERR_IF( NULL != ( pRow = Find( lPurposeID, lLayerNameID ) ), sindyErr_ObjectNotExist )
		{
			pRow->Delete();	// 削除
		}
	}
	return emErr;
}

} // creatablelayers
} // sindy

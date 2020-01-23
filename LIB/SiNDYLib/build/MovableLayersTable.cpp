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
 * @file MovableLayersTable.cpp
 * @brief <b>CMovableLayersTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "MovableLayersTable.h"
#include "ListString.h"
#include "LayerNamesTable.h"
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
	LPCTSTR LAYER_CANNOT_BE_MOVED = _T("移動不可能なレイヤです：%s");
#else
	LPCTSTR LAYER_CANNOT_BE_MOVED = _T("Layer that cannot be moved: %s");
#endif	// SINDY_FOR_ENGLISH
}

namespace sindy {
namespace movablelayers {
using namespace sindy::errorcode;

// 渡されたオブジェクトが移動可能かどうかをチェックする
sindyeEditableType CMovableLayersTable::IsMovable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	sindyeEditableType emType = sindyeEditableNoDefine;	// 返り値

	const CMovableLayers* pRule = Find( lPurposeID, lLayerNameID );
	if( pRule != NULL )
		emType = pRule->IsEditable( cRow, nLen, lpszMessage );
	else {
		_sntprintf( lpszMessage, nLen, LAYER_CANNOT_BE_MOVED, cRow.GetTableName() );
		emType = sindyeEditableNoEditable;	// 移動可能レイヤではない
	}

	return emType;
}

// 作業目的番号とレイヤ名に対応するレコードを返す
const CMovableLayers* CMovableLayersTable::Find( long lPurposeID, long lLayerNameID ) const
{
	std::map<long, std::map<long, CMovableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		std::map<long, CMovableLayers>::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
		if( itByLayerNameID != itByPurposeID->second.end() )
			return &(itByLayerNameID->second);
	}
	return NULL;
}

// 移動可能レイヤのリストを返す
errorcode::sindyErrCode CMovableLayersTable::GetMovableLayers( long lPurposeID, CListString& listLayerName ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	std::map<long, std::map<long, CMovableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		// LAYERNAMESテーブル取得
		const layernames::CLayerNamesTable& cLayers = g_cRule.GetLayerNamesTable();
		TCHAR strLayerName[64];
		for( std::map<long,CMovableLayers>::const_iterator it = itByPurposeID->second.begin(); it != itByPurposeID->second.end(); ++it )
		{
			LOGASSERTERR_IF( NULL != cLayers.Find( it->first, 64, strLayerName ), sindyErr_RuleNotFound )
				listLayerName.push_back( strLayerName );
		}	
	}
	else
		emErr = sindyErr_RuleNotFound;

	return emErr;
}

// テーブルを全てキャッシュする
long CMovableLayersTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CMovableLayersTable::CreateTableCache() : Started.\n") );

	CMovableLayers cMovableLayers;
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
	long lPurposeIDIndex = spFields->FindField( schema::movablelayers::kPurposeID );
	long lLayerNameIDIndex = spFields->FindField( schema::movablelayers::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cMovableLayers.Release() ) ) )
	{
		if( cMovableLayers == NULL ) break;

		// 作業目的名IDとレイヤ名IDをキーにして格納
		long lPurposeID = cMovableLayers.GetLongValue( lPurposeIDIndex, -1 );
		long lLayerNameID = cMovableLayers.GetLongValue( lLayerNameIDIndex, -1 );

		m_mapCache[ lPurposeID ][ lLayerNameID ] = cMovableLayers;
		m_mapCache[ lPurposeID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CMovableLayersTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();

}

// キャッシュをクリアする
void CMovableLayersTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // movablelayers

} // sindy

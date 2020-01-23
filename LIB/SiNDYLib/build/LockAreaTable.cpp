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
 * @file LockAreaTable.cpp
 * @brief <b>CLockAreaTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "LockAreaTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace lockarea {

// テーブルを全てキャッシュする
long CLockAreaTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CLockAreaTable::CreateTableCache() : Started.\n") );

	CLockArea cLockArea;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// キャッシュ前に共有オブジェクトが新規に作成されてしまうことを防ぐため、
	// 接続ユーザ名IDインデックスとレイヤ名IDインデックスを前もって取得
	long lConUserIDIndex = spFields->FindField( schema::lockarea::kConnectUserID );
	long lLayerNameIDIndex = spFields->FindField( schema::lockarea::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cLockArea.Release() ) ) )
	{
		if( cLockArea == NULL ) break;

		// 接続ユーザ名IDとレイヤ名IDをキーにして格納
		long lConUserID = cLockArea.GetLongValue( lConUserIDIndex, -1 );
		long lLayerNameID = cLockArea.GetLongValue( lLayerNameIDIndex, -1 );
		m_mapCache[ lConUserID ][ lLayerNameID ] = cLockArea;
		m_mapCache[ lConUserID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CLockAreaTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// キャッシュをクリアする
void CLockAreaTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

// ロックされているかどうかをチェックする
bool CLockAreaTable::IsLocked( long lConnUserID, long lLayerNameID, IGeometry* ipGeom ) const
{
	std::map<long, std::map<long, CLockArea > >::const_iterator itByConUser = m_mapCache.find( lConnUserID );
	if( itByConUser != m_mapCache.end() )
	{
		std::map<long, CLockArea>::const_iterator itByLayerNameID = itByConUser->second.find( lLayerNameID );
		if( itByLayerNameID != itByConUser->second.end() )
		{
			// [Bug 10510]ロックメッシュの仕様を変更してほしい
			// 離れていない且つ接触でもない（交差、包含、内包、重複）
			if( ! itByLayerNameID->second.GetShape()->Disjoint( ipGeom ) && ! itByLayerNameID->second.GetShape()->Touches( ipGeom ) )
				return true;
		}
	}
	return false;
}

} // lockarea

} // sindy

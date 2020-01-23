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

/** * @file LayerNamesTable.cpp
 * @brief <b>CLayerNamesTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "LayerNamesTable.h"
#include "util.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace layernames {
using namespace errorcode;

// レイヤ名IDからレイヤ名を取得する
LPCTSTR CLayerNamesTable::Find( long lLayerNameID, int nLen, LPTSTR lpszName ) const
{
	// 初期化
	if( nLen > 0 && lpszName != NULL ) lpszName[0] = _T('\0');

	// 引数チェック
	if( lLayerNameID != NULL && nLen > 0 && lpszName != NULL )
	{
		for( std::list<CLayerNames>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
		{
			if( it->GetLayerNameID() == lLayerNameID )
			{
				it->GetLayerName( nLen, lpszName );
				return lpszName;
			}
		}
	}
	else {
		ERRORLOG(sindyErr_ArgIsNull);	// 引数がNULL
		SASSERT( lLayerNameID != NULL && nLen > 0 && lpszName != NULL );
	}

	return NULL;
}

// レイヤ名からレイヤ名IDを取得する
long CLayerNamesTable::Find( LPCTSTR lpcszLayerName ) const
{
	long lID = -1;	// 返り値

	// 引数チェック
	if( lpcszLayerName != NULL )
	{
		LPTSTR lpszName = new TCHAR[64];	// 64文字以上のテーブル名は無いと仮定
		for( std::list<CLayerNames>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
		{
			if( lstrcmpi( lpcszLayerName, it->GetLayerName( 64, lpszName ) ) == 0 )
			{
				lID = it->GetLayerNameID();
				break;
			}
		}
		delete [] lpszName;
	}
	else {
		ERRORLOG(sindyErr_ArgIsNull); // 引数がNULL
		SASSERT( lpcszLayerName != NULL );
	}

	return lID;
}

// テーブルを全てキャッシュする
long CLayerNamesTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CLayerNamesTable::CreateTableCache() : Started.\n") );

	CLayerNames cLayerNames;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cLayerNames.Release() ) ) )
	{
		if( cLayerNames == NULL ) break;

		m_listCache.push_front( cLayerNames );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CLayerNamesTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();

}

// キャッシュをクリアする
void CLayerNamesTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

} // layernames

} // sindy

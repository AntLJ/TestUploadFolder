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

// ConnectUserTable.cpp: CConnectUserTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConnectUserTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace connectuser {

// キャッシュをクリアする
void CConnectUserTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

// テーブルを全てキャッシュする
long CConnectUserTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CConnectUserTable::CreateTableCache() : Started.\n") );

	CConnectUser cConnectUser;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cConnectUser.Release() ) ) )
	{
		if( cConnectUser == NULL ) break;

		m_listCache.push_front( cConnectUser );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CConnectUserTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();

}

// 接続ユーザ名に対応するIDを返す
long CConnectUserTable::GetConnectUserID( LPCTSTR lpcszConnUser ) const
{
	long lID = -1;	// 返り値

	LPTSTR lpszName = new TCHAR[64];
	for( std::list<CConnectUser>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
	{
		if( lstrcmpi( lpcszConnUser, it->GetName( 64, lpszName ) ) == 0 )
			lID = it->GetConnectUserID();
	}
	delete [] lpszName;

	return lID;
}

} // connectuser 

} // sindy

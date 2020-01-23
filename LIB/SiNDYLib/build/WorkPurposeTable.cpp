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
 * @file WorkPurposeTable.cpp
 * @brief <b>CWorkPurposeTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "WorkPurposeTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace workpurpose {

// 作業目的番号に対応するレコードを返す
const CWorkPurpose* CWorkPurposeTable::Find( long lID ) const
{
	// キャッシュに存在するかどうかチェック
	std::map<long,CWorkPurpose>::const_iterator it = m_mapCache.find( lID );
	if( it != m_mapCache.end() )
		return &(it->second);
	else {
		if( ! m_bCached )
		{
			_ASSERT( false );	// 未実装
//			Search(
		}
	}

	return NULL;
}

// テーブルを全てキャッシュする
long CWorkPurposeTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CWorkPurposeTable::CreateTableCache() : Started.\n") );

	CWorkPurpose cWorkPurpose;
	_ICursorPtr ipCursor;
	if( 0 > lPurposeID )
		ipCursor = Search( NULL, bDetatch );
	else
		ipCursor = Search( AheInitQueryFilter( NULL, _T("*"), _T("PURPOSE_ID=%d"), lPurposeID ), bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// キャッシュ前に共有オブジェクトが新規に作成されてしまうことを防ぐため、
	// 作業目的IDインデックスを前もって取得
	long lPurposeIDIndex = spFields->FindField( schema::workpurpose::kPurposeID );

	while( SUCCEEDED( ipCursor->NextRow( cWorkPurpose.Release() ) ) )
	{
		if( cWorkPurpose == NULL ) break;

		// 作業目的IDをキーにして格納する
		long lPurposeID = cWorkPurpose.GetLongValue( lPurposeIDIndex, -1 );

		m_mapCache[ lPurposeID ] = cWorkPurpose;
		m_mapCache[ lPurposeID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CWorkPurposeTable::CreateTableCache() : Finisheded.\n") );

	return m_mapCache.size();
}

//キャッシュをクリアする
void CWorkPurposeTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // workpurpose

} // sindy

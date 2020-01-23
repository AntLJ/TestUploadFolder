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
 * @file RoadNaviClassTable.cpp
 * @brief <b>CRoadNaviClassTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "RoadNaviClassTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace roadnaviclass {

// テーブルを全てキャッシュする
long CRoadNaviClassTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CRoadNaviClassTable::CreateTableCache() : Started.\n") );

	CRoadNaviClass cRoadNaviClass;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// キャッシュ前に共有オブジェクトが新規に作成されてしまうことを防ぐため、
	// 経路種別名称インデックスを前もって取得
	long lNaviClassIndex = spFields->FindField( schema::roadnaviclass::kName );

	LPTSTR lpszNaviClass = new TCHAR[64];
	while( SUCCEEDED( ipCursor->NextRow( cRoadNaviClass.Release() ) ) )
	{
		if( cRoadNaviClass == NULL ) break;

		// 道路リンクの経路種別名称をキーにして格納
		cRoadNaviClass.GetStringValue( lNaviClassIndex, 64, lpszNaviClass );
		m_mapCache[ lpszNaviClass ] = cRoadNaviClass;
		m_mapCache[ lpszNaviClass ].CreateCache( bDetatch, spFields, spNameString );
	}
	delete [] lpszNaviClass;
	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CRoadNaviClassTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// キャッシュをクリアする
void CRoadNaviClassTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // roadnaviclass

} // sindy

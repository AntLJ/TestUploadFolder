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
 * @file DispNaviRelationTable.cpp
 * @brief <b>CDispNaviRelationTableクラス実装ファイル</b>
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "DispNaviRelationTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace dispnavirelation {

// 道路リンクの道路種別名IDと道路リンクのその他の属性のIDに対応するレコードを返す
const CDispNaviRelation* CDispNaviRelationTable::Find( long lDispClassID, long lNaviClassID ) const
{
	std::map<long, std::map<long, CDispNaviRelation> >::const_iterator itByDispClassID = m_mapCache.find( lDispClassID );
	if( itByDispClassID != m_mapCache.end() )
	{
		std::map<long, CDispNaviRelation>::const_iterator itByNaviClassID = itByDispClassID->second.find( lNaviClassID );
		if( itByNaviClassID != itByDispClassID->second.end() )
			return &(itByNaviClassID->second);
	}
	return NULL;
}

// テーブルを全てキャッシュする
long CDispNaviRelationTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CDispNaviRelationTable::CreateTableCache() : Started.\n") );

	CDispNaviRelation cDispNaviRelation;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// キャッシュ前に共有オブジェクトが新規に作成されてしまうことを防ぐため、
	// 道路リンクの道路種別名IDインデックスと道路リンクの経路種別名IDインデックスを前もって取得
	long lDispClassIDIndex = spFields->FindField( schema::dispnavirelation::kDispClassID );
	long lNaviClassIDIndex = spFields->FindField( schema::dispnavirelation::kNaviClassID );

	while( SUCCEEDED( ipCursor->NextRow( cDispNaviRelation.Release() ) ) )
	{
		if( cDispNaviRelation == NULL ) break;

		// 道路リンクの道路種別名IDと道路リンクの経路種別名IDをキーにして格納する
		long lDispClassID = cDispNaviRelation.GetLongValue( lDispClassIDIndex, -1 );
		long lNaviClassID = cDispNaviRelation.GetLongValue( lNaviClassIDIndex, -1 );
		m_mapCache[ lDispClassID ][ lNaviClassID ] = cDispNaviRelation;
		m_mapCache[ lDispClassID ][ lNaviClassID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CDispNaviRelationTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// キャッシュをクリアする
void CDispNaviRelationTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // dispotherrelation

} // sindy

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
 * @file DispOtherRelationTable.cpp
 * @brief  <b>CDispOtherRelationTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "DispOtherRelationTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace dispotherrelation {

// 道路リンクの道路種別名IDと道路リンクのその他の属性のIDに対応するレコードを返す
const CDispOtherRelation* CDispOtherRelationTable::Find( long lDispClassID, long lOtherAttrID ) const
{
	std::map<long, std::map<long, CDispOtherRelation> >::const_iterator itByDispClassID = m_mapCache.find( lDispClassID );
	if( itByDispClassID != m_mapCache.end() )
	{
		std::map<long, CDispOtherRelation>::const_iterator itByOtherAttrID = itByDispClassID->second.find( lOtherAttrID );
		if( itByOtherAttrID != itByDispClassID->second.end() )
			return &(itByOtherAttrID->second);
	}
	return NULL;
}

// テーブルを全てキャッシュする
long CDispOtherRelationTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CDispOtherRelationTable::CreateTableCache() : Started.\n") );

	CDispOtherRelation cDispOtherRelation;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// キャッシュ前に共有オブジェクトが新規に作成されてしまうことを防ぐため、
	// 道路リンクの道路種別名IDインデックスと道路リンクのその他の属性のIDインデックスを前もって取得
	long lDispClassIDIndex = spFields->FindField( schema::dispotherrelation::kDispClassID );
	long lOtherAttrIDIndex = spFields->FindField( schema::dispotherrelation::kOtherAttrID );

	while( SUCCEEDED( ipCursor->NextRow( cDispOtherRelation.Release() ) ) )
	{
		if( cDispOtherRelation == NULL ) break;

		// 道路リンクの道路種別名IDと道路リンクのその他の属性のIDをキーにして格納する
		long lDispClassID = cDispOtherRelation.GetLongValue( lDispClassIDIndex, -1 );
		long lOtherAttrID = cDispOtherRelation.GetLongValue( lOtherAttrIDIndex, -1 );
		m_mapCache[ lDispClassID ][ lOtherAttrID ] = cDispOtherRelation;
		m_mapCache[ lDispClassID ][ lOtherAttrID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CDispOtherRelationTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// キャッシュをクリアする
void CDispOtherRelationTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // dispotherrelation

} // sindy

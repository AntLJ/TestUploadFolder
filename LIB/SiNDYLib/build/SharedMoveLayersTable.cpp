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
 * @file SharedMoveLayersTable.cpp
 * @brief <b>CSharedMoveLayersTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "SharedMoveLayersTable.h"
#include "util.h"
#include "GlobalFunctions.h"
#include "ListLong.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace sharedmovelayers {
using namespace errorcode;

// テーブルを全てキャッシュする
long CSharedMoveLayersTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CSharedMoveLayersTable::CreateTableCache() : Started.\n") );

	CSharedMoveLayers cSharedMoveLayers;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cSharedMoveLayers.Release() ) ) )
	{
		if( cSharedMoveLayers == NULL ) break;

		m_listCache.push_front( cSharedMoveLayers );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CSharedMoveLayersTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// キャッシュをクリアする
void CSharedMoveLayersTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

// 渡されたレイヤ名IDに対応する共有移動対象レイヤ名IDを返す
long CSharedMoveLayersTable::GetSharedMoveLayers( long lLayerNameID, CListLong& listLayerNameID ) const
{
	long lCount = 0;
	LOGASSERTE_IF( lLayerNameID >= 0, sindyErr_ArgLimitOver )
	{
		long lSetID = -1;
		for( std::list<CSharedMoveLayers>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
		{
			if( lLayerNameID == it->GetLayerNameID() )
				lSetID = it->GetSetID();
		}

		// 見つかった場合
		if( lSetID >= 0 )
		{
			for( std::list<CSharedMoveLayers>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
			{
				if( lSetID == it->GetSetID() )
				{
					listLayerNameID.push_back( it->GetLayerNameID() );
					++lCount;
				}
			}
		}
		// 見つからなかった場合は渡されたものを追加しておく
		else {
			listLayerNameID.push_back( lLayerNameID );
			++lCount;
		}
	}

	return lCount;
}

} // sharedmovelayers

} // sindy

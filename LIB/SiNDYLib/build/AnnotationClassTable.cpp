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
 * @file AnnotationClassTable.cpp
 * @brief<b>CAnnotationClassTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "AnnotationClassTable.h"
#include "GlobalFunctions.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace annotation_class {
using namespace errorcode;

double g_dGetRowByAnnoCodeTime = 0.0;	//!< GetRowByAnnoCodeにかかる総時間
// 注記種別で検索を行う
const CAnnotationClass* CAnnotationClassTable::GetRowByAnnoCode( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const
{
	double dStart = GetTickCount();

	const CAnnotationClass* pAnnoClass = NULL;	// 返り値

	// キャッシュに存在するかどうかチェック
	if( m_bCached )
	{
		std::list<CAnnotationClass>::const_iterator it = m_listCache.end();
		for( it = m_listCache.begin(); it != m_listCache.end(); ++it )
		{
			if( it->GetClassCode() == emCode )
			{
				bool bFind = false;
				switch( emTableType )
				{
					case sindyTableType::city_annotation:	bFind = it->GetCityFlag();   break;
					case sindyTableType::d1_annotation:		bFind = true;   break;
					case sindyTableType::base_annotation:	bFind = it->GetBaseFlag();   break;
					case sindyTableType::middle_annotation: bFind = it->GetMiddleFlag(); break;
					case sindyTableType::top_annotation:	bFind = it->GetTopFlag();    break;
					default: 
						ERRORLOG(sindyErr_UnknownTableType);
						SASSERT( false );
						break;
				}
				if( bFind )
				{
					pAnnoClass = &(*it);
					break;
				}
			}
		}	
	}
	// キャッシュに無ければ検索
	else {
		_ASSERT( false );	// 未実装
//		Search(
	}

	g_dGetRowByAnnoCodeTime += (double)GetTickCount()-dStart; // 総時間を計算

	return pAnnoClass;
}

// テーブルを全てキャッシュする
long CAnnotationClassTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CAnnotationClassTable::CreateTableCache() : Started.\n") );

	CAnnotationClass cAnnoClass;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// 共有オブジェクト作成
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cAnnoClass.Release() ) ) )
	{
		if( cAnnoClass == NULL ) break;

		m_listCache.push_front( cAnnoClass );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// 切り離し
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CAnnotationClassTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// キャッシュをクリアする
void CAnnotationClassTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

} // annotation_class

} // sindy

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
 * @file AbleLayers.cpp
 * @brief <b>CAbleLayersTableクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "AbleLayers.h"
#include "CacheAttribute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// 属性のキャッシュを作成する
sindyErrCode CAbleLayers::CreateCache( bool bDetatch/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	errorcode::sindyErrCode emErr = CRow::CreateCache( bDetatch, spFields, spNameString );

	// 属性制限のキャッシュ
	m_cRistrict.clear();
	LPTSTR lpszStr = new TCHAR[1025];
	m_cRistrict.AddRistrict( GetRistrict1( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict2( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict3( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict4( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict5( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict6( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict7( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict8( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict9( 1024, lpszStr ) );
	m_cRistrict.AddRistrict( GetRistrict10( 1024, lpszStr ) );
	delete [] lpszStr;

	_ASSERTE( ! p );

	return emErr;
}

// 渡されたオブジェクトが編集可能かどうかチェックする
sindyeEditableType CAbleLayers::IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	return m_cRistrict.IsEditable( cRow, nLen, lpszMessage );
}

// キャッシュを削除する
void CAbleLayers::DeleteCache( bool bDeleteContainerCache/* = false*/ )
{
	CRow::DeleteCache();
	m_cRistrict.clear();
}

} // sindy

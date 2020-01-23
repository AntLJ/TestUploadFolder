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
 * @file ListLong.cpp
 * @brief CListLongクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "ListLong.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListLong::CListLong()
{
}

CListLong::~CListLong()
{
}

void CListLong::push_back( LPCTSTR lpcszStr, LPCTSTR lpcszSep )
{
	CString strTarget( lpcszStr );

	INT iPos = -1;

	while( 1 )
	{
		iPos = strTarget.Find( lpcszSep );
		if( iPos > 0 )
			push_back( _ttol(strTarget.Left( iPos )) );
		else if( iPos < 0 )
			break;

		strTarget = strTarget.Right( strTarget.GetLength() - ( iPos + 1 ) - ( lstrlen( lpcszSep ) - 1 ) );
	}

	if( ! strTarget.IsEmpty() )
		push_back( _ttol(strTarget) );
}

void CListLong::push_back( long l )
{
	m_listLong.push_back( l );
}

void CListLong::push_back( const CListLong& ids )
{
	for( CListLong::const_iterator it = ids.begin(); it != ids.end(); ++it )
		push_back( *it );
}

void CListLong::push_back( const std::list<long>& ids )
{
	for( std::list<long>::const_iterator it = ids.begin(); it != ids.end(); ++it )
		push_back( *it );
}

void CListLong::clear()
{
	m_listLong.clear();
}

void CListLong::unique()
{
	m_listLong.unique();
}

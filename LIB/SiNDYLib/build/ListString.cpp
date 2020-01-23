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
 * @file ListString.cpp
 * @brief CListString�N���X�����t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "ListString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListString::CListString(){}

CListString::~CListString(){ m_listStr.clear(); }

void CListString::push_back( LPCTSTR lpcszStr, LPCTSTR lpcszSep )
{
	CString strTarget( lpcszStr );

	INT iPos = -1;

	while( 1 )
	{
		iPos = strTarget.Find( lpcszSep );
		if( iPos > 0 )
			push_back( strTarget.Left( iPos ) );
		else if( iPos < 0 )
			break;

		strTarget = strTarget.Right( strTarget.GetLength() - ( iPos + 1 ) - ( lstrlen( lpcszSep ) - 1 ) );
	}

	if( ! strTarget.IsEmpty() )
		push_back( strTarget );
}

void CListString::push_back( LPCTSTR lpcszStr )
{
	m_listStr.push_back( lpcszStr );
}

void CListString::push_back( const CListString& strs )
{
	for( CListString::const_iterator it = strs.begin(); it != strs.end(); ++it )
		push_back( *it );
}

void CListString::push_back( const std::list<CString>& strs )
{
	for( std::list<CString>::const_iterator it = strs.begin(); it != strs.end(); ++it )
		push_back( *it );
}

void CListString::clear()
{
	m_listStr.clear();
}

void CListString::unique()
{
	m_listStr.unique();
}

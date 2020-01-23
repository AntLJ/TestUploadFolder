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
 * @file EditHistoryTable.cpp
 * @brief <b>CEditHistoryTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "EditHistoryTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace edithistory {

// �e�[�u����S�ăL���b�V������
long CEditHistoryTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CEditHistoryTable::CreateTableCache() : Started.\n") );

	CEditHistory cEditHistory;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cEditHistory.Release() ) ) )
	{
		if( cEditHistory == NULL ) break;

		m_listCache.push_front( cEditHistory );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CEditHistoryTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// �L���b�V�����N���A����
void CEditHistoryTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

} // edithistory

} // sindy

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
 * @file OperatorTable.cpp
 * @brief <b>COperatorTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "OperatorTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace soperator {

// �e�[�u����S�ăL���b�V������
long COperatorTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("COperatorTable::CreateTableCache() : Started.\n") );

	COperator cOperator;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cOperator.Release() ) ) )
	{
		if( cOperator == NULL ) break;

		m_listCache.push_front( cOperator );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("COperatorTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// �L���b�V�����N���A����
void COperatorTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

// ��ƎҖ������ƎҖ�ID���擾����
long COperatorTable::GetOperatorID( LPCTSTR lpcszOperatorName ) const
{
	long lID = -1;	// �Ԃ�l
	LPTSTR lpszName = new TCHAR[64];	// 64�����ȏ�̍�ƎҖ��͖����Ƃ���

	for( std::list<COperator>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
	{
		if( lstrcmpi( lpcszOperatorName, it->GetName( 64, lpszName ) ) == 0 )
		{
			lID = it->GetOperatorID();
			break;
		}
	}
	delete [] lpszName;

	return lID;
}

} // soperator

} // sindy

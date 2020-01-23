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
 * @file RoadAttributeTable.cpp
 * @brief <b>CRoadAttributeTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "RoadAttributeTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace roadattribute {

// �e�[�u����S�ăL���b�V������
long CRoadAttributeTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CRoadAttributeTable::CreateTableCache() : Started.\n") );

	CRoadAttribute cRoadAttribute;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cRoadAttribute.Release() ) ) )
	{
		if( cRoadAttribute == NULL ) break;

		m_listCache.push_front( cRoadAttribute );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CRoadAttributeTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// �L���b�V�����N���A����
void CRoadAttributeTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

// �t�B�[���h���ɑΉ�����ID��Ԃ�
long CRoadAttributeTable::GetFieldNameID( LPCTSTR lpcszFieldName ) const
{
	long lID = -1;	// �Ԃ�l
	LPTSTR lpszName = new TCHAR[64]; // 64�����ȏ�͖����Ɖ���

	for( std::list<CRoadAttribute>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
	{
		if( lstrcmpi( it->GetName( 64, lpszName ), lpcszFieldName ) == 0 )
		{
			lID = it->GetNameID();
			break;
		}
	}
	delete [] lpszName;

	return lID;
}

} // roadattribute

} // sindy

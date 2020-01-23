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
 * @file WorkPurposeTable.cpp
 * @brief <b>CWorkPurposeTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "WorkPurposeTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace workpurpose {

// ��ƖړI�ԍ��ɑΉ����郌�R�[�h��Ԃ�
const CWorkPurpose* CWorkPurposeTable::Find( long lID ) const
{
	// �L���b�V���ɑ��݂��邩�ǂ����`�F�b�N
	std::map<long,CWorkPurpose>::const_iterator it = m_mapCache.find( lID );
	if( it != m_mapCache.end() )
		return &(it->second);
	else {
		if( ! m_bCached )
		{
			_ASSERT( false );	// ������
//			Search(
		}
	}

	return NULL;
}

// �e�[�u����S�ăL���b�V������
long CWorkPurposeTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CWorkPurposeTable::CreateTableCache() : Started.\n") );

	CWorkPurpose cWorkPurpose;
	_ICursorPtr ipCursor;
	if( 0 > lPurposeID )
		ipCursor = Search( NULL, bDetatch );
	else
		ipCursor = Search( AheInitQueryFilter( NULL, _T("*"), _T("PURPOSE_ID=%d"), lPurposeID ), bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// ��ƖړIID�C���f�b�N�X��O�����Ď擾
	long lPurposeIDIndex = spFields->FindField( schema::workpurpose::kPurposeID );

	while( SUCCEEDED( ipCursor->NextRow( cWorkPurpose.Release() ) ) )
	{
		if( cWorkPurpose == NULL ) break;

		// ��ƖړIID���L�[�ɂ��Ċi�[����
		long lPurposeID = cWorkPurpose.GetLongValue( lPurposeIDIndex, -1 );

		m_mapCache[ lPurposeID ] = cWorkPurpose;
		m_mapCache[ lPurposeID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CWorkPurposeTable::CreateTableCache() : Finisheded.\n") );

	return m_mapCache.size();
}

//�L���b�V�����N���A����
void CWorkPurposeTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // workpurpose

} // sindy

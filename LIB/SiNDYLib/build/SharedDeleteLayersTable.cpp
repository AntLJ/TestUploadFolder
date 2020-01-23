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
 * @file SharedDeleteLayersTable.cpp
 * @brief <b>CSharedDeleteLayersTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "SharedDeleteLayersTable.h"
#include "util.h"
#include "GlobalFunctions.h"
#include "ListLong.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace shareddeletelayers {
using namespace errorcode;

// �e�[�u����S�ăL���b�V������
long CSharedDeleteLayersTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CSharedDeleteLayersTable::CreateTableCache() : Started.\n") );

	CSharedDeleteLayers cSharedDeleteLayers;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	while( SUCCEEDED( ipCursor->NextRow( cSharedDeleteLayers.Release() ) ) )
	{
		if( cSharedDeleteLayers == NULL ) break;

		m_listCache.push_front( cSharedDeleteLayers );
		m_listCache.begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CSharedDeleteLayersTable::CreateTableCache() : Finished.\n") );

	return m_listCache.size();
}

// �L���b�V�����N���A����
void CSharedDeleteLayersTable::ClearTableCache()
{
	m_listCache.clear();
	m_bCached = false;
}

// �n���ꂽ���C����ID�ɑΉ����鋤�L�폜�Ώۃ��C����ID��Ԃ�
long CSharedDeleteLayersTable::GetSharedDeleteLayers( long lLayerNameID, CListLong& listLayerNameID ) const
{
	long lCount = 0;
	LOGASSERTE_IF( lLayerNameID >= 0, sindyErr_ArgLimitOver )
	{
		long lSetID = -1;
		for( std::list<CSharedDeleteLayers>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
		{
			if( lLayerNameID == it->GetLayerNameID() )
				lSetID = it->GetSetID();
		}

		// ���������ꍇ
		if( lSetID >= 0 )
		{
			for( std::list<CSharedDeleteLayers>::const_iterator it = m_listCache.begin(); it != m_listCache.end(); ++it )
			{
				if( lSetID == it->GetSetID() )
				{
					listLayerNameID.push_back( it->GetLayerNameID() );
					++lCount;
				}
			}
		}
		// ������Ȃ������ꍇ�͓n���ꂽ���̂�ǉ����Ă���
		else {
			listLayerNameID.push_back( lLayerNameID );
			++lCount;
		}
	}

	return lCount;
}

} // sharedmovelayers

} // sindy

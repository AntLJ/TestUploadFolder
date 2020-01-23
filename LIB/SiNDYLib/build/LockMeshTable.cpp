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
 * @file LockMeshTable.cpp
 * @brief<b>CLockMeshTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "LockMeshTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace lockmesh {

// �e�[�u����S�ăL���b�V������
long CLockMeshTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CLockMeshTable::CreateTableCache() : Started.\n") );

	CLockMesh cLockMesh;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// �ڑ����[�U��ID�C���f�b�N�X�ƃ��C����ID�C���f�b�N�X��O�����Ď擾
	long lConUserIDIndex = spFields->FindField( schema::lockmesh::kConnectUserID );
	long lLayerNameIDIndex = spFields->FindField( schema::lockmesh::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cLockMesh.Release() ) ) )
	{
		if( cLockMesh == NULL ) break;

		// �ڑ����[�U��ID�ƃ��C����ID���L�[�ɂ��Ċi�[
		long lConUserID = cLockMesh.GetLongValue( lConUserIDIndex, -1 );
		long lLayerNameID = cLockMesh.GetLongValue( lLayerNameIDIndex, -1 );
		m_mapCache[ lConUserID ][ lLayerNameID ].push_front( cLockMesh );
		m_mapCache[ lConUserID ][ lLayerNameID ].begin()->CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CLockMeshTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CLockMeshTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

// ���b�N����Ă��郁�b�V�����ǂ����`�F�b�N����
bool CLockMeshTable::IsLocked( long lConnUserID, long lLayerNameID, long lCode ) const
{
	std::map<long, std::map<long, std::list<CLockMesh> > >::const_iterator itByConUser = m_mapCache.find( lConnUserID );
	if( itByConUser != m_mapCache.end() )
	{
		std::map<long, std::list<CLockMesh> >::const_iterator itByLayerNameID = itByConUser->second.find( lLayerNameID );
		if( itByLayerNameID != itByConUser->second.end() )
		{
			for( std::list<CLockMesh>::const_iterator it = itByLayerNameID->second.begin(); it != itByLayerNameID->second.end(); ++it )
			{
				if( lCode == it->GetMeshCode() )
					return true;
			}
		}
	}
	return false;
}

} // lockmesh

} // sindy

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
 * @file LockAreaTable.cpp
 * @brief <b>CLockAreaTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "LockAreaTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace lockarea {

// �e�[�u����S�ăL���b�V������
long CLockAreaTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CLockAreaTable::CreateTableCache() : Started.\n") );

	CLockArea cLockArea;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// �ڑ����[�U��ID�C���f�b�N�X�ƃ��C����ID�C���f�b�N�X��O�����Ď擾
	long lConUserIDIndex = spFields->FindField( schema::lockarea::kConnectUserID );
	long lLayerNameIDIndex = spFields->FindField( schema::lockarea::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cLockArea.Release() ) ) )
	{
		if( cLockArea == NULL ) break;

		// �ڑ����[�U��ID�ƃ��C����ID���L�[�ɂ��Ċi�[
		long lConUserID = cLockArea.GetLongValue( lConUserIDIndex, -1 );
		long lLayerNameID = cLockArea.GetLongValue( lLayerNameIDIndex, -1 );
		m_mapCache[ lConUserID ][ lLayerNameID ] = cLockArea;
		m_mapCache[ lConUserID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CLockAreaTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CLockAreaTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

// ���b�N����Ă��邩�ǂ������`�F�b�N����
bool CLockAreaTable::IsLocked( long lConnUserID, long lLayerNameID, IGeometry* ipGeom ) const
{
	std::map<long, std::map<long, CLockArea > >::const_iterator itByConUser = m_mapCache.find( lConnUserID );
	if( itByConUser != m_mapCache.end() )
	{
		std::map<long, CLockArea>::const_iterator itByLayerNameID = itByConUser->second.find( lLayerNameID );
		if( itByLayerNameID != itByConUser->second.end() )
		{
			// [Bug 10510]���b�N���b�V���̎d�l��ύX���Ăق���
			// ����Ă��Ȃ����ڐG�ł��Ȃ��i�����A��܁A����A�d���j
			if( ! itByLayerNameID->second.GetShape()->Disjoint( ipGeom ) && ! itByLayerNameID->second.GetShape()->Touches( ipGeom ) )
				return true;
		}
	}
	return false;
}

} // lockarea

} // sindy

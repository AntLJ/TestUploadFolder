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
 * @file DispNaviRelationTable.cpp
 * @brief <b>CDispNaviRelationTable�N���X�����t�@�C��</b>
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "DispNaviRelationTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace dispnavirelation {

// ���H�����N�̓��H��ʖ�ID�Ɠ��H�����N�̂��̑��̑�����ID�ɑΉ����郌�R�[�h��Ԃ�
const CDispNaviRelation* CDispNaviRelationTable::Find( long lDispClassID, long lNaviClassID ) const
{
	std::map<long, std::map<long, CDispNaviRelation> >::const_iterator itByDispClassID = m_mapCache.find( lDispClassID );
	if( itByDispClassID != m_mapCache.end() )
	{
		std::map<long, CDispNaviRelation>::const_iterator itByNaviClassID = itByDispClassID->second.find( lNaviClassID );
		if( itByNaviClassID != itByDispClassID->second.end() )
			return &(itByNaviClassID->second);
	}
	return NULL;
}

// �e�[�u����S�ăL���b�V������
long CDispNaviRelationTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CDispNaviRelationTable::CreateTableCache() : Started.\n") );

	CDispNaviRelation cDispNaviRelation;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// ���H�����N�̓��H��ʖ�ID�C���f�b�N�X�Ɠ��H�����N�̌o�H��ʖ�ID�C���f�b�N�X��O�����Ď擾
	long lDispClassIDIndex = spFields->FindField( schema::dispnavirelation::kDispClassID );
	long lNaviClassIDIndex = spFields->FindField( schema::dispnavirelation::kNaviClassID );

	while( SUCCEEDED( ipCursor->NextRow( cDispNaviRelation.Release() ) ) )
	{
		if( cDispNaviRelation == NULL ) break;

		// ���H�����N�̓��H��ʖ�ID�Ɠ��H�����N�̌o�H��ʖ�ID���L�[�ɂ��Ċi�[����
		long lDispClassID = cDispNaviRelation.GetLongValue( lDispClassIDIndex, -1 );
		long lNaviClassID = cDispNaviRelation.GetLongValue( lNaviClassIDIndex, -1 );
		m_mapCache[ lDispClassID ][ lNaviClassID ] = cDispNaviRelation;
		m_mapCache[ lDispClassID ][ lNaviClassID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CDispNaviRelationTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CDispNaviRelationTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // dispotherrelation

} // sindy

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
 * @file DispOtherRelationTable.cpp
 * @brief  <b>CDispOtherRelationTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "DispOtherRelationTable.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace dispotherrelation {

// ���H�����N�̓��H��ʖ�ID�Ɠ��H�����N�̂��̑��̑�����ID�ɑΉ����郌�R�[�h��Ԃ�
const CDispOtherRelation* CDispOtherRelationTable::Find( long lDispClassID, long lOtherAttrID ) const
{
	std::map<long, std::map<long, CDispOtherRelation> >::const_iterator itByDispClassID = m_mapCache.find( lDispClassID );
	if( itByDispClassID != m_mapCache.end() )
	{
		std::map<long, CDispOtherRelation>::const_iterator itByOtherAttrID = itByDispClassID->second.find( lOtherAttrID );
		if( itByOtherAttrID != itByDispClassID->second.end() )
			return &(itByOtherAttrID->second);
	}
	return NULL;
}

// �e�[�u����S�ăL���b�V������
long CDispOtherRelationTable::CreateTableCache( bool bDetatch/* = false*/ )
{
//	TRACEMESSAGE( _T("CDispOtherRelationTable::CreateTableCache() : Started.\n") );

	CDispOtherRelation cDispOtherRelation;
	_ICursorPtr ipCursor = Search( NULL, bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// ���H�����N�̓��H��ʖ�ID�C���f�b�N�X�Ɠ��H�����N�̂��̑��̑�����ID�C���f�b�N�X��O�����Ď擾
	long lDispClassIDIndex = spFields->FindField( schema::dispotherrelation::kDispClassID );
	long lOtherAttrIDIndex = spFields->FindField( schema::dispotherrelation::kOtherAttrID );

	while( SUCCEEDED( ipCursor->NextRow( cDispOtherRelation.Release() ) ) )
	{
		if( cDispOtherRelation == NULL ) break;

		// ���H�����N�̓��H��ʖ�ID�Ɠ��H�����N�̂��̑��̑�����ID���L�[�ɂ��Ċi�[����
		long lDispClassID = cDispOtherRelation.GetLongValue( lDispClassIDIndex, -1 );
		long lOtherAttrID = cDispOtherRelation.GetLongValue( lOtherAttrIDIndex, -1 );
		m_mapCache[ lDispClassID ][ lOtherAttrID ] = cDispOtherRelation;
		m_mapCache[ lDispClassID ][ lOtherAttrID ].CreateCache( bDetatch, spFields, spNameString );
	}

	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CDispOtherRelationTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();
}

// �L���b�V�����N���A����
void CDispOtherRelationTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // dispotherrelation

} // sindy

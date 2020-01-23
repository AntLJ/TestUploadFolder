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
 * @file DeletableLayersTable.cpp
 * @brief <b>CDeletableLayersTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "DeletableLayersTable.h"
#include "ListString.h"
#include "GlobalRules.h"
#include "Rule.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR LAYER_CANNOT_BE_DELETED = _T("�폜�s�\�ȃ��C���ł��F%s");	
#else
	LPCTSTR LAYER_CANNOT_BE_DELETED = _T("Layer that cannot be deleted: %s");
#endif	// SINDY_FOR_ENGLISH
}

namespace sindy {
namespace deletablelayers {
using namespace sindy::errorcode;

// �n���ꂽ�I�u�W�F�N�g���ړ��\���ǂ������`�F�b�N����
sindyeEditableType CDeletableLayersTable::IsDeletable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	sindyeEditableType emType = sindyeEditableNoDefine;	// �Ԃ�l

	const CDeletableLayers* pRule = Find( lPurposeID, lLayerNameID );
	if( pRule != NULL )
		emType = pRule->IsEditable( cRow, nLen, lpszMessage );
	else {
		_sntprintf( lpszMessage, nLen, LAYER_CANNOT_BE_DELETED, cRow.GetTableName() );
		emType = sindyeEditableNoEditable;	// �폜�\���C���ł͂Ȃ�
	}

	return emType;
}

// ��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h��Ԃ�
const CDeletableLayers* CDeletableLayersTable::Find( long lPurposeID, long lLayerNameID ) const
{
	std::map<long, std::map<long, CDeletableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		std::map<long, CDeletableLayers>::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
		if( itByLayerNameID != itByPurposeID->second.end() )
			return &(itByLayerNameID->second);
	}
	return NULL;
}

// �����ҏW�\���C�����X�g��Ԃ�
errorcode::sindyErrCode CDeletableLayersTable::GetDeletableLayers( long lPurposeID, CListString& listLayerName ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	std::map<long, std::map<long, CDeletableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		// LAYERNAMES�e�[�u���擾
		const layernames::CLayerNamesTable& cLayers = g_cRule.GetLayerNamesTable();
		TCHAR strLayerName[64];
		for( std::map<long,CDeletableLayers>::const_iterator it = itByPurposeID->second.begin(); it != itByPurposeID->second.end(); ++it )
		{
			LOGASSERTERR_IF( NULL != cLayers.Find( it->first, 64, strLayerName ), sindyErr_RuleNotFound )
			{
				listLayerName.push_back( strLayerName );
			}
		}	
	}
	else
		emErr = sindyErr_RuleNotFound;

	return emErr;
}

// �e�[�u����S�ăL���b�V������
long CDeletableLayersTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CDeletableLayersTable::CreateTableCache() : Started.\n") );

	CDeletableLayers cDeletableLayers;
	_ICursorPtr ipCursor;
	if( 0 > lPurposeID )
		ipCursor = Search( NULL, bDetatch );
	else
		ipCursor = Search( AheInitQueryFilter( NULL, _T("*"), _T("PURPOSE_ID=%d"), lPurposeID ), bDetatch );

	// ���L�I�u�W�F�N�g�쐬
	boost::shared_ptr<CFieldMap> spFields = GetFieldMap();
	boost::shared_ptr<CTableNameString> spNameString = GetNameString();

	// �L���b�V���O�ɋ��L�I�u�W�F�N�g���V�K�ɍ쐬����Ă��܂����Ƃ�h�����߁A
	// ��ƖړIID�C���f�b�N�X�ƃ��C����ID�C���f�b�N�X��O�����Ď擾
	long lPurposeIDIndex = spFields->FindField( schema::deletablelayers::kPurposeID );
	long lLayerNameIDIndex = spFields->FindField( schema::deletablelayers::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cDeletableLayers.Release() ) ) )
	{
		if( cDeletableLayers == NULL ) break;

		// ��ƖړI��ID�ƃ��C����ID���L�[�ɂ��Ċi�[
		long lPurposeID = cDeletableLayers.GetLongValue( lPurposeIDIndex, -1 );
		long lLayerNameID = cDeletableLayers.GetLongValue( lLayerNameIDIndex, -1 );

		m_mapCache[ lPurposeID ][ lLayerNameID ] = cDeletableLayers;
		m_mapCache[ lPurposeID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CDeletableLayersTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();

}

// �L���b�V�����N���A����
void CDeletableLayersTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // deletablelayers

} // sindy

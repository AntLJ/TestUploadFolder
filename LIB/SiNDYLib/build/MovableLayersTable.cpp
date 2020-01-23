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
 * @file MovableLayersTable.cpp
 * @brief <b>CMovableLayersTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "MovableLayersTable.h"
#include "ListString.h"
#include "LayerNamesTable.h"
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
	LPCTSTR LAYER_CANNOT_BE_MOVED = _T("�ړ��s�\�ȃ��C���ł��F%s");
#else
	LPCTSTR LAYER_CANNOT_BE_MOVED = _T("Layer that cannot be moved: %s");
#endif	// SINDY_FOR_ENGLISH
}

namespace sindy {
namespace movablelayers {
using namespace sindy::errorcode;

// �n���ꂽ�I�u�W�F�N�g���ړ��\���ǂ������`�F�b�N����
sindyeEditableType CMovableLayersTable::IsMovable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	sindyeEditableType emType = sindyeEditableNoDefine;	// �Ԃ�l

	const CMovableLayers* pRule = Find( lPurposeID, lLayerNameID );
	if( pRule != NULL )
		emType = pRule->IsEditable( cRow, nLen, lpszMessage );
	else {
		_sntprintf( lpszMessage, nLen, LAYER_CANNOT_BE_MOVED, cRow.GetTableName() );
		emType = sindyeEditableNoEditable;	// �ړ��\���C���ł͂Ȃ�
	}

	return emType;
}

// ��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h��Ԃ�
const CMovableLayers* CMovableLayersTable::Find( long lPurposeID, long lLayerNameID ) const
{
	std::map<long, std::map<long, CMovableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		std::map<long, CMovableLayers>::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
		if( itByLayerNameID != itByPurposeID->second.end() )
			return &(itByLayerNameID->second);
	}
	return NULL;
}

// �ړ��\���C���̃��X�g��Ԃ�
errorcode::sindyErrCode CMovableLayersTable::GetMovableLayers( long lPurposeID, CListString& listLayerName ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	std::map<long, std::map<long, CMovableLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		// LAYERNAMES�e�[�u���擾
		const layernames::CLayerNamesTable& cLayers = g_cRule.GetLayerNamesTable();
		TCHAR strLayerName[64];
		for( std::map<long,CMovableLayers>::const_iterator it = itByPurposeID->second.begin(); it != itByPurposeID->second.end(); ++it )
		{
			LOGASSERTERR_IF( NULL != cLayers.Find( it->first, 64, strLayerName ), sindyErr_RuleNotFound )
				listLayerName.push_back( strLayerName );
		}	
	}
	else
		emErr = sindyErr_RuleNotFound;

	return emErr;
}

// �e�[�u����S�ăL���b�V������
long CMovableLayersTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CMovableLayersTable::CreateTableCache() : Started.\n") );

	CMovableLayers cMovableLayers;
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
	long lPurposeIDIndex = spFields->FindField( schema::movablelayers::kPurposeID );
	long lLayerNameIDIndex = spFields->FindField( schema::movablelayers::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cMovableLayers.Release() ) ) )
	{
		if( cMovableLayers == NULL ) break;

		// ��ƖړI��ID�ƃ��C����ID���L�[�ɂ��Ċi�[
		long lPurposeID = cMovableLayers.GetLongValue( lPurposeIDIndex, -1 );
		long lLayerNameID = cMovableLayers.GetLongValue( lLayerNameIDIndex, -1 );

		m_mapCache[ lPurposeID ][ lLayerNameID ] = cMovableLayers;
		m_mapCache[ lPurposeID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CMovableLayersTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();

}

// �L���b�V�����N���A����
void CMovableLayersTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // movablelayers

} // sindy

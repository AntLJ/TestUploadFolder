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
 * @file EditAttrLayersTable.cpp
 * @brief <b>CEditAttrLayersTable�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "EditAttrLayersTable.h"
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
	LPCTSTR ATTRIBUTE_CANNOT_BE_EDITED = _T("�����ҏW�s�\�ȃ��C���ł��F%s");
#else
	LPCTSTR ATTRIBUTE_CANNOT_BE_EDITED = _T("Layer for which the attributes cannot be edited: %s");
#endif	// SINDY_FOR_ENGLISH
}

namespace sindy {
namespace editattrlayers {
using namespace sindy::errorcode;

// �n���ꂽ�I�u�W�F�N�g�������ҏW�\���ǂ������`�F�b�N����
sindyeEditableType CEditAttrLayersTable::IsAttributeEditable( long lPurposeID, long lLayerNameID, const CRow& cRow, int nLen, LPTSTR lpszMessage ) const
{
	sindyeEditableType emType = sindyeEditableNoDefine;	// �Ԃ�l

	const CEditAttrLayers* pRule = Find( lPurposeID, lLayerNameID );
	if( pRule != NULL )
		emType = pRule->IsEditable( cRow, nLen, lpszMessage );
	else {
		_sntprintf( lpszMessage, nLen, ATTRIBUTE_CANNOT_BE_EDITED, cRow.GetTableName() );
		emType = sindyeEditableNoEditable;	// �����ҏW�\���C���ł͂Ȃ�
	}

	return emType;
}

// ��ƖړI�ԍ��ƃ��C�����ɑΉ����郌�R�[�h��Ԃ�
const CEditAttrLayers* CEditAttrLayersTable::Find( long lPurposeID, long lLayerNameID ) const
{
	std::map<long, std::map<long, CEditAttrLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		std::map<long, CEditAttrLayers>::const_iterator itByLayerNameID = itByPurposeID->second.find( lLayerNameID );
		if( itByLayerNameID != itByPurposeID->second.end() )
			return &(itByLayerNameID->second);
	}
	return NULL;
}

// �����ҏW�\���C���̈ꗗ��Ԃ�
errorcode::sindyErrCode CEditAttrLayersTable::GetEditAttrLayers( long lPurposeID, CListString& listLayerName ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	std::map<long, std::map<long, CEditAttrLayers> >::const_iterator itByPurposeID = m_mapCache.find( lPurposeID );
	if( itByPurposeID != m_mapCache.end() )
	{
		// LAYERNAMES�e�[�u���擾
		const layernames::CLayerNamesTable& cLayers = g_cRule.GetLayerNamesTable();
		TCHAR strLayerName[64];
		for( std::map<long,CEditAttrLayers>::const_iterator it = itByPurposeID->second.begin(); it != itByPurposeID->second.end(); ++it )
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
long CEditAttrLayersTable::CreateTableCache( bool bDetatch/* = false*/, long lPurposeID/* = -1*/ )
{
//	TRACEMESSAGE( _T("CEditAttrLayersTable::CreateTableCache() : Started.\n") );

	CEditAttrLayers cEditAttrLayers;
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
	long lPurposeIDIndex = spFields->FindField( schema::editattrlayers::kPurposeID );
	long lLayerNameIDIndex = spFields->FindField( schema::editattrlayers::kLayerNameID );

	while( SUCCEEDED( ipCursor->NextRow( cEditAttrLayers.Release() ) ) )
	{
		if( cEditAttrLayers == NULL ) break;

		// ��ƖړI��ID�ƃ��C����ID���L�[�ɂ��Ċi�[
		long lPurposeID = cEditAttrLayers.GetLongValue( lPurposeIDIndex, -1 );
		long lLayerNameID = cEditAttrLayers.GetLongValue( lLayerNameIDIndex, -1 );

		m_mapCache[ lPurposeID ][ lLayerNameID ] = cEditAttrLayers;
		m_mapCache[ lPurposeID ][ lLayerNameID ].CreateCache( bDetatch, spFields, spNameString );
	}
	m_bCached = true;

	// �؂藣��
	if( bDetatch )
		CComWrapper<ITable>::Release();

//	TRACEMESSAGE( _T("CEditAttrLayersTable::CreateTableCache() : Finished.\n") );

	return m_mapCache.size();

}

// �L���b�V�����N���A����
void CEditAttrLayersTable::ClearTableCache()
{
	m_mapCache.clear();
	m_bCached = false;
}

} // editattrlayers

} // sindy

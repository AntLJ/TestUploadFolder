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

/*! \file FeatureDef.cpp
	\brief CFeatureDef �N���X�̃C���v�������e�[�V����
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../FeatureDef.h"

#include "ArcHelper.h"
#include "MapHelper.h"

using namespace sindy::schema;

namespace
{
	/**
	 * @brief Z�l�t���̃|�C���g���ǂ����𔻒肷��
	 * @param [in] ipFeatureClass �t�B�[�`���N���X
	 * @retval true �t�B�[�`���N���X��Z�l�t���̃|�C���g
	 * @retval false �t�B�[�`���N���X��Z�l�t���̃|�C���g�ł͂Ȃ�
	 */
	bool IsPointWithZ(IFeatureClassPtr ipFeatureClass)
	{
		// �t�B�[�`���N���X���L������Z�l�����łȂ����false�Ԃ�
		if(!(ipFeatureClass && AheHasZ(ipFeatureClass)))
			return false;

		// �t�B�[�`���N���X�̃W�I���g���^�C�v���擾����BPOINT�^���ǂ����𔻒f��Ƃ���
		esriGeometryType type = esriGeometryNull;
		ipFeatureClass->get_ShapeType(&type);
		
		return type == esriGeometryPoint;
	}
}

/**
 * �R���X�g���N�^
 */
CFeatureDef::CFeatureDef()
{
	Clear();
}

/**
 * �f�X�g���N�^
 */
CFeatureDef::~CFeatureDef()
{
	Clear();
}
/**
 * �����o�ϐ���S�ď�����Ԃɖ߂�
 */
void CFeatureDef::Clear()
{
	CRowDef::Clear();
	m_ipFeature = NULL;
	m_bError = FALSE;
	m_listRelationalTable.clear();
	m_bWarnDelete = FALSE;
	m_sindyeEditableType = sindyeEditableNoDefine;
}
/**
 * �����o�ϐ����t�B�[���h�l�̕ύX��������O�ɖ߂�
 */
void CFeatureDef::Reset()
{
	CRowDef::Reset();
	/// �O���e�[�u�������Z�b�g
	std::list<CRelationalTableDef>::iterator it = m_listRelationalTable.begin();
	while( it != m_listRelationalTable.end() ) {
		it->Reset();

		/// ���Z�b�g�̌��ʊO���e�[�u���̃��R�[�h����0�ɂȂ�����O���e�[�u�����̂��폜
		if( it->size() == 0 )
			it = m_listRelationalTable.erase(it);
		else
			it++;
	}
}
/**
 * �t�B�[���h�l�̕ύX���f�[�^�x�[�X�ɏ������݁A���������X�V����
 *
 * @param bDoOperation		[in]	StartEditOperation �������ł��邩�A�Ăяo�����̃c�[���ł��邩�ǂ���
 *
 */
void CFeatureDef::Store( BOOL bDoOperation )
{
	// �t�B�[�`���̃t�B�[���h�̕ۑ�
	BOOL bStored = CRowDef::Store( bDoOperation );

	// �O���e�[�u���ւ̕ύX��K�p
	// TODO: �����̏����͂ق�Ƃ��ɕK�v���ǂ����܂ߗv����
	//       �����炭����ɂ������̂͋K���̃e�[�u���������Ǝv���邪�Am_listRelationalTable �ɂ�
	//       ArcMap��Ńe�[�u�����������ۂ̃e�[�u�����i�[����Ă���B�Ȃ̂ŁA���x�����O�̂��߂ɑ��̃e�[�u����
	//       �������Ă��肷��Ƃ���������ɂ��Ă��܂����ƂɂȂ�A���Ɋ낤���C������B
	//       ���Ȃ݂ɁA���̒��ł�Store()�́A�ύX�������ꍇ�݂̂�put_Value()�ł���A���ۂɂ͕ۑ��͂���Ȃ��B
	//       �܂��A�ʏ�A�O���e�[�u���͕ύX����Ă��邱�Ƃ͂Ȃ��͂��Ȃ̂ŁAput_Value()������Ă��Ȃ��͂��B
	for( std::list<CRelationalTableDef>::iterator it = m_listRelationalTable.begin(); it != m_listRelationalTable.end(); it++ ) 
	{
		it->Store();

		// ���R�[�h��0�̎��Ƀ����[�V�����t���O�������ςȂ��Ȃ�t���O��OFF�ɂ���
		if( it->IsNoRows() )
		{
			CString strRFName;
			// �e�[�u�������烊���[�V�����t���O�����擾
			if( it->GetTableName().CompareNoCase(TIME_ONEWAY_TABLE_NAME) == 0 )
				strRFName = _T("ONEWAY_RF");
			else if( it->GetTableName().CompareNoCase(TIME_NOPASSAGE_TABLE_NAME) == 0 )
				strRFName = _T("NOPASSAGE_RF");
			else if( it->GetTableName().CompareNoCase(DUPLILINK_TABLE_NAME) == 0 )
				strRFName = _T("DUPLINK_RF");

			// ���̃����[�V������������ɂ��Ȃ�
			// ���̃e�[�u������������Ă���ꍇ�����邪�A���̎��͖����ibug 10209�j
			if( strRFName.IsEmpty() )
				continue;

			// �擾���������[�V�����t���O���`�F�b�N
			CFieldDef* pFieldDef = GetFieldDef( (LPCTSTR)m_strTableName, (LPCTSTR)strRFName );
			ATLASSERT( pFieldDef != NULL ); // ��΂���͂�

			if( pFieldDef->m_vaValue.lVal > 0 )
			{
				pFieldDef->m_vaValue.lVal = 0;
				// �܂� Store() ����Ă��Ȃ���΂��̂܂܁A�łȂ���΂����� Store()
				if( !pFieldDef->m_bChanged )
				{
					pFieldDef->m_bChanged = TRUE;
					pFieldDef->Store();
				}

//{ ADD 02.10.28 by hkakeno for TEST
				//	���ԋK�����R�[�h���Ȃ��Ȃ����ꍇ�A��ʎ�ʂ��u��ʂȂ��v�ɂ���
				if( it->GetTableName().CompareNoCase(TIME_ONEWAY_TABLE_NAME) == 0 )
				{
					CFieldDef* pFieldDef2 = GetFieldDef( (LPCTSTR)m_strTableName, (LPCTSTR)_T("ONEWAY_C") );
					pFieldDef2->m_vaValue.lVal = 0;
					if( !pFieldDef2->m_bChanged )
					{
						pFieldDef2->m_bChanged = TRUE;
						pFieldDef2->Store();
					}
				}
//} ADD 02.10.28 by hkakeno for TEST

				// �t�B�[�`���̑������ύX���ꂽ�̂�
				bStored = TRUE;
			}
		}
	}

	// ��������������
	if( bStored )
	{
		// StartEditOperation �����Ȃ��ꍇ�ɂ͕ҏW�����͌Ăяo���������AStore() ���Ăяo����������
		if( bDoOperation )
		{
			if( !AheSetModifyData( m_ipFeature, sindyUpdateTypeProperty ) )
				ATLASSERT( FALSE );	// iPC Feature �ȊO��ҏW����Ƃ����Ȃ�H
			
			m_ipFeature->Store();

			BOOL		bTest;
			IFieldsPtr	ipFields;
			m_ipFeature->get_Fields(&ipFields);
			CComVariant	vaField;
			long		lTemp;
			ipFields->FindField( CComBSTR(_T("ONEWAY_RF")), &lTemp );
			m_ipFeature->get_Value( lTemp, &vaField );
			bTest = vaField.bVal;
			ipFields->FindField( CComBSTR(_T("ONEWAY_C")), &lTemp );
			m_ipFeature->get_Value( lTemp, &vaField );
			bTest = vaField.lVal;
		}
	}
}
/**
 * CArcHelper �N���X���Z�b�g����
 *
 * @param pHelper	[in]	CArcHelper �N���X�̃|�C���^
 */
void CFeatureDef::SetArcHelper( IApplication* ipApp )
{
	ATLASSERT( ipApp != NULL );

	m_cArcHelper.Init( (IApplicationPtr)ipApp );
}
/**
 * �t�B�[�`�����Z�b�g����
 *
 * @param ipFeature	[in]	���ƂȂ� _IRow �C���^�[�t�F�[�X�|�C���^
 *
 * �S�Ẵt�B�[���h�𕪉����A���̍� *_RF �Ƃ����t�B�[���h�����݂�����O���e�[�u���Ƃ݂Ȃ���
 * �O���e�[�u�������l�ɒ�`���܂��B
 *
 * \note ���ݑΉ����Ă��� *_RF �t�B�[���h�� TIMEREG_RF�ADRMA_RF�ADUPLINK_RF �݂̂ł��B
 * \note ��L�ȊO�� *_RF �t�B�[���h��Ή�������ɂ̓R�[�h���ɖ��ߍ��ޕK�v������܂��B
 */
void CFeatureDef::SetFeature( _IRowPtr ipFeature )
{
	IMapPtr ipMap( m_cArcHelper.GetMap() );
	IFieldsPtr ipFields;
	ITablePtr ipTable;

	CString strTableName, strFieldName;
	LONG lFieldCount = 0;

	if( !ipFeature )
		return;	//!< �t�B�[�`���ł͂Ȃ�

	USES_CONVERSION;

	/// �e�[�u�������擾
	strTableName = AheGetFeatureClassName( ipFeature );

	ipFeature->get_Fields( &ipFields );
	if( !ipFields )
		return;
	ipFields->get_FieldCount( &lFieldCount );

	/// �t�B�[���h�ɕ������ēo�^
	for( LONG i = 0; i < lFieldCount; i++ ) {
		CComBSTR bstrFieldName;
		IFieldPtr ipField;
		ipFields->get_Field( i, &ipField );
		ipField->get_Name( &bstrFieldName );
		strFieldName = OLE2T( bstrFieldName );

		/// �t�B�[���h�̓o�^
		CFieldDef fieldDef;
		fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
		fieldDef.SetFieldDef( strTableName, strFieldName );
		fieldDef.SetRowBuffer( i, ipField, ipFeature );
		push_back( fieldDef );

		// �O���e�[�u���A�t�B�[���h���擾
		CString strOuterTableName;
		CString strOuterFieldName;
		if( ! getOuterTableAndFieldName(strTableName, strFieldName, strOuterTableName, strOuterFieldName) )
			continue;

		if( strOuterTableName.Compare(_T("")) == 0 )
		{
			ATLASSERT( strOuterTableName.Compare(_T("")) != 0 );
			ATLTRACE(_T("CFeatureDef::SetFeature() : �����[�V�����t���O�����݂��Ă��܂����A�e�[�u������������܂���ł��� %s\n"), strFieldName );
			continue;
		} 
			
		/// �����[�V�����t���O�̒l��0�Ȃ�e�[�u���̓o�^�͂��Ȃ�
		CComVariant vaValue;
		ipFeature->get_Value( i, &vaValue );
		if( vaValue.vt == VT_NULL || vaValue == _variant_t(0L) )
			continue;

		// �֘A�e�[�u�����Z�b�g
		ipTable = AheOpenSameOwnerTable( (ITablePtr)AheGetFeatureClass( ipFeature ), strOuterTableName );
		CRelationalTableDef relTableDef;
		relTableDef.SetArcHelper( m_cArcHelper.GetApp() );

		LONG lOID = -1;
		if( strOuterFieldName == _T("LINK_ID") )
			ipFeature->get_OID( &lOID );
		else
			lOID = vaValue.lVal;

		relTableDef.SetTable( ipTable, strOuterTableName, lOID, strOuterFieldName );
		m_listRelationalTable.push_back( relTableDef );

	}
	// Z�l�t���̃|�C���g�ł����Z�l�̉��z�t�B�[���h�ǉ�(2017/2/17 ta_suga)
	if(IsPointWithZ(AheGetFeatureClass(ipFeature)))
	{
		/// �t�B�[���h�̓o�^
		CFieldDef fieldDef;
		fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
		fieldDef.SetFieldDef( strTableName, virtual_field::kShapeZ ); // �t�B�[���h���́uSHAPE.Z�v
		fieldDef.m_strFieldAliasName = virtual_field::kShapeZ;        // �G�C���A�X�����uSHAPE.Z�v
		fieldDef.SetRowBuffer( -1, nullptr, ipFeature );              // �t�B�[���h�C���f�b�N�X�A�t�B�[���h�Ȃ��œo�^
		push_back( fieldDef );
	}

	// �����[�V�����������Ă���Ȃ烊���[�V��������擾�i�e�[�u����������j
	CMapHelper cMapHelper( m_cArcHelper );
	CComBSTR bstrLayerName;
	ILayerPtr ipLayer;

	cMapHelper.GetLayerTableName( ipFeature, &bstrLayerName );
	cMapHelper.GetLayerByTableName( OLE2T( bstrLayerName ), &ipLayer );
	IDisplayRelationshipClassPtr ipDispRelClass( ipLayer );

	IRelationshipClassPtr ipRelClass;

	if( ipDispRelClass != NULL ) ipDispRelClass->get_RelationshipClass( &ipRelClass );
	if( ipRelClass != NULL )
	{
		esriRelCardinality relcardinal;

		ipRelClass->get_Cardinality( &relcardinal );
		if( relcardinal == esriRelCardinalityOneToMany )
		{
			CComBSTR bstrForeignKey, bstrPrimaryKey, bstrTableName;
			IObjectClassPtr ipObjectClass;

			ipRelClass->get_OriginForeignKey( &bstrForeignKey );
			ipRelClass->get_OriginPrimaryKey( &bstrPrimaryKey );

			ipRelClass->get_OriginClass( &ipObjectClass );
			ITablePtr ipTable( ipObjectClass );
			IDatasetPtr ipDataset( ipTable );

			ipDataset->get_Name( &bstrTableName );
			CString strTableName( OLE2T( bstrTableName ) );
			strTableName.MakeUpper();
			strTableName = 	strTableName.ReverseFind('.') < 0 ? strTableName : strTableName.Right( strTableName.GetLength() - strTableName.ReverseFind('.') - 1 );

			// �e�[�u���������Ă��āA���A�t�B�[���h�ɕ������Č������悤�Ƃ��Ă��܂��Ă���ꍇ�i���̊֐��̍ŏ��̂ق��ł���Ă��鏈���j
			// �ɂ́A�j�d�ɓo�^����Ă��܂����ꂪ����̂ŁA�����e�[�u����o�^���Ȃ��悤�ɂ����Œ���
			bool bFind = false;
			for( std::list<CRelationalTableDef>::iterator it = m_listRelationalTable.begin(); it != m_listRelationalTable.end(); it++ ) 
			{
				CString strTest = it->GetTableName();
				if( strTest == strTableName ) 
					bFind = true;
			}
			if( !bFind )
			{
				LONG lForeignKey = 0;
				CComVariant vaValue;

				ipFeature->get_Fields( &ipFields );
				ipFields->FindField( bstrForeignKey, &lForeignKey );
				ipFeature->get_Value( lForeignKey, &vaValue );

				ATLASSERT( vaValue.vt == VT_I4 );	// OBJECTID �Ȃ̂ŕK�� LONG �̂͂�

				CRelationalTableDef relTableDef;

				relTableDef.SetTable( ipTable, strTableName, vaValue.lVal, OLE2T( bstrPrimaryKey ) );
				m_listRelationalTable.push_back( relTableDef );
			}
		}
	}

	m_ipFeature = ipFeature;
	m_strTableName = strTableName;
}
/**
 * �Z�b�g����Ă���t�B�[�`�����擾����
 *
 * @return m_ipFeature ��Ԃ�
 */
_IRowPtr CFeatureDef::GetFeature()
{
	return m_ipFeature;
}
/**
 * �֘A�e�[�u����`�̃��X�g���擾����
 *
 * @return m_listRelationalTable �̃|�C���^��Ԃ�
 */
std::list<CRelationalTableDef>* CFeatureDef::GetRelationalTableDefList()
{
	return &m_listRelationalTable;
}
CRelationalTableDef* CFeatureDef::GetRelationalTableDef( LPCTSTR lpcszTableName )
{
	std::list<CRelationalTableDef>::iterator it;
	for( it = m_listRelationalTable.begin(); it != m_listRelationalTable.end(); it++ )
	{
		CString strTest = it->GetTableName();
		if( lstrcmpi( (LPCTSTR)it->GetTableName(), lpcszTableName ) == 0 ) 
			return &(*it);
	}

	return NULL;
}
/**
 * �e�[�u�����ƃt�B�[���h��������ɓo�^�ς݂� CFieldDef �I�u�W�F�N�g�̃|�C���^��Ԃ�
 *
 * @param lpszTableName	[in]	�e�[�u����
 * @param lpszFieldName	[in]	�t�B�[���h��
 *
 * @return ���݂���� CFieldDef �I�u�W�F�N�g�|�C���^�A���݂��Ȃ���� NULL ��Ԃ�
 *
 * \note �O���e�[�u���ɂ͑Ή����܂���i�����̃��R�[�h�����݂���ꍇ�����邽�߁j
 */
CFieldDef* CFeatureDef::GetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszFieldName )
{
	if( m_strTableName.CompareNoCase( lpszTableName ) == 0 )
		return CRowDef::GetFieldDef( lpszFieldName );
	else
		return NULL;
}
/**
 * �t�B�[�`���̃t�B�[���h�ƁA�����[�V�������Ă���O���e�[�u���̃��R�[�h�ŕύX���Ȃ����ǂ������ׂ�
 *
 * @return �ύX������ꍇ�� TRUE�A�Ȃ���� FALSE
 */
BOOL CFeatureDef::Changed()
{
	// �t�B�[�`���t�B�[���h
	if( CRowDef::Changed() )
		return TRUE;

	// �O���e�[�u���t�B�[���h
	std::list<CRelationalTableDef>* pRelationalTableDef = GetRelationalTableDefList();
	for( std::list<CRelationalTableDef>::iterator it = pRelationalTableDef->begin(); it != pRelationalTableDef->end(); it++ ) 
	{
		if( it->getChanged() )
			return TRUE;

		for( std::list<CRowDef>::iterator itRow = it->begin(); itRow != it->end(); itRow++ )
		{
			// �O���e�[�u���̃��R�[�h�ɐV�K�쐬�t���O���͍폜�t���O���t���Ă�����ύX����Ƃ���
			if( itRow->GetDeleteFlag() || itRow->GetCreateFlag() ) {
				ATLTRACE(_T("CFeatureDef::Changed()�F�O���e�[�u���̃��R�[�h�ɐV�K�쐬�t���O���͍폜�t���O���t���Ă��܂�\n"));

				return TRUE;
			}
			if( itRow->Changed() )
				return TRUE;
		}
	}
	return FALSE;
}


// �O���֘A�e�[�u�����A�t�B�[���h�����擾����
bool CFeatureDef::getOuterTableAndFieldName(const CString& strTableName, const CString& strFieldName, CString& strOuterTableName, CString& strOuterFieldName)
{
	using namespace sindy::schema;

	strOuterTableName = _T("");
	strOuterFieldName = _T("");

	// DUPLILINK�t�B�[���h
	if( strFieldName.CompareNoCase( road_link::kDupliLinkRF ) == 0)
	{
			strOuterTableName = _T("DUPLI_LINK");
			strOuterFieldName = _T("LINK_ID");
			return true;
	}

	// ���H�����N
	if( strTableName.CompareNoCase( road_link::kTableName ) == 0)
	{
		if( strFieldName.CompareNoCase( road_link::kOnewayRF) == 0 ){
			strOuterTableName = time_oneway::kTableName;
			strOuterFieldName = time_oneway::kLinkID;
		}
		else if( strFieldName.CompareNoCase( road_link::kNoPassageRF) == 0 ){
			strOuterTableName = time_nopassage::kTableName;
			strOuterFieldName = time_nopassage::kLinkID;
		}
		return true;
	}
	// ���s�҃����N
	if( strTableName.CompareNoCase( walk_link::kTableName ) == 0)
	{
		if( strFieldName.CompareNoCase( walk_link::kOnewayRF) == 0 ){
			strOuterTableName = _T("WALK_ONEWAY");
			strOuterFieldName = _T("LINK_ID");
		}
		else if( strFieldName.CompareNoCase( walk_link::kNoPassageRF) == 0 ){
			strOuterTableName = _T("WALK_NOPASSAGE");
			strOuterFieldName = _T("LINK_ID");
		}
		return true;
	}
	// �s�s�n�}���L�\���ʒu�e�[�u��
	if( strTableName.CompareNoCase( city_disp_line::kTableName) == 0 )
	{
		if( strFieldName.CompareNoCase( city_disp_line::kAnnoID) == 0 ){
			strOuterTableName = city_annotation::kTableName;
			strOuterFieldName = city_annotation::kObjectID;
		}
		return true;
	}
	// ���k���L�\���ʒu�e�[�u��
	if( strTableName.CompareNoCase( b_sc1disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( b_sc2disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( b_sc3disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( b_sc4disp_line::kTableName) == 0 ) 
	{
		if( strFieldName.CompareNoCase( b_sc1disp_line::kAnnoID) == 0 ){
			strOuterTableName = base_annotation::kTableName;
			strOuterFieldName = base_annotation::kObjectID;
		}
		return true;
	}
	// �~�h�����L�\���ʒu�e�[�u��
	if( strTableName.CompareNoCase( m_sc1disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( m_sc2disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( m_sc3disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( m_sc4disp_line::kTableName) == 0 ) 
	{
		if( strFieldName.CompareNoCase( m_sc1disp_line::kAnnoID) == 0 ){
			strOuterTableName = middle_annotation::kTableName;
			strOuterFieldName = middle_annotation::kObjectID;
		}
		return true;
	}
	// �g�b�v���L�\���ʒu�e�[�u��
	if( strTableName.CompareNoCase( t_sc1disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( t_sc2disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( t_sc3disp_line::kTableName) == 0 ||
	    strTableName.CompareNoCase( t_sc4disp_line::kTableName) == 0 ) 
	{
		if( strFieldName.CompareNoCase( t_sc1disp_line::kAnnoID) == 0 ){
			strOuterTableName = top_annotation::kTableName;
			strOuterFieldName = top_annotation::kObjectID;
		}
		return true;
	}

	// �S�������N�e�[�u��
	if( strTableName.CompareNoCase( sj::railway_link::kTableName) == 0 )
	{
		if( strFieldName.CompareNoCase( object::kObjectID) == 0 ){
			strOuterTableName = sj::rel_railway_link::kTableName;
			strOuterFieldName = sj::rel_railway_link::kRwLinkID;
		}
		return true;
	}
	// �S���m�[�h�e�[�u��
	if( strTableName.CompareNoCase( sj::railway_node::kTableName) == 0 )
	{
		if( strFieldName.CompareNoCase( object::kObjectID) == 0 ){
			strOuterTableName = sj::rel_railway_node::kTableName;
			strOuterFieldName = sj::rel_railway_node::kRwNodeID;
		}
		return true;
	}

	// �w�|���S���e�[�u��
	if( strTableName.CompareNoCase( sj::station_detail::kTableName ) == 0   ||
	    strTableName.CompareNoCase( sj::midzoom_platform::kTableName ) == 0 ||
	    strTableName.CompareNoCase( sj::highzoom_station::kTableName ) == 0 ||
	    strTableName.CompareNoCase( sj::midzoom_station::kTableName ) == 0  ||
	    strTableName.CompareNoCase( sj::lowzoom_platform::kTableName ) == 0   )
	{
		if( strFieldName.CompareNoCase( object::kObjectID) == 0 ){
			strOuterTableName = sj::rel_station_site::kTableName;
			strOuterFieldName = sj::rel_station_site::kLayerOID;
		}
		return true;
	}

	// [ADAM] ���f�������C��
	if( strTableName.CompareNoCase( adam_v2::crosswalk_line::kTableName ) == 0 )
	{
		if( strFieldName.CompareNoCase( adam_v2_adam_ipc_feature::kGlobalID) == 0 ){
			strOuterTableName = adam_v2::rel_lane_node_crosswalk_line::kTableName;
			strOuterFieldName = adam_v2::rel_lane_node_crosswalk_line::kCrosswalkLineGID;
		}
		return true;
	}

	// [ADAM] ���H�W���|�C���g
	if (strTableName.CompareNoCase(adam_v2::roadsign_point::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::rel_lane_node_roadsign_point::kTableName;
			strOuterFieldName = adam_v2::rel_lane_node_roadsign_point::kRoadsignPointGID;
		}
		return true;
	}

	// [ADAM] �Ԑ������N
	if (strTableName.CompareNoCase(adam_v2::lane_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_lane_node::kTableName;
			strOuterFieldName = adam_v2::nq_lane_node::kLaneNodeGID;
		}
		return true;
	}

	// [ADAM] �Ԑ��m�[�h��
	if (strTableName.CompareNoCase(adam_v2::lane_node::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_lane_node::kTableName;
			strOuterFieldName = adam_v2::nq_lane_node::kLaneLinkGID;
		}
		return true;
	}

	// [ADAM] �㉺�������N
	if (strTableName.CompareNoCase(adam_v2::updown_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_updown_node::kTableName;
			strOuterFieldName = adam_v2::nq_updown_node::kUpdownNodeGID;
		}
		return true;
	}

	// [ADAM] �㉺���m�[�h��
	if (strTableName.CompareNoCase(adam_v2::updown_node::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_updown_node::kTableName;
			strOuterFieldName = adam_v2::nq_updown_node::kUpdownLinkGID;
		}
		return true;
	}

	// [ADAM] ���������N
	if (strTableName.CompareNoCase(adam_v2::compart_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_compart_node::kTableName;
			strOuterFieldName = adam_v2::nq_compart_node::kCompartNodeGID;
		}
		return true;
	}

	// [ADAM] �����m�[�h��
	if (strTableName.CompareNoCase(adam_v2::compart_node::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_compart_node::kTableName;
			strOuterFieldName = adam_v2::nq_compart_node::kCompartLinkGID;
		}
		return true;
	}

	// [ADAM] ���H�W���G���A
	if (strTableName.CompareNoCase(adam_v2::roadmark_area::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::rel_roadmark_area_lane_link::kTableName;
			strOuterFieldName = adam_v2::rel_roadmark_area_lane_link::kRoadMarkAreaGID;
		}
		return true;
	}

	// [ADAM] ���H���E�������N
	if (strTableName.CompareNoCase(adam_v2::border_link::kTableName) == 0)
	{
		if (strFieldName.CompareNoCase(adam_v2_adam_ipc_feature::kGlobalID) == 0) {
			strOuterTableName = adam_v2::nq_border_node::kTableName;
			strOuterFieldName = adam_v2::nq_border_node::kBorderLinkGID;
		}
		return true;
	}

	return false;
}

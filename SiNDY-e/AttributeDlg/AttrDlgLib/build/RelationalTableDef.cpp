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

/*! \file RelationalTableDef.cpp
	\brief CRelationalTableDef �N���X�̃C���v�������e�[�V����
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../RelationalTableDef.h"
/**
 * �R���X�g���N�^
 */
CRelationalTableDef::CRelationalTableDef() : m_bChanged(false)
{
	Clear();
}
/**
 * �f�X�g���N�^
 */
CRelationalTableDef::~CRelationalTableDef()
{
	Clear();
}
/**
 * �����o��������Ԃɖ߂�
 */
void CRelationalTableDef::Clear()
{
	for( iterator it = begin(); it != end(); it++ ) 
		it->Clear();

	clear();

	m_ipTable = NULL;
}
/**
 * �t�B�[���h�ύX�O�ɖ߂�
 */
void CRelationalTableDef::Reset()
{
	iterator it = begin();
	while( it != end() )	
	{
		// CreateFlag �������Ă�����͍̂폜
		if( it->GetCreateFlag() ) {
			it = erase( it );
		} else {
			it->Reset();
			it++;
		}
	}
}
/**
 * �֘A�e�[�u����`���쐬����
 *
 * @param ipTable			[in]			��`����ITable�I�u�W�F�N�g
 * @param lpcszTableName	[in]			��`����e�[�u����
 * @patam lOID				[in]			�֘A����O���L�[�i�t�B�[�`���̃I�u�W�F�N�gID�j
 * @param lpcszOuterKey		[in]			�O���L�[�t�B�[���h��
 * @param bForce			[in,optional]	TRUE �̏ꍇ�A��̉��z���R�[�h������������I�ɍ쐬���܂��iCControlRel::ControlChanged()�Ŏg�p���邽�߁j
 * @param bNoSearch			[in,optional]	TRUE �̏ꍇ�A���������s���܂���iCLQAttributeDlgObj::SetLQs() �Ŏg�p���邽�߁j
 */
void CRelationalTableDef::SetTable( ITablePtr ipTable, LPCTSTR lpcszTableName, LONG lOID, LPCTSTR lpcszOuterKey, BOOL bForce/* = FALSE */, BOOL bNoSearch/* = FALSE*/ )
{
	ATLASSERT( ipTable != NULL );

	m_ipTable = ipTable;
	m_strTableName = lpcszTableName;
	m_lOID = lOID;

	ATLASSERT( lstrcmpi( lpcszOuterKey, _T("") ) != 0 );

	SetOuterKey( lpcszOuterKey, bForce, bNoSearch );
}
/**
 * �O���L�[�����ɁA�e�[�u�����猟�����ă��R�[�h���쐬����
 *
 *   SetTable() ������ɌĂ΂�܂�
 * 
 * @param lpcszOuterKey [in]			�O���L�[������
 * @param bForce		[in]			TRUE �̏ꍇ�A��̉��z���R�[�h������������I�ɍ쐬���܂��iCControlRel::ControlChanged()�Ŏg�p���邽�߁j
 * @param bNoSearch		[in,optional]	TRUE �̏ꍇ�A���������s���܂���iCLQAttributeDlgObj::SetLQs() �Ŏg�p���邽�߁j
 *
 * @return ����������ɏI�������� TRUE�A����������ɏI�����Ȃ����A�e�[�u�����̂����݂��Ȃ��ꍇ�� FALSE
 */
BOOL CRelationalTableDef::SetOuterKey( LPCTSTR lpcszOuterKey, BOOL bForce, BOOL bNoSearch/* = FALSE*/ )
{
	ATLASSERT( lpcszOuterKey != NULL );

	IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
	_ICursorPtr ipCursor;
	_IRowPtr ipRow;
	IFieldsPtr ipFields;
	IFieldPtr ipField;

	LONG lFieldCount = -1;
	CString strSubFields, strWhereClause;
	
	m_strOuterKey = lpcszOuterKey;
	
	// �t�B�[���h��o�^
	if( !m_ipTable ) {
		ATLTRACE(_T("CRelationalTableDef::SetOuterKey() : �e�[�u�����Z�b�g����Ă��܂���\n") );
		ATLASSERT(FALSE);
		return FALSE;
	}

	// �������Ȃ��ꍇ�͂�����
	if( bNoSearch )
		return TRUE;

	USES_CONVERSION;
	
	// �����t���O�������Ă������̉��z���R�[�h���쐬
	if( bForce ) {
		return CreateNewRow();
	}

	// �����t���O�������ĂȂ��ꍇ�͂�����
	strWhereClause.Format(_T("%s = %d"), m_strOuterKey, m_lOID );
	ipQueryFilter->put_WhereClause( CComBSTR(strWhereClause) );
	ipQueryFilter->put_SubFields( CComBSTR(strSubFields) );
	m_ipTable->Search( ipQueryFilter, VARIANT_FALSE, &ipCursor );

	if( !ipCursor ) 
		return FALSE;

	BSTR bstrFieldName;
	while( ipCursor->NextRow( &ipRow ) == S_OK ) {
		if( !ipRow )
			break;

		CRowDef rowDef;
		
		rowDef.SetRow( ipRow );
		rowDef.SetTable( m_ipTable );

		ipRow->get_Fields( &ipFields );
		ipFields->get_FieldCount( &lFieldCount );
		for( LONG i = 0; i < lFieldCount; i++ ) {
			ipFields->get_Field( i, &ipField );
			ipField->get_Name( &bstrFieldName );
			CString strFieldName = bstrFieldName;
			::SysFreeString(bstrFieldName);

			CFieldDef fieldDef;
			fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
			fieldDef.SetFieldDef( m_strTableName, strFieldName );
			fieldDef.SetRowBuffer( i, ipField, ipRow );
			rowDef.push_back( fieldDef );
		}
		push_back( rowDef );
	}

	return TRUE;
}
/**
 * �폜�t���O�܂��͐V�K�쐬�t���O�𗧂Ă�
 *
 * @param lIndex	[in]	�폜�t���O�܂��͐V�K�쐬�t���O�𗧂Ă郌�R�[�h�̃C���f�b�N�X�ԍ�
 * @param bDelete	[in]	�폜����ꍇ�� TRUE�A�V�K�쐬����ꍇ�� FALSE
 */
void CRelationalTableDef::SetDeleteFlag( LONG lIndex, BOOL bDelete/* = TRUE */ )
{
	iterator it = begin();
	advance( it, lIndex );
	it->SetDeleteFlag( bDelete );
}
/**
 * �ҏW���f�[�^�x�[�X�ɓK�p����
 */
void CRelationalTableDef::Store()
{
	for( iterator it = begin(); it != end(); it++ )
		it->Store( TRUE	);	// �O���e�[�u���̍X�V�����Ɋւ��Ă͕K�������K�v������
}
/**
 * �o�^����Ă��郌�R�[�h�����݂��邩�ǂ������`�F�b�N����
 *
 * �폜���ꂽ���R�[�h�Ɋւ��Ă̓J�E���g�ΏۊO�ł��B
 *
 * @return ������R�[�h�����݂��Ȃ��ꍇ�� TRUE�A�����łȂ��ꍇ�� FALSE
 */
BOOL CRelationalTableDef::IsNoRows()
{
	LONG lCount = 0;
	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->IsDeleted() )
			continue;
		else
			lCount++;
	}

	if( lCount > 0 ) 
		return FALSE;
	else 
		return TRUE;
}
/**
 * �V�������R�[�h��ǉ�����
 */
BOOL CRelationalTableDef::CreateNewRow()
{
	CRowDef rowDef;
//  MOD 02.10.01 by hkakeno
	//CComVariant vaDefaultValue;
	BSTR bstrFieldName;
	IFieldsPtr ipFields;
	IFieldPtr ipField;
	LONG lFieldCount = -1;
	VARIANT_BOOL vaIsNullable = VARIANT_FALSE;

	USES_CONVERSION;

	rowDef.SetTable( m_ipTable );
	rowDef.SetCreateFlag();	// �V�K�쐬�t���O������

	m_ipTable->get_Fields( &ipFields );
	ipFields->get_FieldCount( &lFieldCount );
	for( LONG i = 0; i < lFieldCount; i++ ) {
		ipFields->get_Field( i, &ipField );

		ipField->get_Name( &bstrFieldName );
		CString strFieldName = bstrFieldName;
		::SysFreeString( bstrFieldName );

		CComVariant vaDefaultValue;
		ipField->get_DefaultValue( &vaDefaultValue );
		ipField->get_IsNullable( &vaIsNullable );

		CFieldDef fieldDef;
		fieldDef.SetArcHelper( m_cArcHelper.GetApp() );
		fieldDef.SetFieldDef( m_strTableName, strFieldName );
		fieldDef.m_ipField = ipField;												// �t�B�[���h�Z�b�g
		fieldDef.m_lFieldIndex = i;													// �t�B�[���h�C���f�b�N�X�ԍ�
		
		LONG lVarType;
		ipField->get_VarType( &lVarType );
		if ((  vaDefaultValue.vt != VT_NULL || vaDefaultValue.vt != VT_EMPTY) 
			&& vaDefaultValue.vt != (unsigned short)lVarType)	
			vaDefaultValue.ChangeType((unsigned short)lVarType);
//} MOD 02.10.02 by hkakeno 		

		// �O���L�[�ێ��t�B�[���h�Ȃ�t�B�[�`����OID�����A�����łȂ���΃f�t�H���g�l���Z�b�g
		fieldDef.m_vaValue = ( 0 == fieldDef.m_strFieldName.CompareNoCase( GetOuterKey() ) ) ? GetOID() : vaDefaultValue;

		// �ύX����Ă���΁A�ύX�t���O���Z�b�g
		if( fieldDef.m_vaInitValue != fieldDef.m_vaValue )							
			fieldDef.m_bChanged = TRUE;

		// NULL OK ���ǂ����̃t���O
		fieldDef.m_bIsNullable = ( vaIsNullable == VARIANT_TRUE ) ? TRUE : FALSE;	

		// �����������t���O
		fieldDef.m_bIsInited = TRUE;

		rowDef.push_back( fieldDef );
	}
	push_back( rowDef );

	return TRUE;
}

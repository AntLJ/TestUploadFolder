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

/*! \file RowDef.cpp
	\brief CRowDef �N���X�̃C���v�������e�[�V����
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../RowDef.h"

#include "../FeatureDef.h"
/**
	�R���X�g���N�^�E�f�X�g���N�^
*/
/**
 * �R���X�g���N�^
 */
CRowDef::CRowDef()
{
	Clear();
}
/**
 * �f�X�g���N�^
 */
CRowDef::~CRowDef()
{
	Clear();
}
/**
 * �����o�ϐ���S�ď�����Ԃɖ߂�
 */
void CRowDef::Clear()
{
	m_ipRow = NULL;
	m_ipTable = NULL;
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;
	m_bDeleted = FALSE;
	clear();
}
/**
 * �����o�ϐ����t�B�[���h�l�̕ύX��������O�ɖ߂�
 */
void CRowDef::Reset()
{
	for( iterator i = begin(); i != end(); i++ ) {
		i->Reset();
	}
	/// �쐬�E�폜�t���O�͏�����
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;
}
/**
 * �t�B�[���h�l�̕ύX���f�[�^�x�[�X�ɏ�������
 *
 * �폜�w��t���O�im_bDeleteFlag�j��V�K�쐬�t���O�im_bCreateFlag�j�������Ă���Ƃ���
 * ���ꂼ��e�[�u������폜���̓e�[�u���ɐV�K�쐬���܂��B
 *
 * @note �Ԃ�l�̓t�B�[�`���̃t�B�[���h�̂ݔ��肷��̂ŁA�Ⴆ�ΊO���e�[�u���ɕύX���������Ƃ��Ă�
 * @note �t�B�[�`���̃t�B�[���h�ɕύX���Ȃ���� FALSE ���Ԃ�܂��B
 *
 * @param bDoOperation		[in]	StartEditOperation �������ł��邩�A�Ăяo�����̃c�[���ł��邩�ǂ���
 *
 * @return �ύX�������āAStore() �����ꍇ�� TRUE �AStore() �o���Ȃ������A���邢�͕ύX���Ȃ������ꍇ�� FALSE
 */
BOOL CRowDef::Store( BOOL bDoOperation )
{
	BOOL bStored = FALSE;

	if( m_bDeleteFlag && !m_bCreateFlag )
		Delete();
	else if( !m_bDeleteFlag && !m_bCreateFlag ) {
		for( iterator i = begin(); i != end(); i++ ) {
			if( i->Store() )
				bStored = TRUE;	// �t�B�[�`���̃t�B�[���h�ɕύX��������
		}

		// m_ipRow �� NULL �̏ꍇ�̓e�[�u���ł͂Ȃ��t�B�[�`���Ȃ̂ŁA���̏ꍇ�� CFeature::Store() �ŗ�������������
		// �V�K�쐬�̍ۂɂ�bDoOperation��FALSE�ł��Ă��܂��A�V�K�쐬�̍ۂɁA�֘A���郊���N����E���Ă��āA
		// ���̊֘A���郊���N��̑�����ύX�����ꍇ�ɁA����������Ȃ��Ȃ��Ă��܂����Ƃ����邽�߁A
		// �����_�C�A���O�ł͕ύX���������ꍇ�ɂ́A�K�������������悤�ɂ���
		// �悤�ɂ������������A�����ł����A�A�V�K�쐬�ł����ꍇ�ɂ͐V�K�쐬�ɂ��Ȃ���΂Ȃ�Ȃ����߁A�����ύX�Ƃ͋�ʂ����Ȃ�
		if( bStored && bDoOperation && m_ipRow )
		{
			if( !AheSetModifyData( m_ipRow, sindyUpdateTypeProperty ) )
				ATLASSERT( FALSE );	// iPC Feature �ȊO��ҏW����Ƃ����Ȃ�H
			m_ipRow->Store();	// �O���e�[�u���͂����� Store() ����
		}
	} else if( !m_bDeleteFlag && m_bCreateFlag ) {
		Create();
	}
	// �����[�V�����t���O ON -> �t�B�[���h���폜�����ꍇ�́A���L�̏����ɂȂ邽�ߓ��ʏ���
	else if( m_bDeleteFlag && m_bCreateFlag && !m_ipRow )
	{
		/// �t���O�ނ��N���A
		m_bDeleteFlag = FALSE;
		m_bCreateFlag = FALSE;
		// �폜�t���O��ݒ肵�Ȃ��ƁACFeatureDef::Store() �� �����[�V�����t���O OFF �̏��������܂������Ȃ�
		m_bDeleted = TRUE;
	}

	return bStored;
}
/**
 * �e�[�u�������擾����
 *
 * @return m_strTableName ��Ԃ�
 */
LPCTSTR CRowDef::GetTableName()
{
	return m_strTableName;
}
/**
 * �O���e�[�u���Ń��R�[�h���폜����ꍇ�A���̃t���O�𗧂Ă�
 *
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == FALSE -> �������R�[�h�폜
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == TRUE  -> �V�K�Ƀ��R�[�h���ǉ����ꂽ�������폜���ꂽ�i�ύX�Ȃ��j
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == FALSE -> �������R�[�h
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == TRUE  -> �V�K�Ƀ��R�[�h���ǉ����ꂽ
 */
void CRowDef::SetDeleteFlag( BOOL bDelete/* = TRUE */ )
{
	m_bDeleteFlag = bDelete;
}
/**
 * �O���e�[�u���Ƀ��R�[�h��ǉ�����ꍇ�A���̃t���O�𗧂Ă�
 *
 *  ��x�Z�b�g���ꂽ���̂��폜����Ƃ��� SetDeleteFlag(TRUE) �����Ă�������
 *
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == FALSE -> �������R�[�h�폜
 *  m_bDeleteFlag == TRUE,  m_bCreateFlag == TRUE  -> �V�K�Ƀ��R�[�h���ǉ����ꂽ�������폜���ꂽ�i�ύX�Ȃ��j
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == FALSE -> �������R�[�h
 *  m_bDeleteFlag == FALSE, m_bCreateFlag == TRUE  -> �V�K�Ƀ��R�[�h���ǉ����ꂽ
 */
void CRowDef::SetCreateFlag()
{
	m_bCreateFlag = TRUE;
}
/**
 * �e�[�u���ɐV�K�Ƀ��R�[�h��ǉ�����
 *
 * m_ipTable �����݂��āA���� m_ipRow �����݂��Ȃ��ꍇ�A�V�K�Ƀ��R�[�h���쐬���܂��B
 * �쐬�ɐ���������Am_bDelteFlag �� m_bCreateFlag �����ꂼ�� FALSE �ɃZ�b�g���܂��B
 */
void CRowDef::Create()
{
	if( !m_ipTable ) {
		ATLASSERT( FALSE );
		ATLTRACE(_T("CRowDef::Create() : �e�[�u�����Z�b�g����Ă��Ȃ����߃��R�[�h���쐬�ł��܂���\n"));
		return;
	}
	if( m_ipRow ) {
		ATLASSERT( FALSE );
		ATLTRACE(_T("CRowDef::Create() : ���Ƀ��R�[�h�������o�ϐ��Ƃ��đ��݂��܂�\n"));
		return;
	}
	
	m_ipTable->CreateRow( &m_ipRow );
	AheSetModifyData( m_ipRow, sindyUpdateTypeCreate );	// �V�K�쐬����
	
	if( !m_ipRow ) {
		ATLTRACE(_T("CRowDef::Create : ���R�[�h�̍쐬�Ɏ��s���܂����B���R�[�h�͍쐬����܂���B\n"));
		return;
	}

	IFieldPtr ipField;

	ATLTRACE(_T("CRowDef::Create() : ���R�[�h���쐬���܂�...\n"));
	for( iterator it = begin(); it != end(); ++it )
	{
		if( it->IsEditable() )
		{
			m_ipRow->put_Value( it->m_lFieldIndex, it->m_vaValue );
			ATLTRACE(_T("CRowDef::Create() : �t�B�[���h��%s �l��%s\n"), it->m_strFieldName, it->value2string() );
		}
	}

	if( m_ipRow->Store() != S_OK )
		ATLTRACE(_T("CRowDef::Create() : �쐬�Ɏ��s���܂���\n"));
	else
		ATLTRACE(_T("CRowDef::Create() : �쐬����\n"));

	/// �t���O�ނ��N���A
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;
}
/**
 * �e�[�u�����烌�R�[�h���폜����
 *
 * m_ipRow �����݂���ꍇ�Ƀe�[�u�����烌�R�[�h���폜���܂��B
 * �폜������������Am_bDeleteFlag �� m_bCreateFlag �� FALSE �ɃZ�b�g���܂��B
 * �܂��A�폜�������������Ƃ����� m_bDeleted �t���O�� TRUE �ɃZ�b�g���܂��B
 */
void CRowDef::Delete()
{
	if( !m_ipRow ) {
		// CFeatureDef::m_ipFeature �����݂��邩�ǂ����`�F�b�N�i�ق�Ƃɂ���Ȃ�ł����́[�H�j
		m_ipRow = reinterpret_cast<CFeatureDef *>(this)->GetFeature();
		if( !m_ipRow )
		{
			ATLASSERT( FALSE );
			ATLTRACE(_T("CRowDef::Delete() : ���R�[�h�������o�ϐ��Ƃ��đ��݂��Ă��܂���\n"));
			return;
		}
	}
	
	ATLTRACE(_T("CRowDef::Delete : ���R�[�h���폜���܂�...\n"));

	AheSetModifyData( m_ipRow, sindyUpdateTypeDelete );	// �폜����
	if( m_ipRow->Delete() != S_OK ) {
		ATLTRACE(_T("CRowDef::Delete() : �폜�Ɏ��s���܂���\n"));
		return;
	}

	/// �t���O�ނ��N���A
	m_bDeleteFlag = FALSE;
	m_bCreateFlag = FALSE;

	if( m_ipRow->Store() != S_OK ) {
		ATLTRACE(_T("CRowDef::Delete() : �폜�Ɏ��s���܂���\n"));
	} else {
		ATLTRACE(_T("CRowDef::Delete() : �폜����\n"));
	}

	/// �폜�����t���O�i���ꂪ�Ȃ���CRelationTableDef::IsNoRows()�Ń����[�V�����t���O�̃`�F�b�N���o���Ȃ��j
	m_bDeleted = TRUE;
}
/**
 * ���ƂȂ郌�R�[�h���Z�b�g����
 *
 * @param ipRow	[in]	���ƂȂ� _IRow �C���^�[�t�F�[�X�|�C���^
 */
void CRowDef::SetRow( _IRowPtr ipRow )
{
	m_ipRow = ipRow;
}
/**
 * m_ipRow �����݂���e�[�u�����Z�b�g����
 *
 * @param ipTable	[in]	ITable �C���^�[�t�F�[�X�|�C���^
 */
void CRowDef::SetTable( ITablePtr ipTable )
{
	m_ipTable = ipTable;
}
/**
 * �t�B�[���h������Y������ CFieldDef �|�C���^���擾����
 *
 * @param lpszFieldName	[in]	�擾�������t�B�[���h��
 *
 * @return ���ɓo�^����Ă���ΊY������ CFieldDef* ���A���݂��Ȃ���� NULL ��Ԃ�
 */
CFieldDef* CRowDef::GetFieldDef( LPCTSTR lpszFieldName )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		if( it->m_strFieldName.CompareNoCase( lpszFieldName ) == 0 )
			return &(*it);
	}

	return NULL;
}
/**
 * ���̃N���X�̑匳�ł��� _IRow �� OBJECTID ��Ԃ�
 *
 * return OBJECTID
 */
LONG CRowDef::GetRowOID()
{
	if( !m_ipRow ) 
		return -1;

	LONG lOID = 0;
	m_ipRow->get_OID( &lOID );

	return lOID;
}
/**
 * �t�B�[���h�ɕύX���Ȃ����ǂ������ׂ�
 *
 * @return �ύX������ꍇ�� TRUE�A�Ȃ���� FALSE
 */
BOOL CRowDef::Changed()
{
	// �쐬�E�폜�t���O�̃`�F�b�N
	if( GetDeleteFlag() || GetCreateFlag() )
	{
		ATLTRACE(_T("CRowDef::Changed()�F�쐬�E�폜�t���O���t���Ă��܂��B\n"));
		return TRUE;
	}
	// �t�B�[���h
	for( std::list<CFieldDef>::iterator itField = begin(); itField != end(); ++itField )
	{
		if( itField->m_bChanged )
		{
			ATLTRACE(_T("CRowDef::Changed()�F�l���ύX����Ă��܂��F%s value=%s and init value=%s\n"), 
				itField->m_strFieldName, itField->value2string(), itField->value2string(TRUE) );

			return TRUE;
		}
	}

	return FALSE;
}

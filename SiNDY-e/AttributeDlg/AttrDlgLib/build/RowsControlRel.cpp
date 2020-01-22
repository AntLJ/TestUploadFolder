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

/*! \file RowsControlRel.cpp
	\brief CRowsControlRel �N���X�̃C���^�[�t�F�[�X
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../RowsControlRel.h"

#include "MapHelper.h"
#include "ArcHelper.h"

/**
 * �R���X�g���N�^
 */
CRowsControlRel::CRowsControlRel()
{
	m_lFeatureDefIndex = -1;
	m_listFeatureDef.clear();
}
/**
 * �f�X�g���N�^
 */
CRowsControlRel::~CRowsControlRel()
{
	m_listFeatureDef.clear();
}
/**
 * �R���g���[����o�^����i�P��t�B�[���h�p�j
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID			[in]			�o�^����R���g���[����ID
 * @param type			[in]			�o�^����R���g���[���̃^�C�v
 * @param nGroupFrom	[in]			�\������O���[�v�̊J�n�C���f�b�N�X
 * @param nGroupTo		[in]			�\������O���[�v�̏I���C���f�b�N�X
 * @param lpszFieldName	[in,optional]	�R���g���[���������t�B�[���h�̖��O
 * @param uiCheckType	[in,optional]	�t�B�[���h�̒l�`�F�b�N�����鎞�̃^�C�v
 * @param lpszTableName	[in,optional]	�R���g���[���������t�B�[�`���̏�������e�[�u����
 * @param lpszOuterKey	[in,optional]	�֘A����t�B�[�`���̊O���L�[�p�t�B�[���h���i�O���e�[�u���������ꍇ�j
 * @param nFollowCtrlID	[in,optional]	�Ǐ]����R���g���[����ID
 * @param nFollowBy		[in,optional]	�Ǐ]����ꍇ�̃^�C�v
 * @param nRelType		[in,optional]	���g�p
 */
void CRowsControlRel::SetControl(INT			nID, 
								  CONTROLTYPE	type, 
								  INT			nGroupFrom,
								  INT			nGroupTo,
								  LPCTSTR		lpszFieldName/* = NULL */, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType )
{
	m_pControlRel.SetControl( nID, type, nGroupFrom, nGroupTo,
		lpszFieldName, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );
}
/**
 * �R���g���[����o�^����i�����t�B�[���h�p�j
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID			[in]			�o�^����R���g���[����ID
 * @param type			[in]			�o�^����R���g���[���̃^�C�v
 * @param nGroupFrom	[in]			�\������O���[�v�̊J�n�C���f�b�N�X
 * @param nGroupTo		[in]			�\������O���[�v�̏I���C���f�b�N�X
 * @param lpszFieldName	[in,optional]	�R���g���[���������t�B�[���h�̖��O���X�g
 * @param uiCheckType	[in,optional]	�t�B�[���h�̒l�`�F�b�N�����鎞�̃^�C�v
 * @param lpszTableName	[in,optional]	�R���g���[���������t�B�[�`���̏�������e�[�u����
 * @param lpszOuterKey	[in,optional]	�֘A����t�B�[�`���̊O���L�[�p�t�B�[���h���i�O���e�[�u���������ꍇ�j
 * @param nFollowCtrlID	[in,optional]	�Ǐ]����R���g���[����ID
 * @param nFollowBy		[in,optional]	�Ǐ]����ꍇ�̃^�C�v
 * @param nRelType		[in,optional]	���g�p
 */
void CRowsControlRel::SetControl(INT				 nID, 
								  CONTROLTYPE		 type, 
								  INT			     nGroupFrom,
								  INT			     nGroupTo,
								  std::list<CString> arrayFieldNames, 
								  UINT				 uiCheckType/* = NULL */,
								  LPCTSTR			 lpszTableName/* = NULL */, 
								  LPCTSTR			 lpszOuterKey/* = NULL */, 
								  INT				 nFollowCtrlID/* = NULL */,
								  UINT				 uiFollowBy/* = NULL */,
								  INT				 nRelType )
{
	m_pControlRel.SetControl( nID, type, nGroupFrom, nGroupTo,
		arrayFieldNames, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );
}
/**
 * �t�B�[�`����o�^����i�����t�B�[�`���p�j
 *
 * @param ipSet		[in]	�t�B�[�`���̓������Z�b�g
 *
 * @return �\�������t�B�[�`��
 */
_IRowPtr CRowsControlRel::SetFeatures( _ISetPtr ipSet )
{
	IUnknownPtr ipFeature;
	CMapHelper cMapHelper( m_cArcHelper );

	ipSet->Reset();
	while( ipSet->Next( &ipFeature ) == S_OK ) {
		if( !ipFeature ) break;
	
		CFeatureDef featDef;
		featDef.SetArcHelper( m_cArcHelper.GetApp() );
		featDef.SetFeature( ipFeature );
		m_listFeatureDef.push_back( featDef );
	}
	// �t�B�[�`����`�N���X���R���g���[���Ɋ֘A�t��
	m_pControlRel.SetFeatureDefList( &m_listFeatureDef );
	// ���݂̃t�B�[�`�����Z�b�g�i��ԍŏ��̂��́j
	SetView( 0 );

	if( m_listFeatureDef.size() == 0 )
		return NULL;
	else
		return m_listFeatureDef.begin()->GetFeature();	// ��ԍŏ���Ԃ�
}
/**
 * �t�B�[�`����o�^����i�P��t�B�[�`���p�j
 *
 * @param ipFeature		[in]	�Z�b�g�������t�B�[�`��
 *
 * \note �����o�^����ꍇ�͑����ČĂԂ��Ƃ� OK �ł����A�܂������Ǝ�������Ă��܂���B
 * \note ���̌��ʁA�t�B�[�`���̐؂�ւ������x������ƕ����o�^����Ă��܂��͂��ł��B
 */
void CRowsControlRel::SetFeature( IFeaturePtr ipFeature )
{
	CFeatureDef featDef;
	
	featDef.SetArcHelper( m_cArcHelper.GetApp() );
	featDef.SetFeature( ipFeature );
	m_listFeatureDef.push_back( featDef );

	// �t�B�[�`����`�N���X���R���g���[���Ɋ֘A�t��
	m_pControlRel.SetFeatureDefList( &m_listFeatureDef );
	// ���݂̃t�B�[�`�����Z�b�g
	SetView( 0 );
}
/**
 * �^����ꂽ�C���f�b�N�X�ԍ��Ŏ������t�B�[�`�����擾����
 *
 * @param lIndex	[in]	�t�B�[�`���̃C���f�b�N�X�ԍ�
 *
 * @return �o�^����Ă���ꍇ�̓t�B�[�`���i���R�[�h�j�A�o�^����Ă��Ȃ���� NULL ��Ԃ�
 */
_IRowPtr CRowsControlRel::GetFeature( LONG lIndex )
{
	ATLASSERT( lIndex >= 0 );

	// ����o�^����Ă��Ȃ��ꍇ�� NULL 
	if( m_listFeatureDef.size() == 0 ) return NULL;

	std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin();
	advance( i, lIndex );

	return i->GetFeature();
}
/**
 * ArcObjects �w���p�[�N���X���Z�b�g����
 *
 * ���̃N���X���g�p����O�ɕK���Z�b�g���Ă��������B
 *
 * @param pHelper	[in]	ArcObjects �w���p�[�N���X�̃|�C���^
 */
void CRowsControlRel::SetArcHelper( IApplication* ipApp )
{
	ATLASSERT( ipApp != NULL );
	m_cArcHelper.Init( (IApplicationPtr)ipApp );
	m_pControlRel.SetArcHelper( m_cArcHelper.GetApp() );
}
/**
 * �����o��S�ď���������
 */
void CRowsControlRel::Clear()
{
	m_listFeatureDef.clear();
	m_pControlRel.Clear();
}
/**
 * ������ԁi�ҏW�O�j�ɖ߂�
 */
void CRowsControlRel::Reset()
{
	for( std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin(); i != m_listFeatureDef.end(); i++ ) {
		i->Reset();		
	}
	m_pControlRel.ResetFeatures();
}
/**
 * �t�B�[�`����ۑ�����
 *
 * @param bDoOperation		[in]	StartEditOperation �������ł��邩�A�Ăяo�����̃c�[���ł��邩�ǂ���
 */
void CRowsControlRel::Store( BOOL bDoOperation )
{
	for( std::list<CFeatureDef>::iterator i = m_listFeatureDef.begin(); i != m_listFeatureDef.end(); i++ ) {
		i->Store( bDoOperation );
	}
}
/**
 * ���ݕ\������Ă���t�B�[�`���̃��b�p�N���X�̃|�C���^��Ԃ�
 *
 * @return ���ݕ\������Ă���t�B�[�`���̃��b�p�N���X�̃|�C���^��Ԃ�
 */
CFeatureDef* CRowsControlRel::GetCurrentFeatureDef()
{
	std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin();
	advance( it, m_lFeatureDefIndex );

	return &(*it);
}
void CRowsControlRel::SetView( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex )
{
	m_pControlRel.SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex );

	m_lFeatureDefIndex = lFeatureIndex;
}
/**
 * �ύX�����邩�ǂ����̔���
 *
 * return �ύX������ꍇ�� TRUE�A�Ȃ���� FALSE
 */
BOOL CRowsControlRel::Changed()
{
	for( std::list<CFeatureDef>::iterator it = m_listFeatureDef.begin(); it != m_listFeatureDef.end(); it++ )
	{
		if( it->Changed() ) 
			return TRUE;
	}

	return FALSE;
}

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

/*! \file ControlRel.cpp
	\brief CControlRel �N���X�̃C���v�������e�[�V����
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/
#include "stdafx.h"
#include "../ControlRel.h"

#include "ArcHelper.h"

/**
 * �R���X�g���N�^
 */
CControlRel::CControlRel() : m_hWnd(NULL)
{
	m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	Clear();
}
/**
 * �f�X�g���N�^
 */
CControlRel::~CControlRel()
{
	Clear();
}
/**
 * �S�Ẵ����o�ϐ�������������
 */
void CControlRel::Clear()
{
	iterator i;

	for( i = begin(); i != end(); i++ ) {
		i->Clear();
	}
	clear();

	m_pFeatureDefList = NULL;
}
void CControlRel::ResetHWND( HWND hWnd )
{
	for( iterator it = begin(); it != end(); it++ )
		it->SetHWND( ::GetDlgItem( hWnd, it->m_nID ) );
}
/**
 * �\���������t�B�[�`�����w�肷��
 *
 * @param lIndex	[in]	�C���f�b�N�X�ԍ�
 */
void CControlRel::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex )
{
	for( iterator it = begin(); it != end(); it++ )
		it->SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex );
}
/**
 * �S�ẴR���g���[����`�N���X�Ƀt�B�[�`����`���X�g���֘A�t����
 *
 * @param pFeatureDefList	[in]	�t�B�[�`����`�N���X�̃��X�g�|�C���^
 */
void CControlRel::SetFeatureDefList( std::list<FEATUREDEF>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	for( iterator it = begin(); it != end(); it++ ) it->SetFeatureDefList( pFeatureDefList );

	m_pFeatureDefList = pFeatureDefList;
}

/**
 * �t�B�[�`���̏����l�ŃR���g���[���̕\��������������
 */
void CControlRel::ResetFeatures()
{
	for( iterator i = begin(); i != end(); i++ ) {
		i->Reset();
		SetValue( &(*i) );	// �R���g���[�����ɒl���ăZ�b�g����K�v������ꍇ������i�O���b�h�Ȃǁj
	}

///	m_bChanged = FALSE;	// �ҏW�t���O������
}
/**
 * �R���g���[�����X�g�ƃt�B�[�`�����X�g�̊֘A���N���A����
 */
void CControlRel::ClearFeatureDefs()
{
	for( iterator i = begin(); i != end(); i++ ) 
		i->ClearFeatureDef();
}
/**
 * �R���g���[����o�^����i�P��t�B�[���h�p�j
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID			[in]			�o�^����R���g���[����ID
 * @param type			[in,optional]	�o�^����R���g���[���̃^�C�v
 * @param nGroupFrom	[in,optional]	�\������O���[�v�̊J�n�C���f�b�N�X
 * @param nGroupTo		[in,optional]	�\������O���[�v�̏I���C���f�b�N�X
 * @param lpszFieldName	[in,optional]	�R���g���[���������t�B�[���h�̖��O
 * @param uiCheckType	[in,optional]	�t�B�[���h�̒l�`�F�b�N�����鎞�̃^�C�v
 * @param lpszTableName	[in,optional]	�R���g���[���������t�B�[�`���̏�������e�[�u����
 * @param lpszOuterKey	[in,optional]	�֘A����t�B�[�`���̊O���L�[�p�t�B�[���h���i�O���e�[�u���������ꍇ�j
 * @param nFollowCtrlID	[in,optional]	�Ǐ]����R���g���[����ID
 * @param nFollowBy		[in,optional]	�Ǐ]����ꍇ�̃^�C�v
 * @param nRelType		[in,optional]	���g�p
 */
void CControlRel::SetControl(INT			nID, 
								  CONTROLTYPE	type, 
								  INT			nGroupFrom/* = 0*/,
								  INT			nGroupTo/* = 0*/,
								  LPCTSTR		lpszFieldName/* = NULL */, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType )
{
	CControlDef def;
	def.Clear();
	def.SetControlDef( m_cArcHelper.GetApp(), nID, m_hWnd, type, nGroupFrom, 
		nGroupTo, lpszFieldName, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );

	push_back(def);
}
/**
 * �R���g���[����o�^����i�����t�B�[���h�p�j
 *
 * @see CAttrDlg::CreateControlRelation()
 *
 * @param nID				[in]			�o�^����R���g���[����ID
 * @param type				[in]			�o�^����R���g���[���̃^�C�v
 * @param nGroupFrom		[in]			�\������O���[�v�̊J�n�C���f�b�N�X
 * @param nGroupTo			[in]			�\������O���[�v�̏I���C���f�b�N�X
 * @param arrayFieldNames	[in,optional]	�R���g���[���������t�B�[���h�̖��O���X�g
 * @param uiCheckType		[in,optional]	�t�B�[���h�̒l�`�F�b�N�����鎞�̃^�C�v
 * @param lpszTableName		[in,optional]	�R���g���[���������t�B�[�`���̏�������e�[�u����
 * @param lpszOuterKey		[in,optional]	�֘A����t�B�[�`���̊O���L�[�p�t�B�[���h���i�O���e�[�u���������ꍇ�j
 * @param nFollowCtrlID		[in,optional]	�Ǐ]����R���g���[����ID
 * @param nFollowBy			[in,optional]	�Ǐ]����ꍇ�̃^�C�v
 * @param nRelType			[in,optional]	���g�p
 */
void CControlRel::SetControl(INT			nID, 
								  CONTROLTYPE	     type, 
								  INT			     nGroupFrom,
								  INT			     nGroupTo,
								  std::list<CString> arrayFieldNames, 
								  UINT			     uiCheckType/* = NULL */,
								  LPCTSTR		     lpszTableName/* = NULL */, 
								  LPCTSTR		     lpszOuterKey/* = NULL */, 
								  INT			     nFollowCtrlID/* = NULL */,
								  UINT			     uiFollowBy/* = NULL */,
								  INT			     nRelType)
{
	CControlDef def;
	def.SetControlDef( m_cArcHelper.GetApp(), nID, m_hWnd, type, nGroupFrom, 
		nGroupTo, arrayFieldNames, uiCheckType, lpszTableName, lpszOuterKey, nFollowCtrlID, uiFollowBy, nRelType );

	push_back(def);
}
/**
 * HWND ����R���g���[��������o��
 *
 * @param hWnd	[in]	�T���o�������R���g���[���̃E�B���h�E�n���h��
 *
 * @return �R���g���[�����o�^����Ă���΂��̃R���g���[���̒�`�N���X�̃|�C���^���A�Ȃ���� NULL ��Ԃ�
 */
CControlDef* CControlRel::GetControlDef( HWND hWnd )
{
	HWND hCtrlWnd;

	for( iterator i = begin(); i != end(); i++ ) {
		hCtrlWnd = ::GetDlgItem( m_hWnd, i->m_nID );
		if( hWnd == hCtrlWnd )
			return &(*i);
	}
	
	return NULL;
}
/**
 * �O���[�v�P�ʂŃR���g���[���̕\���E��\����؂�ւ���
 *
 * @param nGroup	[in]			�\������O���[�v�i@see EditView �j
 * @param bForce	[in,optional]	�����I�ɐ��䂷�邩�ǂ����i�ҏW�J�n�A�I���̃n���h�����O���݂̂Ɏg�p�j
 * @param bEditable	[in,optional]	�����I�ɕҏW�E�s��ݒ肷��i�ҏW�J�n�A�I���̃n���h�����O���݂̂Ɏg�p�j
 */
void CControlRel::Visible( INT nGroup, BOOL bForce/* = FALSE*/, BOOL bEditable/* = TRUE */ )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nGroupFrom <= nGroup && i->m_nGroupTo >= nGroup ) {
			ChangeVisible( &(*i), TRUE );
		} else {
			ChangeVisible( &(*i), FALSE );
		}
		SetValue( &(*i), bForce, bEditable );
	}
}
/**
 * �R���g���[���ɕύX���������ꍇ�ɌĂ΂��֐�
 *
 * @param nID	[in]	�ύX�̂������R���g���[����ID
 */
void CControlRel::ControlChanged( INT nID )
{
	// �O�̒l�Ɣ�ׂĕύX����Ă����瑼�̃R���g���[���ɂ����f������
	LONG lVal = 0;
	CControlDef* def = NULL;
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			def = &(*i);
			break;
		}
	}

	if( !def )
		return;	// �R���g���[�����o�^����Ă��Ȃ�

	// �R���g���[������l���Z�b�g
	def->SetValueFromControl();

	for( iterator i = begin(); i != end(); i++ ) {
		if( ( i->m_nFollowCtrlID != NULL ) && ( def->m_nID == i->m_nFollowCtrlID ) ) {
			// �l���Z�b�g
			SetValue( &(*i) );
			ATLTRACE(_T("CControlRel::ControlChanged()�F�Ǐ]����l���ύX����܂��� %s\n"), i->GetFieldName() );
		}
	}
}
/**
 * �R���g���[���̉��E�s����Ԃ�ύX����
 *
 * @param def		[in]	���䂷�� CControlDef �|�C���^
 * @param bEnable	[in]	���ɂ���ꍇ�� TRUE�A�s���ɂ���ꍇ�� FALSE�i�f�t�H���g�FTRUE�j
 */
void CControlRel::ChangeVisible( CControlDef* def, BOOL bVisible/* = TRUE*/ )
{
	INT nCmdShow = ( bVisible == TRUE ? SW_SHOW : SW_HIDE );
	::ShowWindow( ::GetDlgItem( m_hWnd, def->m_nID ), nCmdShow );
}
/**
 * �^����ꂽ�R���g���[����`�N���X�ɒl���Z�b�g����
 *
 * @param def		[in]			�R���g���[����`�N���X
 * @param bForce	[in,optional]	�����I�ɐ��䂷�邩�ǂ����i�ҏW�J�n�A�I���̃n���h�����O���݂̂Ɏg�p�j
 * @param bEditable	[in,optional]	�����I�ɕҏW�E�s��ݒ肷��i�ҏW�J�n�A�I���̃n���h�����O���݂̂Ɏg�p�j
 */
void CControlRel::SetValue( CControlDef* def, BOOL bForce/* = FALSE*/, BOOL bEditable/* = TRUE */ )
{

	// �\����ԗp�`�F�b�N(�����l�Ƃ��ĂƂ肠�����A-1�i����`�j��
	sindyeDispOtherRelation nCheck = sindyeDispOtherNoDef;

	LONG lVal = 0;

	esriEditState state;
	m_cArcHelper.GetEditor()->get_EditState( &state );

	if( state == esriStateEditing )
	{
		// �\����Ԃ̕ύX
		if( def->m_uiFollowBy & FOLLOW_VALUE ) {
			GetValue( def->m_nFollowCtrlID, lVal );
			if( lVal == 0 ) 
				def->ChangeEnable( FALSE, bForce, bEditable );
			else
				def->ChangeEnable( TRUE, bForce, bEditable );
		}
		else if( def->m_uiFollowBy & FOLLOW_VALUEIS1 ){
			GetValue(def->m_nFollowCtrlID, lVal);
			if(lVal == 1 )
				def->ChangeEnable( TRUE, bForce, bEditable );
			else
				def->ChangeEnable( FALSE, bForce, bEditable );
		}
		else if( def->m_uiFollowBy & FOLLOW_VALUEREVERSE ){
			GetValue(def->m_nFollowCtrlID, lVal);
			if(lVal == 0 )
				def->ChangeEnable( TRUE, bForce, bEditable );
			else
				def->ChangeEnable( FALSE, bForce, bEditable );
		}
		// �ǂݍ��ݐ�p����
		else if( def->m_uiFollowBy & FOLLOW_READONLY ) {
			// [Bug 5509]FOLLOW_READONLY�������Ȃ��o�O�ŁA[Bug 5409]���C�������Ƃ��ɏo�Ă��Ă��܂����o�O
			// bForce��bEditable�̈Ӗ����Ȃ��Ȃ��Ă��邵�A���[�h�I�����[�Ƃ����Ă�񂾂��狭���I�ł����񂶂��
			def->ChangeEnable( FALSE, TRUE, FALSE );	
		}
		else {
			def->ChangeEnable( TRUE, bForce, bEditable );
		}
	}
	else {
		def->ChangeEnable( FALSE, bForce, bEditable );	
	}

	switch( def->m_type ) {
	case TYPE_EDIT:
	case TYPE_EDITSTRING:
	case TYPE_EDITFLOAT:
	case TYPE_EDITDOUBLE:
		def->SetEdit();
		break;
	case TYPE_CHECKBOX:
		def->SetCheckBox();
		break;
	case TYPE_COMBO:
		def->SetComboBox( GetDispClass() );
		break;
	case TYPE_LIST:
		break;
	case TYPE_FEATURESGRID:
		def->SetFeaturesGrid( TRUE );
		break;
	case TYPE_FEATUREGRID:
		def->SetFeatureGrid( FALSE );
		break;
	case TYPE_OTHER:
	case TYPE_BUTTON:
		def->SetOtherControl();
		break;
	case TYPE_TIMEREG:
	case TYPE_OWTIMEREG:
		def->SetLinkTimeRegGrid();
		break;
	case TYPE_OUTERROWS:
		def->SetOuterRowsGrid();
		break;
	default:
		break;
	}
}
/**
 * �n���ꂽID�����R���g���[����`�N���X�̒l�� LONG �l�ŕԂ�
 *
 * ���݃R���g���[���ɕ\������Ă���l�ł͂Ȃ��ACFieldDef::m_vaValue �ɂȂ�܂�
 *
 * @param nID	[in]	�l���擾�������R���g���[����ID
 * @param lVal	[out]	�擾�����l
 */
BOOL CControlRel::GetValue( INT nID, LONG& lVal )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( nID == i->m_nID ) {
			CComVariant vaValue;
			i->GetOldValue( vaValue );
			lVal = vaValue.lVal;	// LONG�l����
	
			return TRUE;
		}
	}

	return FALSE;
}
/**
 * ROADCLASS_C �̒l���擾����
 *
 * ROADCLASS_C �t�B�[���h����`���ꂽ�����o�����݂���΁A����̒l��Ԃ��܂�
 *
 * @return ROADCLASS_C �̒l�A�o�^����Ă��Ȃ���� -1 ��Ԃ�
 */
LONG CControlRel::GetDispClass()
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( lstrcmpi( i->GetFieldName(), _T("ROADCLASS_C") ) == 0 ) {
			CComVariant vaValue;
			i->GetOldValue( vaValue );
			
			if( vaValue.vt == VT_NULL )
				return -1;
			else
				return vaValue.lVal;		// LONG�l����
		}
	}
	return -1;
}
/**
 * �ʏ�̏����ł͏����ł��Ȃ��R���g���[���͂����ŏ���
 *
 * @param l1			[in]	�t���p�����[�^
 * @param l2			[in]	�t���p�����[�^
 *
 * @return �ҏW�\�ȏꍇ�� TRUE�A�ҏW�s�\�ȏꍇ�� FALSE
 */
BOOL CControlRel::ClickOtherControl( INT nID, LONG l1, LONG l2, BOOL bIsGridEditMode )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			return i->Click( l1, l2, bIsGridEditMode );
		}
	}

	return FALSE;
}
BOOL CControlRel::IsClickCelEditable( INT nID, LONG l1, LONG l2 )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			return i->IsClickCelEditable( l1, l2 );
		}
	}

	return FALSE;
}
/**
 * �ʏ�̏����ł͏����ł��Ȃ��R���g���[���͂����ŏ���
 *
 * @param l1			[in]	�t���p�����[�^
 * @param l2			[in]	�t���p�����[�^
 */
void CControlRel::EditedOtherControl( INT nID, LONG l1, LONG l2 )
{
	for( iterator i = begin(); i != end(); i++ ) {
		if( i->m_nID == nID ) {
			i->Edited( l1, l2 );
			break;
		}
	}
}
/**
 * �����o�̃_���v������
 */
void CControlRel::dump()
{
	for( iterator i = begin(); i != end(); i++ ) {
		i->dump( _T("CControlRel::SetValue()") );
	}
}

void CControlRel::SetAliasOrField(BOOL bAliasOrField)
{
	for( iterator def = begin(); def != end(); def++ ) {
		def->m_bAliasOrField = bAliasOrField;
	}
}

BOOL CControlRel::CheckError()
{
	BOOL		bIsError = FALSE;
	for( iterator def = begin(); def != end(); def++) {
		if (def->Errored()) {
			bIsError = TRUE;
		}
	}
	return bIsError;
}

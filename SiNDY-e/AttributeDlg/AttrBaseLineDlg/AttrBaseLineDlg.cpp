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

// AttrBaseLineDlg.cpp: CAttrBaseLineDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseLineDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���k���C���p�j
BEGIN_WINDOW_MAP(BASE_LINE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_DISPSCALE1_F)
				RCTOFIT(-1)
				RCTOFIT(IDC_CHECK_DISPSCALE2_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_DISPSCALE3_F)
				RCTOFIT(-1)
				RCTOFIT(IDC_CHECK_DISPSCALE4_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(-1)
				RCTOFIT(IDC_CHECK_UNDERGROUND_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_BGCLASS_C)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SOURCE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_SOURCE)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrBaseLineDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseLineDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseLineDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseLineDlg::CAttrBaseLineDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrBaseLineDlg::CAttrBaseLineDlg()
	: ATTR_BASE_CLASS(BASE_LINE_MAP, IDD_ATTRBASELINEDLG),
		m_wnd0( _T("CHECKBOX"), this, IDC_CHECK_DISPSCALE1_F ), 
		m_wnd1( _T("CHECKBOX"), this, IDC_CHECK_DISPSCALE2_F ), 
		m_wnd2( _T("CHECKBOX"), this, IDC_CHECK_DISPSCALE3_F ), 
		m_wnd3( _T("CHECKBOX"), this, IDC_CHECK_DISPSCALE4_F ), 
		m_wnd4( _T("CHECKBOX"), this, IDC_CHECK_UNDERGROUND_F ), 
		m_wnd5( _T("COMBOBOX"), this, IDC_COMBO_BGCLASS_C ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrBaseLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBaseLineDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseLineDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBaseLineDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrBaseLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrBaseLineDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_BGCLASS_C) );
	}

    return bRet;
}

/**
 * @fn void CAttrBaseLineDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBaseLineDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// ���k���C���p

		// �X�P�[���P�\���t���O
		m_cControlRel.SetControl( IDC_CHECK_DISPSCALE1_F,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE1_F"),	NULL, BASELINE_TABLE_NAME );
		// �X�P�[���Q�\���t���O
		m_cControlRel.SetControl( IDC_CHECK_DISPSCALE2_F,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE2_F"),	NULL, BASELINE_TABLE_NAME );
		// �X�P�[���R�\���t���O
		m_cControlRel.SetControl( IDC_CHECK_DISPSCALE3_F,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE3_F"),	NULL, BASELINE_TABLE_NAME );
		// �X�P�[���S�\���t���O
		m_cControlRel.SetControl( IDC_CHECK_DISPSCALE4_F,	TYPE_CHECKBOX,	0,	0,	_T("DISPSCALE4_F"),	NULL, BASELINE_TABLE_NAME );
		// �n���t���O
		m_cControlRel.SetControl( IDC_CHECK_UNDERGROUND_F,	TYPE_CHECKBOX,	0,	0,	_T("UNDERGROUND_F"),NULL, BASELINE_TABLE_NAME );
		// �w�i���C����ʃt���O
		m_cControlRel.SetControl( IDC_COMBO_BGCLASS_C,		TYPE_COMBO,		0,	0,	_T("BGCLASS_C"),	NULL, BASELINE_TABLE_NAME );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, BASELINE_TABLE_NAME );
	}
}

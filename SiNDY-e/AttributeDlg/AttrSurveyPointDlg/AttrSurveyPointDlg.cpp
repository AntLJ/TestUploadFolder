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

// AttrSurveyPointDlg.cpp : CAttrSurveyPointDlg �̃C���v�������e�[�V����

#include "stdafx.h"
#include "AttrSurveyPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���n�����|�C���g�p�j
BEGIN_WINDOW_MAP(SURVEYPOINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NODEID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NODEID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MASTERID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_MASTERID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_MESHCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_MESHCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_LOTNO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_LOTNO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_WORKYEAR)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_WORKYEAR)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_WORKSEASON)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_WORKSEASON)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SURVEYLV)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SURVEYLV)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_PRIMID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_PRIMID)
			ENDGROUP()
			RCSPACE(20)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_DUMMY)
				RCTOFIT(-1)
				RCREST(IDC_BUTTON_DISPLAYPICTURE)
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
 * @retval CAttrSurveyPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrSurveyPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSurveyPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrSurveyPointDlg::CAttrSurveyPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrSurveyPointDlg::CAttrSurveyPointDlg() 
	: ATTR_BASE_CLASS(SURVEYPOINT_MAP, IDD_ATTRSURVEYPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_NODEID ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_MASTERID ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_MESHCODE ),
		m_wnd3( _T("EDIT"), this, IDC_EDIT_LOTNO ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_WORKYEAR ),
		m_wnd5( _T("COMBO"), this, IDC_COMBO_WORKSEASON ),
		m_wnd6( _T("COMBO"), this, IDC_COMBO_SURVEYLV ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_PRIMID ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd9( _T("BUTTON"), this, IDC_BUTTON_DISPLAYPICTURE )
{
}

/** 
 * @fn HWND CAttrSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrSurveyPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrSurveyPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrSurveyPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrSurveyPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_EDIT_NODEID) );
	}

    return bRet;
}

/**
 * @fn void CAttrBaseAdminDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrSurveyPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���n�����|�C���g�p

		// �m�[�hID
		m_cControlRel.SetControl( IDC_EDIT_NODEID,		TYPE_EDIT,		0,	0,  _T("NODEID"),		NULL, SURVEY_POINT_TABLE_NAME );
		// �������[ID
		m_cControlRel.SetControl( IDC_EDIT_MASTERID,	TYPE_EDIT,		0,	0,  _T("MASTERID"),	NULL, SURVEY_POINT_TABLE_NAME );
		// ���b�V���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_MESHCODE,	TYPE_EDIT,		0,	0,  _T("MESHCODE"),	NULL, SURVEY_POINT_TABLE_NAME );
		// ���b�gNO
		m_cControlRel.SetControl( IDC_EDIT_LOTNO,		TYPE_EDIT,		0,	0,  _T("LOTNO"),		NULL, SURVEY_POINT_TABLE_NAME );
		// ��ƔN�x
		m_cControlRel.SetControl( IDC_EDIT_WORKYEAR,	TYPE_EDIT,		0,	0,  _T("WORKYEAR"),	NULL, SURVEY_POINT_TABLE_NAME );
		// ��Ǝ���
		m_cControlRel.SetControl( IDC_COMBO_WORKSEASON,	TYPE_COMBO,		0,	0,  _T("WORKSEASON"),	NULL, SURVEY_POINT_TABLE_NAME );
		// �������x��
		m_cControlRel.SetControl( IDC_COMBO_SURVEYLV,	TYPE_COMBO,		0,	0,  _T("SURVEYLV"),	NULL, SURVEY_POINT_TABLE_NAME );
		// �v���~�e�B�uID
		m_cControlRel.SetControl( IDC_EDIT_PRIMID,		TYPE_EDIT,		0,	0,  _T("TMP_PRIMID"),	NULL, SURVEY_POINT_TABLE_NAME );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,0,	0,  _T("SOURCE"),		NULL, SURVEY_POINT_TABLE_NAME );
	}
}

void CAttrSurveyPointDlg::GetRegCode()
{
 	CString strRegOpenKey( _T("SOFTWARE\\INCREMENT P CORPORATION\\SiNDY-e\\TOOLS\\ATTRIBUTETOOL" ) );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		TCHAR strToolPath[255];

		if( EmxGetRegString( _T("SurveyToolPath"), strToolPath, 255, hOpenKey ) )
			m_strToolPath = strToolPath;

		RegCloseKey( hOpenKey );
	}
}

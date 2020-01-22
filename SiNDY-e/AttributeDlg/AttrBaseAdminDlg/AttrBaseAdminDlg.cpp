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

// AttrBaseAdminDlg.cpp: CAttrBaseAdminDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseAdminDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���k�s���E�p�j
BEGIN_WINDOW_MAP(BASE_ADMIN_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CITYCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CITYCODE)
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
 * @retval CAttrBaseAdminDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseAdminDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseAdminDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseAdminDlg::CAttrBaseAdminDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrBaseAdminDlg::CAttrBaseAdminDlg()
	: ATTR_BASE_CLASS(BASE_ADMIN_MAP, IDD_ATTRBASEADMINDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_CITYCODE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrBaseAdminDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBaseAdminDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseAdminDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBaseAdminDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrBaseAdminDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBaseAdminDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_EDIT_CITYCODE) );
	}

    return bRet;
}

/**
 * @fn void CAttrBaseAdminDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBaseAdminDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���k�s���E�p

		// �s�撬���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_CITYCODE,	TYPE_EDITSTRING, 	0,	0,	_T("CITYCODE"),	NULL, BASE_ADMIN_TABLE_NAME );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),	NULL, BASE_ADMIN_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrBaseAdminDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * �_�C�A���O�����ۂɁAAttributeDlgObj����Ă΂��
 * �s�撬���R�[�h��00000�`47999�̊Ԃɂ��邩�`�F�b�N����
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrBaseAdminDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == BASE_ADMIN_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			LONG		lCityCode, lOID;
			CComBSTR	bstrCityCode;
			CString		strDummy;

			// �s�撬���R�[�h�擾
			pFieldDef = it->GetFieldDef( BASE_ADMIN_TABLE_NAME, _T("CITYCODE") );
			bstrCityCode = pFieldDef->m_vaValue.bstrVal;
			strDummy = bstrCityCode.m_str;
			lCityCode = _ttol( strDummy );

			it->GetFeature()->get_OID(&lOID);

			// �s�撬���R�[�h�̌����`�F�b�N
			if( bstrCityCode.Length() != 5 )
			{
				AttrMessageBox( AheLoadString( IDS_FIVEFIGURES ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
				return FALSE;
			}

			if( lCityCode > 47999 )
			{
				CString strMessage;
				strMessage.Format(AheLoadString( IDS_RANGEOFCITYCODE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				return FALSE;	
			}
		}
	}

	return TRUE;
}

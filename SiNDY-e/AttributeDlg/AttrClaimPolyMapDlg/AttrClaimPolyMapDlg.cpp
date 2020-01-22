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

// AttrClaimPolyMapDlg.cpp: CAttrClaimPolyMapDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrClaimPolyMapDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�N���[���|���S���p�j
BEGIN_WINDOW_MAP(CLAIMPOLY_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_TOFIT,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CLAIMTYPE)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_CLAIMTYPE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CLAIMNO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CLAIMNO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_COMFIRM)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_USELESS)
			ENDGROUP()
			RCSPACE(10)
		ENDGROUP()
		BEGINROWS(WRCT_REST, 0, RCMARGINS(4,4))
			RCTOFIT(IDC_STATIC_REMARKS)
			RCTOFIT(-1)
			RCREST(IDC_EDIT_REMARKS)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrClaimPolyMapDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrClaimPolyMapDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrClaimPolyMapDlg::CAttrClaimPolyMapDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrClaimPolyMapDlg::CAttrClaimPolyMapDlg() 
	: ATTR_BASE_CLASS(CLAIMPOLY_MAP, IDD_ATTRCLAIMPOLYMAPDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_CLAIMTYPE ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_CLAIMNO ),
		m_wnd2( _T("CHECKBOX"), this, IDC_CHECK_COMFIRM ),
		m_wnd3( _T("CHECKBOX"), this, IDC_CHECK_USELESS ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_REMARKS )
{
}

/** 
 * @fn HWND CAttrClaimPolyMapDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrClaimPolyMapDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrClaimPolyMapDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrClaimPolyMapDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrClaimPolyMapDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrClaimPolyMapDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_COMBO_CLAIMTYPE) );
	}

    return bRet;
}

/**
 * @fn void CAttrClaimPolyMapDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrClaimPolyMapDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �N���[���|���S���Z��

		// �N���[���^�C�v
		m_cControlRel.SetControl( IDC_COMBO_CLAIMTYPE,	TYPE_COMBO,		0,	0,	_T("CLAIMTYPE_C"),	NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// �N���[��NO
		m_cControlRel.SetControl( IDC_EDIT_CLAIMNO,		TYPE_EDITSTRING,0,	0,	_T("CLAIM_NO"),		NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// �펞�m�F�t���O
		m_cControlRel.SetControl( IDC_CHECK_COMFIRM,	TYPE_CHECKBOX,	0,	0,	_T("COMFIRM_F"),	NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// �s�p�t���O
		m_cControlRel.SetControl( IDC_CHECK_USELESS,	TYPE_CHECKBOX,	0,	0,	_T("USELESS_F"),	NULL, CLAIMPOLY_MAP_TABLE_NAME );
		// �R�����g
		m_cControlRel.SetControl( IDC_EDIT_REMARKS,		TYPE_EDITSTRING,0,	0,	_T("REMARKS"),		NULL, CLAIMPOLY_MAP_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrClaimPolyMapDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɓ��H�����N�̃G���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CAttrClaimPolyMapDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == CLAIMPOLY_ADDR_TABLE_NAME || strTableName == CLAIMPOLY_MAP_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaClaimType;
			if( strTableName == CLAIMPOLY_ADDR_TABLE_NAME )
				pFieldDef = it->GetFieldDef( CLAIMPOLY_ADDR_TABLE_NAME, _T("CLAIMTYPE_C") );
			else
				pFieldDef = it->GetFieldDef( CLAIMPOLY_MAP_TABLE_NAME, _T("CLAIMTYPE_C") );

			vaClaimType = pFieldDef->m_vaValue;

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			if( vaClaimType.lVal == 0 )
			{
				strMessage.Format(AheLoadString( IDS_NOTCLAIMTYPE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

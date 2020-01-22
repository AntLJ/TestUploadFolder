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

// AttrCSAddrPointDlg.cpp: CAttrCSAddrPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCSAddrPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�m�[�h�p�j
BEGIN_WINDOW_MAP(CSADDRPOINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_PCT,0,90)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_CSADDRPOINT)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_CSADDRPOINT)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GOUNO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GOUNO)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_PCT, 0, 10)
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
 * @retval CAttrCSAddrPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCSAddrPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSAddrPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCSAddrPointDlg::CAttrCSAddrPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCSAddrPointDlg::CAttrCSAddrPointDlg()
	: ATTR_BASE_CLASS(CSADDRPOINT_MAP, IDD_ATTRCSADDRPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_EDIT_CSADDRPOINT ), 
		m_wnd1( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd2( _T("EDIT"), this, IDC_EDIT_GOUNO )
{
}

/** 
 * @fn HWND CAttrCSAddrPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCSAddrPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCSAddrPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCSAddrPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCSAddrPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCSAddrPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	//[Bug 7485]�g�[��Ɨp�|�C���gRefS_Point�p�̃_�C�A���O���~�����ɑΉ�
	// ��U���ԍ��́ADISALBE��
	SetDlgStatus( IDC_EDIT_GOUNO, FALSE, TRUE );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_EDIT_CSADDRPOINT) );
		// �ҏW�J�n����REFS_POINT�Ȃ�A���ԍ��ҏW�\��
		if( m_strFeatureClassName == _T("REFS_POINT") )
			SetDlgStatus( IDC_EDIT_GOUNO, TRUE, TRUE );
	}

    return bRet;
}

/**
 * @fn void CAttrCSAddrPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCSAddrPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// CS�Z���|�C���g�p

		//[Bug 7485]�g�[��Ɨp�|�C���gRefS_Point�p�̃_�C�A���O���~�����ɑΉ�
		// ���ԍ�
		if( m_strFeatureClassName == _T("REFS_POINT") )
			m_cControlRel.SetControl( IDC_EDIT_GOUNO,	TYPE_EDITSTRING,	0,	0,	_T("GOU_NO"),	NULL, m_strFeatureClassName );

		// 20���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_CSADDRPOINT,	TYPE_EDITSTRING,	0,	0,	_T("ADDRCODE"),	NULL, m_strFeatureClassName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,		TYPE_EDITSTRING,	0,	0,	_T("SOURCE"),	NULL, m_strFeatureClassName );
	}
}

/**
 * @fn BOOL CAttrCSAddrPointDlg::ErrorCheck()
 * @brief �_�C�A���O��������Ƃ��ɓ��H�����N�̃G���[�`�F�b�N���s��
 *
 * @return �G���[�łȂ������� TRUE�A�G���[�������� FALSE
 */
BOOL CAttrCSAddrPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == CS_ADDRPOINT_TABLE_NAME )
		{
			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaAddrCode;
			pFieldDef = it->GetFieldDef( CS_ADDRPOINT_TABLE_NAME, _T("ADDRCODE") );

			CString strMessage;
			LONG lOID = 0;
			it->GetFeature()->get_OID( &lOID );

			CString strAddrCode = pFieldDef->value2string(FALSE);
			LONG lAddrCodeLong = strAddrCode.GetLength();

			if( lAddrCodeLong != 20 )
			{
				strMessage.Format( AheLoadString( IDS_NOT20FIGURES ),lOID );
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

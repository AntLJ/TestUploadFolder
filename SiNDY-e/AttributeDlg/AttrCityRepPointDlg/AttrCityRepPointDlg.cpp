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

// AttrCityRepPointDlg.cpp: CAttrCityRepPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCityRepPointDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�s�s�n�}��\�_�p�j
BEGIN_WINDOW_MAP(BASE_REP_POINT_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BASE_REP_POINT_STATIC1)
				RCTOFIT(-1)
				RCREST(IDC_BASE_REP_POINT_EDIT1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BASE_REP_POINT_STATIC2)
				RCTOFIT(-1)
				RCREST(IDC_BASE_REP_POINT_EDIT2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCREST(IDC_CHECK_ANNO_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_BASE_REP_POINT_STATIC3)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_POINTCLASS_C)
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
 * @retval CAttrCityRepPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityRepPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCityRepPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCityRepPointDlg::CAttrCityRepPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCityRepPointDlg::CAttrCityRepPointDlg()
	: ATTR_BASE_CLASS(BASE_REP_POINT_MAP, IDD_ATTRCITYREPPOINTDLG),
		m_wnd0( _T("EDIT"), this, IDC_BASE_REP_POINT_EDIT1 ),
		m_wnd1( _T("EDIT"),	this, IDC_BASE_REP_POINT_EDIT2 ), 
		m_wnd2( _T("CHECK"), this, IDC_CHECK_ANNO_F ),
		m_wnd3( _T("COMBO"), this, IDC_COMBO_POINTCLASS_C ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrCityRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCityRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCityRepPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCityRepPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCityRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCityRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		::SetFocus( GetDlgItem(IDC_BASE_REP_POINT_EDIT1) );
	}

    return bRet;
}

/**
 * @fn void CAttrCityRepPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCityRepPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �s�s�n�}��\�_�p

		// 11���Z���R�[�h
		m_cControlRel.SetControl( IDC_BASE_REP_POINT_EDIT1,		TYPE_EDITSTRING,0,	0,	_T("ADDRCODE"),		NULL, CITY_REP_POINT_TABLE_NAME );
		// �n��
		m_cControlRel.SetControl( IDC_BASE_REP_POINT_EDIT2,		TYPE_EDIT,		0,	0,	_T("CHIBAN_NO"),	NULL, CITY_REP_POINT_TABLE_NAME );
		// �n�Ԓ��L�t���O
		m_cControlRel.SetControl( IDC_CHECK_ANNO_F,				TYPE_CHECKBOX,	0,	0,	_T("ANNO_F"),		NULL, CITY_REP_POINT_TABLE_NAME );
		// �n��
		m_cControlRel.SetControl( IDC_COMBO_POINTCLASS_C,		TYPE_COMBO,		0,	0,	_T("POINTCLASS_C"),	NULL, CITY_REP_POINT_TABLE_NAME );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,				TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, CITY_REP_POINT_TABLE_NAME );
	}
}

/**
 * @fn BOOL CAttrCityRepPointDlg::ErrorCheck()
 * @brief �G���[���Ȃ����ǂ������ׂ�
 *
 * @return	�G���[���Ȃ�������TRUE�A�G���[�������FALSE
 */
BOOL CAttrCityRepPointDlg::ErrorCheck()
{
	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == BASE_REP_POINT_TABLE_NAME )
		{

			// ���ݕҏW���̒l���K��
			CFieldDef* pFieldDef;
			CComVariant vaAddrCode, vaAdminName;
			pFieldDef = it->GetFieldDef( BASE_REP_POINT_TABLE_NAME, _T("ADDRCODE") );
			vaAddrCode = pFieldDef->m_vaValue;

			USES_CONVERSION;

			CString strMessage;
			LONG lOID;
			it->GetFeature()->get_OID( &lOID );

			CString strAddrCode;
			strAddrCode.Format( _T("%s"), OLE2T(vaAddrCode.bstrVal) );

			if( strAddrCode.GetLength() != 11 )
			{
				strMessage.Format( AheLoadString( IDS_ADDRCODE11 ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}

			if( strAddrCode.Find( _T(" ") ) != -1 )
			{
				strMessage.Format( AheLoadString( IDS_NOTNEEDSPACE ),lOID);
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, NULL );
				return FALSE;
			}
		}
	}

	return TRUE;
}

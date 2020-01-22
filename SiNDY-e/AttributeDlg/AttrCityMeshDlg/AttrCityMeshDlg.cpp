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

// AttrCitySiteDlg.cpp: CAttrCitySiteDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrCityMeshDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�s�s�n�}���b�V���p�j
BEGIN_WINDOW_MAP(CITYMESH_MAP)
	BEGINROWS(WRCT_TOFIT,0,0)
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC1)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT1)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC2)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT2)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC3)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT3)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC4)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT4)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(-1)
			RCTOFIT(IDC_CITYMESH_CHECK1)
		ENDGROUP()
		RCSPACE(10)
		BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
			RCTOFIT(IDC_CITYMESH_STATIC5)
			RCTOFIT(-1)
			RCREST(IDC_CITYMESH_EDIT5)
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()


/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrCityMeshDlg��Ԃ��܂��B
 */
 extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrCityMeshDlg();
}

/** 
 * @fn CAttrCityMeshDlg::CAttrCityMeshDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCityMeshDlg::CAttrCityMeshDlg()
	: ATTR_BASE_CLASS(CITYMESH_MAP, IDD_ATTRCITYMESHDLG),
		m_wnd0( _T("EDIT"), this, IDC_CITYMESH_EDIT1 ),
		m_wnd1( _T("EDIT"), this, IDC_CITYMESH_EDIT2 ), 
		m_wnd2( _T("EDIT"), this, IDC_CITYMESH_EDIT3 ), 
		m_wnd3( _T("EDIT"), this, IDC_CITYMESH_EDIT4 ), 
		m_wnd4( _T("CHECKBOX"), this, IDC_CITYMESH_CHECK1 ), 
		m_wnd5( _T("EDIT"), this, IDC_CITYMESH_EDIT5 )
{
}

/** 
 * @fn HWND CAttrCityMeshDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCityMeshDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrCityMeshDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCityMeshDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCityMeshDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCityMeshDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

    return bRet;
}

/**
 * @fn void CAttrCityMeshDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B�e�R���g���[���ɒl��ݒ肵���肵�܂��B
 */
void CAttrCityMeshDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// �s�s�n�}���b�V���p

		// �s�s�n�}���b�V���R�[�h
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT1,	TYPE_EDIT,			0,	0,	_T("MESHCODE"),				NULL, CITYMESH_TABLE_NAME );
		// �V�K�쐬�N�x
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT2,	TYPE_EDITSTRING,	0,	0,	_T("CREATE_YEAR"),				NULL, CITYMESH_TABLE_NAME );
		// �����C���N��
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT3,	TYPE_EDITSTRING,	0,	0,	_T("MODIFY_YEAR"),			NULL, CITYMESH_TABLE_NAME );
		// ���L�����N��
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT4,	TYPE_EDITSTRING,	0,	0,	_T("ANNO_SURVEY_YEAR"),			NULL, CITYMESH_TABLE_NAME );
		// ���S���b�V���t���O
		m_cControlRel.SetControl( IDC_CITYMESH_CHECK1,	TYPE_CHECKBOX,		0,	0,	_T("COMPLETE_F"),			NULL, CITYMESH_TABLE_NAME );
		// �K�����n�����N�x
		m_cControlRel.SetControl( IDC_CITYMESH_EDIT5,	TYPE_EDITSTRING,	0,	0,	_T("TR_SURVEY_YEAR"),	NULL, CITYMESH_TABLE_NAME );
	}
}

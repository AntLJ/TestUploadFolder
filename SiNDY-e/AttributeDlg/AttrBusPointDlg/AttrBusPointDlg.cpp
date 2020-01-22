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

//////////////////////////////////////////////////////////////////////
//
// AttrBusPointDlg.cpp: CAttrBusPointDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBusPointDlg.h"
#include "AheLayerFunctions.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "DrawFunctions.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(BUSPOINTMAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_STOP_ID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_STOP_ID)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_POINT_ATTR_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_ATTR)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_POLE_NO)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_POLE_NO)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_YOMI)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_YOMI)
			ENDGROUP()
		ENDGROUP()
		BEGINROWS(WRCT_TOFIT, 0, RCMARGINS(4,4))
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(0,0))
				RCREST(IDC_STATIC_VAR)
			ENDGROUP()
			RCSPACE(5)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(ID_STATIC_SOURCE)
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
 * @retval CAttrBusPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBusPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBusPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBusPointDlg::CAttrBusPointDlg()
 * @brief �R���X�g���N�^
 *
 */
CAttrBusPointDlg::CAttrBusPointDlg() 
	: ATTR_BASE_CLASS(BUSPOINTMAP, IDD_ATTRBUSPOINTDLG),
		m_wnd0( _T( "EDIT" ), this, IDC_EDIT_STOP_ID ),
		m_wnd1( _T( "COMBOBOX" ), this, IDC_COMBO_ATTR ),
		m_wnd2( _T( "EDIT" ), this, IDC_EDIT_POLE_NO ),
		m_wnd3( _T( "EDIT" ), this, IDC_EDIT_NAME ),
		m_wnd4( _T( "EDIT" ), this, IDC_EDIT_YOMI ),
		m_wnd5( _T( "EDIT" ), this, IDC_EDIT_SOURCE )
{
}

/** 
 * @fn HWND CAttrBusPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBusPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create( hWndParent, dwInitParam );
	return m_hChildWnd;
}

/**
 * @fn void CAttrBusPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBusPointDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow( m_hChildWnd );
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrBusPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */

BOOL CAttrBusPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );
	
	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		SetIMEMode( FALSE );
		::SetFocus( GetDlgItem( IDC_EDIT_STOP_ID ) );
	}

    return bRet;
}

/**
 * @fn void CAttrBusPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBusPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() );

	if( !m_cControlRel.empty() )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �◯���R�[�h
		m_cControlRel.SetControl( IDC_EDIT_STOP_ID, TYPE_EDIT, 0, 0, _T( "Stop_ID" ), NULL, _T( "BUS_POINT" )  );
		// ���
		m_cControlRel.SetControl( IDC_COMBO_ATTR, TYPE_COMBO, 0, 0, _T( "Point_Attr_C" ), NULL, _T( "BUS_POINT" ) );
		// �W���ԍ�
		m_cControlRel.SetControl( IDC_EDIT_POLE_NO, TYPE_EDIT, 0, 0, _T( "Pole_No" ), NULL, _T( "BUS_POINT" ) );
		// �◯����
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, _T( "Stop_Name_Kanji" ), NULL, _T( "BUS_POINT" ) );
		// �◯�����
		m_cControlRel.SetControl( IDC_EDIT_YOMI, TYPE_EDITSTRING, 0, 0, _T( "Stop_Name_Yomi" ), NULL, _T( "BUS_POINT" ) );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, _T( "SOURCE" ), NULL, _T( "BUS_POINT" ) );
	}
}



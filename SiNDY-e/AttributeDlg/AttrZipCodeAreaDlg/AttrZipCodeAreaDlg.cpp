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

// AttrZipcodePolygonDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrZipCodeAreaDlg.h"
#include <boost/assign/list_of.hpp>
#include <regex>

using namespace sindy::schema;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(ZIPCODEAREA_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE1)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE2)
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
 * @retval CAttrZipcodePolygonDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrZipCodeAreaDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrZipcodePolygonDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrZipcodePolygonDlg::CAttrZipcodePolygonDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrZipCodeAreaDlg::CAttrZipCodeAreaDlg() 
	: ATTR_BASE_CLASS(ZIPCODEAREA_MAP, IDD_ZIPCODEDLG)
{
	m_mapSubClass = boost::assign::map_list_of
		(IDC_EDIT_ZIPCODE1, CContainedWindow(_T("EDIT"), this, IDC_EDIT_ZIPCODE1))
		(IDC_EDIT_ZIPCODE2, CContainedWindow(_T("EDIT"), this, IDC_EDIT_ZIPCODE2))
		(IDC_EDIT_SOURCE, CContainedWindow(_T("EDIT"), this, IDC_EDIT_SOURCE));
}

/** 
 * @fn HWND CAttrZipcodePolygonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrZipCodeAreaDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrZipcodePolygonDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrZipCodeAreaDlg::Delete()
{
	ClearFeatureDefs();
	if (IsWindow())
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrZipcodePolygonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * �܂��A���e�ԍ��̒l�����������āA���[������擾���Ă���
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */
BOOL CAttrZipCodeAreaDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex(lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable);

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if (IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable)
	{
		::SetFocus(GetDlgItem(IDC_EDIT_ZIPCODE1));
	}

	return bRet;
}

/**
 * @fn void CAttrZipcodePolygonDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrZipCodeAreaDlg::CreateControlRelation()
{
	ATLASSERT(IsWindow() == TRUE);

	if (m_cControlRel.size() > 0)
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND(m_hWnd);
	}
	else
	{
		m_cControlRel.SetHWND(m_hWnd);

		// �X�֔ԍ�1
		m_cControlRel.SetControl(
			IDC_EDIT_ZIPCODE1, TYPE_EDITSTRING, 0, 0,
			zipcode_area::kZipCode1, 0, zipcode_area::kTableName);
		// �X�֔ԍ�2
		m_cControlRel.SetControl(
			IDC_EDIT_ZIPCODE2, TYPE_EDITSTRING, 0, 0,
			zipcode_area::kZipCode2, 0, zipcode_area::kTableName);
		// ���\�[�X
		m_cControlRel.SetControl(
			IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0,
			ipc_table::kSource, 0, zipcode_area::kTableName);
	}
}

BOOL CAttrZipCodeAreaDlg::ErrorCheck()
{
	// 7���̐������ǂ������`�F�b�N����
	// �X�֔ԍ��Ƃ��ēK�؂��ǂ����͕ʓr�`�F�b�N�c�[���őΉ�
	std::regex re("[0-9]{7}");
	std::smatch match;

	CString zipCode1;
	GetDlgItem(IDC_EDIT_ZIPCODE1).GetWindowText(zipCode1);
	if (zipCode1.IsEmpty())
	{
		// zipcode1��NULL�̓G���[
		MessageBox(AheLoadString(IDS_ERR_ZIPCODE1_NULL), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	std::string s = (CStringA)zipCode1;
	if (!std::regex_match(s, match, re))
	{
		MessageBox(AheLoadString(IDS_ERR_ZIPCODE1_DIGIT), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	CString zipCode2;
	GetDlgItem(IDC_EDIT_ZIPCODE2).GetWindowText(zipCode2);
	if (zipCode2.IsEmpty())
	{
		// zipcode2��NULL��OK
		return TRUE;
	}

	std::string s2 = (CStringA)zipCode2;
	if (!std::regex_match(s2, match, re))
	{
		MessageBox(AheLoadString(IDS_ERR_ZIPCODE2_DIGIT), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	// zipcode1��zipcode2�������l�ɂȂ��Ă���̂�NG
	if (0 == zipCode1.CompareNoCase(zipCode2))
	{
		MessageBox(AheLoadString(IDS_ERR_SAME_ZIPCODE), AheLoadString(IDS_CAPTION), MB_OK);
		return FALSE;
	}

	return TRUE;
}

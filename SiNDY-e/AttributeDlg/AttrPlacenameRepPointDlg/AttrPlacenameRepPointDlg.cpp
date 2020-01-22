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

// AttrPlacenameRepPointDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrPlacenameRepPointDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

// �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(CSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ELEVENCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ELEVENCODE)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_NAME)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_NAME)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_ZIPCODE)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_ZIPCODE)
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
 * @retval CAttrPlacenameRepPointDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrPlacenameRepPointDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrPlacenameRepPointDlg::CAttrPlacenameRepPointDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrPlacenameRepPointDlg::CAttrPlacenameRepPointDlg() 
	: ATTR_BASE_CLASS(CSITE_MAP, IDD_PLACENAMEDLG), m_isFirst(false)
{
	// �e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_ELEVENCODE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ELEVENCODE ) )
		( IDC_EDIT_NAME, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_NAME ) )
		( IDC_EDIT_ZIPCODE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_ZIPCODE ) )
		( IDC_EDIT_SOURCE, CContainedWindow(  _T("EDIT"), this, IDC_EDIT_SOURCE ) );
}

/** 
 * @fn HWND CAttrPlacenameRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrPlacenameRepPointDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrPlacenameRepPointDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrPlacenameRepPointDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrPlacenameRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrPlacenameRepPointDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	// �����Ńt���O�𗧂Ă�Ƃ��܂������̂ŁA�Ƃ肠���������ŗ��Ă�
	m_isFirst=true;

	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_EDIT_ELEVENCODE) );
	}

	// �s�撬���R�[�h+�Z���R�[�h��11���Z���R�[�h�𐶐�
	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( pFeatureDef )
	{
		CString strVal;
		CFieldDef* cityFieldDef = pFeatureDef->GetFieldDef( placename_rep_point::kTableName, placename_rep_point::kCityCode );
		if( cityFieldDef ){
			if( VT_BSTR == cityFieldDef->m_vaValue.vt)
				strVal = cityFieldDef->m_vaValue.bstrVal;

			CFieldDef* addrFieldDef = pFeatureDef->GetFieldDef( placename_rep_point::kTableName, placename_rep_point::kAddrCode );
			if( addrFieldDef ){
				if( VT_BSTR == addrFieldDef->m_vaValue.vt)
					strVal += addrFieldDef->m_vaValue.bstrVal;
			}
			else if( !strVal.IsEmpty() ){
				strVal.Empty();
			}
		}
		SetDlgItemText(IDC_EDIT_ELEVENCODE, strVal);
	}

	return bRet;
}

/**
 * @fn void CAttrPlacenameRepPointDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrPlacenameRepPointDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		// 11���Z���R�[�h
		std::list<CString> codeList = boost::assign::list_of
			(placename_rep_point::kCityCode)
			(placename_rep_point::kAddrCode);

		m_cControlRel.SetControl( IDC_EDIT_ELEVENCODE, TYPE_EDITSTRING, 0, 0, codeList, 0, placename_rep_point::kTableName);
		// ��\�_����
		m_cControlRel.SetControl( IDC_EDIT_NAME, TYPE_EDITSTRING, 0, 0, placename_rep_point::kName, 0, placename_rep_point::kTableName );
		// �X�֔ԍ�
		m_cControlRel.SetControl( IDC_EDIT_ZIPCODE, TYPE_EDITSTRING, 0, 0, placename_rep_point::kZipCode, 0, placename_rep_point::kTableName );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, placename_rep_point::kTableName );
	}
}

BOOL CAttrPlacenameRepPointDlg::ErrorCheck(){
	using namespace placename_rep_point;

	for( std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
	{
		CString strTableName = it->GetTableName();

		if( strTableName == kTableName )
		{
			// OBJECTID�擾
			LONG lOID = -1;
			it->GetFeature()->get_OID(&lOID);

			// �s�撬���R�[�h�擾
			CFieldDef* pFieldDef = it->GetFieldDef( kTableName, kCityCode );
			CString strCityCode;
			if( pFieldDef ){
				CComVariant vaCityCode = pFieldDef->m_vaValue;
				if( VT_BSTR == vaCityCode.vt )
					strCityCode = vaCityCode;
			}

			// �Z���R�[�h�擾
			pFieldDef = it->GetFieldDef( kTableName, kAddrCode );
			CString strAddrCode;
			if( pFieldDef ){
				CComVariant vaAddrCode = pFieldDef->m_vaValue;
				if( VT_BSTR == vaAddrCode.vt )
					strAddrCode = vaAddrCode;
			}

			CString strMessage,strMessageTmp;
			// 11���Z���R�[�h�̌����`�F�b�N
			if( strCityCode.GetLength() + strAddrCode.GetLength() != 11 )
				strMessage = AheLoadString( IDS_ELEVENFIGURES );

			// �s�撬���R�[�h�������݂̂ō\������Ă��邩
			if( strCityCode.SpanIncluding( _T("1234567890") ) != strCityCode ){
				strMessage += AheLoadString( IDS_EXCEPTNUMBER_ELEVENCODE );
			}
			// 11���Z���R�[�h�ɔ��p�X�y�[�X�������Ă��Ȃ���
			// �i�c�[���ő����l���i�[�����ۂɔ��p�X�y�[�X������ꍇ������炵���j
			if( strCityCode.Find( _T(" ") ) != -1 || strAddrCode.Find( _T(" ") ) != -1){
				strMessage += AheLoadString( IDS_NONEEDSPACE );
			}

			// �X�֔ԍ��`�F�b�N
			pFieldDef = it->GetFieldDef( kTableName, kZipCode );
			CString strZipCode;
			if( pFieldDef ){
				CComVariant vaZipCode = pFieldDef->m_vaValue;
				// VT_NULL�͏��O
				if( VT_BSTR == vaZipCode.vt ){
					strZipCode = vaZipCode;
					// �X�֔ԍ��̌����`�F�b�N
					if( strZipCode.GetLength() != 7 )
						strMessage += AheLoadString( IDS_NOTSEVENFIGURES );

					// �X�֔ԍ��������݂̂ō\������Ă��邩
					if( strZipCode.SpanIncluding( _T("1234567890") ) != strZipCode )
						strMessage += AheLoadString( IDS_EXCEPTNUMBER_ZIPCODE );
				}
			}

			if( !strMessage.IsEmpty() ){
				strMessageTmp.Format(AheLoadString( IDS_ERRORCOMMON ), lOID);
				strMessage = strMessageTmp + strMessage;
				AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );

				return FALSE;
			}
		}
	}
	return TRUE;
}

// fieldDef����ҏW�O�̏����l�𕶎���Ŏ擾����
CString CAttrPlacenameRepPointDlg::getInitStr(CFieldDef* fieldDef){
	CString ret;
	CComVariant vaInitValue = fieldDef->m_vaInitValue;
	if( VT_BSTR == vaInitValue.vt )
		ret = vaInitValue;
	return ret;
}

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

// AttrNationalParkSiteDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrNationalParkSiteDlg.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

// AttrNationalParkSiteDlg �E�B���h�E�}�l�[�W���z�u�}�b�v
BEGIN_WINDOW_MAP(NPSITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_NPARKID_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_NPARKID)
				RCTOFIT(IDC_BUTTON_SEARCHPARK)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_PARKTYPE_C_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_PARKTYPE_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_PARKNUMBER_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_PARKNUMBER)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_NAME_SIGN)
				RCTOFIT(-1)
				RCREST(IDC_NAME)
			ENDGROUP()
		ENDGROUP()
	ENDGROUP()
END_WINDOW_MAP()
/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrNationalParkSiteDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrNationalParkSiteDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrNationalParkSiteDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrNationalParkSiteDlg::CAttrNationalParkSiteDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrNationalParkSiteDlg::CAttrNationalParkSiteDlg() 
	: ATTR_BASE_CLASS(NPSITE_MAP, IDD_NATIONALPARKSITEDLG)
{
	//�e�R���g���[�����T�u�N���X�p�ɏ���
	m_mapSubClass = boost::assign::map_list_of
		( IDC_NPARKID, CContainedWindow(  _T("EDIT"), this, IDC_NPARKID ) )
		(IDC_BUTTON_SEARCHPARK, CContainedWindow(  _T("BUTTON"), this, IDC_BUTTON_SEARCHPARK ) );
}

/** 
 * @fn HWND CAttrNationalParkSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrNationalParkSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	return ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
}

/**
 * @fn void CAttrNationalParkSiteDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrNationalParkSiteDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrNationalParkSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrNationalParkSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	//�\������Ă�������֘A�e�[�u��ID�����Ɋ֘A������ʏ�ɕ\�����܂�
	//�����_�C�A���O���J�����u�Ԃ́A���b�Z�[�W��\�����Ȃ����߁A������false�ɂ���
	//TODO:IDC_NPARKID�̏����l��0�ɂȂ��Ă���A�V�K�쐬->SetNationalParkName���Ăяo����遨�����̃G���[�`�F�b�N�ň����������ă��b�Z�[�W�\��
	//TODO:�ƂȂ邽�߁A������false�̂Ƃ��̓��b�Z�[�W��\�����Ȃ��Atrue�̂Ƃ��ɂ̓��b�Z�[�W��\������Ƃ��Ă���B���W�b�N���C������΁A�Y��ɂȂ�Ǝv����̂ŁA�����̂��ƁB
	SetNationalParkName( false );

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetDlgStatus(IDC_BUTTON_SEARCHPARK, TRUE );
		SetDlgStatus(IDC_NPARKID, TRUE );
		//�ŏ��Ƀt�H�[�J�X�����R���g���[��
		::SetFocus( GetDlgItem(IDC_NPARKID) );
	}
	else
	{
		SetDlgStatus(IDC_BUTTON_SEARCHPARK, FALSE );
		SetDlgStatus(IDC_NPARKID, FALSE );
	}

	return bRet;
}

/**
 * @fn void CAttrNationalParkSiteDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrNationalParkSiteDlg::CreateControlRelation()
{
	ATLASSERT( SUCCEEDED(IsWindow()) );

	if( !m_cControlRel.empty() )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		m_cControlRel.SetControl( IDC_NPARKID, TYPE_EDITSTRING, 0, 0, sj::nationalpark_site::kNparkID, 0, sj::nationalpark_site::kTableName );
	}

}

/**
* @brief �����E��������̊֘A�e�[�u�����������A�֘A����\������
* @param messageFlag   [in] ���b�Z�[�W��\�����邩�ۂ��̃t���O(�V�K�쐬�����Ƃ��Ƀ��b�Z�[�W���\������Ă��܂��̂�h��)
*/
void CAttrNationalParkSiteDlg::SetNationalParkName( bool messageFlag )
{
	//NATIONALPARK_TABLE���J��
	ITablePtr ipTable = GetNationalParkTable();
	if( !ipTable )
	{
		if(messageFlag) AttrMessageBox( AheLoadString(IDS_FAIL_FIND_NPTABLE), AheLoadString(IDS_CAPTION), MB_OK, NULL );
		clearText();
		return;
	}

	//�����֘A�e�[�u��ID���G�f�B�b�g�{�b�N�X����擾
	CWindow edit( GetDlgItem( IDC_NPARKID ) );
	CString strParkID;
	edit.GetWindowText( strParkID );

	// �擾���������֘A�e�[�u��ID��OID���������̂��e�[�u�����猟��
	_ICursorPtr ipCursor;

	//�N�G���t�B���^��ݒ�
	CString  strSubFields;
	strSubFields.Format( _T("%s,%s,%s"),
		sj::nationalpark_table::kParkTypeC, sj::nationalpark_table::kParkNumber, sj::nationalpark_table::kName );
	IQueryFilterPtr ipQueryFilter = AheInitQueryFilter( nullptr, strSubFields, _T("OBJECTID=%s"), strParkID );

	//��������
	ipTable->Search( ipQueryFilter, VARIANT_TRUE, &ipCursor );

	//�������ĉ�������������Ȃ������ꍇ�͏����𒆒f
	if( !ipCursor )
	{
		if(messageFlag) AttrMessageBox( AheLoadString(IDS_FAIL_SEARCH), AheLoadString(IDS_CAPTION), MB_OK, NULL );
		clearText();
		return;
	}

	//��v����s��1�����Ȃ��͂��Ȃ̂ŁA��s�����ǂݍ���
	//�s�̓ǂݍ��݂Ɏ��s������A�����𒆒f
	_IRowPtr ipRow;
	if( ipCursor->NextRow( &ipRow ) != S_OK || !ipRow )
	{
		if(messageFlag)
		{
			CString errorMessage;
			errorMessage.Format( AheLoadString(IDS_FAIL_FIND_NPOBJECT), strParkID, sj::nationalpark_table::kTableName );
			AttrMessageBox( errorMessage, AheLoadString(IDS_CAPTION), MB_OK, NULL );
		}
		clearText();
		return;
	}

	//�e�[�u���̒l���擾
	//vaParkType�͒l���擾���邪�A���Ɏg�p���Ȃ�
	CComVariant vaParkNumber, vaName, vaParkType;
	vaParkNumber = AheGetAttribute( ipRow, sj::nationalpark_table::kParkNumber );
	vaName = AheGetAttribute( ipRow, sj::nationalpark_table::kName );

	//�h���C�������擾
	CString strDomain;
	AheGetDomainNameAndValue( ipRow, sj::nationalpark_table::kParkTypeC, strDomain, vaParkType);

	ATLASSERT( vaParkType.vt == VT_I4 && vaParkNumber.vt == VT_I4 && vaName.vt == VT_BSTR );

	//�e���x���ɏ���\��
	//�h���C�������Z�b�g
	setText(strDomain, IDC_PARKTYPE_C);

	//���������Z�b�g
	CString strText = vaName.bstrVal;
	setText(strText, IDC_NAME);
	//�����ԍ����Z�b�g
	strText.Format(_T("%d"), vaParkNumber.intVal);
	setText(strText, IDC_PARKNUMBER);
}

/**
* @brief NATIONALPARK_TABLE���J��
* @return ITablePtr �e�[�u���̃I�[�v���ɐ��������ꍇ�ANULL�ȊO���Ԃ�
*/
ITablePtr CAttrNationalParkSiteDlg::GetNationalParkTable()
{
	ITablePtr ipTable;

	//�I������Ă���t�B�[�`���[���擾
	IFeaturePtr ipCurrentFeature = GetCurrentFeatureDef()->GetFeature();
	if( !ipCurrentFeature )
		return ipTable;

	//�t�B�[�`���[����e�[�u�����擾
	ITablePtr ipCurrentTable;
	ipCurrentFeature->get_Table(&ipCurrentTable);
	if( !ipCurrentFeature )
		return ipTable;

	//ipCurrentTable�Ɠ����I�[�i�[��NATIONALPARK_TABLE���擾
	ipTable = AheOpenSameOwnerTable(ipCurrentTable, sj::nationalpark_table::kTableName );
	return ipTable;
}

/**
* @brief Text�ɕ������ݒ肷��
* @param strText Text�ɐݒ肷�镶����
* @param nClassID �������ݒ肷��Text��ID
*/
void CAttrNationalParkSiteDlg::setText( const CString& strText, INT nClassID )
{
	::SetWindowText(GetDlgItem(nClassID), (LPCTSTR)strText);
}

/**
* @brief ������ʃR�[�h�A�����ԍ��A���̂ɋ󕶎���ݒ肷��
* @note �֘A�e�[�u����������擾�ł��Ȃ������ꍇ�Ɏg�p����
*/
void CAttrNationalParkSiteDlg::clearText()
{
	::SetWindowText(GetDlgItem(IDC_PARKTYPE_C), _T(""));
	::SetWindowText(GetDlgItem(IDC_PARKNUMBER), _T(""));
	::SetWindowText(GetDlgItem(IDC_NAME), _T(""));
}

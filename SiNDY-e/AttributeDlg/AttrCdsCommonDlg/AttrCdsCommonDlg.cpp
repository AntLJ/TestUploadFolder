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

// AttrCDSCommonDlg.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "AttrCdsCommonDlg.h"
#include "DlgMap.h"
#include <boost/assign/list_of.hpp>

using namespace sindy::schema;

/**
 * �N���X�̖����I�����N���s�����߂ɁA���̃N���X��CExportDlg�N���X���p�����A�����_�C�A���O����CExportDlg���쐬���邱�Ƃɂ��
 * �����_�C�A���O������t���ɂ����邱�Ƃ��ł���N���X�{�̂��쐬����܂��B
 * �ڂ�����http://ptvi.hp.infoseek.co.jp/vc/p_0000010.html���Q�Ƃ̂���
 *
 * @retval CAttrCdsCommonDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
	return new CAttrCdsCommonDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrCdsCommonDlg::CAttrCdsCommonDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrCdsCommonDlg::CAttrCdsCommonDlg() 
	: ATTR_BASE_CLASS(CITY_SITE_MAP, IDD_CDS_COMMON_DLG)
{
	m_mapSubClass = boost::assign::map_list_of
		( IDC_EDIT_SOURCE, CContainedWindow( _T("EDIT"), this, IDC_EDIT_SOURCE ) )
		( IDC_COMBO_CLASS, CContainedWindow( _T("COMBO"), this, IDC_COMBO_CLASS ) )
		( IDC_EDIT_MSNO, CContainedWindow( _T("EDIT"), this, IDC_EDIT_MSNO ) )
		( IDC_BUTTON_SEARCH_MSNO, CContainedWindow( _T("BUTTON"), this, IDC_BUTTON_SEARCH_MSNO ) )
		( IDC_CHECK_UNDERGROUND, CContainedWindow( _T("CHECK"), this, IDC_CHECK_UNDERGROUND ) )
		( IDC_CHECK_CORRIDOR, CContainedWindow( _T("CHECK"), this, IDC_CHECK_CORRIDOR ) )
		( IDC_CHECK_NOWALL, CContainedWindow( _T("CHECK"), this, IDC_CHECK_NOWALL ) )
		( IDC_EDIT_BLDNAME, CContainedWindow( _T("EDIT"), this, IDC_EDIT_BLDNAME ) )
		( IDC_EDIT_BLDADDR, CContainedWindow( _T("EDIT"), this, IDC_EDIT_BLDADDR ) )
		( IDC_EDIT_GEOID, CContainedWindow( _T("EDIT"), this, IDC_EDIT_GEOID ) )
		( IDC_EDIT_NAMESTRING1, CContainedWindow( _T("EDIT"), this, IDC_EDIT_NAMESTRING1 ) )
		( IDC_EDIT_NAMESTRING2, CContainedWindow( _T("EDIT"), this, IDC_EDIT_NAMESTRING2 ) )
		( IDC_COMBO_DISPTYPE, CContainedWindow( _T("COMBO"), this, IDC_COMBO_DISPTYPE ) )
		( IDC_CHECK_HIMAWARI, CContainedWindow( _T("CHECK"), this, IDC_CHECK_HIMAWARI ) );

	// ���e�ԍ����͏��O�e�[�u���ꗗ�ݒ�
	m_msNoExceptTblList = boost::assign::list_of
		( city_railway_gs::kTableName )
		( city_station_gs::kTableName );
}

/** 
 * @fn HWND CAttrCdsCommonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrCdsCommonDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	// ���C���ɉ����Ĕz�u�}�b�v��ς���
	// TODO: �ǂ����ŗ񋓌^���`���Ĕ��肵�������ǂ�
	if( 0 == m_strClassName.CompareNoCase( city_site_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_SITE_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_line_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_LINE_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_railway_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_RAILWAY_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( building_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(BUILDING_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_ANNO_MAP);
	}
	else if( 0 == m_strClassName.CompareNoCase( city_station_gs::kTableName ) )
	{
		m_winMgr.ChangeMgrMap(CITY_STATION_MAP);
	}

	HWND hWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);

	// ���C���ɉ����āA�s�v�ȃR���g���[���̔�\���ƃL���v�V�����̕ύX���s��
	// CITY_SITE_GS
	if( 0 == m_strClassName.CompareNoCase(city_site_gs::kTableName) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_BG));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_BG));
	}

	// CITY_LINE_GS
	if( 0 == m_strClassName.CompareNoCase( city_line_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_LINE_BG));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_LINE_BG));
	}
	
	// CITY_RAILWAY_GS
	if( 0 == m_strClassName.CompareNoCase( city_railway_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_RAILWAY_CLASS));
	}
	else // CITY_RAILWAY_GS�ȊO
	{
		::ShowWindow( GetDlgItem( IDC_CHECK_UNDERGROUND ), SW_HIDE );
	}

	// BUILDING_GS
	if( 0 == m_strClassName.CompareNoCase( building_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_BLDG_CLASS));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_BLDG_CLASS));
	}
	else // BUILDING_GS�ȊO
	{
		::ShowWindow( GetDlgItem( IDC_CHECK_CORRIDOR ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_CHECK_NOWALL ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_BLDNAME ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_BLDNAME ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_BLDADDR ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_BLDADDR ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_GEOID ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_GEOID ), SW_HIDE );
	}

	// CITY_ANNOTATION_GS
	if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_ANNO_CLASS));
		GetDlgItem(IDC_BUTTON_SEARCH_MSNO).SetWindowText(AheLoadString(IDS_CAPTION_SEARCH_ANNO_CLASS));
	}
	else // CITY_ANNOTATION_GS�ȊO
	{
		::ShowWindow( GetDlgItem( IDC_STATIC_NAMESTRING1 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_NAMESTRING1 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_NAMESTRING2 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_NAMESTRING2 ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_STATIC_DISPTYPE ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_COMBO_DISPTYPE ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_CHECK_HIMAWARI ), SW_HIDE );
	}

	// CITY_STATION_GS
	if( 0 == m_strClassName.CompareNoCase( city_station_gs::kTableName ) )
	{
		GetDlgItem(IDC_STATIC_CLASS).SetWindowText(AheLoadString(IDS_CAPTION_STATION_CLASS));
	}

	// ���e�ԍ����͊֘A
	auto findTargetTbl = std::find_if( 
				m_msNoExceptTblList.begin(),
				m_msNoExceptTblList.end(),
				[this](const CString& tableName)
					{ return 0 == tableName.CompareNoCase(this->m_strClassName); } );
	// ���e�ԍ����͑ΏۊO�̃e�[�u���ł���΁A�R���g���[�����B��
	if( m_msNoExceptTblList.end() != findTargetTbl )
	{
		::ShowWindow( GetDlgItem( IDC_STATIC_MSNO ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_EDIT_MSNO ), SW_HIDE );
		::ShowWindow( GetDlgItem( IDC_BUTTON_SEARCH_MSNO ), SW_HIDE );
	}
	
	return hWnd;
}

/**
 * @fn void CAttrCdsCommonDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrCdsCommonDlg::Delete()
{
	ClearFeatureDefs();
	if( IsWindow() )
		DestroyWindow();
	delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrCdsCommonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrCdsCommonDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowMSNOByClass();
	UpdateDispTypeList();
	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_COMBO_CLASS) );
	}

	// GEOSPACE_ID�͕ҏW����Ȃ��̂�DISABLE��
	SetDlgStatus( IDC_EDIT_GEOID, FALSE, TRUE );

	return bRet;
}

/**
 * @fn void CAttrCdsCommonDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrCdsCommonDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( !m_cControlRel.empty() )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );

		if( 0 == m_strClassName.CompareNoCase( city_site_gs::kTableName ) )
		{
			// �w�i���
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_site_gs::kBgClass, 0, city_site_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_line_gs::kTableName ) )
		{
			// �w�i���C�����
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_line_gs::kBgClass, 0, city_line_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_railway_gs::kTableName ) )
		{
			// �S�����, �n���t���O
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_railway_gs::kRailwayClass, 0, city_railway_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_UNDERGROUND, TYPE_CHECKBOX, 0, 0, city_railway_gs::kUnderground, 0, city_railway_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( building_gs::kTableName ) )
		{
			// �������, �n��L���t���O, ���ǃt���O, ��������, �����Z��, GEOSPACE_ID
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, building_gs::kBldClass, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_CORRIDOR, TYPE_CHECKBOX, 0, 0, building_gs::kCorridor, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_NOWALL, TYPE_CHECKBOX, 0, 0, building_gs::kNoWall, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_BLDNAME, TYPE_EDITSTRING, 0, 0, building_gs::kBldName, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_BLDADDR, TYPE_EDITSTRING, 0, 0, building_gs::kBldAddr, 0, building_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_GEOID, TYPE_EDITSTRING, 0, 0, building_gs::kGeospaceID, 0, building_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
		{
			// ���L���, ���L������P,�Q, ���L�L��, �Ђ܂��t���O
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_annotation_gs::kAnnoClass, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_NAMESTRING1, TYPE_EDITSTRING, 0, 0, city_annotation_gs::kNameString1, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_EDIT_NAMESTRING2, TYPE_EDITSTRING, 0, 0, city_annotation_gs::kNameString2, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_COMBO_DISPTYPE, TYPE_COMBO, 0, 0, city_annotation_gs::kDispType, 0, city_annotation_gs::kTableName );
			m_cControlRel.SetControl( IDC_CHECK_HIMAWARI, TYPE_CHECKBOX, 0, 0, city_annotation_gs::kHimawari, 0, city_annotation_gs::kTableName );
		}
		else if( 0 == m_strClassName.CompareNoCase( city_station_gs::kTableName ) )
		{
			m_cControlRel.SetControl( IDC_COMBO_CLASS, TYPE_COMBO, 0, 0, city_station_gs::kStationClass, 0 , city_station_gs::kTableName );
		}

		// ���e�ԍ�����
		auto findTargetTbl = std::find_if( 
					m_msNoExceptTblList.begin(),
					m_msNoExceptTblList.end(),
					[this](const CString& tableName)
						{ return 0 == tableName.CompareNoCase(this->m_strClassName); } );
		// ���e�ԍ����͑Ώۂ̃e�[�u���̏ꍇ�A�R���g���[����ݒ�
		if( m_msNoExceptTblList.end() == findTargetTbl )
		{
			m_cControlRel.SetControl( IDC_EDIT_MSNO,	TYPE_EDIT,	0,	0 );
			m_cControlRel.SetControl( IDC_BUTTON_SEARCH_MSNO,	TYPE_BUTTON,	0,	0 );
		}

		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE, TYPE_EDITSTRING, 0, 0, ipc_table::kSource, 0, m_strClassName );
	}
}

/**
 * @fn BOOL CAttrCdsCommonDlg::SetClassByMSNO(INT nCodeID, INT nClassID)
 * @brief ��ʃR�[�h���R���{�{�b�N�X�ɐݒ肷��
 *
 * ���e�ԍ�EditBox�ɓ��͂��ꂽ���e�ԍ�����e��ʃR�[�h���擾���A�e��ʃR�[�hComboBox�ɐݒ肷��
 *
 * @param nCodeID			[in]	���e�ԍ��������Ă���EditBox�̃R�[�hID
 * @param nClassID			[in]	�w�肵����ʂ�ComboBox�̃R�[�hID
 *
 * @return ��ʃR�[�h���Z�b�g���邱�Ƃ��ł�����TRUE,�ł��Ȃ�������FALSE
 */
BOOL CAttrCdsCommonDlg::SetClassByMSNO(INT nCodeID, INT nClassID)
{
	CEdit edit( GetDlgItem( nCodeID ) );
	CString strMSNO;
	edit.GetWindowText( strMSNO );

	// ���e�ԍ��𔼊p�ɕϊ�
	strMSNO = AheConvertToGenkouNoStr( strMSNO );

	// ���e�ԍ������ʃR�[�h���擾
	LONG lSearchClassID = -1;
	if( !strMSNO.IsEmpty() )
		lSearchClassID = GetClassByMSNO( strMSNO );

	if( lSearchClassID >= 0 )
	{
		// ��ʃR���{�{�b�N�X�ɃZ�b�g
		SetComboBoxSel(nClassID, lSearchClassID);

		// ���e�ԍ����Z�b�g
		SetDlgItemText(nCodeID, strMSNO);

		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	}
	else
	{
		// ���e�ԍ������ɖ߂�
		ShowMSNOByClass();
		AttrMessageBox( AheLoadString( IDS_NOMANUSCRIPT ), AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
		return FALSE;
	}

	return TRUE;
}

/**
 * @fn void CAttrCdsCommonDlg::ShowMSNOByClass()
 * @brief ���ݑI�𒆂̎�ʃR�[�h�̌��e�ԍ����A���e�ԍ��t�B�[���h�ɕ\��
 */
void CAttrCdsCommonDlg::ShowMSNOByClass()
{
	CWindow	cWMsno(GetDlgItem(IDC_EDIT_MSNO));	// ���e�ԍ��e�L�X�g�{�b�N�X
	LONG lClassID = -1;
	GetClassID( &lClassID );
	cWMsno.SetWindowText( GetMSNOByClass( lClassID ) );
	return;
}

/**
 * @fn LONG CAttrCdsCommonDlg::GetClassID(LONG *lClassID)
 * @brief ���ݑI�𒆂̎��ID���擾
 *
 * @param	lClassID	[out]		�I�𒆂̎�ʂ�ID
 */
void CAttrCdsCommonDlg::GetClassID(LONG *lClassID)
{
	CComboBox	cWComboClass( GetDlgItem( IDC_COMBO_CLASS ) );
	LONG		lSelectIndex = cWComboClass.GetCurSel();
	*lClassID = (LONG)cWComboClass.GetItemData(lSelectIndex);
}

/**
 * @brief ���^�[���L�[�������ꂽ���̓���
 *
 * @param	nClassID	[in]		���^�[���L�[�������ꂽ����ItemID
 *
 * @retval  TRUE  ���^�[���L�[�Ƃ��Ă̓���(�K�p����)��
 * @retval  FALSE ���^�[���L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrCdsCommonDlg::CheckReturnKeyItem(INT nClassID)
{
	BOOL bIsOK = TRUE; 
	if ((nClassID == IDC_EDIT_MSNO) || (nClassID == IDC_BUTTON_SEARCH_MSNO)) {		// ���e�ԍ��G�f�B�b�g�{�b�N�X or �����{�^��
		if( !SetClassByMSNO(IDC_EDIT_MSNO, IDC_COMBO_CLASS) )
			return FALSE;

		// ���L�L�����X�V
		UpdateDispTypeList();
	}
	else
	{
		bIsOK = ErrorCheck();
		if( bIsOK && (nClassID == IDC_EDIT_NAMESTRING1 || nClassID == IDC_EDIT_NAMESTRING2) )
			AutoUpdateStringNum( nClassID );
	}
	return bIsOK;
}

/**
 * @brief GEOSPACE_ID���ύX���ꂽ���̓���
 *
 * @return �Ȃ�
 */
void CAttrCdsCommonDlg::SetGeospaceID()
{
	// GEOSPACE_ID���G�f�B�b�g�{�b�N�X����擾
	CEdit cEdit( GetDlgItem( IDC_EDIT_GEOID ) );
	CString strEdit;
	cEdit.GetWindowText( strEdit );

	// GEOSPACE_ID�𔼊p�ɕϊ�(���e�ԍ��̕ϊ����W�b�N�g�p)
	// �Ђ炪�ȓ��͎�菜����� ex. 123������abcDEF �� 123ABCDEF
	CString strGeoID;
	strGeoID = AheConvertToGenkouNoStr( strEdit );	// ���p,�p��OK,�}�C�i�XNO

	// �ϊ�����GEOSPACE_ID���Z�b�g
	cEdit.SetWindowText( strEdit );
}

/**
 * @brief ���e�ԍ��Ǝ�ʂ̑Ή��\���쐬����
 *
 * @return �Ȃ�
 */
void CAttrCdsCommonDlg::LoadBackgroundRule()
{
	// CITY_ANNOTATION_GS�̏ꍇ�́AAnnotationClassTable.h����
	// ����ȊO�́ABackgroundClassTable.h����Ή��\���쐬����
	if( 0 == m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) )
	{
#define ANNO_MAP( msno, code, domain, disp, layer ) { BACKGROUND_RULE br = {0}; \
	                                                  br.lClass = code; \
 	                                                  br.strClass_Name = domain; \
 	                                                  br.strMS_NO = msno; \
 									                  m_BackgroundRule.emplace_back(br); \
                                                      m_mapAnnoDispType[code] = layer::disp_type::disp; }
#include "AnnotationClassTable.h"
#undef ANNO_MAP
	}
	else
	{
#define BG_MAP( msno, code, domain, layer ) if( 0 == m_strClassName.CompareNoCase(layer::kTableName) ) \
	                                          { BACKGROUND_RULE br = {0}; \
	                                            br.lClass = code; \
	                                            br.strClass_Name = domain; \
	                                            br.strMS_NO = msno; \
									            m_BackgroundRule.emplace_back(br); }
#include "BackgroundClassTable.h"
#undef BG_MAP
	}
}

/**
 * @brief �G���[�`�F�b�N���s��
 *
 * @retval  TRUE  �G���[�Ȃ�
 * @retval  FALSE �G���[����
 */
BOOL CAttrCdsCommonDlg::ErrorCheck()
{
	BOOL bRet = TRUE; // �Ԃ�l
	CString strMsg; // �_�C�A���O�ɕ\�����郁�b�Z�[�W

	// �I�����������̃t�B�[�`���ɑ΂��ăG���[�`�F�b�N
	for( auto& featureDef : *m_pFeatureDefList )
	{
		CString strErr; // �G���[���b�Z�[�W

		// ���L������̃`�F�b�N
		CheckAnnoNameString( featureDef, strErr );

		// �����񂪃t�B�[���h�T�C�Y�𒴂��Ă��Ȃ����`�F�b�N
		CheckExceedFieldSize( featureDef, strErr );

		// �G���[���b�Z�[�W���Ȃ���Ύ��̃t�B�[�`����
		if( strErr.IsEmpty() )
			continue;

		// �G���[���b�Z�[�W������΁A�\���p�̃��b�Z�[�W�ɐ��`
		LONG lOID = 0;
		featureDef.GetFeature()->get_OID(&lOID);
		CString strTmpMsg;
		strTmpMsg.Format( AheLoadString(IDS_ERR_INVALID_ATTR), featureDef.GetTableName(), lOID );
		INT nCurPos = 0;
		CString strToken( strErr.Tokenize( _T(","), nCurPos ) );
		while( -1 != nCurPos )
		{
			strTmpMsg += _T("\n") + strToken;
			strToken = strErr.Tokenize(_T(","), nCurPos );
		}
		strMsg += strTmpMsg + _T("\n");

		bRet = FALSE;
	}

	// �G���[������΃��b�Z�[�W���o��
	if( !bRet )
		AttrMessageBox( strMsg, AheLoadString( IDS_ERRCHK ), MB_OK|MB_ICONWARNING, WARNLEVEL_HIGH );
	
	return bRet;
}

/**
 * @brief ���L������ɕs���ȕ������܂܂�Ă��Ȃ����`�F�b�N����
 *
 * @note  ���L�̕����ȊO���܂܂��ꍇ�́A�G���[�Ƃ���
 *        �S�p�����A�S�p�Ђ炪�ȁA�S�p�J�^�J�i�A
 *        �u�O�v�`�u�X�v�A�u���v�`�u�y�v�A�S�p�M���V�A����
 *        �S�p���p�\����L���i0x8141�i�u�A�v�j�`0x81fc�i�u���v�j�j
 *        �u�D�v�u�`�v�u�X�v�u�Y�v�u�E�v�u�i�v�u�j�v�u�[�v�u�A�v�u�|�v
 *        ����O�����̕����͋��e���Ȃ�
 *        �܂��A�`�F�b�N����̂͒��L������1�̂�
 *
 * @param	featureDef	[in]		�`�F�b�N�Ώۂ̃t�B�[�`��
 * @param	strErr		[out]		�G���[���b�Z�[�W
 *
 * @retval  TRUE  �Ώۂ̃��C���ł͂Ȃ��A�܂��͕s���������܂܂�Ă��Ȃ�
 * @retval  FALSE �s���ȕ������܂܂�Ă���
 */
BOOL CAttrCdsCommonDlg::CheckAnnoNameString( CFeatureDef& featureDef, CString& strErr )
{
	// ���L�ȊO�ł͊m�F�s�v
	CString strTableName( featureDef.GetTableName() );
	if( 0 != strTableName.CompareNoCase(city_annotation_gs::kTableName) )
		return TRUE;
	
	// ���L������1���`�F�b�N		
	CFieldDef* pDef = featureDef.GetFieldDef( strTableName, city_annotation_gs::kNameString1 );
	CString strOrgStr( ( pDef && VT_BSTR == pDef->m_vaValue.vt ) ? pDef->m_vaValue : _T("") );

	// ���L�������K���ȕ�����ɕϊ�
	CString strErrChar; // �s���ȕ���
	CString strCorrectStr = AheConvertToAnnoJustStr(strOrgStr, TRUE, FALSE, FALSE, &strErrChar );

	// �ϊ��O��Œ��L������ɕω����Ȃ���΃��^�[��
	// �ω�������΁A�s���ȕ������܂܂�Ă���
	if( 0 == strCorrectStr.Compare(strOrgStr) )
		return TRUE;
	
	// �s��������΁A���b�Z�[�W�{�b�N�X���o���ĕۑ������Ȃ�
	strErr += AheLoadString( IDS_MSG_INVALID_NAMESTRING );

	return FALSE;
}

/**
 * @brief ���L�������ύX�����ۂɒ��L���������X�V����
 *
 * @retern  �Ȃ�
 */
void CAttrCdsCommonDlg::AutoUpdateStringNum(UINT nTargetControl)
{
	if( ! GetCurrentFeatureDef() )
		return;

	CString strStringNumFieldName, strStringFieldName; // �������E������t�B�[���h���inTargetControl�ŕς��j
	switch( nTargetControl )
	{
	case IDC_EDIT_NAMESTRING1: strStringNumFieldName = city_annotation_gs::kStringNum1; strStringFieldName = city_annotation_gs::kNameString1; break;
	case IDC_EDIT_NAMESTRING2: strStringNumFieldName = city_annotation_gs::kStringNum2; strStringFieldName = city_annotation_gs::kNameString2; break;
		default: _ASSERTE(false); break;
	}

	CFieldDef* pStringFieldDef = GetCurrentFeatureDef()->GetFieldDef( m_strClassName, strStringFieldName );

	CString strText;
	if( pStringFieldDef )
	{
		CComVariant vaNameString( pStringFieldDef->m_vaValue );
		if( VT_BSTR == vaNameString.vt )
			strText = vaNameString.bstrVal;
	}

	// �������v�Z�iANSI�ł̃o�C�g���ł��邱�Ƃɒ��ӁI�I bug 2746�j
	LONG lStringNumber = strlen( CT2CA( strText ) );
	// UTF-8�ł̃o�C�g���v�Z �ꉞ�c���Ă���
	//lStringNumber = ::WideCharToMultiByte( CP_UTF8, 0, strText, -1, NULL, 0, NULL, NULL ) - 1;

	// FieldDef�ɃZ�b�g
	CFieldDef* pStringNumFieldDef = GetCurrentFeatureDef()->GetFieldDef( m_strClassName, strStringNumFieldName );
	pStringNumFieldDef->SetNewValue( lStringNumber, m_hWnd );
}

/**
 * @brief ���L��ʂ�ύX�����ۂɒ��L�L���̃R���{�{�b�N�X���X�g���X�V����
 *
 * @retern  �Ȃ�
 */
void CAttrCdsCommonDlg::UpdateDispTypeList()
{
	// CITY_ANNOTATION_GS�ȊO�A�܂��͕ҏW�J�n���Ă��Ȃ���΃��^�[��
	if( 0 != m_strClassName.CompareNoCase( city_annotation_gs::kTableName ) || !m_cArcHelper.IsStartEditing() )
		return;

	// ���L��ʂ��擾
	LONG lCode = -1;
	GetClassID( &lCode );

	// ���L�L���̃R���{�{�b�N�X���X�g���X�V���A���L��ʂɑΉ����钍�L�L�����Z�b�g
	CComboBox cWDispType(GetDlgItem(IDC_COMBO_DISPTYPE));
	cWDispType.ResetContent();

	CFieldDef* pDispTypeDef = GetCurrentFeatureDef()->GetFieldDef( m_strClassName, city_annotation_gs::kDispType );

	// ���L��ʂɑΉ����钍�L�L���݂̂�I���ł���悤�A���X�g���X�V
	// �Ή�������̂��Ȃ��ꍇ�́A���ׂđI���ł���悤�ɂ���
	auto mapDispTypeDomain = pDispTypeDef->GetDomain();
	auto it = m_mapAnnoDispType.find( lCode );
	for(const auto& domain : *mapDispTypeDomain)
	{
		if(m_mapAnnoDispType.end() == it || domain.idx == it->second)
			cWDispType.SetItemData( cWDispType.AddString( domain.code_discription ), domain.idx );
	}

	// ��ʂɑΉ�����L��������΂��̒��L�L�����A
	// �Ȃ���΃t�B�[���h�ɃZ�b�g����Ă��钍�L�L�����A�R���{�{�b�N�X����I��
	LONG lDispType = -1;
	if( m_mapAnnoDispType.end() != it )
		lDispType = it->second;
	else
		lDispType = ( ( pDispTypeDef && VT_I4 == pDispTypeDef->m_vaValue.vt ) ? pDispTypeDef->m_vaValue.lVal : -1 );

	SetComboBoxSel(IDC_COMBO_DISPTYPE, lDispType);

	InvalidateRect( NULL, FALSE );
	// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
	SetButton( Changed() );
}

/**
 * @brief �w�肵���R���{�{�b�N�X�̃A�C�e����ύX
 *
 * @param	nComboID	[in]		�R���{�{�b�N�X��ItemID
 * @param	lVal	    [in]		�\������A�C�e���ɑΉ�����l
 *
 * @return  �Ȃ� 
 */
void CAttrCdsCommonDlg::SetComboBoxSel(INT nComboID, LONG lVal)
{
	CComboBox	cWComboBox( GetDlgItem(nComboID) );
	LONG		lClassNum = cWComboBox.GetCount();
	LONG		lComboBoxItemNum = 0;
	for (; lComboBoxItemNum < lClassNum ; ++lComboBoxItemNum)
	{
		DWORD dwComboBoxValue = cWComboBox.GetItemData(lComboBoxItemNum);
		LONG lComboBoxValue = (LONG)dwComboBoxValue;
		if (lComboBoxValue == lVal)
			break;
	}
	cWComboBox.SetCurSel(lComboBoxItemNum);

	m_cControlRel.ControlChanged( nComboID );
}

/**
 * @brief ���͂��ꂽ�����񂪃t�B�[���h�T�C�Y�𒴂��Ă��Ȃ����`�F�b�N����
 *
 * @param	featureDef	[in]		�`�F�b�N�Ώۂ̃t�B�[�`��
 * @param	strErr		[out]		�G���[���b�Z�[�W
 *
 * @retval  TRUE  �t�B�[���h�T�C�Y�𒴂��Ă��Ȃ�
 * @retval  FALSE �t�B�[���h�T�C�Y�𒴂��Ă���
 */
BOOL CAttrCdsCommonDlg::CheckExceedFieldSize( CFeatureDef& featureDef, CString& strErr )
{
	CString strTableName( featureDef.GetTableName() );

	// �`�F�b�N�Ώۂ̃t�B�[���h�ꗗ(�ׂ�����)
	std::vector<CString> vecTgtFieldName;
	{
		// ���\�[�X(�e���C������)
		vecTgtFieldName.emplace_back(ipc_table::kSource);

		if( 0 == strTableName.CompareNoCase(city_annotation_gs::kTableName) )
		{
			// ���L������1, ���L������2(CITY_ANNOTATION_GS)
			vecTgtFieldName.emplace_back(city_annotation_gs::kNameString1);
			vecTgtFieldName.emplace_back(city_annotation_gs::kNameString2);
		}
		else if( 0 == strTableName.CompareNoCase(building_gs::kTableName) )
		{
			// ��������, �����Z��(BUILDING_GS)
			vecTgtFieldName.emplace_back(building_gs::kBldName);
			vecTgtFieldName.emplace_back(building_gs::kBldAddr);
		}
	}

	BOOL bIsOK = TRUE;

	// ���͂��ꂽ�����񂪃t�B�[���h�T�C�Y�𒴂��Ă��Ȃ����`�F�b�N
	for( const auto& fieldName : vecTgtFieldName )
	{
		CFieldDef* pDef = featureDef.GetFieldDef(strTableName, fieldName);

		LONG lFieldSize = -1;
		pDef->m_ipField->get_Length(&lFieldSize);

		CString strInput = ( VT_BSTR == pDef->m_vaValue.vt ) ? pDef->m_vaValue : _T("");
		// UTF-8�ł��邱�Ƃɒ���
		INT nInputSize = ::WideCharToMultiByte( CP_UTF8, 0, strInput, -1, NULL, 0, NULL, NULL ) - 1;

		if( lFieldSize >= nInputSize )
			continue;

		CString strTmp;
		strTmp.Format(AheLoadString(IDS_MSG_EXCEED_FIELD_SIZE), pDef->m_strFieldAliasName);
		strErr += strTmp;
		bIsOK = FALSE;
	}

	return bIsOK;
}

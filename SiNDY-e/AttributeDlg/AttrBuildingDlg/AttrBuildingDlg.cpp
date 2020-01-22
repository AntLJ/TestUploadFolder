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

// AttrBuildingDlg.cpp: CAttrBuildingDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBuildingDlg.h"

using namespace sindy::schema;

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i�s�s�n�}�ƌ`�p�j
BEGIN_WINDOW_MAP(BUILDING_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_BLDCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_BLDCLASS_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU)
				RCTOFIT(IDC_BUTTON_SEARCH)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_CORRIDOR_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_CHECK_NOWALL_F)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GEOID)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GEOID)
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
 * @retval CAttrBuildingDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBuildingDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBuildingDlg::CAttrBuildingDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrBuildingDlg::CAttrBuildingDlg() 
	: ATTR_BASE_CLASS(BUILDING_MAP, IDD_ATTRBUILDINGDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_BLDCLASS_C ),
		m_wnd1( _T("EDIT"), this, IDC_EDIT_GENKOU ),
		m_wnd2( _T("BUTTON"), this, IDC_BUTTON_SEARCH ),
		m_wnd3( _T("CHECKBOX"), this, IDC_CHECK_CORRIDOR_F ),
		m_wnd4( _T("EDIT"), this, IDC_EDIT_SOURCE ),
		m_wnd5( _T("CHECKBOX"), this, IDC_CHECK_NOWALL_F ),
		m_wnd6( _T("EDIT"), this, IDC_EDIT_GEOID )
{
}

/** 
 * @fn HWND CAttrBuildingDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBuildingDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBuildingDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBuildingDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrBuildingDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
 * @brief ���ݕ\�������ׂ��t�B�[�`����FeatureDef�C���f�b�N�X��n���܂��B
 * ����ɁA�t�B�[�`���̑����Ƃ͊֌W�Ȃ��R���g���[���̐�������Ă����܂��B
 * �_�C�A���O���J�����Ƃ��ɍŏ��Ƀt�H�[�J�X�̓������Ă��ė~�����R���g���[���̐ݒ�����Ă����܂��B
 * ���e�ԍ��̐ݒ�����Ă����܂��B
 *
 * @param lFeatureIndex	[in]	�t�B�[�`����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lTableIndex	[in]	�e�[�u����`�N���X�̃C���f�b�N�X�ԍ�
 * @param lRowIndex		[in]	���R�[�h��`�N���X�̃C���f�b�N�X�ԍ�
 * @param bForce		[in]	�����I�ɕҏW���邩�ǂ���
 * @param bEditable		[in]	�ҏW�\���ǂ���
 *
 * @retval �Ӗ����Ȃ��iTRUE�����Ԃ��Ȃ��j
 */

BOOL CAttrBuildingDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
    BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowGenkouByBackgroundClass();

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_EDIT_GENKOU) );
	}

	// GEOSPACE_ID�͕ҏW����Ȃ��̂�DISABLE��
	SetDlgStatus( IDC_EDIT_GEOID, FALSE, TRUE );

    return bRet;
}

/**
 * @fn void CAttrBaseRailwayDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBuildingDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// �s�s�n�}�ƌ`�p

		// ���H�d�����t���O(AK�Ή��œn��L�����疼�̕ύX)
		m_cControlRel.SetControl( IDC_CHECK_CORRIDOR_F, TYPE_CHECKBOX,   0, 0, building::kCorridor,   NULL, BUILDING_TABLE_NAME );
		// ���e�ԍ��`�F�b�N
		m_cControlRel.SetControl( IDC_EDIT_GENKOU,      TYPE_EDIT,       0, 0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH,    TYPE_BUTTON,     0, 0 );
		// �������
		m_cControlRel.SetControl( IDC_COMBO_BLDCLASS_C,	TYPE_COMBO,      0, 0, building::kBldClass,   NULL, BUILDING_TABLE_NAME );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,      TYPE_EDITSTRING, 0, 0, building::kSource,     NULL, BUILDING_TABLE_NAME );
		// ���ǎɃt���O
		m_cControlRel.SetControl( IDC_CHECK_NOWALL_F,   TYPE_CHECKBOX,   0, 0, building::kNoWall,     NULL, BUILDING_TABLE_NAME );
		// GEOSPACEID
		m_cControlRel.SetControl( IDC_EDIT_GEOID,       TYPE_EDITSTRING, 0, 0, building::kGeospaceID, NULL, BUILDING_TABLE_NAME );
	}
}


/**
 * @fn BOOL CAttrBuildingDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
 * @brief ������ʐݒ�
 *
 * �������EditBox�ɓ��͂��ꂽ������ʌ��e�ԍ����猚����ʃR�[�h���擾���A�������ComboBox�ɐݒ肷��
 *
 * @param nCodeID			[in]	������ʌ��e�ԍ��������Ă���EditBox�̃R�[�hID
 * @param nClassID			[in]	�������ComboBox�̃R�[�hID
 *
 * @return ������ʂ��Z�b�g���邱�Ƃ��ł�����TRUE,�ł��Ȃ�������FALSE
 */
BOOL CAttrBuildingDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
{
	CEdit edit( GetDlgItem( nCodeID ) );
	CString strGenkouCode;
	edit.GetWindowText( strGenkouCode );

	// ���e�ԍ��𔼊p�ɕϊ�
	CString	strGenkou = AheConvertToGenkouNoStr(strGenkouCode);	// ���p,�p��OK,�}�C�i�XNO

	// BACKGROUND_CLASS����R�[�h�擾
	long lSearchClassID = -1;
	if (!strGenkou.IsEmpty())
		lSearchClassID = GetClassByMSNO( strGenkou );

	if (lSearchClassID >= 0)
	{
		// �w�i��ʃR���{�{�b�N�X�擾
		CComboBox	cWAnnoClass( GetDlgItem(nClassID) );
		LONG		lClassNum = cWAnnoClass.GetCount();
		LONG		lComboBoxItemNum = 0;
		DWORD		dwComboBoxValue = 0;
		LONG		lComboBoxValue = 0;
		for (lComboBoxItemNum ; lComboBoxItemNum < lClassNum ; lComboBoxItemNum++)
		{
			dwComboBoxValue = cWAnnoClass.GetItemData(lComboBoxItemNum);
			lComboBoxValue = (INT)dwComboBoxValue;
			if (lComboBoxValue == lSearchClassID)
				break;
		}
		cWAnnoClass.SetCurSel(lComboBoxItemNum);

		SetDlgItemText(nCodeID, strGenkou);			// ���e�ԍ���ݒ�

		m_cControlRel.ControlChanged( nClassID );
		InvalidateRect( NULL, FALSE );
		// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
		SetButton( Changed() );
	} else {
		AttrMessageBox( AheLoadString( IDS_NOMANUSCRIPT ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		ShowGenkouByBackgroundClass();			// ���e�ԍ���ݒ�
		return FALSE;
	}
	return TRUE;
}

/**
 * @fn void CAttrBuildingDlg::ShowGenkouByBackgroundClass()
 * @brief ���ݑI�𒆂̌�����ʂ̌��e�ԍ����A���e�ԍ��t�B�[���h�ɕ\��
 */
void CAttrBuildingDlg::ShowGenkouByBackgroundClass()
{
	CWindow	cWGenkou(GetDlgItem(IDC_EDIT_GENKOU));			// ���e�ԍ��e�L�X�g�{�b�N�X
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
	return;
}

/**
 * @fn void CAttrBuildingDlg::GetBackgroundClass(LONG *lBackgroundClass)
 * @brief ���ݑI�𒆂̌������ID���擾
 *
 * @param	lBackgroundClass		[o]		�I�𒆂̌������ID		
 */
void CAttrBuildingDlg::GetBackgroundClass(LONG *lBackgroundClass)
{
	CComboBox	cWAnnoClass(GetDlgItem(IDC_COMBO_BLDCLASS_C));
	LONG		lSelectIndex = cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @fn BOOL CAttrBuildingDlg::CheckReturnKeyItem(INT nClassID)
 * @brief ���^�[���L�[�������ꂽ���̓���
 *
 * @param	nClassID	[in]		���^�[���L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ���^�[���L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ���^�[���L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrBuildingDlg::CheckReturnKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// ���e�ԍ��G�f�B�b�g�{�b�N�X or �����{�^��
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_BLDCLASS_C) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @fn BOOL CAttrBuildingDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESC�L�[�������ꂽ���̓���
 *
 * @param	ClassID	[i]		ESC�L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ESC�L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ESC�L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrBuildingDlg::CheckEscKeyItem(INT nClassID)
{
	if ((nClassID == IDC_EDIT_GENKOU) || (nClassID == IDC_BUTTON_SEARCH)) {		// ���e�ԍ��G�f�B�b�g�{�b�N�X or �����{�^��
		if( !SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_BLDCLASS_C) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @brief GEOSPACE_ID���ύX���ꂽ���̓���
 *
 * @return �Ȃ�
 */
void CAttrBuildingDlg::SetGeospceID()
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

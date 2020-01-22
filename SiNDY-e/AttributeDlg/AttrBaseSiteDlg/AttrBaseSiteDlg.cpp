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

// AttrBaseSiteDlg.cpp: CAttrBaseSiteDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttrBaseSiteDlg.h"

/// �E�B���h�E�}�l�[�W���z�u�}�b�v�i���k�w�i�|���S���p�j
BEGIN_WINDOW_MAP(BASE_SITE_MAP)
	BEGINROWS(WRCT_REST,0,RCMARGINS(2,2))
		BEGINROWS(WRCT_REST,0,0)
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC4BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC4BGCLASS_C)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU4)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU4)
				RCTOFIT(IDC_BUTTON_SEARCH4)
			ENDGROUP()

			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC3BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC3BGCLASS_C)
				RCTOFIT(IDC_BUTTON_SAME3)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU3)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU3)
				RCTOFIT(IDC_BUTTON_SEARCH3)
			ENDGROUP()

			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC2BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC2BGCLASS_C)
				RCTOFIT(IDC_BUTTON_SAME2)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU2)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU2)
				RCTOFIT(IDC_BUTTON_SEARCH2)
			ENDGROUP()

			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_SC1BGCLASS_C)
				RCTOFIT(-1)
				RCREST(IDC_COMBO_SC1BGCLASS_C)
				RCTOFIT(IDC_BUTTON_SAME1)
			ENDGROUP()
			RCSPACE(10)
			BEGINCOLS(WRCT_TOFIT, 0, RCMARGINS(10,0))
				RCTOFIT(IDC_STATIC_GENKOU1)
				RCTOFIT(-1)
				RCREST(IDC_EDIT_GENKOU1)
				RCTOFIT(IDC_BUTTON_SEARCH1)
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
 * @retval CAttrBaseSiteDlg��Ԃ��܂��B
 */
extern "C" __declspec(dllexport) CExportDlg * __stdcall CreateDlg()
{
    return new CAttrBaseSiteDlg();
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseSiteDlg �����o�֐�
//
/////////////////////////////////////////////////////////////////////////////

/** 
 * @fn CAttrBaseSiteDlg::CAttrBaseSiteDlg()
 * @brief �R���X�g���N�^
 *
 * �u���V�̏����������Ă����܂��B
 */
CAttrBaseSiteDlg::CAttrBaseSiteDlg()
	: ATTR_BASE_CLASS(BASE_SITE_MAP, IDD_ATTRBASESITEDLG),
		m_wnd0( _T("COMBOBOX"), this, IDC_COMBO_SC1BGCLASS_C ), 
		m_wnd1( _T("BUTTON"), this, IDC_BUTTON_SAME1 ), 
		m_wnd2( _T("COMBOBOX"), this, IDC_COMBO_SC2BGCLASS_C ), 
		m_wnd3( _T("BUTTON"), this, IDC_BUTTON_SAME2 ), 
		m_wnd4( _T("COMBOBOX"), this, IDC_COMBO_SC3BGCLASS_C ), 
		m_wnd5( _T("BUTTON"), this, IDC_BUTTON_SAME3 ), 
		m_wnd6( _T("COMBOBOX"), this, IDC_COMBO_SC4BGCLASS_C ),
		m_wnd7( _T("EDIT"), this, IDC_EDIT_GENKOU1 ),
		m_wnd8( _T("EDIT"), this, IDC_EDIT_GENKOU2 ),
		m_wnd9( _T("EDIT"), this, IDC_EDIT_GENKOU3 ),
		m_wnd10( _T("EDIT"), this, IDC_EDIT_GENKOU4 ),
		m_wnd11( _T("BUTTON"), this, IDC_BUTTON_SEARCH1 ), 
		m_wnd12( _T("BUTTON"), this, IDC_BUTTON_SEARCH2 ), 
		m_wnd13( _T("BUTTON"), this, IDC_BUTTON_SEARCH3 ), 
		m_wnd14( _T("BUTTON"), this, IDC_BUTTON_SEARCH4 ),
		m_wnd15( _T("EDIT"), this, IDC_EDIT_SOURCE )	
{
}

/** 
 * @fn HWND CAttrBaseSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
 * @brief �q�_�C�A���O���쐬���܂��B
 *
 * ATTR_BASE_CLASS::Create(HWND hWndParent, LPARAM dwInitParam = NULL)�Ń_�C�A���O���쐬���܂��B
 *
 * @param hWndParent	[in]	�e�E�B���h�E�n���h��
 * @param dwInitParam	[in]	�_�C�A���O�������p�����[�^�i���ݓ��Ɏg�p���Ă��܂���j
 *
 * @retval �쐬���ꂽ�q�_�C�A���O�̃n���h����Ԃ��܂��B
 */
HWND CAttrBaseSiteDlg::Create(HWND hWndParent, LPARAM dwInitParam)
{ 
	m_hChildWnd = ATTR_BASE_CLASS::Create(hWndParent,dwInitParam);
	return m_hChildWnd;
}

/**
 * @fn void CAttrBaseSiteDlg::Delete()
 * @brief �쐬�����q�_�C�A���O���폜���܂��B
 *
 * �_�C�A���O���폜����Ƃ��ɕK���Ă�ŉ������B�Ă΂Ȃ��ƃ��������[�N���Ă��܂��܂��B
 */
void CAttrBaseSiteDlg::Delete()
{
	ClearFeatureDefs();
	::DestroyWindow(m_hChildWnd);
    delete this; // �K��new�Ŋm�ۂ���O��
}

/**
 * @fn BOOL CAttrBaseSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
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
BOOL CAttrBaseSiteDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable )
{ 
	BOOL bRet = ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex, 0, bForce, bEditable );

	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU1);
	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU2);
	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU3);
	ShowGenkouByBackgroundClass(IDC_EDIT_GENKOU4);

	// ���ڃt�B�[�`���Ɋ֌W�Ȃ��G�f�B�b�g�{�b�N�X(��F�Q�Ɨp�̘H�����V�[�P���X�⌴�e�ԍ�����)�Ȃǂ𐧌䂷��
	// �ҏW�J�n�O�ƕҏW�J�n��ł��ҏW�s�̂Ƃ���DISABLE�ɂ���
	if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && bEditable )
	{
		// �ҏW�J�n����Ă���Ƃ��ɂ��łɁASetFocus���Ă���
		SetIMEMode(FALSE);
		::SetFocus( GetDlgItem(IDC_EDIT_GENKOU4) );
	}

	return bRet;
}

/**
 * @fn void CAttrBaseSiteDlg::CreateControlRelation()
 * @brief �q�_�C�A���O�����ۂɍ쐬���܂��B
 *
 * ���ۂ̍쐬��ControlRel�ɔC���܂��B
 */
void CAttrBaseSiteDlg::CreateControlRelation()
{
	ATLASSERT( IsWindow() == TRUE );

	if( m_cControlRel.size() > 0 )
	{
		// �e�E�C���h�E�n���h����ݒ肵�Ȃ���
		m_cControlRel.ResetHWND( m_hWnd );
	} 
	else {
		m_cControlRel.SetHWND( m_hWnd );
		// ���k�w�i�|���S���p

		// S4�̔w�i���
		m_cControlRel.SetControl( IDC_COMBO_SC4BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC4BGCLASS_C"),	NULL, m_strClassName );
		// S4�̌��e�ԍ�����
		m_cControlRel.SetControl( IDC_EDIT_GENKOU4,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH4,		TYPE_BUTTON,	0,	0 );
		// S3�̔w�i���
		m_cControlRel.SetControl( IDC_COMBO_SC3BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC3BGCLASS_C"),	NULL, m_strClassName );
		// �ȏ㓯���Ƀ{�^��
		m_cControlRel.SetControl( IDC_BUTTON_SAME3,			TYPE_BUTTON,	0,	0 );
		// S3�̌��e�ԍ�����
		m_cControlRel.SetControl( IDC_EDIT_GENKOU3,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH3,		TYPE_BUTTON,	0,	0 );
		// S2�̔w�i���
		m_cControlRel.SetControl( IDC_COMBO_SC2BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC2BGCLASS_C"),	NULL, m_strClassName );
		// �ȏ㓯���Ƀ{�^��
		m_cControlRel.SetControl( IDC_BUTTON_SAME2,			TYPE_BUTTON,	0,	0 );
		// S2�̌��e�ԍ�����
		m_cControlRel.SetControl( IDC_EDIT_GENKOU2,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH2,		TYPE_BUTTON,	0,	0 );
		// S1�̔w�i���
		m_cControlRel.SetControl( IDC_COMBO_SC1BGCLASS_C,	TYPE_COMBO,		0,	0,	_T("SC1BGCLASS_C"),	NULL, m_strClassName );
		// �ȏ㓯���Ƀ{�^��
		m_cControlRel.SetControl( IDC_BUTTON_SAME1,			TYPE_BUTTON,	0,	0 );
		// S1�̌��e�ԍ�����
		m_cControlRel.SetControl( IDC_EDIT_GENKOU1,			TYPE_EDIT,		0,	0 );
		m_cControlRel.SetControl( IDC_BUTTON_SEARCH1,		TYPE_BUTTON,	0,	0 );
		// ���\�[�X
		m_cControlRel.SetControl( IDC_EDIT_SOURCE,			TYPE_EDITSTRING,0,	0,	_T("SOURCE"),		NULL, m_strClassName );
	}
}


/**
 * @fn BOOL CAttrBaseSiteDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
 * @brief �w�i��ʐݒ�
 *
 * �w�i���EditBox�ɓ��͂��ꂽ�w�i��ʌ��e�ԍ�����w�i��ʃR�[�h���擾���A�w�i���ComboBox�ɐݒ肷��
 *
 * @param nCodeID			[in]	�w�i��ʌ��e�ԍ��������Ă���EditBox�̃R�[�hID
 * @param nClassID			[in]	�w�i���ComboBox�̃R�[�hID
 *
 * @return �w�i��ʂ��Z�b�g���邱�Ƃ��ł�����TRUE,�ł��Ȃ�������FALSE
 */
BOOL CAttrBaseSiteDlg::SetBackgroundClassCode(INT nCodeID, INT nClassID)
{
	// ���e�ԍ��R�[�h���G�f�B�b�g�{�b�N�X����擾
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
	}
	else
	{
		AttrMessageBox( AheLoadString( IDS_NOMANUSCRIPT ), AheLoadString( IDS_CAPTION ), MB_OK, WARNLEVEL_HIGH );
		ShowGenkouByBackgroundClass(nCodeID);			// ���e�ԍ���ݒ�
		return FALSE;
	}
	return TRUE;
}

/**
 * @fn void CAttrBaseSiteDlg::ShowGenkouByBackgroundClass()
 * @brief ���ݑI�𒆂̔w�i��ʂ̌��e�ԍ����A���e�ԍ��t�B�[���h�ɕ\��
 */
void CAttrBaseSiteDlg::ShowGenkouByBackgroundClass(INT nCodeID)
{
	if( nCodeID == IDC_COMBO_SC1BGCLASS_C)
		nCodeID = IDC_EDIT_GENKOU1;
	else if( nCodeID == IDC_COMBO_SC2BGCLASS_C )
		nCodeID = IDC_EDIT_GENKOU2;
	else if( nCodeID == IDC_COMBO_SC3BGCLASS_C )
		nCodeID = IDC_EDIT_GENKOU3;
	else if( nCodeID == IDC_COMBO_SC4BGCLASS_C )
		nCodeID = IDC_EDIT_GENKOU4;

	CWindow	cWGenkou(GetDlgItem(nCodeID));			// ���e�ԍ��e�L�X�g�{�b�N�X
	long lBackgroundClass = -1;
	GetBackgroundClass(&lBackgroundClass,nCodeID);
	cWGenkou.SetWindowText( GetMSNOByClass( lBackgroundClass ) );
	return;
}

/**
 * @fn void CAttrBaseSiteDlg::GetBackgroundClass(LONG *lBackgroundClass)
 * @brief ���ݑI�𒆂̔w�i���ID���擾
 *
 * @param	lBackgroundClass		[o]		�I�𒆂̔w�i���ID		
 */
void CAttrBaseSiteDlg::GetBackgroundClass(LONG *lBackgroundClass, INT nCodeID)
{
	INT cClassID = 0;

	if( nCodeID == IDC_EDIT_GENKOU1 )
		cClassID = IDC_COMBO_SC1BGCLASS_C;
	else if( nCodeID == IDC_EDIT_GENKOU2 )
		cClassID = IDC_COMBO_SC2BGCLASS_C;
	else if( nCodeID == IDC_EDIT_GENKOU3 )
		cClassID = IDC_COMBO_SC3BGCLASS_C;
	else if( nCodeID == IDC_EDIT_GENKOU4 )
		cClassID = IDC_COMBO_SC4BGCLASS_C;

	CComboBox	cWAnnoClass(GetDlgItem(cClassID));
	LONG		lSelectIndex = cWAnnoClass.GetCurSel();
	DWORD		dbComboBoxValue = cWAnnoClass.GetItemData(lSelectIndex);
	*lBackgroundClass = (LONG)dbComboBoxValue;
}

/**
 * @fn void CAttrBaseSiteDlg::CopyValue(INT nOrgID, INT nCpyID)
 * @brief ���̃X�P�[���Ɍ��e�ԍ����R�s�[����
 *
 * @param	nOrgID		[in]		�R�s�[���̔w�i��ʃR���g���[��ID		
 * @param	nCpyID		[in]		�R�s�[��̔w�i��ʃR���g���[��ID		
 */
void CAttrBaseSiteDlg::CopyValue(INT nOrgID, INT nCpyID)
{
	CControlDef* pControlDef1 = m_cControlRel.GetControlDef( ::GetDlgItem(m_hWnd, nOrgID ) );
	CComVariant vaValue;
	pControlDef1->GetControlValue(vaValue);

	CControlDef* pControlDef2 = m_cControlRel.GetControlDef( ::GetDlgItem(m_hWnd, nCpyID ) );
	pControlDef2->SetValue(vaValue);
	pControlDef2->SetComboBox(vaValue.intVal);
	m_cControlRel.ControlChanged( nCpyID );	

	// �R�s�[���I�����猴�e�ԍ����ύX����
	if( nCpyID == IDC_COMBO_SC1BGCLASS_C)
		nCpyID = IDC_EDIT_GENKOU1;
	else if( nCpyID == IDC_COMBO_SC2BGCLASS_C )
		nCpyID = IDC_EDIT_GENKOU2;
	else if( nCpyID == IDC_COMBO_SC3BGCLASS_C )
		nCpyID = IDC_EDIT_GENKOU3;
	else if( nCpyID == IDC_COMBO_SC4BGCLASS_C )
		nCpyID = IDC_EDIT_GENKOU4;

	ShowGenkouByBackgroundClass(nCpyID);			// ���e�ԍ���ݒ�
}


/**
 * @fn BOOL CAttrBaseSiteDlg::CheckReturnKeyItem(INT nClassID)
 * @brief ���^�[���L�[�������ꂽ���̓���
 *
 * @param	nClassID	[in]		���^�[���L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ���^�[���L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ���^�[���L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrBaseSiteDlg::CheckReturnKeyItem(INT nClassID)
{
	// ���e�ԍ��G�f�B�b�g�{�b�N�X or �����{�^��
	if ((nClassID == IDC_EDIT_GENKOU1) || (nClassID == IDC_BUTTON_SEARCH1) ||
		(nClassID == IDC_EDIT_GENKOU2) || (nClassID == IDC_BUTTON_SEARCH2) ||
		(nClassID == IDC_EDIT_GENKOU3) || (nClassID == IDC_BUTTON_SEARCH3) ||
		(nClassID == IDC_EDIT_GENKOU4) || (nClassID == IDC_BUTTON_SEARCH4))
	{
		// �ǂ̃X�P�[���ŃG���^�[�������������Đݒ肵�Ȃ���
		INT cCodeID = 0;

		if( nClassID == IDC_EDIT_GENKOU1 || nClassID == IDC_BUTTON_SEARCH1)
		{
			cCodeID = IDC_EDIT_GENKOU1;
			nClassID = IDC_COMBO_SC1BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU2 || nClassID == IDC_BUTTON_SEARCH2)
		{
			cCodeID = IDC_EDIT_GENKOU2;
			nClassID = IDC_COMBO_SC2BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU3 || nClassID == IDC_BUTTON_SEARCH3)
		{
			cCodeID = IDC_EDIT_GENKOU3;
			nClassID = IDC_COMBO_SC3BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU4 || nClassID == IDC_BUTTON_SEARCH4)
		{
			cCodeID = IDC_EDIT_GENKOU4;
			nClassID = IDC_COMBO_SC4BGCLASS_C;
		}

		if( !SetBackgroundClassCode(cCodeID, nClassID) )
			return FALSE;
	}
	return TRUE;
}

/**
 * @fn BOOL CAttrBaseRailwayDlg::CheckEscKeyItem(INT nClassID)
 * @brief ESC�L�[�������ꂽ���̓���
 *
 * @param	ClassID	[i]		ESC�L�[�������ꂽ����ItemID
 *
 * @return	TRUE : ESC�L�[�Ƃ��Ă̓���(�K�p����)�� , FALSE : ESC�L�[�Ƃ��Ă̓���s�� 
 */
BOOL CAttrBaseSiteDlg::CheckEscKeyItem(INT nClassID)
{
	// ���e�ԍ��G�f�B�b�g�{�b�N�X or �����{�^��
	if ((nClassID == IDC_EDIT_GENKOU1) || (nClassID == IDC_BUTTON_SEARCH1) ||
		(nClassID == IDC_EDIT_GENKOU2) || (nClassID == IDC_BUTTON_SEARCH2) ||
		(nClassID == IDC_EDIT_GENKOU3) || (nClassID == IDC_BUTTON_SEARCH3) ||
		(nClassID == IDC_EDIT_GENKOU4) || (nClassID == IDC_BUTTON_SEARCH4))
	{
		// �ǂ̃X�P�[���ŃG���^�[�������������Đݒ肵�Ȃ���
		INT cCodeID = 0;

		if( nClassID == IDC_EDIT_GENKOU1 || nClassID == IDC_BUTTON_SEARCH1)
		{
			cCodeID = IDC_EDIT_GENKOU1;
			nClassID = IDC_COMBO_SC1BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU2 || nClassID == IDC_BUTTON_SEARCH2)
		{
			cCodeID = IDC_EDIT_GENKOU2;
			nClassID = IDC_COMBO_SC2BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU3 || nClassID == IDC_BUTTON_SEARCH3)
		{
			cCodeID = IDC_EDIT_GENKOU3;
			nClassID = IDC_COMBO_SC3BGCLASS_C;
		}
		else if( nClassID == IDC_EDIT_GENKOU4 || nClassID == IDC_BUTTON_SEARCH4)
		{
			cCodeID = IDC_EDIT_GENKOU4;
			nClassID = IDC_COMBO_SC4BGCLASS_C;
		}

		if( !SetBackgroundClassCode(cCodeID, nClassID) )
			return FALSE;
	}
	return TRUE;
}


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

// AttrCDSCommonDlg.h : CAttrCdsCommonDlg �̐錾

#ifndef __ATTRCDSCOMMONDLG_H_
#define __ATTRCDSCOMMONDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrCdsCommonDlg
class CAttrCdsCommonDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrCdsCommonDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH_MSNO, BN_CLICKED, OnClickedButtonSearchMSNO)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
		break;

	case IDC_EDIT_SOURCE:
	case IDC_COMBO_CLASS:
	case IDC_EDIT_MSNO:
	case IDC_BUTTON_SEARCH_MSNO:
	case IDC_CHECK_UNDERGROUND:
	case IDC_CHECK_CORRIDOR:
	case IDC_CHECK_NOWALL:
	case IDC_EDIT_BLDNAME:
	case IDC_EDIT_BLDADDR:
	case IDC_EDIT_GEOID:
	case IDC_EDIT_NAMESTRING1:
	case IDC_EDIT_NAMESTRING2:
	case IDC_COMBO_DISPTYPE:
	case IDC_CHECK_HIMAWARI:
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();
		// �T�u�N���X��
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}


		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_COMMAND �p�C�x���g�n���h��
	 *
	 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
	 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
		if( !m_bFinishInit ) return 0;

		INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// ��ʃR�[�h�̃R���{�{�b�N�X�̒l���Z���`�F���W�����Ƃ��ɁA
		// ���e�ԍ����͂̒l�ƒ��L�L����ς���
		if( nTargetControl == IDC_COMBO_CLASS && msg == CBN_SELCHANGE )
		{
			ShowMSNOByClass();
			UpdateDispTypeList();
		}

		// ���e�ԍ����͂̒l���ς�����Ƃ��ɁA
		// ��ʃR�[�h�̃R���{�{�b�N�X�̒l�ƒ��L�L����ς���
		if ( nTargetControl == IDC_EDIT_MSNO && msg == EN_KILLFOCUS && m_cArcHelper.IsStartEditing() )
		{
			SetClassByMSNO(IDC_EDIT_MSNO, IDC_COMBO_CLASS);
			UpdateDispTypeList();
		}

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( msg == CBN_SELCHANGE || msg == BN_CLICKED || msg == EN_UPDATE )
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );

			// ���L�����񂪕ύX���ꂽ�ꍇ�A���������X�V����
			if( nTargetControl == IDC_EDIT_NAMESTRING1 || nTargetControl == IDC_EDIT_NAMESTRING2 )
				AutoUpdateStringNum( nTargetControl );

			// �_�C�A���O���X�V�̈�ɒǉ����܂�
			InvalidateRect( NULL, FALSE );

			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		// GEOSPACE_ID���ύX���ꂽ�Ƃ�
		if( nTargetControl == IDC_EDIT_GEOID && msg == EN_KILLFOCUS && m_cArcHelper.IsStartEditing() )
			SetGeospaceID();

		return 0;
	}

	/**
	 * ��ʃR�[�h�����p�C�x���g�n���h��
	 *
	 * �����{�^���������ꂽ�ۂɁA���e�ԍ�������
	 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnClickedButtonSearchMSNO(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		SetClassByMSNO(IDC_EDIT_MSNO, IDC_COMBO_CLASS);
		// ���L�L�����X�V
		UpdateDispTypeList();

		return 0;
	}

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strClassName = lpcszFeatureClassName;};

	CAttrCdsCommonDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL CheckEscKeyItem(INT nClassID){return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID);};

private:
	void LoadBackgroundRule();
	CString GetMSNOByClass(LONG lClass){LoadBackgroundRule(); return ATTR_BASE_CLASS::GetMSNOByClass(lClass);};
	LONG GetClassByMSNO(CString strMSNO){LoadBackgroundRule(); return ATTR_BASE_CLASS::GetClassByMSNO(strMSNO);};
	void CreateControlRelation();
	BOOL SetClassByMSNO(INT nCodeID, INT nClassID);
	void ShowMSNOByClass();
	void GetClassID(LONG *lClassID);
	void SetGeospaceID();
	BOOL CheckAnnoNameString(CFeatureDef& featureDef, CString& strErr);
	void AutoUpdateStringNum(UINT nTargetControl);
	void UpdateDispTypeList();
	void SetComboBoxSel(INT nComboID, LONG lVal);
	BOOL CheckExceedFieldSize(CFeatureDef& featureDef, CString& strErr);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<INT, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E

	std::map<LONG, LONG> m_mapAnnoDispType; //!< ���L��ʂƒ��L�L���̑Ή��\

	std::set<CString> m_msNoExceptTblList; //!< ���e�ԍ����͑ΏۊO�e�[�u���ꗗ
};

#endif // __ATTRCDSCOMMONDLG_H_

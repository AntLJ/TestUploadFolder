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

// AttrBuildingDlg.h : CAttrBuildingDlg �̐錾

#ifndef __ATTRBUILDINGDLG_H_
#define __ATTRBUILDINGDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrBuildingDlg
class CAttrBuildingDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrBuildingDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH, BN_CLICKED, OnClickedBackgroundClassSearchButton)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_COMBO_BLDCLASS_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENKOU)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_SEARCH)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_CORRIDOR_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_NOWALL_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)	
ALT_MSG_MAP(IDC_EDIT_GEOID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingDlg ���b�Z�[�W�n���h��
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_BLDCLASS_C));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_GENKOU));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_BUTTON_SEARCH));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_CHECK_CORRIDOR_F));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_CHECK_NOWALL_F));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_GEOID));

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

		// �w�i��ʂ̃R���{�{�b�N�X�̒l���Z���`�F���W�����Ƃ��Ɍ��e�ԍ����͂̒l��ς���
		if( ( nTargetControl == IDC_COMBO_BLDCLASS_C ) && ( msg == CBN_SELCHANGE) )
			ShowGenkouByBackgroundClass();

		// ���e�ԍ����͂̒l���ς�����Ƃ��ɁA�w�i��ʂ̃R���{�{�b�N�X�̒l��ς���
		if( nTargetControl == IDC_EDIT_GENKOU && msg == EN_KILLFOCUS )
			SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_BLDCLASS_C);

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		// GEOSPACE_ID���ύX���ꂽ�Ƃ�
		if( nTargetControl == IDC_EDIT_GEOID && msg == EN_KILLFOCUS )
			SetGeospceID();
		return 0;
	}

	/**
	 *�u������ʌ����v�{�^���������ꂽ���̏���
	 *
	 *  ���̃C�x���g�Ɋւ��Ă�CControlDef���ł܂��Ȃ��鏈���ł͂Ȃ��̂ŁA�����ŏ���
	 */
	LRESULT OnClickedBackgroundClassSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		SetBackgroundClassCode(IDC_EDIT_GENKOU, IDC_COMBO_BLDCLASS_C);	

		return 0;
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingDlg �����o�֐���`
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
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strClassName = lpcszFeatureClassName;};

	CAttrBuildingDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL CheckEscKeyItem(INT nClassID);

private:
	void CreateControlRelation();
	BOOL SetBackgroundClassCode(INT nCodeID, INT nClassID);
	void ShowGenkouByBackgroundClass();
	void GetBackgroundClass(LONG *lBackgroundClass);
	void SetGeospceID();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBuildingDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	// TODO: map�g���悤�ɂ���
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5,m_wnd6;	//!< �e�R���g���[���E�B���h�E

	HWND		m_hChildWnd;				//!< �q�_�C�A���O�n���h��
};

#endif //__ATTRBUILDINGDLG_H_

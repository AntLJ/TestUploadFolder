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

// LQAttrGuideDlg.h : CLQAttrGuideDlg �̐錾

#ifndef __LQATTRGUIDEDLG_H_
#define __LQATTRGUIDEDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "resource.h"

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrGuideDlg
class CLQAttrGuideDlg : 
	public LQATTR_COMMON_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrGuideDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	CHAIN_MSG_MAP(LQATTR_COMMON_BASE_CLASS)

ALT_MSG_MAP(IDC_COMBO_GUIDECLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();
		// �T�u�N���X��
		m_wndGuideClass.SubclassWindow(GetDlgItem(IDC_COMBO_GUIDECLASS));
		m_wndSource.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));

		if( m_cArcHelper.IsStartEditing() )
		{
			LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
			::SetFocus( GetDlgItem(IDC_COMBO_GUIDECLASS) );

			return 0;
		}
		else
			return LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
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

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		bHandled = FALSE;
		return 0;
	}

	// ���[�_���̎��ɃL�[��߂܂���
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ LQATTR_COMMON_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ LQATTR_COMMON_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){ LQATTR_COMMON_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){};
	virtual void ClearFeatureDefs(){ LQATTR_COMMON_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CLQAttrGuideDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };

private:
	void CreateControlRelation();
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrGuideDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndGuideClass, m_wndSource;
	ISiNDYRulePtr		m_ipRule;			//!< ��ƖړIDB�Ή�
	HWND				m_hChildWnd;		//!< �q�_�C�A���O�n���h��
};

#endif //__LQATTRGUIDEDLG_H_

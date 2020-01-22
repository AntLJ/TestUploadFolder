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

// LQAttrDirGuideDlg.h : CLQAttrDirGuideDlg �̐錾

#ifndef __LQATTRDIRGUIDEDLG_H_
#define __LQATTRDIRGUIDEDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "resource.h"

#include "LevelOID.h"
#include <iostream>
#include "DirGuideSignDlg.h"

const unsigned int WM_CHANGECURRENTFEATURE = RegisterWindowMessage(_T("WM_CHANGECURRENTFEATURE"));	//!< �I������Ă���t�B�[�`����ύX�˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_DISABLECONTROL  = RegisterWindowMessage(_T("WM_DISABLECONTROL"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrDirGuideDlg
class CLQAttrDirGuideDlg : 
	public LQATTR_COMMON_BASE_CLASS,
	public CExportDlg
{
public:
	CLQAttrDirGuideDlg();

	~CLQAttrDirGuideDlg()
	{
		::DeleteObject( m_hStraightBmp );
		::DeleteObject( m_hStraightGBmp );
		::DeleteObject( m_hRightTurnBmp );
		::DeleteObject( m_hRightTurnGBmp );
		::DeleteObject( m_hLeftTurnBmp );
		::DeleteObject( m_hLeftTurnGBmp );

		::DeleteObject( m_hNullBmp );
		::DeleteObject( m_hNoPatternBmp );
#ifndef FOR_ASEAN // Bug 11268�Ή�
		::DeleteObject( m_hTwoLevelCrossoverABmp );
		::DeleteObject( m_hTwoLevelCrossoverA0Bmp );
		::DeleteObject( m_hTwoLevelCrossoverA1Bmp );
		::DeleteObject( m_hTwoLevelCrossoverA2Bmp );
		::DeleteObject( m_hTwoLevelCrossoverBBmp );
		::DeleteObject( m_hTwoLevelCrossoverB0Bmp );
		::DeleteObject( m_hTwoLevelCrossoverB1Bmp );
		::DeleteObject( m_hTwoLevelCrossoverB2Bmp );
		::DeleteObject( m_hCrossRoadsBmp );
		::DeleteObject( m_hCrossRoads0Bmp );
		::DeleteObject( m_hCrossRoads1Bmp );
		::DeleteObject( m_hCrossRoads2Bmp );
		::DeleteObject( m_hLeftForkedRoadBmp );
		::DeleteObject( m_hLeftForkedRoad0Bmp );
		::DeleteObject( m_hLeftForkedRoad1Bmp );
		::DeleteObject( m_hRightForkedRoadBmp );
		::DeleteObject( m_hRightForkedRoad0Bmp );
		::DeleteObject( m_hRightForkedRoad1Bmp );
		::DeleteObject( m_hYForkedRoadBmp );
		::DeleteObject( m_hYForkedRoad0Bmp );
		::DeleteObject( m_hYForkedRoad1Bmp );
		::DeleteObject( m_hTForkedRoadBmp );
		::DeleteObject( m_hTForkedRoad0Bmp );
		::DeleteObject( m_hTForkedRoad1Bmp );
#else
		::DeleteObject( m_hAseanGreenBmp );
		::DeleteObject( m_hAseanBlueBmp );
		::DeleteObject( m_hAseanWhiteBmp );
		::DeleteObject( m_hAseanOrangeBmp );
#endif // FOR_ASEAN
		::DeleteObject( m_hFont );
	}
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDirGuideDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrDirGuideDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_COMBO_INLINKID, CBN_SELCHANGE, OnChangeInLinkAndNodeID )
	COMMAND_HANDLER(IDC_COMBO_INNODEID2, CBN_SELCHANGE, OnChangeInLinkAndNodeID )
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI1, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI2, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI3, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI4, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI5, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI6, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI7, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI8, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI9, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENKANJI10, EN_UPDATE, OnUpdateHoumenKanji)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI1, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI2, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI3, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI4, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI5, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI6, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI7, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI8, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI9, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_EDIT_HOUMENYOMI10, EN_UPDATE, OnUpdateHoumenYomi)
	COMMAND_HANDLER(IDC_RADIO1, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO2, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO3, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO4, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO5, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO6, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO7, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO8, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO9, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_RADIO10, BN_CLICKED, OnUpdateRadio)
	COMMAND_HANDLER(IDC_BUTTON_INS, BN_CLICKED, OnDirGuideCommandButtonClick)
	COMMAND_HANDLER(IDC_BUTTON_DEL, BN_CLICKED, OnDirGuideCommandButtonClick)
	COMMAND_HANDLER(IDC_BUTTON_UP, BN_CLICKED, OnDirGuideCommandButtonClick)
	COMMAND_HANDLER(IDC_BUTTON_DN, BN_CLICKED, OnDirGuideCommandButtonClick)

	COMMAND_CODE_HANDLER(BN_CLICKED, OnSignButtonClick)		// OnCommand����ɏ������Ȃ��ƁA���b�Z�[�W��j�������\������

	MESSAGE_HANDLER(WM_DISABLECONTROL, OnDisableControl)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_DRAWITEM, OnCtlColorButton)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)

	CHAIN_MSG_MAP(LQATTR_COMMON_BASE_CLASS)

ALT_MSG_MAP(IDC_COMBO_INLINKID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_INNODEID2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU5)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU6)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU7)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU8)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU9)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HOUMENSHUBETU10)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI5)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI6)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI7)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI8)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI9)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENKANJI10)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI5)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI6)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI7)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI8)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI9)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_HOUMENYOMI10)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_STATIC_GROUPBOX)
	COMMAND_CODE_HANDLER(BN_CLICKED, OnDirGuideButtonClick)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_STATIC_GROUPBOX_BTN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_INS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DEL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_UP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDirGuideDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();
		// �T�u�N���X��
		m_wndDirGuideInLink.SubclassWindow(GetDlgItem(IDC_COMBO_INLINKID));
		m_wndDirGuideInNode.SubclassWindow(GetDlgItem(IDC_COMBO_INNODEID2));
		m_wndDirGuideClass1.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU1));
		m_wndDirGuideClass2.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU2));
		m_wndDirGuideClass3.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU3));
		m_wndDirGuideClass4.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU4));
		m_wndDirGuideClass5.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU5));
		m_wndDirGuideClass6.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU6));
		m_wndDirGuideClass7.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU7));
		m_wndDirGuideClass8.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU8));
		m_wndDirGuideClass9.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU9));
		m_wndDirGuideClass10.SubclassWindow(GetDlgItem(IDC_COMBO_HOUMENSHUBETU10));
		m_wndDirGuideKanji1.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI1));
		m_wndDirGuideKanji2.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI2));
		m_wndDirGuideKanji3.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI3));
		m_wndDirGuideKanji4.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI4));
		m_wndDirGuideKanji5.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI5));
		m_wndDirGuideKanji6.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI6));
		m_wndDirGuideKanji7.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI7));
		m_wndDirGuideKanji8.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI8));
		m_wndDirGuideKanji9.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI9));
		m_wndDirGuideKanji10.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENKANJI10));
		m_wndDirGuideYomi1.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI1));
		m_wndDirGuideYomi2.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI2));
		m_wndDirGuideYomi3.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI3));
		m_wndDirGuideYomi4.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI4));
		m_wndDirGuideYomi5.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI5));
		m_wndDirGuideYomi6.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI6));
		m_wndDirGuideYomi7.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI7));
		m_wndDirGuideYomi8.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI8));
		m_wndDirGuideYomi9.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI9));
		m_wndDirGuideYomi10.SubclassWindow(GetDlgItem(IDC_EDIT_HOUMENYOMI10));
		m_wndDirGuideGroup.SubclassWindow(GetDlgItem(IDC_STATIC_GROUPBOX));
		m_wndDirGuideGroupBtn.SubclassWindow(GetDlgItem(IDC_STATIC_GROUPBOX_BTN));
		m_wndDirGuideIns.SubclassWindow(GetDlgItem(IDC_BUTTON_INS));
		m_wndDirGuideDel.SubclassWindow(GetDlgItem(IDC_BUTTON_DEL));
		m_wndDirGuideUp.SubclassWindow(GetDlgItem(IDC_BUTTON_UP));
		m_wndDirGuideDn.SubclassWindow(GetDlgItem(IDC_BUTTON_DN));
		m_wndSource.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wndSequence.SubclassWindow(GetDlgItem(IDC_COMBO_SEQUENCE));
		
		// �t�H���g�ݒ�
		m_wndDirGuideKanji1.SetFont( m_hFont );
		m_wndDirGuideKanji2.SetFont( m_hFont );
		m_wndDirGuideKanji3.SetFont( m_hFont );
		m_wndDirGuideKanji4.SetFont( m_hFont );
		m_wndDirGuideKanji5.SetFont( m_hFont );
		m_wndDirGuideKanji6.SetFont( m_hFont );
		m_wndDirGuideKanji7.SetFont( m_hFont );
		m_wndDirGuideKanji8.SetFont( m_hFont );
		m_wndDirGuideKanji9.SetFont( m_hFont );
		m_wndDirGuideKanji10.SetFont( m_hFont );
		m_wndDirGuideYomi1.SetFont( m_hFont );
		m_wndDirGuideYomi2.SetFont( m_hFont );
		m_wndDirGuideYomi3.SetFont( m_hFont );
		m_wndDirGuideYomi4.SetFont( m_hFont );
		m_wndDirGuideYomi5.SetFont( m_hFont );
		m_wndDirGuideYomi6.SetFont( m_hFont );
		m_wndDirGuideYomi7.SetFont( m_hFont );
		m_wndDirGuideYomi8.SetFont( m_hFont );
		m_wndDirGuideYomi9.SetFont( m_hFont );
		m_wndDirGuideYomi10.SetFont( m_hFont );

		CButton cButton;
		CRect rect( 10, 33, 150, 123 );
		cButton.Create( m_hWnd, rect, _T("CLevelOID"), BS_OWNERDRAW|BS_BITMAP|BS_FLAT|WS_CHILD|WS_VISIBLE, NULL, 2000 );

		m_bNoComment = false;

		if( m_cArcHelper.IsStartEditing() )
		{
			LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
			::SetFocus( GetDlgItem(IDC_COMBO_HOUMENSHUBETU1) );

			return 0;
		}
		else
			return LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	LRESULT OnUpdateHoumenKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) && !m_bSetCurrentFeatureDefIndex)
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

#ifndef FOR_ASEAN // Bug 11268�Ή�(ASEAN�ł͂��̃`�F�b�N�v��Ȃ� ta_suga)
			// TODO:���̕ӁA�֐�������������B�ǂ݂̏����Ƃقړ����ł��邽��(���r���[:4226�̎w�E�����Q��)
			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				CString strHoumenKanji = AheConvertToDirguideName( lpszText, TRUE );
				if( strHoumenKanji.Compare( lpszText ) != 0 )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strHoumenKanji );
					// ���͈ʒu�̏C��
					INT nDiff = CString( lpszText ).GetLength() - strHoumenKanji.GetLength();
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
#endif // FOR_ASEAN
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
			// �����񂪕ύX���ꂽ�Ƃ��ɃG���[�`�F�b�N����ׂ�����Ȃ����ȁH
			m_bNoComment = true;
			ErrorCheck();
			m_bNoComment = false;
		}

		return 0;
	}
	LRESULT OnUpdateHoumenYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) && !m_bSetCurrentFeatureDefIndex)
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

#ifndef FOR_ASEAN // Bug 11268�Ή�(ASEAN�ł͂��̃`�F�b�N�v��Ȃ� ta_suga)
			// TODO:���̕ӁA�֐�������������B�����̏����Ƃقړ����ł��邽��(���r���[:4226�̎w�E�����Q��)
			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				CString strHoumenYomi = AheConvertToDirguideName( lpszText, FALSE );
				if( strHoumenYomi.Compare( lpszText ) != 0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strHoumenYomi );
					// ���͈ʒu�̏C��
					INT nDiff = CString( lpszText ).GetLength() - strHoumenYomi.GetLength();
					edit.SetSel( nSelStart - nDiff, nSelEnd - nDiff );
				}
			}
#endif // FOR_ASEAN
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
			// �����񂪕ύX���ꂽ�Ƃ��ɃG���[�`�F�b�N����ׂ�����Ȃ����ȁH
			m_bNoComment = true;
			ErrorCheck();
			m_bNoComment = false;
		}

		return 0;
	}
	LRESULT OnChangeInLinkAndNodeID(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CComboBox comboLink( GetDlgItem( IDC_COMBO_INLINKID ) );
		INT nIndexLink = comboLink.GetCurSel();
		LPTSTR lpszInLinkID = (LPTSTR)_alloca( ( comboLink.GetLBTextLen( nIndexLink ) + 1 ) * sizeof(TCHAR) );
		comboLink.GetLBText( nIndexLink, lpszInLinkID );

		// �����m�[�hID�̒l���ύX���ꂽ�Ƃ��̂ݗ����m�[�hID���n��
		LPTSTR lpszInNodeID = _T("-1");
		if( wID == IDC_COMBO_INNODEID2 )
		{
			CComboBox comboNode( GetDlgItem( IDC_COMBO_INNODEID2 ) );
			INT nIndexNode = comboNode.GetCurSel();
			lpszInNodeID = (LPTSTR)_alloca( ( comboNode.GetLBTextLen( nIndexNode ) + 1 ) * sizeof(TCHAR) );
			comboNode.GetLBText( nIndexNode, lpszInNodeID );
		}

		LONG lOID = ChangeINLinkAndNodeID( _ttol( lpszInLinkID ), _ttol( lpszInNodeID ) );

		// �I�[�i���������Ƃ��ēn���Ȃ��Ƃ����Ȃ�
		_IRowPtr ipCurrentRow;
		ipCurrentRow = GetCurrentFeatureDef()->GetFeature();
		CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentRow );
		if( !strOwnerName.IsEmpty() )
			strOwnerName += _T(".") + AheLoadString( IDS_DIRGUIDE );
		else
			strOwnerName =  + AheLoadString( IDS_DIRGUIDE );

		return ::SendMessage( GetParent(), WM_CHANGECURRENTFEATURE, (WPARAM)(LPCTSTR)strOwnerName, (LPARAM)lOID );
	}

	LRESULT OnDirGuideButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		InvalidateRect( NULL, FALSE );
		ULONG ulNum = wID - 1000;
		if( ulNum != 1000 )
		{
			if( m_listSameINFeature.size() > ulNum )
			{
				LONG lOID = -1;

				list<CLevelOID>::iterator it = m_listSameINFeature.begin();
				advance( it, ulNum );
				it->m_pLQRowDef->GetFeature()->get_OID( &lOID );

				// �I�[�i���������Ƃ��ēn���Ȃ��Ƃ����Ȃ�
				_IRowPtr ipCurrentRow;
				ipCurrentRow = GetCurrentFeatureDef()->GetFeature();
				CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentRow );
				if( !strOwnerName.IsEmpty() )
					strOwnerName += _T(".") + AheLoadString( IDS_DIRGUIDE );
				else
					strOwnerName =  + AheLoadString( IDS_DIRGUIDE );

				return ::SendMessage( GetParent(), WM_CHANGECURRENTFEATURE, (WPARAM)(LPCTSTR)strOwnerName, (LPARAM)lOID );
			}
		}

		return 0;
	}

	LRESULT OnSignButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( IsEditableFeature() && m_cArcHelper.IsStartEditing() && m_bEditable && m_lLastLink == 0)
		{
			ULONG ulNum = wID;
			if( ulNum == 2000 )
			{
				CLQRowDef *cLQRowDef = GetCurrentFeatureDef();
				if( cLQRowDef )
				{
					CFieldDef* pGuideFieldDef = cLQRowDef->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));
					if( pGuideFieldDef )
					{
						// ���ʊŔp�^�[���I���_�C�A���O���o��
						CWindow wnd( ::GetActiveWindow() );
						wnd.EnableWindow( FALSE );

						CDirGuideSignDlg cDirGuideSign;
						cDirGuideSign.DoModal( m_cArcHelper.GetHWND());
						wnd.EnableWindow( TRUE );

						// �_�C�A���O������Ƀt�H�[�J�X����ʎ�ʂP�̃R���{�{�b�N�X�ɐݒ肵�Ă���
						// [Bug3037]�ɑΉ����邽��
						::SetFocus( GetDlgItem(IDC_COMBO_HOUMENSHUBETU1) );

						// �I�������Ŕ��擾
						LONG lGuide = cDirGuideSign.GetSelectedSign();
						// �L�����Z���{�^���������ꂽ
						if( lGuide == -1 )
							return 0;

						// ���̃��[�����ҏW�ł��邩�ǂ������ׂ�
						CString strMessage;
						bool bIsEdit = true;
						for( list<CLevelOID>::iterator itEditTest = m_listSameINFeature.begin(); itEditTest != m_listSameINFeature.end(); ++itEditTest )
						{
							// �������g�����̃��[�v�ɓ��邪�A���ɖ��Ȃ�
							FEATUREDEF* it = itEditTest->m_pLQRowDef;
							CFieldDef* pGuideFieldDef = it->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));
							CFieldDef* pSeqFieldDef = it->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));
							// �p�x�ɂ������ݒ�
							long dAngle = itEditTest->m_lAngle;
							long lCopyDir;
							if( 159 < dAngle && dAngle < 201 )
								lCopyDir = 0;
							else if( dAngle < 160 )
								lCopyDir = 1;
							else if( dAngle > 200 )
								lCopyDir = 2;

							// �����l���ύX����Ȃ��Ȃ�A��������
							if( pGuideFieldDef->m_vaValue.lVal == lGuide && pSeqFieldDef->m_vaValue.lVal == lCopyDir )
								continue;

							// �l���ύX�����̂ł���΁A���̕��ʂ������ύX�\���ǂ������ׂ�
							sindyeEditableType	sindyeEditableType;
							CComBSTR bstrEditable;
							// �ҏW�\���ǂ������擾
							if( !m_ipRule )
								m_ipRule = GetRule();
							m_ipRule->IsEditableFeature( it->GetFeature(), it->GetShape(), sindyeEditAttribute, &sindyeEditableType, &bstrEditable );

							if( sindyeEditableType == sindyeEditableNoEditable )
							{
								strMessage.Format( AheLoadString( IDS_RULEERROR ), it->GetRowOID(), CString(bstrEditable) );
								AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_OK|MB_ICONSTOP, WARNLEVEL_HIGH );
								bIsEdit = false;
								break;
							}
							else if( sindyeEditableType == sindyeEditableWarnning && !it->GetWarnDelete() )
							{
								strMessage.Format( AheLoadString( IDS_RULEWORNING ), it->GetRowOID(), CString(bstrEditable) );
								int iMsgRet = AttrMessageBox( strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO|MB_ICONWARNING, WARNLEVEL_HIGH );
								if( iMsgRet == IDYES )
								{
									// �ύX���鋖���ł��̂ŁA���[�j���O���x��TRUE�ɂ�ύX
									it->SetWarnDelete();
								}
								else
								{
									bIsEdit = false;
									break;
								}
							}
						}
						// �����ҏW���Ȃ��̂ł���΁A���Ƃ̂܂܂ɂ��Ă���
						if( !bIsEdit )
							return 0;

						_IRowPtr ipRow = cLQRowDef->GetFeature();
						LONG lOriginalGuide = 0;
						LONG lOriginalSeq = 0;
						if( ipRow != NULL )
						{
							IFieldsPtr ipFields;
							LONG lIndex;
							ipRow->get_Fields( &ipFields );
							ipFields->FindField( CComBSTR(_T("GUIDEPATTERN_C")),&lIndex );
							CComVariant vaValue;
							ipRow->get_Value( lIndex, &vaValue );
							lOriginalGuide = vaValue.lVal;

							ipFields->FindField( CComBSTR(_T("SEQUENCE")),&lIndex );
							ipRow->get_Value( lIndex, &vaValue );

							if( vaValue.vt == VT_NULL )
								lOriginalSeq = -1;
							else
								lOriginalSeq = vaValue.lVal;
						}

						// ���̓������������N�̊Ŕɂ������l��ݒ肷��
						for( list<CLevelOID>::iterator itSame = m_listSameINFeature.begin(); itSame != m_listSameINFeature.end(); itSame++ )
						{
							FEATUREDEF* it = itSame->m_pLQRowDef;
							CFieldDef* pCopyFieldDef = it->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));

							_IRowPtr ipCopyRow = it->GetFeature();
							LONG lCopyOriginalGuide = 0;
							LONG lCopyOriginalSeq = 0;
							if( !ipCopyRow )
								continue;

							LONG lO = 0;
							ipCopyRow->get_OID( &lO );
							LONG lIndex = 0;
							IFieldsPtr ipFields;
							ipCopyRow->get_Fields( &ipFields );
							ipFields->FindField( CComBSTR(_T("GUIDEPATTERN_C")),&lIndex );
							CComVariant vaValue;
							ipCopyRow->get_Value( lIndex, &vaValue );
							lCopyOriginalGuide = vaValue.lVal;

							ipFields->FindField( CComBSTR(_T("SEQUENCE")),&lIndex );
							ipCopyRow->get_Value( lIndex, &vaValue );

							if( vaValue.vt == VT_NULL )
								lCopyOriginalSeq = -1;
							else
								lCopyOriginalSeq = vaValue.lVal;

							// �Ŕ����l�ύX
							// �ŏ��ɐݒ肵�Ă����Ȃ��ƁA2��A���ύX�������A�Ŕ����ɖ߂����ꍇ�ɊŔ̐���������Ă��܂�[Bug4089]
							pCopyFieldDef->m_vaValue = lGuide;

							if( lCopyOriginalGuide == lGuide )
								pCopyFieldDef->m_bChanged = FALSE;
							else
								pCopyFieldDef->m_bChanged = TRUE;

							// �V�[�P���X�̍Đݒ�
							pCopyFieldDef = it->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));

							// �p�x�ɂ������ݒ�
							long dAngle = itSame->m_lAngle;
							long lCopyDir;
							if( 159 < dAngle && dAngle < 201 )
								lCopyDir = 0;
							else if( dAngle < 160 )
								lCopyDir = 1;
							else if( dAngle > 200 )
								lCopyDir = 2;

#ifndef FOR_ASEAN // Bug 11268�Ή�
							// �Ŕς����ꍇ�ɂ́ANULL���A�����̃A�C�R���ŕ\�����Ă���������A����ɂ�����Ȃ��ꍇ�ɂ�0�ɃV�[�P���X�ύX����
							if( lGuide == 0 || lGuide == 99 )
							{
								// �V�[�P���X�ݒ�
								// �ŏ��ɐݒ肵�Ă����Ȃ��ƁA2��A���ύX�������A�Ŕ����ɖ߂����ꍇ�ɊŔ̐���������Ă��܂�[Bug4089]
								pCopyFieldDef->m_vaValue.vt = VT_NULL;
								// �ύX�������邩�ǂ������ׂ�
								if( lCopyOriginalSeq == -1 )
									pCopyFieldDef->m_bChanged = FALSE;
								else
									pCopyFieldDef->m_bChanged = TRUE;
							}
							// �\���H�ł���ꍇ�ɂ́A�A�C�R���̕����ɃV�[�P���X�ݒ肷��
							else if( lGuide == 1 || lGuide == 2 || lGuide == 3 )
							{
								// �V�[�P���X�ݒ�
								// �ŏ��ɐݒ肵�Ă����Ȃ��ƁA2��A���ύX�������A�Ŕ����ɖ߂����ꍇ�ɊŔ̐���������Ă��܂�[Bug4089]
								// �ύX�������邩�ǂ������ׂ�
								pCopyFieldDef->m_vaValue = lCopyDir;
								if( lCopyOriginalSeq == lCopyDir )
									pCopyFieldDef->m_bChanged = FALSE;
								else
									pCopyFieldDef->m_bChanged = TRUE;
							}
							else
							{
								if( lGuide >= 4 && lGuide < 8 )
								{
									long lSeqCode = 0;
									// ��T���H�̏ꍇ
									if( lGuide == 4 )
									{
										if( lCopyDir == 0 || lCopyDir == 1 )
											lSeqCode = 0;
										else
											lSeqCode = 1;
									}
									// �ET���H�̏ꍇ
									else if( lGuide == 5 )
									{
										if( lCopyDir == 2 )
											lSeqCode = 0;
										else
											lSeqCode = lCopyDir;
									}
									// Y���H�AT���H�̏ꍇ
									else if( lGuide == 6 || lGuide == 7)
									{
										if( lCopyDir == 2 )
											lSeqCode = 1;
										else
											lSeqCode = 0;
									}
									// �V�[�P���X�ݒ�
									// �ŏ��ɐݒ肵�Ă����Ȃ��ƁA2��A���ύX�������A�Ŕ����ɖ߂����ꍇ�ɊŔ̐���������Ă��܂�[Bug4089]
									pCopyFieldDef->m_vaValue = lSeqCode;
									// �ύX�������邩�ǂ������ׂ�
									if( lCopyOriginalSeq == lSeqCode )
										pCopyFieldDef->m_bChanged = FALSE;
									else
										pCopyFieldDef->m_bChanged = TRUE;
								}
							}
#else
							// �V�[�P���X�ݒ�(ASEAN�̏ꍇ�A��{�I�ɂ͗v��Ȃ����A��NULL����NULL�ɂȂ邱�Ƃ�z�肵�� ta_suga)
							// �ŏ��ɐݒ肵�Ă����Ȃ��ƁA2��A���ύX�������A�Ŕ����ɖ߂����ꍇ�ɊŔ̐���������Ă��܂�[Bug4089]
							pCopyFieldDef->m_vaValue.vt = VT_NULL;
							// �ύX�������邩�ǂ������ׂ�(-1��NULL���Ӗ�����)
							if( lCopyOriginalSeq == -1 )
								pCopyFieldDef->m_bChanged = FALSE;
							else
								pCopyFieldDef->m_bChanged = TRUE;
#endif // FOR_ASEAN
						}
					}
				}
				SetSequence();
				m_bNoComment = true;
				ErrorCheck();
				m_bNoComment = false;
				InvalidateRect( NULL, FALSE );
				// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
				SetButton( Changed() );
			}
		}

		return 0;
	}

	LRESULT OnDirGuideCommandButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		INT nValidCount = GetValidGuideCount();
		INT nSelected = GetSelectedIndex();
		if(nSelected < 0 || nSelected >= nValidCount)
		{
			//�I�΂�Ă��Ȃ��A���̓f�[�^���Ȃ�����I���BUI����Ŕ��ł��Ȃ�����
			return 0;
		}

		switch(wID)
		{
		case IDC_BUTTON_INS:
			{
				if(nValidCount > 9){
					//����Max�ݒ肳��Ă���BUI����Ŕ��ł��Ȃ�����
					return 0;
				}
				for(INT i = nValidCount; i > nSelected; --i)
				{
					CopyGuideItems(i, i-1);
				}
				//�}���s�̎�ʂ�<null>���Ɩ��Ȃ̂�0�Ԗڂɂ���
				SetGuidItems(nSelected, 0, _T(""), _T(""));
				//���ʈē��i�����j�Ƀt�H�[�J�X
				::SetFocus( GetDlgItem(m_vectorControlIDSet[nSelected].nKanjiID) );
			}
			break;
		case IDC_BUTTON_DEL:
			{
				if(nValidCount < 1){
					//�f�[�^���Ȃ��BUI����Ŕ��ł��Ȃ�����
					return 0;
				}
				for(INT i = nSelected; i < nValidCount-1; ++i)
				{
					CopyGuideItems(i, i+1);
				}
				SetGuidItems(nValidCount-1, -1, _T(""), _T(""));
			}
			break;
		case IDC_BUTTON_UP:
			{
				if(nValidCount < 2 || nSelected == 0){
					//�f�[�^��1���͈�ԏ�BUI����Ŕ��ł��Ȃ�����
					return 0;
				}
				SwapGuideItems(nSelected, nSelected-1);
				//���W�I�{�^���̑I����Ԃ��ړ�
				CButton(GetDlgItem( m_vectorControlIDSet[nSelected].nRadioID )).SetCheck(FALSE);
				CButton(GetDlgItem( m_vectorControlIDSet[nSelected-1].nRadioID )).SetCheck(TRUE);
			}
			break;
		case IDC_BUTTON_DN:
			{
				if(nValidCount < 2 || nSelected == nValidCount-1){
					//�f�[�^��1���͈�ԉ��BUI����Ŕ��ł��Ȃ�����
					return 0;
				}
				SwapGuideItems(nSelected, nSelected+1);
				//���W�I�{�^���̑I����Ԃ��ړ�
				CButton(GetDlgItem( m_vectorControlIDSet[nSelected].nRadioID )).SetCheck(FALSE);
				CButton(GetDlgItem( m_vectorControlIDSet[nSelected+1].nRadioID )).SetCheck(TRUE);
			}
			break;
		default:
			break;
		}
		UIUpdate();
		return 0;
	}

	LRESULT OnUpdateRadio(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		UIUpdate();
		return 0;
	}

	LRESULT OnCtlColorButton(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		UINT nCtrlID = wParam;
		if( nCtrlID == 2000 )
		{
			LPDRAWITEMSTRUCT lpDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);

 			RECT rect;
			::GetClientRect(GetDlgItem(nCtrlID), &rect);
			::Rectangle(lpDrawItem->hDC, rect.left, rect.top, rect.right, rect.bottom);

			HDC hmdc = CreateCompatibleDC(lpDrawItem->hDC);

			CLQRowDef *cLQRowDef;
			cLQRowDef = GetCurrentFeatureDef();

			HBITMAP hBitmap;

			if( cLQRowDef )
			{
#ifndef FOR_ASEAN // Bug 11268�Ή�
				CFieldDef* pGuideFieldDef = cLQRowDef->GetFieldDef( 	m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));
				CFieldDef* pSeqFieldDef = cLQRowDef->GetFieldDef( 	m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));

				if( pGuideFieldDef && pSeqFieldDef )
				{
					LONG lGuidePattern;
					lGuidePattern = pGuideFieldDef->m_vaValue.lVal;

					CComVariant vaValue = pSeqFieldDef->m_vaValue;
					LONG lSequence;
					if( vaValue.vt == VT_NULL )
						lSequence = -1;
					else
						lSequence = pSeqFieldDef->m_vaValue.lVal;

					if( lGuidePattern == 0 )
					{
						hBitmap = m_hNoPatternBmp;
					}
					else if( lGuidePattern == 1 )
					{
						if( lSequence == -1)
							hBitmap = m_hTwoLevelCrossoverABmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hTwoLevelCrossoverA0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hTwoLevelCrossoverA1Bmp;
							else if( lSequence == 2 )
								hBitmap = m_hTwoLevelCrossoverA2Bmp;
						}
					}
					else if( lGuidePattern == 2 )
					{
						if( lSequence == -1)
							hBitmap = m_hTwoLevelCrossoverBBmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hTwoLevelCrossoverB0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hTwoLevelCrossoverB1Bmp;
							else if( lSequence == 2 )
								hBitmap = m_hTwoLevelCrossoverB2Bmp;
						}
					}
					else if( lGuidePattern == 3 )
					{
						if( lSequence == -1)
							hBitmap = m_hCrossRoadsBmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hCrossRoads0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hCrossRoads1Bmp;
							else if( lSequence == 2 )
								hBitmap = m_hCrossRoads2Bmp;
						}
					}
					else if( lGuidePattern == 4 )
					{
						if( lSequence == -1)
							hBitmap = m_hLeftForkedRoadBmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hLeftForkedRoad0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hLeftForkedRoad1Bmp;
						}
					}
					else if( lGuidePattern == 5 )
					{
						if( lSequence == -1)
							hBitmap = m_hRightForkedRoadBmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hRightForkedRoad0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hRightForkedRoad1Bmp;
						}
					}
					else if( lGuidePattern == 6 )
					{
						if( lSequence == -1)
							hBitmap = m_hYForkedRoadBmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hYForkedRoad0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hYForkedRoad1Bmp;
						}
					}
					else if( lGuidePattern == 7 )
					{
						if( lSequence == -1)
							hBitmap = m_hTForkedRoadBmp;
						else
						{
							if( lSequence == 0 )
								hBitmap = m_hTForkedRoad0Bmp;
							else if( lSequence == 1 )
								hBitmap = m_hTForkedRoad1Bmp;
						}
					}
					else if( lGuidePattern == 99 )
							hBitmap = m_hNullBmp;				

				}
				else
					hBitmap = m_hNothingGuidePattern_c;
			}
#else
				// ASEAN�̊Ŕݒ�
				// TODO:sindy/schema�̓K�p���s��(���r���[:4226�̎w�E�����Q��)
				CFieldDef* pGuideFieldDef = cLQRowDef->GetFieldDef( 	m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));
				LONG lGuidePattern = -1;
				lGuidePattern = pGuideFieldDef->m_vaValue.lVal;

				switch(lGuidePattern)
				{
				case 0: // ���̑��̐F(�����̃p�^�[���Ȃ�����)
					hBitmap = m_hNoPatternBmp;
					break;
				case 51: // ��
					hBitmap = m_hAseanGreenBmp;
					break;
				case 52: // ��
					hBitmap = m_hAseanBlueBmp;
					break;
				case 53: // ��
					hBitmap = m_hAseanWhiteBmp;
					break;
				case 54: // �I�����W
					hBitmap = m_hAseanOrangeBmp;
					break;
				case 99: // ���ݒ�
					hBitmap = m_hNullBmp;
					break;
				default: // �G���[
					hBitmap = m_hNothingGuidePattern_c;
					break;
				}
			}
#endif // FOR_ASEAN
			SelectObject(hmdc, hBitmap);
			BitBlt(lpDrawItem->hDC, rect.left, rect.top, rect.right, rect.bottom, hmdc, 0, 0, SRCCOPY);
			::DeleteDC(hmdc);
		}
		return 0;
	}

    LRESULT OnDisableControl( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
		m_lLastLink = (long)wParam;
		return 0;
    }

	// ���[�_���̎��ɃL�[��߂܂���
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}

	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = TRUE;		// �e�Ƀ`�F�[�����Ȃ�

		HDC hdc = (HDC) wParam;    // handle to display context 
		HWND hWnd = (HWND) lParam; // handle to static control

		CControlDef* def = m_cControlRel.GetControlDef( hWnd );

		if( def )
		{
			CString strDefName = def->GetFieldName();
			if( strDefName.Find( _T("SEQUENCE")) != -1 && m_bSequenceError && GetCurrentFeatureDef()->m_bError )
			{
				if ( !m_hBrush3 )
					m_hBrush3 = CreateSolidBrush( RGB(255,0,0) );
 
				// Get the DC provided and set the appropriate attributes
 
				SetBkMode(hdc, TRANSPARENT );
				SetBkColor(hdc, RGB(255,0,0));

				// Return a handle to the brush
				HBRUSH far* hbr = (HBRUSH far*) m_hBrush3;
				return ((DWORD) hbr);
			}
			if( def->Errored() ) 
			{
				if( !m_hBrush3 )
					m_hBrush3 = CreateSolidBrush( RGB(255,0,0) );
	 
				// Get the DC provided and set the appropriate attributes
	 
				SetBkMode(hdc, TRANSPARENT );
				SetBkColor(hdc, RGB(255,0,0));

				// Return a handle to the brush
				HBRUSH far* hbr = (HBRUSH far*) m_hBrush3;
				return ((DWORD) hbr);
			}
			else if( def->Changed() )
			{
				if( !m_hBrush1 )
					m_hBrush1 = CreateSolidBrush( RGB(255,168,126) );
	 
				// Get the DC provided and set the appropriate attributes
	 
				SetBkMode(hdc, TRANSPARENT );
				SetBkColor(hdc, RGB(255,168,126));

				// Return a handle to the brush
				HBRUSH far* hbr = (HBRUSH far*) m_hBrush1;
				return ((DWORD) hbr);
			} 
		}
		if( SetCtrlColorChange( hWnd ) )
		{
			if( !m_hBrush2 )
				m_hBrush2 = CreateSolidBrush( RGB(150,240,255) );
 
			// Get the DC provided and set the appropriate attributes
 
			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(150,240,255));

			// Return a handle to the brush
			HBRUSH far* hbr = (HBRUSH far*) m_hBrush2;
			return ((DWORD) hbr);
		}
		CComboBox box( hWnd );
		if( box.GetCount() > 0 && !box.IsWindowEnabled() )
		{
			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(m_lComboR,m_lComboG,m_lComboB));

			if( !m_hBrushCombo )
				m_hBrushCombo = CreateSolidBrush(RGB(m_lComboR,m_lComboG,m_lComboB));

			HBRUSH far* hbr = (HBRUSH far*) m_hBrushCombo;
			return ((DWORD)hbr );		
		}

		return 0;
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

		if( nTargetControl == IDC_EDIT_HOUMENKANJI1 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI2 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI3 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI4 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI5 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI6 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI7 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI8 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI9 ||
			nTargetControl == IDC_EDIT_HOUMENKANJI10 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI1 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI2 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI3 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI4 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI5 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI6 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI7 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI8 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI9 ||
			nTargetControl == IDC_EDIT_HOUMENYOMI10 )
		{
			if( msg == EN_KILLFOCUS )
			{
				SetIMEMode( FALSE );
				SetToolTip( nTargetControl, TRUE );
			}
			else if( msg == EN_SETFOCUS )
				SetIMEMode( TRUE );
		}

		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �V�[�P���X�̃R���{�{�b�N�X�̏ꍇ�ɂ͒l��ݒ肵�Ȃ���΂Ȃ�Ȃ�
			if( nTargetControl == IDC_COMBO_SEQUENCE )
			{
				CLQRowDef *cLQRowDef;
				cLQRowDef = GetCurrentFeatureDef();

				if( cLQRowDef )
				{
					CFieldDef* pGuideFieldDef = cLQRowDef->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("GUIDEPATTERN_C"));
					CFieldDef* pSeqFieldDef = cLQRowDef->GetFieldDef( m_cLQInfo.GetLoadQueueInfo( sindyeRoadQueueDirguide, sindyeLoadQueueInfoInfoTableName ), _T("SEQUENCE"));

					if( pGuideFieldDef && pSeqFieldDef )
					{
#ifndef FOR_ASEAN // Bug 11268�Ή� (�����̏ꍇ�̓V�[�P���X�`�F�b�N)
						LONG lGuidePattern = pGuideFieldDef->m_vaValue.lVal;

						CComboBox cCombo(GetDlgItem( IDC_COMBO_SEQUENCE ));
						LONG lSelectSeq = cCombo.GetCurSel();
						if( lGuidePattern == 0 || lGuidePattern == 99 )
							pSeqFieldDef->m_vaValue.vt = VT_NULL;
						else
							pSeqFieldDef->m_vaValue.lVal = lSelectSeq;
#else
						pSeqFieldDef->m_vaValue.vt = VT_NULL;
#endif // FOR_ASEAN
					}
				}
				SetSequence();
			}
			else
			{
				// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
				m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			}
			UIUpdate();

			m_bNoComment = true;
			ErrorCheck();
			m_bNoComment = false;

			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		// ���ʂɊւ��ẮA���ʂȏ��������Ă��邽�߁A�e�ɂ̓��b�Z�[�W���܂킳�Ȃ�
		bHandled = TRUE;	// [bug8447][e]���ʂ̑����ҏW�ɂāA�V�[�P���X�ԍ���ύX����ƌ��ɖ߂�Ȃ��Ȃ�

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDirGuideDlg �����o�֐���`
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
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ m_bNoComment = (BOOL)wParam; return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
private:
	/**
	 * @struct CONTROL_ID_SET
	 * @brief �֘A����R���g���[��ID�̍\����
	 */
	typedef struct ControlIDSet{
		INT	nRadioID;	// ���W�I�{�^��ID
		INT	nClassID;	// ���ʎ�ʃR���{�{�b�N�XID
		INT	nKanjiID;	// ���ʈē��i�����j�G�f�B�b�g�R���g���[��ID
		INT	nYomiID;	// ���ʈē��i�ǂ݁j�G�f�B�b�g�R���g���[��ID
		ControlIDSet(const INT nRadio, const INT nClass, const INT nKanji, const INT nYomi)
		{
			nRadioID = nRadio;
			nClassID = nClass;
			nKanjiID = nKanji;
			nYomiID = nYomi;
		}
	} CONTROL_ID_SET;

	void CreateControlRelation();
	void CreateDirGuideButton();
	void SetSequence();
	void SortByAngle( CLQRowDef* pRowDef, LONG lStartLinkID, list<FEATUREDEF*>& listOrg, list<CLevelOID>& listNew );
	DOUBLE GetAngle( IPolylinePtr ipPolyline, LONG lStart = 0, BOOL bIsDrawAngle = FALSE );
	DOUBLE GetAngle2( IPolylinePtr ipPolyline, LONG lStart, LONG lEnd );
	HBITMAP CopyBitmap ( HBITMAP hBitmapSrc, INT nTimes = 1 );
	LONG ChangeINLinkAndNodeID( LONG lInLinkID, LONG lInNodeID );
	LONG GetDirGuideOID( FEATUREDEF* pFeatDef );
	LONG GetDirGuideInLinkID( FEATUREDEF* pFeatDef );
	LONG GetDirGuideOutLinkID( FEATUREDEF* pFeatDef );
	LONG GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName );
	IFeatureClassPtr GetRoadLinkWithLQ( CLQRowDef* pRowDef );
	IFeaturePtr GetRoadFeature( CLQRowDef* pRowDef, LONG lOID );
	BOOL SetCtrlColorChange(HWND hTargetWND);
	void SetToolTip(INT nTargetItem, BOOL bIsChange = FALSE);
	void InitToolTip(INT nTargetItem, TOOLINFO *ti);

	inline INT GetHoumenID(LONG Index){
		switch (Index)
		{
			case 1:
				return IDC_EDIT_HOUMENKANJI1;
			case 2:
				return IDC_EDIT_HOUMENKANJI2;
			case 3:
				return IDC_EDIT_HOUMENKANJI3;
			case 4:
				return IDC_EDIT_HOUMENKANJI4;
			case 5:
				return IDC_EDIT_HOUMENKANJI5;
			case 6:
				return IDC_EDIT_HOUMENKANJI6;
			case 7:
				return IDC_EDIT_HOUMENKANJI7;
			case 8:
				return IDC_EDIT_HOUMENKANJI8;
			case 9:
				return IDC_EDIT_HOUMENKANJI9;
			case 10:
				return IDC_EDIT_HOUMENKANJI10;
			case 11:
				return IDC_EDIT_HOUMENYOMI1;
			case 12:
				return IDC_EDIT_HOUMENYOMI2;
			case 13:
				return IDC_EDIT_HOUMENYOMI3;
			case 14:
				return IDC_EDIT_HOUMENYOMI4;
			case 15:
				return IDC_EDIT_HOUMENYOMI5;
			case 16:
				return IDC_EDIT_HOUMENYOMI6;
			case 17:
				return IDC_EDIT_HOUMENYOMI7;
			case 18:
				return IDC_EDIT_HOUMENYOMI8;
			case 19:
				return IDC_EDIT_HOUMENYOMI9;
			case 20:
				return IDC_EDIT_HOUMENYOMI10;
			default:
				return 0;
		}
	}
	INT GetSelectedIndex(void);
	INT GetValidGuideCount(void);
	BOOL CopyGuideItems(INT nTo, INT nFrom);
	BOOL SetGuidItems(INT nIndex, INT nClass, LPCTSTR strKanji, LPCTSTR strYomi);
	BOOL SwapGuideItems(INT nIndex1, INT nIndex2);
	void UIUpdate(void);

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDirGuideDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndDirGuideClass1, m_wndDirGuideClass2, m_wndDirGuideClass3, m_wndDirGuideClass4, m_wndDirGuideClass5;
	CContainedWindow		m_wndDirGuideClass6, m_wndDirGuideClass7, m_wndDirGuideClass8, m_wndDirGuideClass9, m_wndDirGuideClass10;
	CContainedWindow		m_wndDirGuideKanji1, m_wndDirGuideKanji2, m_wndDirGuideKanji3, m_wndDirGuideKanji4, m_wndDirGuideKanji5;
	CContainedWindow		m_wndDirGuideKanji6, m_wndDirGuideKanji7, m_wndDirGuideKanji8, m_wndDirGuideKanji9, m_wndDirGuideKanji10;
	CContainedWindow		m_wndDirGuideYomi1, m_wndDirGuideYomi2, m_wndDirGuideYomi3, m_wndDirGuideYomi4, m_wndDirGuideYomi5;
	CContainedWindow		m_wndDirGuideYomi6, m_wndDirGuideYomi7, m_wndDirGuideYomi8, m_wndDirGuideYomi9, m_wndDirGuideYomi10;
	CContainedWindow		m_wndDirGuideInLink, m_wndDirGuideInNode, m_wndDirGuideGroup, m_wndSource, m_wndSequence;
	CContainedWindow		m_wndDirGuideGroupBtn, 	m_wndDirGuideIns,m_wndDirGuideDel, m_wndDirGuideUp,	m_wndDirGuideDn;
	
	// TODO:���̕ӂ�HBITMAP����CBitmap�ɕύX���邱�Ƃ���������(���r���[:4226�̎w�E�����Q��)
	HBITMAP					m_hStraightBmp, m_hStraightGBmp, m_hRightTurnBmp, m_hRightTurnGBmp, m_hLeftTurnBmp, m_hLeftTurnGBmp;
	HBITMAP					m_hNullBmp,m_hNoPatternBmp,m_hNothingGuidePattern_c;
	
#ifndef FOR_ASEAN // Bug 11268�Ή�
	HBITMAP					m_hTwoLevelCrossoverABmp,m_hTwoLevelCrossoverA0Bmp,
							m_hTwoLevelCrossoverA1Bmp,m_hTwoLevelCrossoverA2Bmp,m_hTwoLevelCrossoverBBmp,
							m_hTwoLevelCrossoverB0Bmp,m_hTwoLevelCrossoverB1Bmp,m_hTwoLevelCrossoverB2Bmp,
							m_hCrossRoadsBmp,m_hCrossRoads0Bmp,m_hCrossRoads1Bmp,m_hCrossRoads2Bmp,
							m_hLeftForkedRoadBmp,m_hLeftForkedRoad0Bmp,m_hLeftForkedRoad1Bmp,
							m_hRightForkedRoadBmp,m_hRightForkedRoad0Bmp,m_hRightForkedRoad1Bmp,
							m_hYForkedRoadBmp,m_hYForkedRoad0Bmp,m_hYForkedRoad1Bmp,
							m_hTForkedRoadBmp,m_hTForkedRoad0Bmp,m_hTForkedRoad1Bmp;
#else
	HBITMAP					m_hAseanGreenBmp,m_hAseanBlueBmp,m_hAseanWhiteBmp,m_hAseanOrangeBmp;
#endif // FOR_ASEAN
	list<CLevelOID>			m_listSameINFeature;	//!< �������������N�������ʈē��i�[�p
	HBRUSH					m_hBrush1, m_hBrush2, m_hBrush3;	//!< �R�����R���g���[���̐F�ύX�p�u���V
	HFONT					m_hFont;
	CWindow					m_hToolTipWnd;
	HWND					m_TwndKanji1,m_TwndKanji2,m_TwndKanji3,m_TwndKanji4,m_TwndKanji5,
							m_TwndKanji6,m_TwndKanji7,m_TwndKanji8,m_TwndKanji9,m_TwndKanji10;
	HWND					m_TwndKana1,m_TwndKana2,m_TwndKana3,m_TwndKana4,m_TwndKana5,
							m_TwndKana6,m_TwndKana7,m_TwndKana8,m_TwndKana9,m_TwndKana10,m_HWNDHoumen[20];
	CWindow					m_CwndKanji;
	ISiNDYRulePtr			m_ipRule;			//!< ��ƖړIDB�Ή�
	HWND					m_hChildWnd;		//!< �q�_�C�A���O�n���h��
	LONG					m_lDirection;
	BOOL					m_bEditable;
	BOOL					m_bNoComment;
	BOOL					m_bSequenceError;
	long					m_lLastLink;
	bool					m_bSetCurrentFeatureDefIndex;
	vector<CONTROL_ID_SET>	m_vectorControlIDSet;
};

#endif //__LQATTRDIRGUIDEDLG_H_

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

#ifndef __SINDY_CONNECTION_DLG_H__
#define __SINDY_CONNECTION_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <io.h>
#include <Commdlg.h>
#include <dlgs.h>
#include <atlapp.h>
#include <atlctrls.h>
#include "SiNDYConnectionFileListCtrl.h"
#include "SiNDYConnectionDlgI.h"

namespace sindy {

/**
 * @brief �ڑ��t�@�C���_�C�A���O�N���X
 *
 * �u�t�@�C�����J���v�R�����_�C�A���O�ɁAArcCatalog��SDE�t�@�C���ꗗ��\���E�ҏW����@�\��ǉ������_�C�A���O���Ǘ�����N���X�B
 */
class ConnectionDlg : public ConnectionDlgI
{
public:
// ����
	BOOL DoModal(HWND hParentWnd);

private:
	/**
	 * �t�b�N�v���V�[�W���ɓn�����J�X�^���f�[�^�B<br>
	 * ConnectionFileListCtrl�����notify�ɑΏ����邽�߁ACStatic���p�������Ă���B
	 */
	class HookProc : public ConnectionDlgI::HookProcI, public CWindowImpl<HookProc, CStatic>
	{
	public:
	BEGIN_MSG_MAP(ConnectionDlg::HookProc)
		NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemchanged)
		NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDblclk)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	// �n���h���̃v���g�^�C�v:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

		LRESULT OnItemchanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		LRESULT OnDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	public:
	// ����
		void operator()(LPOFNOTIFY lpof);

	private:
	// ����
		HWND Create(HWND hWndParent, RECT& rcPos);

	// �ϐ�
		ConnectionFileListCtrl m_ConnectionFileListCtrl;
	};
};

//////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////

// ConnectionDlg
//////////////////////////////////////////////////////////////////////

inline BOOL ConnectionDlg::DoModal(HWND hParentWnd)
{
	// OPENFILENAME�\���̂ɐe�E�B���h�E�n���h����ݒ�B
	m_OFN.hwndOwner = hParentWnd;

	// �t�@�C�����擾�J�n�B
	HookProc aHookProc;
	m_OFN.lCustData = reinterpret_cast<LPARAM>(&aHookProc);
	BOOL bResult = GetOpenFileName(&m_OFN);

	// �ǂݍ��݉\�ȁc
	if(::_taccess(m_OFN.lpstrFile, 04) == 0) {
		// Shape�t�@�C���ւ̐ڑ���]�ޏꍇ�c
		LPTSTR p = ::_tcsrchr(m_OFN.lpstrFile, _T('.'));
		if(::_tcsicmp(p, _T(".shp")) == 0) {
			// �t�@�C�������폜����B
			LPTSTR p = ::_tcsrchr(m_OFN.lpstrFile, _T('\\'));
			*(p ? p : m_OFN.lpstrFile) = 0;
		}
	}

	return bResult;
}

// ConnectionDlg::HookProc
//////////////////////////////////////////////////////////////////////

inline void ConnectionDlg::HookProc::operator()(LPOFNOTIFY lpof)
{
	CListViewCtrl lv2 = ::GetDlgItem(lpof->hdr.hwndFrom, lst2);
	CComboBox cb1 = ::GetDlgItem(lpof->hdr.hwndFrom, cmb1);
	switch(lpof->hdr.code) {
	case CDN_FOLDERCHANGE:
		if(! IsWindow()) {
			// lst2�Ɠ����`��́A�ڑ��v���p�e�B�I�����X�g�𐶐��B
			RECT rect;
			lv2.GetWindowRect(&rect);
			CWindow(lpof->hdr.hwndFrom).ScreenToClient(&rect);

			Create(lpof->hdr.hwndFrom, rect);

			// lst2���B���A�ڑ��v���p�e�B�I�����X�g��\���B
			lv2.ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
		else {
			if(cb1.GetCurSel() == 0) {
				// �t�B���^��sde�ł������ꍇ�A�t�B���^��mdb�ɕύX�B
				cb1.SetCurSel(1);

				//���̎|��e�ɒʒm
				::SendMessage(
					lpof->hdr.hwndFrom,
					WM_COMMAND,
					MAKEWPARAM(::GetDlgCtrlID(cb1.m_hWnd), CBN_SELENDOK),
					reinterpret_cast<LPARAM>(cb1.m_hWnd));
			}
		}
		break;
	case CDN_TYPECHANGE:
		if(cb1.GetCurSel() == 0) {
			// �t�B���^��sde�ɕύX���ꂽ�ꍇ�Alst2���B���Đڑ��v���p�e�B���X�g��\���B
			lv2.ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
		else if(IsWindowVisible()) {
			// �t�B���^��sde�ȊO�ɕύX���ꂽ�ꍇ�A�ڑ��v���p�e�B���X�g���B����lst2��\���B
			lv2.ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
		break;
	case CDN_FILEOK:
		if(cb1.GetCurSel() == 0) {
			// �t�B���^��sde�ł������ꍇ�A�t�H���_�����K��̂��̂ɒu��������B
			::_tcscpy(lpof->lpOFN->lpstrFile, m_ConnectionFileListCtrl.GetConnectionFileFolder().getPath());
			::_tcscat(lpof->lpOFN->lpstrFile, _T("\\"));
			::_tcscat(lpof->lpOFN->lpstrFile, lpof->lpOFN->lpstrFileTitle);
		}
	}
}

/**
 * @brief �ڑ��v���p�e�B���I�����ꂽ�ۂ̏���
 */
inline LRESULT ConnectionDlg::HookProc::OnItemchanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMLISTVIEW pnmListView = reinterpret_cast<LPNMLISTVIEW>(pnmh);

	TCHAR szBuffer[256];
	m_ConnectionFileListCtrl.GetItemText(pnmListView->iItem, 0, szBuffer, 256);

	CommDlg_OpenSave_SetControlText(GetParent(), edt1, szBuffer);

	return 0;
}

/**
 * @brief �ڑ��v���p�e�B���X�g���Ń_�u���N���b�N���ꂽ�ۂ̏���
 */
inline LRESULT ConnectionDlg::HookProc::OnDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if(::SendMessage(m_ConnectionFileListCtrl.m_hWnd, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0)) == -1)
		return 0;

	HWND hParentWnd = GetParent();

	::PostMessage(
		hParentWnd,
		WM_COMMAND,
		MAKEWPARAM(IDOK, BN_CLICKED),
		reinterpret_cast<LPARAM>(::GetDlgItem(hParentWnd, IDOK)));

	return 0;
}

/**
 * @brief �ڑ��v���p�e�B���X�g���\�z����
 */
inline HWND ConnectionDlg::HookProc::Create(HWND hWndParent, RECT& rcPos)
{
	if(! CWindowImpl<HookProc, CStatic>::Create(hWndParent, rcPos, NULL, WS_CHILD | WS_VISIBLE))
		return 0;

	RECT rect = { 0, 0, rcPos.right - rcPos.left, rcPos.bottom - rcPos.top };

	m_ConnectionFileListCtrl.Create(
		m_hWnd, rect, NULL,
		WS_CHILD | WS_VISIBLE | LVS_LIST | LVS_EDITLABELS | LVS_SINGLESEL,
		WS_EX_CLIENTEDGE);
	m_ConnectionFileListCtrl.ShowWindow(SW_SHOW);
	m_ConnectionFileListCtrl.ResetConnectionFileList();

	return m_hWnd;
}

} // namespace sindy

#endif // __SINDY_CONNECTION_DLG_H__

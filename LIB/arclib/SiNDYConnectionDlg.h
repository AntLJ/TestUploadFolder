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
 * @brief 接続ファイルダイアログクラス
 *
 * 「ファイルを開く」コモンダイアログに、ArcCatalogのSDEファイル一覧を表示・編集する機能を追加したダイアログを管理するクラス。
 */
class ConnectionDlg : public ConnectionDlgI
{
public:
// 処理
	BOOL DoModal(HWND hParentWnd);

private:
	/**
	 * フックプロシージャに渡されるカスタムデータ。<br>
	 * ConnectionFileListCtrlからのnotifyに対処するため、CStaticを継承させている。
	 */
	class HookProc : public ConnectionDlgI::HookProcI, public CWindowImpl<HookProc, CStatic>
	{
	public:
	BEGIN_MSG_MAP(ConnectionDlg::HookProc)
		NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemchanged)
		NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDblclk)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	// ハンドラのプロトタイプ:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

		LRESULT OnItemchanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		LRESULT OnDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	public:
	// 処理
		void operator()(LPOFNOTIFY lpof);

	private:
	// 処理
		HWND Create(HWND hWndParent, RECT& rcPos);

	// 変数
		ConnectionFileListCtrl m_ConnectionFileListCtrl;
	};
};

//////////////////////////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////////////////////////

// ConnectionDlg
//////////////////////////////////////////////////////////////////////

inline BOOL ConnectionDlg::DoModal(HWND hParentWnd)
{
	// OPENFILENAME構造体に親ウィンドウハンドルを設定。
	m_OFN.hwndOwner = hParentWnd;

	// ファイル名取得開始。
	HookProc aHookProc;
	m_OFN.lCustData = reinterpret_cast<LPARAM>(&aHookProc);
	BOOL bResult = GetOpenFileName(&m_OFN);

	// 読み込み可能な…
	if(::_taccess(m_OFN.lpstrFile, 04) == 0) {
		// Shapeファイルへの接続を望む場合…
		LPTSTR p = ::_tcsrchr(m_OFN.lpstrFile, _T('.'));
		if(::_tcsicmp(p, _T(".shp")) == 0) {
			// ファイル名を削除する。
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
			// lst2と同じ形状の、接続プロパティ選択リストを生成。
			RECT rect;
			lv2.GetWindowRect(&rect);
			CWindow(lpof->hdr.hwndFrom).ScreenToClient(&rect);

			Create(lpof->hdr.hwndFrom, rect);

			// lst2を隠し、接続プロパティ選択リストを表示。
			lv2.ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
		else {
			if(cb1.GetCurSel() == 0) {
				// フィルタがsdeであった場合、フィルタをmdbに変更。
				cb1.SetCurSel(1);

				//その旨を親に通知
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
			// フィルタがsdeに変更された場合、lst2を隠して接続プロパティリストを表示。
			lv2.ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
		else if(IsWindowVisible()) {
			// フィルタがsde以外に変更された場合、接続プロパティリストを隠してlst2を表示。
			lv2.ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
		break;
	case CDN_FILEOK:
		if(cb1.GetCurSel() == 0) {
			// フィルタがsdeであった場合、フォルダ名を規定のものに置き換える。
			::_tcscpy(lpof->lpOFN->lpstrFile, m_ConnectionFileListCtrl.GetConnectionFileFolder().getPath());
			::_tcscat(lpof->lpOFN->lpstrFile, _T("\\"));
			::_tcscat(lpof->lpOFN->lpstrFile, lpof->lpOFN->lpstrFileTitle);
		}
	}
}

/**
 * @brief 接続プロパティが選択された際の処理
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
 * @brief 接続プロパティリスト内でダブルクリックされた際の処理
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
 * @brief 接続プロパティリストを構築する
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

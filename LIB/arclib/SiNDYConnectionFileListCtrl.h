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

#ifndef __SINDY_CONNECTION_FILE_LISTCTRL_H__
#define __SINDY_CONNECTION_FILE_LISTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include "SiNDYConnectionFileCtrlI.h"

namespace sindy {

/**
 * @brief 接続ファイル一覧リストコントロール
 */
class ConnectionFileListCtrl : public CWindowImpl<ConnectionFileListCtrl, CListViewCtrl>, public ConnectionFileCtrlI
{
public:
BEGIN_MSG_MAP(ConnectionFileListCtrl)
	if(uMsg == OCM_NOTIFY) {
		uMsg -= OCM__BASE;
		NOTIFY_CODE_HANDLER(NM_RCLICK, OnRclick)
		NOTIFY_CODE_HANDLER(LVN_ENDLABELEDIT, OnEndlabeledit)
		uMsg += OCM__BASE;
	}
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnRclick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnEndlabeledit(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

public:
	virtual void ResetConnectionFileList();
};

//////////////////////////////////////////////////////////////////////
// インライン実装
//////////////////////////////////////////////////////////////////////

// メッセージハンドラ
//////////////////////////////////////////////////////////////////////

inline LRESULT ConnectionFileListCtrl::OnRclick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMLISTVIEW pnmListView = reinterpret_cast<LPNMLISTVIEW>(pnmh);

	int nMenu = PopupConnectionFileMenu(m_hWnd, pnmListView->iItem >= 0);
	if(nMenu) {
		TCHAR szFileName[256] = { 0 };
		if(pnmListView->iItem != -1) {
			GetItemText(pnmListView->iItem, 0, szFileName, 256);
		}

		if(ConnectionFileFunction(m_hWnd, static_cast<EConnectionFileMenu>(nMenu), szFileName)) {
			ResetConnectionFileList();
		}
	}

	return TRUE;
}

inline LRESULT ConnectionFileListCtrl::OnEndlabeledit(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	LV_DISPINFO *pLVDispInfo = reinterpret_cast<LV_DISPINFO*>(pnmh);

	if(pLVDispInfo->item.pszText) {
		CString strNewName = pLVDispInfo->item.pszText;
		TCHAR szOldName[256];
		GetItemText(pLVDispInfo->item.iItem, 0, szOldName, 256);
		if(RenameConnectionFile(szOldName, strNewName)) {
			ResetConnectionFileList();
			return FALSE;
		}
	}

	return TRUE;
}

// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief SDEファイル一覧を更新する
 */
inline void ConnectionFileListCtrl::ResetConnectionFileList()
{
	DeleteAllItems();

	ConnectionFile::Finder aFinder(GetConnectionFileFolder());

	int nItem = 0;
	LPWIN32_FIND_DATA lpFindData = 0;
	while((lpFindData = aFinder.next())) {
		InsertItem(nItem++, lpFindData->cFileName);
	}
}

} // namespace sindy


#endif // __SINDY_CONNECTION_FILE_LISTCTRL_H__

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

#include "stdafx.h"
#include <shlobj.h>
#include <oleidl.h>
#include "SiNDYConnectionFileCtrlI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// ConnectionFileCtrlI
//////////////////////////////////////////////////////////////////////

ConnectionFileCtrlI::ConnectionFileCtrlI()
{
}

/**
 * @brief SDEファイル操作メニューを表示する
 *
 * @param hWnd [in] 親HWND。
 * @param bSelected [in] SDEファイルが選択されている。
 * @retval 0 メニューは選択されなかった。
 * @retval kConnectionFileAdd 「追加」が選択された。
 * @retval kConnectionFileEdit 「編集」が選択された。
 * @retval kConnectionFileDelete 「削除」が選択された。
 */
int ConnectionFileCtrlI::PopupConnectionFileMenu(HWND hWnd, bool bSelected)
{
	HMENU hMenu = ::CreateMenu();
	HMENU hPopup = ::CreateMenu();

	unsigned int nItem = 0;
	if(bSelected) {
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileEdit, _T("編集(&E)"));
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileDelete, _T("削除(&D)"));
		::InsertMenu(hPopup, nItem++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);
	}
	::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileAdd, _T("追加(&A)"));
	if(bSelected) {
		::InsertMenu(hPopup, nItem++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileCopy, _T("コピー(&C)"));
	}
	::InsertMenu(hMenu, nItem++, MF_POPUP | MF_BYPOSITION, reinterpret_cast<UINT>(hPopup), 0);

	POINT pt;
	::GetCursorPos(&pt);

	int nCommand = ::TrackPopupMenu(hPopup, TPM_RETURNCMD, pt.x, pt.y, 0, hWnd, 0);

	::DestroyMenu(hPopup);
	::DestroyMenu(hMenu);

	return nCommand;
}

/**
 * @brief SDEファイルに関する処理を行う
 *
 * @param hWnd [in] 親HWND。
 * @param eMenu [in] 処理内容。
 * @param lpszFilePath [in] 選択しているファイルパス。
 */
bool ConnectionFileCtrlI::ConnectionFileFunction(HWND hWnd, EConnectionFileMenu eMenu, LPCTSTR lpszFilePath)
{
	CString strFilePath = lpszFilePath ? m_ConnectionFileFolder.complementFullPath(lpszFilePath) : _T("");

	IRemoteDatabaseWorkspaceFactoryPtr ipFactory(CLSID_SdeWorkspaceFactory);
	IWorkspaceNamePtr ipWorkspaceName;

	bool bRedraw = false;
	switch(eMenu) {
	case kConnectionFileAdd:
		bRedraw = IWorkspaceFactoryPtr(ipFactory)->Create(
			CComBSTR(m_ConnectionFileFolder.getPath()),	0, 0, reinterpret_cast<OLE_HANDLE>(hWnd), &ipWorkspaceName) == S_OK;
		break;
	case kConnectionFileEdit:
		ipFactory->EditConnectionFile(CComBSTR(strFilePath), reinterpret_cast<OLE_HANDLE>(hWnd), &ipWorkspaceName);
		break;
	case kConnectionFileDelete:
		if(::MessageBox(hWnd, _T("選択項目を削除してもよろしいですか？"), _T("削除の確認"), MB_YESNO | MB_ICONWARNING) == IDYES) {
			bRedraw = ipFactory->DeleteConnectionFile(CComBSTR(strFilePath)) == S_OK;
		}
		break;
	case kConnectionFileCopy:
		fileToClipboard(hWnd, strFilePath, DROPEFFECT_COPY);
		break;
	}

	return bRedraw;
}

/**
 * @brief SDEファイルの名称変更を行う
 *
 * @param lpszOldName [in] 旧名称。
 * @param lpszNewName [in] 新名称。
 * @retval true 名称変更を行った。
 * @retval false 旧名称と新名称が同一であった。
 */
bool ConnectionFileCtrlI::RenameConnectionFile(LPCTSTR lpszOldName, LPCTSTR lpszNewName)
{
	if(::_tcscmp(lpszOldName, lpszNewName) != 0) {
		IRemoteDatabaseWorkspaceFactoryPtr ipFactory(CLSID_SdeWorkspaceFactory);

		IWorkspaceNamePtr ipWorkspaceName;
		if(ipFactory->RenameConnectionFile(
			CComBSTR(m_ConnectionFileFolder.complementFullPath(lpszOldName)),
			CComBSTR(lpszNewName),
			&ipWorkspaceName) == S_OK)
			return true;
	}

	return false;
}

// 静的関数
//////////////////////////////////////////////////////////////////////

/**
 * @brief １つのファイルをクリップボードに登録する。
 */
void ConnectionFileCtrlI::fileToClipboard(HWND hWnd, LPCTSTR lpszFilePath, int nFlag)
{
	// 広域ハンドル作成。
	// DROPFILESと、double null terminatedな文字列を保持できるだけの広さを用意。
	int nSize = sizeof(DROPFILES) + sizeof(TCHAR) * (::_tcslen(lpszFilePath) + 2);
	HGLOBAL hDrop = ::GlobalAlloc(GHND, nSize);

	// 広域ハンドル施錠。
	DROPFILES* pDropFiles = static_cast<DROPFILES*>(::GlobalLock(hDrop));
	pDropFiles->pFiles = sizeof(DROPFILES);
#ifdef _UNICODE
	pDropFiles->fWide = TRUE;
#else
	pDropFiles->fWide = FALSE;
#endif // _UNICODE

	// ファイルパス複製。
	LPTSTR lpszDrop = reinterpret_cast<LPTSTR>(pDropFiles + 1);
	::_tcscpy(lpszDrop, lpszFilePath);

	// 広域ハンドル開錠。
	::GlobalUnlock(hDrop);
	
	// Preferred DropEffectを作成。
	HGLOBAL hDropEffect = ::GlobalAlloc(GHND, sizeof(DWORD));
	*static_cast<DWORD*>(::GlobalLock(hDropEffect)) = nFlag;
	::GlobalUnlock(hDropEffect);

	// クリップボードにデータセット。
	UINT CF_DROPEFFECT = ::RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	if(! ::OpenClipboard(hWnd)){
		::GlobalFree(hDropEffect);
		::GlobalFree(hDrop);
		return;
	}

	if(! ::EmptyClipboard()) {
		::CloseClipboard();
		::GlobalFree(hDropEffect);
		::GlobalFree(hDrop);
		return;
	}

	::SetClipboardData(CF_HDROP, hDrop);
	::SetClipboardData(CF_DROPEFFECT, hDropEffect);

	::CloseClipboard();
}

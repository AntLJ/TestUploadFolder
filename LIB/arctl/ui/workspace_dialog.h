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

#ifndef ARCTL_UI_WORKSPACE_DIALOG_H_
#define ARCTL_UI_WORKSPACE_DIALOG_H_

#include <dlgs.h>
#include <sstream>
#include <atl2/io.h>
#include <atl2/exception.h>
#include <arctl/catalog.h>
#include <CodeAnalysis/SourceAnnotations.h>

#ifndef _INC_COMMDLG
	#error workspace_dialog.h requires commdlg.h to be included first
#endif // _INC_COMMDLG

#ifndef __ATLCTRLS_H__
	#error workspace_dialog.h requires atlctrls.h to be included first
#endif // __ATLCTRLS_H__

namespace arctl {

namespace ui {

namespace workspace_dialog_detail {

inline void file_to_clipboard(HWND hWnd, LPCTSTR lpszFilePath, int nFlag)
{
	// 広域ハンドル作成。
	// DROPFILESと、double null terminatedな文字列を保持できるだけの広さを用意。
	const size_t nLen = ::_tcslen(lpszFilePath);
	const size_t nSize = sizeof(DROPFILES) + sizeof(TCHAR) * (nLen + 2);
	const HGLOBAL hDrop = ::GlobalAlloc(GHND, nSize);

	// 広域ハンドル施錠。
	DROPFILES* pDropFiles = static_cast<DROPFILES*>(::GlobalLock(hDrop));
	pDropFiles->pFiles = sizeof(DROPFILES);
#ifdef _UNICODE
	pDropFiles->fWide = TRUE;
#else
	pDropFiles->fWide = FALSE;
#endif // _UNICODE

	// ファイルパス複製。
	const LPTSTR lpszDrop = reinterpret_cast<LPTSTR>(pDropFiles + 1);
	::_tcscpy_s(lpszDrop, nLen + 1, lpszFilePath);

	// 広域ハンドル開錠。
	::GlobalUnlock(hDrop);
	
	// Preferred DropEffectを作成。
	const HGLOBAL hDropEffect = ::GlobalAlloc(GHND, sizeof(DWORD));
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

enum ECatalogFileAction
{
	kCatalogFileNoAction = 0,
	kCatalogFileEdit,
	kCatalogFileDelete,
	kCatalogFileCopy,
	kSDEFileAdd,
	kPGSFileAdd,
};

inline bool create_workspace(
	HWND hWnd,
	[SA_Pre(Null=SA_No)] IWorkspaceFactory* ipFactory,
	[SA_Pre(Null=SA_No), SA_Pre(NullTerminated=SA_Yes)] LPCTSTR lpszPath
)
{
	IWorkspaceNamePtr ipWorkspaceName;
	return SUCCEEDED(ipFactory->Create(CComBSTR(lpszPath), 0, 0, reinterpret_cast<OLE_HANDLE>(hWnd), &ipWorkspaceName));
}

inline ECatalogFileAction popup_sde_file_context_menu(HWND hWnd, [SA_Pre(Null=SA_No), SA_Pre(NullTerminated=SA_Yes)] LPCTSTR lpszPath)
{
	DWORD dwFileAttribute = ::GetFileAttributes(lpszPath);
	if(dwFileAttribute == -1)
		return kCatalogFileNoAction;

	const bool bDirectory = (dwFileAttribute & FILE_ATTRIBUTE_DIRECTORY) != 0;

	const HMENU hMenu = ::CreateMenu();
	const HMENU hPopup = ::CreateMenu();

	IWorkspaceFactoryPtr ipSDEFactory, ipPGSFactory;
	ipSDEFactory.CreateInstance("esriDataSourcesGDB.SdeWorkspaceFactory");
	ipPGSFactory.CreateInstance("PostGISExtension.PostGISWorkspaceFactory");

	IRemoteDatabaseWorkspaceFactoryPtr ipFactory;

	unsigned int nItem = 0;
	if(! bDirectory) {
		VARIANT_BOOL vb;
		if(ipSDEFactory->IsWorkspace(CComBSTR(lpszPath), &vb) == S_OK && vb) {
			ipFactory = ipSDEFactory;
		}
		else if(ipPGSFactory && ipPGSFactory->IsWorkspace(CComBSTR(lpszPath), &vb) == S_OK && vb) {
			ipFactory = ipPGSFactory;
		}

		if(ipFactory == 0)
			return kCatalogFileNoAction;

		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kCatalogFileEdit, _T("編集(&E)"));
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kCatalogFileDelete, _T("削除(&D)"));
		::InsertMenu(hPopup, nItem++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kCatalogFileCopy, _T("コピー(&C)"));
	}
	else {
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kSDEFileAdd, _T("追加(&SDE)"));
		if(ipPGSFactory) {
			::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kPGSFileAdd, _T("追加(&PostGIS)"));
		}
	}
	::InsertMenu(hMenu, nItem++, MF_POPUP | MF_BYPOSITION, reinterpret_cast<UINT_PTR>(hPopup), 0);

	POINT pt;
	::GetCursorPos(&pt);

	const int nCommand = ::TrackPopupMenu(hPopup, TPM_RETURNCMD, pt.x, pt.y, 0, hWnd, 0);

	::DestroyMenu(hPopup);
	::DestroyMenu(hMenu);

	IWorkspaceNamePtr ipWorkspaceName;

	switch(nCommand) {
	case kSDEFileAdd:
	case kPGSFileAdd:
		switch(nCommand) {
		case kSDEFileAdd:
			ipFactory = ipSDEFactory;
			break;
		case kPGSFileAdd:
			ipFactory = ipPGSFactory;
			break;
		}
		if(! (ipFactory && create_workspace(hWnd, IWorkspaceFactoryPtr(ipFactory), lpszPath)))
			return kCatalogFileNoAction;
		break;
	case kCatalogFileEdit:
		atl2::valid(ipFactory->EditConnectionFile(CComBSTR(lpszPath), reinterpret_cast<OLE_HANDLE>(hWnd), &ipWorkspaceName));
		break;
	case kCatalogFileDelete:
		if(::MessageBox(hWnd, _T("選択項目を削除してもよろしいですか？"), _T("削除の確認"), MB_YESNO | MB_ICONWARNING) == IDNO)
			return kCatalogFileNoAction;
		atl2::valid(ipFactory->DeleteConnectionFile(CComBSTR(lpszPath)));
		break;
	case kCatalogFileCopy:
		file_to_clipboard(hWnd, lpszPath, DROPEFFECT_COPY);
		break;
	}

	return static_cast<ECatalogFileAction>(nCommand);
}

class CSDEFileListCtrl : public ATL::CWindowImpl<CSDEFileListCtrl, WTL::CListViewCtrl>
{
public:
	CSDEFileListCtrl() :
	m_strSDEFileFolderPath(get_sde_file_directory_path())
	{
	}

BEGIN_MSG_MAP(CSDEFileListCtrl)
	if(uMsg == OCM_NOTIFY) {
		uMsg -= OCM__BASE;
		NOTIFY_CODE_HANDLER(NM_RCLICK, OnRClick)
		NOTIFY_CODE_HANDLER(LVN_ENDLABELEDIT, OnEndLabelEdit)
		uMsg += OCM__BASE;
	}
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

	LRESULT OnRClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		LPNMLISTVIEW pnmlv = reinterpret_cast<LPNMLISTVIEW>(pnmh);

		try {
			std::basic_ostringstream<_TCHAR> oss;
			oss << m_strSDEFileFolderPath;

			if(pnmlv->iItem != -1) {
				_TCHAR szFileName[_MAX_PATH] = { 0 };
				GetItemText(pnmlv->iItem, 0, szFileName, _MAX_PATH);

				oss << _T("\\") << szFileName;
			}

			switch(popup_sde_file_context_menu(m_hWnd, oss.str().c_str())) {
			case kSDEFileAdd:
			case kPGSFileAdd:
			case kCatalogFileDelete:
				ResetList();
				break;
			}
		}
		catch(const _com_error& e) {
			MessageBox(atl2::stringize(e).c_str(), _T("ファイルの操作エラー"), MB_OK | MB_ICONERROR);
		}

		return 1;
	}

	LRESULT OnEndLabelEdit(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		LV_DISPINFO *pLVDispInfo = reinterpret_cast<LV_DISPINFO*>(pnmh);

		if(pLVDispInfo->item.pszText) {
			_TCHAR szOldName[_MAX_PATH];
			GetItemText(pLVDispInfo->item.iItem, 0, szOldName, _MAX_PATH);

			const LPCTSTR lpszNewName = pLVDispInfo->item.pszText;

			if(::_tcscmp(lpszNewName, szOldName) != 0) {
				const LPCTSTR lpszOldExt = ::_tcsrchr(szOldName, _T('.'));
				const LPCTSTR lpszNewExt = ::_tcsrchr(lpszNewName, _T('.'));

				if(! (lpszOldExt && lpszNewExt && ::_tcsicmp(lpszNewExt, lpszOldExt) == 0)) {
					MessageBox(_T("拡張子の変更は許可しません。"), _T("名前の変更エラー"), MB_OK | MB_ICONERROR);
					return FALSE; 
				}

				try {
					std::basic_ostringstream<_TCHAR> oss;
					oss << m_strSDEFileFolderPath << _T("\\") << szOldName;

					const IRemoteDatabaseWorkspaceFactoryPtr
						ipFactory(
							::_tcsicmp(lpszOldExt + 1, _T("pgs")) == 0
							? "PostGISExtension.PostGISWorkspaceFactory"
							: "esriDataSourcesGDB.SdeWorkspaceFactory"
						);

					const CComBSTR bstrOldPath(oss.str().c_str());
					const CComBSTR bstrNewName(lpszNewName);

					IWorkspaceNamePtr ipWorkspaceName;
					atl2::valid(ipFactory->RenameConnectionFile(bstrOldPath, bstrNewName, &ipWorkspaceName));
					return TRUE;
				}
				catch(const _com_error& e) {
					MessageBox(atl2::stringize(e).c_str(), _T("名前の変更エラー"), MB_OK | MB_ICONERROR);
				}
			}
		}

		return FALSE;
	}

	void ResetList()
	{
		DeleteAllItems();

		std::basic_ostringstream<_TCHAR> oss;
		oss << m_strSDEFileFolderPath << _T("\\*.*");

		WIN32_FIND_DATA fd;
		const HANDLE hFind = ::FindFirstFile(oss.str().c_str(), &fd);
		if(hFind == INVALID_HANDLE_VALUE)
			return;

		int nItem = 0;
		do {
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				LPCTSTR szExt = ::_tcsrchr(fd.cFileName, _T('.')) + 1;
				if(::_tcsicmp(szExt, _T("sde")) == 0 || ::_tcsicmp(szExt, _T("pgs")) == 0) {
					InsertItem(nItem++, fd.cFileName);
				}
			}
		} while(::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}

private:
	std::basic_string<_TCHAR> m_strSDEFileFolderPath;
};

/**
 * @brief 仲介クラス
 *
 * ファイルダイアログの子であり、SDEファイル選択リストビューコントロールの親であり、フックプロシージャのカスタムデータ。<br>
 * CSDEFileListCtrl からの通知メッセージを受け取るため、CStaticを継承している。
 */
class CHookProc : public CWindowImpl<CHookProc, WTL::CStatic>
{
	typedef CWindowImpl<CHookProc, WTL::CStatic> self_t;
public:
BEGIN_MSG_MAP(CHookProc)
	NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemChanged)
	NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDblclk)
	REFLECT_NOTIFICATIONS()
END_MSG_MAP()

	/**
	 * @brief リストで選択しているSDEファイルが変わった。
	 */
	LRESULT OnItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		const LPNMLISTVIEW pnmlv = reinterpret_cast<LPNMLISTVIEW>(pnmh);

		// 選択しているSDEファイルの名前を、ダイアログのエディットコントロールに反映させる。
		TCHAR szBuffer[_MAX_PATH];
		m_lv.GetItemText(pnmlv->iItem, 0, szBuffer, _MAX_PATH);
		CommDlg_OpenSave_SetControlText(GetParent(), edt1, szBuffer);

		return 0;
	}

	/**
	 * @brief リストのSDEファイルがダブルクリックされた。
	 */
	LRESULT OnDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		// SDEファイルが選択されていなかったら空クリックなので無視。
		if(::SendMessage(m_lv.m_hWnd, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0)) == -1)
			return 0;

		// ダイアログのウィンドウハンドルを取得。
		HWND hParentWnd = GetParent();

		// ダイアログのOKボタンを押す。
		::PostMessage(
			hParentWnd,	
			WM_COMMAND,
			MAKEWPARAM(IDOK, BN_CLICKED),
			reinterpret_cast<LPARAM>(::GetDlgItem(hParentWnd, IDOK))
		);

		return 0;
	}

	void Hook(LPOFNOTIFY lpof)
	{
		// 標準のファイルダイアログの「リストビュー」と「ファイルタイプ選択コンボボックス」を取り出す。
		WTL::CListViewCtrl lv2(::GetDlgItem(lpof->hdr.hwndFrom, lst2));
		WTL::CComboBox cb1(::GetDlgItem(lpof->hdr.hwndFrom, cmb1));
		switch(lpof->hdr.code) {
		case CDN_FOLDERCHANGE: // フォルダ変更
			if(! IsWindow()) {
				// lst2と同じ形状の、接続プロパティリストを生成。
				RECT rect;
				lv2.GetWindowRect(&rect);
				CWindow(lpof->hdr.hwndFrom).ScreenToClient(&rect);

				Create(lpof->hdr.hwndFrom, rect);

				if(cb1.GetCurSel() == 0) {
					// lst2を隠し、接続プロパティリストを表示。
					lv2.ShowWindow(SW_HIDE);
					ShowWindow(SW_SHOW);
				}
			}
			else {
				if(cb1.GetCurSel() == 0) {
					// フィルタがsdeであった場合、フィルタをmdbに変更。
					cb1.SetCurSel(1);

					// そのことをダイアログに通知する。
					::SendMessage(
						lpof->hdr.hwndFrom,
						WM_COMMAND,
						MAKEWPARAM(::GetDlgCtrlID(cb1.m_hWnd), CBN_SELENDOK),
						reinterpret_cast<LPARAM>(cb1.m_hWnd)
					);
				}
			}
			break;
		case CDN_TYPECHANGE: // ファイルタイプ変更
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
		case CDN_FILEOK: // OKボタンが押された
			if(cb1.GetCurSel() == 0) {
				// フィルタがsdeであった場合、フォルダ名を規定のものに置き換える。
				::_tcscpy_s(lpof->lpOFN->lpstrFile, MAX_PATH, get_sde_file_directory_path().c_str());
				::_tcscat_s(lpof->lpOFN->lpstrFile, MAX_PATH, _T("\\"));
				::_tcscat_s(lpof->lpOFN->lpstrFile, MAX_PATH, lpof->lpOFN->lpstrFileTitle);
			}
		}
	}

private:
// 処理
	HWND Create(HWND hWndParent, RECT& rcPos)
	{
		if(! self_t::Create(hWndParent, rcPos, NULL, WS_CHILD))
			return 0;

		RECT rect = { 0, 0, rcPos.right - rcPos.left, rcPos.bottom - rcPos.top };

		m_lv.Create(
			m_hWnd, rect, NULL,
			WS_CHILD | WS_VISIBLE | LVS_LIST | LVS_EDITLABELS | LVS_SINGLESEL,
			WS_EX_CLIENTEDGE
		);

		m_lv.ShowWindow(SW_SHOW);
		m_lv.ResetList();

		return m_hWnd;
	}

// 変数
	CSDEFileListCtrl m_lv;
};

inline UINT CALLBACK OFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	// この初期化はマルチスレッドで問題か……
	static const UINT s_wmFileOkString = ::RegisterWindowMessage(FILEOKSTRING);

	if(uiMsg == WM_NOTIFY) {
		LPOFNOTIFY lpof = reinterpret_cast<LPOFNOTIFY>(lParam);
		reinterpret_cast<CHookProc*>(lpof->lpOFN->lCustData)->Hook(lpof);
	}
	else if(uiMsg == s_wmFileOkString) {
		// file must exist.
		LPOPENFILENAME lpOFN = reinterpret_cast<LPOPENFILENAME>(lParam);
		if(::GetFileAttributes(lpOFN->lpstrFile) == -1) {
			::SetWindowLong(hDlg, DWL_MSGRESULT, 1);
			return 1;
		}
	}

	return 0;
}


} // namespace workspace_dialog_detail

/**
 * @brief ワークスペース（コネクションファイル）選択ダイアログ
 *
 * 標準のファイル選択ダイアログに、 ArcCatalog のデータベースコネクションファイルを選択・操作する機能を加えたもの。
 *
 * @code
 * CWorkspaceDialog dlg;
 * if(dlg.DoModal(0) == IDOK) {
 *     IWorkspaceNamePtr ipName(sindy::create_workspace_name(dlg.m_OFN.lpstrFile));
 * }
 * @endcode
 *
 * @par 選択可能なワークスペース:
 * - ArcCatalog のデータベースコネクションファイル
 * - PGDBファイル
 * - Shapeファイルを含むディレクトリ（Shapeファイルを選択する）
 *
 * @note
 * @a m_OFN は外からいじれるようにしているが、以下の用途以外での値の変更がなされた場合、動作は保証しない。
 * - @a m_OFN.lpstrInitialDir に初期ディレクトリを設定する。
 * - @a m_OFN.lpstrTitle にタイトルを設定する。
 * - @a m_OFN.nFilterIndex に初期フィルタインデックスを設定する。
 */
class CWorkspaceDialog
{
public:
	CWorkspaceDialog()
	{
		m_szFile[0] = 0;
		m_szFileTitle[0] = 0;

		::ZeroMemory(&m_OFN, sizeof(m_OFN));
		m_OFN.lStructSize = 88; //< windows2000以降ならこれで問題ないはず…
		m_OFN.lpstrFilter = _T("ArcCatalog データベース コネクション\0*.*\0データベース (*.sde;*.mdb;*.pgs)\0*.sde;*.mdb;*.pgs\0All (*.*)\0*.*\0");

		m_OFN.lpstrFile = m_szFile;
		m_OFN.nMaxFile = MAX_PATH;
		m_OFN.lpstrFileTitle = m_szFileTitle;
		m_OFN.nMaxFileTitle = MAX_PATH;
		m_OFN.lpfnHook = workspace_dialog_detail::OFNHookProc;
 		m_OFN.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLEHOOK;
	}

	BOOL DoModal(HWND hParentWnd)
	{
		// OPENFILENAME構造体に親ウィンドウハンドルを設定。
		m_OFN.hwndOwner = hParentWnd;

		// ファイル名取得開始。
		workspace_dialog_detail::CHookProc aHookProc;
		m_OFN.lCustData = reinterpret_cast<LPARAM>(&aHookProc);
		BOOL bResult = GetOpenFileName(&m_OFN);

		if((::GetFileAttributes(m_OFN.lpstrFile) & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			// Shapeファイルへの接続を望む場合…
			LPTSTR p = ::_tcsrchr(m_OFN.lpstrFile, _T('.'));
			if(::_tcsicmp(p, _T(".shp")) == 0) {
				// パスからファイル名部分を削除する。
				p = ::_tcsrchr(m_OFN.lpstrFile, _T('\\'));
				*(p ? p : m_OFN.lpstrFile) = 0;
			}
		}

		return bResult;
	}

	OPENFILENAME m_OFN;

private:
// 変数
	TCHAR m_szFile[MAX_PATH];
	TCHAR m_szFileTitle[MAX_PATH];
};

} // namespace ui

} // namespace arctl

#endif // ARCTL_UI_WORKSPACE_DIALOG_H_

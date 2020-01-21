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
	// �L��n���h���쐬�B
	// DROPFILES�ƁAdouble null terminated�ȕ������ێ��ł��邾���̍L����p�ӁB
	const size_t nLen = ::_tcslen(lpszFilePath);
	const size_t nSize = sizeof(DROPFILES) + sizeof(TCHAR) * (nLen + 2);
	const HGLOBAL hDrop = ::GlobalAlloc(GHND, nSize);

	// �L��n���h���{���B
	DROPFILES* pDropFiles = static_cast<DROPFILES*>(::GlobalLock(hDrop));
	pDropFiles->pFiles = sizeof(DROPFILES);
#ifdef _UNICODE
	pDropFiles->fWide = TRUE;
#else
	pDropFiles->fWide = FALSE;
#endif // _UNICODE

	// �t�@�C���p�X�����B
	const LPTSTR lpszDrop = reinterpret_cast<LPTSTR>(pDropFiles + 1);
	::_tcscpy_s(lpszDrop, nLen + 1, lpszFilePath);

	// �L��n���h���J���B
	::GlobalUnlock(hDrop);
	
	// Preferred DropEffect���쐬�B
	const HGLOBAL hDropEffect = ::GlobalAlloc(GHND, sizeof(DWORD));
	*static_cast<DWORD*>(::GlobalLock(hDropEffect)) = nFlag;
	::GlobalUnlock(hDropEffect);

	// �N���b�v�{�[�h�Ƀf�[�^�Z�b�g�B
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

		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kCatalogFileEdit, _T("�ҏW(&E)"));
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kCatalogFileDelete, _T("�폜(&D)"));
		::InsertMenu(hPopup, nItem++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kCatalogFileCopy, _T("�R�s�[(&C)"));
	}
	else {
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kSDEFileAdd, _T("�ǉ�(&SDE)"));
		if(ipPGSFactory) {
			::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kPGSFileAdd, _T("�ǉ�(&PostGIS)"));
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
		if(::MessageBox(hWnd, _T("�I�����ڂ��폜���Ă���낵���ł����H"), _T("�폜�̊m�F"), MB_YESNO | MB_ICONWARNING) == IDNO)
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
			MessageBox(atl2::stringize(e).c_str(), _T("�t�@�C���̑���G���["), MB_OK | MB_ICONERROR);
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
					MessageBox(_T("�g���q�̕ύX�͋����܂���B"), _T("���O�̕ύX�G���["), MB_OK | MB_ICONERROR);
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
					MessageBox(atl2::stringize(e).c_str(), _T("���O�̕ύX�G���["), MB_OK | MB_ICONERROR);
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
 * @brief ����N���X
 *
 * �t�@�C���_�C�A���O�̎q�ł���ASDE�t�@�C���I�����X�g�r���[�R���g���[���̐e�ł���A�t�b�N�v���V�[�W���̃J�X�^���f�[�^�B<br>
 * CSDEFileListCtrl ����̒ʒm���b�Z�[�W���󂯎�邽�߁ACStatic���p�����Ă���B
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
	 * @brief ���X�g�őI�����Ă���SDE�t�@�C�����ς�����B
	 */
	LRESULT OnItemChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		const LPNMLISTVIEW pnmlv = reinterpret_cast<LPNMLISTVIEW>(pnmh);

		// �I�����Ă���SDE�t�@�C���̖��O���A�_�C�A���O�̃G�f�B�b�g�R���g���[���ɔ��f������B
		TCHAR szBuffer[_MAX_PATH];
		m_lv.GetItemText(pnmlv->iItem, 0, szBuffer, _MAX_PATH);
		CommDlg_OpenSave_SetControlText(GetParent(), edt1, szBuffer);

		return 0;
	}

	/**
	 * @brief ���X�g��SDE�t�@�C�����_�u���N���b�N���ꂽ�B
	 */
	LRESULT OnDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		// SDE�t�@�C�����I������Ă��Ȃ��������N���b�N�Ȃ̂Ŗ����B
		if(::SendMessage(m_lv.m_hWnd, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_ALL | LVNI_SELECTED, 0)) == -1)
			return 0;

		// �_�C�A���O�̃E�B���h�E�n���h�����擾�B
		HWND hParentWnd = GetParent();

		// �_�C�A���O��OK�{�^���������B
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
		// �W���̃t�@�C���_�C�A���O�́u���X�g�r���[�v�Ɓu�t�@�C���^�C�v�I���R���{�{�b�N�X�v�����o���B
		WTL::CListViewCtrl lv2(::GetDlgItem(lpof->hdr.hwndFrom, lst2));
		WTL::CComboBox cb1(::GetDlgItem(lpof->hdr.hwndFrom, cmb1));
		switch(lpof->hdr.code) {
		case CDN_FOLDERCHANGE: // �t�H���_�ύX
			if(! IsWindow()) {
				// lst2�Ɠ����`��́A�ڑ��v���p�e�B���X�g�𐶐��B
				RECT rect;
				lv2.GetWindowRect(&rect);
				CWindow(lpof->hdr.hwndFrom).ScreenToClient(&rect);

				Create(lpof->hdr.hwndFrom, rect);

				if(cb1.GetCurSel() == 0) {
					// lst2���B���A�ڑ��v���p�e�B���X�g��\���B
					lv2.ShowWindow(SW_HIDE);
					ShowWindow(SW_SHOW);
				}
			}
			else {
				if(cb1.GetCurSel() == 0) {
					// �t�B���^��sde�ł������ꍇ�A�t�B���^��mdb�ɕύX�B
					cb1.SetCurSel(1);

					// ���̂��Ƃ��_�C�A���O�ɒʒm����B
					::SendMessage(
						lpof->hdr.hwndFrom,
						WM_COMMAND,
						MAKEWPARAM(::GetDlgCtrlID(cb1.m_hWnd), CBN_SELENDOK),
						reinterpret_cast<LPARAM>(cb1.m_hWnd)
					);
				}
			}
			break;
		case CDN_TYPECHANGE: // �t�@�C���^�C�v�ύX
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
		case CDN_FILEOK: // OK�{�^���������ꂽ
			if(cb1.GetCurSel() == 0) {
				// �t�B���^��sde�ł������ꍇ�A�t�H���_�����K��̂��̂ɒu��������B
				::_tcscpy_s(lpof->lpOFN->lpstrFile, MAX_PATH, get_sde_file_directory_path().c_str());
				::_tcscat_s(lpof->lpOFN->lpstrFile, MAX_PATH, _T("\\"));
				::_tcscat_s(lpof->lpOFN->lpstrFile, MAX_PATH, lpof->lpOFN->lpstrFileTitle);
			}
		}
	}

private:
// ����
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

// �ϐ�
	CSDEFileListCtrl m_lv;
};

inline UINT CALLBACK OFNHookProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	// ���̏������̓}���`�X���b�h�Ŗ�肩�c�c
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
 * @brief ���[�N�X�y�[�X�i�R�l�N�V�����t�@�C���j�I���_�C�A���O
 *
 * �W���̃t�@�C���I���_�C�A���O�ɁA ArcCatalog �̃f�[�^�x�[�X�R�l�N�V�����t�@�C����I���E���삷��@�\�����������́B
 *
 * @code
 * CWorkspaceDialog dlg;
 * if(dlg.DoModal(0) == IDOK) {
 *     IWorkspaceNamePtr ipName(sindy::create_workspace_name(dlg.m_OFN.lpstrFile));
 * }
 * @endcode
 *
 * @par �I���\�ȃ��[�N�X�y�[�X:
 * - ArcCatalog �̃f�[�^�x�[�X�R�l�N�V�����t�@�C��
 * - PGDB�t�@�C��
 * - Shape�t�@�C�����܂ރf�B���N�g���iShape�t�@�C����I������j
 *
 * @note
 * @a m_OFN �͊O���炢�����悤�ɂ��Ă��邪�A�ȉ��̗p�r�ȊO�ł̒l�̕ύX���Ȃ��ꂽ�ꍇ�A����͕ۏ؂��Ȃ��B
 * - @a m_OFN.lpstrInitialDir �ɏ����f�B���N�g����ݒ肷��B
 * - @a m_OFN.lpstrTitle �Ƀ^�C�g����ݒ肷��B
 * - @a m_OFN.nFilterIndex �ɏ����t�B���^�C���f�b�N�X��ݒ肷��B
 */
class CWorkspaceDialog
{
public:
	CWorkspaceDialog()
	{
		m_szFile[0] = 0;
		m_szFileTitle[0] = 0;

		::ZeroMemory(&m_OFN, sizeof(m_OFN));
		m_OFN.lStructSize = 88; //< windows2000�ȍ~�Ȃ炱��Ŗ��Ȃ��͂��c
		m_OFN.lpstrFilter = _T("ArcCatalog �f�[�^�x�[�X �R�l�N�V����\0*.*\0�f�[�^�x�[�X (*.sde;*.mdb;*.pgs)\0*.sde;*.mdb;*.pgs\0All (*.*)\0*.*\0");

		m_OFN.lpstrFile = m_szFile;
		m_OFN.nMaxFile = MAX_PATH;
		m_OFN.lpstrFileTitle = m_szFileTitle;
		m_OFN.nMaxFileTitle = MAX_PATH;
		m_OFN.lpfnHook = workspace_dialog_detail::OFNHookProc;
 		m_OFN.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLEHOOK;
	}

	BOOL DoModal(HWND hParentWnd)
	{
		// OPENFILENAME�\���̂ɐe�E�B���h�E�n���h����ݒ�B
		m_OFN.hwndOwner = hParentWnd;

		// �t�@�C�����擾�J�n�B
		workspace_dialog_detail::CHookProc aHookProc;
		m_OFN.lCustData = reinterpret_cast<LPARAM>(&aHookProc);
		BOOL bResult = GetOpenFileName(&m_OFN);

		if((::GetFileAttributes(m_OFN.lpstrFile) & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			// Shape�t�@�C���ւ̐ڑ���]�ޏꍇ�c
			LPTSTR p = ::_tcsrchr(m_OFN.lpstrFile, _T('.'));
			if(::_tcsicmp(p, _T(".shp")) == 0) {
				// �p�X����t�@�C�����������폜����B
				p = ::_tcsrchr(m_OFN.lpstrFile, _T('\\'));
				*(p ? p : m_OFN.lpstrFile) = 0;
			}
		}

		return bResult;
	}

	OPENFILENAME m_OFN;

private:
// �ϐ�
	TCHAR m_szFile[MAX_PATH];
	TCHAR m_szFileTitle[MAX_PATH];
};

} // namespace ui

} // namespace arctl

#endif // ARCTL_UI_WORKSPACE_DIALOG_H_

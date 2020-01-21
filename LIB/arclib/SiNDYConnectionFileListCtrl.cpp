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
 * @brief SDE�t�@�C�����상�j���[��\������
 *
 * @param hWnd [in] �eHWND�B
 * @param bSelected [in] SDE�t�@�C�����I������Ă���B
 * @retval 0 ���j���[�͑I������Ȃ������B
 * @retval kConnectionFileAdd �u�ǉ��v���I�����ꂽ�B
 * @retval kConnectionFileEdit �u�ҏW�v���I�����ꂽ�B
 * @retval kConnectionFileDelete �u�폜�v���I�����ꂽ�B
 */
int ConnectionFileCtrlI::PopupConnectionFileMenu(HWND hWnd, bool bSelected)
{
	HMENU hMenu = ::CreateMenu();
	HMENU hPopup = ::CreateMenu();

	unsigned int nItem = 0;
	if(bSelected) {
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileEdit, _T("�ҏW(&E)"));
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileDelete, _T("�폜(&D)"));
		::InsertMenu(hPopup, nItem++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);
	}
	::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileAdd, _T("�ǉ�(&A)"));
	if(bSelected) {
		::InsertMenu(hPopup, nItem++, MF_SEPARATOR | MF_BYPOSITION, 0, 0);
		::InsertMenu(hPopup, nItem++, MF_STRING | MF_BYPOSITION, kConnectionFileCopy, _T("�R�s�[(&C)"));
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
 * @brief SDE�t�@�C���Ɋւ��鏈�����s��
 *
 * @param hWnd [in] �eHWND�B
 * @param eMenu [in] �������e�B
 * @param lpszFilePath [in] �I�����Ă���t�@�C���p�X�B
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
		if(::MessageBox(hWnd, _T("�I�����ڂ��폜���Ă���낵���ł����H"), _T("�폜�̊m�F"), MB_YESNO | MB_ICONWARNING) == IDYES) {
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
 * @brief SDE�t�@�C���̖��̕ύX���s��
 *
 * @param lpszOldName [in] �����́B
 * @param lpszNewName [in] �V���́B
 * @retval true ���̕ύX���s�����B
 * @retval false �����̂ƐV���̂�����ł������B
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

// �ÓI�֐�
//////////////////////////////////////////////////////////////////////

/**
 * @brief �P�̃t�@�C�����N���b�v�{�[�h�ɓo�^����B
 */
void ConnectionFileCtrlI::fileToClipboard(HWND hWnd, LPCTSTR lpszFilePath, int nFlag)
{
	// �L��n���h���쐬�B
	// DROPFILES�ƁAdouble null terminated�ȕ������ێ��ł��邾���̍L����p�ӁB
	int nSize = sizeof(DROPFILES) + sizeof(TCHAR) * (::_tcslen(lpszFilePath) + 2);
	HGLOBAL hDrop = ::GlobalAlloc(GHND, nSize);

	// �L��n���h���{���B
	DROPFILES* pDropFiles = static_cast<DROPFILES*>(::GlobalLock(hDrop));
	pDropFiles->pFiles = sizeof(DROPFILES);
#ifdef _UNICODE
	pDropFiles->fWide = TRUE;
#else
	pDropFiles->fWide = FALSE;
#endif // _UNICODE

	// �t�@�C���p�X�����B
	LPTSTR lpszDrop = reinterpret_cast<LPTSTR>(pDropFiles + 1);
	::_tcscpy(lpszDrop, lpszFilePath);

	// �L��n���h���J���B
	::GlobalUnlock(hDrop);
	
	// Preferred DropEffect���쐬�B
	HGLOBAL hDropEffect = ::GlobalAlloc(GHND, sizeof(DWORD));
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

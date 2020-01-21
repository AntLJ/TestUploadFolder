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
#include <dlgs.h>
#include "SiNDYConnectionDlgI.h"

#pragma warning ( disable : 4100 )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

static UINT s_wmFileOkString = ::RegisterWindowMessage(FILEOKSTRING);


//////////////////////////////////////////////////////////////////////
// ConnectionDlg
//////////////////////////////////////////////////////////////////////

// 構築/消滅
//////////////////////////////////////////////////////////////////////

ConnectionDlgI::ConnectionDlgI()
{
	m_szFile[0] = 0;
	m_szFileTitle[0] = 0;

	::ZeroMemory(&m_OFN, sizeof(OPENFILENAME));
	m_OFN.lStructSize = 88; //< windows2000以降ならこれで問題ないはず…
	m_OFN.lpstrFilter = _T("接続プロパティ (*.sde)\0*.*\0データベース (*.mdb)\0*.sde;*.mdb\0All (*.*)\0*.*\0");

	m_OFN.lpstrFile = m_szFile;
	m_OFN.nMaxFile = MAX_PATH;
	m_OFN.lpstrFileTitle = m_szFileTitle;
	m_OFN.nMaxFileTitle = MAX_PATH;
	m_OFN.lpfnHook = ConnectionDlgI::OFNHookProc;
 	m_OFN.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLEHOOK;
}

ConnectionDlgI::~ConnectionDlgI()
{

}

// 
//////////////////////////////////////////////////////////////////////

UINT CALLBACK ConnectionDlgI::OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if(uiMsg == WM_NOTIFY) {
		LPOFNOTIFY lpof = reinterpret_cast<LPOFNOTIFY>(lParam);
		(*reinterpret_cast<HookProcI*>(lpof->lpOFN->lCustData))(lpof);
	}
	else if(uiMsg == s_wmFileOkString) {
		// file must exist.
		LPOPENFILENAME lpOFN = reinterpret_cast<LPOPENFILENAME>(lParam);
		if(::GetFileAttributes(lpOFN->lpstrFile) == -1) {
			::SetWindowLong(hdlg, DWL_MSGRESULT, 1);
			return 1;
		}
	}

	return 0;
}

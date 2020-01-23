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

// ATLTestAppDlg.h : CATLTestAppDlg の宣言

#pragma once

#include "resource.h"       // メイン シンボル

#include <atlhost.h>
#include "../../include/WinMgr.h"

// CATLTestAppDlg

class CATLTestAppDlg : 
	public CSizeableDlg<CDialogImpl<CATLTestAppDlg> >
{
typedef CDialogImpl<CATLTestAppDlg> atlClass;
	typedef CSizeableDlg<atlClass> baseClass;
public:
	CATLTestAppDlg();

	virtual ~CATLTestAppDlg()
	{
	}

	enum { IDD = IDD_ATLTESTAPP_DIALOG };

BEGIN_MSG_MAP_EX(CATLTestAppDlg)
	MSG_WM_INITDIALOG(OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CSizeableDlg) // CSizeableDlgのメッセージマップを結合します
END_MSG_MAP()

// ハンドラ プロパティ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	BOOL OnInitDialog(HWND hwnd, LPARAM lParam)
	{
		SetMsgHandled(FALSE); // CSizeableDlgのメッセージハンドラにまわすため

		m_wndSizerBar.Create(WS_VISIBLE|WS_CHILD, *this, m_winMgr, IDC_SIZERBAR);
		CenterWindow();

		CComboBox cmb(GetDlgItem(IDC_COMBO1));
		cmb.AddString(_T("aaa"));
		cmb.AddString(_T("bbb"));
		cmb.AddString(_T("ccc"));
		cmb.AddString(_T("ddd"));
		cmb.AddString(_T("eee"));
		cmb = GetDlgItem(IDC_COMBO2);
		cmb.AddString(_T("aaa"));
		cmb.AddString(_T("bbb"));
		cmb.AddString(_T("ccc"));
		cmb.AddString(_T("ddd"));
		cmb.AddString(_T("eee"));

		return TRUE;
	}
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		baseClass::EndDialog(wID);

		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		baseClass::EndDialog(wID);
		return 0;
	}
private:
	CSizerBar m_wndSizerBar;
};



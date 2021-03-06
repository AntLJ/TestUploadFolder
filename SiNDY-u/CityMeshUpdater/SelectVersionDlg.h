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

// This file was generated by WTL Dialog wizard 
// SelectVersionDlg.h : Declaration of the CSelectVersionDlg

#pragma once

#include "resource.h"       // main symbols
#include <atlddx.h>
// CSelectVersionDlg

class CSelectVersionDlg : 
	public CDialogImpl<CSelectVersionDlg>,
	public CWinDataExchange<CSelectVersionDlg>	
{
public:
	CSelectVersionDlg();
	CSelectVersionDlg( LPCTSTR lpcszFCName, LPCTSTR lpcszServerName );
	~CSelectVersionDlg();
	enum { IDD = IDD_SELECTVERSION_DLG };

    BEGIN_MSG_MAP(CSelectVersionDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
        COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
    END_MSG_MAP()

	BEGIN_DDX_MAP(CSelectVersionDlg)
		DDX_TEXT(IDC_EDIT_SRCVERSION, m_strSrcversion)
		DDX_TEXT(IDC_EDIT_DSTVERSION, m_strDstversion)
		DDX_TEXT(IDC_STATIC_MSG, m_strMsg)
	END_DDX_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
public:
	CString m_strSrcversion,
			m_strDstversion,
			m_strMsg;

};



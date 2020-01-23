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
// SelectVersionDlg.cpp : Implementation of CSelectVersionDlg

#include "stdafx.h"
#include "SelectVersionDlg.h"

// CSelectVersionDlg
CSelectVersionDlg::CSelectVersionDlg()
{
}

CSelectVersionDlg::~CSelectVersionDlg()
{
}

CSelectVersionDlg::CSelectVersionDlg( LPCTSTR lpcszFCName, LPCTSTR lpcszServerName )
{
	m_strMsg.Format( _T("%s@%sはバージョン対応レイヤです。\n更新元と更新先のバージョンを指定してください。"), lpcszFCName, lpcszServerName );
}

LRESULT CSelectVersionDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DoDataExchange();
	return 1;  // Let the system set the focus
}

LRESULT CSelectVersionDlg::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);
	EndDialog(wID);
	return 0;
}

LRESULT CSelectVersionDlg::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

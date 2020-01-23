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
// PlogressDlg.cpp : Implementation of CPlogressDlg

#include "stdafx.h"
#include "PlogressDlg.h"

// CPlogressDlg
CPlogressDlg::CPlogressDlg()
{
	//AtlInitCommonControls(ICC_COOL_CLASSES | ICC_WIN95_CLASSES);
}

CPlogressDlg::~CPlogressDlg()
{
}

LRESULT CPlogressDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_cProgres = GetDlgItem( IDC_PROGRESS1 );
	m_cStaticEdit = GetDlgItem( IDC_EDIT_CURRENT );
	return 1;  // Let the system set the focus
}
void CPlogressDlg::PumpMessages()
{
	// ダイアログを使用する前に Create() を呼んでください。
	if(!m_hWnd)
		return;

	MSG msg;
	// ダイアログ メッセージの処理
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);  
		}
	}
}

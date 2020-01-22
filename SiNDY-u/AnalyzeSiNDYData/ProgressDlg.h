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

#pragma once
#include "Resource.h"

const unsigned int WM_PROGRESSCOMMENT = RegisterWindowMessage(_T("WM_PROGRESSCOMMENT"));
const unsigned int WM_PROGRESSPOS = RegisterWindowMessage(_T("WM_PROGRESSPOS"));	
const unsigned int WM_PROGRESSRANGE = RegisterWindowMessage(_T("WM_PROGRESSRANGE"));
const unsigned int WM_PROGRESSCLOSE = RegisterWindowMessage(_T("WM_PROGRESSCLOSE"));

class CProgressDlg : public CAxDialogImpl<CProgressDlg>
{
public:
	enum { IDD = IDD_PROGRESSDLG };

	CProgressDlg(void);
	~CProgressDlg(void);

BEGIN_MSG_MAP(CProgressDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_PROGRESSCLOSE, OnProgressClose )
    MESSAGE_HANDLER(WM_PROGRESSCOMMENT, OnProgressComment)
    MESSAGE_HANDLER(WM_PROGRESSPOS, OnProgressPos)
    MESSAGE_HANDLER(WM_PROGRESSRANGE, OnProgressRange)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnProgressClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnProgressComment( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnProgressPos( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT OnProgressRange( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

private:
	CProgressBarCtrl m_cProgressDlg1, m_cProgressDlg2;
	CEdit m_cProgressEdit1, m_cProgressEdit2;
};

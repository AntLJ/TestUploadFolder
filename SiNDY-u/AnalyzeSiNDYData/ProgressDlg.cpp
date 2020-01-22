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

#include "StdAfx.h"
#include "ProgressDlg.h"

CProgressDlg::CProgressDlg(void)
{

}

CProgressDlg::~CProgressDlg(void)
{
}
LRESULT CProgressDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_cProgressDlg1 = GetDlgItem( IDC_PROGRESS1 );
	m_cProgressDlg2 = GetDlgItem( IDC_PROGRESS2 );
	m_cProgressEdit1 = GetDlgItem( IDC_STATIC_PROGRESS1 );
	m_cProgressEdit2 = GetDlgItem( IDC_STATIC_PROGRESS2 );
	return 1;
};

LRESULT CProgressDlg::OnProgressClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT CProgressDlg::OnProgressComment( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( wParam == 1 )
		m_cProgressEdit1.SetWindowText( (LPCTSTR)lParam );
	else if( wParam == 2 )
		m_cProgressEdit2.SetWindowText( (LPCTSTR)lParam );

	return 0;
};
LRESULT CProgressDlg::OnProgressPos( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( wParam == 1 )
		m_cProgressDlg1.SetPos( (int)lParam );
	else if( wParam == 2 )
		m_cProgressDlg2.SetPos( (int)lParam );

	return 0;
};
LRESULT CProgressDlg::OnProgressRange( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( wParam == 1 )
		m_cProgressDlg1.SetRange( 0, (int)lParam );
	else if( wParam == 2 )
		m_cProgressDlg2.SetRange( 0, (int)lParam );

	return 0;
};

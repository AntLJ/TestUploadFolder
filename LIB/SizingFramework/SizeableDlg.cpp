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

////////////////////////////////////////////////////////////////
// MSDN Magazine -- July 2001
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0. Runs on Win 98 and probably Win 2000 too.
// Set tabsize = 3 in your editor.
//
#include "StdAfx.h"
#include "include/WinMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSizeableDlg, CDialog)
BEGIN_MESSAGE_MAP(CSizeableDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_REGISTERED_MESSAGE(WM_WINMGR, OnWinMgr)
END_MESSAGE_MAP()

CSizeableDlg::CSizeableDlg(UINT nIDRes, CWnd* pParent, WINRECT* pWinMap)
	: CDialog(nIDRes, pParent),
	m_winMgr(pWinMap)
{
}

CSizeableDlg::~CSizeableDlg()
{
}

//////////////////
// Initialzie dialog: make TOFIT size for all TOFIT controls whatever their
// current size is. Also, do a recalc/setpos because the calculated layout
// will never match exactly the dialog template--need to force calculated
// layout to avoid controls jumping the first time the user sizes. THIS IS
// IMPORTANT!
//
BOOL CSizeableDlg::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();
	m_winMgr.InitToFitSizeFromCurrent(this); // make tofit = current size
	m_winMgr.CalcLayout(this);					  // recalc
	m_winMgr.SetWindowPositions(this);		  // set positions
	return bRet;
}

/////////////////
// Dialog sized: let WinMgr handle it
//
void CSizeableDlg::OnSize(UINT nType, int cx, int cy)
{
	m_winMgr.CalcLayout(cx,cy,this);
	m_winMgr.SetWindowPositions(this);
}

//////////////////
// Handle WM_GETMINMAXINFO: Let WinMgr calculate it.
//
void CSizeableDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	m_winMgr.GetMinMaxInfo(this, lpMMI);
}

LRESULT CSizeableDlg::OnWinMgr(WPARAM wp, LPARAM lp)
{
	ASSERT(lp);
	NMWINMGR& nmw = *(NMWINMGR*)lp;
	if (nmw.code==NMWINMGR::SIZEBAR_MOVED) {
		// User moved a sizer bar: call WinMgr to do it!
		m_winMgr.MoveRect(static_cast<int>(wp), nmw.sizebar.ptMoved, this);
		m_winMgr.SetWindowPositions(this);
		return TRUE;
	}
	return FALSE; // not handled
}

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
#include "CIconButton.h"

/////////////////////////////////////////////////////////////////////////////
//
// CIconButton
//
/////////////////////////////////////////////////////////////////////////////
BOOL CIconButton::SubclassWindow(HWND hWnd)
{
	if ( m_hIcon )
		DestroyIcon( m_hIcon );
	m_hIcon = NULL;

	BOOL bRet = CBitmapButtonImpl<CIconButton>::SubclassWindow( hWnd );
	if ( bRet )
		OpenThemeData(_T("Button"));

	return bRet;
}

LRESULT CIconButton::OnDestroy( UINT, WPARAM, LPARAM, BOOL& bHandled )
{
	if ( m_hIcon )
		DestroyIcon( m_hIcon );
	m_hIcon = NULL;

	bHandled = FALSE;
	return 1;
}

LRESULT CIconButton::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CIconButton* pT = static_cast<CIconButton*>(this);
	if ( wParam )
		pT->DoPaint((HDC)wParam);
	else {
		CPaintDC dc(m_hWnd);
		pT->DoPaint(dc.m_hDC);
	}

	return 0;
}

void CIconButton::DoPaint(CDCHandle dc)
{
	RECT rect={0};
	UINT uFrameState = PBS_NORMAL;
	bool bIsDef = false;
	bool bHover = IsHoverMode();

	GetClientRect(&rect);

	if ( m_fPressed == 1 ) {
		uFrameState = PBS_PRESSED;
		bIsDef = true;
	} else if((!bHover && m_fFocus == 1) || (bHover && m_fMouseOver == 1)) {
		uFrameState = PBS_HOT;
		bIsDef = true;
	} else if(!IsWindowEnabled()) {
		uFrameState = PBS_DISABLED;
		bIsDef = true;
	}

	if ( m_hTheme ) {
		DrawThemeBackground(dc, BP_PUSHBUTTON, uFrameState, &rect, NULL);
		GetThemeBackgroundContentRect(dc, BP_PUSHBUTTON, uFrameState, &rect, &rect);
	} else {
		if ( m_fPressed == 1 )
			dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT | BF_SOFT);
		else
			dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT | BF_SOFT);

		if ( !IsHoverMode() && m_fFocus == 1) {
			::InflateRect(&rect, -2 * ::GetSystemMetrics(SM_CXEDGE), -2 * ::GetSystemMetrics(SM_CYEDGE));
			dc.DrawFocusRect(&rect);
		}
	}

	DrawIcon( dc, rect, uFrameState );
}

void CIconButton::DrawIcon(CDCHandle dc, const CRect& rect, DWORD uFrameState) const
{
	int cx = 0;
	int cy = 0;
	DWORD dwStyle = GetWindowLong( GWL_STYLE );

	if ( m_hIcon ) {
		ICONINFO ii={0};

		GetIconInfo( m_hIcon, &ii );

		BITMAPINFO bmi={0};

		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		if ( ii.hbmColor ) {
			GetDIBits( dc, ii.hbmColor, 0, 0, NULL, &bmi, DIB_RGB_COLORS );
			cx = bmi.bmiHeader.biWidth;
			cy = bmi.bmiHeader.biHeight;
		} else {
			GetDIBits( dc, ii.hbmMask, 0, 0, NULL, &bmi, DIB_RGB_COLORS );
			cx = bmi.bmiHeader.biWidth;
			cy = bmi.bmiHeader.biHeight/2;
		}

		if ( ii.hbmMask )
			DeleteObject( ii.hbmMask );
		if ( ii.hbmColor )
			DeleteObject( ii.hbmColor );
	}

	int x = image_left(cx, rect, dwStyle);
	int y = image_top(cy, rect, dwStyle);

	if ( m_hIcon )
		DrawState( dc, NULL, NULL, (LPARAM) m_hIcon, 0, x, y, cx, cy, (dwStyle & WS_DISABLED) != 0 ? (DST_ICON | DSS_DISABLED) : (DST_ICON | DSS_NORMAL));
}

void CIconButton::SetIcon(const _U_ICONorID& icon)
{
	if ( m_hIcon )
		DestroyIcon( m_hIcon );
	m_hIcon = (HICON)icon.m_hIcon;
}

int CIconButton::image_left(int cx, const CRect& Rect, DWORD style) const 
{
	int cx2 = cx;
	int x = Rect.left;

	if ( cx2 > Rect.Width() )
		cx2 = Rect.Width();
	else if ( (style & BS_CENTER) == BS_LEFT )
		x = Rect.left;
	else if ( (style & BS_CENTER) == BS_RIGHT )
		x = Rect.right - cx2;
	else
		x = Rect.left + (Rect.Width() - cx2) / 2;
	return (x);
}

int CIconButton::image_top(int cy, const CRect& Rect, DWORD style) const 
{
	int cy2 = cy;
	int y = Rect.top;

	if ( cy2 > Rect.Height() )
		cy2 = Rect.Height();
	if ( (style & BS_VCENTER) == BS_TOP )
		y = Rect.top;
	else if ( (style & BS_VCENTER) == BS_BOTTOM )
		y = Rect.bottom - cy2;
	else
		y = Rect.top + (Rect.Height() - cy2) / 2;
	return (y);
}

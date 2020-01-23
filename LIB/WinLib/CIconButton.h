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

#ifndef __ICONBUTTON_H__
#define __ICONBUTTON_H__

#include "Theme.h"

class _U_ICONorID
{
public:
	_U_ICONorID(HICON handle) : m_hIcon(handle)
	{ }
	_U_ICONorID(UINT nID) : m_hIcon( (HICON)::LoadImage( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), IMAGE_ICON, 16, 16, LR_DEFAULTSIZE ) )
	{ }
	HICON m_hIcon;
};

/*!
	\class	CIconButton
	\brief	アイコンボタンクラス
			CBitmapButtonImplがWindowsテーマに対応していないので、
			対応するようにしたクラス
*/
class CIconButton : 
	public CBitmapButtonImpl<CIconButton>,
	public CThemeImpl< CIconButton >
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_IconButton"), GetWndClassName())

	CIconButton(DWORD dwExtendedStyle = BMPBTN_AUTOSIZE, HIMAGELIST hImageList = NULL) : CBitmapButtonImpl<CIconButton>(dwExtendedStyle, hImageList), m_hIcon(NULL) {}
	~CIconButton()
	{
		if ( m_hIcon )
			DestroyIcon( m_hIcon );
		m_hIcon = NULL;
	}

	BEGIN_MSG_MAP(CIconButton)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		CHAIN_MSG_MAP(CBitmapButtonImpl<CIconButton>)
		CHAIN_MSG_MAP(CThemeImpl<CIconButton>)
	END_MSG_MAP()

	BOOL SubclassWindow(HWND hWnd);
	LRESULT OnDestroy( UINT, WPARAM, LPARAM, BOOL& bHandled );
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void DoPaint(CDCHandle dc);
	void DrawIcon(CDCHandle dc, const CRect& rect, DWORD uFrameState) const;
	void SetIcon(const _U_ICONorID& icon);

protected:
	int image_left(int cx, const CRect& Rect, DWORD style) const;
	int image_top(int cy, const CRect& Rect, DWORD style) const;

public:
	HICON	m_hIcon;
};

#endif // __ICON_BUTTON_H__

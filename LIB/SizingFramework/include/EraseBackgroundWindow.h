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

/**
 * @class CEraseBackground
 * 
 * このクラスは、WM_ERASEBKGNDを自前で処理するクラスです。
 * グループボックスではボックス内は透過になっていますが、
 * このためちらつきを低減させようとダイアログ側でWS_CLIPCHILDREN
 * 使用とするとこの仕様が仇となり、グループボックス内の隙間
 * が描画されなくなってしまいます。\n
 * なので、使用するケースとしては、ちらつきを低減させるとき
 * のみに限定されます。\n
 * ただし、このクラスだけではちらつきを低減させることは不可能
 * です。正確には、以下の手順を行う必要があります。
 *
 * ちらつき低減方法（WinMgr使用前提）：\n
 * -# 親ダイアログにWS_CLIPCHILDRENをつける\n
 *    まずこれで、親ダイアログによる全面塗りつぶしが低減される
 *    ため、大幅なちらつき低減が実現されます。ただし、グループ
 *    ボックスを使用している場合には様々な問題が出てきます。
 * -# WM_SIZEで以下のコードを適用します（CSizeableDlgで処理されないように注意）。
 * @code
 *    if( IsWindow() )
 *    {
 *        SetRedraw(FALSE);
 *        m_winMgr.CalcLayout(size.cx,size.cy,this);
 *        m_winMgr.SetWindowPositions(this);
 *        SetRedraw(TRUE);
 *        RedrawWindow( NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW |RDW_ALLCHILDREN);
 *    }
 * @endcode
 *    これにより、再描画の回数がかなり低減されます。
 *    グループボックスを使用しない場合はこれでOKです。
 * -# CEraseBackgroundクラスを使用して背景塗りつぶしをする\n
 *    これにより、グループボックスを使用した場合のリサイズでも問題
 *    なく描画されるようになります。ただし、グループボックス内にあ
 *    るコントロールのタブオーダーはグループボックスよりも若くなけ
 *    れば意味がありませんので注意してください（Ctrl-Dで確認してください）。
 * -# オーナードローピクチャボックスをグループボックスと同じサイズになるようにWinMgrで配置する\n
 *    これはグループボックスに無効領域ができた場合（ウィンドウが重なった等）
 *    の場合に中にあるコントロールがCEraseBackgroundの処理により消されてしまう
 *    事を防ぎます。このとき、ピクチャボックスのタブオーダーはグループボックス
 *    より後になるようにしないと効果を発揮しません。
 *    なお、オーナードローピクチャボックスと言ってもタイプを「オーナー描画」に
 *    するだけで後は何もしなくてOKです。
 *
 * 以上で、かなりのちらつき低減が見込めるはずです。
 */
class CEraseBackground : public CWindowImplBaseT<>
{
public:
	CEraseBackground() : CWindowImplBaseT<>()
	{
	}
	CEraseBackground( HWND hWnd ) : CWindowImplBaseT<>()
	{

		SubclassWindow( hWnd );

		DWORD dwForceStyle = WS_CLIPSIBLINGS;
		DWORD dwStyle = GetStyle();

		if((dwStyle & dwForceStyle) != dwForceStyle)
			ModifyStyle(0, dwForceStyle);

		DWORD dwForceStyleEx = WS_EX_TRANSPARENT;
		DWORD dwStyleEx = GetExStyle();

		if( ( dwStyleEx & dwForceStyle ) != dwForceStyle )
			ModifyStyleEx(0, dwForceStyle);
	}
	~CEraseBackground()
	{
		if( m_hWnd )
			UnsubclassWindow();
	}

BEGIN_MSG_MAP_EX(CEraseBackground)
	MSG_WM_ERASEBKGND(OnEraseBackground)
END_MSG_MAP()
	LRESULT OnEraseBackground( HDC hDC )
	{
		// @see http://support.microsoft.com/kb/79982/ja
		HBRUSH  hBrush, hOldBrush;
		HPEN    hPen, hOldPen;
		RECT    rect;

		// Obtain a handle to the parent window's background brush.
		//hBrush = (HBRUSH)GetClassLong(m_hParent, GCL_HBRBACKGROUND);
		hBrush = GetSysColorBrush(COLOR_BTNFACE);
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

		// Create a background-colored pen to draw the rectangle
		// borders, where gWindowColor is some globally defined
		// COLORREF variable used to paint the window's background

		hPen = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
		hOldPen = (HPEN)SelectObject(hDC, hPen);

		// Erase the group box's background.
		GetClientRect(&rect);
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);

		// Restore the original objects before releasing the DC.
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);

		// Delete the created object.
		DeleteObject(hPen);

		::ReleaseDC(m_hWnd, hDC);

		// Instruct Windows to paint the group box text and frame.
		::InvalidateRect(m_hWnd, NULL, FALSE);

		return TRUE; // Background has been erased.
	}
};

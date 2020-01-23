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
 * ���̃N���X�́AWM_ERASEBKGND�����O�ŏ�������N���X�ł��B
 * �O���[�v�{�b�N�X�ł̓{�b�N�X���͓��߂ɂȂ��Ă��܂����A
 * ���̂��߂������ጸ�����悤�ƃ_�C�A���O����WS_CLIPCHILDREN
 * �g�p�Ƃ���Ƃ��̎d�l���w�ƂȂ�A�O���[�v�{�b�N�X���̌���
 * ���`�悳��Ȃ��Ȃ��Ă��܂��܂��B\n
 * �Ȃ̂ŁA�g�p����P�[�X�Ƃ��ẮA�������ጸ������Ƃ�
 * �݂̂Ɍ��肳��܂��B\n
 * �������A���̃N���X�����ł͂������ጸ�����邱�Ƃ͕s�\
 * �ł��B���m�ɂ́A�ȉ��̎菇���s���K�v������܂��B
 *
 * ������ጸ���@�iWinMgr�g�p�O��j�F\n
 * -# �e�_�C�A���O��WS_CLIPCHILDREN������\n
 *    �܂�����ŁA�e�_�C�A���O�ɂ��S�ʓh��Ԃ����ጸ�����
 *    ���߁A�啝�Ȃ�����ጸ����������܂��B�������A�O���[�v
 *    �{�b�N�X���g�p���Ă���ꍇ�ɂ͗l�X�Ȗ�肪�o�Ă��܂��B
 * -# WM_SIZE�ňȉ��̃R�[�h��K�p���܂��iCSizeableDlg�ŏ�������Ȃ��悤�ɒ��Ӂj�B
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
 *    ����ɂ��A�ĕ`��̉񐔂����Ȃ�ጸ����܂��B
 *    �O���[�v�{�b�N�X���g�p���Ȃ��ꍇ�͂����OK�ł��B
 * -# CEraseBackground�N���X���g�p���Ĕw�i�h��Ԃ�������\n
 *    ����ɂ��A�O���[�v�{�b�N�X���g�p�����ꍇ�̃��T�C�Y�ł����
 *    �Ȃ��`�悳���悤�ɂȂ�܂��B�������A�O���[�v�{�b�N�X���ɂ�
 *    ��R���g���[���̃^�u�I�[�_�[�̓O���[�v�{�b�N�X�����Ⴍ�Ȃ�
 *    ��ΈӖ�������܂���̂Œ��ӂ��Ă��������iCtrl-D�Ŋm�F���Ă��������j�B
 * -# �I�[�i�[�h���[�s�N�`���{�b�N�X���O���[�v�{�b�N�X�Ɠ����T�C�Y�ɂȂ�悤��WinMgr�Ŕz�u����\n
 *    ����̓O���[�v�{�b�N�X�ɖ����̈悪�ł����ꍇ�i�E�B���h�E���d�Ȃ������j
 *    �̏ꍇ�ɒ��ɂ���R���g���[����CEraseBackground�̏����ɂ�������Ă��܂�
 *    ����h���܂��B���̂Ƃ��A�s�N�`���{�b�N�X�̃^�u�I�[�_�[�̓O���[�v�{�b�N�X
 *    ����ɂȂ�悤�ɂ��Ȃ��ƌ��ʂ𔭊����܂���B
 *    �Ȃ��A�I�[�i�[�h���[�s�N�`���{�b�N�X�ƌ����Ă��^�C�v���u�I�[�i�[�`��v��
 *    ���邾���Ō�͉������Ȃ���OK�ł��B
 *
 * �ȏ�ŁA���Ȃ�̂�����ጸ�������߂�͂��ł��B
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

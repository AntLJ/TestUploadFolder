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

class CWindowHook
{
protected:
	CWindowHook();

public:
	virtual ~CWindowHook() {}
	virtual LRESULT OnHook( int id, ULONG HookType, int nCode, WPARAM wParam, LPARAM lParam, bool bAccept );

protected:
	bool SetWindowsHookEx( int id, HWND hWnd, ULONG HookType, DWORD threadID = -1 );
	void UnhookWindowsHookEx( int id );
	LRESULT CallNextHookEx( int nCode, WPARAM wParam, LPARAM lParam );

protected:
	HHOOK m_hHook;	//!< フックハンドル
};

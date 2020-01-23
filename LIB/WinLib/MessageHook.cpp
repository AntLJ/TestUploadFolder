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
#include "MessageHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessageHook::CMessageHook() : CWindowHook(), m_pObserver( NULL )
{
}

CMessageHook::~CMessageHook()
{
}

void CMessageHook::Initialize( int id, HWND hWnd, ULONG hookType, Observer* pObserver, DWORD tid/* = 0*/ )
{
	if ( pObserver )
	{
		m_pObserver = pObserver;
		SetWindowsHookEx( id, hWnd, hookType, tid );
	}
}

void CMessageHook::Shutdown(int id)
{
	UnhookWindowsHookEx( id );
	m_pObserver = NULL;
}

LRESULT CMessageHook::OnHook( int id, ULONG HookType, int nCode, WPARAM wParam, LPARAM lParam, bool bAccept )
{
	if ( nCode >= HC_ACTION && m_pObserver )
		m_pObserver->OnHook( id, HookType, nCode, wParam, lParam, bAccept );

	return CWindowHook::OnHook( id, HookType, nCode, wParam, lParam, bAccept );
}

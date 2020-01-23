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
#include "WindowHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFINE_HOOK( _id, _HookType )			\
		if ( id == _id && HookType == _HookType ) 		\
			return HookProc< _id, _HookType >; 	\

namespace
{
	class CHookManager
	{
	private:
		CHookManager(){}

		LRESULT CALLBACK OnHook( int id, ULONG HookType, int nCode, WPARAM wParam, LPARAM lParam )
		{
			std::map< int, CWindowHook* >::iterator it = m_mapHooks.find( id );
			if ( it != m_mapHooks.end() )
			{
				if ( it->second )
				{
					bool bAccept = true;	// WH_CBT用フック下メッセージを拒否するときはfalseにする
					return it->second->OnHook( id, HookType, nCode, wParam, lParam, bAccept );
				}
			}

			return -1;
		}

	public:
		void Register( int id, CWindowHook* pHook )
		{
			if ( ! pHook || m_mapHooks.find( id ) != m_mapHooks.end() )
				return;

			m_mapHooks[id] = pHook;
		}

		void UnRegister( int id )
		{
			std::map< int, CWindowHook* >::iterator it = m_mapHooks.find( id );
			if ( it != m_mapHooks.end() )
				m_mapHooks.erase( it );
		}

	public:
		static CHookManager & GetInstance()
		{
			static CHookManager theInstance;
			return theInstance;
		}

		static HOOKPROC GetHookProc( int id, ULONG HookType )
		{
			DEFINE_HOOK( 0, WH_KEYBOARD )
			DEFINE_HOOK( 1, WH_KEYBOARD )
			DEFINE_HOOK( 2, WH_KEYBOARD )
			DEFINE_HOOK( 3, WH_KEYBOARD )
			DEFINE_HOOK( 4, WH_KEYBOARD )
			DEFINE_HOOK( 5, WH_KEYBOARD )

			DEFINE_HOOK( 0, WH_GETMESSAGE )
			DEFINE_HOOK( 1, WH_GETMESSAGE )
			DEFINE_HOOK( 2, WH_GETMESSAGE )
			DEFINE_HOOK( 3, WH_GETMESSAGE )
			DEFINE_HOOK( 4, WH_GETMESSAGE )
			DEFINE_HOOK( 5, WH_GETMESSAGE )

			DEFINE_HOOK( 0, WH_CALLWNDPROC )
			DEFINE_HOOK( 1, WH_CALLWNDPROC )
			DEFINE_HOOK( 2, WH_CALLWNDPROC )
			DEFINE_HOOK( 3, WH_CALLWNDPROC )
			DEFINE_HOOK( 4, WH_CALLWNDPROC )
			DEFINE_HOOK( 5, WH_CALLWNDPROC )

			DEFINE_HOOK( 0, WH_CBT )
			DEFINE_HOOK( 1, WH_CBT )
			DEFINE_HOOK( 2, WH_CBT )
			DEFINE_HOOK( 3, WH_CBT )
			DEFINE_HOOK( 4, WH_CBT )
			DEFINE_HOOK( 5, WH_CBT )

			DEFINE_HOOK( 0, WH_SYSMSGFILTER )
			DEFINE_HOOK( 1, WH_SYSMSGFILTER )
			DEFINE_HOOK( 2, WH_SYSMSGFILTER )
			DEFINE_HOOK( 3, WH_SYSMSGFILTER )
			DEFINE_HOOK( 4, WH_SYSMSGFILTER )
			DEFINE_HOOK( 5, WH_SYSMSGFILTER )

			DEFINE_HOOK( 0, WH_MOUSE )
			DEFINE_HOOK( 1, WH_MOUSE )
			DEFINE_HOOK( 2, WH_MOUSE )
			DEFINE_HOOK( 3, WH_MOUSE )
			DEFINE_HOOK( 4, WH_MOUSE )
			DEFINE_HOOK( 5, WH_MOUSE )

			DEFINE_HOOK( 0, WH_HARDWARE )
			DEFINE_HOOK( 1, WH_HARDWARE )
			DEFINE_HOOK( 2, WH_HARDWARE )
			DEFINE_HOOK( 3, WH_HARDWARE )
			DEFINE_HOOK( 4, WH_HARDWARE )
			DEFINE_HOOK( 5, WH_HARDWARE )

			DEFINE_HOOK( 0, WH_DEBUG )
			DEFINE_HOOK( 1, WH_DEBUG )
			DEFINE_HOOK( 2, WH_DEBUG )
			DEFINE_HOOK( 3, WH_DEBUG )
			DEFINE_HOOK( 4, WH_DEBUG )
			DEFINE_HOOK( 5, WH_DEBUG )

			DEFINE_HOOK( 0, WH_SHELL )
			DEFINE_HOOK( 1, WH_SHELL )
			DEFINE_HOOK( 2, WH_SHELL )
			DEFINE_HOOK( 3, WH_SHELL )
			DEFINE_HOOK( 4, WH_SHELL )
			DEFINE_HOOK( 5, WH_SHELL )

			DEFINE_HOOK( 0, WH_CALLWNDPROCRET )
			DEFINE_HOOK( 1, WH_CALLWNDPROCRET )
			DEFINE_HOOK( 2, WH_CALLWNDPROCRET )
			DEFINE_HOOK( 3, WH_CALLWNDPROCRET )
			DEFINE_HOOK( 4, WH_CALLWNDPROCRET )
			DEFINE_HOOK( 5, WH_CALLWNDPROCRET )

			DEFINE_HOOK( 0, WH_KEYBOARD_LL )
			DEFINE_HOOK( 1, WH_KEYBOARD_LL )
			DEFINE_HOOK( 2, WH_KEYBOARD_LL )
			DEFINE_HOOK( 3, WH_KEYBOARD_LL )
			DEFINE_HOOK( 4, WH_KEYBOARD_LL )
			DEFINE_HOOK( 5, WH_KEYBOARD_LL )

			DEFINE_HOOK( 0, WH_MOUSE_LL )
			DEFINE_HOOK( 1, WH_MOUSE_LL )
			DEFINE_HOOK( 2, WH_MOUSE_LL )
			DEFINE_HOOK( 3, WH_MOUSE_LL )
			DEFINE_HOOK( 4, WH_MOUSE_LL )
			DEFINE_HOOK( 5, WH_MOUSE_LL )
			ATLASSERT( FALSE ); // unknown hook type
			return NULL;
		}

	private:
		template< int id, ULONG HookType >
		static LRESULT CALLBACK HookProc( int nCode, WPARAM wParam, LPARAM lParam )
		{
			return GetInstance().OnHook( id, HookType, nCode, wParam, lParam );
		}

	private:
		std::map< int, CWindowHook* > m_mapHooks;
	};
}

CWindowHook::CWindowHook() : m_hHook(NULL)
{
}

LRESULT CWindowHook::OnHook( int id, ULONG HookType, int nCode, WPARAM wParam, LPARAM lParam, bool bAccept )
{
	if ( ! bAccept )
		return 1;
	return CallNextHookEx( nCode, wParam, lParam );
}

bool CWindowHook::SetWindowsHookEx( int id, HWND hWnd, ULONG HookType, DWORD threadID/* = -1*/ )
{
	DWORD tid = threadID;

	if ( tid == -1 )
	{
		// フォアグラウンドウィンドウを作成したスレッドのIDを取得
		tid = ::GetWindowThreadProcessId( hWnd, NULL );
		if ( tid == -1 )
			tid = ::GetCurrentThreadId();
	}

	// フック開始
	m_hHook = ::SetWindowsHookEx( HookType, CHookManager::GetHookProc( id, HookType ), tid != 0 ? NULL : _AtlBaseModule.GetModuleInstance(), tid );

	if ( m_hHook )
	{
		//CString str;
		//str.Format( _T("SetHook %d:%x:%x\n"), id, m_hHook, hWnd );
		//OutputDebugString( str );

		CHookManager::GetInstance().Register( id, this );
		return true;
	}
	else
		ATLASSERT( m_hHook );

	return false;
}

void CWindowHook::UnhookWindowsHookEx(int id)
{
	if ( m_hHook )
	{
		//CString str;
		//str.Format( _T("UnHook %d:%x\n"), id, m_hHook );
		//OutputDebugString( str );

		// フック終了
		::UnhookWindowsHookEx( m_hHook );
		m_hHook = NULL;

		CHookManager::GetInstance().UnRegister( id );
	}
}

LRESULT CWindowHook::CallNextHookEx( int nCode, WPARAM wParam, LPARAM lParam )
{
	return ::CallNextHookEx( m_hHook, nCode, wParam, lParam );
}

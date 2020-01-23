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

#ifndef __THEME_H__
#define __THEME_H__

#pragma once

#include "Psapi.h"
#include "theme.h"

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Version.lib")

#include <vssym32.h>
#include <uxtheme.h>

///////////////////////////////////////////////////////////////////////////////
// CTheme - wrapper for theme handle
class CTheme
{
public:
	typedef HANDLE (WINAPI *LPFNOPENTHEMEDATA)( HWND, LPCWSTR );
	typedef HRESULT (WINAPI *LPFNCLOSETHEMEDATA)( HTHEME );
	typedef HRESULT (WINAPI *LPFNDRAWTHEMEBACKGROUND)( HTHEME, HDC, int, int, const RECT*, const RECT* );
	typedef HRESULT (WINAPI *LPFNDRAWTHEMETEXT)( HTHEME, HDC, int, int, LPCWSTR, int, DWORD, DWORD, const RECT* );
	typedef HRESULT (WINAPI *LPFNGETTHEMEBACKGROUNDCONTENTRECT)( HTHEME, HDC, int, int, const RECT*, RECT* );
	typedef HRESULT (WINAPI *LPFNGETTHEMEBACKGROUNDEXTENT)( HTHEME, HDC, int, int, const RECT*, RECT* );
	typedef HRESULT (WINAPI *LPFNDRAWTHEMEEDGE)( HTHEME, HDC, int, int, const RECT*, UINT, UINT, RECT* );
	typedef HRESULT (WINAPI *LPFNDRAWTHEMEEDGE)( HTHEME, HDC, int, int, const RECT*, UINT, UINT, RECT* );
	typedef HRESULT (WINAPI *LPFNGETTHEMEINT)( HTHEME, int, int, int, int* );
	typedef BOOL (WINAPI *LPFNISTHEMELDIALOGTEXTUREENABLED)( HWND );
	typedef HRESULT (WINAPI *LPFNENABLTHEMELDIALOGTEXTURE)( HWND, DWORD );
	typedef void (WINAPI *LPFNSETTHEMEAPPPROPERTIES)( DWORD );

	LPFNOPENTHEMEDATA lpfnOpenThemeData;
	LPFNCLOSETHEMEDATA lpfnCloseThemeData;
	LPFNDRAWTHEMEBACKGROUND lpfnDrawThemeBackground;
	LPFNDRAWTHEMETEXT lpfnDrawThemeText;
	LPFNGETTHEMEBACKGROUNDCONTENTRECT lpfnGetThemeBackgroundContentRect;
	LPFNDRAWTHEMEEDGE lpfnDrawThemeEdge;
	LPFNGETTHEMEINT lpfnGetThemeInt;
	LPFNISTHEMELDIALOGTEXTUREENABLED lpfnIsThemeDialogTextureEnabled;
	LPFNENABLTHEMELDIALOGTEXTURE lpfnEnableThemeDialogTexture;
	LPFNSETTHEMEAPPPROPERTIES lpfnSetThemeAppProperties;

// Data members
	HTHEME m_hTheme;
	int m_nIsThemingSupported;
	HMODULE m_hLibrary;

// Constructor
	CTheme() : m_hTheme(NULL), m_hLibrary(NULL), m_nIsThemingSupported(-1)
	{
		ClearFunc();
		IsThemingSupported();
	}

	virtual ~CTheme()
	{
		CloseThemeData();
		if ( m_hLibrary != NULL )
			::FreeLibrary( m_hLibrary );
		m_hLibrary = NULL;
		ClearFunc();
	}

	void Load()
	{
		if ( m_hLibrary == NULL ) {
			m_hLibrary = ::LoadLibrary( _T("uxtheme.dll") );
			InitFunc();
		}
	}

	void InitFunc()
	{
		if ( m_hLibrary != NULL ) {
			lpfnOpenThemeData = (LPFNOPENTHEMEDATA)::GetProcAddress( m_hLibrary, "OpenThemeData" );
			lpfnCloseThemeData = (LPFNCLOSETHEMEDATA)::GetProcAddress( m_hLibrary, "CloseThemeData" );
			lpfnDrawThemeBackground = (LPFNDRAWTHEMEBACKGROUND)::GetProcAddress( m_hLibrary, "DrawThemeBackground" );
			lpfnDrawThemeText = (LPFNDRAWTHEMETEXT)::GetProcAddress( m_hLibrary, "DrawThemeText" );
			lpfnGetThemeBackgroundContentRect = (LPFNGETTHEMEBACKGROUNDCONTENTRECT)::GetProcAddress( m_hLibrary, "GetThemeBackgroundContentRect" );
			lpfnDrawThemeEdge = (LPFNDRAWTHEMEEDGE)::GetProcAddress( m_hLibrary, "DrawThemeEdge" );
			lpfnGetThemeInt = (LPFNGETTHEMEINT)::GetProcAddress( m_hLibrary, "GetThemeInt" );
			lpfnIsThemeDialogTextureEnabled = (LPFNISTHEMELDIALOGTEXTUREENABLED)::GetProcAddress( m_hLibrary, "IsThemeDialogTextureEnabled" );
			lpfnEnableThemeDialogTexture = (LPFNENABLTHEMELDIALOGTEXTURE)::GetProcAddress( m_hLibrary, "EnableThemeDialogTexture" );
			lpfnSetThemeAppProperties = (LPFNSETTHEMEAPPPROPERTIES)::GetProcAddress( m_hLibrary, "SetThemeAppProperties" );
		}
	}

	void ClearFunc()
	{
		lpfnOpenThemeData = NULL;
		lpfnCloseThemeData = NULL;
		lpfnDrawThemeBackground = NULL;
		lpfnDrawThemeText = NULL;
		lpfnGetThemeBackgroundContentRect = NULL;
		lpfnDrawThemeEdge = NULL;
		lpfnGetThemeInt = NULL;
		lpfnIsThemeDialogTextureEnabled = NULL;
		lpfnEnableThemeDialogTexture = NULL;
		lpfnSetThemeAppProperties = NULL;
	}

	bool IsCommCtl6()
	{
		BOOL bV6Loaded = false;

		// カレントプロセスのID取得
		DWORD dwProcessID = GetCurrentProcessId();

		HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, dwProcessID );

		if ( hProcess != NULL ) {
			// 確実にするには必要な領域サイズを先に算出しておくのがいいが、
			// ここでは面倒なので100個固定とする(2048個以上あったらたぶんバグります)
			HMODULE* ahModuleHandles = NULL;
			DWORD dwReturnSize = 0, dwSize = 0;

			::EnumProcessModules( hProcess, NULL, 0, &dwSize );
			ahModuleHandles = new HMODULE[dwSize+sizeof(HMODULE)];

			if ( ::EnumProcessModules( hProcess, ahModuleHandles, dwSize, &dwReturnSize ) != FALSE ) {
				// 取得できた個数の計算
				DWORD dwCount = dwReturnSize / sizeof( HMODULE );
				TCHAR	 szBaseName[2048+sizeof(TCHAR)];

				// 取得できたライブラリの数分、ライブラリ情報を取得する
				DWORD i = 0;
				for ( i = 0; i < dwCount; ++i ) {
					::GetModuleBaseName( hProcess, ahModuleHandles[i], szBaseName, 2048 );

					if ( _tcsicmp( szBaseName, _T("comctl32.dll") ) == 0 ) {
						// comctl32.dllの発見
						// ahModuleHandles[i]が必要とする情報
						// comctl32.dllのライブラリハンドルからファイルへのパスを得る
						TCHAR szFileName[2048+sizeof(TCHAR)];

						GetModuleFileName( ahModuleHandles[i], szFileName, 2048 );

						DWORD dwInfoSize = ::GetFileVersionInfoSize( szFileName, NULL );

						if ( dwInfoSize != 0 ) {
							BYTE* pbyData = new BYTE[dwInfoSize+sizeof(BYTE)];

							if ( pbyData != NULL ) {
								// バージョン情報をファイルから取り出す
								if ( GetFileVersionInfo( szFileName, 0, dwInfoSize, (LPVOID)pbyData ) != FALSE ) {
									VS_FIXEDFILEINFO* pFileInfo;
									UINT uiSize;

									// バージョン情報よりお目当ての情報を得る
									if ( ::VerQueryValue( (const LPVOID)pbyData, _T("\\"), (LPVOID*)&pFileInfo, &uiSize ) != FALSE ) {
										if ( 6 <= HIWORD( pFileInfo->dwFileVersionMS ) ) {
											// comctl32.dllのバージョンは6以降である
											bV6Loaded = true;
										}
									}
								}

								delete [] pbyData;
							}
						}
					}
				}
			}

			delete [] ahModuleHandles;
			CloseHandle( hProcess );
		}

		return bV6Loaded;
	}

// Operators and helpers
	bool IsThemeNull() const
	{
		return (m_hTheme == NULL);
	}

	CTheme& operator =(HTHEME hTheme)
	{
		m_hTheme = hTheme;
		return *this;
	}

	operator HTHEME() const
	{
		return m_hTheme;
	}

	void Attach(HTHEME hTheme)
	{
		m_hTheme = hTheme;
	}

	HTHEME Detach()
	{
		HTHEME hTheme = m_hTheme;
		m_hTheme = NULL;
		return hTheme;
	}

// Theme support helper
	bool IsThemingSupported()
	{
		if ( m_nIsThemingSupported == -1 ) {
			if ( !IsCommCtl6() ) {
				m_nIsThemingSupported = 0;
			} else {
				Load();
				m_nIsThemingSupported = (m_hLibrary != NULL) ? 1 : 0;
			}
		}

		ATLASSERT(m_nIsThemingSupported != -1);
		return (m_nIsThemingSupported == 1);
	}

// Operations and theme properties
	HTHEME OpenThemeData(HWND hWnd, LPCWSTR pszClassList)
	{
		if ( !IsThemingSupported() )
			return NULL;

		ATLASSERT(m_hTheme == NULL);

		if ( lpfnOpenThemeData != NULL )
			m_hTheme = lpfnOpenThemeData( hWnd, pszClassList );

		return m_hTheme;
	}

	HRESULT CloseThemeData()
	{
		HRESULT hRet = S_FALSE;
		if ( m_hTheme != NULL ) {
			if ( lpfnCloseThemeData != NULL )
				hRet = lpfnCloseThemeData( m_hTheme );

			if ( SUCCEEDED(hRet) )
				m_hTheme = NULL;
		}

		return hRet;
	}

	void SetThemeAppProperties( DWORD dwFlags )
	{
		if ( !IsThemingSupported() )
			return;

		if ( lpfnSetThemeAppProperties != NULL )
			lpfnSetThemeAppProperties( dwFlags );
	}

	HRESULT DrawThemeBackground(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, LPCRECT pClipRect = NULL)
	{
		if ( !IsThemingSupported() )
			return S_FALSE;

		ATLASSERT(m_hTheme != NULL);
		HRESULT hRet = S_FALSE;

		if ( m_hTheme != NULL ) {
			if ( lpfnDrawThemeBackground != NULL )
				hRet = lpfnDrawThemeBackground( m_hTheme, hDC, nPartID, nStateID, pRect, pClipRect );
		}

		return hRet;
	}

	HRESULT DrawThemeText(HDC hDC, int nPartID, int nStateID, LPCWSTR pszText, int nCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
	{
		if ( !IsThemingSupported() )
			return S_FALSE;

		ATLASSERT(m_hTheme != NULL);
		HRESULT hRet = S_FALSE;

		if ( m_hTheme != NULL ) {
			if ( lpfnDrawThemeText != NULL )
				hRet = lpfnDrawThemeText( m_hTheme, hDC, nPartID, nStateID, pszText, nCharCount, dwTextFlags, dwTextFlags2, pRect );
		}

		return hRet;
	}

	HRESULT GetThemeBackgroundContentRect(HDC hDC, int nPartID, int nStateID,  LPCRECT pBoundingRect, LPRECT pContentRect)
	{
		if ( !IsThemingSupported() )
			return S_FALSE;

		ATLASSERT(m_hTheme != NULL);
		HRESULT hRet = S_FALSE;

		if ( m_hTheme != NULL ) {
			if ( lpfnGetThemeBackgroundContentRect != NULL )
				hRet = lpfnGetThemeBackgroundContentRect( m_hTheme, hDC, nPartID, nStateID,  pBoundingRect, pContentRect );
		}

		return hRet;
	}

	HRESULT DrawThemeEdge(HDC hDC, int nPartID, int nStateID, LPCRECT pDestRect, UINT uEdge, UINT uFlags, LPRECT pContentRect = NULL)
	{
		if ( !IsThemingSupported() )
			return S_FALSE;

		ATLASSERT(m_hTheme != NULL);
		HRESULT hRet = S_FALSE;

		if ( m_hTheme != NULL ) {
			if ( lpfnDrawThemeEdge != NULL )
				hRet = lpfnDrawThemeEdge( m_hTheme, hDC, nPartID, nStateID, pDestRect, uEdge, uFlags, pContentRect );
		}

		return hRet;
	}

	HRESULT GetThemeInt(int nPartID, int nStateID, int nPropID, int* pnVal)
	{
		if ( !IsThemingSupported() )
			return S_FALSE;

		ATLASSERT(m_hTheme != NULL);
		HRESULT hRet = S_FALSE;

		if ( m_hTheme != NULL ) {
			if ( lpfnGetThemeInt != NULL )
				hRet = lpfnGetThemeInt( m_hTheme, nPartID, nStateID, nPropID, pnVal );
		}

		return hRet;
	}

	HRESULT EnableThemeDialogTexture(HWND hWnd, DWORD dwFlags)
	{
		if(!IsThemingSupported())
			return S_FALSE;

		ATLASSERT(::IsWindow(hWnd));

		HRESULT hRet = S_FALSE;

		if ( lpfnEnableThemeDialogTexture != NULL )
			hRet = lpfnEnableThemeDialogTexture( hWnd, dwFlags );

		return hRet;
	}

	BOOL IsThemeDialogTextureEnabled(HWND hWnd)
	{
		if ( !IsThemingSupported() )
			return FALSE;

		ATLASSERT(::IsWindow(hWnd));

		if ( lpfnIsThemeDialogTextureEnabled != NULL )
			return lpfnIsThemeDialogTextureEnabled( hWnd );

		return FALSE;
	}
};

///////////////////////////////////////////////////////////////////////////////
// CThemeImpl - theme support implementation

// Theme extended styles
#define THEME_EX_3DCLIENTEDGE		0x00000001
#define THEME_EX_THEMECLIENTEDGE	0x00000002

template <class T, class TBase = CTheme>
class CThemeImpl : public TBase
{
public:
// Data members
	LPWSTR m_lpstrThemeClassList;
	DWORD m_dwExtendedStyle;   // theme specific extended styles

// Constructor & destructor
	CThemeImpl() : m_lpstrThemeClassList(NULL), m_dwExtendedStyle(0)
	{ }

	~CThemeImpl()
	{
		delete [] m_lpstrThemeClassList;
	}

// Attributes
	bool SetThemeClassList(LPCWSTR lpstrThemeClassList)
	{
		if(m_lpstrThemeClassList != NULL)
		{
			delete [] m_lpstrThemeClassList;
			m_lpstrThemeClassList = NULL;
		}

		if(lpstrThemeClassList == NULL)
			return true;

		int cchLen = lstrlenW(lpstrThemeClassList) + 1;
		ATLTRY(m_lpstrThemeClassList = new WCHAR[cchLen+sizeof(WCHAR)]);

		if(m_lpstrThemeClassList == NULL)
			return false;

		bool bRet = (lstrcpyW(m_lpstrThemeClassList, lpstrThemeClassList) != NULL);
		if(!bRet)
		{
			delete [] m_lpstrThemeClassList;
			m_lpstrThemeClassList = NULL;
		}
		return bRet;
	}

	bool GetThemeClassList(LPWSTR lpstrThemeClassList, int cchListBuffer) const
	{
		int cchLen = lstrlenW(m_lpstrThemeClassList) + 1;
		if(cchListBuffer < cchLen)
			return false;

		return (lstrcpyW(lpstrThemeClassList, m_lpstrThemeClassList) != NULL);
	}

	LPCWSTR GetThemeClassList() const
	{
		return m_lpstrThemeClassList;
	}

	DWORD SetThemeExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask = 0)
	{
		DWORD dwPrevStyle = m_dwExtendedStyle;
		if(dwMask == 0)
			m_dwExtendedStyle = dwExtendedStyle;
		else
			m_dwExtendedStyle = (m_dwExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);
		return dwPrevStyle;
	}

	DWORD GetThemeExtendedStyle() const
	{
		return m_dwExtendedStyle;
	}

// Operations
	HTHEME OpenThemeData()
	{
		T* pT = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		ATLASSERT(m_lpstrThemeClassList != NULL);
		if(m_lpstrThemeClassList == NULL)
			return NULL;
		CloseThemeData();
		return TBase::OpenThemeData(pT->m_hWnd, m_lpstrThemeClassList);
	}

	HTHEME OpenThemeData(LPCWSTR pszClassList)
	{
		if(!SetThemeClassList(pszClassList))
			return NULL;
		return OpenThemeData();
	}

	HRESULT EnableThemeDialogTexture(DWORD dwFlags)
	{
		T* pT = static_cast<T*>(this);
		return CTheme::EnableThemeDialogTexture( pT->m_hWnd, dwFlags );
	}

	BOOL IsThemeDialogTextureEnabled()
	{
		T* pT = static_cast<T*>(this);
		return CTheme::IsThemeDialogTextureEnabled( pT->m_hWnd );
	}

// Message map and handlers
	// Note: If you handle any of these messages in your derived class,
	// it is better to put CHAIN_MSG_MAP at the start of your message map.
	BEGIN_MSG_MAP(CThemeImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_THEMECHANGED, OnThemeChanged)
		MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(m_lpstrThemeClassList != NULL)
			OpenThemeData();
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CloseThemeData();
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnThemeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CloseThemeData();
		if(m_lpstrThemeClassList != NULL)
			OpenThemeData();
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		T* pT = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pT->m_hWnd));
		LRESULT lRet = 0;
		bHandled = FALSE;
		if ( IsThemingSupported() && ((pT->GetExStyle() & WS_EX_CLIENTEDGE) != 0) ) {
			if ( (m_dwExtendedStyle & THEME_EX_3DCLIENTEDGE) != 0 ) {
				lRet = ::DefWindowProc(pT->m_hWnd, uMsg, wParam, lParam);
				bHandled = TRUE;
			} else if ( (m_hTheme != NULL) && ((m_dwExtendedStyle & THEME_EX_THEMECLIENTEDGE) != 0) ) {
				HRGN hRgn = (wParam != 1) ? (HRGN)wParam : NULL;
				if(pT->DrawThemeClientEdge(hRgn))
					bHandled = TRUE;
			}
		}

		return lRet;
	}

// Drawing helper
	bool DrawThemeClientEdge(HRGN hRgnUpdate)
	{
		T* pT = static_cast<T*>(this);
		return AtlDrawThemeClientEdge(m_hTheme, pT->m_hWnd, hRgnUpdate, NULL, 0, 0);
	}

	// Helper for drawing theme client edge
	inline bool AtlDrawThemeClientEdge(HTHEME hTheme, HWND hWnd, HRGN hRgnUpdate = NULL, HBRUSH hBrush = NULL, int nPartID = 0, int nStateID = 0)
	{
		ATLASSERT(hTheme != NULL);
		ATLASSERT(::IsWindow(hWnd));

		CWindowDC dc(hWnd);
		if(dc.IsNull())
			return false;

		// Get border size
		int cxBorder = GetSystemMetrics(SM_CXBORDER);
		int cyBorder = GetSystemMetrics(SM_CYBORDER);
		if(SUCCEEDED(CTheme::GetThemeInt(nPartID, nStateID, TMT_SIZINGBORDERWIDTH, &cxBorder)))
			cyBorder = cxBorder;

		RECT rect;
		::GetWindowRect(hWnd, &rect);

		// Remove the client edge from the update region
		int cxEdge = GetSystemMetrics(SM_CXEDGE);
		int cyEdge = GetSystemMetrics(SM_CYEDGE);
		::InflateRect(&rect, -cxEdge, -cyEdge);
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rect);
		if(rgn.IsNull())
			return false;

		if(hRgnUpdate != NULL)
			rgn.CombineRgn(hRgnUpdate, rgn, RGN_AND);

		::OffsetRect(&rect, -rect.left, -rect.top);

		::OffsetRect(&rect, cxEdge, cyEdge);
		dc.ExcludeClipRect(&rect);
		::InflateRect(&rect, cxEdge, cyEdge);

		CTheme::DrawThemeBackground(dc, nPartID, nStateID, &rect, NULL);

		// Use background brush too, since theme border might not cover everything
		if ( cxBorder < cxEdge && cyBorder < cyEdge ) {
			if ( hBrush == NULL)
// need conditional code because types don't match in winuser.h
#ifdef _WIN64
				hBrush = (HBRUSH)::GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
#else
				hBrush = (HBRUSH)UlongToPtr(::GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND));
#endif

			::InflateRect(&rect, cxBorder - cxEdge, cyBorder - cyEdge);
			dc.FillRect(&rect, hBrush);
		}

		::DefWindowProc(hWnd, WM_NCPAINT, (WPARAM)rgn.m_hRgn, 0L);

		return true;
	}
};

#endif // __CTHEME_H__

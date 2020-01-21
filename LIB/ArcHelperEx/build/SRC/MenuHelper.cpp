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

/**
* @file MenuHelper.cpp
*
* @brief CMenuHelper クラスのインプリメンテーション
*
* @author 千葉 智博
*
* @date   2002/01/23
*/
#include "stdafx.h"
#include "AheGlobals.h"
#include "InifileHelper.h"
#include "MenuHelper.h"
#include "TargetFeatures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
namespace AHE
{

LPCTSTR CMenu::m_lpcszMenuParent = TEXT( "MenuParent#4ArcMap");

CMenu::~CMenu( void)
{
	Delete();
	if( 0 != m_atmMenuParent) {
		if( ::UnregisterClass( m_lpcszMenuParent, _AtlBaseModule.GetModuleInstance())) 
			m_atmMenuParent = 0;
	}
	::DeleteObject( hFont);
//	listData.clear();
}

/// メニュー作成。最上位階層を作成する際、親となるWindowの作成を同時に行う
BOOL CMenu::CreateMenu( BOOL blParent)
{
	/// 最上位階層？
	if( blParent) {
		/// 親となるWindowのクラスを登録して、Windowを作成する

		if( 0 == m_atmMenuParent) {
			m_atmMenuParent = RegisterClass();
			if( 0 == m_atmMenuParent )
			{
				LPTSTR lpszMessage = NULL;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpszMessage, 0, NULL );
				ATLTRACE(_T("CMenu::CreateMenu : ERROR : %s\n"), lpszMessage );
				LocalFree( (HLOCAL)lpszMessage );
				_ASSERTE( 0 != m_atmMenuParent);
			}
			if( 0 == m_atmMenuParent)return FALSE;
		}

		m_hParent = ::CreateWindow( m_lpcszMenuParent, TEXT( ""), WS_POPUP, 0, 0, 0, 0, NULL, NULL, _AtlBaseModule.GetModuleInstance(), this);
		ATLASSERT( NULL != m_hParent);
		if( NULL == m_hParent)
			return FALSE;
	}

	/// メニュー作成
	m_hMenu = ::CreatePopupMenu();
	ATLASSERT( NULL != m_hMenu);
	if( NULL == m_hMenu)
		return FALSE;

	return TRUE;
}

/// メニュー項目の挿入。文字列を未指定は強制的にセパレータ
BOOL CMenu::InsertMenu( UINT unPosition, UINT unFlag, UINT unIDNewItem, LPCTSTR lpcszNewItem, HBITMAP hBitmap)
{
	ATLASSERT( NULL != m_hMenu);

	if( NULL == lpcszNewItem) {
		::InsertMenu( m_hMenu, unPosition, unFlag | MF_SEPARATOR, -1, NULL);
		return ::DrawMenuBar( m_hParent );
	} else {
		/// WM_MEASUREITEMで返すべきサイズを計算しておく
		RECT stRect;
		ZeroMemory( &stRect, sizeof( RECT));

		int nCheckAreaWidth = 0;
		int nMenuTextWidth = 0;
		int nMenuTextHeight = 0;

		HDC hDC = ::GetDC( m_hParent);
		if( hDC == NULL )
			return FALSE;

		BITMAP		stBitmap;
		HBITMAP	hChkMark = ::LoadBitmap( NULL, reinterpret_cast<LPCTSTR>(OBM_CHECK) );
		if ( ::GetObject( hChkMark, sizeof( BITMAP), &stBitmap) == 0 )
			return FALSE;
		DeleteObject(hChkMark);

		nCheckAreaWidth = stBitmap.bmWidth;
		HFONT hFontOld = ( HFONT)::SelectObject( hDC, hFont);

		stRect.top = 0;
		stRect.left = 0;
		stRect.right = 1024;
		stRect.bottom = 400;
		if ( ::DrawText( hDC, lpcszNewItem, -1, &stRect, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK) == 0 )
			return FALSE;
		nMenuTextWidth = stRect.right - stRect.left;
		nMenuTextHeight = stRect.bottom - stRect.top;

		::SelectObject( hDC, hFontOld);
		::ReleaseDC( m_hParent, hDC);

		ITEMDATA* lpstItemData = reinterpret_cast<ITEMDATA*>( malloc( sizeof( ITEMDATA ) + ( sizeof( TCHAR ) * ( lstrlen( lpcszNewItem ) + 1 ) ) ) );
		if ( lpstItemData == NULL )
			return FALSE;
//		lpstItemData->mallocAddr = reinterpret_cast<void*>(lpstItemData);
		lpstItemData->cbSize = sizeof(ITEMDATA) + ( sizeof( TCHAR) * ( lstrlen( lpcszNewItem) + 1));
		lpstItemData->hBitmap = hBitmap;
		lpstItemData->unIDItem = unIDNewItem;
		lpstItemData->stSize.cx = nMenuTextWidth + CXFrame + CXBorder + _ICON_WIDTH + nCheckAreaWidth;
		lpstItemData->stSize.cy = max( _ICON_HEIGHT, nMenuTextHeight) + ( CYBorder * 2);
		lstrcpy( lpstItemData->lpItemString, lpcszNewItem);

//		listData.push_back(reinterpret_cast<LPVOID>(lpstItemData));

		return ::InsertMenu( m_hMenu, unPosition, unFlag | MF_OWNERDRAW, unIDNewItem, reinterpret_cast< LPCTSTR>( lpstItemData));

//		return ::DrawMenuBar( m_hParent );
	}
}

/// ポップアップメニューを表示
int CMenu::TrackPopupMenu( UINT unFlag, int x, int y, LPCRECT lpstRect)
{
	ATLASSERT( NULL != m_hMenu);
	ATLASSERT( NULL != m_hParent);
	int nResult = ::TrackPopupMenu( m_hMenu, unFlag | TPM_RETURNCMD | TPM_NONOTIFY, x, y, 0, m_hParent, lpstRect);

	return nResult;
}

/// メニューのチェック
UINT CMenu::CheckMenuItem( UINT nIDCheckItem, UINT nCheck)
{
	return ::CheckMenuItem( m_hMenu, nIDCheckItem, nCheck);
}

/// メニューの使用許可
UINT CMenu::EnableMenuItem( UINT nIDEnableItem, UINT nEnable)
{
	return ::EnableMenuItem( m_hMenu, nIDEnableItem, nEnable);
}

/// メニューの状態
UINT CMenu::GetMenuState( UINT nID, UINT nFlags ) const
{
	return ::GetMenuState( m_hMenu, nID, nFlags);
}

/// メニュー削除関数。階層にしたがって再起的に削除を実行する。
BOOL CMenu::DestroyMenu( HMENU hMenu)
{
	if( NULL != hMenu) {
		MENUITEMINFO stMenu;
		int nCount = ::GetMenuItemCount( hMenu);
		for( int nIndex = 0; nIndex < nCount; nIndex++) {
			ZeroMemory( &stMenu, sizeof( MENUITEMINFO));
			stMenu.cbSize = sizeof( MENUITEMINFO);
			stMenu.fMask = MIIM_DATA | MIIM_SUBMENU | MIIM_TYPE;
			::GetMenuItemInfo( hMenu, nIndex, TRUE, &stMenu);
			/// 削除すべきデータか、確認する
			if( MFT_OWNERDRAW & stMenu.fType) {
				/// データが存在する場合、削除を行う
				if( NULL != stMenu.dwItemData) {
//					ITEMDATA* lpstItemData = reinterpret_cast<ITEMDATA*>(stMenu.dwItemData);
//					free( reinterpret_cast<void *>( lpstItemData->mallocAddr ) );
					free( reinterpret_cast<void *>( stMenu.dwItemData ) );
				}
			}
			/// サブメニューは、挿入時に委譲されたものとみなして、削除する
			if( NULL != stMenu.hSubMenu)
				DestroyMenu( stMenu.hSubMenu);
		}
		::DestroyMenu( hMenu);

		return TRUE;
	}
	return FALSE;
}

/// 親となるWindowの削除用
BOOL CMenu::DestroyWindow( HWND hWnd)
{
	if( NULL != hWnd) {
		::DestroyWindow( hWnd);
		return TRUE;
	}
	return FALSE;
}

/// メニューおよび、親となるWindowの削除を同時に行う
void CMenu::Delete( void)
{
	if( DestroyMenu( m_hMenu))
		m_hMenu = NULL;
//	if ( listData.size() >= 0 ) {
//		for( list<LPVOID>::iterator it = listData.begin(); it != listData.end(); it++ ) {
//			if ( (*it) != NULL )
//				free( (*it) );
//		}
//	}
	if( DestroyWindow( m_hParent))
		m_hParent = NULL;
}

/// HMENUをクラスに関連付ける
BOOL CMenu::Attach( HMENU hMenu)
{
	/// 現在関連付けられているHMENUは破棄する。
	DestroyMenu( m_hMenu);
	m_hMenu = hMenu;
	return TRUE;
}

/// HMENUをこのクラスから切り離す。
HMENU CMenu::Detach( void)
{
	HMENU hMenu = m_hMenu;
	m_hMenu = NULL;
	return hMenu;
}

/// WindowsMessageのオーナー描画対応関数
void CMenu::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if( ODT_MENU != lpDrawItemStruct->CtlType)
		return;

	COLORREF	clrBkColor;
	COLORREF	clrTextColor;

	/// MENUITEMの表示状態に従って背景色と文字色を選択
	if( lpDrawItemStruct->itemState & ODS_SELECTED) {
		clrBkColor = clrBkColorHighlight;
		clrTextColor = clrTextColorHighlight;
	} else {
		if( lpDrawItemStruct->itemState & ODS_DISABLED) {
			clrBkColor = clrBkColorMenu;
			clrTextColor = clrTextColorGray;
		} else {
			clrBkColor = clrBkColorMenu;
			clrTextColor = clrTextColorText;
		}
	}

	HBRUSH hBrush = ::CreateSolidBrush( clrBkColor);
	if ( ::FillRect( lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, hBrush) == FALSE )
		return;
	::DeleteObject( hBrush );

	if( 0 <= lpDrawItemStruct->itemID) {
		ITEMDATA* pstItemData = reinterpret_cast< ITEMDATA*>( lpDrawItemStruct->itemData);
		if( NULL != pstItemData && sizeof( ITEMDATA) <= pstItemData->cbSize) {
			int nSaveID = SaveDC( lpDrawItemStruct->hDC);
			if( nSaveID == 0)
				return;
			if ( ::SetBkColor( lpDrawItemStruct->hDC, clrBkColor) == CLR_INVALID )
				return;
			if ( ::SetTextColor( lpDrawItemStruct->hDC, clrTextColor) == CLR_INVALID )
				return;

			BITMAP		stBitmap;
			HBITMAP	hChkMark = ::LoadBitmap( NULL, reinterpret_cast<LPCTSTR>(OBM_CHECK) );
			if ( ::GetObject( hChkMark, sizeof( BITMAP), &stBitmap) == 0 )
				return;

			/// チェックの描画を実施
			if( lpDrawItemStruct->itemState & ODS_CHECKED) {
				HDC	hMemDC = ::CreateCompatibleDC( lpDrawItemStruct->hDC);
				if (hMemDC == NULL )
					return;

				HBITMAP hBitmapOld = (HBITMAP)::SelectObject( hMemDC, hChkMark);
				if ( ::BitBlt( lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.left + CYBorder, lpDrawItemStruct->rcItem.top  + ( ( ( lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) - stBitmap.bmHeight) / 2), stBitmap.bmWidth, stBitmap.bmHeight, hMemDC, 0, 0, SRCCOPY) == FALSE )
					return;
				::SelectObject( hMemDC, hBitmapOld);
				if ( ::DeleteDC( hMemDC) == FALSE )
					return;
			}
			lpDrawItemStruct->rcItem.left += stBitmap.bmWidth;
			lpDrawItemStruct->rcItem.left += CXBorder;

			DeleteObject( hChkMark);

			/// アイコンの描画を実施
			if( pstItemData->hBitmap) {
				HDC	hMemDC = ::CreateCompatibleDC( lpDrawItemStruct->hDC);
				if( hMemDC == NULL )
					return;
				HBITMAP hBitmapOld = ( HBITMAP)::SelectObject( hMemDC, pstItemData->hBitmap);
				if ( ::BitBlt( lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.left + CYBorder, lpDrawItemStruct->rcItem.top + 1, _ICON_WIDTH, _ICON_HEIGHT, hMemDC, 0, 0, SRCCOPY) == FALSE )
					return;
				::SelectObject( hMemDC, hBitmapOld);
				if ( ::DeleteDC( hMemDC) == FALSE )
					return;
			}

			/// メニュー文字列の描画を実施
			HFONT hFonttemp = (HFONT)::SelectObject( lpDrawItemStruct->hDC, hFont);
			if ( ::SetBkColor( lpDrawItemStruct->hDC, clrBkColor) == CLR_INVALID )
				return;
			if ( ::SetTextColor( lpDrawItemStruct->hDC, clrTextColor) == CLR_INVALID )
				return;
			lpDrawItemStruct->rcItem.left += _ICON_WIDTH;
			lpDrawItemStruct->rcItem.left += CXBorder;
			lpDrawItemStruct->rcItem.left += CXFrame;
			if ( ::DrawText( lpDrawItemStruct->hDC, pstItemData->lpItemString, -1, &lpDrawItemStruct->rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE) == 0 )
				return;
			/// 影表示になるよう文字列を再描画する
			if( ( lpDrawItemStruct->itemState & ODS_SELECTED) && ( lpDrawItemStruct->itemState & ODS_DISABLED)) {
				if ( ::SetBkMode( lpDrawItemStruct->hDC, TRANSPARENT) == 0 )
					return;
				lpDrawItemStruct->rcItem.left--;
				lpDrawItemStruct->rcItem.top--;
				lpDrawItemStruct->rcItem.bottom--;
				if ( ::SetTextColor( lpDrawItemStruct->hDC, dwColor) == CLR_INVALID )
					return;
				if ( ::DrawText( lpDrawItemStruct->hDC, pstItemData->lpItemString, -1, &lpDrawItemStruct->rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE) == FALSE )
					return;
			}
			::SelectObject( lpDrawItemStruct->hDC, hFonttemp);
			::RestoreDC( lpDrawItemStruct->hDC, nSaveID);
		}
	}

//LPVOID lpMsgBuf;
//
//FormatMessage(
//    FORMAT_MESSAGE_ALLOCATE_BUFFER |
//    FORMAT_MESSAGE_FROM_SYSTEM |
//    FORMAT_MESSAGE_IGNORE_INSERTS,
//    NULL,
//    GetLastError(),
//    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
//    (LPTSTR) &lpMsgBuf,
//    0,
//    NULL
//
//);
//
//ATLTRACE( _T("%s"), lpMsgBuf );
//LocalFree(lpMsgBuf);

}

/// WindowsMessageのメニューサイズ応答関数
void CMenu::OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if( ODT_MENU != lpMeasureItemStruct->CtlType)
		return;

	if( NULL != lpMeasureItemStruct->itemData) {
		ITEMDATA* pstItemData = reinterpret_cast< ITEMDATA*>( lpMeasureItemStruct->itemData);
		if( NULL != pstItemData && sizeof( ITEMDATA) <= pstItemData->cbSize) {
			lpMeasureItemStruct->itemWidth = pstItemData->stSize.cx;
			lpMeasureItemStruct->itemHeight = pstItemData->stSize.cy;
		}
	}
}

HRESULT CALLBACK CMenu::WindowProc( HWND hWnd, UINT unMessage, WPARAM wParam, LPARAM lParam)
{
	switch( unMessage) {
	case WM_CREATE:
		{
			LPCREATESTRUCT lpCreateStruct = reinterpret_cast< LPCREATESTRUCT>( lParam);
			::SetWindowLong( hWnd, GWL_USERDATA, reinterpret_cast< LONG>( lpCreateStruct->lpCreateParams));
		}
		return 0;

	case WM_DRAWITEM:
		{
			CMenu* pcMenu = reinterpret_cast< CMenu*>( ::GetWindowLong( hWnd, GWL_USERDATA));
			pcMenu->OnDrawItem( static_cast< int>( wParam), reinterpret_cast< LPDRAWITEMSTRUCT>( lParam));
		}
		return TRUE;

	case WM_MEASUREITEM:
		{
			CMenu* pcMenu = reinterpret_cast< CMenu*>( ::GetWindowLong( hWnd, GWL_USERDATA));
			pcMenu->OnMeasureItem( static_cast< int>( wParam), reinterpret_cast< LPMEASUREITEMSTRUCT>( lParam));
		}
		return TRUE;
	}
	return ::DefWindowProc( hWnd, unMessage, wParam, lParam);
}

/// 親となるWindowのクラス登録関数
ATOM CMenu::RegisterClass( void)
{
	WNDCLASS stWndClass;
	stWndClass.style = CS_HREDRAW|CS_VREDRAW;
	stWndClass.lpfnWndProc = CMenu::WindowProc;
	stWndClass.cbClsExtra = 0;
	stWndClass.cbWndExtra = sizeof( this);
	stWndClass.hInstance = _AtlBaseModule.GetModuleInstance();
	stWndClass.hIcon = NULL;
	stWndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	stWndClass.hbrBackground = (HBRUSH)::GetSysColorBrush(COLOR_BTNFACE);
	stWndClass.lpszMenuName = 0;
	stWndClass.lpszClassName = m_lpcszMenuParent;
	return ::RegisterClass( &stWndClass);
}

/// メニューに含まれれるアイテムの個数を返す
UINT CMenu::GetMenuItemCount( ) const
{
	return ::GetMenuItemCount( m_hMenu);
}

/// 親付きのメニューの親を取得。WM_MENUSELECT&SubclassWindow対策
HWND CMenu::GetParentWnd() const
{
	return m_hParent;
}
}

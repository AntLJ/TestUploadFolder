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
 * @file winutil.cpp
 * @brief ユーティリティー関数の実装
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "winutil.h"
#include "exdisp.h"
#include "shellapi.h"

namespace winutil {

bool BSTR2TCpy( LPTSTR lpszDst, BSTR bstrSrc, int nLen )
{
#ifdef _UNICODE
	if( (unsigned int)nLen >= SysStringLen(bstrSrc) + 1 )	// SysStringLenはNULL文字を含めないが、lstrcpynはNULL文字分も要求するので+1
	{
		if( NULL != lstrcpyn( lpszDst, bstrSrc, nLen ) )
			return true;
		else {
			return false;
		}
	}
	else return false;
#else
	int nReqBufferSize = WideCharToMultiByte( GetACP(), 0, bstrSrc, -1, lpszDst, 0, NULL, NULL );	// 必要なバッファサイズを取得
	if( (int)sizeof(char*) * nLen < nReqBufferSize )
	{
		return false;
		//nRet = nReqBufferSize - sizeof(char*) * nLen;
	}
	else {
		if( 0 == WideCharToMultiByte( GetACP(), 0, bstrSrc, -1, lpszDst, sizeof(char*) * nLen, NULL, NULL ) )
		{
			return false;
			// GetLastError() で ERROR_INSUFFICIENT_BUFFER か ERROR_INVALID_FLAGS か ERROR_INVALID_PARAMETER がかえるはず
		}
		else return true;
	}
#endif
}

// ウィンドウの文字列を取得する
CString GetWindowText( HWND hWnd )
{
	if( hWnd == NULL ) return _T("");

	USES_ATL_SAFE_ALLOCA;

	INT nLen = ::GetWindowTextLength( hWnd ) + 1;
	LPTSTR lpszText = (LPTSTR)_ATL_SAFE_ALLOCA( nLen * sizeof(TCHAR), _ATL_SAFE_ALLOCA_DEF_THRESHOLD );
	if( lpszText )
		::GetWindowText( hWnd, lpszText, nLen );
	else
		_ASSERTE( lpszText );

	CString strRet( lpszText );

	return strRet;
}

// 時間を文字列で取得する
CString GetTime( LPSYSTEMTIME lpSystemTime/* = NULL*/ )
{
	SYSTEMTIME stSystemTime;
	if( NULL != lpSystemTime)
	{
		CopyMemory( &stSystemTime, lpSystemTime, sizeof( SYSTEMTIME));
	}
	else
	{
		::GetLocalTime( &stSystemTime);
	}
	CString cStrModifyDate;
	cStrModifyDate.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	return cStrModifyDate;
}

// 文字列でマシン名を取得する
CString GetMachineName()
{
	TCHAR				strMachineName[64];
	DWORD				nSize = 63;
	ZeroMemory(strMachineName, 64 * sizeof(TCHAR));
	::GetComputerName(strMachineName, &nSize);

	return strMachineName;
}

// 文字列でOS名を取得する
CString GetOperatingSystem()
{
	CString strVerName;
	OSVERSIONINFO tInfo;
	ZeroMemory( &tInfo, sizeof( OSVERSIONINFO));
	tInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (::GetVersionEx(&tInfo) != 0)
	{
		switch(tInfo.dwMajorVersion)
		{
		case 3:
			strVerName = _T("Windows NT 3.51");
			break;
		case 4:
			if (tInfo.dwMinorVersion == 0 && tInfo.dwPlatformId == 1)
				strVerName = _T("Windows 95");
			else if (tInfo.dwMinorVersion == 0 && tInfo.dwPlatformId == 2)
				strVerName = _T("Windows NT 4");
			else if (tInfo.dwMinorVersion == 10)
				strVerName = _T("Windows 98");
			else
				strVerName = _T("Unknown");
			break;
		case 5:
			if (tInfo.dwMinorVersion == 0)
				strVerName = _T("Windows 2000");
			else if (tInfo.dwMinorVersion == 1)
				strVerName = _T("Windows XP");
			else
				strVerName = _T("Unknown");
			break;
		default:
			strVerName = _T("Unknown");
			break;
		}
	}
	else
	{
		strVerName.Empty();
	}

	return strVerName;
}

_COM_SMARTPTR_TYPEDEF( IWebBrowser2, __uuidof(IWebBrowser2) );

// 別ウィンドウでブラウザを表示する
void Navigate( LPCTSTR lpcszURL )
{
	// この関数はsindykToolsからコピーしました。

	// デフォルトのブラウザで何を使っているか取得
	CString strPath;
	CRegKey cRegKey;
	if( ERROR_SUCCESS == cRegKey.Open( HKEY_CLASSES_ROOT, _T("HTTP\\shell\\open\\command") ) )
	{
		TCHAR cPath[1024] = {0};
		ULONG ulLen = 1024;
		if( ERROR_SUCCESS == cRegKey.QueryStringValue( NULL, cPath, &ulLen ) )
			strPath = cPath;
		cRegKey.Close();
	}

	//[Bug 6227]情報源URLは新しいウィンドウで開くようにして欲しい。
	// ShellExecuteだと、IE複数開けないので、場合わけ
	if( strPath.Find( _T("iexplore.exe") ) > 0 )
	{
		IWebBrowser2Ptr pIE( __uuidof( InternetExplorer ) );
		if ( pIE ) {
			CComVariant dummy1, dummy2, dummy3, dummy4;
			CComVariant url( (CComBSTR)lpcszURL );

			//IEの表示
			pIE->Navigate2( &url, &dummy1, &dummy2, &dummy3, &dummy4 );
			pIE->put_Visible(VARIANT_TRUE);
		}
	}
	// IE以外はタブブラウザだろうと思われる、、、ので、ShellExecuteで楽にやってくれる
	else
		ShellExecute(0, _T("open"), lpcszURL, 0, 0, SW_SHOWNORMAL);
}

// クリップボードに文字列をコピーする関数
bool SetToClipboard( const CString& strText )
{
	int iLength = strText.GetLength();
	if (iLength > 1024)
		iLength = 1024;
	HGLOBAL hGlobal = ::GlobalAlloc(GHND, 2*( iLength + 1 ));
	if (hGlobal == NULL)
		return false;

	//グローバルメモリ領域のロック
	LPTSTR str = (LPTSTR)::GlobalLock(hGlobal);
	if( ! str )
	{
		GlobalFree(hGlobal);
		return false;
	}

	::lstrcpy(str,strText);

	//グローバルメモリ領域のロック解除
	::GlobalUnlock(hGlobal);

	//	クリップボードを開く
	if( !( ::OpenClipboard(NULL) ) )
	{
		GlobalFree(hGlobal);
		return false;
	}
	if( !( ::EmptyClipboard() ) )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return false;
	}
	if( ::SetClipboardData(CF_UNICODETEXT, hGlobal) == NULL )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return false;
	}
	::CloseClipboard();//クリップボードのクローズ

	return true;
}

// クリップボードの文字列を返す関数
CString GetFromClipboard()
{
	CString str;
	// クリップボードのデータがテキスト型か判定する
	UINT PriList[2] = {CF_UNICODETEXT,CF_TEXT};
	int format = ::GetPriorityClipboardFormat(PriList, 2);
	if ( format == CF_TEXT || format == CF_UNICODETEXT )
	{
		OpenClipboard(NULL);
		HANDLE hText = GetClipboardData(format);
		if(hText == NULL)
			return str;
		else
		{
			LPTSTR lpData = static_cast<LPTSTR>(GlobalLock(hText));
			str = lpData;
			GlobalUnlock(hText);
		}

		CloseClipboard();
	}
	return str;
}

} // winutil

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
 * @file ErrorCode.cpp
 * @brief SiNDYで使用するエラーコードの定義実装
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "ErrorCode.h"
#include "GlobalFiles.h"
#include "GlobalFunctions.h"
#include <limits.h>
#include "sindymacroutil.h"
#include "winutil.h"
#include "resource.h"
#if defined(UNICODE) && defined(NDEBUG)
#include <stdio.h>
#endif // define(UNICODE) && define(NDEBUG)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace winutil;
namespace sindy {
namespace errorcode {

// sindyErrCodeを文字列に変換する
LPCTSTR SINDYERRCODE2STRING( sindyErrCode code )
{
	switch( code )
	{
#ifdef UNICODE
#define ERROR_CODE(id,...) case id: return L#__VA_ARGS__; break;
#else // UNICODE
#define ERROR_CODE(id,...) case id: return #__VA_ARGS__; break;
#endif // UNICODE
#include "ErrorCodeBase.h"
#undef ERROR_CODE
		default: break;
	}
	return NULL;
}

// sindyErrCodeをエラーログに出力する
void _ERRORLOG( sindyErrCode code, LPCTSTR lpcszFormat, ...)
{
	if( code == sindyErr_NoErr ) return;	// エラー無しはログに出さない

	// エラーログファイルオープン
	if( ! g_Errlog )
		errno_t err = _tfopen_s( &g_Errlog, g_strErrlog, _T("a") );

	va_list args = NULL;
	if( lpcszFormat )
		va_start(args, lpcszFormat);

	ErrorlogV( code, lpcszFormat, args, g_Errlog );

	// 後処理
	va_end(args);
}

// sindyErrCodeとHRESULT値をエラーログに出力する
void _ERRORHRLOG( sindyErrCode code, HRESULT hr, LPCTSTR lpcszFormat, ...)
{
	if( code == sindyErr_NoErr && hr == S_OK ) return;	// エラー無しはログに出さない

	// エラーログファイルオープン
	if( ! g_Errlog )
		errno_t err = _tfopen_s( &g_Errlog, g_strErrlog, _T("a") );

	va_list args = NULL;
	if( lpcszFormat )
		va_start(args, lpcszFormat);

	ErrorlogV( code, lpcszFormat, args, g_Errlog, hr );

	// 後処理
	va_end(args);
}

void ErrorlogV( sindyErrCode code, LPCTSTR lpcszFormat, va_list args/* = NULL*/, FILE* out/* = NULL*/, HRESULT hr/* = S_OK*/ )
{
	if( code == sindyErr_NoErr && hr == S_OK ) return;	// エラー無しはログに出さない

	// エラーコードからエラー文字列取得
	LPCTSTR lpszErrCode = SINDYERRCODE2STRING( code );

	// フォーマット文字列作成
	CString strFMsg;
	if( args )
		strFMsg.FormatV( lpcszFormat, args );
	else
		strFMsg = lpcszFormat;

	// COMエラー文字列取得
	CString strCOMErr = GETCOMERRORSTRING();
	
	CString strDbgMsg;
	strDbgMsg.Format(_T("%s%s : %s\n"), strFMsg, lpszErrCode, strCOMErr );

	CString strMsg;
	strMsg.Format(_T("%s,%4d,%s,%s,%s\n"), GetTime(), code, lpszErrCode, strFMsg, strCOMErr );

#ifdef _DEBUG
#ifdef _UNICODE
	OutputDebugStringW(strDbgMsg);
#else
	OutputDebugStringA(strDbgMsg);
#endif // _UNICODE
#endif // DEBUG

	// 標準出力に表示する（スタティックリンク版のみ）
#if ! defined(SINDYLIB_EXPORTS) || ! defined(USE_SINDYLIB_DLL)
	_tprintf_s( _T("%s"), strMsg );
#endif // ! defined(SINDYLIB_EXPORTS) || ! defined(USE_SINDYLIB_DLL)
	
	// ファイルに表示する
	if( ! out )
	{
		_tprintf_s( strMsg );
		fflush( stdout );
	}
	else {
		_ftprintf_s( out, _T("%s"), strMsg );
		fflush( out );
	}
}

} // errorcode

} // sindy

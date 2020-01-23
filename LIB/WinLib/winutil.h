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
 * @file winutil.h
 * @brief Windows ユーティリティー関数の定義
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _WINUTIL_H_
#define _WINUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winutil {

bool BSTR2TCpy( LPTSTR lpszDst, BSTR bstrSrc, int nLen );

/// ウィンドウの文字列を取得する
CString GetWindowText( HWND hWnd );

/// 時間を文字列で取得する
CString GetTime( LPSYSTEMTIME lpSystemTime = NULL );

/**
 * @brief <b>文字列でマシン名を取得する</b>\n
 * @return マシン名
 */
CString GetMachineName();

/**
 * @brief <b>文字列でOS名を取得する</b>\n
 * @return OS名
 */
CString GetOperatingSystem();

/// 別ウィンドウでブラウザを表示する
void Navigate( LPCTSTR lpcszURL );

/// クリップボードに文字列をコピーする関数
bool SetToClipboard( const CString& strText );

/// クリップボードの文字列を返す関数
CString GetFromClipboard();


} // winutil

#endif // _UTIL_H_

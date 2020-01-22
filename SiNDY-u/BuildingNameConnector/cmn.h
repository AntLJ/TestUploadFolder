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
 *	@file	cmn.h
 *	@brief	<b>共通ヘッダファイル</b>\n
 *	@author	Fusatoshi Abe
 *	$ID$
 */
#ifndef	_CMN_H_
#define	_CMN_H_

#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>

#include <iostream>

extern CComModule _Module;

#include <atlcom.h>
#include <comdef.h>

#include <Oaidl.h>
#pragma comment(lib, "Oleaut32.lib")

#include <CStringTokenizer.h>

/**
 *	@brief	<b>コンソールウィンドウの取得関数</b>\n
 *	@return コンソールウィンドウハンドル
 */
inline HWND GET_CONSOLE_WINDOW()
{
	return reinterpret_cast<HWND(_stdcall*)(VOID)>(GetProcAddress(LoadLibrary(_T("kernel32.dll")), "GetConsoleWindow"))();
}


#endif	//_CMN_H_

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

// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__7EA8BC90_D03A_4A6B_B0E9_80095744F68D__INCLUDED_)
#define AFX_STDAFX_H__7EA8BC90_D03A_4A6B_B0E9_80095744F68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C標準の危険な関数の警告を出さない
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996の警告を出さない
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATEや_CRT_SECURE_NO_DEPRECATEでC4996が回避されない！！

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows 2000 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0500		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows 2000 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0500	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600	// これを IE. の他のバージョン向けに適切な値に変更してください。
#endif

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
#include <stdio.h>
#include <tchar.h>

#define _CRTDBG_MAP_ALLOC // mallocでメモリ確保したもののリークを出力
///////////////////////
// MFCを使用する場合
#ifdef _AFXDLL
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外します。
#endif

#include <afx.h>
#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#else
///////////////////////
// MFCを使用しない場合
#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
// MFCと同じ方法でnewでメモリ確保したもののリークを出力
// 各cppファイルに以下の記述が必要です（MFCと共通）
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif // ifdef _DEBUG
#endif // ifdef _AFX_DLL

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#include <atlbase.h>
#include <atlstr.h>

#ifndef NOT_USE_ARC_OBJECTS
#include <WinLib/arcobjects_import_rawmethod.h>
#import <sindyeCore.tlb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#endif // NOT_USE_ARC_OBJECTS

#include <list>
#include <map>
#include <vector>
#include <boost/assign.hpp>
#include <sindy/libschema.h>

#define SINDYLIB_CALC_TIME		//!< 各種時間計測
#define _TEST_NEW_MODIFYCHECK	//!< 新しいModifyCheckプロシージャテスト用

#pragma warning(disable: 4503)	// 限界超えたっていいじゃん
#pragma warning(disable: 4786)	// 切り捨てられたっていいじゃん
#pragma warning(disable: 4290)	// 例外指定くらい、無視されても良い…んじゃ？

extern "C" const GUID __declspec(selectany) CLSID_SMWProgressDialogFactory = {0x5a2ea561,0xfca0,0x41ab,{0xa6,0xd7,0xbb,0xb2,0xff,0x4a,0x03,0x81}};
extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj             = {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}}; // for AheGlobalsX.h

#include "../LIB/WinLib/libwinlib.h"
#include "../LIB/WinLib/winutil.h"
#include "../LIB/WinLib/Str2.h"
#include "../LIB/WinLib/charutil.h"
#include "../LIB/WinLib/macro_util.h"
#include "../LIB/WinLib/type_convert.h"
#include "../../WinLib/libwinlib.h"
#include "coord_converter.h"
#include "define.h"
#include "libarchelper.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__7EA8BC90_D03A_4A6B_B0E9_80095744F68D__INCLUDED_)

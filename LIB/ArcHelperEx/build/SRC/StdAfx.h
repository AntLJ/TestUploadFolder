/**
 * @file StdAfx.h
 * @brief 標準のシステム インクルード ファイル、または参照回数が多く、<br>
 *        かつあまり変更されない
 *        プロジェクト専用のインクルード ファイルが記述されています。<br>
 *        また、このファイルはプリコンパイルヘッダとなるため、すべての
 *        *.cppファイルでインクルードする必要があります。
 *
 * @author furukawa (INCREMENT P CORP.) <furukawa@mr.ipc.pioneer.co.jp>
 * $Date: 2004/02/27 05:12:16 $
 * $Id: StdAfx.h,v 1.14 2004/02/27 05:12:16 hata_k Exp $
 *
 * Copyright &copy; 2004 INCREMENT P CORP., Inc. All rights reserved.
 */

#if !defined(AFX_STDAFX_H__403F5974_6201_4365_ABEE_2D0AD8554D0D__INCLUDED_)
#define AFX_STDAFX_H__403F5974_6201_4365_ABEE_2D0AD8554D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C標準の危険な関数の警告を出さない
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996の警告を出さない
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATEや_CRT_SECURE_NO_DEPRECATEでC4996が回避されない！！

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

// 実はsindykToolsがv110対応できていなかった暫定対応。。
// 今のところ問題となるのはnullptrだけなのでこれで凌ぐ
#if _MSC_VER == 1500
#define nullptr NULL
#endif

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。
#include <stdio.h>
#include <tchar.h>

#define 	_QUOTE(x)   # x
#define 	QUOTE(x)   _QUOTE(x)
#define 	__FILE__LINE__   __FILE__ "(" QUOTE(__LINE__) ") : "

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

#define OEMRESOURCE

#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlctl.h>	// IPropertyPageImpl を使用するので

#include <WinLib/arcobjects_import_rawmethod.h>
#import "tlb/sindyeCore.tlb"					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "_ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#import <msxml6.dll>

extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj = {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
extern "C" const GUID __declspec(selectany) CLSID_AttributeSelectDlgObj = {0x5AEA755B,0xFCCA,0x41C0,{0x8C,0x3F,0x19,0xBF,0x60,0xFC,0x3C,0x3F}};

#define __SINDYE_NO_CHECK_NODE_ATTRIBUTE__	// [bug 1944] 二叉路のノードに信号機・交差点名称属性がついている場合の確認は不要
#define _RELATIONALOPERATOR_EQUALS_BUG_FIXED
#define _NOT_USE_SDE_RELATIONSHIP

#include <list>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "define.h"
#include "charutil.h"
#include "Str2.h"
#include "sindy/libschema.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

/**
 * \mainpage
 *
 * このライプラリは、ArcObjectsを使用する上で便利と思われるクラス
 * 群や関数群を集めた物となっています。
 *
 * ライブラリ自体はCVSで管理されています。
 * 特に理由がない限り、最新版のArcHelperを使用することをお勧めします。
 * 
 * ArcHelperExの取得方法：
 *
 * インクルードファイル、ライブラリ、関連ライブラリ、関連ライブラリインクルードファイル
 * をすべて取得しておく必要があります。
 *
 * - SiNDY-e/ArcHelperEx/include 以下
 * - SiNDY-e/COMMON/ 以下
 * - SiNDY-u/COMMON/lib/crd_cnv.lib
 * - SiNDY-u/crd_cnv/crd_cnv.h
 * - TOOLS/ 以下
 *
 * もしくは、SiNDY-e、SiNDY-uを丸ごとチェックアウトしてください。
 *
 * ArcHelperExの利用方法：
 * 
 * COMMON/ArcHelperEx.hをインクルードしてください。
 */
#endif // !defined(AFX_STDAFX_H__403F5974_6201_4365_ABEE_2D0AD8554D0D__INCLUDED_)

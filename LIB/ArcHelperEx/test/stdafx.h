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

// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C標準の危険な関数の警告を出さない
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996の警告を出さない
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATEや_CRT_SECURE_NO_DEPRECATEでC4996が回避されない！！

#ifndef STRICT
#define STRICT
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

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atlctl.h>	// IPropertyPageImpl を使用するので

// Disable a warning when importing the type library, but only for this one import
#pragma warning(push)
#pragma warning(disable : 4005)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#pragma warning(disable : 4278)
#import <esriSystem.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <esriSystemUI.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IProgressDialog", "_IProgressDialog") rename("ICommand", "_ICommand")
#import <esriFramework.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE", "UINT_PTR") rename("ICommand", "_ICommand") 
#import <esriGeoDatabaseUI.olb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <esriArcMapUI.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <esriGeometry.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("ISegment", "_ISegment")
#import <esriDisplay.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("ISegment", "_ISegment")
#import <esriGeoDatabase.olb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#import <esriDataSourcesFile.olb>			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <esriDataSourcesGDB.olb>			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <esriCarto.olb>						raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE", "UINT_PTR")
#import <esriLocation.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <esriEditor.olb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <sindyeCore.tlb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#pragma warning(pop)

#include <CatIDs/ArcCATIDs.h>
#include "SiNDYCATIDs.h"
#include "SiNDYCLSIDs.h"

#include <list>
#include <map>
#include <vector>

using namespace ATL;

#include "ArcHelperEx.h"

#pragma comment(lib,"cppunitd.lib")

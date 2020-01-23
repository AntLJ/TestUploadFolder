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

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600	// これを IE. の他のバージョン向けに適切な値に変更してください。
#endif

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#include <atlbase.h>
#include <atlstr.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。
#define _ATL_TMP_NO_CSTRING

#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>

#include <atlcrack.h>
#include <atlmisc.h>
//#include <atlctrls.h>
#include <atlddx.h>
#include <atlstr.h>

#pragma warning(push)
#pragma warning(disable : 4146 4192)
#import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriSystemUI.olb> raw_interfaces_only no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment")
#import <esriDisplay.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment")
#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("IRelationship", "_IRelationship")
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import <esriDataSourcesFile.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
#import <esriDataSourcesRaster.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_COLOR","OLE_HANDLE","ICursor","VARTYPE") rename("IBinFunction","_IBinFunction") rename("ICursor","_ICursor")
#import <esriOutput.olb> raw_interfaces_only no_namespace named_guids
#import <esriGISClient.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriGeoDatabaseExtensions.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor")
#import <esriCarto.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids exclude("UINT_PTR")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" high_method_prefix("_") raw_method_prefix("") no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
#pragma warning(pop)
#pragma warning(disable : 4503 4786 6334)

#include "LocalDefine.h"
#include "Output.h"

extern COutput g_cOutput;

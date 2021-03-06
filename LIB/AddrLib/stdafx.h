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

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <atlbase.h>
#include <atlstr.h>

#pragma warning (push)

#pragma warning (disable : 4192)
#pragma warning (disable : 4278)
#pragma warning (disable : 4996)

#include <WinLib/arcobjects_import_rawmethod.h>

#pragma warning (pop)

#include <map>
#include <vector>
#include <list>
#include <TDC/useful_headers/tstring.h>

#define _USE_SINDYLIB_STATIC
#include <sindy/libschema.h>
#include <SiNDYLib/SiNDYLib.h>
#include <WinLib/libwinlib.h>
#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#import <tlb/sindyeCore.tlb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#import <tlb/msado15.dll> no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")

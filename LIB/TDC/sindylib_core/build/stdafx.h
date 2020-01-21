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

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクターは明示的です。


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#if _MSC_VER < 1700
#include "memdbg.h" // メモリリーク検出
#endif // 

#include <atlbase.h>
#include <atlcom.h>
//#include <atlctl.h>
#include <atlstr.h>

#include "arcobjects_import_rawmethod.h"
#import "tlb/sindyeCore.tlb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#import "tlb/msado15.dll" no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")

#if _MSC_VER >= 1700
#include "memdbg.h" // メモリリーク検出
#endif // 

#include <map>
#include <vector>
#include <list>

#include "sindy/schema.h"

using namespace ATL;

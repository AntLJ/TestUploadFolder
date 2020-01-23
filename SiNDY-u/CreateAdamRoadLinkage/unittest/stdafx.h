// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト固有のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

// CppUnitTest のヘッダー
#include "CppUnitTest.h"

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlstr.h>
#include <TDC/useful_headers/tstring.h>
#include <vector>
#include <assert.h>

#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#import "tlb/sindyeCore.tlb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#pragma warning(pop)

// lib
#define _USE_SINDYLIB_STATIC
#include <coord_converter.h>
#include <libarchelper.h>
#include <libsindylibcore.h>
#include <libsindylibbase.h>
#include <sindy/libschema.h>
#include <sindy/workspace.h>
#include <libwinlib.h>

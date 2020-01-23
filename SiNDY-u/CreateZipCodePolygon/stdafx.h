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

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlstr.h>
#include <assert.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

#include <TDC/useful_headers/tstring.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_rawmethod.h>
#import <tlb/sindyeCore.tlb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#pragma warning(pop)

#include <sindy/libschema.h>
#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>
#include <WinLib/libwinlib.h>
#define _USE_SINDYLIB_STATIC
#include <TDC/sindylib_core/libsindylibcore.h>
#include <TDC/sindylib_base/libsindylibbase.h>
#include <AddrLib/AddrLib.h>
#include <sindy/workspace.h>

#include "ToolDefine.h"

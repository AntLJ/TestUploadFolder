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

#pragma once

#ifndef STRICT
#define STRICT
#endif
#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。
#define _ATL_TMP_NO_CSTRING     // ATLとWTLでCStringがかぶるため

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
// atlctrls.hより先に読み込むことでコントロールクラスでCStringサポート
// そのほか、atlapp.hにもCStringのifdefがあるので、WTLよりも先に読み込む必要がある
#include <atlstr.h>
#include <atlapp.h>             // WTLを使用
#include <atlctl.h>
#include <atldlgs.h>
#define _ATL_TMP_NO_CSTRING     // ATLとWTLでCStringがかぶるため
#include <atlmisc.h>            // CPoint CSize CRect
#include <atlctrls.h>
#include <atlgdi.h>				// CDC                 （WinMgrで使用）
#include <atlcrack.h>			// メッセージ系
#include <shellapi.h>
#include <atlctrlx.h>

using namespace ATL;


#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192) 
#include <WinLib/arcobjects_import_rawmethod.h>
#import "tlb/sindyeCore.tlb"			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#import "VSFLEXGRID/VSFLEX8N.OCX"		no_namespace named_guids 
#import "tlb/msado15.dll"				no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")

#pragma warning(pop)

#include <list>
#include <map>
#include <vector>
#include <fstream>

#include "../SiNDY-e/COMMON/HelpID.h"
#include "../SiNDY-e/COMMON/SiNDYCATIDs.h"
#include "../SiNDY-e/COMMON/SiNDYCLSIDs.h"
#include <sindy/libschema.h>
#include "CatIDs/ArcCATIDs.h"
#include "ArcHelperEx.h"
#define _USE_SINDYLIB_STATIC
#include "sindylib.h"
#include "macro_util.h"
#include "winutil.h"

#include "coord_converter.h"

#include <arctl/coinitializer.h>

using namespace std;

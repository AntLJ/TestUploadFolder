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

#pragma once


#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

////////////////////////////
// ATL サポート
////////////////////////////
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>

using namespace ATL;

#include <iostream>
#include <list>
#include <map>


////////////////////////////
// ArcObjects
////////////////////////////
#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#pragma warning(disable : 4042)
#pragma warning(disable : 4278)

#include "WinLib\arcobjects_import_rawmethod.h"

//#import "tlb/sindyeCore.tlb"		raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
//#import"OCIExtension.tlb"			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
//#import <msado15.dll> no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")
#pragma warning(pop)

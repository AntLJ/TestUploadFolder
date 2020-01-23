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

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#include <atlbase.h>
#include <atlapp.h>
extern CComModule _Module;
#include <atlconv.h>
#include <atlwin.h>

 #pragma warning(push)
 #pragma warning(disable : 4146) /* Ignore warnings for use of minus on unsigned types */
 #pragma warning(disable : 4192) /* Ignore warnings for types that are duplicated in win32 header files */
 #pragma warning(disable : 4278) /* Ignore warnings for use of minus on unsigned types */ 
 #import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR") rename("XMLSerializer", "_XMLSerializer")
 #import <esriSystemUI.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids, rename("IProgressDialog", "_IProgressDialog"), rename("ICommand", "_ICommand")
 #import <esriGeometry.olb>	high_method_prefix("_") raw_method_prefix("")  no_namespace named_guids, rename("ISegment", "_ISegment")
 #import <esriDisplay.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids
 #import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix(""), no_namespace, named_guids, rename("IRow", "_IRow"), rename("ICursor", "_ICursor")
 #import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
#pragma warning(pop)

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <atlstr.h>
#include "coord_converter.h"
#include "libwkf.h"
#include <sindy/workspace.h>
#include <sindy/schema.h>

#include <SHPWrap.h>
#include "shapefil.h"

#include "sindy/libschema.h"
#include "libshape.h"
#include "libshapewrap.h"

const static LPCTSTR NoParkingRegR = _T("Reg_R");
const static LPCTSTR NoParkingRegRImp = _T("Reg_R_Imp");

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

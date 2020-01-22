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

#include <stdio.h>
#include <tchar.h>


// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <iostream>


#include <atlbase.h>
extern CComModule _Module;

#include <arcobjects_import_highmethod.h>
#include <coord_converter.h>
#include <libarchelper.h>
#include <libwinlib.h>

//#import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR")
//#import <esriSystemUI.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids, rename("IProgressDialog", "_IProgressDialog"), rename("ICommand", "_ICommand")
//#import <esriGeometry.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids, rename("ISegment", "_ISegment")
//#import <esriDisplay.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
//#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix("")  no_namespace, named_guids, rename("IRow", "_IRow"), rename("ICursor", "_ICursor"), rename("IRelationship", "_IRelationship")
//#import <esriCarto.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids,  rename("IRow", "_IRow"), rename("ICursor", "_ICursor"), rename("ITableDefinition", "_ITableDefinition")
//#import <esriDataSourcesGDB.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids
//#import <esriDataSourcesFile.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids

//#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" no_namespace raw_interfaces_only no_implementation rename("esriProductCode", "esriVersionProductCode")
#define ESRI_SET_VERSION(prod) \
{\
  HRESULT hr; \
  VARIANT_BOOL vb; \
  IArcGISVersionPtr ipVersion(__uuidof(VersionManager)); \
  if(!SUCCEEDED(hr = ipVersion->LoadVersion(prod, L"", &vb))) \
    fprintf(stderr, "LoadVersion() failed with code 0x%.8x\n", hr); \
  else if(vb != VARIANT_TRUE) \
    fprintf(stderr, "LoadVersion() failed\n"); \
}

#include "Definition.h"


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
#include <atlcom.h>
#include <atlwin.h>
#include <atlapp.h>             // WTLを使用
#include <atlstr.h>             // CString
#include <atltime.h>

#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192) 
#import <esriSystem.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer") rename("ISet","_ISet")
#import <esriSystemUI.olb> raw_interfaces_only no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment")
#import <esriDisplay.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment")
#import <esriGeoDatabase.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("ItemInfo","_ItemInfo") rename("IRelationship","_IRelationship")
#import <esriGeoDatabaseUI.olb> raw_interfaces_only no_namespace named_guids rename("ICursor","_ICursor") rename("VersionManager","ESRIVersionManager")
#import <esriDataSourcesFile.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import <esriDataSourcesOleDB.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesRaster.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_COLOR","OLE_HANDLE","ICursor","VARTYPE") rename("IBinFunction","_IBinFunction") rename("ICursor","_ICursor")
#import <esriCarto.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR") rename("PropertyInfo", "_PropertyInfo")  rename("ITableDefinition", "_ITableDefinition" )
#import <esriCartoUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriDisplayUI.olb> raw_interfaces_only no_namespace named_guids
#import <esriArcMapUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids function_mapping rename("ICommand","_ICommand") exclude("IMxApplication2","IDataConnectionPropertyPage2","IDataGraphWizard")
#import <esriFramework.olb> raw_interfaces_only no_namespace named_guids function_mapping exclude("OLE_COLOR","OLE_HANDLE","ICursorPtr","VARTYPE","UINT_PTR") rename("ICommand","_ICommand") rename("IColorPtr","_IColorPtr")
#import <esriEditor.olb> raw_interfaces_only raw_native_types no_namespace named_guids rename("IRow","_IRow")
#import <esriCatalog.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esri3DAnalyst.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import "tlb/sindyeCore.tlb"			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")

#pragma warning(pop)

/* Engine Version support */
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" no_namespace raw_interfaces_only no_implementation rename("esriProductCode", "esriVersionProductCode")
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
#include <list>
#include <map>
#include <vector>
#include <fstream>

using namespace ATL;

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


#include <arctl/coinitializer.h>

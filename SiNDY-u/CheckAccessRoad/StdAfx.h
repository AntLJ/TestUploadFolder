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

// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__1091F74C_B997_4E01_AF17_18BE4ECC7F7B__INCLUDED_)
#define AFX_STDAFX_H__1091F74C_B997_4E01_AF17_18BE4ECC7F7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _WIN32_WINNT 0x0501

#define VC_EXTRALEAN        // Windows ヘッダーから殆ど使用されないスタッフを除外します

#include <afx.h>
#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>       // MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

// TODO: プログラムで必要なヘッダー参照を追加してください。
#pragma warning (disable : 4192)
#pragma warning (disable : 4146)
#import <esriSystem.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriSystemUI.olb> raw_interfaces_only no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment")
#import <esriDisplay.olb> raw_interfaces_only no_namespace named_guids rename("ISegment","_ISegment")
#import <esriServer.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriOutput.olb> raw_interfaces_only no_namespace named_guids
#import <esriGeoDatabase.olb> raw_interfaces_only no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("ItemInfo","_ItemInfo")
#import <esriGISClient.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriDataSourcesFile.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import <esriDataSourcesOleDB.olb> raw_interfaces_only no_namespace named_guids
#import <esriDataSourcesRaster.olb> raw_interfaces_only no_namespace named_guids exclude("OLE_COLOR","OLE_HANDLE","ICursor","VARTYPE") rename("IBinFunction","_IBinFunction") rename("ICursor","_ICursor")
#import <esriGeoDatabaseDistributed.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriCarto.olb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("UINT_PTR") rename("ITableDefinition", "_ITableDefinition")
#import <esriCartoUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriGeoDatabaseUI.olb> raw_interfaces_only no_namespace named_guids rename("ICursor","_ICursor") rename("VersionManager","ESRIVersionManager")
#import <esriCatalog.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import <esriArcMapUI.olb> raw_interfaces_only raw_native_types no_namespace named_guids function_mapping rename("ICommand","_ICommand") exclude("IMxApplication2","IDataConnectionPropertyPage2","IDataGraphWizard")
#import <esriEditor.olb> raw_interfaces_only raw_native_types no_namespace named_guids
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
#include <iostream>
#include <list>
#include <set>
#include <vector>

#include <atlbase.h>
extern	CComModule _Module;
#include <atlcom.h>

using namespace std;

#include "coord_converter.h"
#include "sindy/libschema.h"
#include "libshapewrap.h"
#include "shapelib/libshape.h"

#include <boost/foreach.hpp>
#include <boost/version.hpp>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__1091F74C_B997_4E01_AF17_18BE4ECC7F7B__INCLUDED_)

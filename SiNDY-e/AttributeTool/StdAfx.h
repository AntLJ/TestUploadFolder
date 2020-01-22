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

#if !defined(AFX_STDAFX_H__281A2147_D94F_4A5A_9999_02D9ECCC00F7__INCLUDED_)
#define AFX_STDAFX_H__281A2147_D94F_4A5A_9999_02D9ECCC00F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
// CComModule クラスから派生したクラスを使用して、オーバーライドする場合
// _Module の名前は変更しないでください。
extern CComModule _Module;
#include <atlcom.h>

// FOR ARCOBJECTS
// Disable a warning when importing the type library, but only for this one import
#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192)
#pragma warning(disable : 4278)

#include "WinLib/libwinlib.h"
#include "WinLib/arcobjects_import_rawmethod.h"

#import <tlb/sindyeCore.tlb>		raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ISet", "_ISet")
#import <tlb/Renderer.tlb>			raw_interfaces_only,raw_native_types,no_namespace,named_guids,exclude("OLE_HANDLE", "OLE_COLOR") 

#pragma warning(pop)
extern "C" const GUID __declspec(selectany) CLSID_AttributeDlgObj = {0x449732FF,0x4171,0x4F3C,{0xA8,0x19,0xBC,0x01,0x72,0x30,0xC6,0x4D}};
extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj = {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
#include "../../Lib/SiNDYLib/SiNDYLib.h"

#include <list>
#include <map>
#include <vector>
#include <atlstr.h>
#include "crd_cnv/coord_converter.h"
#include <ArcHelperEx/ArcHelperEx.h>
#include <atlctl.h>
#include <CatIDs/ArcCATIDs.h>
#include <SiNDYCATIDs.h>
#include <HelpID.h>

const static LPCTSTR REGOPENKEY	= (_T("%s%s%sAttributeTool"));

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__281A2147_D94F_4A5A_9999_02D9ECCC00F7__INCLUDED)

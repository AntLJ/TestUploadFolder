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
#include <atlconv.h>
#include <atlwin.h>

#pragma warning(push)
#pragma warning(disable : 4146) /* Ignore warnings for use of minus on unsigned types */
#pragma warning(disable : 4192) /* Ignore warnings for types that are duplicated in win32 header files */
#pragma warning(disable : 4278) /* Ignore warnings for use of minus on unsigned types */ 
#import <esriSystem.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids exclude("OLE_HANDLE","OLE_COLOR") rename("XMLSerializer","_XMLSerializer")
#import <esriSystemUI.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IProgressDialog","_IProgressDialog") rename("ICommand","_ICommand")
#import <esriGeometry.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment")
#import <esriDisplay.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("ISegment","_ISegment")
#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix("") no_namespace named_guids rename("IRow","_IRow") rename("ICursor","_ICursor") rename("IRelationship", "_IRelationship")
#import <esriDataSourcesGDB.olb> high_method_prefix("_") raw_method_prefix("") raw_native_types no_namespace named_guids exclude("IDataServerManagerAdmin")
#import "c:\Program Files\Common Files\system\ado\msadox.dll"
#import "c:\Program Files\Common Files\system\ado\msado60.tlb" no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter") rename("Record","ADORecord")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" high_method_prefix("_") raw_method_prefix("") no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")
#pragma warning(pop)

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <atlstr.h>
#include <map>
#include <set>
#include <string>

#include "GlobalFunc.h"

#include <libschema.h>
#include <libcommoncheck.h>
#include <libwinlib.h>

#pragma comment(lib, "../../LIB/oo4o/lib/ORACLM32.LIB")

#define CON_STR_SIZE		100		///< 接続文字列のサイズ

#define ALL_KANJI_MAX_LEN	93		///< 漢字名称（結合）の最大長
#define KEN_KANJI_MAX_LEN	9		///< 漢字名称（都道府県）の最大長
#define SHI_KANJI_MAX_LEN	25		///< 漢字名称（市区町村）の最大長
#define OAZA_KANJI_MAX_LEN	37		///< 漢字名称（大字）の最大長
#define AZA_KANJI_MAX_LEN	25		///< 漢字名称（字・丁目）の最大長

#define ALL_KANA_MAX_LEN	185		///< 全角カナ名称（結合）の最大長
#define KEN_KANA_MAX_LEN	17		///< 全角カナ名称（都道府県）の最大長
#define SHI_KANA_MAX_LEN	49		///< 全角カナ名称（市区町村）の最大長
#define OAZA_KANA_MAX_LEN	73		///< 全角カナ名称（大字）の最大長
#define AZA_KANA_MAX_LEN	49		///< 全角カナ名称（字・丁目）の最大長

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

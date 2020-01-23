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

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600	// これを IE. の他のバージョン向けに適切な値に変更してください。
#endif

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。

#include <stdexcept>
#include <atlbase.h>

#import <esriSystem.olb> raw_interfaces_only raw_native_types no_namespace exclude("OLE_COLOR", "OLE_HANDLE")
#import <esriGeometry.olb> raw_interfaces_only raw_native_types no_namespace
#import <esriDisplay.olb> raw_interfaces_only raw_native_types no_namespace
#import <esriGeoDatabase.olb> high_method_prefix("_") raw_method_prefix("") no_namespace
#import <esriDataSourcesGDB.olb> raw_interfaces_only raw_native_types no_namespace

#pragma warning(disable : 6334)

#include <CodeAnalysis/SourceAnnotations.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

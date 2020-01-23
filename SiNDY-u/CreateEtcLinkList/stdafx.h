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

#include <atlbase.h>
#import <esriSystem.olb>		high_method_prefix("_"), raw_method_prefix(""), no_namespace, named_guids, exclude("OLE_HANDLE", "OLE_COLOR")
#import <esriGeometry.olb>		high_method_prefix("_"), raw_method_prefix(""), no_namespace, named_guids, rename("ISegment", "_ISegment")
#import <esriDisplay.olb>		raw_interfaces_only, raw_native_types, no_namespace
#import <esriGeoDatabase.olb>	high_method_prefix("_"), raw_method_prefix(""), no_namespace, named_guids, rename("IRow", "_IRow"), rename("ICursor", "_ICursor")
#import "libid:6FCCEDE0-179D-4D12-B586-58C88D26CA78" raw_interfaces_only no_namespace no_implementation rename("esriProductCode","esriVersionProductCode")

#include "crd_cnv/coord_converter.h"
#include "sindy/libschema.h"

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

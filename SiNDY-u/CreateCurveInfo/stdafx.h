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

// TODO: 警告を全て明示的に抑制

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlstr.h>		// CStringのため

#include <WinLib/arcobjects_import_highmethod.h>
#import "tlb/sindyeCore.tlb"	raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE", "ISiNDYSnapEnvironment") rename("IRow", "_IRow") rename("ICursor", "_ICursor")

#include <vector>
#include <set>
#include <deque>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
//#pragma warning(push)
#pragma warning(disable:4819)	// warning C4819: ファイルは、現在のコード ページ (932) で表示できない文字を含んでいます。データの損失を防ぐために、ファイルを Unicode 形式で保存してください。
#include <boost/lexical_cast.hpp>
//#pragma warning(pop)
#include <boost/assign.hpp>
#include <boost/utility.hpp>	// for prior/next

#include <ArcHelperEx/libarchelper.h>
#include <ArcHelperEx/ArcHelperEx.h>
#include <crd_cnv/coord_converter.h>
#include <sindy/libschema.h>
#include <RNSlibs/RNS/librns.h>
#include <RNSlibs/RNSShape/librnsshape.h>
#ifndef _USE_SHAPELIB_STATIC
#define _USE_SHAPELIB_STATIC // これがないとshapelibのリンクエラーになる
#endif
#include <shapelib/libshape.h>
#include <adlib.h>
#include <RNSlibs/RNS/RNSPlant.h>	// NONZERO()のため
//#include <RNSlibs/RNS/Type.h>
//typedef adlib::MeshCode MeshCode;
#define _USE_SINDYLIB_STATIC	// sindylib_baseをスタティックリンクするために必要
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#include <WinLib/libwinlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

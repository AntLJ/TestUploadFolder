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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#include <atlbase.h>
#include <atlstr.h>

#include "arcobjects_import_rawmethod.h"
#include "sindy/libschema.h"

#include <map>
#include <list>
#include <set>

#include <iostream>
#include <fstream>

#include <time.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/directory_util.h>
#include <boost/format.hpp>
#include <WinLib/CStringTokenizer.h>
#include "ErrorMsg.h"

#define _USE_SHAPELIB_STATIC
#include "libshape.h"
#include "DBFHandle.h"
#include "SHPHandle.h"

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// プログラム引数
static char* const g_strTaregetDB = "target_db";
static char* const g_strLayer = "layer";
static char* const g_strMeshDB = "mesh_db";
static char* const g_strMeshSchema = "mesh_schema";
static char* const g_strMeshlist = "meshlist";
static char* const g_strOutDir = "outdir";
static char* const g_strIdentifier = "identifier";
static char* const g_strOutFileName = "filename";
static char* const g_strColumnlist = "fieldlist";
static char* const g_strNoShape = "no_shape";
static char* const g_strWhereQuery = "where_query";
static char* const g_strWhereLoop = "where_loop";
static char* const g_Runlog = "run_log";
static char* const g_Errlog = "err_log";

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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#include <atlbase.h>
#include <atlstr.h>

// 追加ヘッダー
#include <exception>
#include <WinLib/arcobjects_import_rawmethod.h>
#include <WinLib/VersionInfo.h>
#include <WinLib/CStringTokenizer.h>

#include <arctl/coinitializer.h>
#include <atl2/io.h>

#include <fstream>
#include <chrono>

#include <shlwapi.h>
#include <direct.h>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/boost_program_options.h>



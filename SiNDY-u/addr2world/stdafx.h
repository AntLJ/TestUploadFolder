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


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。

#include <atlbase.h>
#include <atlstr.h>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <windows.h>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include "TSVFileReader.h"
#include "FileFinder.h"
#include <crd_cnv/crd_cnv.h>
#include <time.h>


#ifdef DEBUG
#pragma comment(lib, "../../LIB/crd_cnv/lib/crd_cnv_vc11-mud.lib")
#else
#pragma comment(lib, "../../LIB/crd_cnv/lib/crd_cnv_vc11-mu.lib")
#endif // DEBUG

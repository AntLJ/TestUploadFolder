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
#include <iostream>
#include <fstream>
#include <iomanip>
#include <atlbase.h>
#include <atlstr.h>
#include <atlconv.h>
#include <string>
#include <set>
#include <map>
#include <vector>

#include <WinLib/arcobjects_import_rawmethod.h>
#include <sindy/libschema.h>
#include <sindy/schema.h>
#include <ArcHelperEx/libarchelper.h>
#define _USE_SINDYLIB_STATIC
#include <TDC/sindylib_core/libsindylibcore.h>
#include <WinLib/libwinlib.h>

#import "C:\Program Files\Common Files\System\ado\msado60.tlb" rename("EOF", "adoEOF")

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

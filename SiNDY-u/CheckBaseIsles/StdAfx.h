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

// SDKDDKVer.h をインクルードすると、利用できる最も上位の Windows プラットフォームが定義されます。

// 以前の Windows プラットフォーム用にアプリケーションをビルドする場合は、WinSDKVer.h をインクルードし、
// SDKDDKVer.h をインクルードする前に、サポート対象とするプラットフォームを示すように _WIN32_WINNT マクロを設定します。

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0501 // XP
#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。

#include <atlbase.h>
#include <atlstr.h>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <float.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <list>
#include "../OutputErrLog/OutputErrLog.h"
#include "../SDEAccessMS/SDEAccessMS.h"
#include "../JDBAccess/JDBAccess.h"
#include "sindy/libschema.h"
#include "crd_cnv/coord_converter.h"
//#include "../JDBAccess/libjdbaccess.h"
#include "../OutputErrLog/liboutputerrlog.h"
#include "../SDEAccessMS/libsdeaccessms.h"
//#include "../XSVHandle/libxsvhandle.h"

using namespace std;

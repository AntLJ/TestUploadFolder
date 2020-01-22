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
#pragma warning (push)
#pragma warning (pop)

#define _USE_SINDYLIB_STATIC
#define SJIS_CREATE     L"w+"
#define UTF8_CREATE     L"w+,ccs=UTF-8"
#define SJIS_APPEND     L"a"
#define UTF8_APPEND     L"a,ccs=UTF-8"
#define UTF8_READ       L"r,ccs=UTF-8"
#define CITY_TXT        L"city.txt"
#define CONV_MODE_CITY  L"city"
#define CONV_MODE_ADDR  L"addr"
#define CONV_MODE_ALL   L"all"

#include <atlbase.h>
#include <iostream>
#include "TDC/useful_headers/tstring.h"
#include "TDC/useful_headers/str_util.h"
#include "FileController.h"
#include "Argument.h"
#include "Message.h"

using namespace std;
using namespace message;
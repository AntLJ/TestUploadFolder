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
#include <atlstr.h>

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <set>
#include <map>
#include <fstream>
#include <vector>
#include <libzcl.h>
#include <libzin.h>

#define IN_VERSION		590

// 仕向け先
enum Customer
{
	PEC		= 0,		//!< PEC,TR
	OTHER	= 1,		//!< BE,KIWI-C,OTHERS
};

using namespace std;

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

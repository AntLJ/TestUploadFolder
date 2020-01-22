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

// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__7E30377F_6324_4BDF_B919_96F4198A0F6A__INCLUDED_)
#define AFX_STDAFX_H__7E30377F_6324_4BDF_B919_96F4198A0F6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1
#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN // Windows ヘッダーから殆ど使用されないスタッフを除外します
#define _SCL_SECURE_NO_WARNINGS
#include "atlbase.h"

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#include <WinLib/arcobjects_import_rawmethod.h>
#pragma warning(pop)

#include <stdio.h>

// TODO: プログラムで必要なヘッダー参照を追加してください。
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <set>
#include "atlstr.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__7E30377F_6324_4BDF_B919_96F4198A0F6A__INCLUDED_)

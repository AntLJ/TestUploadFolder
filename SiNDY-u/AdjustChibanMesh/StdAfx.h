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

#if !defined(AFX_STDAFX_H__499EFAE8_38B5_48A3_AC35_9AFB3A2A107D__INCLUDED_)
#define AFX_STDAFX_H__499EFAE8_38B5_48A3_AC35_9AFB3A2A107D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define _CRT_SECURE_NO_WARNINGS
#include "atlbase.h"

#define WIN32_LEAN_AND_MEAN // Windows ヘッダーから殆ど使用されないスタッフを除外します

#include <stdio.h>

// TODO: プログラムで必要なヘッダー参照を追加してください。
#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#include <WinLib/arcobjects_import_rawmethod.h>
#pragma warning(pop)
#include <conio.h>
#include <time.h>
#include <set>
#include <list>
#include <map>
#include "atlstr.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__499EFAE8_38B5_48A3_AC35_9AFB3A2A107D__INCLUDED_)

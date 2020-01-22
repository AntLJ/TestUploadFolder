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

#if !defined(AFX_STDAFX_H__BEDA8D00_F416_45F1_B132_05E69F3A92A5__INCLUDED_)
#define AFX_STDAFX_H__BEDA8D00_F416_45F1_B132_05E69F3A92A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C標準の危険な関数の警告を出さない
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996の警告を出さない
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATEや_CRT_SECURE_NO_DEPRECATEでC4996が回避されない！！

#include "atlbase.h"
#include <atlstr.h>

#include <WinLib/arcobjects_import_rawmethod.h>

// TODO: プログラムで必要なヘッダー参照を追加してください。
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__BEDA8D00_F416_45F1_B132_05E69F3A92A5__INCLUDED_)

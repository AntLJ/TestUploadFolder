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

#if !defined(AFX_STDAFX_H__F32FC2F0_04CB_41D5_A884_F5D3C834E3A8__INCLUDED_)
#define AFX_STDAFX_H__F32FC2F0_04CB_41D5_A884_F5D3C834E3A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// TODO: プログラムで必要なヘッダー参照を追加してください。
#include <atlbase.h>
#include <comdef.h>
#include <windows.h>
#include <Str.h>
#include "../../../LIB/SiNDYLib/include/CStringTokenizer.h"

#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <io.h>
#include <direct.h>
#include <boost/lexical_cast.hpp>

// shapelib関連

#include "../../../LIB/shapelib/SHPHandle.h"
#include "../../../LIB/shapelib/DBFHandle.h"

//#define	_USE_SHAPELIB_STATIC	// staticライブラリを使用
#ifdef	_DEBUG
#pragma comment(lib, "../../../LIB/shapelib/lib/shapelibmd.lib")
#else
#pragma comment(lib, "../../../LIB/shapelib/lib/shapelibm.lib")
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__F32FC2F0_04CB_41D5_A884_F5D3C834E3A8__INCLUDED_)

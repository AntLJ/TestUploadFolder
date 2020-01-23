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

#if !defined(AFX_STDAFX_H__3816B477_7AAD_461A_BB16_9989EA3DC3B0__INCLUDED_)
#define AFX_STDAFX_H__3816B477_7AAD_461A_BB16_9989EA3DC3B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// TODO: プログラムで必要なヘッダー参照を追加してください。
#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから使用されていない部分を除外します。

#include <atlbase.h>
#include <atlstr.h>

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#include "WinLib/arcobjects_import_rawmethod.h"
#pragma warning(pop)


// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#include <iostream>
#include <fstream>
#include <string>
#include <map>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__3816B477_7AAD_461A_BB16_9989EA3DC3B0__INCLUDED_)

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

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#if _MSC_VER < 1700
#include "memdbg.h" // メモリリーク検出
#endif // 


//#include "resource.h"
////////////////////////////
// ATL サポート
////////////////////////////
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>

using namespace ATL;

#include "arcobjects.h" // ArcObjects Type Library インポート

#if _MSC_VER >= 1700
#include "memdbg.h" // メモリリーク検出
#endif // 
////////////////////////////
////////////////////////////
////////////////////////////
#include <list>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <math.h>
#include "sindy/schema.h"
#include "CatIDs/ArcCATIDs.h"
#include "crd_cnv.h"
#include "macro_util.h"
#include "winutil.h"
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <TDC/useful_headers/tstring.h>

#pragma warning(disable: 4503)	// 限界超えたっていいじゃん
#pragma warning(disable: 4786)	// 切り捨てられたっていいじゃん
#pragma warning(disable: 4290)	// 例外指定くらい、無視されても良い…んじゃ？


// VARIANT型同士の比較の際、「ー(長音)」と「―(全角ダッシュ)」を同じ文字と見なしてしまうため、
// 正しく判定できるよう上書きしておく(bug5775, 11947)
// 必要であれば、ツール本体のstdafx.hの最後に、下のoperatorの宣言をコピペしておくこと
bool operator==(const CComVariant& va1, const CComVariant& va2);
bool operator!=(const CComVariant& va1, const CComVariant& va2);
bool operator<(const CComVariant& va1, const CComVariant& va2);
bool operator>(const CComVariant& va1, const CComVariant& va2);

inline bool operator==(const CComVariant& va1, const CComVariant& va2)
{
	if(VT_BSTR == va1.vt && VT_BSTR == va2.vt)
		return (0 == wcscmp(va1.bstrVal, va2.bstrVal));
	return va1.operator==(va2);
}

inline bool operator!=(const CComVariant& va1, const CComVariant& va2)
{
	return !operator==(va1, va2);
}

inline bool operator<(const CComVariant& va1, const CComVariant& va2)
{
	if(VT_BSTR == va1.vt && VT_BSTR == va2.vt)
		return (0 > wcscmp(va1.bstrVal, va2.bstrVal));
	return va1.operator<(va2);
}

inline bool operator>(const CComVariant& va1, const CComVariant& va2)
{
	return (va2 < va1);
}

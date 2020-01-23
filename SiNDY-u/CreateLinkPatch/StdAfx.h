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

/**
 *	@file StdAfx.h
 *	@brief 標準システムインクルードファイル 定義部
 *	@author	F.Adachi
 *	@date	2005/10/18		新規作成
 *	$ID$
 */

// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__6780A134_E1B0_4A91_B314_862E128BDD51__INCLUDED_)
#define AFX_STDAFX_H__6780A134_E1B0_4A91_B314_862E128BDD51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>			// CComBSTR, _T

#pragma warning(push)
#pragma warning(disable : 4146)		// VC++のバグ対策
#pragma warning(disable : 4192)		// exclude属性を指定すべき？

#include "WinLib\arcobjects_import_highmethod.h"
#include "WinLib\arcobjects_import_rawmethod.h"

#pragma warning(pop)

#include <atlstr.h>

// TODO: プログラムで必要なヘッダー参照を追加してください。
#include <ostream>
#include <iostream>
#include <boost/utility.hpp>		// boost::noncopyable

#include "ZCL/libzcl.h"
#include "crd_cnv/coord_converter.h"
#include "WKH/libwkh.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__6780A134_E1B0_4A91_B314_862E128BDD51__INCLUDED_)

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

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#import "progid:Excel.Sheet" auto_search auto_rename rename_search_namespace("Office")
#include <WinLib\arcobjects_import_highmethod.h>

#pragma warning( disable : 4503 ) //!< 「装飾された名前の長さが限界を越えました。名前は切り捨てられます。」

#include <atlbase.h>
#include <atlstr.h>
#include <atlsafe.h>

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <sindy/libschema.h>
#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>

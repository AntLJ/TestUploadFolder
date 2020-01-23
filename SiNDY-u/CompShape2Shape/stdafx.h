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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "atlbase.h"
#include <atlstr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <boost/lexical_cast.hpp>
#include "arcobjects_import_rawmethod.h"
#include "VersionInfo.h"

#include "coord_converter.h"
#define _USE_SHAPELIB_STATIC
#include "libshape.h"

#include<SHPHandle.h>
#include<DBFHandle.h>
#include<crd_cnv.h>

#pragma warning(push)
#pragma warning(disable : 4192)
#pragma warning(disable : 4146)
#pragma warning(pop)

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



// TODO: プログラムに必要な追加ヘッダーをここで参照してください。
#define		ON_OBJECTID_MODE	0x01
#define		ON_FID_MODE			0x10
#define		ON_ROAD_MODE		0x100
#define		ON_MAP_MODE			0x200
#define		ON_FALL_MODE		0x1000
#define		ON_FSPECIFY_MODE	0x2000
#define		ON_EXE_NORMAL_MODE	0x4000
#define		ON_EXE_BINARY_MODE	0x8000

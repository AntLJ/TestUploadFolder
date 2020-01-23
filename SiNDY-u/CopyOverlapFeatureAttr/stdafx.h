// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlstr.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/tformat.h>
#include <vector>
#include <assert.h>

#include <iostream>
#include <fstream>

#include <WinLib/libwinlib.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>

#define _USE_SINDYLIB_STATIC	// sindylib_baseをスタティックリンクするために必要
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>

#include "FeatureObjects.h"

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
#include <vector>
#include <set>
#include <map>
#include <list>
#include <assert.h>
#include <sindy/schema.h>

#include <iostream>
#include <fstream>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#include <sindy/libschema.h>
#include <WinLib/libwinlib.h> // ArcHeplerExを使用するためのinclude
#include <crd_cnv/coord_converter.h> // ArcHeplerExを使用するためのinclude
#include <ArcHelperEx/libarchelper.h>
#pragma warning(pop)

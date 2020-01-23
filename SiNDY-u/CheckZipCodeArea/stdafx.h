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
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <assert.h>

#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#import "tlb/sindyeCore.tlb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#pragma warning(pop)

// リンクに必要なヘッダ
#define _USE_SINDYLIB_STATIC
#include <sindy/libschema.h>
#include <WinLib/libwinlib.h>
#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#include <AddrLib/AddrLib.h>

// チェックに必要なヘッダ
#include <AddrLib/AddrRec.h>
#include <AddrLib/AddrRecHelper.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <TDC/sindylib_core/meshutil.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/RepPointCalculator/libRepPointCalculator.h>

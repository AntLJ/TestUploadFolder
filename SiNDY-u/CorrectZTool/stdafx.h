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

#include <atlbase.h>
#include <atlstr.h>
#include <TDC/useful_headers/tstring.h>
#include <boost/next_prior.hpp>
#include <TDC/useful_headers/enumerate_elements.h>
#include <vector>
#include <assert.h>

#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

#include <list>
#include <set>
#include <map>
#include <sindy/libschema.h>
#include <sindy/schema/autodrive.h>
#include <sindy/workspace.h>
#define _USE_SINDYLIB_STATIC	// sindylib_baseをスタティックリンクするために必要
#include <TDC/sindylib_core/libsindylibcore.h>
#include <TDC/sindylib_base/libsindylibbase.h>
#include <arctl/coinitializer.h>
#include <atl2/io.h>
#include <crd_cnv/coord_converter.h>
#include <WinLib/libwinlib.h>
#include <ArcHelperEx/libarchelper.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/define.h>
#include <ArcHelperEx/AheMeshUtil.h>
#include <ArcHelperEx/AheGlobals.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <TDC/adamlib/libadamlib.h>
#include <TDC/adamlib/AdamGlobals.h>
#include <TDC/adamlib/AdamZValueAdjuster.h>
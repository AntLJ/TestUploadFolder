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
#include <assert.h>
#include <atlstr.h>
#include <atlcom.h>
#include <atltime.h>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <sstream>
#include <iostream>
#include <fstream>
#include <mutex>

#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_rawmethod.h>
#pragma warning(pop)

using namespace ATL;

#include <crd_cnv/coord_converter.h>
#include <sindy/libschema.h>
#include <WinLib/libwinlib.h>
#include <ArcHelperEx/libarchelper.h>

#include "resource.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/assign.hpp>

namespace boost
{
namespace property_tree
{
	typedef string_path<uh::tstring, id_translator<uh::tstring> > tpath;
	typedef basic_ptree<uh::tstring, uh::tstring> tptree;
	typedef basic_ptree<uh::tstring, uh::tstring, detail::less_nocase<uh::tstring> > tiptree;
} // property_tree
} // boost

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstringstream tstringstream;
#else
typedef std::stringstream tstringstream;
#endif

#include "Utility.h"
#include "Logger.h"

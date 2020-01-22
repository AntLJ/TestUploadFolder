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
#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlapp.h>

#define _ATL_TMP_NO_CSTRING
#include <atlmisc.h>
#include <atlctrls.h>
#include <atlstr.h>
#include <atlcrack.h>

#include <vector>
#include <list>
#include <map>
#include <set>


#pragma warning(push)
#  pragma warning(disable : 4146)
#  include <WinLib/arcobjects_import_rawmethod.h>
#  import <tlb/sindyeCore.tlb>		raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE", "sindyeDispNaviRelation", "sindyeDispOtherRelation", "sindyeEditType", "sindyeEditableType", "sindyeWorkKind") rename("IRow", "_IRow")
#  import <VSFLEXGRID/vsflex8n.ocx>	no_namespace, named_guids
#pragma warning(pop)

extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj        = {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
extern "C" const GUID __declspec(selectany) CLSID_SiNDYModifyCheckObj = {0x67dc3e12,0x061b,0x4ca8,{0xa8,0x29,0x40,0xc6,0x8d,0x7d,0x17,0x7a}};


#include <ArcHelperEx/ArcHelperEx.h>
#include <AttributeDlg/AttrDlgLib/AttrDlgLib.h>
#include <WinLib/libwinlib.h>
#include <WinLib/grid_util.h>
#include <crd_cnv/coord_converter.h>

#define _USE_CHILDDLG_SHORTCUT

static const LPCTSTR kREGOPENKEY   = _T("%s%s%sAttributeTool");
static const LPCTSTR kREGCOMMONKEY = _T("%s%s%sCOMMON");

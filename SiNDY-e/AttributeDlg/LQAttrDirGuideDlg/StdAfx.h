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

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B

#pragma once

#include "WinLib/libwinlib.h"
#include <atlbase.h>
#include "resource.h"
#include <atlcom.h>
#include <atlwin.h>
#define _ATL_TMP_NO_CSTRING     // ATL��WTL��CString�����Ԃ邽��
#include <atlapp.h>             // WTL���g�p
#include <atlmisc.h>            // CPoint CSize CRect
#include <atlctrls.h>
#include <atlstr.h>             // CString
#include <atlcrack.h>           // ���b�Z�[�W�n

using namespace ATL;

// FOR ARCOBJECTS
// Disable a warning when importing the type library, but only for this one import
#pragma warning(push)
#pragma warning(disable : 4146)
#include "WinLib/arcobjects_import_rawmethod.h"
#import <tlb/sindyeCore.tlb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#import <VSFLEXGRID/VSFLEX8N.OCX>			no_namespace, named_guids  

#pragma warning(pop)
extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj = {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
extern "C" const GUID __declspec(selectany) CLSID_SiNDYModifyCheckObj = {0x67dc3e12,0x061b,0x4ca8,{0xa8,0x29,0x40,0xc6,0x8d,0x7d,0x17,0x7a}};

#include <map>
#include <list>
#include <vector>
#include <functional>

#ifdef _LQATTRIBUTE
#define __SINDY_ROADQUEUEMODE__
#endif

#include "crd_cnv/coord_converter.h"
#include "ArcHelperEx.h"
#include "AttrDlgLib.h"

const static LPCTSTR REGOPENKEY	= (_T("%s%s%sAttributeTool"));
const static LPCTSTR REGCOMMONOPENKEY	= (_T("%s%s%sCOMMON"));

#ifdef _DEBUG
#include <mmsystem.h>
#endif

#define _USE_CHILDDLG_SHORTCUT	// [bug 4079] �q�_�C�A���O�̃V���[�g�J�b�g���T�|�[�g
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#define foreach BOOST_FOREACH

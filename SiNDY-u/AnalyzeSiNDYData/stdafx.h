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

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

#pragma once

#ifndef STRICT
#define STRICT
#endif
#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B
#define _ATL_TMP_NO_CSTRING     // ATL��WTL��CString�����Ԃ邽��

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
// atlctrls.h����ɓǂݍ��ނ��ƂŃR���g���[���N���X��CString�T�|�[�g
// ���̂ق��Aatlapp.h�ɂ�CString��ifdef������̂ŁAWTL������ɓǂݍ��ޕK�v������
#include <atlstr.h>
#include <atlapp.h>             // WTL���g�p
#include <atlctl.h>
#include <atldlgs.h>
#define _ATL_TMP_NO_CSTRING     // ATL��WTL��CString�����Ԃ邽��
#include <atlmisc.h>            // CPoint CSize CRect
#include <atlctrls.h>
#include <atlgdi.h>				// CDC                 �iWinMgr�Ŏg�p�j
#include <atlcrack.h>			// ���b�Z�[�W�n
#include <shellapi.h>
#include <atlctrlx.h>

using namespace ATL;


#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192) 
#include <WinLib/arcobjects_import_rawmethod.h>
#import "tlb/sindyeCore.tlb"			raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#import "VSFLEXGRID/VSFLEX8N.OCX"		no_namespace named_guids 
#import "tlb/msado15.dll"				no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")

#pragma warning(pop)

#include <list>
#include <map>
#include <vector>
#include <fstream>

#include "../SiNDY-e/COMMON/HelpID.h"
#include "../SiNDY-e/COMMON/SiNDYCATIDs.h"
#include "../SiNDY-e/COMMON/SiNDYCLSIDs.h"
#include <sindy/libschema.h>
#include "CatIDs/ArcCATIDs.h"
#include "ArcHelperEx.h"
#define _USE_SINDYLIB_STATIC
#include "sindylib.h"
#include "macro_util.h"
#include "winutil.h"

#include "coord_converter.h"

#include <arctl/coinitializer.h>

using namespace std;

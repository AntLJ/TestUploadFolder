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
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <atlstr.h>
#include <atlcom.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/str_util.h>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

using namespace ATL;

#define _USE_SINDYLIB_STATIC
#include <TDC/sindylib_core/libsindylibcore.h>
#include <TDC/sindylib_base/libsindylibbase.h>
#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>
#include <WinLib/libwinlib.h>
#include <sindy/libschema.h>
#import <tlb/sindyeCore.tlb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")

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

#include <atlbase.h>
#include <atlstr.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/str_util.h>
#include "TDC/useful_headers/variant_util.h"
#include <ArcHelperEx/libarchelper.h>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <filesystem>
#include <assert.h>
#include <coord_converter.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

#define _USE_SINDYLIB_STATIC
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#include <WinLib/libwinlib.h>
#import <tlb/sindyeCore.tlb> raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
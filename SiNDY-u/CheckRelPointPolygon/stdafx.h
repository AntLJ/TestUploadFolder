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
#include <stdlib.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlstr.h>
#include <assert.h>

#include <TDC/useful_headers/tstring.h>
#include <vector>
#include <list>
#include <map>
#include <string>

#include <assert.h>
#include <iostream>
#include <fstream>

#define _USE_SINDYLIB_STATIC
#include <libarchelper.h>
#include <coord_converter.h>
#include <libwinlib.h>
#include <libschema.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#import "tlb/sindyeCore.tlb"						raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")exclude("sindyeDispNaviRelation", "sindyeDispOtherRelation", "sindyeEditType", "sindyeEditableType", "sindyeWorkKind") rename("IRow", "_IRow") rename("ICursor", "_ICursor")
#pragma warning(pop)
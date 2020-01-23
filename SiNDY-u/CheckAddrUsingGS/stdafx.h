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

#include <locale.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <set>
#include <map>
#include <list>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �ꕔ�� CString �R���X�g���N�^�[�͖����I�ł��B

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <WinLib/arcobjects_import_rawmethod.h>
#import <tlb/sindyeCore.tlb>		raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#include <WinLib/libwinlib.h>
#include <SiNDYLib/SiNDYLib.h>
#include <sindy/libschema.h>
#include <ArcHelperEx/libarchelper.h>

#pragma comment(lib, "Version.lib")
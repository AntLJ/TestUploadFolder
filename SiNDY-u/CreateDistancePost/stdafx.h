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

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�[�͖����I�ł��B
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlstr.h>
#include <TDC/useful_headers/tstring.h>
#include <vector>
#include <assert.h>

#include <WinLib/arcobjects_import_rawmethod.h>

#pragma warning(disable: 4503)	// ���E���������Ă��������
#pragma warning(disable: 4786)	// �؂�̂Ă�ꂽ���Ă��������
#pragma warning(disable: 4290)	// ��O�w�肭�炢�A��������Ă��ǂ��c�񂶂�H
#pragma warning(disable: 4273)
#import <tlb/sindyeCore.tlb>					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ISet", "_ISet")

#include "build/memdbg.h" // ���������[�N���o

#define _USE_SINDYLIB_STATIC
#define CRD_CNV_API
#define __SINDYE_NO_CHECK_NODE_ATTRIBUTE__ 0

#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <crd_cnv/coord_converter.h>

#include <macro_util.h>

#include <sindy/libschema.h>
#include <libsindylibcore.h>
#include <libsindylibbase.h>
#include <libarchelper.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH
#include <boost/filesystem.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/timer.hpp>

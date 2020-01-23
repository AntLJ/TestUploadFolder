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

// TODO: �x����S�Ė����I�ɗ}��

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <atlbase.h>
#include <atlstr.h>		// CString�̂���

#include <WinLib/arcobjects_import_highmethod.h>
#import "tlb/sindyeCore.tlb"	raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE", "ISiNDYSnapEnvironment") rename("IRow", "_IRow") rename("ICursor", "_ICursor")

#include <vector>
#include <set>
#include <deque>
#include <fstream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
//#pragma warning(push)
#pragma warning(disable:4819)	// warning C4819: �t�@�C���́A���݂̃R�[�h �y�[�W (932) �ŕ\���ł��Ȃ��������܂�ł��܂��B�f�[�^�̑�����h�����߂ɁA�t�@�C���� Unicode �`���ŕۑ����Ă��������B
#include <boost/lexical_cast.hpp>
//#pragma warning(pop)
#include <boost/assign.hpp>
#include <boost/utility.hpp>	// for prior/next

#include <ArcHelperEx/libarchelper.h>
#include <ArcHelperEx/ArcHelperEx.h>
#include <crd_cnv/coord_converter.h>
#include <sindy/libschema.h>
#include <RNSlibs/RNS/librns.h>
#include <RNSlibs/RNSShape/librnsshape.h>
#ifndef _USE_SHAPELIB_STATIC
#define _USE_SHAPELIB_STATIC // ���ꂪ�Ȃ���shapelib�̃����N�G���[�ɂȂ�
#endif
#include <shapelib/libshape.h>
#include <adlib.h>
#include <RNSlibs/RNS/RNSPlant.h>	// NONZERO()�̂���
//#include <RNSlibs/RNS/Type.h>
//typedef adlib::MeshCode MeshCode;
#define _USE_SINDYLIB_STATIC	// sindylib_base���X�^�e�B�b�N�����N���邽�߂ɕK�v
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#include <WinLib/libwinlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B

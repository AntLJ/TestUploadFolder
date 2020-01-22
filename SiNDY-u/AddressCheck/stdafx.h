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


#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlpath.h>
#include <arcobjects_import_rawmethod.h>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
#pragma warning (push)

#import <tlb/sindyeCore.tlb>				raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")

#pragma warning (pop)
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <atl2/io.h>

#define _USE_SINDYLIB_STATIC
#include "SiNDYLib.h"
#include <sindy/libschema.h>
#include "../../LIB/crd_cnv/coord_converter.h"
#include "TDC/useful_headers/tstring.h"
#include "TDC/useful_headers/str_util.h"
#include "TDC/useful_headers/variant_util.h"
#include <WinLib/libwinlib.h>
#include <ArcHelperEx/ArcHelperEx.h>

#include "define.h"
#include "resource.h"
#include "Logger.h"
#include "LayerCheckBase.h"

#include "Argument.h"
#include "MeshCodeList.h"
#include "Message.h"

#include <boost/filesystem.hpp>

using namespace std;
using namespace message;
using namespace sindy::schema;
using namespace uh::str_util;
using namespace uh::variant_util;

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
#include <atltime.h>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <math.h>

#include <iostream>
#include <fstream>

#include <sindy/libschema.h>
#include <sindy/libschema_draft.h>

#include <crd_cnv/coord_converter.h>
#define _USE_SINDYLIB_STATIC	// sindylib_core���X�^�e�B�b�N�����N���邽�߂ɕK�v
#include <TDC/sindylib_core/libsindylibcore.h>
#include <ArcHelperEx/libarchelper.h>
#include <WinLib/libwinlib.h>
#include <WinLib/VersionInfo.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

#include <boost/assign.hpp>
#include <TDC/useful_headers/tstring.h>
#include<TDC/useful_headers/str_util.h>
#include <sindy/schema/road.h>
#include <sindy/schema_draft/road_draft.h>

#include "TimeReg.h"
#include "FeatureObjects.h"


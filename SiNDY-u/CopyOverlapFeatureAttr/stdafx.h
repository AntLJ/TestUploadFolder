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
#include <TDC/useful_headers/tformat.h>
#include <vector>
#include <assert.h>

#include <iostream>
#include <fstream>

#include <WinLib/libwinlib.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#pragma warning(pop)

#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>

#define _USE_SINDYLIB_STATIC	// sindylib_base���X�^�e�B�b�N�����N���邽�߂ɕK�v
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>

#include "FeatureObjects.h"

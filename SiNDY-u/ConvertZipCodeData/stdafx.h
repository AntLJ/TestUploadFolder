// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <atlstr.h>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <codecvt>

#include <atlbase.h>
#include <useful_headers/tstring.h>
#include <useful_headers/str_util.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <arcobjects_import_highmethod.h>
#import "tlb/msado15.dll" no_namespace rename("EOF", "EndOfFile") rename("Field", "ADOField") rename("Fields", "ADOFields") rename("Parameter", "ADOParameter")
#pragma warning(pop)

#include <libschema.h>
#include <workspace.h>
#include <libwinlib.h>
#include <ADOBase.h>
#include <coord_converter.h>
#include <libarchelper.h>
#define USE_ARCOBJECTS
#include <libRepPointCalculator.h>
#include <libwkf.h>

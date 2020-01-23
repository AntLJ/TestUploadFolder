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
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <assert.h>

#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4192)
#pragma warning(disable: 4278)
#include <WinLib/arcobjects_import_highmethod.h>
#import "tlb/sindyeCore.tlb" raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow")
#pragma warning(pop)

// �����N�ɕK�v�ȃw�b�_
#define _USE_SINDYLIB_STATIC
#include <sindy/libschema.h>
#include <WinLib/libwinlib.h>
#include <crd_cnv/coord_converter.h>
#include <ArcHelperEx/libarchelper.h>
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#include <AddrLib/AddrLib.h>

// �`�F�b�N�ɕK�v�ȃw�b�_
#include <AddrLib/AddrRec.h>
#include <AddrLib/AddrRecHelper.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <TDC/sindylib_core/meshutil.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/RepPointCalculator/libRepPointCalculator.h>

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


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �ꕔ�� CString �R���X�g���N�^�[�͖����I�ł��B

#include <atlbase.h>
#include <atlstr.h>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <list>
#include <vector>

#include <WinLib/CStringTokenizer.h>


#include <SHPHandle.h>
#include <DBFHandle.h>

#include <crd_cnv.h>

#include <coord_converter.h>


#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(disable : 4192) 
#include <WinLib/arcobjects_import_rawmethod.h>
#import"sindyeCore.tlb"					raw_interfaces_only raw_native_types no_namespace named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE") rename("IRow", "_IRow") rename("ICursor", "_ICursor")



#include <arctl/coinitializer.h>

#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <list>
#include <vector>

#include <CatIDs/ArcCATIDs.h>
#include "../../SiNDY-e/COMMON/SiNDYCATIDs.h"
#include "../../SiNDY-e/COMMON/SiNDYCLSIDs.h"

using namespace ATL;

#include "ArcHelperEx.h"
//#ifdef _DEBUG
#define _USE_SINDYLIB_STATIC
//#endif // ifdef _DEBUG
#include "sindylib.h"

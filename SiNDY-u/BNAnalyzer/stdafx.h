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
#include <vector>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <WinLib/arcobjects_import_rawmethod.h>
#include <arctl/coinitializer.h>
#include <PgDatabase.h>
#include <crd_cnv/crd_cnv.h>

#ifdef DEBUG
#pragma comment(lib, "../../LIB/crd_cnv/lib/crd_cnv_vc11-mud.lib")
#else
#pragma comment(lib, "../../LIB/crd_cnv/lib/crd_cnv_vc11-mu.lib")
#endif // DEBUG




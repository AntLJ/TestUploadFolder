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

// SDKDDKVer.h ���C���N���[�h����ƁA���p�ł���ł���ʂ� Windows �v���b�g�t�H�[������`����܂��B

// �ȑO�� Windows �v���b�g�t�H�[���p�ɃA�v���P�[�V�������r���h����ꍇ�́AWinSDKVer.h ���C���N���[�h���A
// SDKDDKVer.h ���C���N���[�h����O�ɁA�T�|�[�g�ΏۂƂ���v���b�g�t�H�[���������悤�� _WIN32_WINNT �}�N����ݒ肵�܂��B

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0501 // XP
#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �ꕔ�� CString �R���X�g���N�^�[�͖����I�ł��B

#include <atlbase.h>
#include <atlstr.h>

// TODO: �v���O�����ɕK�v�Ȓǉ��w�b�_�[�������ŎQ�Ƃ��Ă��������B
#include <float.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <list>
#include <crd_cnv/coord_converter.h>
#include "../OutputErrLog/OutputErrLog.h"
#include "../SDEAccessMS/SDEAccessMS.h"
#include "../JDBAccess/JDBAccess.h"
#include "CityList.h"

#include "crd_cnv/coord_converter.h"
#include "../JDBAccess/libjdbaccess.h"
#include "../OutputErrLog/liboutputerrlog.h"
#include "../SDEAccessMS/libsdeaccessms.h"
#include "../GKWAccess/libgkwaccess.h"

using namespace std;
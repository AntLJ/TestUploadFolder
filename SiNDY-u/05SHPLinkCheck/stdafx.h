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

#ifndef _WIN32_WINNT		// Windows XP �ȍ~�̃o�[�W�����ɌŗL�̋@�\�̎g�p�������܂��B                   
#define _WIN32_WINNT 0x0501	// ����� Windows �̑��̃o�[�W���������ɓK�؂Ȓl�ɕύX���Ă��������B
#endif						






#include <stdio.h>
#include <tchar.h>

#include <iostream>

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include <windows.h>
#include <arcobjects_import_highmethod.h>

#pragma warning(push)
#pragma warning(disable : 4146)
#pragma warning(pop)

#define ESRI_SET_VERSION(prod) \
{\
  HRESULT hr; \
  VARIANT_BOOL vb; \
  IArcGISVersionPtr ipVersion(__uuidof(VersionManager)); \
  if(!SUCCEEDED(hr = ipVersion->LoadVersion(prod, L"", &vb))) \
    fprintf(stderr, "LoadVersion() failed with code 0x%.8x\n", hr); \
  else if(vb != VARIANT_TRUE) \
    fprintf(stderr, "LoadVersion() failed\n"); \
}
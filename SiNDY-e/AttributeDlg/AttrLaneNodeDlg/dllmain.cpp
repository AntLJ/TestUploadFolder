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

// ExportDlg.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ExportDlg.h"
#include "../../LIB/WinLib/crashdump.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CAttrChildDlgModule : public CAtlDllModuleT<CAttrChildDlgModule>, public CAtlWinModule
{
};

CAttrChildDlgModule _AtlModule;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	AtlAxWinInit();
	_tsetlocale(LC_ALL, _T("")); // VS2005 SP1 �ł������ĂȂ��Ȃ�āc

								 // �N���b�V���_���v
	SetUnhandledExceptionFilter(myExceptionHandler);	// ��O�n���h���ݒ�

	return _AtlModule.DllMain(dwReason, lpReserved);
}

// #include "../../LIB/WinLib/crashdump.cpp"

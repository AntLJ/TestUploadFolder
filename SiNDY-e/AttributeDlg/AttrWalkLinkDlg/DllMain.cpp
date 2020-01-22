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

class CAttrChildDlgModule : public CAtlDllModuleT<CAttrChildDlgModule>, public CAtlWinModule
{
};

CAttrChildDlgModule _AtlModule;

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	AtlAxWinInit();
	_tsetlocale ( LC_ALL, _T("") ); // VS2005 SP1 �ł������ĂȂ��Ȃ�āc

    return _AtlModule.DllMain(dwReason, lpReserved);
}

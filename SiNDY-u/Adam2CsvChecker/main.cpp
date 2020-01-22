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


#include "stdafx.h"
#include "CsvCheckerDlg.h"
#include "LogSystem.h"

CComModule _Module;

int APIENTRY WinMain(
	HINSTANCE hInst,
	HINSTANCE /*hPrevInst*/,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	LOG_CREATE();

	auto hr = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hr));

	::DefWindowProc(NULL, 0, 0, 0L);


	hr = _Module.Init(0, hInst);
	ATLASSERT(SUCCEEDED(hr));

	AtlAxWinInit();

	CCsvCheckerDlg dlg;
	dlg.DoModal();

	_Module.Term();
	::CoUninitialize();

	return 0;
}

/**
 * @env
 * - Windows 7 Pro 74bit
 * - Visual Studio 2015 Update3
 * - Boost 1.61.0
 * - WTL 9.1.5321 final
 */

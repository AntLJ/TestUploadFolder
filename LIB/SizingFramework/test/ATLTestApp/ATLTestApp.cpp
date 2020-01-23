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

// ATLTestApp.cpp : WinMain の実装


#include "stdafx.h"
#include "resource.h"
#include "ATLTestApp.h"
#include "ATLTestAppDlg.h"
#include <atlstr.h>

class CATLTestAppModule : public CAtlModuleT< CATLTestAppModule >
{
public:
	int WinMain(int nShowCmd);
};

CATLTestAppModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}


int CATLTestAppModule::WinMain(int nShowCmd)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CATLTestAppDlg dlg;
	dlg.DoModal();

	return 0;//CAtlExeModuleT<CATLTestAppModule>::WinMain(nShowCmd);
}


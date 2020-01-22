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

// AnalyzeSiNDYData.cpp : WinMain ‚ÌŽÀ‘•


#include "stdafx.h"
#include "resource.h"
#include "AnalyzeSiNDYData_i.h"
#include "AnalyzeSiNDYDataDlg.h"


class CAnalyzeSiNDYDataModule : public CAtlExeModuleT< CAnalyzeSiNDYDataModule >
{
public :
	DECLARE_LIBID(LIBID_AnalyzeSiNDYDataLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ANALYZESINDYDATA, "{FD28B08D-0027-46B6-80E6-6964FDF305F0}")
	BEGIN_MSG_MAP(CAnalyzeSiNDYDataModule)
		COMMAND_HANDLER(IDC_COMBO_FEATURECLASS, CBN_SELCHANGE, OnCbnSelchangeComboFeatureclass)
	END_MSG_MAP()
	LRESULT OnCbnSelchangeComboFeatureclass(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

CAnalyzeSiNDYDataModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	const arctl::coinitializer coinit;
	{
		CAnalyzeSiNDYDataDlg dlg;
		dlg.DoModal();
	}

    return _AtlModule.WinMain(nShowCmd);
}


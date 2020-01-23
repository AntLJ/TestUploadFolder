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

// CreatePOITxtForTel.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, _T("Japanese") );
	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;

		try
		{
			CApp cApp;
			if( cApp.Init(argc, argv) )
				cApp.Run();
			else
				nRet = 1;
		}
		catch( std::exception& e )
		{
			wcout << CA2W(e.what()) << endl;
			nRet = 2;
		}
	}
	catch( std::exception& e )
	{
		wcout << CA2W(e.what()) << endl;
		nRet = 2;
	}

	return nRet;
}


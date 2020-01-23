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

// CreateAddrWorkTxt.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"Japanese" );

	if( argc == 1 )
	{
		CApp::Usage();
		return 0;
	}

#ifdef _DEBUG
	_tputenv( L"DB_ADDRESS=SiNDY2010A/SiNDY2010A/SDE.REL_DB-100503M-020A/5151/topaz2" );
	_tputenv( L"FC_TARGET_POINT=klink_point" );
	_tputenv( L"FC_CITY_ADMIN=CITY_ADMIN" );
#endif

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CApp cApp;
		if( !cApp.Init(argc, argv) || !cApp.Run() )
		{
			nRet = 1;
		}
	}
	catch( _com_error& e )
	{
		std::cerr << "#COM Error : " << CT2CA(e.ErrorMessage()) << std::endl;
		nRet = 1;
	}
	catch( std::exception& e )
	{
		std::cerr << "#Fatal Error 例外 : " << e.what() << std::endl;
		nRet = 1;
	}
	catch( ... )
	{
		std::cerr << "#Fatal Error 例外が発生" << std::endl;
		nRet = 1;
	}
	
	if( !nRet )
		std::cout << "正常終了" << std::endl;

	return nRet;
}


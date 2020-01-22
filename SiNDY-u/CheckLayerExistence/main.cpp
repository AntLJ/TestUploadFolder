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
#include "CheckLayerExistence.h"
#include <arctl/coinitializer.h>

int main(int argc, char* argv[])
{
	if( argc == 1 )
	{
		CCheckLayerExistence::printUsage();
		return 1;
	}
	
	int nRet = 0;
//	if( SUCCEEDED(::CoInitialize(0)) )
	arctl::coinitializer coinitializer;
	try
	{
		CCheckLayerExistence* lpApp = new CCheckLayerExistence();
		lpApp->getEnv();
		if( lpApp->init(argc, argv) )
		{
			lpApp->execute();
		}
		else
		{
			lpApp->printUsage();
			nRet = 1;
		}
		delete lpApp;
	}
	catch( _com_error& e )
	{
		std::cerr << "#Fatal Error COMエラー発生 : " << CT2CA(e.ErrorMessage()) << std::endl;
		nRet = 2;
	}
	catch( std::exception& e )
	{
		std::cerr << "#Fatal Error 例外発生 : " << e.what() << std::endl;
		nRet = 3;
	}
	catch( ... )
	{
		std::cerr << "#Fatal Error 例外発生" << std::endl;
		nRet = 4;
	}

	::CoUninitialize();

	if( nRet )
		std::cerr << "異常終了！" << std::endl;

	return 0;
}

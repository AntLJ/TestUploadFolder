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

// AddList2Point.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	if( 1 == argc )
	{
		CApp::printUsage();
		return 0;
	}

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CApp cApp;
		if( !cApp.init(argc, argv) )
			nRet = 1;
		else if( !cApp.execute() )
			nRet = 2;
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		nRet = 3;
	}

	if( !nRet )
		std::cerr << "正常終了しました" << std::endl;

	return nRet;
}

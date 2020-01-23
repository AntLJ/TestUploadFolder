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

// CnvExTPAddrcode.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CnvAddrcode.h"
#include <arctl/coinitializer.h>

int main( int argc, char* argv[] )
{
	if( 1 == argc )
	{
		CCnvAddrcode::printUsage();
		return 0;
	}

	OStartup();

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CCnvAddrcode cApp;
		if( !cApp.init(argc, argv) )
		{
			cApp.printUsage();
			nRet = 2;
		}
		else if( !cApp.execute() )
			nRet = 3;
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		nRet = 1;
	}

	if( !nRet )
		std::cerr << "正常終了" << std::endl;

	OShutdown();

	return 0;
}


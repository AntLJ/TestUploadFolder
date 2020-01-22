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

// AttriSearchInMesh.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "ShowInfo.h"
#include <arctl/coinitializer.h>

int main(int argc, char* argv[])
{
	if( 1 == argc )
	{
		CShowInfo::printUsage();
		return 1;
	}

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CShowInfo cApp;
		if( cApp.init(argc, argv) )
		{
			if( ! cApp.execute() )
			{
				nRet = 1;
			}
		}
		else
		{
			cApp.printUsage();
			nRet = 1;
		}
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		nRet = 2;
	}
	catch( const _com_error& e ) {
		std::cerr << atl2::stringize(e) << std::endl;
		nRet = 2;
	}
	catch( ... ) {
		std::cerr << "例外が発生しました" << std::endl;
		nRet = 2;
	}

	if( nRet == 0 )
		std::cout << "正常終了" << std::endl;
	else
		std::cout << "異常終了" << std::endl;

	return nRet;
}

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

// DBOutput.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>
#include <atl2/io.h>

/**
	@brief	メイン処理
	@retval 0	正常終了
	@retval 1	異常終了(ツールは正常に動作したがエラーあり)
	@retval 2	異常終了(例外)
*/
int _tmain(int argc, _TCHAR* argv[])
{
	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CApp cApp;
		if( cApp.init(argc, argv) )
		{
			if( cApp.execute() )
				nRet = 0;
			else
				nRet = 1;
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
	catch(const _com_error& e)
	{
		std::cerr << atl2::stringize(e) << std::endl;
		nRet = 2;
	}
	catch(...)
	{
		std::cerr << "例外発生" << std::endl;
		nRet = 2;
	}

	if (nRet == 0)
		std::cerr << "正常終了" << std::endl;
	else
		std::cerr << "異常終了" << std::endl;

	return nRet;

}


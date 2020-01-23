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

// CreateClaPolyList.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "CreateList.h"
#include <arctl/coinitializer.h>

int main(int argc, char* argv[])
{
	if( argc == 1 )
	{
		CCreateList::printUsage();
		return 0;
	}

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CCreateList cApp;

		// 初期化
		if( !cApp.init(argc, argv) || !cApp.execute() )
			nRet = 2;
	}
	catch( std::exception& e )
	{
		cerr << e.what() << endl;
		nRet = 1;
	}

	if( nRet == 0 )
		cerr << "正常終了" << endl;
	else
		cerr << "異常終了" << endl;

	return nRet;
}

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

// CharAddrConvert.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>


int _tmain(int argc, _TCHAR* argv[])
{
	if( argc == 1 )
	{
		CApp::PrintUsage();
		return 0;
	}
	
	int nRet = 0;
	try
	{
		arctl::coinitializer init;
		CApp cApp;
		if( cApp.Init(argc, argv) )
			cApp.Run();
		else
			nRet = 1;
	}
	catch( std::exception& e )
	{
		progress_out::Progress( _T("%s"), _T(e.what()) );
		nRet = 2;
	}

	if( !nRet )
		progress_out::Progress( "正常終了" );

	return nRet;
}


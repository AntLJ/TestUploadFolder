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

// chkRoad.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "RoadApplication.h"

bool g_bLatest = false;

void getCommandLine( const int nNum, LPTSTR lpcszToken[] )
{
	// 引数を指定
	for( int i = 3; i < nNum; ++i )
	{
		CString strOp(lpcszToken[i]);
		if( _tcscmp( _T("-l"), strOp ) == 0 || _tcscmp( _T("--latest"), strOp ) == 0 )
			g_bLatest = true;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	long lReturn = 0;
	try
	{
		if( argc < 3 || 4 < argc )
		{
			cerr << "Usage    　　：chkRoad.exe [Config.txt] [Customer] [-l(--latest)]\n";
			cerr << "[Config.txt] ：コンフィグファイルへのパス\n";
			cerr << "[Customer] ：仕向け先（PEC,TF,KIWI-C,BE,OTHERS）\n";
			cerr << "[-l(latest)] ：B版以降のリリースかどうか\n";
			return 1;
		}
		CString aConf = argv[1];
		CString strCustomer = argv[2];
		if( argc > 3 )
			getCommandLine( argc, argv );
		CRoadApplication aRoadApp;

		lReturn = aRoadApp.run( aConf, strCustomer, g_bLatest );
		switch( lReturn )
		{
			case 0:
				cerr << "正常終了致しました。" << endl;
				break;
			case 1:
			case 2:
			default:
				cerr << "異常終了致しました。" << endl;
				break;
			case 3:
				cerr << "チェックでエラーがあります。" << endl;
				break;
		}
	}
	catch( ... )
	{
		cerr << "例外が発生しました。" << endl;
		cerr << "異常終了致しました。" << endl;
		return 2;
	}

	return lReturn;
}




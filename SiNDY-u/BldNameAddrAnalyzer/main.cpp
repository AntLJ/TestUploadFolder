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
#include "Arguments.h"
#include "Logger.h"
#include "BldNameAddrAnalyzer.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	// 続行するには何かキーを押してください
	uh::scope_guard sg( []{system( "pause" );} );
#endif

	try
	{
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		// コマンドライン引数処理
		Arguments args;
		if(! args.parse(argc, argv))
		{
			cerr << "コマンドライン引数に問題があります" << endl;
			return 1;
		}

		// オプション表示
		RUNLOGGER.Initialize( args.getValue( option::kRunLog ).GetString() );
		RUNLOGGER.Log( args.description(), true );

		// 実処理
		BldNameAddrAnalyzer app( args );
		try{
			app.execute();
			RUNLOGGER.Log( _T("\n正常終了"), true );
		}
		catch( const std::runtime_error& e )
		{
			RUNLOGGER.Fatal( uh::toTStr(e.what()), true );
			RUNLOGGER.Log( _T("\n異常終了"), true );
			return 1;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 2;
	}

	return 0;
}

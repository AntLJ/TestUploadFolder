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
#include "CSV2DB.h"
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

		try
		{
			// コマンドライン引数処理
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "コマンドライン引数に問題があります" << endl;
				return 1;
			}

			// ログファイル準備
			if( !RUNLOGGER.Initialize( args.getValue( arg::kRunLog ).GetString() ) ||
				!ERRLOGGER.Initialize( args.getValue( arg::kErrLog ).GetString() ) )
			{
				throw std::runtime_error( "Log file open failure." );
			}

			// オプション表示
			RUNLOGGER.Log( args.description(), true );

			// 実処理
			try{
				CCSV2DB app( args );
				app.execute();
			}
			catch( const std::runtime_error& err ){
				RUNLOGGER.Fatal( uh::toTStr(err.what()), true );
				RUNLOGGER.Log( uh::toTStr("\n異常終了"), true );
				return 1;
			}
			
			RUNLOGGER.Log( uh::toTStr("\n正常終了"), true );

			RUNLOGGER.Finalize();
			ERRLOGGER.Finalize();

			return 0;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 2;
	}
}

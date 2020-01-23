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
#include "Diff2Sindy.h"
#include "Notifier.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <TDC/useful_headers/scope_guard.h>

using namespace std;

class PrintEndTime
{
public:
	~PrintEndTime(){
		wcout << L"#――――――――――――――――――――――――――――――" << endl;
		wcout << L"# 終了時刻 : " << boost::lexical_cast< std::wstring >( boost::posix_time::second_clock::local_time() ) << endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	uh::scope_guard sg( []{ system("pause"); } );
#endif

	int ret = 0;
	try
	{
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		std::wofstream runLogStream;		//!< 実行ログファイルの出力ストリーム
		std::wofstream errLogStream;		//!< エラーログファイルの出力ストリーム

		try
		{
			PrintEndTime ptime;

			// コマンドライン引数処理
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "コマンドライン引数に問題があります" << endl;
				return 1;
			}

			// ログファイルパスのチェックとストリーム作成
			// wcout, wcerrの出力先をログストリームに変更する
			// 参考：C++: ストリームの出力先をファイルや標準出力に切り替える http://handasse.blogspot.com/2009/05/c.html
			if( !args.m_runLog.empty() )
			{
				runLogStream.open( args.m_runLog );
				if (! runLogStream )
					notifier::critical::asCannotOpenLogFile( args.m_runLog );
				wcout.rdbuf( runLogStream.rdbuf() );
			}
			if( !args.m_errLog.empty() )
			{
				errLogStream.open( args.m_errLog );
				if (! errLogStream )
					notifier::critical::asCannotOpenLogFile( args.m_errLog );
				wcerr.rdbuf( errLogStream.rdbuf() );
			}

			// 実処理
			CDiff2Sindy app( args );
			if( app.execute() ){
				// 正常終了
				cerr << "正常終了" << endl;
				ret = 0;
			}
			else{
				// 異常終了
				cerr << "異常終了" << endl;
				ret = 1;
			}
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
		ret = 2;
	}
	return ret;
}

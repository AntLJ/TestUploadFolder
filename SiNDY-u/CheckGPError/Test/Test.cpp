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

// Test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <arctl/coinitializer.h>
#include <AddrLib/Initializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	_tsetlocale( LC_ALL, L"Japanese" );

	// テスト結果収集リスナをコントローラにアタッチする
	CPPUNIT_NS::TestResultCollector result;

	try
	{
		arctl::coinitializer aoinit;

		// 文字付き住所テーブル・ヨミ変換テーブル読み込み
		if( addr::Init_CodeConverter(_T("CodeConvTable.mdb")) )
		{
			// イベントマネージャとテスト・コントローラを生成
			CPPUNIT_NS::TestResult controller;
			controller.addListener( &result );

			// 「.」で進行状況を出力するリスナをアタッチする
			CPPUNIT_NS::BriefTestProgressListener progress;
			controller.addListener( &progress );

			// テスト・ランナーにテスト郡を与え、テストする
			CPPUNIT_NS::TestRunner runner;
			runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
			runner.run( controller );

			// テスト結果を標準出力に吐き出す
			CPPUNIT_NS::CompilerOutputter outputer( &result, CPPUNIT_NS::stdCOut() );
			outputer.write();
		}

	}
	catch( std::exception& e )
	{
		std::cout << e.what() << std::endl;
	}

	return result.wasSuccessful() ? 0 : 1;
}


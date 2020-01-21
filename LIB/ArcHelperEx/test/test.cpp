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

// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <locale>

int _tmain(int argc, TCHAR* argv[])
{
	int nRet = 0;

//	std::locale::global(std::locale(""));
//	_tsetlocale( LC_ALL, _T("") );

	CoInitialize(NULL);
	{
		esriLicenseStatus emStatus;
		IAoInitializePtr ipAoInitialize( CLSID_AoInitialize );
		ipAoInitialize->Initialize( esriLicenseProductCodeArcEditor, &emStatus );

		// イベント・マネージャとテスト・コントローラを生成する
		CppUnit::TestResult controller;

		// テスト結果収集リスナをコントローラにアタッチする
		CppUnit::TestResultCollector result;
		controller.addListener( &result );

		// 「.」で進行状況を出力するリスナをアタッチする
		CppUnit::BriefTestProgressListener progress;
		controller.addListener( &progress );

		// テスト・ランナーにテスト群を与え、テストする
		CppUnit::TextUi::TestRunner runner;
//		runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
		runner.addTest(CppUnit::TestFactoryRegistry::getRegistry("CTestGeometryCheck").makeTest());
		runner.run( controller );

		// テスト結果を標準出力に吐き出す
		CppUnit::CompilerOutputter outputter2( &result, std::cout );
		outputter2.write();

		// デバッグに出力する
		std::ostringstream stream;
		stream << "************ テスト開始 ************\n";
		CppUnit::CompilerOutputter outputter( &result, stream );
		outputter.write();
		stream << "************ テスト終了 ************\n";
		// 得られた文字列をTRACEマクロで出力する
		ATLTRACE(stream.str().c_str());

		nRet = runner.result().wasSuccessful() ? 0 : 1;
	}
	CoUninitialize();
	getchar();

	return nRet;
}

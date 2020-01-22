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

// test.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

const char* kDataFolderPath = "data";
const char* kSampleFilePath = "data\\メッシュリスト.txt";
const char* kSampleFile2Path = "data\\meshlist2.txt";
const char* kSampleFileShrPath = "data\\メッシュリスト.txt.shr";
const char* kSampleFileShrPath2 = "data\\meshlist.txt.shr2";
const char* kSampleFileRelPath = ".\\メッシュリスト.txt";
const char* kSampleFile2RelPath = ".\\meshlist2.txt";
const char* kGlobalSampleFilePath = "\\\\globe\\TDC\\tmp\\test\\rns\\メッシュリスト.txt";
const char* kGlobalSampleFileShrPath = "\\\\globe\\TDC\\tmp\\test\\rns\\メッシュリスト.txt.shr";

/////////////////////////////////////////////////////////////////////////////
// エントリーポイント
/////////////////////////////////////////////////////////////////////////////

int _main(int argc, char* argv[])
{
	bool bRes = true;

	CppUnit::TextUi::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
	runner.setOutputter(CppUnit::CompilerOutputter::defaultOutputter(&runner.result(), std::cerr));
	bRes &= runner.run();

	return bRes ? 0 : 1;
}

int main(int argc, char* argv[])
{
	// 処理開始。
	return _main(argc, argv);
}

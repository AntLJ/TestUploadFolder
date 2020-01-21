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
#include <arctl/coinitializer.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

CComModule _Module;
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

const TCHAR kSourceWorkspace[] = _T("sindy2004@amber(SDE.DEFAULT)");
const TCHAR kTestWorkspace[] = _T("data/test.mdb");
const TCHAR kTable1[] = _T("TABLE1");
const TCHAR kTable2[] = _T("TABLE2");
const TCHAR kTable3[] = _T("TABLE3");
const TCHAR kValue1[] = _T("VALUE1");
const TCHAR kValue2[] = _T("VALUE2");
const TCHAR kValue3[] = _T("VALUE3");
const TCHAR kShape[] = _T("SHAPE");

/////////////////////////////////////////////////////////////////////////////
// エントリーポイント
/////////////////////////////////////////////////////////////////////////////

int _main(int argc, char* argv[])
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
	CppUnit::Outputter* outputter = CppUnit::CompilerOutputter::defaultOutputter(&runner.result(), std::cerr);
	runner.setOutputter(outputter);
	return runner.run() ? 0 : 1;
}

int main(int argc, char* argv[])
{
	// 初期化。
	arctl::coinitializer aCoInitializer;
	_Module.Init(ObjectMap, ::GetModuleHandle(NULL));

	// 処理開始。
	return _main(argc, argv);
}

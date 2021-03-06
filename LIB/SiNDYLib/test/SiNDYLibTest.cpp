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

// SiNDYLibTest.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <locale>

int _tmain(int argc, TCHAR* argv[])
{
	int nRet = 0;

	std::locale::global(std::locale("japanese"));
	CoInitialize(NULL);
	{
		esriLicenseStatus emStatus;
		IAoInitializePtr ipAoInitialize( CLSID_AoInitialize );
		ipAoInitialize->Initialize( esriLicenseProductCodeArcEditor, &emStatus );

		CppUnit::TextUi::TestRunner runner;
		runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
		CppUnit::Outputter* outputter = new CppUnit::CompilerOutputter(&runner.result(),std::cout);
		runner.setOutputter(outputter);

		nRet = runner.run() ? 0 : 1;

	}
	CoUninitialize();
	getchar();
	return nRet;
}


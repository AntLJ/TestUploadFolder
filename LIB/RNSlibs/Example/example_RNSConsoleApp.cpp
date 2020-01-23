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

#include <iostream>
#include <RNSConsoleApp.h>

class RNSTestApp : public RNSConsoleApp
{
protected:
	virtual bool isNeedPrintUsage();
	virtual void printUsage(std::ostream& cStream);
	virtual int entrust();
}

bool RNSTestApp::isNeedPrintUsage()
{
	return argList().size() != 1;
}

void RNSTestApp::printUsage(std::ostream& cStream)
{
	RNSConsoleApp::printUsage(cStream);
	cStream << "Usage: " << appName() << " <arg>" << std::endl;
}

int RNSTestApp::entrust()
{
	std::cout << "argument is " argList().at(0).c_str() << std::endl;
}

int main(int argc, char* argv[])
{
	RNSTestApp aApp("第１引数をそのまま標準エラー出力するプログラム");
	aApp.setArgs(argc, argv);

	return aApp.run();
}

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
#include "CheckMultiLanguageRoad.h"
#include <arctl/coinitializer.h>

int main(int argc, char* argv[])
{
	int	nRet = 0;
	try{
		arctl::coinitializer coinitializer;

		CCheckMultiLanguageRoad lpApp;

		if( !lpApp.init(argc, argv) )
		{
			lpApp.printUsage();
			nRet = 1;
		}
		else
		{
			if( !lpApp.execute() )
				nRet = 1;
			else if( lpApp.IsError() )
				nRet = 3;
		}

		if( !nRet )
			std::cerr << "³íI—¹" << std::endl;
		else
			std::cerr << "ˆÙíI—¹I" << std::endl;

	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		nRet = 2;

	}
	return nRet;
}

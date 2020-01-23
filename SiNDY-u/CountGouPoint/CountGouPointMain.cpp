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

// CountGouPointMain.cpp

#include "CountGouPoint.h"
#include <arctl/coinitializer.h>

int main(int cArgc, char *cArgv[])
{
	int		aRet = 0;

	try{
		arctl::coinitializer coinitializer;


//		::CoInitialize(0);
		{
			CountGouPoint	aCountGP;
			if(!aCountGP.Execute(cArgc, cArgv)) aRet = -1;
		}
//		::CoUninitialize();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return aRet;

}

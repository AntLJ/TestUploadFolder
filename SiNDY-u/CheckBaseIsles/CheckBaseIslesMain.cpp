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

// CheckBaseIslesMain.cpp
#include "stdafx.h"
#include "CheckBaseIsles.h"
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/flush_scope_guard.h>

int main(int cArgc, char **cArgv)
{
	int	aRet = 0;

	uh::flush_scope_guard fsg;

	try{
		arctl::coinitializer coinitializer;
		
//		::CoInitialize(0);
		{
			CheckBaseIsles	aChkBaseIsles;
			if(!aChkBaseIsles.Execute(cArgc, cArgv)) aRet = -1;
		}
//		::CoUninitialize();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return aRet;

}

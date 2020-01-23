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

// CreateExTPTxtMain.cpp

#include "stdafx.h"
#include "CreateExTPTxt.h"
#include <arctl/coinitializer.h>

int main(int argc, char **argv)
{
	int nRet = 0;

	try
	{
		arctl::coinitializer aoinit;
		CreateExTPTxt	aCrExTPTxt;
		if( aCrExTPTxt.Init(argc, argv) )
		{
			aCrExTPTxt.Execute();
		}
		else
			nRet = 1;
	}
	catch( std::exception& e )
	{
		nRet = 2;
		std::cout << e.what() << std::endl;
	}

	if( !nRet )
		std::cout << "#³íI—¹" << std::endl;

	return nRet;
}

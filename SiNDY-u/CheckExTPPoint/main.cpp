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
#include "CheckExTPPoint.h"
#include <arctl/coinitializer.h>

int main(int argc, char* argv[])
{
	// ���ϐ��擾
	if( argc == 1 )
	{
		CCheckExTPPoint::printUsage();
		return 1;
	}

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CCheckExTPPoint cApp;
		if( cApp.init(argc, argv) )
			cApp.execute();
		else
			nRet = 2;
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		nRet = 1;
	}

	if( !nRet )
		std::cerr << "����I��" << std::endl;
		
	return 0;
}

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

// CheckCityRepMain.cpp

#include "stdafx.h"
#include "CheckCityRep.h"
#include <arctl/coinitializer.h>

// ÉÅÉCÉìä÷êî
int main(int cArgc, char **cArgv)
{
	if( cArgc == 1 )
	{
		CheckCityRep::Usage();
		return 0;
	}
	int	aRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CheckCityRep	aChkCR;
		if( !aChkCR.Init(cArgc, cArgv) || !aChkCR.Execute() )
			aRet = -1;
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
	}

	return aRet;

}

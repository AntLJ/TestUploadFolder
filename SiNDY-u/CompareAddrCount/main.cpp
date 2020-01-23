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

// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CompareAddrCount.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
	int aRet = 0;

	try
	{
		arctl::coinitializer aoinit;
		CompareAddrCount aCompAC;
		if(aCompAC.Init(argc, argv))
			aCompAC.Execute();
		else
			aRet = 1;
	}
	catch(exception& e)
	{
		cerr << e.what() << endl;
		aRet = 2;
	}

	return aRet;
}


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

// CreateExistFile_Main.cpp

#include "CreateExistFile.h"
#include <arctl/coinitializer.h>

int main(int cArgc, char **cArgv)
{
	const arctl::coinitializer coinit;

//#ifdef _DEBUG
//	_putenv("DB_MAP=sinup2007b/sinup2007b/SDE.Current07B/5151/topaz2");
//	_putenv("EXIST_DIR=C:\\test\\CreateExistFile\\exist");
//#endif

	int	aRet = 1;

//	::CoInitialize(0);
	{
		CreateExistFile	aCreateEF;
		if(aCreateEF.Create(cArgc, cArgv)) aRet = 0;	// ë∂ç›îªíËÉtÉ@ÉCÉãÇÃê∂ê¨
	}
//	::CoUninitialize();

	return aRet;

}

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

// CheckBaseRepExistMain.cpp

#include "CheckBaseRepExist.h"
#include <arctl/coinitializer.h>

#ifdef _DEBUG
#include <conio.h>
#include <ctype.h>
#endif

using namespace std;

int main(int cArgc, char **cArgv)
{
	const arctl::coinitializer coinit;

#ifdef	_DEBUG
	_putenv("JDB_CONNECT=release/release@delltcp");
#endif

	int aRet = 0;

	CheckBaseRepExist	aChkBRExist;
	if(!aChkBRExist.Execute(cArgc, cArgv)) aRet = -1;

#ifdef _DEBUG
	_getch();
#endif

	return aRet;

}

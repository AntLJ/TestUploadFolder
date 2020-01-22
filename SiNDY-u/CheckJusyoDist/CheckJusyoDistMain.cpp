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

// CheckJusyoDistMain.cpp
#include "StdAfx.h"
#include "CheckJusyoDist.h"
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/flush_scope_guard.h>

int main(int cArgc, char **cArgv)
{
#ifdef _DEBUG
	_putenv("DB_ADDRESS=fix201511/fix201511/SDE.DEFAULT/5151/plum.mr.ipc.pioneer.co.jp");
	_putenv("JDB_CONNECT=y1511/y1511@proteus");
	_putenv("FC_GOU_POINT=Gou_Point");
	_putenv("FC_CITY_ADMIN=City_Admin");
	_putenv("FC_CS_ADDR_POINT=CS_Addr_Point");
	_putenv("FC_KLINK_POINT=Klink_Point");
	_putenv("PGDB_OUT=C:\\test\\CheckJusyoDist\\test_addr2.mdb");
	_putenv("FC_OUT=points_test");
#endif

	// ã≠êßÉtÉâÉbÉVÉÖ
	uh::flush_scope_guard fsg;

	if( 1 == cArgc )
	{
		CheckJusyoDist::Usage();
		return 0;
	}

	int aRet = 0;

	try
	{
		arctl::coinitializer aoinit;
		CheckJusyoDist	aChkJDist;
		if(!aChkJDist.Execute(cArgc, cArgv)) aRet = -1;
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		aRet = 2;
	}

	return aRet;

}

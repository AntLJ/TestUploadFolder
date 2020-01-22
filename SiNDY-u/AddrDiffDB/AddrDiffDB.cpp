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

// AddrDiffDB.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "App.h"
#include <arctl/coinitializer.h>

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	_tputenv( _T("PGDB_MODE=on") );
	_tputenv( _T("DB_ADDRESS_NEW=\\\\Ishtar\\common\\WORK\\13spring_daichou\\PGDB") );
//	_tputenv( _T("DB_ADDRESS_NEW=SiNDY2007A/SiNDY2007A/SDE.Current07A/5151/topaz2") );
	_tputenv( _T("FC_CITY_ADMIN_NEW=city_admin") );
	_tputenv( _T("FC_GOU_POINT_NEW=gou_point") );
	_tputenv( _T("DB_ADDRESS_OLD=\\\\Ishtar\\common\\WORK\\13spring_pre\\daichou\\PGDB") );
//	_tputenv( _T("DB_ADDRESS_OLD=FIX200611/FIX200611/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("FC_CITY_ADMIN_OLD=city_admin") );
	_tputenv( _T("FC_GOU_POINT_OLD=gou_point") );
	_tputenv( _T("FC_CITYMESH=CityMesh") );
#endif

	if( 1 == argc )
	{
		CApp::printUsage();
		return 0;
	}

	int nRet = 0;
	try
	{
		arctl::coinitializer aoinit;
		CApp cApp;
		if( !cApp.init(argc, argv) || !cApp.execute() )
			nRet = 1;
	}
	catch( std::exception& e )
	{
		std::cout << e.what() << std::endl;
		nRet = 2;
	}

	if( !nRet )
		std::cout << "正常終了しました" << std::endl;

	return nRet;
}




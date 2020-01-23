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

// DeleteAddrPointMain.cpp

#include "stdafx.h"
#include "DeleteAddrPoint.h"

#include <boost/progress.hpp>
#include <arctl/coinitializer.h>

int _tmain( int cArgc, _TCHAR* cArgv[] )
{
	_tsetlocale( LC_ALL, L"Japanese" );

	boost::progress_timer t;
	if( cArgc == 1 )
	{
		DeleteAddrPoint::PrintUsage();
		return 0;
	}

	int	nRet = 0;
	//::CoInitialize(0);
	arctl::coinitializer coinitializer;
	try
	{
		DeleteAddrPoint aDeleteAddrPoint;
		if( !aDeleteAddrPoint.Init(cArgc, cArgv) || !aDeleteAddrPoint.Execute() )
			nRet = 1;
	}
	catch( _com_error& e )
	{
		cerr << "#Fatal Error COMエラー発生 : " << CT2CA(e.ErrorMessage()) << endl;
		nRet = 2;
	}
	catch( std::exception& e )
	{
		cerr << "#Fatal Error 例外発生 : " << e.what() << endl;
		nRet = 3;
	}
	catch( ... )
	{
		cerr << "#Fatal Error 例外発生" << endl;
		nRet = 4;
	}
	::CoUninitialize();

	if( nRet == 0 )
		cout << "正常終了しました";
	
	return nRet;
}

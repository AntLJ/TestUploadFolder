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
#include "Environ.h"
#include "CheckTelTPPoint.h"
#include <arctl/coinitializer.h>

/**
 * @brief	ŠÂ‹«•Ï”æ“¾
 * @param	rParam	[in, out]	ŠÂ‹«•Ï”ƒpƒ‰ƒ[ƒ^
 */
bool getEnv( CEnviron& rEnv )
{
#ifdef _DEBUG
//	_tputenv( _T("DB_TP=SINDY2009B/SINDY2009B/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("DB_TP=sindy2010B/sindy2010B/SDE.EDT_SINDY2010B/5151/topaz2") );
	//_tputenv( _T("DB_TP=TECHMAP/TECHMAP/TECHMAP.CheckTelTP_Test/5151/spinel2") );
//	_tputenv( _T("DB_POI=REFERENCE/REFERENCE/SDE.DEFAULT/5151/ruby2") );	// Bug 5701‘Î‰
	_tputenv( _T("DB_POI=REFERENCE/REFERENCE/SDE.DEFAULT/5151/ruby2") );
//	_tputenv( _T("DB_ADDRESS=SINDY2009B/SINDY2009B/SDE.DEFAULT/5151/topaz2") );
	_tputenv( _T("DB_ADDRESS=ronly/ronly/SDE.EDT_SINDY2010B/5151/topaz2") );
//	_tputenv( _T("DB_MAP=SINDY2009B/SINDY2009B/SDE.DEFAULT/5151/ruby2") );
	_tputenv( _T("DB_MAP=ronly/ronly/SDE.EDT_SINDY2010B/5151/ruby2") );
//	_tputenv( _T("DB_BASE=SINDY2009/SINDY2009/SDE.DEFAULT/5151/onyx") );
	_tputenv( _T("DB_BASE=ronly/ronly/SDE.DEFAULT/5151/onyx") );
	//_tputenv( _T("DB_BASE=ronly/ronly/SDE.DEFAULT/5151/onyx") );
	_tputenv( _T("FC_POI=REFERENCE.Common_Contents") );						// Bug 5701‘Î‰
	_tputenv( _T("FC_TelTP_POINT=SiNDY2010B.TelTP_Point") );
	//_tputenv( _T("FC_TelTP_POINT=TECHMAP.TelTP_POINT") );
	_tputenv( _T("FC_CITY_ADMIN=SiNDY2010B.City_Admin") );
	_tputenv( _T("FC_CITY_SITE=SiNDY2010B.City_Site") );
	_tputenv( _T("FC_BASE_SITE=SiNDY2010.Base_Site") );
	//_tputenv( _T("FC_BASE_SITE=SiNDY2007.Base_Site") );
#endif

	CString strError(_T(""));
	if( !rEnv.m_strTPDBProp.GetEnvironmentVariable(_T("DB_TP")) )				strError += _T("#Error ŠÂ‹«•Ï” DB_TP ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strAddrDBProp.GetEnvironmentVariable(_T("DB_ADDRESS")) )		strError += _T("#Error ŠÂ‹«•Ï” DB_ADDRESS ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strCityDBProp.GetEnvironmentVariable(_T("DB_MAP")) )			strError += _T("#Error ŠÂ‹«•Ï” DB_MAP ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strBaseDBProp.GetEnvironmentVariable(_T("DB_BASE")) ) 			strError += _T("#Error ŠÂ‹«•Ï” DB_BASE ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strTelTPFName.GetEnvironmentVariable(_T("FC_TelTP_POINT")) )	strError += _T("#Error ŠÂ‹«•Ï” FC_TelTP_POINT ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strCityAdminFName.GetEnvironmentVariable(_T("FC_CITY_ADMIN")) )	strError += _T("#Error ŠÂ‹«•Ï” FC_CITY_ADMIN ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strCitySiteFName.GetEnvironmentVariable(_T("FC_CITY_SITE")) )	strError += _T("#Error ŠÂ‹«•Ï” FC_CITY_SITE ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strBaseSiteFName.GetEnvironmentVariable(_T("FC_BASE_SITE")) )	strError += _T("#Error ŠÂ‹«•Ï” FC_BASE_SITE ‚Ìæ“¾‚É¸”s\n");
	if( !rEnv.m_strPOIDBProp.GetEnvironmentVariable(_T("DB_POI")) )				strError += _T("#Error ŠÂ‹«•Ï” DB_POI ‚Ìæ“¾‚É¸”s\n");	// Bug 5701‘Î‰
	if( !rEnv.m_strPOIFName.GetEnvironmentVariable(_T("FC_POI")) )				strError += _T("#Error ŠÂ‹«•Ï” FC_POI ‚Ìæ“¾‚É¸”s\n");	// Bug 5701‘Î‰

	if( !strError.IsEmpty() )
	{
		std::cerr << strError;
		return false;
	}
	else
		return true;
}

int main(int argc, char* argv[])
{
	try{
		arctl::coinitializer coinitializer;
		
		// ŠÂ‹«•Ï”æ“¾
		CEnviron cEnv;
		if( argc == 1 || !getEnv(cEnv) )
		{
			CCheckTelTPPoint::printUsage();
			return 1;
		}

		int nRet = 0;
	//	if( SUCCEEDED(::CoInitialize(0)) )
	//	{
	//		CCheckTelTPPoint* lpApp = new CCheckTelTPPoint(cEnv);
			CCheckTelTPPoint lpApp(cEnv);
			if( lpApp.init(argc, argv) )
			{
				lpApp.execute();
			}
			else
			{
				lpApp.printUsage();
				nRet = 2;
			}
			//delete lpApp;
	//	}
	//	else
	//		nRet = 1;

	//	::CoUninitialize();

		if( !nRet )
			std::cerr << "³íI—¹" << std::endl;
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

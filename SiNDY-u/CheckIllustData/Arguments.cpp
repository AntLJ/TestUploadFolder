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

#include "StdAfx.h"
#include "Arguments.h"
#include "resource.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/illustlib/illust_util.h>
#include <ArcHelperEx/AheglobalsMisc.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;
// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_mode      = "mode";
	const char * const k_config    = "config";
	const char * const k_namerule  = "name_rule";
	const char * const k_illustdb  = "illustdb";
	const char * const k_roaddb    = "roaddb";
	const char * const k_runlog    = "runlog";
	const char * const k_errlog    = "errlog";

	po::options_description desc("How to use");
	try{
		desc.add_options()
			(k_mode,      tvalue<tstring>(&m_mode),      CStringA(AheLoadString(IDS_ARGUMENT_MODE)).GetString())
			(k_config,    tvalue<tstring>(&m_config),    CStringA(AheLoadString(IDS_ARGUMENT_CONFIGFILE)).GetString())
			(k_namerule,  tvalue<tstring>(&m_namerule),  CStringA(AheLoadString(IDS_ARGUMENT_NAMERULE)).GetString())
			(k_illustdb,  tvalue<tstring>(&m_illustdb),  CStringA(AheLoadString(IDS_ARGUMENT_ILLUSTDB)).GetString())
			(k_roaddb,    tvalue<tstring>(&m_roaddb),    CStringA(AheLoadString(IDS_ARGUMENT_ROADDB)).GetString())
			(k_runlog,    tvalue<tstring>(&m_runlog),    CStringA(AheLoadString(IDS_ARGUMENT_RUNLOG)).GetString())
			(k_errlog,    tvalue<tstring>(&m_errlog),    CStringA(AheLoadString(IDS_ARGUMENT_ERRORLOG)).GetString())
			;


		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( k_mode, m_mode, false, true );
		good &= CheckParam( k_config, m_config, true );
		good &= CheckParam( k_namerule, m_namerule, true );
		good &= CheckParam( k_illustdb, m_illustdb );
		good &= CheckParam( k_roaddb, m_roaddb );
		good &= CheckParam( k_runlog, m_runlog );
		good &= CheckParam( k_errlog, m_errlog );

		if( ! good ){
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e){
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */, bool bInt /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << CStringA(AheLoadString(IDS_REQUIREDOPTION)).GetString() << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() )){
			cerr << toStr( optValue ) << CStringA(AheLoadString(IDS_ISNOTFILE)).GetString() << endl;
			return false;
		}
	}

	if( bInt ){
		if( !illust_util::isNumber(optValue.c_str()) )
		{
			cerr << toStr( optValue ) << CStringA(AheLoadString(IDS_ISNOTNUMBER)).GetString() << endl;
			return false;
		}
	}
	return true;
}

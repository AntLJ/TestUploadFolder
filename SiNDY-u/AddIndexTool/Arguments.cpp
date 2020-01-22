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
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/text_util.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

Arguments::Arguments(){}

bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_db     = "db";
	const char * const k_config = "config";
	const char * const k_runlog = "runlog";
	const char * const k_errlog = "errlog";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_db,  tvalue<tstring>(&m_db),     AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_INPUT))
			(k_config, tvalue<tstring>(&m_config),    AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_CONFIG))
			(k_runlog, tvalue<tstring>(&m_runLog),    AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_RUNLOG))
			(k_errlog, tvalue<tstring>(&m_errLog),    AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_ERRLOG))
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= checkParam( k_db,  m_db );
		good &= checkParam( k_config, m_config, true );
		good &= checkParam( k_runlog, m_runLog );
		good &= checkParam( k_errlog, m_errLog );

		if( ! good )
		{
			cerr << AheLoadStringA(IDS_REQUIRED_PARAMETER).GetString() << endl;
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

bool Arguments::checkParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ ) const
{
	if( optValue.empty() ){
		cerr << optName << AheLoadStringA(IDS_NONE_NECESSARY_PARAMETER).GetString() << endl;
		return false;
	}
	if( !bFile ) return true;
	return checkFile( optName, optValue );
}
bool Arguments::checkFile( const char* const optName, const uh::tstring& optValue ) const
{
	if( ! uh::isFile(optValue.c_str()) )
	{
		cerr << toStr( optValue ) << AheLoadStringA(IDS_IS_NOT_FILE).GetString() << endl;
		return false;
	}
	return true;
}

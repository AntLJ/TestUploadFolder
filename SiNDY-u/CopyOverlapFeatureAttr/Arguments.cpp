#include "StdAfx.h"
#include "Arguments.h"
#include "resource.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <ArcHelperEx/AheglobalsMisc.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, TCHAR* argv[])
{
	const char * const k_setting   = "setting";
	const char * const k_runlog    = "runlog";
	const char * const k_errlog    = "errlog";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_setting,     tvalue<tstring>(&m_setting),     CStringA(AheLoadString(IDS_ARGUMENT_SETTING)).GetString())
			(k_runlog,      tvalue<tstring>(&m_runlog),      CStringA(AheLoadString(IDS_ARGUMENT_RUNLOG)).GetString())
			(k_errlog,      tvalue<tstring>(&m_errlog),      CStringA(AheLoadString(IDS_ARGUMENT_ERRLOG)).GetString())
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( k_setting,  m_setting, true );
		good &= CheckParam( k_runlog,   m_runlog,  true );
		good &= CheckParam( k_errlog,   m_errlog,  true );
		
		if( ! good )
		{
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ ) const
{
	if( optValue.empty() )
	{	
		cerr << optName << CStringA(AheLoadString(IDS_REQUIREDOPTION)).GetString() << endl;
		return false;
	}

	if( bFile )
	{
		if( ! isFile( optValue.c_str() ))
		{
			cerr << toStr( optValue ) << CStringA(AheLoadString(IDS_ISNOTFILE)).GetString() << endl;
			return false;
		}
	}

	return true;
}

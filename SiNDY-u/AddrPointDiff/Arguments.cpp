#include "StdAfx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <regex>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try{
		desc.add_options()
			(arg::k_ipc_addr_db,    uh::tvalue<uh::tstring>(&m_ipc_addr_db),    "[必須] SiNDY（住所）接続先")
			(arg::k_ipc_bg_db,      uh::tvalue<uh::tstring>(&m_ipc_bg_db),      "[必須] SiNDY（背景）接続先")
			(arg::k_mid_addr_db,    uh::tvalue<uh::tstring>(&m_mid_addr_db),    "[必須] 住所中間ファイル")
			(arg::k_mid_addr_layer, uh::tvalue<uh::tstring>(&m_mid_addr_layer), "[必須] 住所中間ファイルレイヤ名")
			(arg::k_citycode_list,  uh::tvalue<uh::tstring>(&m_citycode_list),  "[必須] 市区町村コードリスト")
			(arg::k_point_dist,     uh::tvalue<uh::tstring>(&m_point_dist),     "[必須] ポイント距離閾値")
			(arg::k_run_log,        uh::tvalue<uh::tstring>(&m_run_log),        "[必須] 実行ログファイルパス")
			(arg::k_err_log,        uh::tvalue<uh::tstring>(&m_err_log),        "[必須] エラーログフォルダパス")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( arg::k_ipc_addr_db,    m_ipc_addr_db );
		good &= CheckParam( arg::k_ipc_bg_db,      m_ipc_bg_db );
		good &= CheckParam( arg::k_mid_addr_db,    m_mid_addr_db );
		good &= CheckParam( arg::k_mid_addr_layer, m_mid_addr_layer );
		good &= CheckParam( arg::k_citycode_list,  m_citycode_list, ArgValueType::File );
		good &= CheckParam( arg::k_point_dist,     m_point_dist );
		good &= CheckParam( arg::k_run_log,        m_run_log );
		good &= CheckParam( arg::k_err_log,        m_err_log, ArgValueType::Folder );
		good &= CheckDistValue( arg::k_point_dist, m_point_dist );

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

bool Arguments::CheckParam( const char* const optName, const uh::tstring& optValue, ArgValueType type /* = ArgValueType::Other */ )
{
	
	if( optValue.empty() ){	
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	if( type == ArgValueType::File )
	{
		if( !PathFileExists(optValue.c_str()) )
		{
			cerr << toStr(optValue) << "が存在しません" << endl;
			return false;
		}
	}

	if( type == ArgValueType::Folder )
	{
		if( !PathIsDirectory(optValue.c_str()) )
		{
			cerr << toStr(optValue) << "が存在しません" << endl;
			return false;
		}
	}

	return true;
}

bool Arguments::CheckDistValue( const char* const optName, const uh::tstring& optValue )
{
	static const std::wregex re(L"^([0-9]+|[0-9]+\\.[0-9]+)$");

	if( !std::regex_match(CStringW(optValue.c_str()).GetString(), re) )
	{
		cerr << optName << "が数値ではありません（" << toStr(optValue) << "）"  << endl;
		return false;
	}

	return true;
}

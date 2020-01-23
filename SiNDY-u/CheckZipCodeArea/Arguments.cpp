#include "StdAfx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <boost/assign.hpp>

namespace exec_opt
{
	// チェック実行モードオプション引数
	const uh::tstring CHECK_TABLE = _T("table");
	const uh::tstring CHECK_TABLEMASTER = _T("table_master");
	const uh::tstring CHECK_TABLEADDR = _T("table_addr");
	const uh::tstring CHECK_POLYGON = _T("polygon");
	const uh::tstring CHECK_POLYGONTABLE = _T("polygon_table");
	const uh::tstring CHECK_POLYGONADDR = _T("polygon_addr");
	const uh::tstring CHECK_ALL = _T("all");
}

using namespace std;
using namespace uh;
using namespace exec_opt;
namespace po = boost::program_options;

namespace
{
	const char * const k_hyphen = "--";
	const char * const k_checkMode = "check_mode";
	const char * const k_zipDb = "zip_db";
	const char * const k_addrDb = "addr_db";
	const char * const k_charConv = "char_conv";
	const char * const k_charConvTmp = "char_conv_tmp";
	const char * const k_jusyoMaster = "jusyo_master";
	const char * const k_kajyoMaster = "kajyo_master";
	const char * const k_dupliRate = "dupli_rate";
	const char * const k_mesh = "mesh";
	const char * const k_runLog = "run_log";
	const char * const k_errLog = "err_log";
}


// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_checkMode, tvalue<tstring>(&m_checkMode), "[必須]チェックモード")
			(k_zipDb, tvalue<tstring>(&m_zipDb), "[必須]郵便番号ポリゴンDB接続先")
			(k_addrDb, tvalue<tstring>(&m_addrDb), "[必須]住所(行政界・号・CS)DB接続先")
			(k_charConv, tvalue<tstring>(&m_charConv), "[table_addr polygon_addrで必須]文字付き住所コード変換テーブル")
			(k_charConvTmp, tvalue<tstring>(&m_charConvTmp), "[table_addr polygon_addrで必須]文字付き住所コード仮変換テーブル")
			(k_jusyoMaster, tvalue<tstring>(&m_jusyoMaster), "[必須]住所マスタ Yyymm/Yyymm@proteus")
			(k_kajyoMaster, tvalue<tstring>(&m_kajyoMaster), "[必須]加除マスタ Kyymm/Kyymm@proteus")
			(k_dupliRate, tvalue<tstring>(&m_dupliRate), "[polyogn_addrで必須]郵便番号ポリゴンと行政界の重複閾値(整数)")
			(k_mesh, tvalue<tstring>(&m_mesh), "[polygonで必須]ポリゴンチェック対象メッシュコードリスト")
			(k_runLog, tvalue<tstring>(&m_runLog), "[必須]実行ログ出力先ディレクトリ")
			(k_errLog, tvalue<tstring>(&m_errLog), "[必須]エラーログ出力先ディレクトリ")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam(k_checkMode, m_checkMode);
		good &= CheckParam(k_zipDb, m_zipDb);
		good &= CheckParam(k_addrDb, m_addrDb);
		good &= CheckParam(k_jusyoMaster, m_jusyoMaster);
		good &= CheckParam(k_kajyoMaster, m_kajyoMaster);
		good &= CheckParam(k_runLog, m_runLog);
		good &= CheckParam(k_errLog, m_errLog);

		// モードによって必要なオプションのチェック
		if (0 == m_checkMode.compare(CHECK_TABLEADDR) ||
			0 == m_checkMode.compare(CHECK_POLYGONADDR) ||
			0 == m_checkMode.compare(CHECK_ALL))
		{
			good &= CheckParam(k_charConv, m_charConv, true);
			good &= CheckParam(k_charConvTmp, m_charConvTmp, true);
		}

		if (0 == m_checkMode.compare(CHECK_POLYGONADDR) ||
			0 == m_checkMode.compare(CHECK_ALL))
		{
			good &= CheckParam(k_dupliRate, m_dupliRate);
		}

		if (0 == m_checkMode.compare(CHECK_POLYGON) ||
			0 == m_checkMode.compare(CHECK_ALL))
		{
			good &= CheckParam(k_mesh, m_mesh);
		}

		if (!good)
		{
			cerr << desc << endl;
			return false;
		}
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::CheckParam(const char* const optName, const tstring& optValue, bool bFile /* = false */)
{
	if (optValue.empty())
	{
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	if (bFile)
	{
		if (!isFile(optValue.c_str()))
		{
			cerr << toStr(optValue) << "はファイルではありません" << endl;
			return false;
		}
	}

	return true;
}

CString Arguments::GetAllOptionInfo()
{
	stringstream ss;
	ss << k_hyphen << k_checkMode << " : " << uh::toStr(m_checkMode) << endl;
	ss << k_hyphen << k_zipDb << " : " << uh::toStr(m_zipDb) << endl;
	ss << k_hyphen << k_addrDb << " : " << uh::toStr(m_addrDb) << endl;
	ss << k_hyphen << k_charConv << " : " << uh::toStr(m_charConv) << endl;
	ss << k_hyphen << k_charConvTmp << " : " << uh::toStr(m_charConvTmp) << endl;
	ss << k_hyphen << k_jusyoMaster << " : " << uh::toStr(m_jusyoMaster) << endl;
	ss << k_hyphen << k_kajyoMaster << " : " << uh::toStr(m_kajyoMaster) << endl;
	ss << k_hyphen << k_dupliRate << " : " << uh::toStr(m_dupliRate) << endl;
	ss << k_hyphen << k_mesh << " : " << uh::toStr(m_mesh) << endl;
	ss << k_hyphen << k_runLog << " : " << uh::toStr(m_runLog) << endl;
	ss << k_hyphen << k_errLog << " : " << uh::toStr(m_errLog) << endl;
	ss << endl;

	return ss.str().c_str();
}
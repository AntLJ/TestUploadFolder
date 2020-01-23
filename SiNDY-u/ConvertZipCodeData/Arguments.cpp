#include "StdAfx.h"
#include "Arguments.h"
#include "Log.h"
#include "Common.h"
#include <iostream>
#include <WinLib/VersionInfo.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

const char * const k_zip_db = "zip_db";
const char * const k_zip_schema = "zip_schema";
const char * const k_addr_db = "addr_db";
const char * const k_addr11_dir = "addr11_dir";
const char * const k_out_dir = "out_dir";
const char * const k_run_log = "run_log";
const char * const k_err_log = "err_log";

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_zip_db,		tvalue<tstring>(&m_zip_db),		"[必須]郵便番号ポリゴンの接続先")
			(k_zip_schema,	tvalue<tstring>(&m_zip_schema),	"[任意]郵便番号ポリゴンのスキーマ名")
			(k_addr_db,		tvalue<tstring>(&m_addr_db),	"[必須]住所DB接続先")
			(k_addr11_dir,	tvalue<tstring>(&m_addr11_dir),	"[任意]11桁住所データ格納先")
			(k_out_dir,		tvalue<tstring>(&m_out_dir),	"[必須]データ出力先")
			(k_run_log,		tvalue<tstring>(&m_run_log),	"[必須]実行ログ")
			(k_err_log,		tvalue<tstring>(&m_err_log),	"[必須]エラーログ")
			;

		po::store(po::parse_command_line(argc, argv, desc), m_vm);
		po::notify(m_vm);

		// 必須パラメータのチェック
		bool good = true;
		if(m_zip_db.empty() ||
			m_addr_db.empty() ||
			m_out_dir.empty() ||
			m_run_log.empty() ||
			m_err_log.empty()){
				std::cerr << "必須オプションが指定されていません" << std::endl;
				good = false;
		} else {
			// 出力先ディレクトリの存在確認
			if(!PathIsDirectory(m_out_dir.c_str())){
				std::cerr << k_out_dir << "オプション : 「" << toStr(m_out_dir) << "」のディレクトリは存在しません" << std::endl;
				good = false;
			}

			// 住所データ格納先ディレクトリが存在するか
			if(!m_addr11_dir.empty()){
				if(!PathIsDirectory(m_addr11_dir.c_str())){
					std::cerr << k_addr11_dir << "オプション : 「" << toStr(m_addr11_dir) << "」のディレクトリは存在しません" << std::endl;
					good = false;
				}
			}

			// ログ初期化
			if(good){
				if(!initLog(m_run_log.c_str(), m_err_log.c_str())){
					good = false;
				}
			}
		}
		if(!good){
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

CString Arguments::GetOption() const
{
	CString strOption = _T("[option]\n");
	for(const auto& op : m_vm){
		CString strOptionLine;
		strOptionLine.Format(_T("   --%s %s\n"), CString(op.first.c_str()), CString(op.second.as<uh::tstring>().c_str()));
		strOption += strOptionLine;
	}

	return strOption;
}

bool Arguments::initLog(const CString& strRunLog, const CString& strErrLog)
{
	// ログファイル作成
	if(!CLog::GetInstance().SetRunLog(strRunLog) ||
		!CLog::GetInstance().SetErrLog(strErrLog)){
		return false;
	}
	CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
	CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\t"));

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	CLog::GetInstance().PrintLog(true, false, true, false, GetOption());

	strMsg.Format(_T("Start : %s\n"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}

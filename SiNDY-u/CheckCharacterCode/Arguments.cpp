#include "stdafx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("使い方");

	try{
		desc.add_options()
			("help,h", "ヘルプを表示")
			(g_strFileListPath,		uh::tvalue<uh::tstring>(&strFileListPath),		"[必須]入力ファイルリストのパス")
			(g_strFileType,			uh::tvalue<uh::tstring>(&strFileType),			"[必須]チェック対象ファイルのフォーマット [shape]")
			(g_strCharCodeListPath,	uh::tvalue<uh::tstring>(&strCharCodeListPath),	"[必須]許容文字コード範囲指定ファイルのパス")
			(g_strExitMode,			uh::tvalue<uh::tstring>(&strExitMode),			"[必須]不正文字コード検出時の終了モード [immediate | all]")
			(g_strRunLogFilePath,	uh::tvalue<uh::tstring>(&strRunLogFilePath),	"[必須]実行ログファイルのパス")
			(g_strErrLogFilePath,	uh::tvalue<uh::tstring>(&strErrLogFilePath),	"[必須]エラーログファイルのパス")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if(argc == 1){
			std::cout << desc << std::endl;
			return false;
		}

		// オプションチェック
		if(vm.count(g_strFileListPath) == 0 || vm.count(g_strFileType) == 0 || vm.count(g_strCharCodeListPath) == 0 ||
			vm.count(g_strExitMode) == 0 || vm.count(g_strRunLogFilePath) == 0 || vm.count(g_strErrLogFilePath) == 0){
				std::cout << "引数の数が合いません" << std::endl;
				std::cout << desc << std::endl;
				return false;
		}
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}


CString Arguments::GetOption()
{
	CString strOption = _T("[option]\n");
	if(!strFileListPath.empty())
		strOption += GetOptionLine(g_strFileListPath, strFileListPath.c_str());

	if(!strFileType.empty())
		strOption += GetOptionLine(g_strFileType, strFileType.c_str());

	if(!strCharCodeListPath.empty())
		strOption += GetOptionLine(g_strCharCodeListPath, strCharCodeListPath.c_str());

	if(!strExitMode.empty())
		strOption += GetOptionLine(g_strExitMode, strExitMode.c_str());

	if(!strRunLogFilePath.empty())
		strOption += GetOptionLine(g_strRunLogFilePath, strRunLogFilePath.c_str());

	if(!strErrLogFilePath.empty())
		strOption += GetOptionLine(g_strErrLogFilePath, strErrLogFilePath.c_str());

	return strOption;
}


CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue)
{
	CString strOptionLine = _T("   --");
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}


bool Arguments::CheckOptionParameter()
{
	// 入力ファイルリストがなければエラー
	if(!PathFileExists(strFileListPath.c_str())){
		std::cout << "入力ファイルリスト：" << CT2A(strFileListPath.c_str()) << " がありません" << std::endl;
		return false;
	}

	// チェック対象ファイルのフォーマットが [shape] でなければエラー
	if(strFileType.c_str() != g_strSHAPE){
		std::cout << "ファイルタイプが shape ではありません" << std::endl;
		return false;
	}

	// 文字コード範囲指定ファイルがなければエラー
	if(!PathFileExists(strCharCodeListPath.c_str())){
		std::cout << "文字コード範囲指定ファイル：" << CT2A(strCharCodeListPath.c_str()) << " がありません" << std::endl;
		return false;
	}

	// 終了モードが [immediate] か [all] でなければエラー
	if(strExitMode.c_str() != g_strIMMEDIATE && strExitMode.c_str() != g_strALL){
		std::cout << "終了モードが immediate | all ではありません" << std::endl;
		return false;
	}

	// 実行ログファイルがすでにあればエラー
	if(PathFileExists(strRunLogFilePath.c_str())){
		std::cout << "実行ログファイル：" << CT2A(strRunLogFilePath.c_str()) << " はすでにあります" << std::endl;
		return false;
	}

	// エラーログファイルがすでにあればエラー
	if(PathFileExists(strErrLogFilePath.c_str())){
		std::cout << "エラーログファイル：" << CT2A(strErrLogFilePath.c_str()) << " はすでにあります" << std::endl;
		return false;
	}

	return true;
}
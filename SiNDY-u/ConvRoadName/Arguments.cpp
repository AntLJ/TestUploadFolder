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
#include <WinLib/VersionInfo.h>

namespace po = boost::program_options;

const char * const ROAD_DB = "road_db";
const char * const ROAD_OWNER = "road_owner";
const char * const NAME_DB = "name_db";
const char * const NAME_OWNER = "name_owner";
const char * const DICTIONARY = "dictionary";
const char * const OUT_DIR = "out_dir";
const char * const RUN_LOG = "run_log";
const char * const ERR_LOG = "err_log";

// コマンドライン引数の解析
bool Arguments::parse(int argc, TCHAR* argv[])
{
	po::options_description desc("How to use");

	try {
		desc.add_options()
			(ROAD_DB, uh::tvalue<uh::tstring>(&m_roadDB), "[必須]道路DB接続情報")
			(ROAD_OWNER, uh::tvalue <uh::tstring>(&m_roadOwner), "[任意]道路DBのオーナ")
			(NAME_DB, uh::tvalue<uh::tstring>(&m_nameDB), "[必須]名称DB接続情報")
			(NAME_OWNER, uh::tvalue<uh::tstring>(&m_nameOwner), "[任意]名称DBのオーナ")
			(DICTIONARY, uh::tvalue<uh::tstring>(&m_dicPath), "[必須]辞書ファイルパス")
			(OUT_DIR, uh::tvalue<uh::tstring>(&m_outDir), "[必須]データ出力ディレクトリ")
			(RUN_LOG, uh::tvalue<uh::tstring>(&m_runLog), "[必須]実行ログ")
			(ERR_LOG, uh::tvalue<uh::tstring>(&m_errLog), "[必須]エラーログ")
			;

		po::store(po::parse_command_line(argc, argv, desc), m_vm);
		po::notify(m_vm);

		// パラメータチェック
		if (!chkOptions()) {
			std::cerr << desc << std::endl;
			return false;
		}
		if (!initLog())
			return false;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}

// 引数チェック
bool Arguments::chkOptions()
{
	bool ret = true;
	if (m_roadDB.empty() ||
		m_nameDB.empty() ||
		m_dicPath.empty() ||
		m_outDir.empty() ||
		m_runLog.empty() ||
		m_errLog.empty()) {
		std::cerr << "ERROR\t必須オプションが指定されていません" << std::endl;
		return false;
	}

	ret &= fileExistChk(m_dicPath.c_str());
	ret &= dirExistChk(m_outDir.c_str());
	ret &= fileDirExistChk(m_runLog.c_str());
	ret &= fileDirExistChk(m_errLog.c_str());
	return ret;
}

// ファイル存在チェック
bool Arguments::fileExistChk(const CString& file)
{
	if (!uh::isFile(file)) {
		std::cerr << "ERROR\t" << CT2CA(file) << "が存在しません" << std::endl;
		return false;
	}
	return true;
}

// ディレクトリ存在チェック
bool Arguments::dirExistChk(const CString& dir)
{
	if (!uh::isDirectory(dir)) {
		std::cerr << "ERROR\t" << CT2CA(dir) << "が存在しません" << std::endl;
		return false;
	}
	return true;
}

// ファイル格納ディレクトリ存在チェック
bool Arguments::fileDirExistChk(const CString& filePath)
{
	long dirPos = filePath.ReverseFind(_T('\\'));
	CString dirPath = filePath.Left(dirPos);
	return dirExistChk(dirPath);
}

// ログの初期化
bool Arguments::initLog()
{
	// ログファイル作成
	if (!Log::GetInstance().SetRunLog(m_runLog.c_str()) ||
		!Log::GetInstance().SetErrLog(m_errLog.c_str())) {
		std::cerr << "ERROR\tログ作成に失敗しました" << std::endl;
		return false;
	}
	//
	Log::GetInstance().PrintLog(false, false, false, true, _T("#FREESTYLELOG"));
	Log::GetInstance().PrintLog(false, false, false, true, _T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\t"));

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	Log::GetInstance().PrintLog(true, false, true, false, GetOption());

	return true;
}

CString Arguments::GetOption() const
{
	CString strOption = _T("[option]\n");
	for (const auto& op : m_vm) {
		CString strOptionLine;
		strOptionLine.Format(_T("   --%s %s\n"), CString(op.first.c_str()), CString(op.second.as<uh::tstring>().c_str()));
		strOption += strOptionLine;
	}

	return strOption;
}
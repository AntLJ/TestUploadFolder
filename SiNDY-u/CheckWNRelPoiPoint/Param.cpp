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
#include "Param.h"

CParam::CParam(void) {
}

CParam::~CParam(void) {
}

bool CParam::checkParam(int argc, _TCHAR* argv[]) {
	po::options_description opt("オプション");

	// 書式を定義
	opt.add_options()
		("help,h", "ヘルプ表示")
		(OPT_INI_FILE, uh::tvalue<uh::tstring>(&m_tstrIniFile), "[必須]設定ファイル")
		(OPT_RUN_LOG,  uh::tvalue<uh::tstring>(&m_tstrRunLog),	"[選択]実行ログファイル")
		(OPT_ERR_LOG,  uh::tvalue<uh::tstring>(&m_tstrErrLog),	"[選択]エラーログファイル");

	// オプションの値を読み込む変数
	po::variables_map vmOpt;

	// コマンドライン引数をパースした結果を variable_mapsに格納
	po::store(po::parse_command_line(argc, argv, opt), vmOpt);
	po::notify(vmOpt);

	// 必須パラメータチェック
	if (vmOpt.count("help,h") ||
		!vmOpt.count(g_strIniFile)) {
		std::cerr << opt << std::endl;
		return false;
	}

	// 設定ファイル存在チェック
	if (!PathFileExists(CA2CT(getIniFile().c_str()))) {
		std::cerr << LOG_ERROR << "\t" << OPT_INI_FILE << " オプション不正（ファイル存在なし）" << std::endl;
		return false;
	}

	// 設定ファイルチェック
	if (!checkIni()) {
		return false;
	}

	return true;
}

bool CParam::checkIni() {
	bool bRet = true;
	std::string sMsg;

	po::options_description ini("設定ファイル項目");

	CString csDiscDistance;
	csDiscDistance.Format(_T("[選択]距離チェック用閾値 (default:%s)"), _T(DISTANCE_DEF));

	ini.add_options()
		(INI_REL_TBL,	po::value<std::string>(&m_strRelTbl),		"[必須]チェック対象関連テーブル")
		(INI_WN_DIR,	po::value<std::string>(&m_strWnDir),		"[必須]道路・歩行者ノード格納Dir")
		(INI_POI_LIST,	po::value<std::string>(&m_strPoiList),		"[必須]POIファイルリスト")
		(INI_MESHLIST,	po::value<std::string>(&m_strMeshList),		"[必須]チェック対象メッシュリスト")
		(INI_CODE_LIST,	po::value<std::string>(&m_strCodeList),		"[必須]チェック対象コンテンツコードリスト")
		(INI_NODECLASS,	po::value<std::string>(&m_strNodeClass),	"[必須]チェック対象歩行者ノード種別")
		(INI_DISTANCE,	po::value<std::string>(&m_strDistance),		CT2CA(csDiscDistance))
		(INI_PARAM,		po::value<std::string>(&m_strParam),		"[必須]世界測地系パラメータファイル");

	// 設定項目の値を読み込む変数
	po::variables_map vmIni;

	// 外部ファイルから設定項目読み込み
	std::ifstream iniFile(getIniFile().c_str());
	po::store(po::parse_config_file(iniFile, ini), vmIni);
	po::notify(vmIni);

	// 必須項目チェック
	if (!vmIni.count(INI_REL_TBL) ||
		!vmIni.count(INI_WN_DIR) ||
		!vmIni.count(INI_POI_LIST) ||
		!vmIni.count(INI_MESHLIST) ||
		!vmIni.count(INI_CODE_LIST) ||
		!vmIni.count(INI_NODECLASS) ||
		!vmIni.count(INI_PARAM)) {
		// 設定項目説明表示時の"--"と"arg"を消す
		std::stringstream	ini_tmp;
		ini_tmp << ini << std::endl;
		auto ini_kai = boost::algorithm::replace_all_copy(ini_tmp.str(), " arg ", "     ");
		ini_kai = boost::algorithm::replace_all_copy(ini_kai, "--", "");
		std::cerr << ini_kai << std::endl;
		return false;
	}

	// 関連テーブル設定チェック
	if (m_strRelTbl.empty()) {
		outputCerrItem(LOG_ERROR, INI_REL_TBL, "未設定");
		bRet = false;
	}
	// 関連テーブルファイル存在チェック
	else if (!PathFileExists(CA2CT(getRelTbl().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_REL_TBL, "ファイル存在なし");
		bRet = false;
	}

	// 道路・歩行者ノード格納Dir設定チェック
	if (m_strWnDir.empty()) {
		outputCerrItem(LOG_ERROR, INI_WN_DIR, "未設定");
		bRet = false;
	}
	// 道路・歩行者ノード格納Dir存在チェック
	else if (!PathIsDirectory(CA2CT(getWnDir().c_str()))) {
		sMsg = "Dir存在なし";
		outputCerrItem(LOG_ERROR, INI_WN_DIR, "Dir存在なし");
		bRet = false;
	}

	// POIファイルリスト設定チェック
	if (m_strPoiList.empty()) {
		outputCerrItem(LOG_ERROR, INI_POI_LIST, "未設定");
		bRet = false;
	}
	// POIファイルリスト存在チェック
	else if (!PathFileExists(CA2CT(getPoiList().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_POI_LIST, "ファイル存在なし");
		bRet = false;
	}

	// メッシュリスト設定チェック
	if (m_strMeshList.empty()) {
		outputCerrItem(LOG_ERROR, INI_MESHLIST, "未設定");
		bRet = false;
	}
	// メッシュリスト存在チェック
	else if (!PathFileExists(CA2CT(getMeshList().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_MESHLIST, "ファイル存在なし");
		bRet = false;
	}

	// コンテンツコードリスト存在チェック
	if (m_strCodeList.empty()) {
		outputCerrItem(LOG_ERROR, INI_CODE_LIST, "未設定");
		bRet = false;
	}
	// コンテンツコードリスト存在チェック
	else if (!PathFileExists(CA2CT(getCodeList().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_CODE_LIST, "ファイル存在なし");
		bRet = false;
	}

	// 歩行者ノード種別設定チェック
	if (m_strNodeClass.empty()) {
		outputCerrItem(LOG_ERROR, INI_NODECLASS, "未設定");
		bRet = false;
	}
	// 歩行者ノード種別数値チェック
	else {
		CString csWalkNodeKind(getNodeClass().c_str());
		if (csWalkNodeKind != csWalkNodeKind.SpanIncluding(CHECK_INTEGER_STR)) {
			outputCerrItem(LOG_ERROR, INI_NODECLASS, "数値以外");
			bRet = false;
		}
	}

	// 距離チェック用閾値設定チェック
	if (m_strDistance.empty()) {
		// 閾値デフォルト値設定
		m_strDistance = DISTANCE_DEF;
	}
	CString csDistance(getDistance().c_str());
	// 距離チェック用閾値数値チェック
	if (csDistance != csDistance.SpanIncluding(CHECK_DOUBLE_STR)) {
		outputCerrItem(LOG_ERROR, INI_DISTANCE, "数値以外");
		bRet = false;
	}

	// 世界測地系パラメータファイル設定チェック
	if (m_strParam.empty()) {
		outputCerrItem(LOG_ERROR, INI_PARAM, "未設定");
		bRet = false;
	}
	// 世界測地系パラメータファイル存在チェック
	else if (!PathFileExists(CA2CT(getParam().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_PARAM, "ファイル存在なし");
		bRet = false;
	}

	return bRet;
}

std::string CParam::getOption() {
	std::string strOption = "  option\n";
	strOption.append(getOptionLine(g_strIniFile, uh::toStr(m_tstrIniFile)));

	if (!m_tstrRunLog.empty()) {
		strOption.append(getOptionLine(g_strRunLog, uh::toStr(m_tstrRunLog)));
	}

	if (!m_tstrErrLog.empty()) {
		strOption.append(getOptionLine(g_strErrLog, uh::toStr(m_tstrErrLog)));
	}
	
	strOption.append("\n");
	// iniファイル設定内容出力
	strOption.append(getIniItem());

	return strOption;
}

std::string CParam::getOptionLine(const char* const cParam, std::string strValue) {
	return (boost::format("%4s--%-10s %s\n") % " " % cParam % strValue).str();
}

std::string CParam::getIniItem() {
	std::string strItem = "  iniFile Item\n";
	strItem.append(getIniItemLine(INI_REL_TBL, uh::toStr(m_strRelTbl)));
	strItem.append(getIniItemLine(INI_WN_DIR, uh::toStr(m_strWnDir)));
	strItem.append(getIniItemLine(INI_POI_LIST, uh::toStr(m_strPoiList)));
	strItem.append(getIniItemLine(INI_MESHLIST, uh::toStr(m_strMeshList)));
	strItem.append(getIniItemLine(INI_CODE_LIST, uh::toStr(m_strCodeList)));
	strItem.append(getIniItemLine(INI_NODECLASS, uh::toStr(m_strNodeClass)));
	strItem.append(getIniItemLine(INI_DISTANCE, uh::toStr(m_strDistance)));
	strItem.append(getIniItemLine(INI_PARAM, uh::toStr(m_strParam)));

	return strItem;
}

std::string CParam::getIniItemLine(const char* const cParam, std::string strValue) {
	return (boost::format("%4s%-20s = %s\n") % " " % cParam % strValue).str();
}

void CParam::outputCerrItem(std::string errKind, std::string itemName, std::string msg) {
	std::cerr << errKind <<  "\t" << itemName << " 項目不正（" << msg << "）" << std::endl;
}

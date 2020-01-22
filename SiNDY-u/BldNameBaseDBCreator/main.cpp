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

// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "LogSys.h"
#include "common.h"

#include <arctl/coinitializer.h>
#include <WinLib\/VersionInfo.h>
#include <WinLib/CStringTokenizer.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <shlwapi.h>


using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;

extern bool exec(const po::variables_map &vmContent, bool isUpdate, bool isSurvey);

void WriteOptionContent(LPCSTR option, LPCTSTR content)
{
	CString opAndCon;
	opAndCon.Format(_T("  %s:\t%s\n"), CString(option), content);
	LogSys::GetInstance().WriteProcLog(true, false, opAndCon);
}

void ProcInfoWrite(const po::variables_map &vmContent)
{
	// ツール情報の表示
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s FILVERSION:%s PRODUCTVERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion(), toolVersion.GetProductVersion());
	LogSys::GetInstance().WriteProcLog(true, false, toolInfo);
	// 設定内容の表示
	LogSys::GetInstance().WriteProcLog(true, false, _T("ini contents\n"));
	WriteOptionContent(SRC_DB, vmContent[SRC_DB].as<uh::tstring>().c_str());
	WriteOptionContent(MAP_DB, vmContent[MAP_DB].as<uh::tstring>().c_str());
	WriteOptionContent(ADDR_DB, vmContent[ADDR_DB].as<uh::tstring>().c_str());
	WriteOptionContent(TWN_DB, vmContent[TWN_DB].as<uh::tstring>().c_str());
	WriteOptionContent(POI_DB, vmContent[POI_DB].as<uh::tstring>().c_str());
	WriteOptionContent(BASE_DB, vmContent[BASE_DB].as<uh::tstring>().c_str());
	WriteOptionContent(MESH_DB, vmContent[MESH_DB].as<uh::tstring>().c_str());
	WriteOptionContent(AM_DB, vmContent[AM_DB].as<uh::tstring>().c_str());
	WriteOptionContent(SURVEY_LIST, vmContent[SURVEY_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(TGT_ADDR_LIST, vmContent[TGT_ADDR_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(SRC_PRI_LIST_PRIOR, vmContent[SRC_PRI_LIST_PRIOR].as<uh::tstring>().c_str());
	WriteOptionContent(SRC_PRI_LIST_MATCH, vmContent[SRC_PRI_LIST_MATCH].as<uh::tstring>().c_str());
	WriteOptionContent(POI_PRI_LIST, vmContent[POI_PRI_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(SUP_POI_PRI_LIST, vmContent[SUP_POI_PRI_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(BLD_EXC_LIST, vmContent[BLD_EXC_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(REPLACE_LIST, vmContent[REPLACE_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(COND_LIST, vmContent[COND_LIST].as<uh::tstring>().c_str());
	WriteOptionContent(TWN_TBL_NAME, vmContent[TWN_TBL_NAME].as<uh::tstring>().c_str());
	WriteOptionContent(SOURCENAME, vmContent[SOURCENAME].as<uh::tstring>().c_str());
	WriteOptionContent(SOURCEDATE, vmContent[SOURCEDATE].as<uh::tstring>().c_str());
}

bool WriteHowToSetIni()
{
	std::cout << "HOW to set ini_file" << std::endl;
	std::cout << "  " << SRC_DB << " = <素材DB>" << std::endl;
	std::cout << "  " << MAP_DB << " = <地図DB>" << std::endl;
	std::cout << "  " << ADDR_DB << " = <行政界DB>" << std::endl;
	std::cout << "  " << TWN_DB << " = <TWNDB>" << std::endl;
	std::cout << "  " << POI_DB << " = <POIDB>" << std::endl;
	std::cout << "  " << BASE_DB << " = <ベースDB(出力先)>" << std::endl;
	std::cout << "  " << MESH_DB << " = <メッシュDB>" << std::endl;
	std::cout << "  " << AM_DB << " = <住所マスタ格納住所DB>" << std::endl;
	std::cout << "  " << SURVEY_LIST << " = <調査ポイントリスト(出力先)>" << std::endl;
	std::cout << "  " << TGT_ADDR_LIST << " = <処理対象行政コード(5桁)リスト" << std::endl;
	std::cout << "  " << SRC_PRI_LIST_PRIOR << " = <素材優先度種別リスト(優先利用)>" << std::endl;
	std::cout << "  " << SRC_PRI_LIST_MATCH << " = <素材優先度種別リスト(マッチング)>" << std::endl;
	std::cout << "  " << POI_PRI_LIST << " = <注記POI優先度種別リスト>" << std::endl;
	std::cout << "  " << SUP_POI_PRI_LIST << " = <補完用注記POI優先度種別リスト>" << std::endl;
	std::cout << "  " << BLD_EXC_LIST << " = <処理対象外建物種別リスト>" << std::endl;
	std::cout << "  " << REPLACE_LIST << " = <置換文字列リスト>" << std::endl;
	std::cout << "  " << COND_LIST << " = <採用条件記述リスト>" << std::endl;
	std::cout << "  " << TWN_TBL_NAME << " = <townpageテーブル名>" << std::endl;
	std::cout << "  " << SOURCENAME << " = <SOURCENAME出力値>" << std::endl;
	std::cout << "  " << SOURCEDATE << " = <SOURCEDATE出力値>" << std::endl;
	return false;
}

bool option_exist_chk(const po::variables_map &vmMap, LPCSTR option)
{
	if (vmMap.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "が不正です" << std::endl;
		return false;
	}
	return true;
}

bool file_exist_chk(const po::variables_map &vmMap, LPCSTR option) 
{
	if (! PathFileExists(vmMap[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "が不正です" << std::endl;
		return false;
	}
	return true;
}

bool yyyymmddd_chk(const po::variables_map &vmMap, LPCSTR option) 
{
	CComVariant yyyymmdd = vmMap[option].as<uh::tstring>().c_str();
	yyyymmdd.ChangeType(VT_DATE);

	if (yyyymmdd.vt != VT_DATE) {
		std::cerr << "ERROR\t" << option << "が不正です" << std::endl;
		return false;
	}
	return true;
}

bool ini_check(const po::variables_map &vmContent, bool isUpdate, bool isSurvey)
{
	bool ok = true;
	// DB関連は値が入っているかのチェック・リスト関連はファイルの存在チェック
	ok &= option_exist_chk(vmContent, SRC_DB);
	ok &= option_exist_chk(vmContent, MAP_DB);
	ok &= option_exist_chk(vmContent, ADDR_DB);
	ok &= option_exist_chk(vmContent, POI_DB);
	ok &= option_exist_chk(vmContent, BASE_DB);
	if (isSurvey) {
		ok &= option_exist_chk(vmContent, MESH_DB);
		ok &= option_exist_chk(vmContent, AM_DB);
		ok &= option_exist_chk(vmContent, TWN_DB);
		// TWN_TBL_NAME, SOURCENAMEについても、値が入っているかチェック
		ok &= option_exist_chk(vmContent, TWN_TBL_NAME);
		ok &= option_exist_chk(vmContent, SOURCENAME);
		// SOURCEDATEについてはyyyy/mm/ddで入力されているかチェック
		ok &= yyyymmddd_chk(vmContent, SOURCEDATE);
	}
	ok &= file_exist_chk(vmContent, TGT_ADDR_LIST);
	ok &= file_exist_chk(vmContent, SRC_PRI_LIST_PRIOR);
	ok &= file_exist_chk(vmContent, SRC_PRI_LIST_MATCH);
	ok &= file_exist_chk(vmContent, POI_PRI_LIST);
	ok &= file_exist_chk(vmContent, SUP_POI_PRI_LIST);
	ok &= file_exist_chk(vmContent, BLD_EXC_LIST);
	ok &= file_exist_chk(vmContent, REPLACE_LIST);
	ok &= file_exist_chk(vmContent, COND_LIST);
	return ok;
}

bool arg_check(int argc, _TCHAR* argv[], po::variables_map *vmContent, bool *isUpdate, bool *isSurvey)
{
	// 初期化
	*isUpdate = false;
	*isSurvey = false;
	// ロケール関連の不具合防止
	uh::setJapaneseCharacterTypeFacet();
	// 引数読み込み
	uh::tstring iniFile, procLog, errLog;
	po::options_description descPath("How to user");
	descPath.add_options()
		(INI, uh::tvalue<uh::tstring>(&iniFile), "設定ファイル(必須)")
		(UPDATE, "ベースDB更新モード(任意)")
		(SURVEY, "調査対象ポイント出力モード(任意)")
		(PROCLOG, uh::tvalue<uh::tstring>(&procLog), "実行ログ(任意)")
		(ERRLOG, uh::tvalue<uh::tstring>(&errLog), "エラーログ(任意)");
	if (1 == argc) {
		std::cout << descPath << std::endl;
		return WriteHowToSetIni();
	}

	po::variables_map vmPath;
	po::store(po::parse_command_line(argc, argv, descPath), vmPath);
	po::notify(vmPath);

	// 設定ファイルパスチェック
	if (! PathFileExists(iniFile.c_str())) {
		std::cerr << "ERROR\t" << INI << "オプションが不正です" << std::endl;
		return false;
	}
	// モードのチェック
	if ((!vmPath.count(UPDATE)) && (!vmPath.count(SURVEY))) {
		std::cerr << "ERROR\t" << UPDATE << "オプション or " << SURVEY << "オプションを指定してください" << std::endl;
		return false;
	}
	if (vmPath.count(UPDATE))
		*isUpdate = true;
	if (vmPath.count(SURVEY))
		*isSurvey = true;

	// 実行ログ・エラーログのチェック
	if (vmPath.count(PROCLOG) != 0) {
		if (! LogSys::GetInstance().SetProcLog(procLog.c_str())) {
			std::cerr << "ERROR\t" << PROCLOG << "オプションが不正です" << std::endl;
			return false;
		}
	}
	if (vmPath.count(ERRLOG) != 0) {
		if (! LogSys::GetInstance().SetErrLog(errLog.c_str())) {
			std::cerr << "ERROR\t" << ERRLOG << "オプションが不正です" << std::endl;
			return false;
		}
		// SiNDYFreeStyelLog用のヘッダ記入
		LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
		LogSys::GetInstance().WriteErrLog(false, false, _T("FLAG\tLAYER\tOBJECTID\tエラーレベル\tエラーメッセージ\n"));
	}
	// 設定ファイル読み込み
	po::options_description descContent("How to set a ini file");
	descContent.add_options()
		(SRC_DB, uh::tvalue<uh::tstring>(), "素材DB")
		(MAP_DB, uh::tvalue<uh::tstring>(), "地図DB")
		(ADDR_DB, uh::tvalue<uh::tstring>(), "行政界DB")
		(TWN_DB, uh::tvalue<uh::tstring>(), "TWNDB")
		(POI_DB, uh::tvalue<uh::tstring>(), "POIDB")
		(BASE_DB, uh::tvalue<uh::tstring>(), "ベースDB(出力先)")
		(MESH_DB, uh::tvalue<uh::tstring>(), "メッシュDB")
		(AM_DB, uh::tvalue<uh::tstring>(), "住所マスタ格納住所DB")
		(SURVEY_LIST, uh::tvalue<uh::tstring>(), "調査ポイントリスト(出力先)")
		(TGT_ADDR_LIST, uh::tvalue<uh::tstring>(), "処理対象行政コード(5桁)リスト")
		(SRC_PRI_LIST_PRIOR, uh::tvalue<uh::tstring>(), "素材優先度種別リスト(優先利用)")
		(SRC_PRI_LIST_MATCH, uh::tvalue<uh::tstring>(), "素材優先度種別リスト(マッチング)")
		(POI_PRI_LIST, uh::tvalue<uh::tstring>(), "注記POI優先度種別リスト")
		(SUP_POI_PRI_LIST, uh::tvalue<uh::tstring>(), "補完用注記POI優先度種別リスト")
		(BLD_EXC_LIST, uh::tvalue<uh::tstring>(), "処理対象外建物種別リスト")
		(REPLACE_LIST, uh::tvalue<uh::tstring>(), "置換文字列リスト")
		(COND_LIST, uh::tvalue<uh::tstring>(), "採用条件記述リスト")
		(TWN_TBL_NAME, uh::tvalue<uh::tstring>(), "townpageテーブル名称")
		(SOURCENAME, uh::tvalue<uh::tstring>(), "SOURCENAME出力値")
		(SOURCEDATE, uh::tvalue<uh::tstring>(), "SOURCEDATE出力値");

	std::ifstream ifsIniFile(CT2CA(iniFile.c_str()));
	if (! ifsIniFile.is_open()) {
		std::cerr << "ERROR\t設定ファイルのオープンに失敗しました" << std::endl;
		return false;
	}
	po::store(po::parse_config_file(ifsIniFile, descContent), *vmContent);
	po::notify(*vmContent);

	if (! ini_check(*vmContent, *isUpdate, *isSurvey))
		return WriteHowToSetIni();
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;
		// 引数取得&チェック
		po::variables_map vmContent;
		bool isUpdate = false, isSurvey = false;
		if (! arg_check(argc, argv, &vmContent, &isUpdate, &isSurvey)) {
			retval = 1;
		} else {
			// 処理実行
			LogSys::GetInstance().WriteProcLog(true, true, _T("処理開始\n"));
			if (! exec(vmContent, isUpdate, isSurvey))
				retval = 1;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
		retval = 2;
	}
	if (retval == 0) {
		LogSys::GetInstance().WriteProcLog(true, true, _T("正常終了\n"));
	} else {
		LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
	}
	std::cout << std::flush;
	std::cerr << std::flush;
	return retval;
}

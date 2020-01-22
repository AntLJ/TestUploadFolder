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
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

#include <boost/algorithm/string/classification.hpp> 
#include <boost/algorithm/string/split.hpp>

#include "TimeUtil.h"
#include "LogUtil.h"

using namespace std;
using namespace uh;
namespace po = boost::program_options;

const uh::tstring Arguments::PS	=	_T("PS");
const uh::tstring Arguments::AT	=	_T("AT");
const uh::tstring Arguments::KK	=	_T("KK");
const uh::tstring Arguments::IPC	=	_T("IPC");

Arguments::Arguments() : 
	survey_dir(_T(""))
	,import_db(_T(""))
	,company(_T(""))
	,lot(_T(""))
	,log_dir(_T(".\\"))	
	,crd_param(_T("\\\\win\\tdc\\SiNDY\\config\\TKY2JGD.par"))
	,unuse_version(false)
	,skip_delete_record(false)
{
	//日付を設定
	time_t now = time(NULL);
	struct tm pnow;
	localtime_s(&pnow, &now);

	date = TimeUtil::GetTime();
}


// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const SURVERY_DIR		=	"survey_dir";
	const char * const IMPORT_DB		=	"import_db";
	const char * const COMPANY			=	"company";
	const char * const DATA				=	"date";
	const char * const LOT				=	"lot";
	const char * const LOG_DIR			=	"log_dir";
	const char * const CRD_PARAM		=	"crd_param";
	const char * const UNUSE_VERSION	=	"unuse_version";
	const char * const SKIP_DELETE_RECORD = "skip_delete_record";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(SURVERY_DIR,	tvalue<tstring>(&survey_dir	),	"[必須]インポートディレクトリ"			)
			(IMPORT_DB,		tvalue<tstring>(&import_db	),	"[必須]インポート先DB接続情報"			)
			(COMPANY,		tvalue<tstring>(&company	),	"[必須]データ作成元"					)
			(DATA,			tvalue<tstring>(&date		),	"[必須]データ受領日"					)
			(LOT,			tvalue<tstring>(&lot		),	"[必須]ロット名"					)
			(LOG_DIR,		tvalue<tstring>(&log_dir	),	"[必須]ログ出力先フォルダパス"			)
			(CRD_PARAM,		tvalue<tstring>(&crd_param	),	"[任意]座標変換用パラメータファイル"	)
			(UNUSE_VERSION,									"[任意]バージョン未使用(編集開始～終了しない)"	)
			(SKIP_DELETE_RECORD,							"[任意]更新コード「削除」のレコードをスキップするか"	)
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= CheckParam( SURVERY_DIR,	survey_dir	);
		good &= CheckParam( IMPORT_DB,		import_db	);
		good &= CheckParam( COMPANY,		company		);
		good &= CheckParam( DATA,			date		);
		good &= CheckParam( LOT,			lot			);
		good &= CheckParam( LOG_DIR,		log_dir		);

		if( vm.count(UNUSE_VERSION) != 0 )
		{
			unuse_version = true;
		}
		if( vm.count(SKIP_DELETE_RECORD) != 0 )
		{
			skip_delete_record = true;
		}

		if( ! good ){
			LogUtil::printError(desc);
			return false;
		}

		if(!CheckParamCompany(company))
		{
			LogUtil::printError(desc);
			LogUtil::printError(70, "データ作成元\t入力パラメータが不正です");
			return false;
		}

		if(!CheckParamDate(date))
		{
			LogUtil::printError(desc);
			LogUtil::printError(71, "データ受領日\t入力パラメータが不正です");
			return false;
		}
	}
	catch(const std::exception& e){
		LogUtil::printError(desc);
		LogUtil::printError("異常終了しました");
		LogUtil::printError("例外検出\t" +  uh::toStr(e.what()));
		return false;
	}

	//パラメータ出力
	LogUtil::print("[入力パラメータ]");
	LogUtil::printParam(uh::toTStr(SURVERY_DIR),		survey_dir	);
	LogUtil::printParam(uh::toTStr(IMPORT_DB),			import_db	);
	LogUtil::printParam(uh::toTStr(COMPANY),			company		);
	LogUtil::printParam(uh::toTStr(DATA),				date		);
	LogUtil::printParam(uh::toTStr(LOT),				lot			);
	LogUtil::printParam(uh::toTStr(LOG_DIR),			log_dir		);
	LogUtil::printParam(uh::toTStr(CRD_PARAM),			crd_param	);
	LogUtil::printParam(uh::toTStr(UNUSE_VERSION),		unuse_version ? _T("バージョン利用しない")  : _T("バージョン利用する"));
	LogUtil::printParam(uh::toTStr(SKIP_DELETE_RECORD), skip_delete_record ? _T("更新コード「削除」のレコードをスキップする")  : _T("更新コード「削除」のレコードをスキップしない"));

	return true;
}

bool Arguments::CheckParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ )
{
	if( optValue.empty() ){	
		cerr << optName << "オプションは必須です" << endl;
		return false;
	}

	if( bFile ){
		if( ! isFile( optValue.c_str() )){
			cerr << toStr( optValue ) << "はファイルではありません" << endl;
			return false;
		}
	}

	return true;
}

bool Arguments::CheckParamCompany(const uh::tstring& optValue)
{
	bool ret = false;

	if(optValue == PS)
	{
		ret = true;
	}
	else if(optValue == AT)
	{
		ret = true;
	}
	else if(optValue == KK)
	{
		ret = true;
	}
	else if(optValue == IPC)
	{
		ret = true;
	}

	return ret;
}

bool Arguments::CheckParamDate(const uh::tstring& optValue)
{
	bool ret = true;

	//ひとまず 文字列4文字 + "/" + 文字列2文字 + "/" + 文字列2文字 をチェック

	// "/"で分割
	std::vector<uh::tstring> splitValue;
	boost::algorithm::split(splitValue, optValue, boost::is_any_of("/"));

	if(splitValue.size() != 3)
	{
		return false;
	}

	//文字列数をチェック
	if(splitValue.at(0).size() != 4)
	{
		return false;
	}
	else
	{
		//範囲チェック
		int intValue = std::atoi(uh::toStr(splitValue.at(0)).c_str());
		if(intValue < 1990 || intValue > 2100)
		{
			return false;
		}
	}

	if(splitValue.at(1).size() != 2)
	{
		return false;
	}
	else
	{
		//範囲チェック
		int intValue = std::atoi(uh::toStr(splitValue.at(1)).c_str());
		if(intValue < 0 || intValue > 12)
		{
			return false;
		}
	}

	if(splitValue.at(2).size() != 2)
	{
		return false;
	}
	else
	{
		//範囲チェック
		int intValue = std::atoi(uh::toStr(splitValue.at(2)).c_str());
		if(intValue < 1 || intValue > 31)
		{
			return false;
		}
	}

	return true;
}
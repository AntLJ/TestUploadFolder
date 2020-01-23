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
#include "TransParam.h"

TransParam::TransParam(void)
{
}


TransParam::~TransParam(void)
{
}

bool TransParam::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");
	desc.add_options()
		(REPDB, uh::tvalue<uh::tstring>(&m_repDB), "変換元イラストリンクレプリカ接続情報(必須)")
		(REPSCHEMA, uh::tvalue<uh::tstring>(&m_repSchema), "変換元イラストテーブル格納スキーマ(必須)")
		(ILLSET, uh::tvalue<uh::tstring>(&m_illSetFile), "イラスト処理情報設定ファイルパス(必須)")
		(INPUTLIST, uh::tvalue<uh::tstring>(&m_inputList), "イラストファイル名対応表パス(必須)")
		(EGGDB, uh::tvalue<uh::tstring>(&m_eggDB), "変換先EGG接続情報(必須)")
		(EGGSCHEMA, uh::tvalue<uh::tstring>(&m_eggSchema), "変換先EGG出力スキーマ(必須)")
		(RUNLOG, uh::tvalue<uh::tstring>(&m_runLog), "実行ログ(必須)")
		(ERRLOG, uh::tvalue<uh::tstring>(&m_errLog), "エラーログ(必須)");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// 引数が無い場合は、使い方を出力
	if (1 == argc) {
		std::cout << desc << std::endl;
		return false;
	}

	// 必須オプションの有無チェック
	if (! OptionExistChk(vm, REPDB) || ! OptionExistChk(vm, REPSCHEMA) || 
		! OptionExistChk(vm, ILLSET) || ! OptionExistChk(vm, INPUTLIST) ||
		! OptionExistChk(vm, EGGDB) || ! OptionExistChk(vm, EGGSCHEMA) ||
		! OptionExistChk(vm, RUNLOG) || ! OptionExistChk(vm, ERRLOG)) {
			std::cout << desc << std::endl;
			return false;
	}
	// ファイルパスパスチェック
	if (! FileExistChk(ILLSET, m_illSetFile.c_str()) || ! FileExistChk(INPUTLIST, m_inputList.c_str())) {
		std::cout << desc << std::endl;
		return false;
	}
	return true;

}

std::string TransParam::GetOptions()
{
	std::string options = "[option]\n";
	options.append(GetOptionLine(REPDB, m_repDB));
	options.append(GetOptionLine(REPSCHEMA, m_repSchema));
	options.append(GetOptionLine(ILLSET, m_illSetFile));
	options.append(GetOptionLine(INPUTLIST, m_inputList));
	options.append(GetOptionLine(EGGDB, m_eggDB));
	options.append(GetOptionLine(EGGSCHEMA, m_eggSchema));
	options.append(GetOptionLine(RUNLOG, m_runLog));
	options.append(GetOptionLine(ERRLOG, m_errLog));
	return options;
}

bool TransParam::OptionExistChk(const po::variables_map& vm, LPCSTR option)
{
	if (vm.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "オプションが設定されていません" << std::endl;
		return false;
	}
	return true;
}

bool TransParam::FileExistChk(LPCSTR option, LPCTSTR path)
{
	if (! PathFileExists(path)){
		std::cerr << "ERROR\t" << option << "オプションが不正です" << std::endl;
		return false;
	}
	return true;
}

std::string TransParam::GetOptionLine(LPCSTR option, uh::tstring content)
{
	std::string optionLine = "  --";
	optionLine.append(option);
	if (0 != content.length()) {
		optionLine.append("\t");
		optionLine.append(uh::toStr(content));
	}
	optionLine.append("\n");

	return optionLine;
}

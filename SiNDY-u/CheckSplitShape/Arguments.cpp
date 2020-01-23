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
#include <iostream>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <regex>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input = "input";

	po::options_description desc("使い方");

	try
	{
		desc.add_options()
			("help,h", "ヘルプを表示")
			(g_strBeforeShpPath,         uh::tvalue<uh::tstring>(&BeforeShpPath),     "[必須]変換前シェープファイルディレクトリ")
			(g_strBeforeDBPath,          uh::tvalue<uh::tstring>(&BeforeDBPath),      "[選択]変換前DB (高さノード分割処理時に必須)")
			(g_strAfterShpPath,          uh::tvalue<uh::tstring>(&AfterShpPath),      "[必須]変換後シェープファイルディレクトリ")
			(g_strMeshDBPath,            uh::tvalue<uh::tstring>(&MeshDBPath),        "[選択]メッシュ形状用のDB（高さノード分割時は必須）")
			(g_strMeshSchema,            uh::tvalue<uh::tstring>(&MeshSchema),        "[選択]メッシュ形状用のDBスキーマ名")
			(g_strMeshlistPath,          uh::tvalue<uh::tstring>(&MeshlistPath),      "[必須]メッシュリスト")
			(g_strCheckMode,             uh::tvalue<uh::tstring>(&CheckMode),         "[必須]チェックモード (1=リンク内属性リンク化処理, 2=歩行者ノード分割・歩行者リンクマージ処理, 3=高さノード分割処理)")
			(g_strTky2JgdPath,           uh::tvalue<uh::tstring>(&Tky2JgdPath),       "[選択]地域パラメータパス (高さノード分割処理時に選択可)")
			(g_strMinSegment,            uh::tvalue<uh::tstring>(&MinSegment),        "[必須]微小セグメント長 (単位[m])")
			(g_strRunlog,                uh::tvalue<uh::tstring>(&RunLogPath),        "[必須]実行ログ出力先")
			(g_strErrlog,                uh::tvalue<uh::tstring>(&ErrLogPath),        "[必須]エラーログ出力先")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if(1 == argc) {
			std::cout << desc << std::endl;
			return false;
		}

		// 必須パラメータのチェック
		bool bAllow = true;
		if(0 == vm.count(g_strBeforeShpPath) ||
			0 == vm.count(g_strAfterShpPath) ||
			0 == vm.count(g_strMeshlistPath) ||
			0 == vm.count(g_strCheckMode) ||
			0 == vm.count(g_strMinSegment) ||
			0 == vm.count(g_strRunlog) ||
			0 == vm.count(g_strErrlog)){
				std::cout << "必須パラメータを指定してください" << std::endl;
				std::cout << desc << std::endl;
				return false;
		}
		// 高さノード分割時は「mesh_db」オプションは必須
		if(0 == CheckMode.compare(_T("3")) && 0 == vm.count(g_strMeshDBPath)){
			std::cout << "高さノード分割時処理時に「mesh_db」オプションは必須" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}
		// 高さノード分割時は「before_db」オプションは必須
		if(0 == CheckMode.compare(_T("3")) && 0 == vm.count(g_strBeforeDBPath)){
			std::cout << "高さノード分割処理時に[before_db]オプションは必須" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}
		if(0 != CheckMode.compare(_T("3")) && 0 < vm.count(g_strBeforeDBPath)){
			std::cout << "高さノード処理以外は[before_db]オプションの指定は不要" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}

		// 高さノード分割時は「tky2jgd_path」オプションは選択制、それ以外はNG
		if(0 != CheckMode.compare(_T("3")) && 0 < vm.count(g_strTky2JgdPath)){
			std::cout << "高さノード処理以外は[tky2jgd_path]オプションの指定は不要" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}

		// チェックモードの値は「1,2,3」のみ有効
		if(0 != CheckMode.compare(_T("1")) &&
			0 != CheckMode.compare(_T("2")) &&
			0 != CheckMode.compare(_T("3")) &&
			0 != CheckMode.compare(_T("99"))){
				std::cout << "チェックモードは[1=リンク内属性リンク化処理, 2=歩行者ノード分割・歩行者リンクマージ処理, 3=高さノード分割処理]のみ指定可能" << std::endl;
				std::cout << desc << std::endl;
				return false;
		}

		// 微小セグメント長は数値のみ許容
		std::wregex reg(L"\\d+.\\d+");
		if(!std::regex_match(MinSegment.c_str(), reg)){
			std::cout << "微小セグメント長は数値のみ指定可能" << std::endl;
			std::cout << desc << std::endl;
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

CString Arguments::GetOption()
{
	CString strOption = _T("[option]\n");
	if(!BeforeShpPath.empty() )
		strOption += GetOptionLine(g_strBeforeShpPath, BeforeShpPath.c_str());

	if(!BeforeDBPath.empty() )
		strOption += GetOptionLine(g_strBeforeDBPath, BeforeDBPath.c_str());

	if(!AfterShpPath.empty() )
		strOption += GetOptionLine(g_strAfterShpPath, AfterShpPath.c_str());

	if(!MeshDBPath.empty() )
		strOption += GetOptionLine(g_strMeshDBPath, MeshDBPath.c_str());

	if(!MeshSchema.empty() )
		strOption += GetOptionLine(g_strMeshSchema, MeshSchema.c_str());

	if(!MeshlistPath.empty() )
		strOption += GetOptionLine(g_strMeshlistPath, MeshlistPath.c_str());

	if(!CheckMode.empty())
		strOption += GetOptionLine(g_strCheckMode, CheckMode.c_str());

	if(!Tky2JgdPath.empty())
		strOption += GetOptionLine(g_strTky2JgdPath, Tky2JgdPath.c_str());

	if(!MinSegment.empty())
		strOption += GetOptionLine(g_strMinSegment, MinSegment.c_str());

	if(!RunLogPath.empty())
		strOption += GetOptionLine(g_strRunlog, ErrLogPath.c_str());

	if(!ErrLogPath.empty())
		strOption += GetOptionLine(g_strErrlog, RunLogPath.c_str());

	return strOption;
}

CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue)
{
	CString strOptionLine = _T("   --");
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}

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

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("使い方");

	try
	{
		desc.add_options()
			(g_strShapeDir,         uh::tvalue<uh::tstring>(&strShapeDir),        "[必須]シェープファイルディレクトリ")
			(g_strMeshlist,         uh::tvalue<uh::tstring>(&strMeshlist),        "[必須]メッシュリストのパス")
			(g_strSpatialDB,        uh::tvalue<uh::tstring>(&strSpatialDB),       "[必須] 空間参照取得用DBパス")
			(g_strSpatialDBSchema,  uh::tvalue<uh::tstring>(&strSpatialDBSchema), "[選択] 空間参照取得用DBのスキーマ（PostgreSQL用）")
			(g_strRunLog,           uh::tvalue<uh::tstring>(&strRunLog),          "[必須] 実行ログパス")
			(g_strErrLog,           uh::tvalue<uh::tstring>(&strErrLog),          "[必須] エラーログパス")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		if(strShapeDir.empty()){
			cerr << g_strShapeDir << "オプションは必須です" << endl;
			good = false;
		} else {
			if(!isDirectory(strShapeDir.c_str())){
				cerr << toStr(strShapeDir) << "はディレクトリではありません" << endl;
				good = false;
			}
		}
		if(strMeshlist.empty()){
			cerr << g_strMeshlist << "オプションは必須です" << endl;
			good = false;
		} else {
			if(!isFile(strMeshlist.c_str())){
				cerr << toStr(strMeshlist) << "はファイルではありません" << endl;
				good = false;
			}
		}
		if(strSpatialDB.empty()){
			cerr << g_strSpatialDB << "オプションは必須です" << endl;
			good = false;
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

CString Arguments::GetOption()
{
	CString strOption = _T("[option]\n");
	if(!strShapeDir.empty() )
		strOption += GetOptionLine(g_strShapeDir, strShapeDir.c_str());

	if(!strMeshlist.empty() )
		strOption += GetOptionLine(g_strMeshlist, strMeshlist.c_str());

	if(!strSpatialDB.empty() )
		strOption += GetOptionLine(g_strSpatialDB, strSpatialDB.c_str());

	if(!strSpatialDBSchema.empty() )
		strOption += GetOptionLine(g_strSpatialDBSchema, strSpatialDBSchema.c_str());

	if(!strRunLog.empty() )
		strOption += GetOptionLine(g_strRunLog, strRunLog.c_str());

	if(!strErrLog.empty() )
		strOption += GetOptionLine(g_strErrLog, strErrLog.c_str());

	return strOption;
}

CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue)
{
	CString strOptionLine = _T("   --");
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}


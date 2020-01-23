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
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_point_con =   "point-con";
	const char * const k_poly_con =    "poly-con";
	const char * const k_point =       "point";
	const char * const k_poly =        "polygon";
	const char * const k_pointW =      "point-where";
	const char * const k_polyW =       "poly-where";
	const char * const k_runlog =      "runlog,r";
	const char * const k_errlog =      "errlog,e";
	

	po::options_description desc("How to use");

	try
	{
		desc.add_options()
			(k_point_con,   tvalue<tstring>(&m_PointConnect), "[必須]ポイントの接続先")
			(k_poly_con,    tvalue<tstring>(&m_PolyConnect),  "[必須]ポリゴンの接続先")
			(k_point,       tvalue<tstring>(&m_Point),        "[必須]チェック対象のポイントのフィーチャ名")
			(k_poly,        tvalue<tstring>(&m_Poly),         "[必須]チェック対象のポリゴンのフィーチャ名")
			(k_pointW,      tvalue<tstring>(&m_PointWhere),   "[任意]対象ポイントを絞るWHERE句")
			(k_polyW,       tvalue<tstring>(&m_PolyWhere),    "[任意]対象ポリゴンを絞るWHERE句")
			(k_runlog,      tvalue<tstring>(&m_RunLog),       "[必須]実行ログ")
			(k_errlog,      tvalue<tstring>(&m_ErrLog),       "[必須]エラーログ")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		if(m_PointConnect.empty() || m_PolyConnect.empty())
		{
			cerr << "接続先を指定してください" << endl;
			good = false;
		}

		if(m_RunLog.empty() || m_ErrLog.empty())
		{
			cerr << "ログファイルを指定してください" << endl;
			good = false;
		}

		if(m_Point.empty())
		{
			cerr << k_point << "オプションは必須です" << endl;
			good = false;
		}

		if(m_Poly.empty())
		{
			cerr << k_poly << "オプションは必須です" << endl;
			good = false;
		}

		if(! good)
		{
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

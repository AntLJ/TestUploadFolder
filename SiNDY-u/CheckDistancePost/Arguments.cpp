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

/**
 * @class CArguments
 * @brief コマンドライン引数クラス
 * @file  Arguments.cpp
 * @author ビットエイジ　秦
 * $Id$
*/
#include "StdAfx.h"
#include <fstream>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include "Arguments.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy
{
	using namespace std;
	using namespace uh;

	namespace po = boost::program_options;

	const char * k_db =         "db";                ///< 処理DB
	const char * k_roadlist =   "road_list";         ///< 対象路線一覧ファイルパス
	const char * k_pitch =      "pitch";             ///< ピッチ
	const char * k_outdir =     "out_dir";           ///< 作業ログ出力パス
	const char * k_roadclass =  "roadclass_cost";    ///< 対象道路種別コスト定義ファイルパス
	const char * k_linkclass =  "linkclass_cost";    ///< 対象リンク種別コスト定義ファイルパス
	const char * k_buffer =     "getlink_buffer";    ///< リンク特定バッファ
	const char * k_range =      "permissible_range"; ///< 許容範囲
	const char * k_separate =   "separate_range";    ///< 2条分離許範囲

	// コマンドライン引数の解析
	bool CArguments::parse(int argc, _TCHAR* argv[])
	{
		po::options_description desc("How to use");

		try
		{
			desc.add_options()
				(k_db,		   tvalue<tstring>(&m_strDb),        "[必須]入力パラメータ 処理DB")
				(k_roadlist,   tvalue<tstring>(&m_strList),      "[必須]入力パラメータ 対象路線一覧ファイルパス")
				(k_pitch,      tvalue<tstring>(&m_strPitch),     "[必須]入力パラメータ ピッチ[10.0/1.0/0.1]km")
				(k_outdir,     tvalue<tstring>(&m_strLogPath),   "[必須]入力パラメータ 作業ログ出力パス")
				(k_roadclass,  tvalue<tstring>(&m_strRoadCost),  "[必須]入力パラメータ 対象道路種別コスト定義ファイルパス")
				(k_linkclass,  tvalue<tstring>(&m_strLinkCost),  "[必須]入力パラメータ 対象リンク種別コスト定義ファイルパス")
				(k_buffer,     tvalue<tstring>(&m_strBuf),       "[必須]入力パラメータ リンク特定バッファ[m]")
				(k_range,      tvalue<tstring>(&m_strRange),     "[必須]入力パラメータ 許容範囲[%]")
				(k_separate,   tvalue<tstring>(&m_strSeparate),  "[必須]入力パラメータ 2条分離許範囲[%]")
				;

			po::variables_map vm;
			po::store(po::parse_command_line(argc, argv, desc), vm);
			po::notify(vm);

			// 必須パラメータのチェック
			bool good = true;

			// 処理DB
			if(m_strDb.empty())
			{
				cerr << k_db << "オプションは必須です" << endl;
				good = false;
			}

			// 対象路線一覧ファイルパス
			if(m_strList.empty())
			{
				cerr << k_roadlist << "オプションは必須です" << endl;
				good = false;
			}
			else
			{
				if(! isFile(m_strList.c_str()))
				{
					cerr << toStr(m_strList) << "はファイルではありません" << endl;
					good = false;
				}
			}

			// ピッチ
			if(m_strPitch.empty())
			{
				cerr << k_pitch << "オプションは必須です" << endl;
				good = false;
			}

			// 作業ログ出力パス
			if(m_strLogPath.empty())
			{
				cerr << k_outdir << "オプションは必須です" << endl;
				good = false;
			}

			// 対象道路種別コスト定義ファイルパス
			if(m_strRoadCost.empty())
			{
				cerr << k_roadclass << "オプションは必須です" << endl;
				good = false;
			}
			else
			{
				if(!isFile(m_strRoadCost.c_str()))
				{
					cerr << toStr(m_strRoadCost) << "はファイルではありません" << endl;
					good = false;
				}
			}

			// 対象リンク種別コスト定義ファイルパス
			if(m_strLinkCost.empty())
			{
				cerr << k_linkclass << "オプションは必須です" << endl;
				good = false;
			}
			else
			{
				if(!isFile(m_strLinkCost.c_str()))
				{
					cerr << toStr(m_strLinkCost) << "はファイルではありません" << endl;
					good = false;
				}
			}

			// リンク特定バッファ
			if(m_strBuf.empty())
			{
				cerr << k_buffer << "オプションは必須です" << endl;
				good = false;
			}

			// 許容範囲
			if(m_strRange.empty())
			{
				cerr << k_range << "オプションは必須です" << endl;
				good = false;
			}

			// 2条分離許容距離
			if(m_strSeparate.empty())
			{
				cerr << k_separate << "オプションは必須です" << endl;
				good = false;
			}

			GetOptionParam();

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

	// オプション引数一覧作成
	void CArguments::GetOptionParam()
	{
		CString strRet, strTmp;

		strRet  = _T("#  option\n");

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_db).c_str(), m_strDb.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_roadlist).c_str(), m_strList.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_pitch).c_str(), m_strPitch.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_outdir).c_str(), m_strLogPath.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_roadclass).c_str(), m_strRoadCost.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_linkclass).c_str(), m_strLinkCost.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_buffer).c_str(), m_strBuf.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_range).c_str(), m_strRange.c_str());
		strRet += strTmp;

		strTmp.Format(_T("#    -%s %s\n"), toTStr(k_separate).c_str(), m_strSeparate.c_str());
		strRet += strTmp;

		m_strOpParam = strRet;
	}
}

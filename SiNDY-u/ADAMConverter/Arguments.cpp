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
#include <WinLib/VersionInfo.h>
#include <iostream>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

const char * const k_db_info = "db_info";
const char * const k_conv_table_path = "conv_table_path";
const char * const k_conv_field_path = "conv_field_path";
const char * const k_cnv_param_path = "cnv_param_path";
const char * const k_output_dir = "output_dir";
const char * const k_output_header = "output_header";
const char * const k_output_delimiter = "output_delimiter";
const char * const k_output_encode = "output_encode";
const char * const k_output_newline = "output_newline";
const char * const k_output_georef = "output_georef";
const char * const k_output_shapetype = "output_shapetype";
const char * const k_output_polygontype = "output_polygontype";
const char * const k_runlog_path = "runlog_path";
const char * const k_errlog_path = "errlog_path";

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try {
		desc.add_options()
			(k_db_info,				tvalue<tstring>(&m_db_info),			"[必須]DB接続情報")
			(k_conv_table_path,		tvalue<tstring>(&m_conv_table_path),	"[必須]変換テーブルのパス")
			(k_conv_field_path,		tvalue<tstring>(&m_conv_field_path),	"[必須]変換フィールド対応表のパス")
			(k_cnv_param_path,		tvalue<tstring>(&m_cnv_param_path),		"[必須]座標変換用パラメータファイル")
			(k_output_dir,			tvalue<tstring>(&m_output_dir),			"[必須]データ出力ディレクトリ")
			(k_output_header,												"[選択]出力データにヘッダ出力を行う")
			(k_output_delimiter,	tvalue<tstring>(&m_output_delimiter),	"[必須]出力データのフォーマット (comma,tab)")
			(k_output_encode,		tvalue<tstring>(&m_output_encode),		"[必須]出力データの文字コード (UTF-8,SHIFT_JIS)")
			(k_output_newline,		tvalue<tstring>(&m_output_newline),		"[必須]出力データの改行コード (CR,LF,CRLF)")
			(k_output_georef,		tvalue<tstring>(&m_output_georef),		"[必須]出力データの測地系 (4301,4612,6668)")
			(k_output_shapetype,	tvalue<tstring>(&m_output_shapetype),	"[必須]出力データの形状タイプ (NORMAL,WKT)")
			(k_runlog_path,			tvalue<tstring>(&m_runlog_path),		"[必須]実行ログ")
			(k_errlog_path,			tvalue<tstring>(&m_errlog_path),		"[必須]エラーログ")
			;

		po::store(po::parse_command_line(argc, argv, desc), m_vm);
		po::notify(m_vm);

		// 必須パラメータのチェック
		bool good = true;
		if(m_db_info.empty() ||
			m_conv_table_path.empty() ||
			m_conv_field_path.empty() ||
			m_cnv_param_path.empty() ||
			m_output_dir.empty() ||
			m_output_delimiter.empty() ||
			m_output_encode.empty() ||
			m_output_newline.empty() ||
			m_output_georef.empty() ||
			m_output_shapetype.empty() ||
			m_runlog_path.empty() ||
			m_errlog_path.empty()){
				std::cerr << "必須オプションが指定されていません" << std::endl;
				good = false;
		} else {
			// 変換テーブルの存在確認
			if(!PathFileExists(m_conv_table_path.c_str())){
				std::cerr << k_conv_table_path << "オプション : 「" << toStr(m_conv_table_path) << "」のファイルは存在しません" << std::endl;
				good = false;
			}
			// 変換フィールド対応表の存在確認
			if(!PathFileExists(m_conv_field_path.c_str())){
				std::cerr << k_conv_field_path << "オプション : 「" << toStr(m_conv_field_path) << "」のファイルは存在しません" << std::endl;
				good = false;
			}
			// 測地系変換パラメータの存在確認
			if(!PathFileExists(m_cnv_param_path.c_str())){
				std::cerr << k_cnv_param_path << "オプション : 「" << toStr(m_cnv_param_path) << "」のファイルは存在しません" << std::endl;
				good = false;
			}
			// 出力先ディレクトリの存在確認
			if(!PathIsDirectory(m_output_dir.c_str())){
				std::cerr << k_output_dir << "オプション : 「" << toStr(m_output_dir) << "」のディレクトリは存在しません" << std::endl;
				good = false;
			}
			// ヘッダ出力有無設定
			if(m_vm.count(k_output_header)){
				m_output_header = true;
			}
			// ログ初期化
			if(good){
				if(!initLog(m_runlog_path.c_str(), m_errlog_path.c_str())){
					good = false;
				}
			}
		}
		if(!good){
			std::cerr << desc << std::endl;
			return false;
		}
	} catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}

CString Arguments::GetOption() const
{
	CString strOption = _T("[option]\n");
	for(const auto& op : m_vm){
		CString strOptionLine;
		if(op.first == k_output_header){
			strOptionLine.Format(_T("   --%s\n"), CString(op.first.c_str()));
		} else {
			strOptionLine.Format(_T("   --%s %s\n"), CString(op.first.c_str()), CString(op.second.as<uh::tstring>().c_str()));
		}
		strOption += strOptionLine;
	}

	return strOption;
}

CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue) const
{
	CString strOptionLine;
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}

bool Arguments::initLog(const CString& strRunLog, const CString& strErrLog)
{
	// ログファイル作成
	if(!CLog::GetInstance().SetRunLog(strRunLog) ||
		!CLog::GetInstance().SetErrLog(strErrLog)){
		return false;
	}
	CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
	CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\t"));

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	CLog::GetInstance().PrintLog(true, false, true, false, GetOption());

	strMsg.Format(_T("Start : %s\n"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}

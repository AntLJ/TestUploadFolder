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

// 必要なオプション情報があるかをチェックする
bool Argument::checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR description) const
{
	if( vm.count(option) == 0 ){
		cerr << description << "が未設定です。" << std::endl;
		return false;
	}
	return true;
}

// コマンドライン引数の解析
bool Argument::setParameter(int argc, TCHAR** argv)
{
	po::options_description desc("Allowed options");

	const char * const k_edtAddrDB  = "edt_addr_db";
	const char * const k_meshDB     = "mesh_db";
	const char * const k_mapDB      = "map_db";
	const char * const k_cityMeshList   = "city_mesh_list";
	const char * const k_baseMeshList   = "base_mesh_list";
	
	const char * const k_layer      = "layer";
	const char * const k_logDir     = "log_dir";
	const char * const k_master     = "master";
	const char * const k_iniFile    = "ini_file";
	const char * const k_runLog     = "run_log";

	// コマンドライン引数処理
	Arguments args;
	desc.add_options()
		(k_edtAddrDB,  uh::tvalue<uh::tstring>(&args.edtAddrDB),"[必須]住所（編集）DB")
		(k_meshDB,     uh::tvalue<uh::tstring>(&args.meshDB),   "[必須]都市地図メッシュDB")
		(k_mapDB,     uh::tvalue<uh::tstring>(&args.mapDB),     "[必須]背景ポリゴンDB")
		(k_cityMeshList,   uh::tvalue<uh::tstring>(&args.cityMeshList), "[任意]都市地図メッシュコードリスト")
		(k_baseMeshList,   uh::tvalue<uh::tstring>(&args.baseMeshList), "[任意]ベースメッシュコードリスト")
		(k_layer,      uh::tvalue<uh::tstring>(&args.layer),    "[必須]チェック対象リスト")
		(k_logDir,     uh::tvalue<uh::tstring>(&args.logDir),   "[必須]ログフォルダ")
		(k_iniFile,    uh::tvalue<uh::tstring>(&args.iniFile),  "[必須]設定ファイル")
		(k_master,     uh::tvalue<uh::tstring>(&args.master),   "[任意]テキスト住所マスタ")
		(k_runLog,     uh::tvalue<uh::tstring>(&args.runLog),   "[任意]実行ログファイル")
		;

	if(argc == 1) {
		cerr << "コマンドライン引数に問題があります" << endl;
		return false;
	}

	// argc, argv を解析して、結果をvmに格納
	po::variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	// 必須パラメータチェック
	bool bMust = true;

	bMust &= checkParameter( vm, k_edtAddrDB,       "[必須]住所（編集）DB");
	bMust &= checkParameter( vm, k_meshDB,          "[必須]都市地図メッシュDB");
	bMust &= checkParameter( vm, k_mapDB,           "[必須]背景ポリゴンDB");
	bMust &= checkParameter( vm, k_layer,           "[必須]チェック対象リスト");
	bMust &= checkParameter( vm, k_logDir,          "[必須]ログフォルダ");
	bMust &= checkParameter( vm, k_iniFile,         "[必須]設定ファイル");

	// パラメータが正常ではない場合は、オプション表示して終了
	if( !bMust){
		cerr << "住所系チェックツール" << std::endl;
		cerr << desc << std::endl;
		return false;
	}	

	m_addressDBPath = args.edtAddrDB.c_str();
	m_meshDBPath    = args.meshDB.c_str();
	m_mapDBPath     = args.mapDB.c_str();
	m_masterPath    = args.master.c_str();
	m_iniFilePath   = args.iniFile.c_str();
	m_cityMeshListPath  = args.cityMeshList.c_str();
	m_baseMeshListPath  = args.baseMeshList.c_str();
	m_layer         = args.layer.c_str();
	m_logDir        = args.logDir.c_str();
	m_runLog        = args.runLog.c_str();

	return true;
}

// 引数の取得
const CString& Argument::getValue(argumentType type)
{
	switch	( type )
	{
	case kAddressDBPath:
		return m_addressDBPath;
	case kMeshDBPath:
		return m_meshDBPath;
	case kMapDBPath:
		return m_mapDBPath;
	case kMasterPath:
		return m_masterPath;
	case kIniFilePath:
		return m_iniFilePath;
	case kCityMeshListPath:
		return m_cityMeshListPath;
	case kBaseMeshListPath:
		return m_baseMeshListPath;
	case kLayer:
		return m_layer;
	case kLogDir:
		return m_logDir;
	case kRunLog:
		if(m_runLog.IsEmpty())
		{
			CString strLogSuffix;
			SYSTEMTIME st;
			::GetLocalTime( &st );
			strLogSuffix.Format( _T("\\AddressCheck_%d%02d%02d%02d%02d%02d.log"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

			m_runLog = m_logDir + strLogSuffix;
		}
		return m_runLog;
	default:
		break;
	}

	return _T("");
}
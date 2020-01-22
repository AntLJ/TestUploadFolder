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
#include<iostream>
#include<io.h>
#include<cstdio>

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
bool Argument::setParameter(int argc, TCHAR** argv, FileController& runLogger, FileController& errLogger)
{
	po::options_description desc("Allowed options");
	const char * const k_convMode     = "conv_mode";
	const char * const k_transResult  = "trans_result";
	const char * const k_out          = "out";
	const char * const k_runLog       = "run_log";
	const char * const k_errLog       = "err_log";

	// コマンドライン引数処理
	Arguments args;
	desc.add_options()
		(k_convMode,        uh::tvalue<uh::tstring>(&args.convMode),       "[必須]オプション：city,addr,all")
		(k_transResult,     uh::tvalue<uh::tstring>(&args.transResult),    "[必須]翻訳結果フォルダ")
		(k_out,             uh::tvalue<uh::tstring>(&args.out),            "[必須]変換結果フォルダ")
		(k_runLog,          uh::tvalue<uh::tstring>(&args.runLog),         "[必須]実行ログファイル")
		(k_errLog,          uh::tvalue<uh::tstring>(&args.errLog),         "[必須]エラーログファイル")
		;

	if(argc == 1) {
		wcerr << L"コマンドライン引数に問題があります" << endl;
		return false;
	}

	// argc, argv を解析して、結果をvmに格納
	po::variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	// 必須パラメータチェック
	bool bMust = true;

	bMust &= checkParameter( vm, k_convMode,        "[必須]オプション：city,addr,all");
	bMust &= checkParameter( vm, k_transResult,     "[必須]翻訳結果フォルダ");
	bMust &= checkParameter( vm, k_out,             "[必須]変換結果フォルダ");
	bMust &= checkParameter( vm, k_runLog,          "[必須]実行ログファイル");
	bMust &= checkParameter( vm, k_errLog,          "[必須]エラーログファイル");

	// パラメータが正常ではない場合は、オプション表示して終了
	if( !bMust){
		wcerr << L"多言語翻訳フォーマット変換ツール" << std::endl;
		cerr << desc << std::endl;
		return false;
	}

	m_convMode      = args.convMode.c_str();
	m_transResult   = args.transResult.c_str();
	m_out           = args.out.c_str();
	m_runLog        = args.runLog.c_str();
	m_errLog        = args.errLog.c_str();

	// 実行時ログ初期化
	if( !runLogger.initialize(m_runLog.GetString(), SJIS_CREATE) )
	{
		wcerr << (message::getMsg( eFailRunLogInitialize)) << (L">>Path:") << ( m_runLog.GetString() ) << endl;
		return false;
	}
	// エラーログ初期化
	if( !errLogger.initialize(m_errLog.GetString(), UTF8_CREATE) )
	{
		wcerr << (message::getMsg( eFailErrLogInitialize)) << (L">>Path:") << ( m_errLog.GetString() ) << endl;
		return false;
	}
	//　引数が存在する場合のチェック
	if (m_convMode != CONV_MODE_CITY && m_convMode != CONV_MODE_ADDR && m_convMode != CONV_MODE_ALL)
	{
		runLogger.log(message::getMsg( eFailAnalyzeConvMode));
		wcerr << (message::getMsg( eFailAnalyzeConvMode));
		return false;
	}
	if(!PathIsDirectory(m_transResult.GetString()))
	{
		runLogger.log(message::getMsg( eFailAnalyzeTransResult));
		wcerr << (message::getMsg( eFailAnalyzeTransResult));
		return false;
	}
	if(!PathIsDirectory(m_out.GetString()))
	{
		runLogger.log(message::getMsg( eFailAnalyzeOut));
		wcerr << (message::getMsg( eFailAnalyzeOut));
		return false;
	}
	return true;
}

// 引数の取得
const CStringW& Argument::getValue(argumentType type) const
{
	switch	( type )
	{
	case kConvMode:
		return m_convMode;
	case kTransResult:
		return m_transResult;
	case kOut:
		return m_out;
	case kRunLog:
		return m_runLog;
	case kErrLog:
		return m_errLog;
	default:
		break;
	}
	return L"";
}
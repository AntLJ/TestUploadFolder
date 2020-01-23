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

#include "CreateNodelist.h"
#include "NodelistCreator.h"
#include "Defs.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>


// 実行
bool CCreateNodelist::execute()
{
	// 初期化
	if( ! init() )
		return false;


	// 設定ファイル読み込み
	CIllustOutputInfo cOutputInfos;
	if( ! cOutputInfos.read( CT2A( m_args.m_output_ini.c_str() ) ))
	{
		outputErrMsg( _T("ノードリスト出力情報ファイルの読み込みに失敗しました")); 
		mRunLog   << "処理結果:\t異常終了" << std::endl;
		return false;
	}

	CIllustRuleInfo cRuleInfos;
	if( ! cRuleInfos.read( m_args.m_name_rule_ini.c_str() ) )
	{
		outputErrMsg( _T("イラスト名ルール設定ファイルの読み込みに失敗しました")); 
		mRunLog   << "処理結果:\t異常終了" << std::endl;
		return false;
	}


	// ノードリスト作成オブジェクト初期化
	CNodelistCreator cCreator;
	CString strErrMsg;
	if( m_args.m_road_db.empty() ){
		cCreator.init( m_args.m_input_db.c_str(), m_args.m_output_dir.c_str(), m_args.m_latlon.c_str(), strErrMsg );	
	}else{
		cCreator.init( m_args.m_input_db.c_str(), m_args.m_road_db.c_str(), m_args.m_output_dir.c_str(), m_args.m_latlon.c_str(), strErrMsg );	
	}

	if( ! strErrMsg.IsEmpty() ) {
		outputErrMsg( strErrMsg );
		mRunLog   << "処理結果:\t異常終了" << std::endl;
		return false;
	}


	// ノードリスト出力情報ファイルのレコード数分のノードリスト作成
	// (または既存のノードリストに追記)
	for( auto iter = cOutputInfos.begin(); iter != cOutputInfos.end(); ++iter )
	{
		// イラスト名生成ルールを取得
		const auto& rule_iter = cRuleInfos.find( *iter );
		if( rule_iter == cRuleInfos.end() )
		{
			outputErrMsg( _T("イラスト名ルールの取得に失敗しました") );
			mRunLog   << "処理結果:\t異常終了" << std::endl;
			return false;
		}
		
		// ノードリスト作成
		CString strErrMsg;
		if( ! cCreator.create( *iter, rule_iter->mIllustNameRule, strErrMsg ))
		{
			outputErrMsg( strErrMsg );
			mRunLog   << "処理結果:\t異常終了" << std::endl;
			return false;
		}
	}

	mRunLog << "処理結果:\t正常終了" << std::endl;
	return true;
}


// 初期化
bool CCreateNodelist::init()
{
	using namespace uh;

	// ログのオープン
	mRunLog.open( m_args.m_runlog );
	if( ! mRunLog.good() ) {
		outputErrMsg( str_util::format( _T("処理ログのファイルパスが不正です : %s"), m_args.m_runlog) );
		return false;
	}

	mErrLog.open( m_args.m_errlog );
	if( ! mErrLog.good() ) {
		outputErrMsg( str_util::format( _T("エラーログのファイルパスが不正です : %s"), m_args.m_errlog) );
		return false;
	}

	// 処理ログに設定情報を出力
	if( ! m_args.m_road_db.empty() )
	{
		mRunLog << "DB接続先(イラストリンク):\t" << uh::toStr( m_args.m_input_db ) << std::endl;
		mRunLog << "DB接続先(道路):\t" << uh::toStr( m_args.m_road_db ) << std::endl;
	}
	else
	{
		mRunLog << "DB接続先:\t" << uh::toStr( m_args.m_input_db ) << std::endl;
	}
	mRunLog << "イラスト名生成ルール情報ファイル:\t" << uh::toStr( m_args.m_name_rule_ini ) << std::endl;
	mRunLog << "イラスト出力情報設定ファイル:\t" << uh::toStr( m_args.m_output_ini ) << std::endl;
	mRunLog << "出力フォルダ:\t" << uh::toStr( m_args.m_output_dir ) << std::endl;
	mRunLog << "処理ログ:\t" << uh::toStr( m_args.m_runlog ) << std::endl;
	mRunLog << "エラーログ:\t" << uh::toStr( m_args.m_errlog ) << std::endl;
	
	if( m_args.m_latlon == latlon::DEG )
		mRunLog << "緯度経度出力設定:\t実数緯度経度" << std::endl;
	else if( m_args.m_latlon == latlon::DMS )
		mRunLog << "緯度経度出力設定:\t度分秒緯度経度" << std::endl;
	else
		mRunLog << "緯度経度出力設定:\t出力なし" << std::endl;

	return true;
}


// エラーメッセージ出力
void CCreateNodelist::outputErrMsg( const CString& strErrMsg )
{
	std::cerr << "ERROR\t" << CT2A( strErrMsg ) << std::endl;
	if( mErrLog.is_open() && mErrLog.good() )
		mErrLog   << "ERROR\t" << CT2A( strErrMsg ) << std::endl;
}

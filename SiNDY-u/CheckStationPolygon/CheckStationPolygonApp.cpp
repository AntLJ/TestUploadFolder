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

#include "CheckStationPolygonApp.h"
#include "Logger.h"
#include "StationChecker.h"
#include "StationDetailCheckHandler.h"
#include "MidZoomPlatformCheckHandler.h"
#include "HighZoomStationCheckHandler.h"
#include "MidZoomStationCheckHandler.h"
#include "LowZoomPlatformCheckHandler.h"
#include "RelStationSiteCheckHandler.h"


using namespace sindy;


// 実行
bool CheckStationPolygonApp::execute()
{
	// ログ初期化
	if( ! initLog() ) {
		finish( false );
		return false;
	}

	// チェッカー登録
	resistChecker();

	// チェッカー初期化
	if( ! initChecker() ) {
		finish( false );
		return false;
	}

	// チェック実行
	if( ! runCheck() ) {
		finish( false );
		return false;
	}

	// 終了処理
	finish( true );
	return true;
}


// チェッカー登録
void CheckStationPolygonApp::resistChecker()
{
	m_CheckerContainer.push_back( std::make_shared<StationDetailCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<MidZoomPlatformCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<HighZoomStationCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<MidZoomStationCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<LowZoomPlatformCheckHandler>() );
	m_CheckerContainer.push_back( std::make_shared<RelStationSiteCheckHandler>() );
}


// チェッカー初期化
bool CheckStationPolygonApp::initChecker()
{
	// ワークスペースオープン
	CWorkspace workspace;
	if( ! workspace.Connect( m_args.getDB().c_str() ) ) {
		RUNLOGGER.Error( _T("DBの接続に失敗しました") );
		return false;
	}

	// チェックロジックを保持するオブジェクト初期化
	SPStationChecker checkLogic( std::make_shared<StationChecker>() );
	if( ! checkLogic->init( workspace )) {
		RUNLOGGER.Error( _T("テーブルのオープンに失敗しました") );
		return false;
	}

	// チェッカーの初期化
	for( auto checker : m_CheckerContainer )
	{
		if( ! checker->init( workspace, checkLogic ) ) {
			RUNLOGGER.Error( _T("チェックの前処理に失敗しました") );
			return false;
		}
	}

	return true;
}


// ログファイルの初期化
bool CheckStationPolygonApp::initLog()
{
	// 処理ログファイルオープン
	if( ! RUNLOGGER.Initialize( m_args.getLogDir() ) )
	{
		std::cerr <<  "処理ログファイルのオープンに失敗しました : " <<  uh::toStr( m_args.getLogDir() ) << std::endl;
		return false;
	}

	// ツール実行情報出力
	RUNLOGGER.Log( _T("接続先DB:\t%s\n"), m_args.getDB().c_str() );
	RUNLOGGER.Log( _T("ログ出力フォルダパス:\t%s\n"), m_args.getLogDir().c_str() );

	// エラーログファイルオープン
	if( ! ERRLOGGER.Initialize( m_args.getLogDir() ) )
	{
		RUNLOGGER.Error( _T("エラーログファイルのオープンに失敗しました : ") + m_args.getLogDir() );
		return false;
	}

	RUNLOGGER.Info( _T("実行開始 "));

	return true;
}


// チェック実行
bool CheckStationPolygonApp::runCheck()
{
	for( auto checker : m_CheckerContainer )
	{
		RUNLOGGER.Info( _T("チェック開始 -> ") + checker->getTarget() );

		if( ! checker->check() ) {
			RUNLOGGER.Error( _T("チェックに失敗しました") + checker->getTarget() );
			return false;
		}
	}
	return true;
}


// 後処理
void CheckStationPolygonApp::finish( bool bSuccess )
{
	// 終了メッセージ出力
	uh::tstring result = bSuccess ? _T("正常終了") : _T("異常終了");
	RUNLOGGER.Info( _T("処理結果 -> ") + result );

	// ファイルのクローズ
	RUNLOGGER.Finalize();
	ERRLOGGER.Finalize();
}

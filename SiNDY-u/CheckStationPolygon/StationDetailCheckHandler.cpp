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

#include "Logger.h"
#include "StationDetailCheckHandler.h"
#include "ErrDef.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindy
#include <schema.h>

using namespace sindy;
using namespace sindy::schema::sj;


// 初期化
bool StationDetailCheckHandler::init( const CWorkspace& workspace, SPStationChecker checker  )
{
	m_Checker = checker;
	m_TableFinder.InitCollection( workspace.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

	return true;
}


// チェック実行
bool StationDetailCheckHandler::check()
{
	// テーブルコンテナ取得
	auto& table = *m_TableFinder.FindTable( station_detail::kTableName );
	table.clear();

	// 全レコード取得
	table._Select( IQueryFilterPtr(), true );
	table.CreateCache();

	// 1レコードずつチェック
	using namespace err_def;
	for( const auto& row : table )
	{
		const auto& rowc = *CAST_ROWC( row );

		// [ERR_CODE 101]: 1つ以上の駅情報が紐付けられているか
		if( ! m_Checker->checkLinkedStation( rowc, station_detail::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrNotLinkedStation) );

		// [ERR_CODE 102]: ポリゴン種別が規定値外の値(コード値ドメイン定義外)の値になっていないか
		if( ! m_Checker->checkPolygonC( rowc ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrPolygonCOutOfValue) );

		// [ERR_CODE 201]: 自身を包括する"駅詳細ポリゴン"が存在するか
		if( ! m_Checker->checkContain( rowc, highzoom_station::kTableName ) )
			ERRLOGGER.Warn( rowc.GetTableName(), rowc.GetOID(), err(kWarnOutOfHighZoomStation) );

		// [ERR_CODE 203]: 自身を包括する"駅詳細ポリゴン"が存在し、かつ駅IDが一致しているか
		if( ! m_Checker->checkContainedBySameStationPolygon( rowc, highzoom_station::kTableName ) )
			ERRLOGGER.Warn( rowc.GetTableName(), rowc.GetOID(), err(kWarnOutOfSameIdHighZoomStation) );
	}

	return true;
}

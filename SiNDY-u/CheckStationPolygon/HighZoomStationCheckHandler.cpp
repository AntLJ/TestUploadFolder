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
#include "HighZoomStationCheckHandler.h"
#include "ErrDef.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>

// sindy
#include <schema.h>

using namespace sindy;
using namespace sindy::schema::sj;


// 初期化
bool HighZoomStationCheckHandler::init( const CWorkspace& workspace, SPStationChecker checker  )
{
	m_Checker = checker;
	m_TableFinder.InitCollection( workspace.OpenSameOwnersTable( highzoom_station::kTableName ) , CModel() );

	return true;
}


// チェック実行
bool HighZoomStationCheckHandler::check()
{
	// テーブルコンテナ取得
	auto& table = *m_TableFinder.FindTable( highzoom_station::kTableName );
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
		if( ! m_Checker->checkLinkedStation( rowc, highzoom_station::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrNotLinkedStation) );

		// [ERR_CODE 104]: 自身を包括する"駅簡易ポリゴン"が存在するか
		if( ! m_Checker->checkContain( rowc, midzoom_station::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrOutOfMidZoomStation) );

		// [ERR_CODE 108]: 自身を包括する"駅簡易ポリゴン"が存在し、かつ駅IDが一致しているか
		if( ! m_Checker->checkContainedBySameStationPolygon( rowc, midzoom_station::kTableName ) )
			ERRLOGGER.Error( rowc.GetTableName(), rowc.GetOID(), err(kErrOutOfSameIdMidZoomStation) );
	}

	return true;
}

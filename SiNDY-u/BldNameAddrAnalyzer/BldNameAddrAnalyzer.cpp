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
#include "BldNameAddrAnalyzer.h"
#include "AddrAnalyzer.h"
#include "ConditionMgr.h"
#include "globalfunc.h"
#include "logger.h"
#include "EditOperationMgr.h"

#include <WinLib/ADOBase.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <TDC/sindylib_base/GlobalRules.h>

using namespace sindy;
using namespace sindy::schema;
using namespace sindy::errorcode;

// 初期化
void BldNameAddrAnalyzer::init()
{
	auto tbl = TryOpenTable( 
		m_args.getValue(option::kTargetDb),
		m_args.getValue(option::kTargetTable) );

	m_finder.InitCollection( tbl, CModel() );

	ERRLOGGER.Initialize( m_args.getValue( option::kErrLog ).GetString() );
}

// 非メンバ関数前方宣言
void store( CTableContainer& table );
// 更新対象テーブルチェック
void checkTable( const CTableContainer& table );

// 実行
void BldNameAddrAnalyzer::execute()
{
	init();
	
	// 素材テーブルのキャッシュ設定しておく
	auto bldgSrcT = *m_finder.FindTable( m_args.getValue(option::kTargetTable) );
	bldgSrcT.SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

	// 対象条件のリストを読み込む
	ConditionMgr conditions;
	conditions.init( m_args.getValue( option::kTargetList ) );

	// 住所サーバ準備
	AddrAnalyzer analyzer;
	analyzer.init( m_args.getValue( option::kAddrDB ),
		           m_args.getValue( option::kXyUser ),
				   m_args.getValue( option::kReleaseUser ),
				   m_args.getValue( option::kBuildingDB ) );

	RUNLOGGER.Info( _T("住所解析開始...\n"), true );
	for( const auto& condition : conditions )
	{
		// 条件ごとに検索して処理
		CString query = condition.getQuery( bldgSrcT.GetWorkspace() );
		auto filter = AheInitQueryFilter( nullptr, nullptr, _T("%s"), query );
		RUNLOGGER.Log( uh::str_util::format( _T(" 対象：%s (%d件)\n"), query, bldgSrcT.RowCount(filter)), true );

		bldgSrcT._Select( filter, false );
		bldgSrcT.CreateCache();
		long failed(0), update(0);
		for( auto& row : bldgSrcT )
		{
			auto& bldgSrcRow = *CAST_ROWC( row );
			if( !analyzer.updateRow( bldgSrcRow ) )
			{
				ERRLOGGER.WriteLog( bldgSrcRow, _T("住所解析に失敗しました。") );
				bldgSrcRow.Reset();
				++failed;
				continue;
			}

			if( bldgSrcRow.Changed() )
				++update;
		}
		RUNLOGGER.Log( uh::str_util::format( _T(" 更新：%d件\n"), update ), true );
		RUNLOGGER.Log( uh::str_util::format( _T(" 解析失敗：%d件\n\n"), failed ), true );

		store( bldgSrcT );
		bldgSrcT.clear();
	}
	RUNLOGGER.Info( _T("住所解析完了\n"), true );
}


void store( CTableContainer& table )
{
	// BUILDINGNAME_SOURCE_POINT はSiNDY-cとは無関係なUPDATE_Cフィールドを
	// 持っているため、sindylib_baseで勝手に更新されるのを防ぐ。
	// ipcテーブルのフィールドは継承していないので、これで問題はないはずだが、いいのか、これで、、、
	g_bSetHistory = false;

	// 編集開始
	EditOperationMgr ope( table.GetWorkspace() );
	ope.Start();

	// 保存
	RUNLOGGER.Info( _T("保存... "), true );
	CErrorObjectsBase errs;
	if( sindyErr_NoErr != table.Store( errs ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("保存失敗: %s"), errs.ToString() ) );
	}
	RUNLOGGER.Info( _T("OK.\n"), true );

	ope.Stop();
}

void checkTable( const CTableContainer& table )
{
	// BUILDINGNAME_SRC_POINTと同じスキーマであることが前提。
	// 少なくとも、更新対象のフィールドは存在する必要がある。
	using namespace sindy::schema::sj::buildingname_src_point;

	std::vector<CString> fields = boost::assign::list_of
		( kAddrCode )          // ADDRCODE
		( kAddrLon )           // ADDR_LON
		( kAddrLat )           // ADDR_LAT
		( kAddrVer )           // ADDR_VER
		( kRepType )           // REPTYPE_C
		( kAddrLevel )         // ADDRLEVEL_C
		( kPinPoint )          // PINPOINT_F
		( kSameAddrCount )     // SAMEADDR_COUNT
		( kSameAddr )          // SAMEADDR_F
		( kMultiSameBldg );    // MULTISAMEBLDG_F

	const auto& fm = *table.GetFieldMap();
	for( const CString& field : fields )
	{
		if( 0 > fm.HasField( field ) )
		{
			throw std::runtime_error( uh::str_util::formatA( 
				_T("フィールドが存在しません。: %s.%s"), table.GetTableName(), field) );
		}
	}
}

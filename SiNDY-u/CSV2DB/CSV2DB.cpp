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
#include "CSV2DB.h"
#include "logger.h"
#include "CsvLoader.h"
#include "globalfunc.h"
#include "EditOperationMgr.h"
#include <WinLib/ADOBase.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/GeometryBase.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_core/ErrorCode.h>
#include <TDC/sindylib_base/ErrorObject2.h>
#include <TDC/sindylib_base/GlobalRules.h>


using namespace sindy;
using namespace sindy::schema;
using namespace sindy::errorcode;


// execute()で使う非メンバ関数のプロトタイプ宣言
void createRows( CTableContainer& destT, CsvLoader& csv, bool testMode );
void updateRows( CTableContainer& destT, CsvLoader& csv, bool testMode );

// 実行
void CCSV2DB::execute()
{
	// 子バージョン作る前のテーブル
	auto parentT = GetDestTable();

	// 入力CSV読み込み
	CsvLoader csv;
	csv.Load( m_args.getValue( arg::kInput ), m_args.getValue( arg::kForceOutput ), *parentT );

	// 子バージョンを作って編集開始
	CSPTableContainer spDest;
	EditOperationMgr editOpe;
	bool testMode = m_args.hasOption( arg::kTestMode );
	if( testMode )
	{
		spDest = parentT;
	}
	else
	{
		auto workVer = TryCreateVersion( *parentT );
		editOpe.Init( workVer );
		editOpe.Start();
		spDest = GetDestTable( workVer );
	}

	// SiNDY履歴は弄らないオプション
	if( m_args.hasOption( arg::kNoUpdateHistry ) )
		g_bSetHistory = false;
	
	// 新規レコード追加
	createRows( *spDest, csv, testMode );

	// 既存レコード更新
	updateRows( *spDest, csv, testMode );

	// 編集終了
	if( editOpe.IsBeingEdited() )
	{
		editOpe.Stop();
	}

	// 処理結果
	RUNLOGGER.WriteResult( csv );
}

// 対象テーブルを取得
sindy::CSPTableContainer CCSV2DB::GetDestTable( const IWorkspacePtr work /*= nullptr*/ )
{
	// ファインダから作り直し
	m_finder.ClearCollection();

	// ワークスペース指定されなければ引数のDBに接続
	IWorkspacePtr destWork = work ? work : TryConnect( m_args.getValue(arg::kTargetDB) );

	m_finder.InitCollection( 
		TryOpenTable( destWork, m_args.getValue(arg::kTargetTbl) ),
		CModel() );

	return m_finder.FindTable( m_args.getValue(arg::kTargetTbl) );
}


//////////////////////////
// 以下は非メンバ関数の実装
//////////////////////////

// csvのレコードからDBのレコードにフィールド値をコピーする
void setAttribute( CRowContainer& row, RecordInfo& ri )
{
	// 属性
	for( auto& fldVal : ri )
	{
		FieldInfo& fi = fldVal.first;
		const CComVariant& val = fldVal.second;

		// オブジェクトIDとLAT,LONは無視
		if( fi.isOidField() || fi.isLatLonField() )
			continue;

		// 値が指定されていなければ弄らない
		if( uh::variant_util::isNullOrEmpty( val ) )
			continue;

		row.SetValue( fi.destIndex, val );
		fi.isChanged = true;
	}

	// 形状
	if( ri.hasShape )
	{
		auto spRef = AheGetSpatialReference( (_IRow*)row );
		IPointPtr point( CLSID_Point );
		point->putref_SpatialReference( spRef );
		point->PutCoords( ri.latlon.X, ri.latlon.Y );
		row.SetShape( point );
	}
}

void StoreTable( CTableContainer& destT, const CsvLoader& csv, bool testMode  )
{
	// Storeすると差分がなくなるのでここでログ
	ERRLOGGER.WriteLog( csv, destT );

	if( testMode )
		return;

	// 保存
	CErrorObjectsBase errs;
	RUNLOGGER.Info( _T("保存... "), true );
	if( sindyErr_NoErr != destT.Store( errs ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("保存失敗: %s"), errs.ToString() ) );
	}
	RUNLOGGER.Info( _T("OK.\n"), true );
}

void createRows( CTableContainer& destT, CsvLoader& csv, bool testMode )
{
	auto recordsList = csv.GetNewRecordsList();

	RUNLOGGER.Info( _T("新規レコード作成... "), true );
	long count = 0;
	boost::timer t;

	for( auto& records : recordsList )
	{
		for( auto& record : records )
		{
			auto& row = *destT.CreateDummyRow();
			row.CreateCache();

			const auto& fm = *destT.GetFieldMap();

			// OPERATORは非NULLなので値を入れておかないとエラーに成ってしまう
			if( 0 < fm.HasField( ipc_table::kOperator )  )
				row.SetValue( fm.HasField( ipc_table::kOperator ), _T("sindy") );

			// PURPOSE_Cも
			if( 0 < fm.HasField( ipc_table::kPurpose )  )
				row.SetValue( fm.HasField( ipc_table::kPurpose ), 0L );

			setAttribute( row, record );

			++count;
		}
		
		// 実体化
		CErrorObjectsBase errs;
		destT.Materialization(nullptr, 0, errs);

		StoreTable( destT, csv, testMode );

		// 大量の場合ここでclearしないとあふれる
		destT.clear();
		
		std::cerr << count << "records. " << t.elapsed() << "[s]" <<std::endl;
	}

	RUNLOGGER.Info( _T("ok. "), true );
}

void updateRows( CTableContainer& destT, CsvLoader& csv, bool testMode )
{
	// 1000件ごとにIN句で更新対象のオブジェクトを選択
	for( const auto& ids1000 : uh::enumerate_elements( csv.GetUpdateIDs() ) )
	{
		CString where = uh::str_util::format(
			_T("%s IN (%s)"), kObjectID, CString(ids1000.c_str())
		);

		destT._Select( AheInitQueryFilter( nullptr, nullptr, where ), false );
		destT.CreateCache();


		for( auto& row : destT )
		{
			auto& destRow = *CAST_ROWC(row);
			setAttribute( destRow, const_cast<RecordInfo&>(csv.find_by_oid(destRow.GetOID()) ) );
		}
		
		// 大量の場合ここでclearしないとあふれる
		StoreTable( destT, csv, testMode );

		destT.clear();
	}
}

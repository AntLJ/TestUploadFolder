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
#include "CppUnitTest.h"

// テスト対象
#include "../StationChecker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Arc
#include <arctl/coinitializer.h>

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// sindy::schema
#include <sindy/schema.h>

namespace unittest
{	
	// テストデータ (PGDB)
	const TCHAR* TEST_DB = _T("..\\unittest\\TestData\\TestData.mdb");
	const TCHAR* TEST_DB_ERR = _T("..\\unittest\\TestData\\TestData_Err.mdb");


	TEST_CLASS(StationCheckerTest)
	{
	public:

		// 初期化関数のチェック
		TEST_METHOD(init_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;

			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CWorkspace cWS_Err;
			if( ! cWS_Err.Connect(TEST_DB_ERR) )
				Assert::Fail();

			
			// 正常
			// 初期化に成功するか
			{
				StationChecker checker;
				if( ! checker.init( cWS ) )
					Assert::Fail();
			}

			// エラー
			// STASIOTN_DETAILが存在しないWorkspaceを
			// 渡したとき初期化に失敗するか
			{
				StationChecker checker;
				if( checker.init( cWS_Err ) )
					Assert::Fail();
			}
		}

		// チェック関数のテスト
		TEST_METHOD(checkPolygonC_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// テストデータ取得準備
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( station_detail::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

			// 準備
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;;

			// テスト -----------------------------
			// 正常
			{
				const auto& row = getRowCByOID( table, 5);
				Assert::IsTrue( checker.checkPolygonC( *row ) );
			}
			// エラー
			{
				const auto& row = getRowCByOID( table, 10);
				Assert::IsFalse( checker.checkPolygonC( *row ) );
			}
			// ------------------------------------

		}

		// 駅情報紐付きチェック
		TEST_METHOD(checkLinkedStation_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// テストデータ取得準備
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( station_detail::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// 準備
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// テスト -----------------------------
			// 正常
			{
				const auto& row = getRowCByOID( table, 5);
				Assert::IsTrue( checker.checkLinkedStation( *row, station_detail::kTableName ) );
			}

			// エラー
			{
				const auto& row = getRowCByOID( table, 6);
				Assert::IsFalse( checker.checkLinkedStation( *row, station_detail::kTableName ) );
			}
			// ------------------------------------
		}

		// 駅情報紐付きチェック
		TEST_METHOD(checkContain_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// テストデータ取得準備
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( station_detail::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( station_detail::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

			const auto& table2 = cFinder.FindTable( highzoom_station::kTableName );
			table2->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );

			// 準備
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// テスト -----------------------------
			// 正常
			// 完全に内包しているパターン
			{
				const auto& row = getRowCByOID( table, 5);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// 一部の辺が接する形で内包しているパターン
			{
				const auto& row = getRowCByOID( table, 8);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}

			// エラー
			// ポリゴン全体がはみ出しているパターン
			{
				const auto& row = getRowCByOID( table, 7);
				Assert::IsFalse( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// 一部だけはみ出しているパターン
			{
				const auto& row = getRowCByOID( table, 9);
				Assert::IsFalse( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// 内包しているが駅IDが異なるパターン
			{
				const auto& row = getRowCByOID( table, 12);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsFalse( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// 包括先のレイヤにフィーチャが1件もないケース
			{
				const auto& row = getRowCByOID( table2, 8);
				Assert::IsFalse( checker.checkContain( *row, midzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, midzoom_station::kTableName ) );
			}
			// 自身に駅IDが紐付けられていない、かつ包括するポリゴンが存在するケース
			// 別のチェックで検出すべき内容のため、エラーとしない
			{
				const auto& row = getRowCByOID( table, 15);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsTrue( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}

			// 包括先のポリゴンに駅IDが紐付けられていないケース
			{
				const auto& row = getRowCByOID( table, 14);
				Assert::IsTrue( checker.checkContain( *row, highzoom_station::kTableName ) );
				Assert::IsFalse( checker.checkContainedBySameStationPolygon( *row, highzoom_station::kTableName ) );
			}
			// ------------------------------------
		}

		//  駅ポリゴンとの紐付き正当性チェック
		TEST_METHOD(checkLinkedNotExistPolygon_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// テストデータ取得準備
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( rel_station_site::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( rel_station_site::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// 準備
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// テスト -----------------------------
			// 正常
			{
				const auto& row = getRowCByOID( table, 15);
				Assert::IsTrue( checker.checkLinkedNotExistPolygon( *row ) );
			}
			// エラー
			{
				const auto& row = getRowCByOID( table, 16);
				Assert::IsFalse( checker.checkLinkedNotExistPolygon( *row ) );
			}
			// ------------------------------------

		}

		//  駅との紐付き正当性チェック
		TEST_METHOD(checkLinkedNotExistStation_Test)
		{
			const arctl::coinitializer aCoInitializer;
			using namespace sindy;
			using namespace sindy::schema::sj;

			// テストデータ取得準備
			CWorkspace cWS;
			if( ! cWS.Connect(TEST_DB) )
				Assert::Fail();

			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( rel_station_site::kTableName ) , CModel() );

			const auto& table = cFinder.FindTable( rel_station_site::kTableName );
			table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// 準備
			StationChecker checker;
			if( ! checker.init( cWS ) )
				Assert::Fail();;

			// テスト -----------------------------
			// 正常
			{
				const auto& row = getRowCByOID( table, 15);
				Assert::IsTrue( checker.checkLinkedNotExistStation( *row ) );
			}
			// エラー
			{
				const auto& row = getRowCByOID( table, 17);
				Assert::IsFalse( checker.checkLinkedNotExistStation( *row ) );
			}
			// ------------------------------------
		}


	// テスト用関数

	private:

		// TableContainerからOIDで1レコード取得
		sindy::CSPRowContainer getRowCByOID( const sindy::CSPTableContainer& table, long lOid )
		{
			using namespace sindy;
			using namespace schema;

			const auto& row = *table->find_by_key( sindy::schema::ipcfeature::kObjectID, lOid );
			const auto& row_c = CAST_ROWC(row);

			return row_c;
		}

	};	// TEST_CLASS


}

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

#include <arctl/coinitializer.h>

#include <atltime.h>

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// sindy::schema
#include <schema.h>

// テスト対象
#include "AheBuddyGlobals.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{		
	// テストデータ(PGDB)
	const TCHAR* TESTDB = _T("..\\unittest\\TestData\\WalkNw.mdb");

	TEST_CLASS(AheBuddyGlobalsTest)
	{

	public:
		
		TEST_METHOD(JedgeLinkRelationTest)
		{
			const arctl::coinitializer aCoInitializer;

			using namespace sindy;
			using namespace schema;

			// Workspace に接続
			CWorkspace cWS;
			if( ! cWS.Connect(TESTDB) )
				Assert::Fail();

			// テスト準備
			//   テーブルファインダ準備
			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( _T("WALK_LINK")) , CModel() );
			//   テーブル接続
			CSPTableContainer walk_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("WALK_LINK") ));
			auto road_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("ROAD_LINK") ));
			walk_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
			road_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// 以下、テスト実行
			sindy::LinkRelationPosition nResultCode = sindy::LinkRelationPosition::kNotFound;
			// パターン1
			IGeometryPtr ipBaseGeo   = getGeoByOID( walk_table, 12 );
			IGeometryPtr ipTargetGeo = getGeoByOID( road_table, 13 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン2
			ipBaseGeo   = getGeoByOID( walk_table, 13 );
			ipTargetGeo = getGeoByOID( road_table, 15 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン3
			ipBaseGeo   = getGeoByOID( walk_table, 15 );
			ipTargetGeo = getGeoByOID( road_table, 17 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// 右

			// パターン4
			ipBaseGeo   = getGeoByOID( walk_table, 16 );
			ipTargetGeo = getGeoByOID( road_table, 18 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 5.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 0);	// 見つからない

			// パターン5
			ipBaseGeo   = getGeoByOID( walk_table, 19 );
			ipTargetGeo = getGeoByOID( road_table, 21 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// 右

			// パターン6
			ipBaseGeo   = getGeoByOID( walk_table, 21 );
			ipTargetGeo = getGeoByOID( road_table, 22 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 20.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン7
			ipBaseGeo   = getGeoByOID( walk_table, 23 );
			ipTargetGeo = getGeoByOID( road_table, 25 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン8
			ipBaseGeo   = getGeoByOID( walk_table, 24 );
			ipTargetGeo = getGeoByOID( road_table, 26 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン9
			ipBaseGeo   = getGeoByOID( walk_table, 26 );
			ipTargetGeo = getGeoByOID( road_table, 27 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 50.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// 右

			// パターン10
			ipBaseGeo   = getGeoByOID( walk_table, 28 );
			ipTargetGeo = getGeoByOID( road_table, 28 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 50.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン11
			ipBaseGeo   = getGeoByOID( walk_table, 29 );
			ipTargetGeo = getGeoByOID( road_table, 29 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン12
			ipBaseGeo   = getGeoByOID( walk_table, 30 );
			ipTargetGeo = getGeoByOID( road_table, 30 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			 // パターン13
			ipBaseGeo   = getGeoByOID( walk_table, 33 );
			ipTargetGeo = getGeoByOID( road_table, 33 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン14
			ipBaseGeo   = getGeoByOID( walk_table, 34 );
			ipTargetGeo = getGeoByOID( road_table, 34 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 40.0, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// 右

			// 速度(10,000件1分以内としておく)
			ipBaseGeo   = getGeoByOID( walk_table, 31 );
			ipTargetGeo = getGeoByOID( road_table, 31 );
			CFileTime cTimeStart = CFileTime::GetCurrentTime();
			for( int i = 0; i < 10000; ++i )
				AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode );
			CFileTime cTimeEnd = CFileTime::GetCurrentTime(); 
			CFileTimeSpan cTimeSpan = cTimeEnd - cTimeStart;
			Assert::IsTrue( cTimeSpan.GetTimeSpan()/10000 < 60000 );  //[ms]

		}

		TEST_METHOD(JudgeLinkRelation2Test)
		{
			const arctl::coinitializer aCoInitializer;

			using namespace sindy;
			using namespace schema;

			// Workspace に接続
			CWorkspace cWS;
			if( ! cWS.Connect(TESTDB) )
				Assert::Fail();

			// テスト準備
			//   テーブルファインダ準備
			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( _T("WALK_LINK")) , CModel() );
			//   テーブル接続
			CSPTableContainer walk_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("WALK_LINK") ));
			auto road_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("ROAD_LINK") ));
			walk_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
			road_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// 以下、テスト実行
			unsigned int nResultCode = 99;
			// パターン1
			IGeometryPtr ipBaseGeo   = getGeoByOID( walk_table, 12 );
			IGeometryPtr ipTargetGeo = getGeoByOID( road_table, 13 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン2
			ipBaseGeo   = getGeoByOID( walk_table, 13 );
			ipTargetGeo = getGeoByOID( road_table, 15 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン3
			ipBaseGeo   = getGeoByOID( walk_table, 16 );
			ipTargetGeo = getGeoByOID( road_table, 18 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン4
			ipBaseGeo   = getGeoByOID( walk_table, 23 );
			ipTargetGeo = getGeoByOID( road_table, 25 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン5
			ipBaseGeo   = getGeoByOID( walk_table, 24 );
			ipTargetGeo = getGeoByOID( road_table, 26 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// パターン6
			ipBaseGeo   = getGeoByOID( walk_table, 26 );
			ipTargetGeo = getGeoByOID( road_table, 27 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 1);	// 右

			// パターン7
			ipBaseGeo   = getGeoByOID( walk_table, 28 );
			ipTargetGeo = getGeoByOID( road_table, 28 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			 // パターン8
			ipBaseGeo   = getGeoByOID( walk_table, 33 );
			ipTargetGeo = getGeoByOID( road_table, 33 );
			AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			Assert::AreEqual((int)nResultCode, 2);	// 左

			// 速度(10,000件1分以内としておく)
			ipBaseGeo   = getGeoByOID( walk_table, 31 );
			ipTargetGeo = getGeoByOID( road_table, 31 );
			CFileTime cTimeStart = CFileTime::GetCurrentTime();
			for( int i = 0; i < 10000; ++i )
				AheSimpleJudgeLinkRelation( ipBaseGeo, ipTargetGeo, nResultCode );
			CFileTime cTimeEnd = CFileTime::GetCurrentTime(); 
			CFileTimeSpan cTimeSpan = cTimeEnd - cTimeStart;
			Assert::IsTrue( cTimeSpan.GetTimeSpan()/10000 < 60000 );  //[ms]

		}

		TEST_METHOD(JudgeLinkRelation3Test)
		{
			const arctl::coinitializer aCoInitializer;

			using namespace sindy;
			using namespace schema;

			// Workspace に接続
			CWorkspace cWS;
			if( ! cWS.Connect(TESTDB) )
				Assert::Fail();

			// テスト準備
			//   テーブルファインダ準備
			CTableFinder cFinder;
			cFinder.InitCollection( cWS.OpenSameOwnersTable( _T("WALK_LINK")) , CModel() );
			//   テーブル接続
			CSPTableContainer walk_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("WALK_LINK") ));
			auto road_table = boost::static_pointer_cast<CTableContainer>( cFinder.FindTable( _T("ROAD_LINK") ));
			walk_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
			road_table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );


			// 以下、テスト実行
			// 各パターンはJedgeLinkRelationTestでテストしているため、ここでは判定結果で値が妥当かのみチェック

			sindy::LinkRelationPosition nResultCode = sindy::LinkRelationPosition::kNotFound;
			double dFindDist = 0.0;
			// パターン1
			IGeometryPtr ipBaseGeo   = getGeoByOID( walk_table, 12 );
			IGeometryPtr ipTargetGeo = getGeoByOID( road_table, 13 );
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode, dFindDist );
			Assert::AreEqual((int)nResultCode, 2);	// 左
			Assert::IsTrue( 15.0 < dFindDist &&  dFindDist < 16.0 );	// 約15.8m

			// パターン2

			// パターン3
			ipBaseGeo   = getGeoByOID( walk_table, 15 );
			ipTargetGeo = getGeoByOID( road_table, 17 );
			dFindDist = 0.0;
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 30.0, nResultCode, dFindDist );
			Assert::AreEqual((int)nResultCode, 1);	// 右
			Assert::IsTrue( 10.0 < dFindDist &&  dFindDist < 11.0 );	// 約10.4m

			// パターン4
			ipBaseGeo   = getGeoByOID( walk_table, 16 );
			ipTargetGeo = getGeoByOID( road_table, 18 );
			dFindDist = 0.0;
			AheJudgeLinkRelation( ipBaseGeo, ipTargetGeo, 5.0, nResultCode, dFindDist );
			Assert::AreEqual((int)nResultCode, 0);	// 見つからない
			Assert::AreEqual(dFindDist, -1.0);		// -1.0m

		}

	private:
		// TableFinderからOIDで1レコード取得
		IGeometryPtr getGeoByOID( const sindy::CSPTableContainer& table, long lOid )
		{
			using namespace sindy;
			using namespace schema;

			IGeometryPtr ipRetGeo;
			_IRowPtr ipRow = table->GetRow( lOid );
			IFeaturePtr(ipRow)->get_Shape( &ipRetGeo );
			return ipRetGeo;
		}

	};
}

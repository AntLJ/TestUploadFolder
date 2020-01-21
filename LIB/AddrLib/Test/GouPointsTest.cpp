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
#include "CheckInit.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sindy;
using namespace sindy::schema;
using namespace addr;

namespace Test
{
	// FIXユーザが消えたら適宜変更して下さい
	const CString strTableName = _T("FIX201605.City_Admin");

	TEST_CLASS(GouPointsTest)
	{
	private:
		// CGouPointsをstaticメンバで持つと、通しでチェックしたときにエラーになるので
		// CWorkspaceをメンバで持ち、CGouPointsは都度生成するようにする
		static std::shared_ptr<CWorkspace> m_spWorkspacePgdb;
		static std::shared_ptr<CWorkspace> m_spWorkspaceSde;

	public:
		// テストクラス初期化処理
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			m_spWorkspacePgdb.reset(new CWorkspace(verificationDatDir + _T("53392442.mdb")));

			// サーバが変わったら適宜変更して下さい
			m_spWorkspaceSde.reset(new CWorkspace(_T("ronly/ronly/SDE.DEFAULT/5151/plum")));
		}

		// テストクラス終了処理
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 指定行政界以下、号ポイント取得テスト(都市地図行政界と同一ユーザ・ワークスペースの号ポイント)
		TEST_METHOD(Gous_SelectByAddrCode1)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));

			// 「01 101 002 001」を持つ、都市地図行政界以下の号ポイントを検索
			gouPoints.SelectByAddrCode(_T("01101002001"));
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin)
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)6, pAdmin->GetOID());

				// 号ポイントは、2つ
				Assert::AreEqual((size_t)2, pAdmin->size(sindyTableType::gou_point));

				// 行政界内にある号ポイント取得
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(12 <= p->GetOID() && p->GetOID() <= 13);
				}
			}
		}

		// 指定行政界以下、号ポイント取得テスト(ワークスペースが所有ユーザでない場合)
		TEST_METHOD(Gous_SelectByAddrCode2)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspaceSde->OpenTable(strTableName)));

			// 「07 201 118 000」を持つ、都市地図行政界以下の号ポイントを検索
			gouPoints.SelectByAddrCode(_T("07201118000"));
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin )
			{
				// 行政界が取れている
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::IsTrue(0 < pAdmin->GetOID());

				// 号ポイントがいることがわかっている
				Assert::IsTrue(0 < pAdmin->size(sindyTableType::gou_point));

				// 行政界内にある号ポイント取得
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					// 号ポイントが取れる
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(0 < p->GetOID());
				}
			}
		}

		// 指定行政界以下、号ポイント取得テスト(号ポイント手動設定Ver)
		TEST_METHOD(Gous_SelectByAddrCode3)
		{
			// 都市地図行政界は、PGDB
			// 号ポイントは、SDE
			CGouPoints gouPoints1(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));
			CGouPoints gouPoints2(CFeatureClass(m_spWorkspaceSde->OpenTable(strTableName)));

			// 先に号ポイント手動設定
			gouPoints1.SetGouPointTable(gouPoints2.OpenSameOwnerTable(gou_point::kTableName));

			// 「02 201 001 002」を持つ、都市地図行政界以下の号ポイントを検索
			gouPoints1.SelectByAddrCode(_T("02201001002"));
			auto tableType = gouPoints1.GetTableType();
			for (auto itAdmin = gouPoints1.begin(tableType); itAdmin != gouPoints1.end(tableType); ++itAdmin )
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)4, pAdmin->GetOID());

				// 号ポイントは、1つ
				Assert::AreEqual((size_t)1, pAdmin->size(sindyTableType::gou_point));
			
				// 行政界内にある号ポイント取得
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(21429856 == p->GetOID());
				}
			}
		}

		// 16桁コード指定行政界以下、号ポイント取得テスト
		TEST_METHOD(Gous_SelectByAddrCode4)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));

			// 「01 101 001 001 00001」を持つ、都市地図行政界以下の号ポイントを検索
			gouPoints.SelectByAddrCode(_T("0110100100100001"));
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin )
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)11, pAdmin->GetOID());

				// 号ポイントは、2つ
				Assert::AreEqual((size_t)2, pAdmin->size(sindyTableType::gou_point));
			
				// 行政界内にある号ポイント取得
				for( auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou )
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(1 == p->GetOID() || 3 == p->GetOID());
				}
			}
		}

		// Where句指定による行政界以下、号ポイント取得テスト
		TEST_METHOD(Gous_SelectByWhereClause)
		{
			CGouPoints gouPoints(CFeatureClass(m_spWorkspacePgdb->OpenTable(city_admin::kTableName)));

			// 「01 101 001 001 00001」を持つ、都市地図行政界以下の号ポイントを検索
			CString strWhere;
			strWhere.Format(
				_T("%s='01101' and %s='001001' and %s=1"),
				city_admin::kCityCode, city_admin::kAddrCode, city_admin::kGaikuFugo);
			gouPoints.SelectByWhereClause(strWhere);
			auto tableType = gouPoints.GetTableType();
			for (auto itAdmin = gouPoints.begin(tableType); itAdmin != gouPoints.end(tableType); ++itAdmin)
			{
				const boost::shared_ptr<CCityAdmin> pAdmin = boost::static_pointer_cast<CCityAdmin>(*itAdmin);
				Assert::AreEqual((long)11, pAdmin->GetOID());

				// 号ポイントは、2つ
				Assert::AreEqual((size_t)2, pAdmin->size(sindyTableType::gou_point));
			
				// 行政界内にある号ポイント取得
				for (auto itGou = pAdmin->begin(sindyTableType::gou_point); itGou != pAdmin->end(sindyTableType::gou_point); ++itGou)
				{
					const boost::shared_ptr<CGouPoint> p = boost::static_pointer_cast<CGouPoint>(*itGou);
					Assert::IsTrue(1 == p->GetOID() || 3 == p->GetOID());
				}
			}
		}
	};

	// staticメンバ変数定義
	std::shared_ptr<CWorkspace> GouPointsTest::m_spWorkspacePgdb;
	std::shared_ptr<CWorkspace> GouPointsTest::m_spWorkspaceSde;
}

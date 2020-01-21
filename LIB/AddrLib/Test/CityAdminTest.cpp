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
	TEST_CLASS(CityAdminTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcCityAdmin;

	public:
		// テストクラス初期化処理
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkSpace(verificationDatDir + _T("53392442.mdb"));
			m_spFcCityAdmin.reset(new CFeatureClass(cWorkSpace.OpenTable(city_admin::kTableName)));
		}

		// テストクラス終了処理
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 市区町村コード取得
		TEST_METHOD(Adm_GetCityCode)
		{
			Assert::AreEqual(_T("01101"), CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetCityCode());
			Assert::AreEqual(_T("01101"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetCityCode());
		}

		// 大字・字コード取得
		TEST_METHOD(Adm_GetOazaAzaCode)
		{
			Assert::AreEqual(_T("001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetOazaAzaCode());
			Assert::AreEqual(_T("001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetOazaAzaCode());
		}

		// 街区符号取得
		TEST_METHOD(Adm_GetGaikuFugo)
		{
			Assert::AreEqual((long)1, CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetGaikuFugo());
		}

		// 拡張街区符号取得
		TEST_METHOD(Adm_GetExGaikuFugo)
		{
			Assert::AreEqual(_T("テスト"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetExGaikuFugo());
		}

		// 拡張街区符号ヨミ
		TEST_METHOD(Adm_GetExGaikuFugoYomi)
		{
			Assert::AreEqual(_T("スズキ"), CCityAdmin(m_spFcCityAdmin->GetFeature(7)).GetExGaikuFugoYomi());
			Assert::AreEqual(_T("タナカ"), CCityAdmin(m_spFcCityAdmin->GetFeature(8)).GetExGaikuFugoYomi());
			// オブジェクトヨミテーブルが優先されているかを確認
			Assert::AreEqual(_T("デンチュン"), CCityAdmin(m_spFcCityAdmin->GetFeature(9)).GetExGaikuFugoYomi());
		}

		// 住所整備種別取得
		TEST_METHOD(Adm_GetAddrClass)
		{
			using namespace sindy::schema::city_admin::addr_class;
			Assert::IsTrue(kResidenceIndicationAndChibanUndeveloped == CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetAddrClass());
			Assert::IsTrue(kResidenceIndicationMaintenance == CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrClass());
			Assert::IsTrue(kAzaUndeveloped == CCityAdmin(m_spFcCityAdmin->GetFeature(2)).GetAddrClass());
			Assert::IsTrue(kChibanMaintenance == CCityAdmin(m_spFcCityAdmin->GetFeature(3)).GetAddrClass());
		}

		// エリア判別コード取得
		TEST_METHOD(Adm_GetAreaClass)
		{
			using namespace sindy::schema::city_admin::area_class;
			Assert::IsTrue(kOutArea == CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetAreaClass() );	
			Assert::IsTrue(kInArea == CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAreaClass() );
			Assert::IsTrue(kSea == CCityAdmin(m_spFcCityAdmin->GetFeature(2)).GetAreaClass() );	
		}

		// 住所変換モード取得
		TEST_METHOD(Adm_GetConvType)
		{
			using namespace sindy::schema::city_admin;
			Assert::IsTrue(conv_type::kNormal == CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetConvType() );
			Assert::IsTrue(conv_type::kOazaGeneration == CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetConvType() );
			Assert::IsTrue(conv_type::kAzaGeneration == CCityAdmin(m_spFcCityAdmin->GetFeature(3)).GetConvType() );
		}

		// 20桁コード取得
		TEST_METHOD(Adm_GetAddrCode)
		{
			// 20桁コード(文字考慮せず)
			Assert::AreEqual(_T("01101001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(10)).GetAddrCode());
			Assert::AreEqual(_T("0110100100100001"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrCode());
			Assert::AreEqual(_T("01101001001"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode());

			// 20桁コード(文字考慮)
			Assert::AreEqual(_T("01101001001A0000"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode(true));

			// 20桁コード(通称)
			Assert::AreEqual(_T("01101081000"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode2());
			Assert::AreEqual(_T("0110108200000001"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrCode2());

			// 20桁コード(通称・文字考慮)
			Assert::AreEqual(_T("01101081000A0000"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrCode2(true));
		}

		// 住所名取得
		TEST_METHOD(Adm_GetAddrName)
		{
			// 住所名取得
			Assert::AreEqual(_T("北海道札幌市中央区旭ケ丘１丁目１"), CCityAdmin(m_spFcCityAdmin->GetFeature(11)).GetAddrName());
			Assert::AreEqual(_T("北海道札幌市中央区旭ケ丘１丁目テスト"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrName());
			Assert::AreEqual(_T("北海道札幌市中央区円山北町テスト"), CCityAdmin(m_spFcCityAdmin->GetFeature(1)).GetAddrName2());
			Assert::AreEqual(_T("青森県青森市青柳２丁目"), CCityAdmin(m_spFcCityAdmin->GetFeature(4)).GetAddrName());
		}
	};

	// staticメンバ変数定義
	std::shared_ptr<CFeatureClass> CityAdminTest::m_spFcCityAdmin;
}

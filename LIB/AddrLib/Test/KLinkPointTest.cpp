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
	TEST_CLASS(KLinkPointTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcKLinkPoint;

	public:
		// テストクラス初期化処理
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkspace(verificationDatDir + _T("53392442.mdb"));
			m_spFcKLinkPoint.reset(new CFeatureClass(cWorkspace.OpenTable(klink_point::kTableName)));
		}

		// テストクラス終了処理
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 初期22桁住所コード取得テスト
		TEST_METHOD(KLink_GetAddrCodeOrg)
		{
			Assert::AreEqual(_T("3820407700100001003501"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetAddrCodeOrg());
			Assert::AreEqual(_T("3820407700100001003701"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetAddrCodeOrg());
		}

		// 最新22桁住所コード取得テスト
		TEST_METHOD(KLink_GetAddrCode)
		{
			Assert::AreEqual(_T("3820403100100001003501"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetAddrCode());
			Assert::AreEqual(_T("3820408300100001003701"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetAddrCode());
		}

		// 方書名称取得テスト
		TEST_METHOD(KLink_GetKatagakiName)
		{
			Assert::AreEqual(_T(""), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetKatagakiName());
			Assert::AreEqual(_T("テスト"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetKatagakiName());
		}

		// 掲載名称取得テスト
		TEST_METHOD(KLink_GetKeisaiName)
		{
			Assert::AreEqual(_T("フジパン　愛媛営業所"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetKeisaiName());
			Assert::AreEqual(_T("八幡浜ダイハツモータース"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetKeisaiName());
		}

		// 情報提供元コード取得テスト
		TEST_METHOD(KLink_GetInfoSrc)
		{
			using namespace klink_point::info_src;
			Assert::IsTrue(kWallLinkData == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetInfoSrc());
			Assert::IsTrue(kGouMaintenanceData == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetInfoSrc());
		}

		// 入力用座標精度コード取得テスト
		TEST_METHOD(KLink_GetInputSeido)
		{
			using namespace klink_point::input_seido;
			Assert::IsTrue(kNoVerification == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetInputSeido());
			Assert::IsTrue(kMultipleWallCounterData == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetInputSeido());
		}

		// リリース用座標精度コード取得テスト
		TEST_METHOD(KLink_GetReleaseSeido)
		{
			using namespace klink_point::release_seido;
			Assert::IsTrue(kMunicipalityBorder == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetReleaseSeido());
			Assert::IsTrue(kOazaBorder == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetReleaseSeido());
		}

		// ビル名称取得テスト
		TEST_METHOD(KLink_GetBuildName)
		{
			Assert::AreEqual(_T("鈴木ビル"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetBuildName());
			Assert::AreEqual(_T(""), CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetBuildName());
		}

		// ビル情報提供元コード取得テスト
		TEST_METHOD(KLink_GetBuildInfoSrc)
		{
			using namespace klink_point::buildinfo_src;
			Assert::IsTrue(kAutoGeneration == CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetBuildInfoSrc());
			Assert::IsTrue(kPhoneSurvey == CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetBuildInfoSrc());
		}

		// ビル最上階数取得テスト
		TEST_METHOD(KLink_GetMaxFloors)
		{
			Assert::AreEqual((long)1, CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetMaxFloors());
			Assert::AreEqual((long)10, CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetMaxFloors());
		}

		// テナント階数取得テスト
		TEST_METHOD(KLink_GetFloor)
		{
			Assert::AreEqual((long)1, CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetFloor());
			Assert::AreEqual((long)10, CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetFloor());
		}

		// 住所名取得
		TEST_METHOD(KLink_GetAddrName)
		{
			Assert::AreEqual(_T("愛媛県八幡浜市八代１丁目１－３５"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetAddrName());
			Assert::AreEqual(_T("愛媛県八幡浜市八代５５９"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(5)).GetAddrName());
			Assert::AreEqual(_T("愛媛県八幡浜市八代"), CKLinkPoint(m_spFcKLinkPoint->GetFeature(6)).GetAddrName());
		}

		// プロット位置の都市地図行政界取得
		TEST_METHOD(KLink_GetCityAdmin)
		{
			Assert::IsNull(CKLinkPoint(m_spFcKLinkPoint->GetFeature(1)).GetCityAdmin());
			Assert::AreEqual((long)3, CKLinkPoint(m_spFcKLinkPoint->GetFeature(2)).GetCityAdmin()->GetOID());
		}
	};

	// staticメンバ変数定義
	std::shared_ptr<CFeatureClass> KLinkPointTest::m_spFcKLinkPoint;
}

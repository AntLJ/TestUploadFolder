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
	TEST_CLASS(BuildingTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcBuilding;

	public:
		// テストクラス初期化処理
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkSpace(verificationDatDir + _T("53392442.mdb"));
			m_spFcBuilding.reset(new CFeatureClass(cWorkSpace.OpenTable((building::kTableName))));
		}

		// テストクラス終了処理
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 建物種別取得
		TEST_METHOD(Bld_GetBldClass)
		{
			using namespace building::bld_class;
			Assert::IsTrue(kHospital == CBuilding(m_spFcBuilding->GetFeature(1)).GetBldClass() );
			Assert::IsTrue(kHotel == CBuilding(m_spFcBuilding->GetFeature(3)).GetBldClass() );
		}

		// 代表住所コード取得
		TEST_METHOD(Bld_GetRepAddrCode)
		{
			Assert::AreEqual(_T("38204002000000100000"), CBuilding(m_spFcBuilding->GetFeature(1)).GetRepAddrCode());
			Assert::AreEqual(_T("01101001001000120002"), CBuilding(m_spFcBuilding->GetFeature(4)).GetRepAddrCode());
		}
	};

	// staticメンバ変数定義
	std::shared_ptr<CFeatureClass> BuildingTest::m_spFcBuilding;
}

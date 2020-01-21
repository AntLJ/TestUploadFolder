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
	TEST_CLASS(ZipcodeAreaTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_spFcZipcodeArea;

	public:
		// テストクラス初期化処理
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkSpace(verificationDatDir + _T("ZipcodeArea.mdb"));
			m_spFcZipcodeArea.reset(new CFeatureClass(cWorkSpace.OpenTable((zipcode_area::kTableName))));
		}

		// テストクラス終了処理
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 郵便番号1、2取得
		TEST_METHOD(Zip_GetZipcode)
		{
			CZipcodeArea cZipArea(m_spFcZipcodeArea->GetFeature(1));
			Assert::AreEqual(_T("1234567"), cZipArea.GetZipcode1());
			Assert::AreEqual(_T("0987654"), cZipArea.GetZipcode2());
		}
	};

	// staticメンバ変数定義
	std::shared_ptr<CFeatureClass> ZipcodeAreaTest::m_spFcZipcodeArea;
}

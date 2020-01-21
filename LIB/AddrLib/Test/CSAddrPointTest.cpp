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
	TEST_CLASS(CSAddrPointTest)
	{
	private:
		static std::shared_ptr<CFeatureClass> m_fcCSAddrPoint;

	public:
		// ÉeÉXÉgÉNÉâÉXèâä˙âªèàóù
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
			CWorkspace cWorkSpace(verificationDatDir + _T("53392442.mdb"));
			m_fcCSAddrPoint.reset(new CFeatureClass(cWorkSpace.OpenTable(cs_addr_point::kTableName)));
		}

		// ÉeÉXÉgÉNÉâÉXèIóπèàóù
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 20åÖÉRÅ[ÉhéÊìæ
		TEST_METHOD(CS_GetAddrCode)
		{
			Assert::AreEqual(_T("38204002000012800013"), CCSAddrPoint(m_fcCSAddrPoint->GetFeature(1)).GetAddrCode());
			Assert::AreEqual(_T("38204002000000100000"), CCSAddrPoint(m_fcCSAddrPoint->GetFeature(3)).GetAddrCode());
		}

		// èZèäñºéÊìæ
		TEST_METHOD(CS_GetAddrName)
		{
			Assert::AreEqual(_T("à§ïQåßî™î¶ïlésåäà‰ÇPÇQÇWÇOÅ|ÇPÇR"), CCSAddrPoint(m_fcCSAddrPoint->GetFeature(1)).GetAddrName());
			Assert::AreEqual(_T("à§ïQåßî™î¶ïlésåäà‰ÇPÇO"), CCSAddrPoint(m_fcCSAddrPoint->GetFeature(3)).GetAddrName());
			Assert::AreEqual(_T("à§ïQåßî™î¶ïlésåäà‰"), CCSAddrPoint(m_fcCSAddrPoint->GetFeature(5)).GetAddrName());
		}

		// ÉvÉçÉbÉgà íuÇÃìsésínê}çsê≠äEéÊìæ
		TEST_METHOD(CS_GetCityAdmin)
		{
			Assert::IsNull(CCSAddrPoint(m_fcCSAddrPoint->GetFeature(1)).GetCityAdmin());
			Assert::AreEqual(long(10), CCSAddrPoint(m_fcCSAddrPoint->GetFeature(3)).GetCityAdmin()->GetOID());
		}
	};

	// staticÉÅÉìÉoïœêîíËã`
	std::shared_ptr<CFeatureClass> CSAddrPointTest::m_fcCSAddrPoint;
}

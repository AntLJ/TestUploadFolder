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
#include <sstream>
#include <atl2/io.h>

/**
 * @ingroup unit_test
 * @brief atl2::io::compatible の @ref unit_test クラス。
 */
class IoCompatibleTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(IoCompatibleTest);
	CPPUNIT_TEST(testW2A);
	CPPUNIT_TEST(testA2W);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	void testW2A()
	{
		static const wchar_t szSample[] = L"サンプル文字列";
		std::ostringstream ossActual, ossExpected;

		// 名前空間を有効にしていない場合はアドレスが出力される。		
		ossActual << szSample;
		ossExpected << static_cast<const void*>(szSample);

		CPPUNIT_ASSERT_EQUAL(ossExpected.str(), ossActual.str());

		ossActual.str("");
		ossExpected.str("");

		// 名前空間を有効にすることでワイド文字列からマルチバイト文字列への変換が働く。
		using namespace atl2::io::compatible;
		USES_CONVERSION;

		ossActual << szSample;
		ossExpected << W2CA(szSample);

		CPPUNIT_ASSERT_EQUAL(ossExpected.str(), ossActual.str());
	}

	void testA2W()
	{
		static const char szSample[] = "サンプル文字列";
		std::wostringstream ossActual, ossExpected;

		// 名前空間を有効にすることでマルチバイト文字列からワイド文字列への変換が働く。
		using namespace atl2::io::compatible;
		USES_CONVERSION;

		ossActual << szSample;
		ossExpected << A2CW(szSample);

		CPPUNIT_ASSERT(ossExpected.str() == ossActual.str());
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(IoCompatibleTest);

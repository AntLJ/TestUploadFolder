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
#include <string>
#include <rns/algorithm/string/url.h>

class DecodeURLStringTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DecodeURLStringTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	void test()
	{
		std::string str("http://www.google.co.jp/search?q=%3A-%29+%3A-%28+%3D-%29+B-%29+%3Ap+%3A-O");
		rns::algorithm::decode_url(str);

		CPPUNIT_ASSERT_EQUAL(
			std::string("http://www.google.co.jp/search?q=:-) :-( =-) B-) :p :-O"),
			str
		);
	}
 //@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DecodeURLStringTest);

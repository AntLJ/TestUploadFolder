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
#include <map>
#include <rns/functional.h>

typedef std::map<long, long> Map;

class MappedValueRefererTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(MappedValueRefererTest);
	CPPUNIT_TEST(testMutable);
	CPPUNIT_TEST(testConst);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	void testMutable()
	{
		Map aMap;
		aMap[1] = 10;
		aMap[3] = 30;

		typedef rns::mapped_value_referer<Map> Referer;

		const Referer aReferer(aMap);
		long* p;

		// 0
		CPPUNIT_ASSERT(! aReferer(0));

		// 1
		p = aReferer(1);
		CPPUNIT_ASSERT(p);
		*p = 11;
		CPPUNIT_ASSERT_EQUAL(11L, aMap[1]);

		// 2
		CPPUNIT_ASSERT(! aReferer(2));

		// 3
		p = aReferer(3);
		CPPUNIT_ASSERT(p);
		CPPUNIT_ASSERT_EQUAL(30L, *p);
	}

	void testConst()
	{
		Map aMap;
		aMap[1] = 10;
		aMap[3] = 30;

		typedef rns::mapped_value_referer<const Map> Referer;

		const Referer aReferer(aMap);
		const long* p;

		// 0
		CPPUNIT_ASSERT(! aReferer(0));

		// 1
		p = aReferer(1);
		CPPUNIT_ASSERT(p);
		CPPUNIT_ASSERT_EQUAL(10L, *p);

		// 2
		CPPUNIT_ASSERT(! aReferer(2));

		// 3
		p = aReferer(3);
		CPPUNIT_ASSERT(p);
		CPPUNIT_ASSERT_EQUAL(30L, *p);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(MappedValueRefererTest);

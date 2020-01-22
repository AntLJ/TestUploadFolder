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
#include <vector>
#include <list>
#include <rns/algorithm.h>

namespace {

template <typename InputIterator>
void evaluate(InputIterator first, InputIterator last)
{
	__w64 int n = std::distance(first, last);
	CPPUNIT_ASSERT_EQUAL(n, rns::distance_n(first, last, n));
	CPPUNIT_ASSERT_EQUAL(n, rns::distance_n(first, last, n + 1));
	CPPUNIT_ASSERT_EQUAL(n - 1, rns::distance_n(first, last, n - 1));
}

} // anonymous namespace

class DistanceNTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(DistanceNTest);
	CPPUNIT_TEST(testVector);
	CPPUNIT_TEST(testList);
	CPPUNIT_TEST(testArray);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	/**
	 * @brief std::vector のランダムアクセス反復子を検査。
	 */
	void testVector()
	{
		std::vector<int> aVec(5);
		evaluate(aVec.begin(), aVec.end());
	}

	/**
	 * @brief std::list の双方向反復子を検査。
	 */
	void testList()
	{
		std::list<int> aList(5);
		evaluate(aList.begin(), aList.end());
	}

	/**
	 * @brief ポインタをランダムアクセス反復子として検査。
	 */
	void testArray()
	{
		int rg[5];
		evaluate(rg, rg + sizeof(rg) / sizeof(int));
	}
 //@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(DistanceNTest);

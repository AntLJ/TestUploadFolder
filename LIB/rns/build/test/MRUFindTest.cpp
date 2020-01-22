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
#include <list>
#include <boost/assign/list_of.hpp>
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

class MRUFindTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(MRUFindTest);
	CPPUNIT_TEST(testList);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	void testList()
	{
		using namespace boost::assign;

		std::list<int> aList = list_of(1)(2)(3)(4)(5);
		std::list<int>::iterator it;
		std::ostringstream oss;

		it = rns::mru_find(aList, 3);
		std::copy(aList.begin(), aList.end(), std::ostream_iterator<int>(oss));
		CPPUNIT_ASSERT(it == aList.begin());
		CPPUNIT_ASSERT_EQUAL(std::string("31245"), oss.str());

		oss.str("");

		it = rns::mru_find(aList, 0);
		std::copy(aList.begin(), aList.end(), std::ostream_iterator<int>(oss));
		CPPUNIT_ASSERT(it == aList.end());
		CPPUNIT_ASSERT_EQUAL(std::string("31245"), oss.str());

		oss.str("");

		it = rns::mru_find_if(aList, std::bind2nd(std::greater<int>(), 3));
		std::copy(aList.begin(), aList.end(), std::ostream_iterator<int>(oss));
		CPPUNIT_ASSERT(it == aList.begin());
		CPPUNIT_ASSERT_EQUAL(std::string("43125"), oss.str());

		oss.str("");

		it = rns::mru_find_if(aList, std::bind2nd(std::less<int>(), 0));
		std::copy(aList.begin(), aList.end(), std::ostream_iterator<int>(oss));
		CPPUNIT_ASSERT(it == aList.end());
		CPPUNIT_ASSERT_EQUAL(std::string("43125"), oss.str());
	}
 //@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(MRUFindTest);

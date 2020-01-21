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

/**
 * @file RandomAccessIteratorTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <vector>
#include <boost/range_ex/algorithm.hpp>
#include <atl2/range.h>
#include <arctl/iterator/zero_based_random_access_iterators.h>
#include "Misc.h"

/**
 * @ingroup unit_test
 * @brief zero_based_random_access_iterator_traits と <atl2/range.h> のテスト
 */
class RandomAccessIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RandomAccessIteratorTest);
	CPPUNIT_TEST(testIntegral);
	CPPUNIT_TEST(testInterface);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{ 
	void testIntegral()
	{
		// 要素用意。
		ILongArrayPtr ipLongArray(CLSID_LongArray);
		std::vector<long> aVector;

		CPPUNIT_ASSERT(boost::empty(ipLongArray));

		for(long i = 0; i < 5; i++) {
			ipLongArray->Add(i);
			aVector.push_back(i);
		}

		CPPUNIT_ASSERT(! boost::empty(ipLongArray));

		// 検証
		checkEqual(aVector, ipLongArray);
	}

	void testInterface()
	{
		DummyUnknown aDummyUnknowns[5];
		std::vector<DummyUnknown*> aVector;
		IArrayPtr ipArray(CLSID_Array);

		CPPUNIT_ASSERT(boost::empty(ipArray));

		for(int i = 0; i < 5; i++) {
			aVector.push_back(aDummyUnknowns + i);
			CPPUNIT_ASSERT_EQUAL(S_OK, ipArray->Add(aDummyUnknowns + i));
		}

		CPPUNIT_ASSERT(! boost::empty(ipArray));

		// 検証
		checkEqual(aVector, ipArray);
	}

	template <typename T1, typename T2>
	void checkEqual(const std::vector<T1>& rExpect, const T2& rpActual)
	{
		// 数量検査。
		long nCount;
		rpActual->get_Count(&nCount);
		CPPUNIT_ASSERT_EQUAL(static_cast<long>(rExpect.size()), nCount);
		CPPUNIT_ASSERT_EQUAL(static_cast<typename boost::range_size<T2>::type>(nCount), boost::size(rpActual));

		// 順方向走査テスト1。
		CPPUNIT_ASSERT(std::equal(rExpect.begin(), rExpect.end(), atl2::begin(rpActual)));

		// 順方向走査テスト2。
		CPPUNIT_ASSERT(std::equal(atl2::begin(rpActual), atl2::end(rpActual), rExpect.begin()));

		// 順方向走査テスト3。
		CPPUNIT_ASSERT(std::equal(atl2::begin(rpActual), atl2::end<T2::Interface>(), rExpect.begin()));

		// 順方向走査テスト4。
		CPPUNIT_ASSERT(std::equal(boost::begin(rpActual), boost::end(rpActual), rExpect.begin()));

		// 順方向走査テスト5。
		CPPUNIT_ASSERT(boost::equal(rpActual, rExpect.begin()));

		// 逆方向走査テスト1。
		CPPUNIT_ASSERT(std::equal(rExpect.rbegin(), rExpect.rend(), atl2::rbegin(rpActual)));

		// 逆方向走査テスト2。
		CPPUNIT_ASSERT(std::equal(atl2::rbegin(rpActual), atl2::rend(rpActual), rExpect.rbegin()));

		// 逆方向走査テスト3。
		CPPUNIT_ASSERT(std::equal(boost::rbegin(rpActual), boost::rend(rpActual), rExpect.rbegin()));

		// std::advance() std::distance() 特殊化テスト
		boost::range_iterator<T2>::type it(boost::begin(rpActual));
		std::advance(it, 3);
		CPPUNIT_ASSERT_EQUAL(std::size_t(2), std::distance(it, boost::end(rpActual)));

		boost::range_reverse_iterator<T2>::type rit(boost::rbegin(rpActual));
		std::advance(rit, 2);
		CPPUNIT_ASSERT_EQUAL(std::size_t(3), std::distance(rit, boost::rend(rpActual)));
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(RandomAccessIteratorTest);

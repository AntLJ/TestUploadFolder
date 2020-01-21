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
 * @file SinglePassIteratorTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <vector>
#include <boost/range_ex/algorithm.hpp>
#include <atl2/seal.h>
#include <atl2/iterator/iterator_base.h>
#include <atl2/iterator/single_pass_iterator.h>
#include <atl2/range.h>
#include "Misc.h"

/// @b ISet コンテナ特性。
template <> struct atl2::container_pointer_traits<ISet*> :
	public atl2::enumeration_container_traits<ISet, LPUNKNOWN, &ISet::Next> {};

/**
 * @ingroup unit_test
 * @brief atl2::enumeration_container_traits と <atl2/range.h> のテスト
 *
 * atl2::single_pass_container_traits をテストするクラスだったはずなのだが……
 */
class SinglePassIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(SinglePassIteratorTest);
	CPPUNIT_TEST(testInterface);
	CPPUNIT_TEST(testBackInsert);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{ 
	void testInterface()
	{
		DummyUnknown aDummyUnknowns[5];
		std::set<IUnknown*> aSet1, aSet2, aSet3;
		_ISetPtr ipSet(CLSID_Set);

		for(int i = 0; i < 5; i++) {
			aSet1.insert(aDummyUnknowns + i);
			CPPUNIT_ASSERT_EQUAL(S_OK, ipSet->Add(aDummyUnknowns + i));
		}

		// 数量比較。
		CPPUNIT_ASSERT_EQUAL(static_cast<long>(aSet1.size()), ipSet->GetCount());

		// 取り出し。
		std::copy(boost::begin(ipSet), boost::end(ipSet), std::inserter(aSet2, aSet2.end()));

		// 比較。
		CPPUNIT_ASSERT_EQUAL(aSet1.size(), aSet2.size());
		CPPUNIT_ASSERT(std::equal(aSet1.begin(), aSet1.end(), aSet2.begin()));

		// Rangeで取り出し
		boost::copy(ipSet, std::inserter(aSet3, aSet3.end()));

		// 比較
		CPPUNIT_ASSERT_EQUAL(aSet1.size(), aSet3.size());
		CPPUNIT_ASSERT(std::equal(aSet1.begin(), aSet1.end(), aSet3.begin()));
	}

	/**
	 * @note どちらかというと atl2::seal のテスト用。
	 */
	void testBackInsert()
	{
		DummyUnknown aDummyUnknowns[5];
		_ISetPtr ipSet(CLSID_Set);

		for(int i = 0; i < 5; i++) {
			CPPUNIT_ASSERT_EQUAL(S_OK, ipSet->Add(aDummyUnknowns + i));
		}

		std::vector<atl2::seal<IUnknownPtr> > aList1;

		// 複写。
		std::copy(atl2::begin(ipSet), atl2::end(ipSet), std::back_inserter(aList1));

		// 比較。
		CPPUNIT_ASSERT_EQUAL(S_OK, ipSet->Reset());
		CPPUNIT_ASSERT_EQUAL(ipSet->GetCount(), static_cast<long>(aList1.size()));
		CPPUNIT_ASSERT(std::equal(aList1.begin(), aList1.end(), atl2::begin(ipSet)));
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SinglePassIteratorTest);

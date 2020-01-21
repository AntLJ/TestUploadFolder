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
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <boost/utility.hpp>
#include <boost/assign/std/vector.hpp>
#include <sindy/algorithm/transform_to_nq.h>

using namespace sindy;
using namespace sindy::road_link;
using namespace sindy::link_queue;
using namespace boost::assign;

/**
 * @ingroup unit_test
 * @brief transform_to_nq() アルゴリズムの @ref unit_test クラス。
 */
class TransformToNqTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(TransformToNqTest);
	CPPUNIT_TEST(test1fore1);
	CPPUNIT_TEST(test1fore2);
	CPPUNIT_TEST(test1reverse1);
	CPPUNIT_TEST(test1reverse2);
	CPPUNIT_TEST(test1composite1);
	CPPUNIT_TEST(test1ng1);
	CPPUNIT_TEST(test1ng2);
	CPPUNIT_TEST(test1ng3);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
	}

	virtual void tearDown()
	{
	}

//@{ @name テストケース
	/// 順方向単体テスト。
	void test1fore1()
	{
		Value1 aValue = gen(1L, 2L, link_dir::kFore);

		std::vector<long> aExpected, aActual;
		aExpected += 1, 2;
		transform_to_nq(&aValue, &aValue + 1, std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}

	/// 順方向複数テスト。
	void test1fore2()
	{
		std::vector<Value1> aValueList;
		aValueList += gen(1L, 2L, link_dir::kFore), gen(2L, 3L, link_dir::kFore);

		std::vector<long> aExpected, aActual;
		aExpected += 1, 2, 3;
		transform_to_nq(aValueList.begin(), aValueList.end(), std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}

	/// 逆方向単体テスト。
	void test1reverse1()
	{
		Value1 aValue = gen(1L, 2L, link_dir::kReverse);

		std::vector<long> aExpected, aActual;
		aExpected += 2, 1;
		transform_to_nq(&aValue, &aValue + 1, std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}

	/// 逆方向複数テスト。
	void test1reverse2()
	{
		std::vector<Value1> aValueList;
		aValueList += gen(1L, 2L, link_dir::kReverse), gen(3L, 1L, link_dir::kReverse);

		std::vector<long> aExpected, aActual;
		aExpected += 2, 1, 3;
		transform_to_nq(aValueList.begin(), aValueList.end(), std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}

	/// 複合テスト。
	void test1composite1()
	{
		std::vector<Value1> aValueList;
		aValueList += gen(1L, 2L, link_dir::kReverse), gen(1L, 3L, link_dir::kFore), gen(4L, 3L, link_dir::kReverse);

		std::vector<long> aExpected, aActual;
		aExpected += 2, 1, 3, 4;
		transform_to_nq(aValueList.begin(), aValueList.end(), std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}

	/// 失敗テスト1:向きの食い違い。
	void test1ng1()
	{
		std::vector<Value1> aValueList;
		aValueList += gen(1L, 2L, link_dir::kFore), gen(2L, 3L, link_dir::kReverse);

		std::vector<long> aExpected, aActual;
		aExpected += 1, 2;
		transform_to_nq(aValueList.begin(), aValueList.end(), std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}

	/// 失敗テスト2:向き属性値の不正・単体。
	void test1ng2()
	{
		Value1 aValue = gen(1L, 2L, 0);
		std::vector<long> aActual;
		transform_to_nq(&aValue, &aValue + 1, std::back_inserter(aActual));
		CPPUNIT_ASSERT(aActual.empty());
	}

	/// 失敗テスト3:向き属性値の不正・複数。
	void test1ng3()
	{
		std::vector<Value1> aValueList;
		aValueList += gen(1L, 2L, link_dir::kFore), gen(2L, 3L, 0);

		std::vector<long> aExpected, aActual;
		aExpected += 1, 2;
		transform_to_nq(aValueList.begin(), aValueList.end(), std::back_inserter(aActual));
		CPPUNIT_ASSERT(aExpected == aActual);
	}
//@}

private:
// 定義
	ARCTL_FIELD_VALUE_CLASS(
		Value1,
		(2(road_link::kFromNodeId, long))
		(2(road_link::kToNodeId, long))
		(2(link_queue::kLinkDir, long))
	);

// 静的生成
	static Value1 gen(long nFromNodeId, long nToNodeId, long nLinkDir)
	{
		using namespace arctl::field;
		Value1 aValue;
		get<kFromNodeId>(aValue) = nFromNodeId;
		get<kToNodeId>(aValue) = nToNodeId;
		get<kLinkDir>(aValue) = nLinkDir;
		return aValue;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(TransformToNqTest);

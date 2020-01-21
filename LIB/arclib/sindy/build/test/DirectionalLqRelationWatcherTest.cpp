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
#include <set>
#include <algorithm>
#include <sindy/lq_relation/bidirectional.h>
#include <sindy/lq_relation/directional_watcher.h>
#include <SiNDYAttrCommunity.h>

using namespace sindy;
using namespace sindy::link_queue;

/**
 * @ingroup unit_test
 */
class DirectionalLqRelationWatcherTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(DirectionalLqRelationWatcherTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testInsertNewLink);
	CPPUNIT_TEST(testInsertUni1);
	CPPUNIT_TEST(testInsertUni2);
	CPPUNIT_TEST(testDeleteUni1);
	CPPUNIT_TEST(testDeleteUni2);
	CPPUNIT_TEST(testDeleteBi);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		boost::shared_ptr<directional_lq_relation_mutable> pMutble(new bidirectional_lq_relation());

		CPPUNIT_ASSERT(pMutble->attach_lq(1, link_dir::kFore, 1));
		CPPUNIT_ASSERT(pMutble->attach_lq(2, link_dir::kReverse, 2));
		CPPUNIT_ASSERT(pMutble->attach_lq(3, link_dir::kFore, 3));
		CPPUNIT_ASSERT(pMutble->attach_lq(3, link_dir::kReverse, 4));

		m_pWatcher = new directional_lq_relation_watcher(pMutble);
	}

	virtual void tearDown()
	{
		delete m_pWatcher;
	}

//@{ @name テストケース
	void testConstruct()
	{
		// 存在確認の透過テスト。
		CPPUNIT_ASSERT(m_pWatcher->exist_lq(1));
		CPPUNIT_ASSERT(m_pWatcher->exist_lq(2));
		CPPUNIT_ASSERT(m_pWatcher->exist_lq(3));
		CPPUNIT_ASSERT(! m_pWatcher->exist_lq(4));

		CPPUNIT_ASSERT(m_pWatcher->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pWatcher->exist_directional_lq(1, link_dir::kReverse));

		CPPUNIT_ASSERT(! m_pWatcher->exist_directional_lq(2, link_dir::kFore));
		CPPUNIT_ASSERT(m_pWatcher->exist_directional_lq(2, link_dir::kReverse));

		CPPUNIT_ASSERT(m_pWatcher->exist_directional_lq(3, link_dir::kFore));
		CPPUNIT_ASSERT(m_pWatcher->exist_directional_lq(3, link_dir::kReverse));
	}

	/// 新たなリンクに対する関連付けのテスト。
	void testInsertNewLink()
	{
		std::map<long, boost::tribool> aExpectedMap;

		// 新たなリンクに順方向Lqを関連付ける。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(4, link_dir::kFore, 5));
		aExpectedMap[4] = true;
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// 逆方向Lqを関連付けても更新情報は変わらない。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(4, link_dir::kReverse, 6));
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// 片方向Lqをはがしても更新情報は変わらない。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(4, 5));
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// 両方向Lqをはがした時点で、更新情報はなくなる。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(4, 6));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// 片方向にLqが付いているリンクに対する関連付けのテスト1。
	void testInsertUni1()
	{
		// 逆方向Lqを関連付ける。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kReverse, 5));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));

		// 逆方向Lqをはがす。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 5));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// 片方向にLqが付いているリンクに対する関連付けのテスト2。
	void testInsertUni2()
	{
		// 逆方向Lqを関連付け、順方向Lqをはがす。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kReverse, 5));
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 1));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// 片方向にLqが付いているリンクのLqはがし・付け替えテスト1。
	void testDeleteUni1()
	{
		std::map<long, boost::tribool> aExpectedMap;

		// 順方向Lqをはがす。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 1));
		aExpectedMap[1] = false;
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));
	
		// 順方向Lqを関連付ける。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kFore, 1));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// 片方向にLqが付いているリンクのLqはがしテスト2。
	void testDeleteUni2()
	{
		// 順方向Lqをはがし、逆方向Lqを関連付ける。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 1));
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kReverse, 5));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// 両方向にLqが付いているリンクのLqはがし・付け替えテスト。
	void testDeleteBi()
	{
		std::map<long, boost::tribool> aExpectedMap;

		// 順方向Lqをはがす。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(3, 3));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));

		// 逆方向Lqをはがす。
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(3, 4));
		aExpectedMap[3] = false;
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// 順方向Lqを再付与する。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(3, link_dir::kFore, 3));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));

		// 逆方向Lqを再付与する。
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(3, link_dir::kReverse, 4));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}
//@}

private:
// 処理
	template <typename TInputeIterator, typename TExpectedContainer>
	static bool equal(TInputeIterator first, TInputeIterator last, const TExpectedContainer& rExpectedContainer)
	{
		CPPUNIT_ASSERT_EQUAL(rExpectedContainer.size(), static_cast<unsigned int>(std::distance(first, last)));
		return std::equal(first, last, rExpectedContainer.begin());
	}

// 変数
	directional_lq_relation_watcher* m_pWatcher;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DirectionalLqRelationWatcherTest);

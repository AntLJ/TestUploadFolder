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

//@{ @name �e�X�g�P�[�X
	void testConstruct()
	{
		// ���݊m�F�̓��߃e�X�g�B
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

	/// �V���ȃ����N�ɑ΂���֘A�t���̃e�X�g�B
	void testInsertNewLink()
	{
		std::map<long, boost::tribool> aExpectedMap;

		// �V���ȃ����N�ɏ�����Lq���֘A�t����B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(4, link_dir::kFore, 5));
		aExpectedMap[4] = true;
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// �t����Lq���֘A�t���Ă��X�V���͕ς��Ȃ��B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(4, link_dir::kReverse, 6));
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// �Е���Lq���͂����Ă��X�V���͕ς��Ȃ��B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(4, 5));
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// ������Lq���͂��������_�ŁA�X�V���͂Ȃ��Ȃ�B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(4, 6));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// �Е�����Lq���t���Ă��郊���N�ɑ΂���֘A�t���̃e�X�g1�B
	void testInsertUni1()
	{
		// �t����Lq���֘A�t����B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kReverse, 5));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));

		// �t����Lq���͂����B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 5));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// �Е�����Lq���t���Ă��郊���N�ɑ΂���֘A�t���̃e�X�g2�B
	void testInsertUni2()
	{
		// �t����Lq���֘A�t���A������Lq���͂����B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kReverse, 5));
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 1));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// �Е�����Lq���t���Ă��郊���N��Lq�͂����E�t���ւ��e�X�g1�B
	void testDeleteUni1()
	{
		std::map<long, boost::tribool> aExpectedMap;

		// ������Lq���͂����B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 1));
		aExpectedMap[1] = false;
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));
	
		// ������Lq���֘A�t����B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kFore, 1));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// �Е�����Lq���t���Ă��郊���N��Lq�͂����e�X�g2�B
	void testDeleteUni2()
	{
		// ������Lq���͂����A�t����Lq���֘A�t����B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(1, 1));
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(1, link_dir::kReverse, 5));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}

	/// ��������Lq���t���Ă��郊���N��Lq�͂����E�t���ւ��e�X�g�B
	void testDeleteBi()
	{
		std::map<long, boost::tribool> aExpectedMap;

		// ������Lq���͂����B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(3, 3));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));

		// �t����Lq���͂����B
		CPPUNIT_ASSERT(m_pWatcher->detach_lq(3, 4));
		aExpectedMap[3] = false;
		CPPUNIT_ASSERT(equal(m_pWatcher->begin(), m_pWatcher->end(), aExpectedMap));

		// ������Lq���ĕt�^����B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(3, link_dir::kFore, 3));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));

		// �t����Lq���ĕt�^����B
		CPPUNIT_ASSERT(m_pWatcher->attach_lq(3, link_dir::kReverse, 4));
		CPPUNIT_ASSERT_EQUAL(0, std::distance(m_pWatcher->begin(), m_pWatcher->end()));
	}
//@}

private:
// ����
	template <typename TInputeIterator, typename TExpectedContainer>
	static bool equal(TInputeIterator first, TInputeIterator last, const TExpectedContainer& rExpectedContainer)
	{
		CPPUNIT_ASSERT_EQUAL(rExpectedContainer.size(), static_cast<unsigned int>(std::distance(first, last)));
		return std::equal(first, last, rExpectedContainer.begin());
	}

// �ϐ�
	directional_lq_relation_watcher* m_pWatcher;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DirectionalLqRelationWatcherTest);

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
#include <memory>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <sindy/lq_relation/directional_interface.h>
#include <SiNDYAttrCommunity.h>

/**
 * @ingroup unit_test
 */
class BidirectionalLqRelationTestBase : public CppUnit::TestFixture 
{
public:
//@{ @name テストケース
	/// 構築テスト。
	void testConstruct()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// Lqが関連付けられていないことを確認する。
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kReverse));
	}

	/// 単方向テスト。
	void testUnidirection()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// 正方向Lqの追加に成功する。
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, 1, link_dir::kFore));
		CPPUNIT_ASSERT(m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kReverse));

		// 余計なリンクにLqが関連付けられていない。
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(2));

		// 存在しないリンクのLqの削除に失敗する。
		CPPUNIT_ASSERT(! m_pRelation->detach_lq(2, 1));

		// 存在しないLqの削除に失敗する。
		CPPUNIT_ASSERT(! m_pRelation->detach_lq(1, 2));

		// 存在するLqの削除に成功する。
		CPPUNIT_ASSERT(m_pRelation->detach_lq(1, 1));
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kFore));
	}

	/// 単方向重複禁止テスト。
	void testDuplicate()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// 同方向Lqの追加には失敗する
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, 1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pRelation->attach_lq(1, 1, link_dir::kFore));
	}

	/// 双方向テスト。
	void testBidirection()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// 双方向Lqの追加に成功する。
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, link_dir::kFore, 1));
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, link_dir::kReverse, 2));
		CPPUNIT_ASSERT(m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kReverse));

		// 片方向のLqの削除に成功する。
		CPPUNIT_ASSERT(m_pRelation->detach_lq(1, 1));
		CPPUNIT_ASSERT(m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kReverse));

		// Lqを完全に削除する。
		CPPUNIT_ASSERT(m_pRelation->detach_lq(1, 2));
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kReverse));
	}
//@}

protected:
	sindy::directional_lq_relation_mutable* m_pRelation;
};

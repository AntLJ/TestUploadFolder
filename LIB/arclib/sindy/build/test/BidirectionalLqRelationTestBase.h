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
//@{ @name �e�X�g�P�[�X
	/// �\�z�e�X�g�B
	void testConstruct()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// Lq���֘A�t�����Ă��Ȃ����Ƃ��m�F����B
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kReverse));
	}

	/// �P�����e�X�g�B
	void testUnidirection()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// ������Lq�̒ǉ��ɐ�������B
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, 1, link_dir::kFore));
		CPPUNIT_ASSERT(m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kReverse));

		// �]�v�ȃ����N��Lq���֘A�t�����Ă��Ȃ��B
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(2));

		// ���݂��Ȃ������N��Lq�̍폜�Ɏ��s����B
		CPPUNIT_ASSERT(! m_pRelation->detach_lq(2, 1));

		// ���݂��Ȃ�Lq�̍폜�Ɏ��s����B
		CPPUNIT_ASSERT(! m_pRelation->detach_lq(1, 2));

		// ���݂���Lq�̍폜�ɐ�������B
		CPPUNIT_ASSERT(m_pRelation->detach_lq(1, 1));
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kFore));
	}

	/// �P�����d���֎~�e�X�g�B
	void testDuplicate()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// ������Lq�̒ǉ��ɂ͎��s����
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, 1, link_dir::kFore));
		CPPUNIT_ASSERT(! m_pRelation->attach_lq(1, 1, link_dir::kFore));
	}

	/// �o�����e�X�g�B
	void testBidirection()
	{
		using namespace sindy;
		using namespace sindy::link_queue;

		// �o����Lq�̒ǉ��ɐ�������B
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, link_dir::kFore, 1));
		CPPUNIT_ASSERT(m_pRelation->attach_lq(1, link_dir::kReverse, 2));
		CPPUNIT_ASSERT(m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kReverse));

		// �Е�����Lq�̍폜�ɐ�������B
		CPPUNIT_ASSERT(m_pRelation->detach_lq(1, 1));
		CPPUNIT_ASSERT(m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kFore));
		CPPUNIT_ASSERT(m_pRelation->exist_directional_lq(1, link_dir::kReverse));

		// Lq�����S�ɍ폜����B
		CPPUNIT_ASSERT(m_pRelation->detach_lq(1, 2));
		CPPUNIT_ASSERT(! m_pRelation->exist_lq(1));
		CPPUNIT_ASSERT(! m_pRelation->exist_directional_lq(1, link_dir::kReverse));
	}
//@}

protected:
	sindy::directional_lq_relation_mutable* m_pRelation;
};

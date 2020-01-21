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
#include <sindy/lq_relation/bidirectional.h>
#include "BidirectionalLqRelationTestBase.h"

using namespace sindy;
using namespace sindy::link_queue;

/**
 * @ingroup unit_test
 */
class BidirectionalLqRelationTest : public BidirectionalLqRelationTestBase 
{
	CPPUNIT_TEST_SUITE(BidirectionalLqRelationTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testUnidirection);
	CPPUNIT_TEST(testDuplicate);
	CPPUNIT_TEST(testBidirection);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		m_pRelation = new bidirectional_lq_relation();
	}

	virtual void tearDown()
	{
		delete m_pRelation;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(BidirectionalLqRelationTest);

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
#include <boost/assign/std/vector.hpp>
#include <boost/utility.hpp>
#include <rns/algorithm.h>

class OverlaTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(OverlaTest);
	CPPUNIT_TEST(testEqual);
	CPPUNIT_TEST(testSuccess1);
	CPPUNIT_TEST(testFail1);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		m_pList1 = new std::vector<int>;
		m_pList2 = new std::vector<int>;
		m_pExpect = new std::vector<int>;
	}

	virtual void tearDown()
	{
		delete m_pExpect;
		delete m_pList2;
		delete m_pList1;
	}

/// @name �e�X�g�P�[�X
//@{
	/**
	 * @brief ���S��v����P�[�X�B
	 */
	void testEqual()
	{
		using namespace boost::assign;

		*m_pList1  += 1,2,3,4,5;
		*m_pList2  += 1,2,3,4,5;
		*m_pExpect += 1,2,3,4,5;

		evaluate();
	}

	/**
	 * @brief �͈�1�̌㔼�Ɣ͈�2�̑O������v����P�[�X�B
	 */
	void testSuccess1()
	{
		using namespace boost::assign;

		*m_pList1  += 1,2,3,4,5;
		*m_pList2  +=     3,4,5,6;
		*m_pExpect +=     3,4,5;

		evaluate();
	}

	/**
	 * @brief �͈�2���͈�1��[�n�_, �I�_)�ɂ���P�[�X�B
	 */
	void testFail1()
	{
		using namespace boost::assign;

		*m_pList1  += 1,2,3,4,5;
		*m_pList2  +=     3,4;

		evaluate();
	}

 //@}
private:
	void evaluate()
	{
		std::vector<int> aActual;

		std::copy(
			rns::overlap(
				boost::prior(m_pList1->end(), std::min<int>(m_pList1->size(), m_pList2->size())),
				m_pList1->end(),
				m_pList2->begin()
			),
			m_pList1->end(),
			std::back_inserter(aActual)
		);

		CPPUNIT_ASSERT(*m_pExpect == aActual);
	}

	std::vector<int>* m_pList1;
	std::vector<int>* m_pList2;
	std::vector<int>* m_pExpect;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OverlaTest);

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
#include <rns/algorithm.h>

namespace {

template <typename InputRange, typename OutputIterator>
OutputIterator copy_range(InputRange cRange, OutputIterator result)
{
	return std::copy(cRange.first, cRange.second, result);
}

} // anonymous namespace

class IntersectionRangeTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(IntersectionRangeTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(test1B2F);
	CPPUNIT_TEST(test1F2B);
	CPPUNIT_TEST(test1Contain2);
	CPPUNIT_TEST(test2Contain1);
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

/// @name テストケース
//@{
	/**
	 * @brief 適当なテストケース。
	 */
	void test1()
	{
		using namespace boost::assign;

		*m_pList1  += 1,2,3,4,5;
		*m_pList2  +=   2,3  ,5,6;
		*m_pExpect +=   2,3;

		evaluate();
	}

	/**
	 * @brief 範囲1の後半と範囲2の前半が一致するケース。
	 */
	void test1B2F()
	{
		using namespace boost::assign;

		*m_pList1  += 1,2,3,4,5;
		*m_pList2  +=     3,4,5,6;
		*m_pExpect +=     3,4,5;

		evaluate();
	}

	/**
	 * @brief 範囲1の前半と範囲2の後半が一致するケース。
	 */
	void test1F2B()
	{
		using namespace boost::assign;

		*m_pList1  +=     3,4,5,6;
		*m_pList2  += 1,2,3,4,5;
		*m_pExpect +=     3,4,5;

		evaluate();
	}

	/**
	 * @brief 範囲1が範囲2を含むケース
	 */
	void test1Contain2()
	{
		using namespace boost::assign;

		*m_pList1  += 1,2,3,4,5,6;
		*m_pList2  +=   2,3,4;
		*m_pExpect +=   2,3,4;

		evaluate();
	}

	/**
	 * @brief 範囲2が範囲1を含むケース
	 */
	void test2Contain1()
	{
		using namespace boost::assign;

		*m_pList1  +=     3,4,5;
		*m_pList2  += 1,2,3,4,5,6;
		*m_pExpect +=     3,4,5;

		evaluate();
	}
//@}

private:
	void evaluate()
	{
		std::vector<int> aActual;

		copy_range(
			rns::intersection_range(m_pList1->begin(), m_pList1->end(), m_pList2->begin(), m_pList2->end(), std::equal_to<int>()),
			std::back_inserter(aActual)
		);

		CPPUNIT_ASSERT(*m_pExpect == aActual);
	}

	std::vector<int>* m_pList1;
	std::vector<int>* m_pList2;
	std::vector<int>* m_pExpect;
};

class IntersectionRangeDTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(IntersectionRangeDTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(testLessDistance);
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

/// @name テストケース
//@{
	/**
	 * @brief 適当なテストケース。
	 */
	void test1()
	{
		using namespace boost::assign;

		m_nDist = 2;
		*m_pList1  += 1,2,3,4,5,6;
		*m_pList2  +=   2,  4,5;
		*m_pExpect +=       4,5;

		evaluate();
	}

	/**
	 * @brief 重なる部分はあるが、範囲長が足りないケース。
	 */
	void testLessDistance()
	{
		using namespace boost::assign;

		m_nDist = 2;
		*m_pList1  += 1,2,3,4,5;
		*m_pList2  +=   2,  4;

		evaluate();
	}
//@}

protected:
	virtual void evaluate()
	{
		std::vector<int> aActual;

		copy_range(
			rns::intersection_range_d(m_pList1->begin(), m_pList1->end(), m_pList2->begin(), m_pList2->end(), m_nDist, std::equal_to<int>()),
			std::back_inserter(aActual)
		);

		CPPUNIT_ASSERT(*m_pExpect == aActual);
	}

	int m_nDist;
	std::vector<int>* m_pList1;
	std::vector<int>* m_pList2;
	std::vector<int>* m_pExpect;
};

class ReversibleIntersectionRangeDTest : public IntersectionRangeDTest 
{
	CPPUNIT_TEST_SUB_SUITE(ReversibleIntersectionRangeDTest, IntersectionRangeDTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(testFirstFore);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	/**
	 * @brief 適当なテストケース。
	 */
	void test1()
	{
		using namespace boost::assign;

		m_nDist = 2;
		*m_pList1  += 1,2,3,4,5,6;
		*m_pList2  += 2,    5,4;
		*m_pExpect +=       4,5;

		evaluate();
	}

	/**
	 * @brief 順方向一致が優先されることの保証。
	 */
	void testFirstFore()
	{
		using namespace boost::assign;

		m_nDist = 2;
		*m_pList1  += 1,2,3,4,5,6;
		*m_pList2  +=       4,5,3,2,1;
		*m_pExpect +=       4,5;

		evaluate();
	}

//@}

protected:
	virtual void evaluate()
	{
		std::vector<int> aActual;

		copy_range(
			rns::reversible_intersection_range_d(m_pList1->begin(), m_pList1->end(), m_pList2->begin(), m_pList2->end(), m_nDist, std::equal_to<int>()),
			std::back_inserter(aActual)
		);

		CPPUNIT_ASSERT(*m_pExpect == aActual);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(IntersectionRangeTest);
CPPUNIT_TEST_SUITE_REGISTRATION(IntersectionRangeDTest);
CPPUNIT_TEST_SUITE_REGISTRATION(ReversibleIntersectionRangeDTest);

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
#include <sstream>
#include <boost/assign/std/vector.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <rns/iterator/any_output_iterator.h>

namespace {

typedef rns::any_output_iterator<long> long_output_iterator;

template <typename InputIterator>
long_output_iterator any_copy(InputIterator first, InputIterator last, long_output_iterator result)
{
	return std::copy(first, last, result);
}

} // anonymous namespace

/**
 * @ingroup unit_test
 * @brief rns::any_output_iterator の@ref unit_testクラス。
 */
class AnyOutputIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(AnyOutputIteratorTest);
	CPPUNIT_TEST(testOStream);
	CPPUNIT_TEST(testSet);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		using namespace boost::assign;

		m_pSrc = new std::vector<long>();
		*m_pSrc += 1,4,3,5,2;
	}

	virtual void tearDown()
	{
		delete m_pSrc;
	}

/// @name テストケース
//@{
	/**
	 * @brief rns::any_output_iterator に ostream_iterator を入れてみるテスト。
	 */
	void testOStream()
	{
		std::ostringstream ossExpect, ossActual;

		ossExpect << "14352";
		any_copy(
			m_pSrc->begin(),
			m_pSrc->end(),
			long_output_iterator(std::ostream_iterator<long>(ossActual))
		);

		CPPUNIT_ASSERT_EQUAL(ossExpect.str(), ossActual.str());
	}

	/**
	 * @brief rns::any_output_iterator に insert_iterator を入れてみるテスト。
	 */
	void testSet()
	{
		std::set<long> aActual;

		any_copy(
			m_pSrc->begin(),
			m_pSrc->end(),
			long_output_iterator(std::inserter(aActual, aActual.end()))
		);

		std::ostringstream ossExpect, ossActual;

		ossExpect << "12345";
		std::copy(aActual.begin(), aActual.end(), std::ostream_iterator<long>(ossActual));

		CPPUNIT_ASSERT_EQUAL(ossExpect.str(), ossActual.str());
	}
//@}

private:
	std::vector<long>* m_pSrc;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AnyOutputIteratorTest);

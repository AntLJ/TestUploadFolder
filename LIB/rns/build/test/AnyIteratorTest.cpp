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
#include <algorithm>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <rns/iterator/any_iterator.h>

namespace {

typedef rns::any_iterator<char, boost::single_pass_traversal_tag, char> char_input_iterator;

template <typename OutputIterator>
OutputIterator any_copy(char_input_iterator first, char_input_iterator last, OutputIterator result)
{
	return std::copy(first, last, result);
}

} // anonymous namespace

/**
 * @ingroup unit_test
 * @brief rns::any_iterator ��@ref unit_test�N���X�B
 */
class AnyInputIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(AnyInputIteratorTest);
	CPPUNIT_TEST(testArray);
	CPPUNIT_TEST(testIStream);
	CPPUNIT_TEST_EXCEPTION(testBadCast, std::bad_cast);
	CPPUNIT_TEST(testAnyToAny);
	CPPUNIT_TEST(baseExtract);
	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * @brief any_input_iterator �ɕ����z����i�[����e�X�g�B
	 */
	void testArray()
	{
		static const char* lpszSrc = "14352";

		std::ostringstream ossExpect, ossActual;

		ossExpect << lpszSrc;

		any_copy(
			char_input_iterator(lpszSrc),
			char_input_iterator(lpszSrc + ::strlen(lpszSrc)),
			std::ostream_iterator<char>(ossActual)
		);

		CPPUNIT_ASSERT_EQUAL(ossExpect.str(), ossActual.str());
	}

	/**
	 * @brief any_input_iterator �� std::istream_iterator<char> ���i�[����e�X�g�B
	 */
	void testIStream()
	{
		std::stringstream ssExpect, ssActual;

		ssExpect << "14352";

		any_copy(
			char_input_iterator(std::istream_iterator<char>(ssExpect)),
			char_input_iterator(std::istream_iterator<char>()),
			std::ostream_iterator<char>(ssActual)
		);

		CPPUNIT_ASSERT_EQUAL(ssExpect.str(), ssActual.str());
	}

	/**
	 * @brief �قȂ�^�̔����q���m�̔�r�����݂邱�Ƃ� std::bad_cast ��O���������邱�Ƃ��m�F����e�X�g�B
	 */
	void testBadCast()
	{
		static const char* lpszSrc = "14352";

		std::stringstream ssSrc, ssDst;

		ssSrc << lpszSrc;

		any_copy(
			lpszSrc,
			std::istream_iterator<char>(ssSrc),
			std::ostream_iterator<char>(ssDst)
		);
	}

	/**
	 * @brief ���z�����q�̌����I�ȕ��������B
	 */
	void testAnyToAny()
	{
		typedef rns::any_iterator<char, boost::bidirectional_traversal_tag, char> char_bidirectional_iterator;

		static const char* lpszSrc = "14352";

		char_bidirectional_iterator itSrc(lpszSrc);

		// ���������B
		char_input_iterator itDst(itSrc);

		// ��������B
		itDst = itSrc;
	}

	/**
	 * @brief ���z�����q���猳�̔����q�����o���e�X�g�B
	 */
	void baseExtract()
	{
		static const char* lpszSrc = "14352";

		const char* pExpect = std::find(lpszSrc, lpszSrc + ::strlen(lpszSrc), 3);
		const char* pActual = std::find(char_input_iterator(lpszSrc), char_input_iterator(lpszSrc + ::strlen(lpszSrc)), 3).base<const char*>();
		CPPUNIT_ASSERT_EQUAL(pActual, pExpect);

		CPPUNIT_ASSERT_THROW(char_input_iterator(lpszSrc).base<char*>(), std::bad_cast);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(AnyInputIteratorTest);

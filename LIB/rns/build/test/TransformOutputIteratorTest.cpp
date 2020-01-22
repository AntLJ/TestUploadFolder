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
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include <rns/iterator/transform_output_iterator.h>

class TransformIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(TransformIteratorTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	/**
	 * @brief 3足した値を書き込むテスト。
	 */
	void test1()
	{
		using namespace boost::assign;

		std::vector<int> aSource, aActual, aExpect;
		aSource += 1,2,3,4,5;
		aExpect += 4,5,6,7,8;

		std::copy(
			aSource.begin(),
			aSource.end(),
			rns::make_transform_output_iterator(
				std::back_inserter(aActual),
				std::bind1st(std::plus<int>(), 3)
			)
		);

		CPPUNIT_ASSERT(aExpect == aActual);
	}
//@}
};


CPPUNIT_TEST_SUITE_REGISTRATION(TransformIteratorTest);

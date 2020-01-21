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

/**
 * @file 
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <sstream>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <arctl/iterator/add_iterator.h>

/**
 * @brief arctl::adder のテストクラス
 */
class AddIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(AddIteratorTest);
	CPPUNIT_TEST(testQueryFilterHigh);
	CPPUNIT_TEST(testQueryFilter);
	CPPUNIT_TEST_SUITE_END();

public:
	void testQueryFilterHigh()
	{
		const LPCTSTR rgsz[] = {
			_T("HUGA"), _T("PIYO")
		};

		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipQueryFilter->put_SubFields(CComBSTR(_T("HOGE"))));
		std::copy(boost::begin(rgsz), boost::end(rgsz), arctl::adder(ipQueryFilter, &IQueryFilter::_AddField));

		CPPUNIT_ASSERT_EQUAL(
			std::string("HOGE, HUGA, PIYO"),
			std::string(ipQueryFilter->SubFields)
		);
	}

	void testQueryFilter()
	{
		const LPCTSTR rgsz[] = {
			_T("BAR"), _T("BAZ")
		};

		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipQueryFilter->put_SubFields(CComBSTR(_T("FOO"))));
		std::copy(boost::begin(rgsz), boost::end(rgsz), arctl::adder(ipQueryFilter));

		CPPUNIT_ASSERT_EQUAL(
			std::string("FOO, BAR, BAZ"),
			std::string(ipQueryFilter->SubFields)
		);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(AddIteratorTest);

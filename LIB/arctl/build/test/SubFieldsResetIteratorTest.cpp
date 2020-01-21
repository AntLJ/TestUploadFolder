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
#include <string>
#include <algorithm>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <arctl/iterator/sub_fields_reset_iterator.h>

/**
 * @brief arctl::sub_fields_reset_iterator のテストクラス
 */
class SubFieldsResetIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(SubFieldsResetIteratorTest);
	CPPUNIT_TEST(testCast1);
	CPPUNIT_TEST(testCast2);
	CPPUNIT_TEST(testConvert1);
	CPPUNIT_TEST_SUITE_END();

public:
	void testCast1()
	{
		const CComBSTR rgbstr[] = {
			CComBSTR(_T("FOO")), CComBSTR(_T("BAR")), CComBSTR(_T("BAZ"))
		};

		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		std::copy(boost::begin(rgbstr), boost::end(rgbstr), arctl::sub_fields_resetter(ipQueryFilter));

		CPPUNIT_ASSERT_EQUAL(
			std::string("FOO, BAR, BAZ"),
			std::string(ipQueryFilter->SubFields)
		);
	}

	void testCast2()
	{
		const _bstr_t rgbstr[] = {
			_bstr_t(_T("FOO")), _bstr_t(_T("BAR")), _bstr_t(_T("BAZ"))
		};

		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		std::copy(boost::begin(rgbstr), boost::end(rgbstr), arctl::sub_fields_resetter(ipQueryFilter));

		CPPUNIT_ASSERT_EQUAL(
			std::string("FOO, BAR, BAZ"),
			std::string(ipQueryFilter->SubFields)
		);
	}

	void testConvert1()
	{
		const LPCTSTR rgsz[] = {
			_T("FOO"), _T("BAR"), _T("BAZ")
		};

		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		std::copy(boost::begin(rgsz), boost::end(rgsz), arctl::sub_fields_resetter(ipQueryFilter));

		CPPUNIT_ASSERT_EQUAL(
			std::string("FOO, BAR, BAZ"),
			std::string(ipQueryFilter->SubFields)
		);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SubFieldsResetIteratorTest);

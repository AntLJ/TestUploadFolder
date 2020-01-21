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
 * @file TableNameAlgorithmTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/algorithm/table_name.h>

/**
 * @brief arctl::skip_owner_prefix() のテストクラス
 */
class SkipOwnerPrefixTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(SkipOwnerPrefixTest);
	CPPUNIT_TEST(testConstStringHasPrefix);
	CPPUNIT_TEST(testConstStringHasntPrefix);
	CPPUNIT_TEST(testConstCharArrayHasPrefix);
	CPPUNIT_TEST(testConstCharArrayHasntPrefix);
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstStringHasPrefix()
	{
		const std::basic_string<TCHAR> strTableName(_T("foo.bar"));
		CPPUNIT_ASSERT(boost::next(strTableName.begin(), 4) == arctl::skip_owner_prefix(strTableName));
	}

	void testConstStringHasntPrefix()
	{
		const std::basic_string<TCHAR> strTableName(_T("bar"));
		CPPUNIT_ASSERT(boost::next(strTableName.begin(), 0) == arctl::skip_owner_prefix(strTableName));
	}

	void testConstCharArrayHasPrefix()
	{
		const TCHAR szTableName[] = _T("foo.bar");
		CPPUNIT_ASSERT(szTableName + 4 == arctl::skip_owner_prefix(szTableName));
	}

	void testConstCharArrayHasntPrefix()
	{
		const TCHAR szTableName[] = _T("bar");
		CPPUNIT_ASSERT(szTableName + 0 == arctl::skip_owner_prefix(szTableName));
	}
};

/**
 * @brief arctl::copy_owner_prefix() のテストクラス
 */
class CopyOwnerPrefixTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(CopyOwnerPrefixTest);
	CPPUNIT_TEST(testConstCharArrayHasPrefix);
	CPPUNIT_TEST(testConstCharArrayHasntPrefix);
	CPPUNIT_TEST(testBStringHasPrefix);
	CPPUNIT_TEST(testBStringHasntPrefix);
	CPPUNIT_TEST_SUITE_END();

public:
	void testConstCharArrayHasPrefix()
	{
		CPPUNIT_ASSERT(std::basic_string<TCHAR>(_T("foo.")) == arctl::copy_owner_prefix(_T("foo.bar")));
	}

	void testConstCharArrayHasntPrefix()
	{
		const std::basic_string<TCHAR> strPrefix(arctl::copy_owner_prefix(_T("bar")));
		CPPUNIT_ASSERT(strPrefix.empty());
		CPPUNIT_ASSERT(std::basic_string<TCHAR>(_T("")) == strPrefix);
	}

	void testBStringHasPrefix()
	{
		CPPUNIT_ASSERT_EQUAL(_bstr_t(_T("foo.")), arctl::copy_owner_prefix(_bstr_t("foo.bar")));
	}

	void testBStringHasntPrefix()
	{
		CPPUNIT_ASSERT_EQUAL(_bstr_t(_T("")), arctl::copy_owner_prefix(_bstr_t("bar")));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SkipOwnerPrefixTest);
CPPUNIT_TEST_SUITE_REGISTRATION(CopyOwnerPrefixTest);

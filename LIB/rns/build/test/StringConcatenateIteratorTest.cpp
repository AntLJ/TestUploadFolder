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
#include <atlbase.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <rns/iterator/string_concatenate_iterator.h>

namespace {

inline std::ostream& operator<<(std::ostream& os, const std::wstring& rhs)
{
	os << ATL::CW2A(rhs.c_str()).m_psz;
	return os;
}

} // anonymous namespace

class StringConcatenateIteratorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(StringConcatenateIteratorTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(test2);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name テストケース
//@{
	void test1()
	{
		const char* rgsz[] = { "foo", "bar", "baz" };

		CPPUNIT_ASSERT_EQUAL(
			std::string("foo,bar,baz"),
			std::copy(boost::begin(rgsz), boost::end(rgsz), rns::string_concatenate_iterator(",")).str()
		);
	}

	void test2()
	{
		const wchar_t* rgsz[] = { L"foo", L"bar", L"baz" };

		CPPUNIT_ASSERT_EQUAL(
			std::wstring(L"foo,bar,baz"),
			std::copy(boost::begin(rgsz), boost::end(rgsz), rns::wstring_concatenate_iterator(L",")).str()
		);
	}
 //@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(StringConcatenateIteratorTest);

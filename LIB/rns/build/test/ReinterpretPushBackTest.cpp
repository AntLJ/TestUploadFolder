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
#include <list>
#include <rns/reinterpret_push_back.h>

class ReinterpretPushBackTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ReinterpretPushBackTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name �e�X�g�P�[�X
//@{
	void test()
	{
		std::vector<char> buffer;

		// 4Byte�ǉ�
		const long n1 = 42;
		rns::reinterpret_push_back(buffer, n1);

		// 2Byte�ǉ�
		const unsigned short n2 = 21;
		rns::reinterpret_push_back(buffer, n2);

		// 6Byte�ǉ�����Ă���H
		CPPUNIT_ASSERT_EQUAL(
			sizeof(int) + sizeof(unsigned short),
			buffer.size()
		);

		// �擪4Byte�m�F
		CPPUNIT_ASSERT_EQUAL(
			42,
			*reinterpret_cast<int*>(&buffer[0])
		);

		// ���̌���2Byte�m�F
		CPPUNIT_ASSERT_EQUAL(
			static_cast<unsigned short>(21U),
			*reinterpret_cast<unsigned short*>(&buffer[sizeof(int)])
		);
	}
 //@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(ReinterpretPushBackTest);

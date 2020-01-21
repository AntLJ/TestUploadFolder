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
#include <sstream>
#include <atl2/io.h>

/**
 * @ingroup unit_test
 * @brief atl2::io::compatible �� @ref unit_test �N���X�B
 */
class IoCompatibleTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(IoCompatibleTest);
	CPPUNIT_TEST(testW2A);
	CPPUNIT_TEST(testA2W);
	CPPUNIT_TEST_SUITE_END();

public:
/// @name �e�X�g�P�[�X
//@{
	void testW2A()
	{
		static const wchar_t szSample[] = L"�T���v��������";
		std::ostringstream ossActual, ossExpected;

		// ���O��Ԃ�L���ɂ��Ă��Ȃ��ꍇ�̓A�h���X���o�͂����B		
		ossActual << szSample;
		ossExpected << static_cast<const void*>(szSample);

		CPPUNIT_ASSERT_EQUAL(ossExpected.str(), ossActual.str());

		ossActual.str("");
		ossExpected.str("");

		// ���O��Ԃ�L���ɂ��邱�ƂŃ��C�h�����񂩂�}���`�o�C�g������ւ̕ϊ��������B
		using namespace atl2::io::compatible;
		USES_CONVERSION;

		ossActual << szSample;
		ossExpected << W2CA(szSample);

		CPPUNIT_ASSERT_EQUAL(ossExpected.str(), ossActual.str());
	}

	void testA2W()
	{
		static const char szSample[] = "�T���v��������";
		std::wostringstream ossActual, ossExpected;

		// ���O��Ԃ�L���ɂ��邱�ƂŃ}���`�o�C�g�����񂩂烏�C�h������ւ̕ϊ��������B
		using namespace atl2::io::compatible;
		USES_CONVERSION;

		ossActual << szSample;
		ossExpected << A2CW(szSample);

		CPPUNIT_ASSERT(ossExpected.str() == ossActual.str());
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(IoCompatibleTest);

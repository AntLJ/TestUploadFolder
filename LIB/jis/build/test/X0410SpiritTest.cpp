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
#include <string>
#include <jis/x0410/spirit.h>

using namespace boost::spirit;
using namespace jis::x0410::spirit;

class MeshCodePrimitiveTest2 : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(MeshCodePrimitiveTest2);
	CPPUNIT_TEST(testSuccess1);
	CPPUNIT_TEST(testFailShort);
	CPPUNIT_TEST(testFailLong);
	CPPUNIT_TEST(testFailIllegal);
	CPPUNIT_TEST_SUITE_END();

public:
//@{ @name �e�X�g�P�[�X
	/**
	 * @brief ���������b�V���R�[�h�̉�́B
	 */
	void testSuccess1()
	{
		CPPUNIT_ASSERT(parse("533945", meshcode2_p).full);
	}

	/**
	 * @brief �w�肳�ꂽ���b�V���R�[�h�Ƃ��Ă͒Z���ꍇ�B
	 */
	void testFailShort()
	{
		CPPUNIT_ASSERT(! parse("53394", meshcode2_p).hit);
	}

	/**
	 * @brief �w�肳�ꂽ���b�V���R�[�h�Ƃ��Ă͒����ꍇ�B
	 *
	 * �}�b�`���O�ɂ͐������邪�A�Ō�܂œǂݐ؂�Ȃ��B
	 */
	void testFailLong()
	{
		CPPUNIT_ASSERT_EQUAL(6U, parse("5339454", meshcode2_p).length);
	}

	/**
	 * @brief �s���Ȓl���܂܂�Ă���ꍇ�B
	 */
	void testFailIllegal()
	{
		CPPUNIT_ASSERT(! parse("533948", meshcode2_p).hit);
	}
//@}
};

CPPUNIT_TEST_SUITE_REGISTRATION(MeshCodePrimitiveTest2);

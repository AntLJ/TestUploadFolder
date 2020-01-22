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

#pragma once

#include "../CSHelper.h"

class CCSHelperTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( CCSHelperTest );
	CPPUNIT_TEST( test_findPoint1 );
	CPPUNIT_TEST( test_findPoint2 );
	CPPUNIT_TEST( test_findPoint3 );
	CPPUNIT_TEST( test_findPoint4 );
	CPPUNIT_TEST( test_findPoint5 );
	CPPUNIT_TEST( test_findPoint6 );
	CPPUNIT_TEST_SUITE_END();

protected:

	CCSHelper*	m_pHelper;

public:
	void setUp();
	void tearDown();

protected:

	/**
	 * @brief	[�y�v�m�F�zCS�|�C���g����������]�e�X�g
	 */
	void test_findPoint1();

	/**
	 * @brief	[�y�v�m�F�zCS�|�C���g�Ɠ���16���̍s���E�����݂��܂�]�e�X�g ... Bug6180
	 */
	void test_findPoint2();

	/**
	 * @brief	[�V�K�ǉ�(CS)]�e�X�g
	 */
	void test_findPoint3();

	/**
	 * @brief	[���Ȃ�(CS)]�e�X�g
	 */
	void test_findPoint4();

	/**
	 * @brief	[�y�v�m�F�zCS�|�C���g�ړ�]�e�X�g
	 */
	void test_findPoint5();

	/**
	 * @brief	[������Ȃ�]�e�X�g
	 */
	void test_findPoint6();
};

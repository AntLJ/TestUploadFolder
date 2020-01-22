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

#include "../GPHelper.h"

class CGPHelperTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( CGPHelperTest );
	CPPUNIT_TEST( test_findPoint1 );
	CPPUNIT_TEST( test_findPoint2 );
	CPPUNIT_TEST( test_findPoint3 );
	CPPUNIT_TEST( test_findPoint4 );
	CPPUNIT_TEST( test_findPoint5 );
	CPPUNIT_TEST( test_findPoint6 );
	CPPUNIT_TEST( test_findPoint7 );
	CPPUNIT_TEST( test_findPoint8 );
	CPPUNIT_TEST( test_findPoint9 );
	CPPUNIT_TEST_SUITE_END();
protected:

	CGPHelper*	m_pHelper;

public:
	void setUp();
	void tearDown();

protected:

	/**
	 * @brief	[�y�v�m�F�z����R�[�h�̕����E������]�e�X�g
	 */
	void test_findPoint1();

	/**
	 * @brief	[�y�v�m�F�z�D��t���O�������Ă��Ȃ�]�e�X�g
	 */
	void test_findPoint2();

	/**
	 * @brief	[�V�K�ǉ��^�y�v�m�F�z�D��t���O�̗����Ă���|�C���g��TPG]�e�X�g
	 */
	void test_findPoint3();

	/**
	 * @brief	[�V�K�ǉ�]�e�X�g
	 */
	void test_findPoint4();

	/**
	 * @brief	[���Ȃ�]�e�X�g
	 */
	void test_findPoint5();

	/**
	 * @brief	[�y�v�m�F�z�D��t���O�̗����Ă���|�C���g��TPG]�e�X�g
	 */
	void test_findPoint6();

	/**
	 * @brief	[�y�v�m�F�z���|�C���g�ړ�]�e�X�g
	 */
	void test_findPoint7();

	/**
	 * @brief	[Bug6181]�e�X�g
	 */
	void test_findPoint8();

	/**
	 * @brief	[�y�v�m�F�z����20���R�[�h�ŗD��t���O�����Ă�����̂���������]�e�X�g
	 */
	void test_findPoint9();
};

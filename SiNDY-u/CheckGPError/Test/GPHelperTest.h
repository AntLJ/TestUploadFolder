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
	 * @brief	[【要確認】同一コードの符号界が存在]テスト
	 */
	void test_findPoint1();

	/**
	 * @brief	[【要確認】優先フラグが立っていない]テスト
	 */
	void test_findPoint2();

	/**
	 * @brief	[新規追加／【要確認】優先フラグの立っているポイントがTPG]テスト
	 */
	void test_findPoint3();

	/**
	 * @brief	[新規追加]テスト
	 */
	void test_findPoint4();

	/**
	 * @brief	[問題なし]テスト
	 */
	void test_findPoint5();

	/**
	 * @brief	[【要確認】優先フラグの立っているポイントがTPG]テスト
	 */
	void test_findPoint6();

	/**
	 * @brief	[【要確認】号ポイント移動]テスト
	 */
	void test_findPoint7();

	/**
	 * @brief	[Bug6181]テスト
	 */
	void test_findPoint8();

	/**
	 * @brief	[【要確認】同一20桁コードで優先フラグ立っているものが複数あり]テスト
	 */
	void test_findPoint9();
};

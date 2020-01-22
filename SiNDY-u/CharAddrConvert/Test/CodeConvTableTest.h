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

#include "../CodeConvTable.h"

class CodeConvTableTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( CodeConvTableTest );
	CPPUNIT_TEST( test_getpntcode );
	CPPUNIT_TEST( test_getgoucode );
	CPPUNIT_TEST_SUITE_END();

protected:
	CCodeConvTable*	m_pConvTable;

public:
	void setUp();
	void tearDown();

protected:

	/**
	 * @brief	地番コード取得テスト
	 */
	void test_getpntcode();

	/**
	 * @brief	号コード取得テスト
	 */
	void test_getgoucode();
};

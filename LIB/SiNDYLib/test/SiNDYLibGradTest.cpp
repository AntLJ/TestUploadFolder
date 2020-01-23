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
 * @file SiNDYLibGradTest.cpp
 * @brief SiNDYLibの CGrad をテストするクラスの定義ファイルです。
 * @author 
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "Grad.h"
#include "Workspace.h"
#include "Table.h"
#include <list>
#include "TableTypeUtil.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

using namespace sindy;
using namespace sindy::errorcode;
/**
 * @class CSiNDYGradPropertyTest
 * @brief SiNDYLibの CGrad のプロパティをテストするクラス
 * <h3>チェック内容：<h3>
 * @li 閾値の入力チェック
 * @li 例外値（NULL、範囲外）
 */
class CSiNDYGradPropertyTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CSiNDYGradPropertyTest);
  CPPUNIT_TEST(testLink_ID);
  CPPUNIT_TEST(testGrad_Value);
  CPPUNIT_TEST_SUITE_END();
public:
	CSiNDYGradPropertyTest(){}
	virtual ~CSiNDYGradPropertyTest(){}

	void setUp ()
	{
		cWk.Connect(_T("Test.mdb"));
		sindy::CTable cTable(sindy::sindyTableType::grad, cWk.OpenTable(AheTableType2TableName(sindy::sindyTableType::grad)));
		cClass = (IFeaturePtr)cTable.GetRow(1);
		cWk.StartEditing(true);
		cWk.StartEditOperation();
	}

	void tearDown()
	{
		cWk.StopEditOperation();
		cWk.StopEditing(false);
	}


	/**
	 * @brief Set/Get Test
	 */
	void testLink_ID()
	{
		/// Getter Test
		CPPUNIT_ASSERT( cClass.GetLinkID() >= 0 );
		CPPUNIT_ASSERT( cClass.GetLinkID() <= 2147483647 );

		/// Setter Test
		CPPUNIT_ASSERT_EQUAL( sindyErr_ArgLimitOver, cClass.SetLinkID(-1));
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.SetLinkID(0));
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.SetLinkID(2147483647));
//		CPPUNIT_ASSERT_EQUAL( sindyErr_ArgLimitOver, cClass.SetLink_ID(2147483647+1));
	}

	void testGrad_Value()
	{
		/// Getter Test
		CPPUNIT_ASSERT( cClass.GetGradValue() >= 0 );
		CPPUNIT_ASSERT( cClass.GetGradValue() <= 99999 );

		/// Setter Test
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.SetGradValue(-1));
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.SetGradValue(0));
		CPPUNIT_ASSERT_EQUAL( sindyErr_NoErr, cClass.SetGradValue(99999));
		CPPUNIT_ASSERT_EQUAL( sindyErr_ArgLimitOver, cClass.SetGradValue(100000));
	}
private:
	sindy::CWorkspace cWk;
	sindy::grad::CGrad cClass;
};

#ifdef _NETWORK_TEST
CPPUNIT_TEST_SUITE_REGISTRATION(CSiNDYGradPropertyTest);
#endif // ifdef _NETWORK_TEST

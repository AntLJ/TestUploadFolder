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
 * @file SiNDYLibGeometryCheckTest.cpp
 * @brief SiNDYLibの CGeometryCheck.cpp のをテストするクラスの定義ファイルです。
 * @author 
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "Geometry.h"
//#include "Workspace.h"
//#include "Table.h"
#include "ErrorCode.h"
#include <list>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
/**
 * @class CSiNDYLibGeometryCheckTest
 * @brief SiNDYLibの CGeometryのジオメトリチェックをテストするクラス
 * <h3>チェック内容：<h3>
 * @li 閾値の入力チェック
 * @li 例外値（NULL、範囲外）
 */
using namespace sindy;
using namespace sindy;
using namespace sindy::errorcode;

class CSiNDYLibGeometryCheckTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CSiNDYLibGeometryCheckTest);
  CPPUNIT_TEST(testGeometryCheck);
  CPPUNIT_TEST_SUITE_END();
public:
	CSiNDYLibGeometryCheckTest(){}
	virtual ~CSiNDYLibGeometryCheckTest(){}

	void setUp ()
	{
	}


	void tearDown()
	{
	}


	/**
	 * @brief Set/Get Test
	 */
	void testGeometryCheck()
	{
		// 空形状チェック
		// 編集可能エリアチェック
		// ロックエリアチェック
		// パート数チェック（ポイント、ポリライン）
		// サポート外形状チェック
		// エクステリア数チェック（ポリゴン）
		// ねじれチェック（ポリゴン）
		// ループチェック

		// 稲妻形状チェック
		// パルス・鋭角ターンチェック
		// セグメント長チェック
		// 自己接触チェック
		// 最低セグメント数チェック
	}

	void testSegmentCheck()
	{
	}

	void testPointCheck()
	{
	}
	
private:
};

CPPUNIT_TEST_SUITE_REGISTRATION(CSiNDYLibGeometryCheckTest);

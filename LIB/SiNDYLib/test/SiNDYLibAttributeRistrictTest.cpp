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
 * @file SiNDYLibAttributeRistrict.cpp
 * @brief AttributeRistRict.[h|cpp]をテストする
 * クラスの定義ファイルです。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "AttributeRistrict.h"
#include <list>
#include "Workspace.h"
#include "Row.h"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

using namespace sindy;

/**
 * @class CAttributeRistrictTest
 * @brief SiNDYLibのグローバル関数であるsindy::AheCheckOrFixAnnotationStringをテストする
 * クラス
 * <h3>チェック内容：<h3>
 * @li 注記文字列と文字列長の整合性テスト
 * @li 全角文字列使用可能文字のテスト
 * @li ジャンクション、インターチェンジ、フェリーターミナルの場合の最終文字列テスト
 * @li 主要地方道番号、国道番号の場合のテスト
 * @li 山岳標高の場合のテスト
 */
class CAttributeRistrictTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CAttributeRistrictTest);
  CPPUNIT_TEST(test_rule);
  CPPUNIT_TEST(test_rule_minmax_error);
  CPPUNIT_TEST(test_rule_minmax_non_error);
  CPPUNIT_TEST(test_rule_minmax_warning);
  CPPUNIT_TEST(test_rule_values_error);
  CPPUNIT_TEST(test_rule_values_warning);
  CPPUNIT_TEST(test_rule_and);
  CPPUNIT_TEST(test_rule_and_mix);
  CPPUNIT_TEST_SUITE_END();
public:
	CAttributeRistrictTest(){}
	virtual ~CAttributeRistrictTest(){}

	void setUp()
	{
		m_cWork.ConnectToPGDB( _T("Test.mdb") );

		ITablePtr ipTable( m_cWork.OpenTable( _T("ROAD_LINK") ) );
		ipTable->GetRow( 1, m_cRow.Release() );

		m_cRow.Trace();

		m_cRow.CreateCache( true );
	}

	void tearDown()
	{
		m_cWork = NULL;
	}
	/**
	 * @brief シンプルなルールのテスト
	 * @li 単一制限でエラーになるかどうか
	 * @li 単一制限でエラーにならないかどうか
	 * @li 単一制限で警告になるかどうか
	 */
	void test_rule()
	{
		m_cRow.SetBoolValueByFieldName(_T("FIELDSURVEY_F"), true );

		// [test] 単一制限でエラーになるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "simple error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 単一制限でエラーにならないかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=0") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "simple non-error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 単一制限で警告が出るかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "simple warning test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
 	 * @brief 最大値・最小値での制限ルールのエラーテスト
	 * @li 最大最小同値は認められないかどうかのテスト
	 * @li 単純なルールでエラーになるかどうか
	 * @li 最小の境界値でエラーになるかどうかのテスト
	 * @li 最大の境界値でエラーになるかどうかのテスト
	 * @li 最小が負値でエラーになるかどうかのテスト
	 * @li 最大が負値でエラーになるかどうかのテスト
	 */
	void test_rule_minmax_error()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] 最大最小同値が認められないかどうかのテスト
		m_cRule.clear();
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax same value error test failed.", false, m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-3") ) );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax same value error test failed.", false, m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-3:W") ) );

		// [test] エラーになるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最小の境界値でエラーになるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min threthold error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最大の境界値でエラーになるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1-3") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max threthold error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最小が負値でエラーになるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-5-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min is minus error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最大が負値でエラーになるかどうかのテスト
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), -5 );
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-30--2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max is minus error test failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
 	 * @brief 最大値・最小値での制限ルールの非エラーテスト
	 * @li 単純なルールでエラーにならないかどうか
	 * @li 最小の境界値でエラーにならないかどうかのテスト
	 * @li 最大の境界値でエラーにならないかどうかのテスト
	 * @li 最小が負値でエラーになるらないどうかのテスト
	 * @li 最大が負値でエラーになるらないどうかのテスト
	 */
	void test_rule_minmax_non_error()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] エラーにならないかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=5-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最小の境界値でエラーにならないかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=4-10") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min threthold error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最大の境界値でエラーにならないかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1-2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max threthold error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最小が負値でエラーにならないかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-5-2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min is minus error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最大が負値でエラーにならないかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-30--2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max is minus error test failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
 	 * @brief 最大値・最小値での制限ルールの警告テスト
	 * @li 単純なルールで警告になるかどうか
	 * @li 最小の境界値で警告になるかどうかのテスト
	 * @li 最大の境界値で警告になるかどうかのテスト
	 * @li 最小が負値で警告になるかどうかのテスト
	 * @li 最大が負値で警告になるかどうかのテスト
	 */
	void test_rule_minmax_warning()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] 警告になるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2-10:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最小の境界値で警告になるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3-10:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min threthold error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最大の境界値で警告になるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1-3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max threthold error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 最小が負値で警告になるかどうかのテスト
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-5-10:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax min is minus error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

/*		// [test] 最大が負値で警告になるかどうかのテスト
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F:W"), -5 );
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-30--2:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "minmax max is minus error test failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );*/
	}
	/**
	 * @brief 複数値での制限ルールのエラーテスト
	 */
	void test_rule_values_error()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] 単純なルールでエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2,3,4") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 単純なルールでエラーにならないかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=4,5,6") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] error test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 左値境界でエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3,4,5") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 右値境界でエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1,2,3") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[4] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 負値が入っている場合にエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,3,4") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[5] error test of rule of values failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 負値が入っている場合にエラーにならないかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,4,5") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[6] error test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
	 * @brief 複数値での制限ルール警告テスト
	 */
	void test_rule_values_warning()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );

		// [test] 単純なルールで警告になるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2,3,4:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 単純なルールで警告にならないかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=4,5,6:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] warning test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 左値境界でエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3,4,5:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 右値境界でエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1,2,3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[4] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 負値が入っている場合にエラーになるかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,3,4:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[5] warning test of rule of values failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 負値が入っている場合にエラーにならないかどうか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=-2,4,5:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[6] warning test of rule of values failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
	 * @brief AND条件でのルールテスト
	 */
#include <comutil.h>
	void test_rule_and()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 3 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 1 );

		// [test] 2つのAND条件のうち二つとも合致の場合にエラーとなるか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=1") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] error test of rule of and failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2つのAND条件のうち一つのみ合致の場合にエラーとならないか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] error test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2つのAND条件のうち二つとも合致しない場合にエラーとならないか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=5|ROADCLASS_C=2") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] error test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 2つのAND条件のうち二つとも合致の場合に警告となるか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=1:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] warning test of rule of and failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2つのAND条件のうち一つのみ合致の場合に警告とならないか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=3|ROADCLASS_C=2:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] warning test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 2つのAND条件のうち二つとも合致しない場合に警告とならないか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=5|ROADCLASS_C=2:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		// [test] 少し複雑な条件でエラーが検出できるか（bug 4521）
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 1 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 1 );
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C!=0") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] error test of rule of and failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 0 );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] non-error test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

		m_cRow.SetStringValueByFieldName(_T("SOURCE"), _T("aaa") );
		m_cRow.SetLongValueByFieldName(_T("USERCLAIM_F"), 1 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 150 );
		m_cRule.clear();
		m_cRule.AddRistrict(_T("SOURCE!=NULL|USERCLAIM_F=1|ROADCLASS_C=100-200:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] warning test of rule of and failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		VARIANT vaNull;
		VariantInit(&vaNull);
		vaNull.vt = VT_NULL;
		m_cRow.SetValueByFieldName(_T("SOURCE"), vaNull );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[bug 4521] non-warning test of rule of and failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
	}
	/**
	 * @brief AND条件と他の条件のMIXテスト
	 */
	void test_rule_and_mix()
	{
		m_cRow.SetLongValueByFieldName(_T("FIELDSURVEY_F"), 1 );
		m_cRow.SetLongValueByFieldName(_T("ROADCLASS_C"), 15 );
		m_cRow.SetLongValueByFieldName(_T("DISPCLASS_C"), 100 );
		m_cRow.SetLongValueByFieldName(_T("TO_NODE_ID"), 3 );
		// [test] 片方の条件のみエラーとなる場合にエラーとなるか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=4") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[1] error test of rule of and-mix failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 片方の条件がエラー、もう片方が警告となる場合にエラーとなるか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[2] error test of rule of and-mix failed.", sindyeEditableNoEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] 両方の条件が警告となる場合に警告となるか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=1|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500:W") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=3:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of and-mix failed.", sindyeEditableWarnning, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );
		// [test] どの条件にも引っかからない場合に編集可となるか
		m_cRule.clear();
		m_cRule.AddRistrict( _T("FIELDSURVEY_F=2|ROADCLASS_C=15,16,17|DISPCLASS_C!=200-500") );
		m_cRule.AddRistrict( _T("TO_NODE_ID=4:W") );
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "[3] warning test of rule of and-mix failed.", sindyeEditableEditable, m_cRule.IsEditable( m_cRow, 1023, m_strMsg ) );

	}
private:
	CWorkspace m_cWork;
	CRow m_cRow;
	CAttributeRistrictsOr m_cRule;
	TCHAR m_strMsg[1024];
};

CPPUNIT_TEST_SUITE_REGISTRATION(CAttributeRistrictTest);

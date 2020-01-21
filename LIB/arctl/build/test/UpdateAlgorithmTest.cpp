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
 * @file UpdateAlgorithmTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <atl2/algorithm.h>
#include <arctl/pgdb.h>
#include <arctl/cursor.h>
#include <arctl/iterator.h>
#include <arctl/algorithm.h>

/**
 * @ingroup unit_test
 * @brief arctl::update() の @ref unit_test クラス
 */
class UpdateAlgorithmTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(UpdateAlgorithmTest);
	CPPUNIT_TEST(testUpdate);
	CPPUNIT_TEST(testSearch);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		// テスト用のPGDBが残っていたら削除する。
		::DeleteFile(kTestWorkspace);

		// テスト用のPGDBを作成する。
		IFeatureWorkspacePtr ipFeatureWorkspace = arctl::create_pgdb(kTestWorkspace);
		CPPUNIT_ASSERT(ipFeatureWorkspace != 0);

		// テスト用のテーブルを作成する。
		IFieldsEditPtr ipFieldsEdit(CLSID_Fields);
		IFieldEditPtr ipFieldEdit;

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(_T("OBJECTID"))));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeOID));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kValue1)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeInteger));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(
			S_OK, ipFeatureWorkspace->CreateTable(CComBSTR(_T("test_table")), ipFieldsEdit, 0, 0, 0, &m_ipTable)
		);
	}

	virtual void tearDown()
	{
		m_ipTable = 0;

		// テスト用のPGDBを削除する。
		CPPUNIT_ASSERT_EQUAL(TRUE, ::DeleteFile(kTestWorkspace));
	}

//@{ @name テストケース
	void testUpdate()
	{
		using namespace arctl;

		// 10行生成する。
		for(int i = 0; i < 10; i++) {
			_IRowPtr ipRow;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRow(&ipRow));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->put_Value(1, _variant_t(0L)));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->Store());
		}
		CPPUNIT_ASSERT_EQUAL(10L, m_ipTable->_RowCount(0));

		// VALUE1の値をOBJECTID+100に更新する。
		update(m_ipTable->_Update(0, VARIANT_TRUE), updateRow);

		// 正しく更新されているか確認する。
		atl2::for_each(m_ipTable->_Search(0, VARIANT_TRUE), checkRow);

		// 全ての行を削除する。
		delete_all(m_ipTable->_Update(0, VARIANT_TRUE));
		CPPUNIT_ASSERT_EQUAL(0L, m_ipTable->_RowCount(0));
	}

	void testSearch()
	{
		using namespace arctl;

		// 10行生成する。
		for(int i = 0; i < 10; i++) {
			_IRowPtr ipRow;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRow(&ipRow));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->put_Value(1, _variant_t(0L)));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->Store());
		}
		CPPUNIT_ASSERT_EQUAL(10L, m_ipTable->_RowCount(0));

		// VALUE1の値をOBJECTID+100に更新する。
		update(m_ipTable->_Search(0, VARIANT_FALSE), updateRow);

		// 正しく更新されているか確認する。
		atl2::for_each(m_ipTable->_Search(0, VARIANT_TRUE), checkRow);

		// 全ての行を削除する。
		delete_all(m_ipTable->_Search(0, VARIANT_FALSE));
		CPPUNIT_ASSERT_EQUAL(0L, m_ipTable->_RowCount(0));
	}
//@}

private:
// 処理
	static bool updateRow(_IRow* ipRow)
	{
		ipRow->put_Value(1, _variant_t(ipRow->OID + 100L));
		return true;
	}

	static void checkRow(_IRow* ipRow)
	{
		CPPUNIT_ASSERT_EQUAL(ipRow->OID + 100L, static_cast<long>(ipRow->Value[1]));
	}

// 変数
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(UpdateAlgorithmTest);

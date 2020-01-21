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
 * @file DeleteIteratorTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <set>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/pgdb.h>
#include <arctl/iterator/delete_iterator.h>
#include <arctl/algorithm.h>

/**
 * @ingroup unit_test
 * @brief arctl::delete_iterator の @ref unit_test クラス。
 */
class DeleteIteratorTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(DeleteIteratorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testHelper);
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
	void test()
	{
		using namespace arctl;

		// 全ての行を削除する。
		delete_all(m_ipTable->_Update(0, VARIANT_FALSE));
		CPPUNIT_ASSERT_EQUAL(0L, m_ipTable->_RowCount(0));

		// 10行生成する。
		std::set<long> aIdSet;
		for(int i = 0; i < 10; i++) {
			_IRowPtr ipRow;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRow(&ipRow));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->Store());

			// うち5行はあとで削除する。
			if(i < 5) {
				aIdSet.insert(ipRow->OID);
			}
		}
		CPPUNIT_ASSERT_EQUAL(10L, m_ipTable->_RowCount(0));

		// 5行削除する。
		{
			arctl::delete_iterator<ITable> itDelete(m_ipTable, _T("OBJECTID"));
			for(std::set<long>::iterator it = aIdSet.begin(); it != aIdSet.end(); ++it) {
				*itDelete = *it;
				++itDelete;
			}

			// この段階ではまだ削除されていない。
			CPPUNIT_ASSERT_EQUAL(10L, m_ipTable->_RowCount(0));
		}

		// スコープを抜けた時点で削除される。
		CPPUNIT_ASSERT_EQUAL(5L, m_ipTable->_RowCount(0));

	}

	void testHelper()
	{
		using namespace arctl;

		// 全ての行を削除する。
		delete_all(m_ipTable->_Update(0, VARIANT_FALSE));
		CPPUNIT_ASSERT_EQUAL(0L, m_ipTable->_RowCount(0));

		// 10行生成する。
		std::set<long> aIdSet;
		for(int i = 0; i < 10; i++) {
			_IRowPtr ipRow;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRow(&ipRow));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->Store());

			// うち5行はあとで削除する。
			if(i < 5) {
				aIdSet.insert(ipRow->OID);
			}
		}
		CPPUNIT_ASSERT_EQUAL(10L, m_ipTable->_RowCount(0));

		// 5行削除する。
		std::copy(aIdSet.begin(), aIdSet.end(), deleter(m_ipTable, _T("OBJECTID")));
		CPPUNIT_ASSERT_EQUAL(5L, m_ipTable->_RowCount(0));

		// 全ての行を削除する。
		delete_all(m_ipTable->_Update(0, VARIANT_FALSE));
		CPPUNIT_ASSERT_EQUAL(0L, m_ipTable->_RowCount(0));
	}
//@}

private:
// 変数
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DeleteIteratorTest);

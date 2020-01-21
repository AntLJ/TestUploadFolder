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
 * @file FieldIndexTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <sstream>
#include <arctl/pgdb.h>
#include <arctl/cursor.h>
#include <arctl/iterator.h>
#include <arctl/field/index.h>

/**
 * @ingroup unit_test
 * @brief arctl::field::index_t の @ref unit_test クラス。
 */
class FieldIndexTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FieldIndexTest);
	CPPUNIT_TEST(testConstruct1);
	CPPUNIT_TEST(testConstructM1);
	CPPUNIT_TEST(testConstructM2);
	CPPUNIT_TEST(testCopyConstructM1);
	CPPUNIT_TEST(testNot1);
	CPPUNIT_TEST(testNotM1);
	CPPUNIT_TEST(testCopyNameM1);
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

		// 属性0:OBJECTID:OID型。
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(_T("OBJECTID"))));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeOID));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		// 属性1:VALUE1:整数型。
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kValue1)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeInteger));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		// 属性2:VALUE2:整数型。
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kValue2)));
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
	/// field::index_t の構築確認。
	void testConstruct1()
	{
		using namespace arctl::field;
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(index_t<kValue1>(m_ipTable->Fields)));
		CPPUNIT_ASSERT_EQUAL(2L, static_cast<long>(index_t<kValue2>(m_ipTable->Fields)));
		CPPUNIT_ASSERT_EQUAL(-1L, static_cast<long>(index_t<kValue3>(m_ipTable->Fields)));
	}

	/// IFields* による ARCTL_FIELD_INDEX_CLASS の構築確認。
	void testConstructM1()
	{
		using namespace arctl::field;
		FieldIndex aFieldIndex(static_cast<IFields*>(m_ipTable->Fields));
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(static_cast<const index_t<kValue1>&>(aFieldIndex)));
		CPPUNIT_ASSERT_EQUAL(2L, static_cast<long>(static_cast<const index_t<kValue2>&>(aFieldIndex)));
	}

	/// IFieldsPtr による ARCTL_FIELD_INDEX_CLASS の構築確認。
	void testConstructM2()
	{
		using namespace arctl::field;
		FieldIndex aFieldIndex(m_ipTable->Fields);
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(static_cast<const index_t<kValue1>&>(aFieldIndex)));
		CPPUNIT_ASSERT_EQUAL(2L, static_cast<long>(static_cast<const index_t<kValue2>&>(aFieldIndex)));
	}

	/// ARCTL_FIELD_INDEX_CLASS のコピーコンストラクタの動作確認。
	void testCopyConstructM1()
	{
		using namespace arctl::field;
		const FieldIndex aFieldIndex_(m_ipTable->Fields);
		FieldIndex aFieldIndex(aFieldIndex_);
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(static_cast<const index_t<kValue1>&>(aFieldIndex)));
		CPPUNIT_ASSERT_EQUAL(2L, static_cast<long>(static_cast<const index_t<kValue2>&>(aFieldIndex)));
	}

	/// field::index_t の否定演算確認。
	void testNot1()
	{
		using namespace arctl::field;
		CPPUNIT_ASSERT_EQUAL(true, ! index_t<kValue1>());
		CPPUNIT_ASSERT_EQUAL(false, ! index_t<kValue1>(m_ipTable->Fields));
	}

	/// ARCTL_FIELD_INDEX_CLASS の否定演算確認。
	void testNotM1()
	{
		using namespace arctl::field;

		FieldIndex aFieldIndex;
		CPPUNIT_ASSERT_EQUAL(true, ! aFieldIndex);

		aFieldIndex = m_ipTable->Fields;
		CPPUNIT_ASSERT_EQUAL(false, ! aFieldIndex);

		CPPUNIT_ASSERT_EQUAL(true, ! FieldIndex2(m_ipTable->Fields));
	}

	/// ARCTL_FIELD_INDEX_CLASS::copy の動作確認。
	void testCopyNameM1()
	{
#ifdef _UNICODE
		std::wostringstream ossExpected;
		std::wostringstream ossActual;
#else
		std::ostringstream ossExpected;
		std::ostringstream ossActual;
#endif

		// "VALUE1 VALUE2 " となるはず。
		ossExpected << kValue1 << _T(" ") << kValue2 << _T(" ");
		FieldIndex::copy_name(std::ostream_iterator<LPCTSTR, TCHAR>(ossActual, _T(" ")));

		CPPUNIT_ASSERT(ossExpected.str() == ossActual.str());
	}
//@}

private:
// 定義
	ARCTL_FIELD_INDEX_CLASS(FieldIndex, (kValue1) (kValue2) );
	ARCTL_FIELD_INDEX_CLASS(FieldIndex2, (kValue1) (kValue2) (kValue3) );

// 変数
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldIndexTest);

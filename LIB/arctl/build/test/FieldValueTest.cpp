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
 * @file FieldValueTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/pgdb.h>
#include <arctl/cursor.h>
#include <arctl/iterator.h>
#include <arctl/field/value.h>

/// かなりやる気のない、@b _variant_t 用特殊化版小なり演算
template <>
struct std::less<_variant_t>
{
	bool operator()(const _variant_t& lhs, const _variant_t& rhs) { return lhs.lVal < rhs.lVal; }
};

/**
 * @ingroup unit_test
 * @brief arctl::field::value_t の @ref unit_test クラス。
 */
class FieldValueTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FieldValueTest);
	CPPUNIT_TEST(testZeroInitialization);
	CPPUNIT_TEST(testConstruct1);
	CPPUNIT_TEST(testConstructM1);
	CPPUNIT_TEST(testConstructM2);
	CPPUNIT_TEST(testSubstitionM1);
	CPPUNIT_TEST(testCompareM1);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST(testDistributeValue);
	CPPUNIT_TEST(testInterpretIterator);
	CPPUNIT_TEST(testPut);
	CPPUNIT_TEST(testCast);
	CPPUNIT_TEST(testOptionalGet);
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

		// 10行ほど生成する。
		for(int i = 0; i < 10; i++) {
			_IRowPtr ipRow;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRow(&ipRow));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->put_Value(1, _variant_t(ipRow->OID)));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->put_Value(2, _variant_t(ipRow->OID * 2L)));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->Store());
		}
	}

	virtual void tearDown()
	{
		m_ipTable = 0;

		// テスト用のPGDBを削除する。
		CPPUNIT_ASSERT_EQUAL(TRUE, ::DeleteFile(kTestWorkspace));
	}

//@{ @name テストケース
	/// 属性値をPOD型で管理する field::index_t のゼロ初期化テスト。
	void testZeroInitialization()
	{
		arctl::field::value_t<kValue2, long> aValue;
		CPPUNIT_ASSERT_EQUAL(0L, aValue.value());
	}

	/// field::index_t の構築確認。
	void testConstruct1()
	{
		using namespace arctl::field;

		_ICursorPtr ipCursor;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->Search(0, VARIANT_FALSE, &ipCursor));

		_IRowPtr ipRow;
		while(ipCursor->NextRow(&ipRow) == S_OK) {
			value_t<kValue1> aValue1(ipRow);
			CPPUNIT_ASSERT_EQUAL(static_cast<unsigned short>(VT_I4), aValue1.value().vt);
			CPPUNIT_ASSERT_EQUAL(ipRow->OID, static_cast<long>(aValue1.value()));

			value_t<kValue2, long> aValue2(ipRow);
			CPPUNIT_ASSERT_EQUAL(ipRow->OID * 2L, aValue2.value());
		}
	}

	/// _IRowPtr による ARCTL_FIELD_VALUE_CLASS の構築確認。
	void testConstructM1()
	{
		using namespace arctl::field;

		_ICursorPtr ipCursor;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->Search(0, VARIANT_FALSE, &ipCursor));

		_IRowPtr ipRow;
		while(ipCursor->NextRow(&ipRow) == S_OK) {
			const FieldValue aFieldValue(ipRow);
			CPPUNIT_ASSERT_EQUAL(ipRow->OID, static_cast<long>(get<kValue1>(aFieldValue)));
			CPPUNIT_ASSERT_EQUAL(ipRow->OID * 2L, get<kValue2>(aFieldValue));
		}
	}

	/// 値直接渡しによる ARCTL_FIELD_VALUE_CLASS の構築確認。
	void testConstructM2()
	{
		using namespace arctl::field;

		FieldValue aFieldValue1(1L, 2L);
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(get<kValue1>(aFieldValue1)));
		CPPUNIT_ASSERT_EQUAL(2L, get<kValue2>(aFieldValue1));

		FieldValue2 aFieldValue2(1L, 2L, 3.4);
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(get<kValue1>(aFieldValue2)));
		CPPUNIT_ASSERT_EQUAL(2L, get<kValue2>(aFieldValue2));
		CPPUNIT_ASSERT_EQUAL(3.4, get<kValue3>(aFieldValue2));

		FieldValue3 aFieldValue3(aFieldValue1, 3.4);
		CPPUNIT_ASSERT_EQUAL(1L, static_cast<long>(get<kValue1>(aFieldValue3)));
		CPPUNIT_ASSERT_EQUAL(2L, get<kValue2>(aFieldValue3));
		CPPUNIT_ASSERT_EQUAL(3.4, get<kValue3>(aFieldValue3));
	}

	/// ARCTL_FIELD_VALUE_CLASS への _IRowPtr 代入確認。
	void testSubstitionM1()
	{
		using namespace arctl::field;

		_ICursorPtr ipCursor;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->Search(0, VARIANT_FALSE, &ipCursor));

		_IRowPtr ipRow;
		while(ipCursor->NextRow(&ipRow) == S_OK) {
			FieldValue aFieldValue;
			aFieldValue = ipRow;
			CPPUNIT_ASSERT_EQUAL(ipRow->OID, static_cast<long>(get<kValue1>(aFieldValue)));
			CPPUNIT_ASSERT_EQUAL(ipRow->OID * 2L, get<kValue2>(aFieldValue));
		}
	}

	// ARCTL_FIELD_VALUE_CLASS の比較演算テスト。
	void testCompareM1()
	{
		using namespace arctl::field;

		FieldValue2 aFieldValue[2];
		get<kValue1>(aFieldValue[0]) = get<kValue1>(aFieldValue[1]) = 0L;
		CPPUNIT_ASSERT(aFieldValue[0] == aFieldValue[1]); // (0,0,0) = (0,0,0)
		CPPUNIT_ASSERT(! (aFieldValue[0] < aFieldValue[1]));
		CPPUNIT_ASSERT(! (aFieldValue[1] < aFieldValue[0]));

		get<kValue3>(aFieldValue[1]) = 1.0;
		CPPUNIT_ASSERT(aFieldValue[0] < aFieldValue[1]); // (0,0,0) < (0,0,1)
		CPPUNIT_ASSERT(! (aFieldValue[0] == aFieldValue[1]));
		CPPUNIT_ASSERT(! (aFieldValue[1] < aFieldValue[0]));

		get<kValue2>(aFieldValue[0]) = 1L;
		CPPUNIT_ASSERT(aFieldValue[1] < aFieldValue[0]); // (0,1,0) > (0,0,1)
		CPPUNIT_ASSERT(! (aFieldValue[0] == aFieldValue[1]));
		CPPUNIT_ASSERT(! (aFieldValue[0] < aFieldValue[1]));
	}

	/// ARCTL_FIELD_VALUE_CLASS::write() の動作確認。
	void testWrite()
	{
		using namespace arctl::field;

		IRowBufferPtr ipRowBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRowBuffer(&ipRowBuffer));

		FieldValue aFieldValue;
		get<kValue1>(aFieldValue) = _variant_t(10L);
		get<kValue2>(aFieldValue) = 100L;

		aFieldValue.write(ipRowBuffer);

		CPPUNIT_ASSERT_EQUAL(10L, static_cast<long>(ipRowBuffer->Value[1]));
		CPPUNIT_ASSERT_EQUAL(100L, static_cast<long>(ipRowBuffer->Value[2]));

		// 例外テスト。
		CPPUNIT_ASSERT_THROW(value_t<kValue1>(10L).write(ipRowBuffer, index_t<kValue1>()), ::_com_error);
	}

	/// ARCTL_FIELD_VALUE_CLASS::distribute() の動作確認。
	void testDistributeValue()
	{
		using namespace arctl::field;

		_ICursorPtr ipCursor;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->Search(0, VARIANT_FALSE, &ipCursor));

		_IRowPtr ipRow;
		while(ipCursor->NextRow(&ipRow) == S_OK) {
			FieldValue aFieldValue(ipRow);
			aFieldValue.distribute_value(&checkFieldValue2);
		}
	}

	/// interpret_iterator の動作確認。
	void testInterpretIterator()
	{
		using namespace atl2;
		using namespace arctl::field;

		std::for_each(
			make_interpret_iterator<FieldValue>(begin(m_ipTable->_Search(0, VARIANT_FALSE)), FieldValue::index_type(m_ipTable)),
			make_interpret_iterator<FieldValue>(end<_ICursor>()),
			&checkFieldValue1
		);
	}

	/// get() による値設定の動作確認。
	void testPut()
	{
		using namespace arctl;
		using namespace arctl::field;

		// 値の代入。
		FieldValue2 aFieldValue;
		get<kValue1>(aFieldValue) = _variant_t(10L);
		get<kValue2>(aFieldValue) = 100;
		get<kValue3>(aFieldValue) = 1000.5;

		// 値の確認。
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned short>(VT_I4), get<kValue1>(aFieldValue).vt);
		CPPUNIT_ASSERT_EQUAL(10L, static_cast<long>(get<kValue1>(aFieldValue)));
		CPPUNIT_ASSERT_EQUAL(100L, get<kValue2>(aFieldValue));
		CPPUNIT_ASSERT_EQUAL(1000.5, get<kValue3>(aFieldValue));
	}

	void testCast()
	{
		using namespace arctl;
		using namespace arctl::field;

		// 値の代入。
		const FieldValue2 aFieldValue(_variant_t(10L), 100L, 1000.5);
		const value_t<kValue2> aValue(100L);

		CPPUNIT_ASSERT(value_cast<kValue2>(aFieldValue) == aValue);
		CPPUNIT_ASSERT(value_cast<kValue2>(aFieldValue) == 100L);
	}

	void testOptionalGet()
	{
		using namespace arctl;
		using namespace arctl::field;

		_ICursorPtr ipCursor;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->Search(0, VARIANT_FALSE, &ipCursor));

		const FieldValueO1::index_type aIndex(m_ipTable);

		_IRowPtr ipRow;
		while(ipCursor->NextRow(&ipRow) == S_OK) {
			const FieldValueO1 aFieldValue(ipRow, aIndex);
			CPPUNIT_ASSERT_EQUAL(ipRow->OID, static_cast<long>(get<kValue1>(aFieldValue)));
			CPPUNIT_ASSERT_EQUAL(static_cast<VARTYPE>(VT_EMPTY), V_VT(&get<kValue3>(aFieldValue)));
		}
	}
//@}

private:
// 定義
	ARCTL_FIELD_VALUE_CLASS(FieldValue, (1(kValue1)) (2(kValue2, long)) );
	ARCTL_FIELD_VALUE_CLASS(FieldValue2, (1(kValue1)) (2(kValue2, long)) (2(kValue3, double)) );
	ARCTL_FIELD_VALUE_CLASS(FieldValue3, ((FieldValue)) (2(kValue3, double)) );
	ARCTL_FIELD_VALUE_CLASS(FieldValueO1, (1(kValue1)) (O1(kValue3)) );

// 処理
	static void checkFieldValue1(const FieldValue& rFieldValue)
	{
		using namespace arctl::field;

		CPPUNIT_ASSERT(get<kValue2>(rFieldValue));
		CPPUNIT_ASSERT_EQUAL(get<kValue2>(rFieldValue), static_cast<long>(get<kValue1>(rFieldValue)) * 2L);
	}

	static void checkFieldValue2(long nValue1, long nValue2)
	{
		CPPUNIT_ASSERT(nValue2);
		CPPUNIT_ASSERT_EQUAL(nValue2, nValue1 * 2L);
	}

// 変数
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldValueTest);

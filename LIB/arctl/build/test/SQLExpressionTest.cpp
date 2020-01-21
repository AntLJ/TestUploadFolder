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

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <arctl/sql/expression.h>

namespace {

class ATL_NO_VTABLE CSimpleRowBuffer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IRowBuffer
{
public:
BEGIN_COM_MAP(CSimpleRowBuffer)
	COM_INTERFACE_ENTRY(IRowBuffer)
END_COM_MAP()

// IRowBuffer
	STDMETHOD(get_Fields)(IFields** Fields)
	{
		*Fields = m_ipFields;
		(*Fields)->AddRef();
		return S_OK;
	}

	STDMETHOD(get_Value)(long Index, VARIANT* Value)
	{
		*Value = m_Variants[Index];
		return S_OK;
	}

	STDMETHOD(put_Value)(long Index, VARIANT Value)
	{
		m_Variants[Index] = Value;
		return S_OK;
	}

// CSimpleRowBuffer
	static IRowBufferPtr create(IFields* ipFields)
	{
		typedef CComObject<CSimpleRowBuffer> SimpleRowBuffer;

		SimpleRowBuffer* pSimpleRowBuffer;
		if(FAILED(SimpleRowBuffer::CreateInstance(&pSimpleRowBuffer)))
			return 0;

		pSimpleRowBuffer->m_ipFields = ipFields;

		return IRowBufferPtr(pSimpleRowBuffer);
	}

protected:
	IFieldsPtr m_ipFields;
	std::map<long, _variant_t> m_Variants;
};

} // anonymous namespace

using namespace arctl::sql::expression;

/**
 * @ingroup unit_test
 * arctl::sql::expression::parse() と、その戻り値を使った arctl::sql::expression::evaluate() の @ref unit_test クラス
 */
class EvaluateRowTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(EvaluateRowTest);
	CPPUNIT_TEST(testWithFind);
	CPPUNIT_TEST(testPreFind);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		const IFieldsEditPtr ipFieldsEdit(__uuidof(Fields));

		// 0: kValue1
		IFieldEditPtr ipFieldEdit(__uuidof(Field));
		ipFieldEdit->put_Name(CComBSTR(kValue1));
		ipFieldsEdit->AddField(ipFieldEdit);

		// 1: kValue2
		ipFieldEdit.CreateInstance(__uuidof(Field));
		ipFieldEdit->put_Name(CComBSTR(kValue2));
		ipFieldsEdit->AddField(ipFieldEdit);

		m_ipFields = ipFieldsEdit;
	}

	virtual void tearDown()
	{
		m_ipFields = 0;
	}

/// @name テストケース
//@{ 
	/**
	 * @brief 評価時に属性フィールドを検索する方式の動作テスト。
	 */
	void testWithFind()
	{
		std::basic_ostringstream<TCHAR> oss;
		oss << kValue1 << _T("=1");

		const IRowBufferPtr ipRowBuff(CSimpleRowBuffer::create(m_ipFields));
		const parse_info<LPCTSTR> aParseInfo = (parse(oss.str().c_str()));

		ipRowBuff->put_Value(0, _variant_t(1L));
		CPPUNIT_ASSERT(evaluate(ipRowBuff, aParseInfo.solution));

		ipRowBuff->put_Value(1, _variant_t(3L));
		CPPUNIT_ASSERT(evaluate(ipRowBuff, aParseInfo.solution));

		ipRowBuff->put_Value(0, _variant_t(2L));
		CPPUNIT_ASSERT(! evaluate(ipRowBuff, aParseInfo.solution));
	}

	/**
	 * @brief 解析時に属性フィールドを検索しておく方式の動作テスト。
	 */
	void testPreFind()
	{
		std::basic_ostringstream<TCHAR> oss;
		oss << kValue2 << _T("=10");

		const IRowBufferPtr ipRowBuff(CSimpleRowBuffer::create(m_ipFields));
		const parse_info<LPCTSTR> aParseInfo(parse(oss.str().c_str(), m_ipFields));

		ipRowBuff->put_Value(1, _variant_t(1L));
		CPPUNIT_ASSERT(! evaluate(ipRowBuff, aParseInfo.solution));

		ipRowBuff->put_Value(0, _variant_t(10L));
		CPPUNIT_ASSERT(! evaluate(ipRowBuff, aParseInfo.solution));

		ipRowBuff->put_Value(1, _variant_t(10L));
		CPPUNIT_ASSERT(evaluate(ipRowBuff, aParseInfo.solution));
	}
//@}

private:
	IFieldsPtr m_ipFields;
};

CPPUNIT_TEST_SUITE_REGISTRATION(EvaluateRowTest);

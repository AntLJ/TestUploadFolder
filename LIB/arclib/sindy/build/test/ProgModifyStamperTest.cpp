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
#include <arctl/coinitializer.h>
#include <arctl/pgdb.h>
#include <sindy/stamper/prog_modify_stamper.h>
#include <SiNDYAttrCommunity.h>

using namespace sindy;

/**
 * @ingroup unit_test
 * @brief @ref sindy::prog_modify_stamperの@ref unit_test。
 */
class ProgModifyStamperTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(ProgModifyStamperTest);
	CPPUNIT_TEST(testModifyStamper);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		m_pCoInitializer = new arctl::coinitializer;

		// テスト用のPGDBが残っていたら削除する。
		::DeleteFile(kTmpWorkspace);

		// テスト用のPGDBを作成する。
		IFeatureWorkspacePtr ipFeatureWorkspace = arctl::create_pgdb(kTmpWorkspace);
		CPPUNIT_ASSERT(ipFeatureWorkspace != 0);

		// テスト用のテーブルを作成する。
		IFieldsEditPtr ipFieldsEdit(CLSID_Fields);
		IFieldEditPtr ipFieldEdit;

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(sindy::kObjectId)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeOID));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(sindy::kProgModifyDate)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeDate));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(sindy::kModifyProgName)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeString));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Length(32));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(sindy::kUpdateTypeCode)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeInteger));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(
			S_OK, ipFeatureWorkspace->CreateTable(CComBSTR(_T("test")), ipFieldsEdit, 0, 0, 0, &m_ipTable)
		);
	}

	virtual void tearDown()
	{
		m_ipTable = 0;

		CPPUNIT_ASSERT(::DeleteFile(kTmpWorkspace));

		delete m_pCoInitializer;
	}

//@{ @name テストケース
	void testModifyStamper()
	{
		prog_modify_stamper aModifyStamper(m_ipTable->Fields);

		SYSTEMTIME st;

		IRowBufferPtr ipRowBuffer;
		m_ipTable->CreateRowBuffer(&ipRowBuffer);

		// 前後の時間を記録しながら更新。
		double dStart, dEnd;
		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &dStart);

		CPPUNIT_ASSERT(aModifyStamper(ipRowBuffer, sindy::updatetype_code::kCreated));

		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &dEnd);

		// 確認。
		USES_CONVERSION;
		_variant_t va;
		TCHAR szBuffer[_MAX_PATH];

		// 更新日時の確認。
		CPPUNIT_ASSERT_EQUAL(S_OK, ipRowBuffer->get_Value(1, &va));
		va.ChangeType(VT_DATE);
		CPPUNIT_ASSERT(dStart <= va.date && va.date <= dEnd);

		// プログラム名の確認。
		::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
		CPPUNIT_ASSERT_EQUAL(S_OK, ipRowBuffer->get_Value(2, &va));
		va.ChangeType(VT_BSTR);
		CPPUNIT_ASSERT(::_tcscmp(::_tcsrchr(szBuffer, _T('\\')) + 1, OLE2CT(va.bstrVal)) == 0);

		// 更新内容の確認。
		CPPUNIT_ASSERT_EQUAL(S_OK, ipRowBuffer->get_Value(3, &va));
		va.ChangeType(VT_I4);
		CPPUNIT_ASSERT_EQUAL(static_cast<long>(sindy::updatetype_code::kCreated), va.lVal);
	}
//@}

private:
	arctl::coinitializer* m_pCoInitializer;
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ProgModifyStamperTest);

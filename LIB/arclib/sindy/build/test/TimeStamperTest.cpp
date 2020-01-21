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
#include <sindy/stamper/time_stamper.h>
#include <SiNDYAttrCommunity.h>

using namespace sindy;

/**
 * @ingroup unit_test
 * @brief @ref sindy::time_stamper�A@ref sindy::time_stamper_t��@ref unit_test�B
 */
class TimeStamperTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(TimeStamperTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(test2);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		m_pCoInitializer = new arctl::coinitializer;

		// �e�X�g�p��PGDB���c���Ă�����폜����B
		::DeleteFile(kTmpWorkspace);

		// �e�X�g�p��PGDB���쐬����B
		IFeatureWorkspacePtr ipFeatureWorkspace = arctl::create_pgdb(kTmpWorkspace);
		CPPUNIT_ASSERT(ipFeatureWorkspace != 0);

		// �e�X�g�p�̃e�[�u�����쐬����B
		IFieldsEditPtr ipFieldsEdit(CLSID_Fields);
		IFieldEditPtr ipFieldEdit;

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kObjectId)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeOID));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(kModifyDate)));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeDate));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(
			S_OK,
			ipFeatureWorkspace->CreateTable(CComBSTR(_T("test")), ipFieldsEdit, 0, 0, 0, &m_ipTable)
		);
	}

	virtual void tearDown()
	{
		m_ipTable = 0;

		CPPUNIT_ASSERT(::DeleteFile(kTmpWorkspace));

		delete m_pCoInitializer;
	}

//@{ @name �e�X�g�P�[�X
	/// @ref sindy::time_stamper�̃e�X�g
	void test1()
	{
		time_stamper aStamper(1);

		IRowBufferPtr ipRowBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRowBuffer(&ipRowBuffer));

		SYSTEMTIME st;

		// �O��̎��Ԃ��L�^���Ȃ���X�V�B
		double dStart, dEnd;
		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &dStart);

		CPPUNIT_ASSERT(aStamper(ipRowBuffer));

		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &dEnd);

		// ���؁B
		_variant_t va;
		CPPUNIT_ASSERT_EQUAL(S_OK, ipRowBuffer->get_Value(1, &va));
		va.ChangeType(VT_DATE);
		CPPUNIT_ASSERT(dStart <= va.date && va.date <= dEnd);
	}

	/// @ref sindy::time_stamper_t�̃e�X�g
	void test2()
	{
		time_stamper_t<kModifyDate> aStamper(static_cast<IFields*>(m_ipTable->Fields));

		IRowBufferPtr ipRowBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRowBuffer(&ipRowBuffer));

		SYSTEMTIME st;

		// �O��̎��Ԃ��L�^���Ȃ���X�V�B
		double dStart, dEnd;
		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &dStart);

		CPPUNIT_ASSERT(aStamper(ipRowBuffer));

		::GetLocalTime(&st);
		::SystemTimeToVariantTime(&st, &dEnd);

		// ���؁B
		_variant_t va;
		CPPUNIT_ASSERT_EQUAL(S_OK, ipRowBuffer->get_Value(1, &va));
		va.ChangeType(VT_DATE);
		CPPUNIT_ASSERT(dStart <= va.date && va.date <= dEnd);
	}

//@}

private:
	arctl::coinitializer* m_pCoInitializer;
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeStamperTest);

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
 * @file FieldOIDTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <sstream>
#include <arctl/pgdb.h>
#include <arctl/field/oid.h>

using namespace arctl::field;

/**
 * @ingroup unit_test
 * @brief arctl::field::oid_t �� @ref unit_test �N���X�B
 */
class FieldOIDTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FieldOIDTest);
	CPPUNIT_TEST(testIndexConstruct);
	CPPUNIT_TEST(testIndexNot);
	CPPUNIT_TEST(testValueConstruct);
	CPPUNIT_TEST(testValueWrite);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		// �e�X�g�p��PGDB���c���Ă�����폜����B
		::DeleteFile(kTestWorkspace);

		// �e�X�g�p��PGDB���쐬����B
		IFeatureWorkspacePtr ipFeatureWorkspace = arctl::create_pgdb(kTestWorkspace);
		CPPUNIT_ASSERT(ipFeatureWorkspace != 0);

		// �e�X�g�p�̃e�[�u�����쐬����B
		IFieldsEditPtr ipFieldsEdit(CLSID_Fields);
		IFieldEditPtr ipFieldEdit;

		// ����0:OBJECTID:OID�^�B
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit.CreateInstance(CLSID_Field));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Name(CComBSTR(_T("OBJECTID"))));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldEdit->put_Type(esriFieldTypeOID));
		CPPUNIT_ASSERT_EQUAL(S_OK, ipFieldsEdit->AddField(ipFieldEdit));

		CPPUNIT_ASSERT_EQUAL(
			S_OK, ipFeatureWorkspace->CreateTable(CComBSTR(_T("test_table")), ipFieldsEdit, 0, 0, 0, &m_ipTable)
		);

		// 10�s�قǐ�������B
		for(int i = 0; i < 10; i++) {
			_IRowPtr ipRow;
			CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRow(&ipRow));
			CPPUNIT_ASSERT_EQUAL(S_OK, ipRow->Store());
		}
	}

	virtual void tearDown()
	{
		m_ipTable = 0;

		// �e�X�g�p��PGDB���폜����B
		CPPUNIT_ASSERT_EQUAL(TRUE, ::DeleteFile(kTestWorkspace));
	}

//@{ @name �e�X�g�P�[�X
	/// oid_index_t::oid_index_t() �̊m�F�B
	void testIndexConstruct()
	{
		CPPUNIT_ASSERT_EQUAL(-1L, static_cast<long>(oid_t::index_type()));
		CPPUNIT_ASSERT_EQUAL(0L, static_cast<long>(oid_t::index_type(m_ipTable)));
	}

	/// oid_index_t::operator!() �̊m�F�B
	void testIndexNot()
	{
		CPPUNIT_ASSERT_EQUAL(true, ! oid_t::index_type());
		CPPUNIT_ASSERT_EQUAL(false, ! oid_t::index_type(m_ipTable));
	}

	/// oid_t::oid_t() �̊m�F�B
	void testValueConstruct()
	{
		CPPUNIT_ASSERT_EQUAL(0L, oid_t().value());

		_ICursorPtr ipCursor;
		m_ipTable->Search(0, VARIANT_FALSE, &ipCursor);
		oid_t::index_type aIndexType(m_ipTable);

		_IRowPtr ipRow;
		while(ipCursor->NextRow(&ipRow) == S_OK) {
			CPPUNIT_ASSERT_EQUAL(ipRow->OID, oid_t(ipRow).value());
			CPPUNIT_ASSERT_EQUAL(ipRow->OID, oid_t(ipRow, aIndexType).value());
		}
	}

	/// oid_t::write() �̊m�F�B
	void testValueWrite()
	{
		IRowBufferPtr ipRowBuffer;
		CPPUNIT_ASSERT_EQUAL(S_OK, m_ipTable->CreateRowBuffer(&ipRowBuffer));
		CPPUNIT_ASSERT_EQUAL(false, oid_t().write(ipRowBuffer, oid_t::index_type(m_ipTable)));
	}
//@}

private:
// �ϐ�
	ITablePtr m_ipTable;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FieldOIDTest);

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

#include "StdAfx.h"
#include "CodeConvTableTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CodeConvTableTest );

void CodeConvTableTest::setUp()
{
	m_pConvTable = new CCodeConvTable();
	m_pConvTable->OpenDB( _T("CodeConvTable.mdb") );
}

void CodeConvTableTest::tearDown()
{
	delete m_pConvTable;
}

// �n�ԃR�[�h�擾�e�X�g
void CodeConvTableTest::test_getpntcode()
{
	// �ʏ퓮��m�F
	CPPUNIT_ASSERT_EQUAL( CString(_T("A0000")), m_pConvTable->GetPntCode(_T("11111111110"), _T("�e�X�g1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A0000")), m_pConvTable->GetPntCode(_T("11111111111"), _T("�e�X�g1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A0001")), m_pConvTable->GetPntCode(_T("11111111110"), _T("�e�X�g1-2")) );

	// �����ӂ�m�F(A9999 �� B0000)
	CPPUNIT_ASSERT_EQUAL( CString(_T("B0000")), m_pConvTable->GetPntCode(_T("22222222220"), _T("�e�X�g2")) );

	// ���蓖�Ĕ͈͒����m�F(F9999 �� error)
	CPPUNIT_ASSERT_EQUAL( CString(_T("")), m_pConvTable->GetPntCode(_T("33333333330"), _T("�e�X�g3")) );
}

// ���R�[�h�擾�e�X�g
void CodeConvTableTest::test_getgoucode()
{
	// �ʏ퓮��m�F
	CPPUNIT_ASSERT_EQUAL( CString(_T("A000")), m_pConvTable->GetGouCode(_T("11111111110"), _T("�e�X�g1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A000")), m_pConvTable->GetGouCode(_T("11111111111"), _T("�e�X�g1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A001")), m_pConvTable->GetGouCode(_T("11111111110"), _T("�e�X�g1-2")) );

	// �����ӂ�m�F(A9999 �� B0000)
	CPPUNIT_ASSERT_EQUAL( CString(_T("B000")), m_pConvTable->GetGouCode(_T("22222222220"), _T("�e�X�g2")) );

	// ���蓖�Ĕ͈͒����m�F(F9999 �� error)
	CPPUNIT_ASSERT_EQUAL( CString(_T("")), m_pConvTable->GetGouCode(_T("33333333330"), _T("�e�X�g3")) );
}

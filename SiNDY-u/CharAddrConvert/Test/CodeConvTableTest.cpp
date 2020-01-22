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

// 地番コード取得テスト
void CodeConvTableTest::test_getpntcode()
{
	// 通常動作確認
	CPPUNIT_ASSERT_EQUAL( CString(_T("A0000")), m_pConvTable->GetPntCode(_T("11111111110"), _T("テスト1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A0000")), m_pConvTable->GetPntCode(_T("11111111111"), _T("テスト1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A0001")), m_pConvTable->GetPntCode(_T("11111111110"), _T("テスト1-2")) );

	// 桁あふれ確認(A9999 → B0000)
	CPPUNIT_ASSERT_EQUAL( CString(_T("B0000")), m_pConvTable->GetPntCode(_T("22222222220"), _T("テスト2")) );

	// 割り当て範囲超え確認(F9999 → error)
	CPPUNIT_ASSERT_EQUAL( CString(_T("")), m_pConvTable->GetPntCode(_T("33333333330"), _T("テスト3")) );
}

// 号コード取得テスト
void CodeConvTableTest::test_getgoucode()
{
	// 通常動作確認
	CPPUNIT_ASSERT_EQUAL( CString(_T("A000")), m_pConvTable->GetGouCode(_T("11111111110"), _T("テスト1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A000")), m_pConvTable->GetGouCode(_T("11111111111"), _T("テスト1-1")) );
	CPPUNIT_ASSERT_EQUAL( CString(_T("A001")), m_pConvTable->GetGouCode(_T("11111111110"), _T("テスト1-2")) );

	// 桁あふれ確認(A9999 → B0000)
	CPPUNIT_ASSERT_EQUAL( CString(_T("B000")), m_pConvTable->GetGouCode(_T("22222222220"), _T("テスト2")) );

	// 割り当て範囲超え確認(F9999 → error)
	CPPUNIT_ASSERT_EQUAL( CString(_T("")), m_pConvTable->GetGouCode(_T("33333333330"), _T("テスト3")) );
}

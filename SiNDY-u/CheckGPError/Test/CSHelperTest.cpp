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
#include "CSHelperTest.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>

CPPUNIT_TEST_SUITE_REGISTRATION( CCSHelperTest );

void CCSHelperTest::setUp()
{
	CWorkspace cWorkspace( _T("PRODENG2/PRODENG2/PRODENG2.ETC_CheckGPError2/5151/coral2") );

	CFeatureClass cAdminClass( cWorkspace.OpenTable(CComBSTR(_T("city_admin"))) );
	CFeatureClass cCSClass( cWorkspace.OpenTable(CComBSTR(_T("cs_addr_point"))) );

	m_pHelper = new CCSHelper(cCSClass, cAdminClass);
}

void CCSHelperTest::tearDown()
{
	delete m_pHelper;
}

// [�y�v�m�F�zCS�|�C���g����������]�e�X�g
void CCSHelperTest::test_findPoint1()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("08212086000003250001");
	CPPUNIT_ASSERT_EQUAL( kW_DUPLI_CS, m_pHelper->findPoint(cRec, true) );

	// OID = 1 or 2 �̂����ꂩ�̍��W���i�[�����
	CPPUNIT_ASSERT_EQUAL( 53374213, cRec.m_cMXY_Ref.m_nMesh );	// Mesh
	CPPUNIT_ASSERT( 173772 == cRec.m_cMXY_Ref.m_nX || 235389 == cRec.m_cMXY_Ref.m_nX );		// X
	CPPUNIT_ASSERT( 795794 == cRec.m_cMXY_Ref.m_nY || 801823 == cRec.m_cMXY_Ref.m_nY );		// Y
}

// [�y�v�m�F�zCS�|�C���g�Ɠ���16���̍s���E�����݂��܂�]�e�X�g ... Bug6180
void CCSHelperTest::test_findPoint2()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("14203013000000010000");
	CPPUNIT_ASSERT_EQUAL( kW_CS_SAME_ADMIN, m_pHelper->findPoint(cRec, true) );

	// OID = 49 �̍��W�E���\�[�X���i�[�����
	CPPUNIT_ASSERT_EQUAL( 53374214, cRec.m_cMXY_Ref.m_nMesh );	// Mesh
	CPPUNIT_ASSERT_EQUAL( 123473, cRec.m_cMXY_Ref.m_nX );		// X
	CPPUNIT_ASSERT_EQUAL( 865112, cRec.m_cMXY_Ref.m_nY );		// Y
	CPPUNIT_ASSERT( _T("�e�X�g_�y�v�m�F�zCS�|�C���g�Ɠ���16���̍s���E�����݂��܂�") == cRec.m_strSource );	// ���\�[�X
}

// [�V�K�ǉ�(CS)]�e�X�g
void CCSHelperTest::test_findPoint3()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("08212086000003250003");
	CPPUNIT_ASSERT_EQUAL( kO_CS_ADDED, m_pHelper->findPoint(cRec, true) );

	// OID = 5 �̍��W�E���\�[�X���i�[�����
	CPPUNIT_ASSERT_EQUAL( 53374213, cRec.m_cMXY.m_nMesh );	// Mesh
	CPPUNIT_ASSERT_EQUAL( 325135, cRec.m_cMXY.m_nX );		// X
	CPPUNIT_ASSERT_EQUAL( 798977, cRec.m_cMXY.m_nY );		// Y
	CPPUNIT_ASSERT( _T("�V�K�ǉ�(CS)") == cRec.m_strSource );	// ���\�[�X
}

// [���Ȃ�(CS)]�e�X�g
void CCSHelperTest::test_findPoint4()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("08212086000003250004");
	cRec.m_cMXY.m_nMesh = 53374213;
	cRec.m_cMXY.m_nX = 179100;
	cRec.m_cMXY.m_nY = 683200;
	CPPUNIT_ASSERT_EQUAL( kO_CS_OK, m_pHelper->findPoint(cRec, false) );

	// OID = 3 �̏��\�[�X���i�[�����
	CPPUNIT_ASSERT( _T("���Ȃ�(CS)") == cRec.m_strSource );	// ���\�[�X
}

// [�y�v�m�F�zCS�|�C���g�ړ�]�e�X�g
void CCSHelperTest::test_findPoint5()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("08212086000003250005");
	cRec.m_cMXY.m_nMesh = 53374213;
	cRec.m_cMXY.m_nX = 314932;
	cRec.m_cMXY.m_nY = 397879;
	CPPUNIT_ASSERT_EQUAL( kW_CS_MOVED, m_pHelper->findPoint(cRec, false) );

	// OID = 4 �̍��W�E���\�[�X���i�[�����
	CPPUNIT_ASSERT_EQUAL( 53374213, cRec.m_cMXY_Ref.m_nMesh );	// Mesh
	CPPUNIT_ASSERT_EQUAL( 177715, cRec.m_cMXY_Ref.m_nX );		// X
	CPPUNIT_ASSERT_EQUAL( 533681, cRec.m_cMXY_Ref.m_nY );		// Y
	CPPUNIT_ASSERT( _T("�y�v�m�F�zCS�|�C���g�ړ�") == cRec.m_strSource );	// ���\�[�X
}

// [������Ȃ�]�e�X�g
void CCSHelperTest::test_findPoint6()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("01234567890123456789");
	CPPUNIT_ASSERT_EQUAL( kO_NOT_FOUND, m_pHelper->findPoint(cRec, true) );
}

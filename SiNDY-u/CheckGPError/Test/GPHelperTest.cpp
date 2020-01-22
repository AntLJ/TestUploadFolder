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
#include "GPHelperTest.h"
#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>

CPPUNIT_TEST_SUITE_REGISTRATION( CGPHelperTest );

void CGPHelperTest::setUp()
{
	CWorkspace cWorkspace( _T("PRODENG2/PRODENG2/PRODENG2.ETC_CheckGPError2/5151/coral2") );
	CWorkspace cWorkspace_Ref( _T("PRODENG1/PRODENG1/PRODENG1.ETC_CheckGPError/5151/coral2") );

	CFeatureClass cAdminClass( cWorkspace.OpenTable(CComBSTR(_T("city_admin"))) );
	CFeatureClass cGouClass( cWorkspace.OpenTable(CComBSTR(_T("gou_point"))) );
	CFeatureClass cAdminClass_Ref( cWorkspace_Ref.OpenTable(CComBSTR(_T("city_admin"))) );
	CFeatureClass cGouClass_Ref( cWorkspace_Ref.OpenTable(CComBSTR(_T("gou_point"))) );

	m_pHelper = new CGPHelper( cGouClass, cGouClass_Ref, cAdminClass, cAdminClass_Ref );
}

void CGPHelperTest::tearDown()
{
	delete m_pHelper;
}

// [y—vŠm”Fz“¯ˆêƒR[ƒh‚Ì•„†ŠE‚ª‘¶Ý]ƒeƒXƒg
void CGPHelperTest::test_findPoint1()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("08212087000000050000");
	CPPUNIT_ASSERT_EQUAL( kW_GP_SAME_ADMIN, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00000000");
	cRec.m_strAddrCodeNew = _T("13101001001A00000000");
	CPPUNIT_ASSERT_EQUAL( kW_GP_SAME_ADMIN, m_pHelper->findPoint(cRec, true) );
}

// [y—vŠm”Fz—Dæƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚È‚¢]ƒeƒXƒg
void CGPHelperTest::test_findPoint2()
{
	CLRec cRec;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212087000000050016");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(16Œ…)", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00000001");
	cRec.m_strAddrCodeNew = _T("13101001001A00000001");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(16Œ…)_•¶Žš•t‚«", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212089000000050016");
	cRec.m_strAddrCodeNew = _T("08212089000000050016");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(11Œ…)", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100100001A000");
	cRec.m_strAddrCodeNew = _T("1310100100100001A000");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(11Œ…)_•¶Žš•t‚«", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212091000000050016");
	cRec.m_strAddrCodeNew = _T("08212091000000050016");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(11Œ…+Šg’£†)", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00010001");
	cRec.m_strAddrCodeNew = _T("13101001001A00010001");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(11Œ…+Šg’£†)_•¶Žš•t‚«", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212093000000050016");
	cRec.m_strAddrCodeNew = _T("08212093000000050016");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(’ÊÌs­ŠE)", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100000001A000");
	cRec.m_strAddrCodeNew = _T("1310100100000001A000");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test2(’ÊÌs­ŠE)_•¶Žš•t‚«", kW_GP_NOT_PRIOR, m_pHelper->findPoint(cRec, true) );
}

// [V‹K’Ç‰Á^y—vŠm”Fz—Dæƒtƒ‰ƒO‚Ì—§‚Á‚Ä‚¢‚éƒ|ƒCƒ“ƒg‚ªTPG]ƒeƒXƒg
void CGPHelperTest::test_findPoint3()
{
	CLRec cRec;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212087000000050017");
	cRec.m_strAddrCodeNew = _T("08212087000000050017");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(16Œ…)", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00000002");
	cRec.m_strAddrCodeNew = _T("13101001001A00000002");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(16Œ…)_•¶Žš•t‚«", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212089000000050017");
	cRec.m_strAddrCodeNew = _T("08212089000000050017");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(11Œ…)", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100100002A000");
	cRec.m_strAddrCodeNew = _T("1310100100100002A000");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(11Œ…)_•¶Žš•t‚«", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212091000000050017");
	cRec.m_strAddrCodeNew = _T("08212091000000050017");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(11Œ…+Šg’£†)", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00010002");
	cRec.m_strAddrCodeNew = _T("13101001001A00010002");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(11Œ…+Šg’£†)_•¶Žš•t‚«", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212093000000050017");
	cRec.m_strAddrCodeNew = _T("08212093000000050017");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(’ÊÌs­ŠE)", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100000002A000");
	cRec.m_strAddrCodeNew = _T("1310100100000002A000");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test3(’ÊÌs­ŠE)_•¶Žš•t‚«", kW_GP_ADDED_PRIOR_TPG, m_pHelper->findPoint(cRec, true) );
}

// [V‹K’Ç‰Á]ƒeƒXƒg
void CGPHelperTest::test_findPoint4()
{
	CLRec cRec;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212087000000050018");
	cRec.m_strAddrCodeNew = _T("08212087000000050018");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(16Œ…)", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00000003");
	cRec.m_strAddrCodeNew = _T("13101001001A00000003");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(16Œ…)_•¶Žš•t‚«", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212089000000050018");
	cRec.m_strAddrCodeNew = _T("08212089000000050018");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(11Œ…)", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100100003A000");
	cRec.m_strAddrCodeNew = _T("1310100100100003A000");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(11Œ…)_•¶Žš•t‚«", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212091000000050018");
	cRec.m_strAddrCodeNew = _T("08212091000000050018");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(11Œ…+Šg’£†)", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("13101001001A00010003");
	cRec.m_strAddrCodeNew = _T("13101001001A00010003");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(11Œ…+Šg’£†)_•¶Žš•t‚«", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCode = _T("08212093000000050018");
	cRec.m_strAddrCodeNew = _T("08212093000000050018");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(’ÊÌs­ŠE)", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100000003A000");
	cRec.m_strAddrCodeNew = _T("1310100100000003A000");
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test4(’ÊÌs­ŠE)_•¶Žš•t‚«", kO_GP_ADDED_PRIOR, m_pHelper->findPoint(cRec, true) );
}

// [–â‘è‚È‚µ]ƒeƒXƒg
void CGPHelperTest::test_findPoint5()
{
	CLRec cRec;
	cRec.m_cMXY.m_nMesh = 53374213;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212088000000060018");
	cRec.m_cMXY.m_nX = 411389;
	cRec.m_cMXY.m_nY = 413200;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(16Œ…)", kO_GP_OK, m_pHelper->findPoint(cRec, false) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212090000000060018");
	cRec.m_cMXY.m_nX = 579300;
	cRec.m_cMXY.m_nY = 172300;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(11Œ…)", kO_GP_OK, m_pHelper->findPoint(cRec, false) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212092000000060018");
	cRec.m_cMXY.m_nX = 792900;
	cRec.m_cMXY.m_nY = 331400;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(11Œ…+Šg’£†)", kO_GP_OK, m_pHelper->findPoint(cRec, false) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212094000000060018");
	cRec.m_cMXY.m_nX = 805100;
	cRec.m_cMXY.m_nY = 686800;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(’ÊÌs­ŠE)", kO_GP_OK, m_pHelper->findPoint(cRec, false) );

	CLRec cRec2;
	cRec2.m_cMXY.m_nMesh = 53374204;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("13101001001A00000004");
	cRec2.m_strAddrCodeNew = _T("13101001001A00000004");
	cRec2.m_cMXY.m_nX = 100100;
	cRec2.m_cMXY.m_nY = 912148;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(16Œ…)_•¶Žš•t‚«", kO_GP_OK, m_pHelper->findPoint(cRec2, false) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("1310100100100004A000");
	cRec2.m_strAddrCodeNew = _T("1310100100100004A000");
	cRec2.m_cMXY.m_nX = 518174;
	cRec2.m_cMXY.m_nY = 914762;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(11Œ…)_•¶Žš•t‚«", kO_GP_OK, m_pHelper->findPoint(cRec2, false) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("13101001001A00010004");
	cRec2.m_strAddrCodeNew = _T("13101001001A00010004");
	cRec2.m_cMXY.m_nX = 215375;
	cRec2.m_cMXY.m_nY = 759877;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(11Œ…+Šg’£†)_•¶Žš•t‚«", kO_GP_OK, m_pHelper->findPoint(cRec2, false) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("1310100100000004A000");
	cRec2.m_strAddrCodeNew = _T("1310100100000004A000");
	cRec2.m_cMXY.m_nX = 177562;
	cRec2.m_cMXY.m_nY = 429509;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test5(’ÊÌs­ŠE)_•¶Žš•t‚«", kO_GP_OK, m_pHelper->findPoint(cRec2, false) );
}

// [y—vŠm”Fz—Dæƒtƒ‰ƒO‚Ì—§‚Á‚Ä‚¢‚éƒ|ƒCƒ“ƒg‚ªTPG]ƒeƒXƒg
void CGPHelperTest::test_findPoint6()
{
	CLRec cRec;
	cRec.m_cMXY.m_nMesh = 53374213;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212088000000060017");
	cRec.m_cMXY.m_nX = 378520;
	cRec.m_cMXY.m_nY = 417400;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(16Œ…)", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec, false) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212090000000060017");
	cRec.m_cMXY.m_nX = 661300;
	cRec.m_cMXY.m_nY = 245320;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(11Œ…)", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec, false) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212092000000060017");
	cRec.m_cMXY.m_nX = 837300;
	cRec.m_cMXY.m_nY = 405800;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(11Œ…+Šg’£†)", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec, false) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212094000000060017");
	cRec.m_cMXY.m_nX = 848300;
	cRec.m_cMXY.m_nY = 754900;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(’ÊÌs­ŠE)", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec, false) );

	CLRec cRec2;
	cRec2.m_cMXY.m_nMesh = 53374204;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("13101001001A00000002");
	cRec2.m_strAddrCodeNew = _T("13101001001A00000002");
	cRec2.m_cMXY.m_nX = 67413;
	cRec2.m_cMXY.m_nY = 881190;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(16Œ…)_•¶Žš•t‚«", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec2, false) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("1310100100100002A000");
	cRec2.m_strAddrCodeNew = _T("1310100100100002A000");
	cRec2.m_cMXY.m_nX = 420100;
	cRec2.m_cMXY.m_nY = 870826;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(11Œ…)_•¶Žš•t‚«", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec2, false) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("13101001001A00010002");
	cRec2.m_strAddrCodeNew = _T("13101001001A00010002");
	cRec2.m_cMXY.m_nX = 58800;
	cRec2.m_cMXY.m_nY = 713424;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(11Œ…+Šg’£†)_•¶Žš•t‚«", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec2, false) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("1310100100000002A000");
	cRec2.m_strAddrCodeNew = _T("1310100100000002A000");
	cRec2.m_cMXY.m_nX = 65682;
	cRec2.m_cMXY.m_nY = 419246;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test6(’ÊÌs­ŠE)_•¶Žš•t‚«", kW_GP_PRIOR_TPG, m_pHelper->findPoint(cRec2, false) );
}

// [y—vŠm”Fz†ƒ|ƒCƒ“ƒgˆÚ“®]ƒeƒXƒg
void CGPHelperTest::test_findPoint7()
{
	CLRec cRec;
	cRec.m_cMXY.m_nMesh = 53374213;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212088000000060019");
	cRec.m_cMXY.m_nX = 394908;
	cRec.m_cMXY.m_nY = 448404;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(16Œ…)", kW_GP_MOVED, m_pHelper->findPoint(cRec, false) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212090000000060019");
	cRec.m_cMXY.m_nX = 394908;
	cRec.m_cMXY.m_nY = 448404;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(11Œ…)", kW_GP_MOVED, m_pHelper->findPoint(cRec, false) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212092000000060019");
	cRec.m_cMXY.m_nX = 849000;
	cRec.m_cMXY.m_nY = 326200;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(11Œ…+Šg’£†)", kW_GP_MOVED, m_pHelper->findPoint(cRec, false) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec.m_strAddrCodeNew = _T("08212094000000060019");
	cRec.m_cMXY.m_nX = 849000;
	cRec.m_cMXY.m_nY = 326200;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(’ÊÌs­ŠE)", kW_GP_MOVED, m_pHelper->findPoint(cRec, false) );

	CLRec cRec2;
	cRec2.m_cMXY.m_nMesh = 53374204;

	// 16Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("13101001001A00000004");
	cRec2.m_strAddrCodeNew = _T("13101001001A00000004");
	cRec2.m_cMXY.m_nX = 101100;
	cRec2.m_cMXY.m_nY = 911148;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(16Œ…)_•¶Žš•t‚«", kW_GP_MOVED, m_pHelper->findPoint(cRec2, false) );

	// 11Œ…s­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("1310100100100004A000");
	cRec2.m_strAddrCodeNew = _T("1310100100100004A000");
	cRec2.m_cMXY.m_nX = 511174;
	cRec2.m_cMXY.m_nY = 911762;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(11Œ…)_•¶Žš•t‚«", kW_GP_MOVED, m_pHelper->findPoint(cRec2, false) );

	// 11Œ… + Šg’£†‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("13101001001A00010004");
	cRec2.m_strAddrCodeNew = _T("13101001001A00010004");
	cRec2.m_cMXY.m_nX = 21375;
	cRec2.m_cMXY.m_nY = 75987;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(11Œ…+Šg’£†)_•¶Žš•t‚«", kW_GP_MOVED, m_pHelper->findPoint(cRec2, false) );

	// ’ÊÌs­ŠE‚©‚çŽæ“¾Žž
	cRec2.m_strAddrCode = _T("1310100100000004A000");
	cRec2.m_strAddrCodeNew = _T("1310100100000004A000");
	cRec2.m_cMXY.m_nX = 17752;
	cRec2.m_cMXY.m_nY = 429109;
	CPPUNIT_ASSERT_EQUAL_MESSAGE( "Test7(’ÊÌs­ŠE)_•¶Žš•t‚«", kW_GP_MOVED, m_pHelper->findPoint(cRec2, false) );
}

// [Bug6181]ƒeƒXƒg
void CGPHelperTest::test_findPoint8()
{
	CLRec cRec;
	cRec.m_strAddrCodeNew = _T("14203013000000020001");
	cRec.m_cMXY.m_nMesh = 53374214;
	cRec.m_cMXY.m_nX = 317701;
	cRec.m_cMXY.m_nY = 626011;

	// 16Œ…ŒŸõ‚ÌŒ‹‰Ê(—Dæƒtƒ‰ƒO‚È‚µƒ|ƒCƒ“ƒg‚ªŒ©‚Â‚©‚Á‚½‚Æ‚«)A
	// 11Œ…+Šg’£†ŒŸõ‚ÌŒ‹‰Ê‚ª—Dæƒtƒ‰ƒO‚ ‚è‚¾‚Á‚½‚ç‚»‚ê‚ª—Dæ‚³‚ê‚é
	CPPUNIT_ASSERT_EQUAL( kW_GP_MOVED, m_pHelper->findPoint(cRec, false) );

	// Šg’£†ƒ|ƒCƒ“ƒg‚ÌÀ•Wƒ`ƒFƒbƒN(OID = 3769‚Ì‚à‚Ì‚ªŒ©‚Â‚©‚é‚Í‚¸)
	CPPUNIT_ASSERT_EQUAL( 53374214, cRec.m_cMXY_Ref.m_nMesh );	// Mesh
	CPPUNIT_ASSERT_EQUAL( 317701, cRec.m_cMXY_Ref.m_nX );		// X
	CPPUNIT_ASSERT_EQUAL( 621992, cRec.m_cMXY_Ref.m_nY );		// Y

	CLRec cRec2;
	cRec2.m_strAddrCode = _T("13101001002A00000001");
	cRec2.m_strAddrCodeNew = _T("13101001002A00000001");
	cRec2.m_cMXY.m_nMesh = 53374204;
	cRec2.m_cMXY.m_nX = 479189;
	cRec2.m_cMXY.m_nY = 458931;

	// 16Œ…ŒŸõ‚ÌŒ‹‰Ê(—Dæƒtƒ‰ƒO‚È‚µƒ|ƒCƒ“ƒg‚ªŒ©‚Â‚©‚Á‚½‚Æ‚«)A
	// 11Œ…+Šg’£†ŒŸõ‚ÌŒ‹‰Ê‚ª—Dæƒtƒ‰ƒO‚ ‚è‚¾‚Á‚½‚ç‚»‚ê‚ª—Dæ‚³‚ê‚é
	CPPUNIT_ASSERT_EQUAL( kW_GP_MOVED, m_pHelper->findPoint(cRec2, false) );

	// Šg’£†ƒ|ƒCƒ“ƒg‚ÌÀ•Wƒ`ƒFƒbƒN(OID = 3769‚Ì‚à‚Ì‚ªŒ©‚Â‚©‚é‚Í‚¸)
	CPPUNIT_ASSERT_EQUAL( 53374204, cRec2.m_cMXY_Ref.m_nMesh );	// Mesh
	CPPUNIT_ASSERT_EQUAL( 589982, cRec2.m_cMXY_Ref.m_nX );		// X
	CPPUNIT_ASSERT_EQUAL( 467039, cRec2.m_cMXY_Ref.m_nY );		// Y

}

// [y—vŠm”Fz“¯ˆê20Œ…ƒR[ƒh‚Å—Dæƒtƒ‰ƒO—§‚Á‚Ä‚¢‚é‚à‚Ì‚ª•¡”‚ ‚è]ƒeƒXƒg
void CGPHelperTest::test_findPoint9()
{
	CLRec cRec;
	cRec.m_strAddrCode = _T("14203022001000010001");
	cRec.m_strAddrCodeNew = _T("14203022001000010001");
	CPPUNIT_ASSERT_EQUAL( kW_GP_DUPLI_PRIOR, m_pHelper->findPoint(cRec, true) );

	cRec.m_strAddrCode = _T("1310100100000005A000");
	cRec.m_strAddrCodeNew = _T("1310100100000005A000");
	CPPUNIT_ASSERT_EQUAL( kW_GP_DUPLI_PRIOR, m_pHelper->findPoint(cRec, true) );

}

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
#include "ErrObj.h"

/**
 * @class CTestErrObj
 * @brief �G���[�I�u�W�F�N�g�e�X�g�N���X
 */
class CTestErrObj : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CTestErrObj);
  CPPUNIT_TEST(testErrObj_test);
  CPPUNIT_TEST_SUITE_END();
public:
	CTestErrObj()
	{
	}
	virtual ~CTestErrObj(){}

	void testErrObj_test()
	{
		// [test] ToString �P��
		CErrObjContainer errs;
		IGeometryPtr ipGeom( CLSID_Polyline );
		WKSPoint p = { 135.0, 99.0 };
		errs.add( _T("ROAD_LINK"), 100, ipGeom, -1, p, -1, sindyeLogLevel_ERR, _T("����[�Ă��Ƃ���%s %d"), _T("[hoge]"), 33 );
		CPPUNIT_ASSERT_EQUAL( CString(_T("[ROAD_LINK.100]\n  [ERR ] ����[�Ă��Ƃ���[hoge] 33")), errs.ToString() );
		// [test] ToString �P��t�B�[�`���������O
		errs.add( _T("ROAD_LINK"), 100, ipGeom, -1, p, -1, sindyeLogLevel_WARN, _T("��[�ɂ�Ă��Ƃ���%s %s %d"), _T("aaa"), _T("hoge"), 103 );
		CPPUNIT_ASSERT_EQUAL( CString(_T("[ROAD_LINK.100]\n  [ERR ] ����[�Ă��Ƃ���[hoge] 33\n  [WARN] ��[�ɂ�Ă��Ƃ���aaa hoge 103")), errs.ToString() );

		// [test] ToLog
		FILE* out = _tfopen( _T("free.log"), _T("w") );
		errs.ToLog( sindyeLogFormat_Freestyle, out );
		fclose(out);
		out = _tfopen( _T("common.log"), _T("w") );
		errs.ToLog( sindyeLogFormat_Common, out );
		fclose(out);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(CTestErrObj);

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
#include "aheGeometryOp.h"

/**
 * @class CTestRemoveAdjoin
 * @brief 重複構成点削除ルーチンテストクラス
 * <h3>チェック内容：<h3>
 * @li 
 */
class CTestRemoveAdjoin : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CTestRemoveAdjoin);
  CPPUNIT_TEST(testRemoveAdjoin_RemoveStartPointRepetition);
  CPPUNIT_TEST(testRemoveAdjoin_RemoveEndPointRepetition);
  CPPUNIT_TEST(testRemoveAdjoin_RemoveMiddlePointRepetition);
  CPPUNIT_TEST(testRemoveAdjoin_RemoveStepPointRepetition);
  CPPUNIT_TEST(testRemoveAdjoin_RemoveMultiPointRepetition);
  CPPUNIT_TEST_SUITE_END();
public:
	CTestRemoveAdjoin()
	{
	}
	virtual ~CTestRemoveAdjoin(){}

	/// 開始点の重複を削除することができるかどうかのテスト
	void testRemoveAdjoin_RemoveStartPointRepetition()
	{
		WKSPoint pPoints[6] = {
			{ 1.0, 1.0 },
			{ 1.0, 1.0 },
			{ 1.0, 2.0 },
			{ 2.0, 2.0 },
			{ 2.0, 1.0 },
			{ 1.0, 1.0 }
		};
		IGeometryPtr ipGeom( CLSID_Polygon );
		((IPointCollectionPtr)ipGeom)->SetWKSPoints( 6, pPoints );

		IGeometryPtr ipNewGeom( AheRemoveAdjoiningPoint( ipGeom ) );

		long lCount = 0;
		((IPointCollectionPtr)ipNewGeom)->get_PointCount( &lCount );
		AheTraceGeometry( ipNewGeom, _T("ipNewGeom") );
		CPPUNIT_ASSERT( lCount == 5 );
	}
	/// 中間点の重複を削除することができるかどうかのテスト
	void testRemoveAdjoin_RemoveMiddlePointRepetition()
	{
		WKSPoint pPoints[6] = {
			{ 1.0, 1.0 },
			{ 1.0, 2.0 },
			{ 2.0, 2.0 },
			{ 2.0, 2.0 },
			{ 2.0, 1.0 },
			{ 1.0, 1.0 }
		};
		IGeometryPtr ipGeom( CLSID_Polygon );
		((IPointCollectionPtr)ipGeom)->SetWKSPoints( 6, pPoints );

		IGeometryPtr ipNewGeom( AheRemoveAdjoiningPoint( ipGeom ) );

		long lCount = 0;
		((IPointCollectionPtr)ipNewGeom)->get_PointCount( &lCount );
		AheTraceGeometry( ipNewGeom, _T("ipNewGeom") );
		CPPUNIT_ASSERT( lCount == 5 );
	}
	/// 中間点の重複を削除することができるかどうかのテスト
	void testRemoveAdjoin_RemoveEndPointRepetition()
	{
		WKSPoint pPoints[6] = {
			{ 1.0, 1.0 },
			{ 1.0, 2.0 },
			{ 2.0, 2.0 },
			{ 2.0, 1.0 },
			{ 1.0, 1.0 },
			{ 1.0, 1.0 },
		};
		IGeometryPtr ipGeom( CLSID_Polygon );
		((IPointCollectionPtr)ipGeom)->SetWKSPoints( 6, pPoints );

		IGeometryPtr ipNewGeom( AheRemoveAdjoiningPoint( ipGeom ) );

		long lCount = 0;
		((IPointCollectionPtr)ipNewGeom)->get_PointCount( &lCount );
		AheTraceGeometry( ipNewGeom, _T("ipNewGeom") );
		CPPUNIT_ASSERT( lCount == 5 );
	}
	/// 飛び地の重複を削除することができるかどうかのテスト
	void testRemoveAdjoin_RemoveStepPointRepetition()
	{
		WKSPoint pPoints[6] = {
			{ 1.0, 1.0 },
			{ 1.0, 2.0 },
			{ 2.0, 2.0 },
			{ 1.0, 2.0 },
			{ 2.0, 1.0 },
			{ 1.0, 1.0 },
		};
		IGeometryPtr ipGeom( CLSID_Polygon );
		((IPointCollectionPtr)ipGeom)->SetWKSPoints( 6, pPoints );

		IGeometryPtr ipNewGeom( AheRemoveAdjoiningPoint( ipGeom ) );

		long lCount = 0;
		((IPointCollectionPtr)ipNewGeom)->get_PointCount( &lCount );
		AheTraceGeometry( ipNewGeom, _T("ipNewGeom") );
		CPPUNIT_ASSERT( lCount == 5 );
	}
	/// 飛び地の重複を削除することができるかどうかのテスト
	void testRemoveAdjoin_RemoveMultiPointRepetition()
	{
		WKSPoint pPoints[10] = {
			{ 1.0, 1.0 },
			{ 1.0, 2.0 },
			{ 2.0, 2.0 },
			{ 1.0, 2.0 },
			{ 2.0, 1.0 },
			{ 2.0, 2.0 },
			{ 2.0, 2.0 },
			{ 2.0, 2.0 },
			{ 2.0, 2.0 },
			{ 1.0, 1.0 },
		};
		IGeometryPtr ipGeom( CLSID_Polygon );
		((IPointCollectionPtr)ipGeom)->SetWKSPoints( 10, pPoints );

		IGeometryPtr ipNewGeom( AheRemoveAdjoiningPoint( ipGeom ) );

		long lCount = 0;
		((IPointCollectionPtr)ipNewGeom)->get_PointCount( &lCount );
		AheTraceGeometry( ipNewGeom, _T("ipNewGeom") );
		CPPUNIT_ASSERT( lCount == 5 );
	}

};

CPPUNIT_TEST_SUITE_REGISTRATION(CTestRemoveAdjoin);

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
#include "../../lib/crd_cnv/coord_converter.h"

/**
 * @class CTestAheEquals
 * @biref AheEqualsルーチンテストクラス
 */
class CTestAheEquals : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(CTestAheEquals);
		CPPUNIT_TEST(testAheEquals_NULL);
		CPPUNIT_TEST(testAheEquals_Empty);
		CPPUNIT_TEST(testAheEquals_Type);
		CPPUNIT_TEST(testAheEquals_Length);
		CPPUNIT_TEST(testAheEquals_Area);
		CPPUNIT_TEST(testAheEquals_Point);
		CPPUNIT_TEST(testAheEquals_Line);
		CPPUNIT_TEST(testAheEquals_Polygon);
	CPPUNIT_TEST_SUITE_END();
public:
	CTestAheEquals()
	{
		ISpatialReferenceFactoryPtr ipSpF( CLSID_SpatialReferenceEnvironment );
		IGeographicCoordinateSystemPtr ipGeoCS;
		if( SUCCEEDED( ipSpF->CreateGeographicCoordinateSystem( esriSRGeoCS_Tokyo, &ipGeoCS ) ) )
		{
			ipSpRef = ipGeoCS;
			ipSpRef->SetDomain( 122.0, 155.0, 19.0, 50.0 );
			ipSpRef->SetFalseOriginAndUnits( 122.0, 19.0, 64000000 );
			ipSpRef->SetZFalseOriginAndUnits( 0, 1 );
			ipSpRef->SetMFalseOriginAndUnits( 0, 1 );
		}

		ipPoint.CreateInstance( CLSID_Point );
		ipPoint->putref_SpatialReference( ipSpRef );
		ipPoint->PutCoords( 135, 49 );
		ipPolyline.CreateInstance( CLSID_Polyline );
		ipPolyline->putref_SpatialReference( ipSpRef );
		WKSPoint pPoints[3] = { {0,0}, {0,1}, {1,1} };
		((IPointCollectionPtr)ipPolyline)->SetWKSPoints( 3, pPoints );
		ipPolygon.CreateInstance( CLSID_Polygon );
		ipPolygon->putref_SpatialReference( ipSpRef );
		((IPointCollectionPtr)ipPolygon)->SetWKSPoints( 3, pPoints );
		ipPolygon->Close();
		ipPolygon->SimplifyPreserveFromTo();
	}
	virtual ~CTestAheEquals(){}

	// NULLを渡したときのテスト
	void testAheEquals_NULL()
	{
		CPPUNIT_ASSERT( FALSE == AheIsEqual( NULL, NULL ) );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint, NULL ) );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( NULL, ipPoint ) );
	}
	// 空形状を渡したときのテスト
	void testAheEquals_Empty()
	{
		IPointPtr ipPoint1( CLSID_Point ), ipPoint2(CLSID_Point);
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint1, ipPoint2 ) );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint1, ipPoint ) );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint, ipPoint1 ) );
	}
	// 別の型を渡したときのテスト
	void testAheEquals_Type()
	{
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint, ipPolyline ) );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint, ipPolygon ) );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPolyline, ipPolygon ) );
		CPPUNIT_ASSERT( AheIsEqual( ipPoint, AheCreateClone( ipPoint ) ) );
		CPPUNIT_ASSERT( AheIsEqual( ipPolyline, AheCreateClone( ipPolyline ) ) );
		CPPUNIT_ASSERT( AheIsEqual( ipPolygon, AheCreateClone( ipPolygon ) ) );
	}
	// 距離が異なる場合のテスト（ポリライン）
	void testAheEquals_Length()
	{
		IPolylinePtr ipLine2( CLSID_Polyline );
		ipLine2->putref_SpatialReference( ipSpRef );
		WKSPoint p[3] = { {0,0}, {0,1}, {2,2} };
		((IPointCollectionPtr)ipLine2)->SetWKSPoints( 3, p );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPolyline, ipLine2 ) );
	}
	// 面積が異なる場合のテスト（ポリゴン）
	void testAheEquals_Area()
	{
		IPolygonPtr ipGon2( CLSID_Polygon );
		ipGon2->putref_SpatialReference( ipSpRef );
		WKSPoint p[3] = { {0,0}, {0,1}, {2,2} };
		((IPointCollectionPtr)ipGon2)->SetWKSPoints( 3, p );
		ipGon2->Close();
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPolygon, ipGon2 ) );
	}
	// ポイント同士の比較でのテスト
	void testAheEquals_Point()
	{
		// 同形状
		CPPUNIT_ASSERT( AheIsEqual( ipPoint, AheCreateClone( ipPoint ) ) );
		// 別形状
		IPointPtr ipP(CLSID_Point);
		ipP->putref_SpatialReference( ipSpRef );
		ipP->PutCoords(2,2);
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint, ipP ) );
		// 1Unitしか離れていない場合
		double d = AheGetXYDomain1UnitLength( AheGetSpatialReference(ipPoint) );
		ipP->PutCoords(135.0,49.0 + d);
		VARIANT_BOOL vbEquals = VARIANT_FALSE;
		((IRelationalOperatorPtr)ipPoint)->Equals( ipP, &vbEquals );
		CPPUNIT_ASSERT( vbEquals );
		((IClonePtr)ipPoint)->IsEqual( (IClonePtr)ipP, &vbEquals );
		CPPUNIT_ASSERT( vbEquals );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPoint, ipP ) );
	}
	// ライン同士の比較でのテスト
	void testAheEquals_Line()
	{
		// 同形状
		CPPUNIT_ASSERT( AheIsEqual( ipPolyline, AheCreateClone( ipPolyline ) ) );
		// 異形状
		IPolylinePtr ipLine(CLSID_Polyline);
		ipLine->putref_SpatialReference( ipSpRef );
		WKSPoint p[3] = { {1,1}, {2,2},{3,3} };
		((IPointCollectionPtr)ipLine)->SetWKSPoints( 3, p );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPolyline, ipLine ) );
		// 同形状（反転）
		ipLine = AheCreateClone( ipPolyline );
		ipLine->ReverseOrientation();
		CPPUNIT_ASSERT( AheIsEqual( ipPolyline, ipLine ) );
		// 同形状だが構成点数が異なるもの
		WKSPoint p2[2] = { {0,1}, {0,3} };
		WKSPoint p3[3] = { {0,1}, {0,2}, {0,3} };
		ipLine->SetEmpty();
		((IPointCollectionPtr)ipLine)->SetWKSPoints( 2, p2 );
		IPolylinePtr ipLine3(CLSID_Polyline);
		ipLine3->putref_SpatialReference( ipSpRef );
		((IPointCollectionPtr)ipLine3)->SetWKSPoints( 3, p3 );
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipLine, ipLine3 ) );
		VARIANT_BOOL vbEquals = VARIANT_TRUE;
		((IRelationalOperatorPtr)ipLine)->Equals( ipLine3, &vbEquals );
		CPPUNIT_ASSERT( vbEquals );
	}
	// ポリゴン同士の比較でのテスト
	void testAheEquals_Polygon()
	{
		// 同形状
		CPPUNIT_ASSERT( AheIsEqual( ipPolygon, AheCreateClone( ipPolygon ) ) );
		// 異形状
		IPolygonPtr ipgon(CLSID_Polygon);
		ipgon->putref_SpatialReference( ipSpRef );
		WKSPoint p[3] = { {10,11}, {12,13}, {14,15} };
		((IPointCollectionPtr)ipgon)->SetWKSPoints( 3, p );
		ipgon->Close();
		ipgon->SimplifyPreserveFromTo();
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPolygon, ipgon ) );
		// 同形状だが始点が異なるもの
		WKSPoint p1[3] = { {1,1}, {0,0}, {0,1} };
		ipgon->SetEmpty();
		((IPointCollectionPtr)ipgon)->SetWKSPoints( 3, p1 );
		ipgon->Close();
		ipgon->SimplifyPreserveFromTo();
		CPPUNIT_ASSERT( AheIsEqual( ipPolygon, ipgon ) );
		// 面積、構成点数が一緒だが異なるもの
		WKSPoint p2[3] = { {0,0}, {1,0}, {1,1} };
		ipgon->SetEmpty();
		((IPointCollectionPtr)ipgon)->SetWKSPoints( 3, p2 );
		ipgon->Close();
		ipgon->SimplifyPreserveFromTo();
		CPPUNIT_ASSERT( FALSE == AheIsEqual( ipPolygon, ipgon ) );
		// ドーナツポリゴンで外側、内側両方とも開始点が異なるもの
		WKSPoint ppppp[10] = { {0,3}, {3,3}, {3,0}, {0,0}, {0,3}, {2,1}, {2,2}, {1,2}, {1,1}, {2,1} };
		ipgon->SetEmpty();
		((IPointCollectionPtr)ipgon)->SetWKSPoints( 10, ppppp );
		ipgon->SimplifyPreserveFromTo();
		IPolygonPtr ipgon3(CLSID_Polygon);
		ipgon->putref_SpatialReference( ipSpRef );
		WKSPoint p3[10] = { {0,0}, {0,3}, {3,3}, {3,0}, {0,0}, {1,1}, {2,1}, {2,2}, {1,2}, {1,1} };
		((IPointCollectionPtr)ipgon3)->SetWKSPoints( 10, p3 );
		ipgon3->SimplifyPreserveFromTo();
		CPPUNIT_ASSERT( AheIsEqual( ipgon, ipgon3 ) );
	}
private:
	IPointPtr ipPoint;
	IPolylinePtr ipPolyline;
	IPolygonPtr ipPolygon;
	ISpatialReferencePtr ipSpRef;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CTestAheEquals);

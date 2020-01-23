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
#include "GeomProc.h"

CGeomProc::CGeomProc()
{}

CGeomProc::~CGeomProc()
{}

bool CGeomProc::getGeom2PointVec( const IGeometryPtr ipGeometry, std::vector< WKSPoint >& rPointVec )
{
	esriGeometryType aGeomType;
	if( FAILED(ipGeometry->get_GeometryType( &aGeomType ) ) ){
		return false;
	}

	if( esriGeometryPolyline != aGeomType ){
		return false;
	}

	IGeometryCollectionPtr ipGeomCollection( ipGeometry );
	long cPartsCount = 0;
	if( ipGeomCollection->get_GeometryCount( &cPartsCount ) ){
		return false;
	}
	
	if( cPartsCount != 1 )
		return false;

	IGeometryPtr ipSubGeom;
	if( FAILED( ipGeomCollection->get_Geometry( 0, &ipSubGeom ) ) )
		return false;

	IPointCollectionPtr ipPointCollection( ipSubGeom );

	long nPointCount;

	if( FAILED( ipPointCollection->get_PointCount( &nPointCount ) ) )
		return false;

	for( int i = 0; i < nPointCount; i++ ){
		IPointPtr ipPoint;
		if( FAILED( ipPointCollection->get_Point( i, &ipPoint ) ) )
			return false;

		WKSPoint aWKSPoint;

		double x,y;
		if( FAILED( ipPoint->get_X( &x ) ) || FAILED( ipPoint->get_Y( &y ) ) )
			return false;

		aWKSPoint.X = x;
		aWKSPoint.Y = y;

		rPointVec.push_back( aWKSPoint );
	}

	return true;
}

bool CGeomProc::getPoint( const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint )
{
	esriGeometryType aGeomType;
	if( FAILED(ipGeometry->get_GeometryType( &aGeomType ) ) ){
		return false;
	}

	if( esriGeometryPoint != aGeomType ){
		return false;
	}

	IPointPtr ipPoint( ipGeometry );

	if( FAILED( ipPoint->get_X( &(rWKSPoint.X) ) ) || FAILED( ipPoint->get_Y( &(rWKSPoint.Y) ) ) )
		return false;

	return true;
}

bool CGeomProc::judgePointSame( WKSPoint cWKSPoint1, WKSPoint cWKSPoint2 )
{
	return ( cWKSPoint1.X == cWKSPoint2.X && cWKSPoint1.Y == cWKSPoint2.Y );
}

bool CGeomProc::judgePointVecSame( std::vector<WKSPoint>& rWKSPointVec1, std::vector<WKSPoint>& rWKSPointVec2 )
{
	if( rWKSPointVec1.size() != rWKSPointVec2.size() )
		return false;

	for( unsigned int i=0; i < rWKSPointVec1.size(); i++ ){
		if( !judgePointSame( rWKSPointVec1[i], rWKSPointVec2[i] ) )
			return false;
	}

	return true;
}

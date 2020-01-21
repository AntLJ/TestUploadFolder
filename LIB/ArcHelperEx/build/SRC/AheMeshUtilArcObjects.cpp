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
#include <ArcHelperEx/AheMeshUtilArcObjects.h>
#include <ArcHelperEx/AheMeshUtil.h>


// メッシュコードから矩形に変換する
IEnvelopePtr AheMESH2ENV( unsigned int lMeshCode, ISpatialReference* ipSpRef )
{
	IEnvelopePtr ipEnv( CLSID_Envelope );
	double dMinLon = 0.0, dMinLat = 0.0, dMaxLon = 0.0, dMaxLat = 0.0;
	
	AheMESH2ENV( lMeshCode, dMinLon, dMinLat, dMaxLon, dMaxLat );
	ipEnv->PutCoords( dMinLon, dMinLat, dMaxLon, dMaxLat );
	if( ipSpRef ) 
		ipEnv->putref_SpatialReference( ipSpRef );

	return ipEnv;
}

// メッシュコードからポリゴンに変換する
IGeometryPtr AheMESH2POLY( unsigned int lMeshCode, ISpatialReference* ipSpRef )
{
	IPointCollectionPtr ipPointCol( CLSID_Polygon );
	double dMinLon = 0.0, dMinLat = 0.0, dMaxLon = 0.0, dMaxLat = 0.0;
	std::vector<WKSPoint> points(5);

	AheMESH2ENV( lMeshCode, dMinLon, dMinLat, dMaxLon, dMaxLat );
	points[0].X = dMinLon; points[0].Y = dMinLat;
	points[1].X = dMinLon; points[1].Y = dMaxLat;
	points[2].X = dMaxLon; points[2].Y = dMaxLat;
	points[3].X = dMaxLon; points[3].Y = dMinLat;
	points[4].X = dMinLon; points[4].Y = dMinLat;
	ipPointCol->SetWKSPoints( 5, &(points.at(0)) );
	if( ipSpRef ) 
		((IGeometryPtr)ipPointCol)->putref_SpatialReference( ipSpRef );

	return ipPointCol;
}

// Pointからメッシュコード＋正規化XYに変換する
unsigned int AhePoint2MESH( IPoint* ipPoint, int level, int& x, int& y, int no_mill/* = 0*/ )
{
	if( ! ipPoint )
		return -1;

	double dx = 0, dy = 0;
	ipPoint->QueryCoords( &dx, &dy );

	return AheLL2MESH( dx, dy, level, x, y, no_mill );
}

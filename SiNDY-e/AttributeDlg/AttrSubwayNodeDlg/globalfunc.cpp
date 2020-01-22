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
#include "globalfunc.h"
#include "AheGlobals.h"

using namespace sindy::schema;

namespace{
IFeatureCursorPtr Neighboringsearch( IPointPtr ipPoint, double dRange, IFeatureClassPtr ipPointFC )
{
	ITopologicalOperatorPtr ipTopo( ipPoint );
	if( !(ipTopo && ipPointFC) )
		return false;

	// m → 度変換
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( dRange, esriMeters, esriDecimalDegrees, &dConvRange );

	// バッファリング形状で空間検索
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );
	return AheSelectByShape( ipGeoBuffer, ipPointFC, esriSpatialRelIntersects );
}
} // namespace

std::list<long> GetNeighboringStationPoint( IPointPtr ipPoint, double dRange, IFeatureClassPtr ipPointFC )
{
	std::list<long> ids;
	IFeatureCursorPtr ipFeatureCursor = Neighboringsearch( ipPoint, dRange, ipPointFC );
	if( ipFeatureCursor )
	{
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature(&ipFeature) == S_OK )
		{
			LONG lOID = 0;
			ipFeature->get_OID( &lOID );
			ids.push_back( lOID );
		}
	}

	return ids;
}

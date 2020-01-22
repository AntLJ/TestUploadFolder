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
#include "FeatureObjects.h"

namespace
{
	crd_cnv g_crd;
}

BGBox GeoClass::GetBox(double range) const
{
	IEnvelopePtr ipEnvelope;
	geometry->get_Envelope(&ipEnvelope);
	WKSPoint minPoint = {}, maxPoint = {};
	if(range == 0)
	{
		minPoint.X = ipEnvelope->GetXMin(); minPoint.Y = ipEnvelope->GetYMin();
		maxPoint.X = ipEnvelope->GetXMax(); maxPoint.Y = ipEnvelope->GetYMax();
	}
	else
	{
		g_crd.GetLLPt(ipEnvelope->GetXMin(), ipEnvelope->GetYMin(), -range, -range, &(minPoint.X), &(minPoint.Y));
		g_crd.GetLLPt(ipEnvelope->GetXMax(), ipEnvelope->GetYMax(),  range,  range, &(maxPoint.X), &(maxPoint.Y));
	}
	return BGBox(BGPoint(minPoint.X, minPoint.Y), BGPoint(maxPoint.X, maxPoint.Y));
}

bool GeoClass::IsOverRap( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel(geometry.m_T);
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint(ipGeometry, &isDisjoint);
	if(isDisjoint == VARIANT_TRUE)
		return false;

	VARIANT_BOOL isTouches = VARIANT_FALSE;
	ipRel->Touches(ipGeometry, &isTouches);
	if(isTouches == VARIANT_TRUE)
		return false;

	return true;
}

bool GeoClass::IsEqual(IGeometryPtr ipGeometry) const
{
	IRelationalOperatorPtr ipRel(geometry.m_T);
	VARIANT_BOOL isEqual = VARIANT_FALSE;
	ipRel->Equals(ipGeometry, &isEqual);
	if(isEqual == VARIANT_TRUE)
		return true;

	return false;
}

bool GeoClass::IsDisjoint( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel( geometry.m_T );
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint(ipGeometry, &isDisjoint);
	if( isDisjoint == VARIANT_TRUE)
		return true;

	return false;
}

bool GeoClass::IsCross( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel( geometry.m_T );
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint(ipGeometry, &isDisjoint);
	if( isDisjoint == VARIANT_TRUE)
		return false;
	VARIANT_BOOL isCross = VARIANT_FALSE;
	ipRel->Crosses(ipGeometry, &isCross );
	if( isCross == VARIANT_TRUE)
		return true;

	return false;
}

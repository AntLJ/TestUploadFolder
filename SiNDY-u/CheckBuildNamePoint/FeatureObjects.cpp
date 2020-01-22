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

// ジオメトリからR-Tree用のボックスを得る
BGBox GeoClass::GetBox(double range) const
{
	IEnvelopePtr ipEnvelope;
	m_geometry->get_Envelope(&ipEnvelope);
	WKSPoint minPoint = {}, maxPoint = {};
	if(range == 0)
		ipEnvelope->QueryCoords( &(minPoint.X), &(minPoint.Y), &(maxPoint.X), &(maxPoint.Y) );
	else{
		g_crd.GetLLPt(ipEnvelope->GetXMin(), ipEnvelope->GetYMin(), -range, -range, &(minPoint.X), &(minPoint.Y));
		g_crd.GetLLPt(ipEnvelope->GetXMax(), ipEnvelope->GetYMax(),  range,  range, &(maxPoint.X), &(maxPoint.Y));
	}
	return BGBox(BGPoint(minPoint.X, minPoint.Y), BGPoint(maxPoint.X, maxPoint.Y));
}

// 指定ジオメトリとの重なりを判定する
bool GeoClass::IsDisjoint( IGeometryPtr ipGeometry ) const
{
	IRelationalOperatorPtr ipRel( m_geometry );
	VARIANT_BOOL isDisjoint = VARIANT_FALSE;
	ipRel->Disjoint( ipGeometry, &isDisjoint );
	if( isDisjoint == VARIANT_TRUE )
		return true;

	return false;
}

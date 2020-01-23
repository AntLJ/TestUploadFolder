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

// SindyPointClass.cpp: SindyPointClass クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyPointClass.h"


SindyPointClass::SindyPointClass(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyFeatureClass(cDBConnection, cName),
mPointList()
{

}

SindyPointClass::~SindyPointClass()
{

}

HRESULT SindyPointClass::LoadByPolygon(IPolygonPtr ipPolygon)
{
	HRESULT hr;
	const char *func = "SindyPointClass::LoadByPolygon";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByGeometry((IFeatureClassPtr)ipDataset, (IGeometryPtr)ipPolygon, esriSpatialRelIntersects, &ipFeatureCursor)))
		return hr;

	IFeaturePtr ipFeature = NULL;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
	{
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry)))
			return GDBERROR(hr, func);

		bool rel;
		if (FAILED(hr = GDBCheckRelation(ipGeometry, ipPolygon, &rel)))
			return hr;
		if (! rel)
			continue;

		long aOID;
		if (FAILED(hr = ipFeature->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipFeature));

		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);
		mPointList.insert(GDBPNTSET(ipPoint, ipFeature));
	}

	return S_OK;
}

HRESULT SindyPointClass::SearchByPoint(IPointPtr ipPoint, GDBPNTRANGE *cRange)
{
	*cRange = mPointList.equal_range(ipPoint);
	return S_OK;
}

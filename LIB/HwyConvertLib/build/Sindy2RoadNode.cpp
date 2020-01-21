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

// Sindy2RoadNode.cpp: Sindy2RoadNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Sindy2RoadNode.h"

Sindy2RoadNode::Sindy2RoadNode(SindyDBConnection *cDBConnection) :
SindyRoadNode(cDBConnection)
{

}

Sindy2RoadNode::~Sindy2RoadNode()
{

}

HRESULT Sindy2RoadNode::LoadByPoints(SindyPointClass *cPointClass)
{
	HRESULT hr;
	const char *func = "Sindy2RoadNode::LoadByPoints";

#ifdef	_STOP	// Multipointで検索すると遅いのでやめた(2003/10/05)

	// MultiPointの生成
	IPointCollectionPtr ipPointCollection(CLSID_Multipoint);
	ATLASSERT(ipPointCollection != NULL);
	for (GDBOBJITER aPoint = cPointClass->begin(); aPoint != cPointClass->end(); aPoint++)
	{
		IFeaturePtr ipFeature = aPoint->second;
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);
		if (FAILED(hr = ipPointCollection->AddPoint(ipPoint)))
			return GDBERROR(hr, func);
	}

	IMultipointPtr ipMultipoint(ipPointCollection);
	ATLASSERT(ipMultipoint != NULL);

	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByGeometry((IFeatureClassPtr)ipDataset, (IGeometryPtr)ipMultipoint, esriSpatialRelIntersects, &ipFeatureCursor))) return hr;

	IFeaturePtr ipFeature = NULL;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
	{
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry)))
			return GDBERROR(hr, func);

		long aOID;
		if (FAILED(hr = ipFeature->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipFeature));

		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);
		mPointList.insert(GDBPNTSET(ipPoint, ipFeature));
	}

#endif


#ifdef	_DEBUG
	long count = 0;
#endif
	for (GDBOBJITER aPoint = cPointClass->begin(); aPoint != cPointClass->end(); aPoint++)
	{
#ifdef	_DEBUG
		std::cerr << "進行状況：" << ++count << "/" << cPointClass->size() << "\r";
#endif
		IFeaturePtr ipHwyPnt = aPoint->second;
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipHwyPnt->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		IFeatureCursorPtr ipFeatureCursor = NULL;
		if (FAILED(hr = GDBSearchByGeometry((IFeatureClassPtr)ipDataset, ipGeometry, esriSpatialRelIntersects, &ipFeatureCursor))) return hr;
		IFeaturePtr ipFeature = NULL;
		while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
		{
			IGeometryPtr ipNodeGeom = NULL;
			if (FAILED(hr = ipFeature->get_ShapeCopy(&ipNodeGeom))) return GDBERROR(hr, func);

			long aOID;
			if (FAILED(hr = ipFeature->get_OID(&aOID))) return GDBERROR(hr, func);
			insert(GDBOBJSET(aOID, ipFeature));

			IPointPtr ipPoint(ipNodeGeom);
			ATLASSERT(ipPoint != NULL);
			mPointList.insert(GDBPNTSET(ipPoint, ipFeature));
		}
	}
#ifdef	_DEBUG
//	std::cerr << std::endl;
#endif


	return S_OK;
}

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

// SindyPolygonClass.cpp: SindyPolygonClass クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyPolygonClass.h"


SindyPolygonClass::SindyPolygonClass(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyFeatureClass(cDBConnection, cName)
{

}

SindyPolygonClass::~SindyPolygonClass()
{

}

HRESULT SindyPolygonClass::LoadByPolygon(IPolygonPtr ipPolygon)
{
	HRESULT hr;
	const char *func = "SindyPolygonClass::LoadByPolygon";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	// Arc9.3.1対応
	// ISelectionSetは使用しないように修正
	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByGeometry(((IFeatureClassPtr)ipDataset), (IGeometryPtr)ipPolygon, esriSpatialRelIntersects, &ipFeatureCursor)))
		return hr;

	IFeaturePtr ipFeature = NULL;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
	{
		long aOID;
		if (FAILED(hr = ipFeature->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipFeature));
	}

	return S_OK;
}

HRESULT SindyPolygonClass::SearchByGeometry(IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor)
{
	HRESULT hr;
	const char *func = "SindyPolygonClass::SearchByGeometry";

	ISpatialFilterPtr ipSpatialFilter = NULL;
	if (FAILED(hr = GDBCreateSpatialFilter((IFeatureClassPtr)ipDataset, ipGeometry, cSpatRel, &ipSpatialFilter)))
		return hr;

	if (FAILED(hr = ((IFeatureClassPtr)ipDataset)->Search(ipSpatialFilter, VARIANT_FALSE, (IFeatureCursor **)ipFeatureCursor)))
		return GDBERROR(hr, func);

	return S_OK;
}

struct poly_info
{
	double		value;
	IFeaturePtr	ipFeature;
};

HRESULT SindyPolygonClass::SearchPrefPoly(IGeometryPtr ipGeometry, const std::string &cKeyField, IFeature **ipFeature)
{
	HRESULT hr;
	const char *func = "SindyPolygonClass::SearchPrefPoly";

	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = SearchByGeometry(ipGeometry, esriSpatialRelIntersects, &ipFeatureCursor)))
		return hr;
	if (ipFeatureCursor == NULL)
		return S_OK;

	esriGeometryType aGeometryType;
	if (FAILED(hr = ipGeometry->get_GeometryType(&aGeometryType)))
		return GDBERROR(hr, func);

	std::map<VARIANT,poly_info,lessVariant> aList;

	IFeaturePtr ipPrefFeature = NULL;

	switch (aGeometryType)
	{
	case esriGeometryPoint:
		if (FAILED(hr = ipFeatureCursor->NextFeature(ipFeature)))
			return GDBERROR(hr, func);
		return S_OK;
	case esriGeometryPolyline:
		{
			IPolylinePtr ipPolyline(ipGeometry);
			ATLASSERT(ipPolyline != NULL);

			IFeaturePtr ipFeat = NULL;
			while (ipFeatureCursor->NextFeature(&ipFeat) == S_OK && ipFeat != NULL)
			{
				if (ipPrefFeature == NULL) ipPrefFeature = ipFeat;

				CComVariant vaKey;
				if (cKeyField.size())
				{
					if (FAILED(hr = GDBGetValue(ipFeat, cKeyField, &vaKey)))
						return hr;
				}
				else
				{
					long aOID;
					if (FAILED(hr = ipFeat->get_OID(&aOID)))
						return GDBERROR2(hr, func);
					CComVariant vaTemp(aOID);
					vaKey = vaTemp;
				}

				IGeometryPtr ipPolygon = NULL;
				if (FAILED(hr = ipFeat->get_ShapeCopy(&ipPolygon)))
					return GDBERROR(hr, func);

				IGeometryPtr ipIntersects = NULL;
				ITopologicalOperatorPtr ipTopologicalOperator(ipGeometry);
				ATLASSERT(ipTopologicalOperator != NULL);
				if (FAILED(hr = ipTopologicalOperator->Intersect(ipPolygon, esriGeometry1Dimension, &ipIntersects)))
					return GDBERROR(hr, func);

				IPolylinePtr ipCutLine(ipIntersects);
				ATLASSERT(ipCutLine != NULL);

				double len = 0.0;
				if (FAILED(hr = ipCutLine->get_Length(&len)))
					return GDBERROR(hr, func);

				poly_info aInfo;
				aInfo.value = len;
				aInfo.ipFeature = ipFeat;

				std::map<VARIANT,poly_info,lessVariant>::iterator aIter = aList.find(vaKey);
				if (aIter == aList.end()) aList.insert(std::pair<VARIANT,poly_info>(vaKey, aInfo));
				else aIter->second.value+= aInfo.value;
			}
		}
		break;
	case esriGeometryPolygon:
		{
			IPolylinePtr ipArea(ipGeometry);
			ATLASSERT(ipArea != NULL);

			IFeaturePtr ipFeat = NULL;
			while (ipFeatureCursor->NextFeature(&ipFeat) == S_OK && ipFeat != NULL)
			{
				if (ipPrefFeature != NULL) ipPrefFeature = ipFeat;

				CComVariant vaKey;
				if (cKeyField.size())
				{
					if (FAILED(hr = GDBGetValue(ipFeat, cKeyField, &vaKey)))
						return hr;
				}
				else
				{
					long aOID;
					if (FAILED(hr = ipFeat->get_OID(&aOID)))
						return GDBERROR2(hr, func);
					CComVariant vaTemp(aOID);
					vaKey = vaTemp;
				}

				IGeometryPtr ipPolygon = NULL;
				if (FAILED(hr = ipFeat->get_ShapeCopy(&ipPolygon)))
					return GDBERROR(hr, func);

				IGeometryPtr ipIntersects = NULL;
				ITopologicalOperatorPtr ipTopologicalOperator(ipGeometry);
				ATLASSERT(ipTopologicalOperator != NULL);
				if (FAILED(hr = ipTopologicalOperator->Intersect(ipPolygon, esriGeometry1Dimension, &ipIntersects)))
					return GDBERROR(hr, func);

				IAreaPtr ipCutArea(ipIntersects);
				ATLASSERT(ipCutArea != NULL);

				double area = 0.0;
				if (FAILED(hr = ipCutArea->get_Area(&area)))
					return GDBERROR(hr, func);

				poly_info aInfo;
				aInfo.value = area;
				aInfo.ipFeature = ipFeat;

				std::map<VARIANT,poly_info,lessVariant>::iterator aIter = aList.find(vaKey);
				if (aIter == aList.end()) aList.insert(std::pair<VARIANT,poly_info>(vaKey, aInfo));
				else aIter->second.value += aInfo.value;
			}
		}
		break;
	default:
		return GDBERROR2(E_NOINTERFACE, func);
	}

	double max = 0.0;
	for (std::map<VARIANT,poly_info,lessVariant>::iterator aIter = aList.begin(); aIter != aList.end(); aIter++)
	{
		if (max < aIter->second.value)
		{
			ipPrefFeature = aIter->second.ipFeature;
			max = aIter->second.value;
		}
	}

	(*ipFeature) = ipPrefFeature;
	(*ipFeature)->AddRef();

	return S_OK;
}

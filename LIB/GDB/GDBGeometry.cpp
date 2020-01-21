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

/**
 *	GDBGeometry.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		新規作成
 */

#include "GDB.h"
#include "ZMapInfo.h"

static HRESULT GetMeshCodeOffset(long cMesh1, long cMesh2, long *cX, long *cY)
{
	const char *func = "GetMeshCodeOffset";

	*cX = *cY = 0;

	switch (strlen((boost::lexical_cast<std::string>(cMesh2).c_str())))
	{
	case 4:
		{
			long aMY1 = cMesh1 / 100;
			long aMX1 = cMesh1 - aMY1;
			long aMY2 = cMesh2 / 100;
			long aMX2 = cMesh2 - aMY2;
			*cX = aMX2 - aMX1;
			*cY = aMY2 - aMY1;
		}
		break;
	case 6:
		{
			long aMY1 = cMesh1 / 10000;
			long aMX1 = (cMesh1 / 100) - (aMY1 * 100);
			long aMY2 = cMesh2 / 10000;
			long aMX2 = (cMesh2 / 100) - (aMY2 * 100);

			long aBY1 = (cMesh1 / 10) - ((cMesh1 / 100) * 10);
			long aBX1 = cMesh1 - ((cMesh1 / 10) * 10);
			long aBY2 = (cMesh2 / 10) - ((cMesh2 / 100) * 10);
			long aBX2 = cMesh2 - ((cMesh2 / 10) * 10);

			*cX = ((aMX2 - aMX1) * 8) + (aBX2 - aBX1);
			*cY = ((aMY2 - aMY1) * 8) + (aBY2 - aBY1);
		}
		break;
	case 8:
		{
			long aMY1 = cMesh1 / 1000000;
			long aMX1 = (cMesh1 / 10000) - (aMY1 * 10000);
			long aMY2 = cMesh2 / 1000000;
			long aMX2 = (cMesh2 / 10000) - (aMY2 * 10000);

			long aBY1 = (cMesh1 / 1000) - ((cMesh1 / 10000) * 1000);
			long aBX1 = (cMesh1 / 100) - ((cMesh1 / 1000) * 1000);
			long aBY2 = (cMesh2 / 1000) - ((cMesh2 / 10000) * 1000);
			long aBX2 = (cMesh2 / 100) - ((cMesh2 / 1000) * 1000);

			long aCY1 = (cMesh1 / 10) - ((cMesh1 / 100) * 10);
			long aCX1 = cMesh1 - ((cMesh1 / 10) * 10);
			long aCY2 = (cMesh2 / 10) - ((cMesh2 / 100) * 10);
			long aCX2 = cMesh2 - ((cMesh2 / 10) * 10);

			*cX = ((aMX2 - aMX1) * 8 * 8) + ((aBX2 - aBX1) * 8) + (aCX2 - aCX1);
			*cY = ((aMY2 - aMY1) * 8 * 8) + ((aBY2 - aBY1) * 8) + (aCY2 - aCY1);
		}
		break;
	default:
		return GDBERROR2(E_NOINTERFACE, func);
	}

	return S_OK;
}


HRESULT GDBGetBaseMeshNormalizeCoord(IPointPtr ipPoint, long *cMesh, long *cX, long *cY, ISpatialReferencePtr ipSpatRef)
{
	HRESULT hr;
	const char *func = "GDBGetBaseMeshNormalizeCoord";

	if (ipSpatRef != 0)
	{
		if (FAILED(hr = ipPoint->putref_SpatialReference(ipSpatRef)))
			return GDBERROR(hr, func);
	}

	double longi, latit;
	if (FAILED(hr = ipPoint->get_X(&longi)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipPoint->get_Y(&latit)))
		return GDBERROR(hr, func);

	ZMeshPos aMeshPos(longi, latit);

	*cX = aMeshPos.eX;
	*cY = aMeshPos.eY;

	if (*cMesh != 0 && *cMesh != aMeshPos.eMesh)
	{
		long xoff = 0, yoff = 0;
		if (FAILED(hr = GetMeshCodeOffset(*cMesh, aMeshPos.eMesh, &xoff, &yoff)))
			return hr;
		(*cX) += (xoff * 1000000);
		(*cY) += (yoff * 1000000);
	}
	else
	{
		*cMesh = aMeshPos.eMesh;
	}

	return S_OK;
}

HRESULT GDBGetDistance(IPointPtr ipPoint1, IPointPtr ipPoint2, double *cDistance)
{
	HRESULT hr;
	const char *func = "GDBGetDistance";

	IProjectedCoordinateSystemPtr ipPrjCoordSys = NULL;
	ISpatialReferenceFactory2Ptr ipSpRefFactory(CLSID_SpatialReferenceEnvironment);
	ATLASSERT(ipSpRefFactory != NULL);

	if (FAILED(hr = ipSpRefFactory->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan10, &ipPrjCoordSys)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipPoint1->Project(ipPrjCoordSys)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipPoint2->Project(ipPrjCoordSys)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ((IProximityOperatorPtr)ipPoint1)->ReturnDistance(ipPoint2, cDistance)))
		return GDBERROR(hr, func);

	return S_OK;

}

/**
 * @brief ポイントとポリゴンの関係チェック
 * @note	GDBCheckRelation(IGeometryPtr, IPolygonPtr, bool)の中だけで使われる関数。
 *			ipPointが「ipPolygonの構成点上にある(Contains)か、または辺上にある(Touches)」か否かを判定する
 * @param[in]	ipPoint		対象ポイント
 * @param[in]	ipPolygon	対象ポリゴン
 * @param[out]	cContains	判定結果
 * @return	エラー情報（常にS_OK）
 */
static HRESULT GDBCheckRelation(IPointPtr ipPoint, IPolygonPtr ipPolygon, bool *cContains)
{
	const char *func = "GDBCheckRelation";

	IRelationalOperatorPtr ipRelationalOperator(ipPolygon);
	ATLASSERT(ipRelationalOperator != NULL);

	*cContains = true;

	VARIANT_BOOL rel;
	if (ipRelationalOperator->Contains(ipPoint, &rel) != S_OK || rel == VARIANT_FALSE)
	{
		if (ipRelationalOperator->Touches(ipPoint, &rel) != S_OK || rel == VARIANT_FALSE)
			*cContains = false;
	}

	return S_OK;
}

HRESULT GDBCheckRelation(IGeometryPtr ipGeometry, IPolygonPtr ipPolygon, bool *cContains)
{
	HRESULT hr;
	const char *func = "GDBCheckRelation";

	esriGeometryType aType;
	if (FAILED(hr = ipGeometry->get_GeometryType(&aType)))
		return GDBERROR(hr, func);

	switch (aType)
	{
	case esriGeometryPoint:
		if (FAILED(hr = GDBCheckRelation((IPointPtr)ipGeometry, ipPolygon, cContains)))
			return hr;
		break;
	case esriGeometryPolyline:
		{
			IRelationalOperatorPtr ipRelationalOperator(ipPolygon);
			ATLASSERT(ipRelationalOperator != NULL);
			
			VARIANT_BOOL rel;
			if( ipRelationalOperator->Contains(ipGeometry,&rel) != S_OK || rel == VARIANT_FALSE)
			{
				*cContains = false;
			}
			else{
				*cContains =true;
			}

			return S_OK;
			
		}

	case esriGeometryPolygon:
		{
			IPointCollectionPtr ipPointCollection(ipGeometry);
			ATLASSERT(ipPointCollection != NULL);

			long aCount;
			if (FAILED(hr = ipPointCollection->get_PointCount(&aCount)))
				return GDBERROR(hr, func);
			for (long i = 0; i < aCount; i++)
			{
				IPointPtr ipPoint = NULL;
				if (FAILED(hr = ipPointCollection->get_Point(i, &ipPoint)))
					return GDBERROR(hr, func);
				if (FAILED(hr = GDBCheckRelation(ipPoint, ipPolygon, cContains)))
					return hr;
				if (! *cContains) return S_OK;
			}
			*cContains = true;
		}
		break;
	default:
		return GDBERROR2(E_NOINTERFACE, func);
	}

	return S_OK;
}

HRESULT GDBGetPolygonEnvelope(IPolygonPtr ipPolygon, IEnvelope **ipEnvelope)
{
	HRESULT hr;
	const char *func = "GDBGetPolygonEnvelope";

	IEnvelopePtr ipEnv(CLSID_Envelope);
	ATLASSERT(ipEnv != NULL);

	IPointCollectionPtr ipPointCollection(ipPolygon);
	ATLASSERT(ipPointCollection != NULL);

	long aPointCount;
	if (FAILED(hr = ipPointCollection->get_PointCount(&aPointCount)))
		return GDBERROR(hr, func);

	double xmin, xmax, ymin, ymax;
	for (long i = 0; i < aPointCount; i++)
	{
		IPointPtr ipPoint = NULL;
		if (FAILED(hr = ipPointCollection->get_Point(i, &ipPoint)))
			return GDBERROR(hr, func);

		double aX, aY;
		if (FAILED(hr = ipPoint->get_X(&aX)))
			return GDBERROR(hr, func);
		if (FAILED(hr = ipPoint->get_Y(&aY)))
			return GDBERROR(hr, func);

		if (! i || xmin > aX) xmin = aX;
		if (! i || xmax < aX) xmax = aX;
		if (! i || ymin > aY) ymin = aY;
		if (! i || ymax < aY) ymax = aY;
	}

	if (FAILED(hr = ipEnv->put_XMin(xmin))) return GDBERROR(hr, func);
	if (FAILED(hr = ipEnv->put_XMax(xmax))) return GDBERROR(hr, func);
	if (FAILED(hr = ipEnv->put_YMin(ymin))) return GDBERROR(hr, func);
	if (FAILED(hr = ipEnv->put_YMax(ymax))) return GDBERROR(hr, func);

	(*ipEnvelope) = ipEnv;
	(*ipEnvelope)->AddRef();

	return S_OK;
}

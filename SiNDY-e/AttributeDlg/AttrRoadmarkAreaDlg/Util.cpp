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
#include "Util.h"

// ArcHelperEx
//#include <GlobalFunctions.h>
//#include <AheGlobalsCoord.h>
//#include <AheGeometryOp.h>
#include <AheGeometryConvert.h>
//#include <AheGlobalsMisc.h>

namespace
{
	/**
	* @brief  ポリラインの構成点群を取得する
	* @param  [in]  polyline  ポリライン
	* @return ポリラインの構成点群
	*/
	std::vector<IPointPtr> getPointList(const IPolylinePtr& polyline)
	{
		std::vector<IPointPtr> retPoints;
		IPointCollectionPtr pointCol(ToMultipoint(polyline));

		long count = 0;
		pointCol->get_PointCount(&count);
		for (long i = 0; i < count; ++i)
		{
			IPointPtr point;
			pointCol->get_Point(i, &point);

			retPoints.push_back(point);
		}

		return retPoints;
	}
}

std::vector<IPolylinePtr> util::splitPolyline(IPolylinePtr polyline)
{
	ISpatialReferencePtr sr;
	polyline->get_SpatialReference(&sr);

	// 分割後のポリライン群格納用
	std::vector<IPolylinePtr> retPolylines;

	ISegmentCollectionPtr ipSegCol(polyline);
	long lSegCount(-1L);
	ipSegCol->get_SegmentCount(&lSegCount);
	for (long i = 0; i < lSegCount; ++i)
	{
		_ISegmentPtr partSeg;
		ipSegCol->get_Segment(i, &partSeg);

		ISegmentCollectionPtr tmpSegGeo(CLSID__Path);
		((IGeometryPtr)tmpSegGeo)->putref_SpatialReference(sr);
		tmpSegGeo->AddSegment(partSeg);

		IGeometryCollectionPtr tmpGeoCol(CLSID_Polyline);
		((IGeometryPtr)tmpGeoCol)->putref_SpatialReference(sr);
		tmpGeoCol->AddGeometry(IGeometryPtr(tmpSegGeo));

		IPolylinePtr tmpPolyline(tmpGeoCol);
		tmpPolyline->putref_SpatialReference(sr);

		retPolylines.push_back(tmpPolyline);
	}

	return retPolylines;
}

bool util::getNearestPoint(IPolygonPtr polygon, IPolylinePtr polyline, IPointPtr & nearest_point1, IPointPtr & nearest_point2)
{
	if (!polygon || !polyline)
		return false;

	IPolylinePtr polygonPolyline(ToPolyline(polygon));
	auto polylines = splitPolyline(polygonPolyline);
	double nearestDist = DBL_MAX;
	for (auto line : polylines)
	{
		IPointPtr retPoint1, retPoint2;
		if (!getNearestPointBetweenPolyline(line, polyline, retPoint1, retPoint2))
			return false;

		double dist = DBL_MAX;
		if (FAILED(((IProximityOperatorPtr)retPoint1)->ReturnDistance(retPoint2, &dist)))
			return false;

		if (dist < nearestDist) {
			nearestDist = dist;
			nearest_point1 = retPoint1;
			nearest_point2 = retPoint2;
		}
	}

	if (nearestDist == DBL_MAX)
		return false;

	if (!nearest_point1 || !nearest_point2)
		return false;

	return true;
}

bool util::getNearestPointBetweenPolyline(IPolylinePtr polyline1, IPolylinePtr polyline2, IPointPtr & nearest_point1, IPointPtr & nearest_point2)
{
	// ベース側、ターゲット側のポリラインの構成点群を取得
	std::vector<IPointPtr> basePoints = getPointList(polyline1);
	std::vector<IPointPtr> targetPoints = getPointList(polyline2);

	double nearest_dist = DBL_MAX;
	for (auto pt_iter = basePoints.begin(); pt_iter != basePoints.end(); ++pt_iter)
	{
		IPointPtr point = *pt_iter;

		// 最近傍点とそこまでの距離を取得
		IPointPtr tmpNearestPoint;
		double dist = DBL_MAX;
		if (!getNearestPointAndDistance(point, polyline2, tmpNearestPoint, dist))
			return false;

		// より近い最近傍点を取得
		if (dist < nearest_dist) {
			nearest_dist = dist;
			nearest_point1 = point;
			nearest_point2 = tmpNearestPoint;
		}
	}

	for (auto pt_iter = targetPoints.begin(); pt_iter != targetPoints.end(); ++pt_iter)
	{
		IPointPtr point = *pt_iter;

		// 最近傍点とそこまでの距離を取得
		IPointPtr tmpNearestPoint;
		double dist(DBL_MAX);
		if (!getNearestPointAndDistance(point, polyline1, tmpNearestPoint, dist))
			return false;

		// より近いターゲット側の構成点を取得
		if (dist < nearest_dist) {
			nearest_dist = dist;
			nearest_point1 = tmpNearestPoint;
			nearest_point2 = point;
		}
	}

	if (!nearest_point1 || !nearest_point2)
		return false;

	return true;
}

bool util::getNearestPointAndDistance(IPointPtr ipBasePoint, IPolylinePtr ipTargetPolyline, IPointPtr & ipNearestPoint, double & distance)
{
	// 最近傍点を取得
	if (FAILED(((IProximityOperatorPtr)ipTargetPolyline)->ReturnNearestPoint(ipBasePoint, esriSegmentExtension::esriNoExtension, &ipNearestPoint)))
	{
		_ASSERTE(FALSE);
		return false;
	}

	// 最近傍点への距離を取得
	if (FAILED(((IProximityOperatorPtr)ipTargetPolyline)->ReturnDistance(ipBasePoint, &distance)))
	{
		_ASSERTE(FALSE);
		return false;
	}

	return true;
}

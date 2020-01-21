#include "stdafx.h"
#include "AheRepPoint.h"
#include "AheGlobals.h"

// プロトタイプ宣言
rpc::Point GetRepPoint(const IGeometryPtr& ipGeometry, rpc::CalcMode mode);
rpc::Polygon ToPolygon(const IGeometryPtr& ipGeometry);
rpc::Ring ToRing(const IGeometryPtr& ipRingGeometry);

IPointPtr AheGetRepPoint(const IGeometryPtr & ipGeometry, rpc::CalcMode mode)
{
	IPointPtr ipPoint(CLSID_Point);
	ipPoint->putref_SpatialReference(AheGetSpatialReference(ipGeometry));

	rpc::Point point = GetRepPoint(ipGeometry, mode);
	ipPoint->PutCoords(point.X, point.Y);
	return ipPoint;
}

WKSPoint AheGetRepWKSPoint(const IGeometryPtr & ipGeometry, rpc::CalcMode mode)
{
	rpc::Point point = GetRepPoint(ipGeometry, mode);
	WKSPoint wksPoint = { point.X, point.Y };
	return wksPoint;
}

// 代表点計算本編
rpc::Point GetRepPoint(const IGeometryPtr& ipGeometry, rpc::CalcMode mode)
{
	RepPointCalculator repCalculator;

	esriGeometryType geometryType = esriGeometryNull;
	ipGeometry->get_GeometryType(&geometryType);

	rpc::Point repPoint;
	switch (geometryType)
	{
	case esriGeometryPolygon:
	{
		IGeometryBagPtr ipPolygonBag;
		((IPolygon4Ptr)ipGeometry)->get_ConnectedComponentBag(&ipPolygonBag);
		IGeometryCollectionPtr ipPolygonCollection(ipPolygonBag);
		long polygonConut = 0;
		ipPolygonCollection->get_GeometryCount(&polygonConut);

		rpc::MultiPolygon multiPolygon;
		for (long i = 0; i < polygonConut; ++i)
		{
			IGeometryPtr ipPolygonGeometry;
			ipPolygonCollection->get_Geometry(i, &ipPolygonGeometry);
			multiPolygon.m_polygons.push_back(ToPolygon(ipPolygonGeometry));
		}

		if (1 == multiPolygon.Count())
		{
			repPoint = repCalculator.CalculateRepPoint(multiPolygon.m_polygons[0], mode);
		}
		else
		{
			repPoint = repCalculator.CalculateRepPoint(multiPolygon, mode);
		}

		break;
	}
	case esriGeometryPolyline:
	{
		IGeometryCollectionPtr ipCollection(ipGeometry);
		long lineCount = 0;
		ipCollection->get_GeometryCount(&lineCount);

		rpc::MultiPolyline multiPolyline;
		for (long j = 0; j < lineCount; ++j)
		{
			IGeometryPtr ipGeo;
			ipCollection->get_Geometry(j, &ipGeo);

			IPointCollectionPtr ipPointCol(ipGeo);
			long ptCount = 0;
			ipPointCol->get_PointCount(&ptCount);

			std::vector<WKSPoint> wksPoints(ptCount);
			ipPointCol->QueryWKSPoints(0, ptCount, &wksPoints[0]);

			rpc::Polyline polyline;
			for (auto& wksPt : wksPoints)
			{
				polyline.m_points.emplace_back(wksPt.X, wksPt.Y);
			}
			multiPolyline.m_lines.push_back(polyline);
		}

		if (1 == multiPolyline.Count())
		{
			repPoint = repCalculator.CalculateRepPoint(multiPolyline.m_lines[0], mode);
		}
		else
		{
			repPoint = repCalculator.CalculateRepPoint(multiPolyline, mode);
		}

		break;
	}
	case esriGeometryPoint:
	{
		IPointPtr ipPoint(ipGeometry);
		double x = 0.0, y = 0.0;
		ipPoint->QueryCoords(&x, &y);

		rpc::Point point(x, y);
		repPoint = repCalculator.CalculateRepPoint(point, mode);
		break;
	}
	case esriGeometryMultipoint:
	{
		rpc::MultiPoint multiPoint;
		IPointCollectionPtr pointCollection(ipGeometry);

		long count = 0;
		pointCollection->get_PointCount(&count);
		std::vector<WKSPoint> wksPoints(count);
		pointCollection->QueryWKSPoints(0, count, &wksPoints[0]);
		for (auto& wksPt : wksPoints)
		{
			multiPoint.m_points.emplace_back(wksPt.X, wksPt.Y);
		}
		repPoint = repCalculator.CalculateRepPoint(multiPoint, mode);
		break;
	}
	default:
		break;
	}

	return repPoint;
}

rpc::Polygon ToPolygon(const IGeometryPtr& ipGeometry)
{
	rpc::Polygon polygon;

	// Exterior
	IGeometryBagPtr ipExteriorRingBag;
	((IPolygon4Ptr)ipGeometry)->get_ExteriorRingBag(&ipExteriorRingBag);
	IGeometryCollectionPtr ipExteriorCollection(ipExteriorRingBag);

	if (!ipExteriorCollection)
		return rpc::Polygon();

	IGeometryPtr ipExteriorGeometry;
	ipExteriorCollection->get_Geometry(0, &ipExteriorGeometry);

	polygon.m_outerRing = ToRing(ipExteriorGeometry);

	// Interior
	IGeometryBagPtr ipInteriorRingBag;
	((IPolygon4Ptr)ipGeometry)->get_InteriorRingBag((IRingPtr)ipExteriorGeometry, &ipInteriorRingBag);
	IGeometryCollectionPtr ipInteriorCollection(ipInteriorRingBag);

	long inCount = 0;
	ipInteriorCollection->get_GeometryCount(&inCount);
	for (long i = 0; i < inCount; ++i)
	{
		IGeometryPtr ipInteriorGeometry;
		ipInteriorCollection->get_Geometry(i, &ipInteriorGeometry);
		polygon.m_innerRings.push_back(ToRing(ipInteriorGeometry));
	}

	return polygon;
}

rpc::Ring ToRing(const IGeometryPtr& ipRingGeometry)
{
	long count = 0;
	((IPointCollectionPtr)ipRingGeometry)->get_PointCount(&count);
	if (count == 0)
		return rpc::Ring();

	std::vector<WKSPoint> wksPoints(count);
	((IPointCollectionPtr)ipRingGeometry)->QueryWKSPoints(0, count, &wksPoints[0]);

	rpc::Ring ring;
	for (auto& wksPt : wksPoints)
	{
		ring.m_vertices.emplace_back(wksPt.X, wksPt.Y);
	}
	return ring;
}
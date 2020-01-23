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

#include "StdAfx.h"
#include "GlobalFunc.h"

namespace gf
{
	// ドメイン名取得
	bool getDomain( const _IRowPtr& ipRow, const LONG& lFID, const LONG& lCode, CString& strDomain )
	{
		if( !ipRow )
			return false;

		IFieldsPtr ipFields;
		if( FAILED( ipRow->get_Fields( &ipFields ) ) || !ipFields )
			return false;

		IFieldPtr ipField;
		if( FAILED( ipFields->get_Field( lFID, &ipField ) ) || !ipField )
			return false;

		IDomainPtr ipDomain;
		if( FAILED( ipField->get_Domain( &ipDomain ) ) )
			return false;

		if( ipDomain)
		{
			ICodedValueDomainPtr ipCVDomain( ipDomain );
			LONG lCount;
			if( FAILED( ipCVDomain->get_CodeCount( &lCount ) ) )
				return false;

			CComVariant vaValue;
			for( LONG i=0; i<lCount; i++ )
			{
				if( FAILED( ipCVDomain->get_Value( i, &vaValue ) ) )
					return false;

				if( lCode == vaValue.lVal )
				{
					CComBSTR bstr;
					if( FAILED( ipCVDomain->get_Name( i, &bstr ) ) )
						return false;

					strDomain = CString(bstr);
					break;
				}
			}
		}

		return true;
	}

	// 緯度経度取得
	bool getLonLat( const IGeometryPtr& ipGeo, WKSPoint& point, bool bLabelPoint )
	{
		if( !ipGeo )
			return false;

		esriGeometryType esriGeoType;
		if( FAILED( ipGeo->get_GeometryType( &esriGeoType ) ) )
			return false;

		IPointPtr ipPoint;
		if( esriGeoType == esriGeometryPoint )
		{
			ipPoint = ipGeo;
		}
		else if( esriGeoType == esriGeometryPolyline )
		{
			IPolylinePtr ipPolyline( ipGeo );
			double dLen = 0.0;
			if( FAILED( ipPolyline->get_Length( &dLen ) ) )
				return false;
			if( FAILED( ipPoint.CreateInstance( CLSID_Point ) ) )
				return false;
			if( FAILED( ipPolyline->QueryPoint( esriNoExtension, dLen/2.0, VARIANT_FALSE, ipPoint ) ) || !ipPoint )
				return false;
		}
		else if( esriGeoType == esriGeometryPolygon )
		{
			IAreaPtr ipArea( ipGeo );
			if( bLabelPoint )
			{
				if( FAILED( ipArea->get_LabelPoint( &ipPoint ) ) || !ipPoint )
					return false;
			}
			else
			{
				if( FAILED( ipArea->get_Centroid( &ipPoint ) ) || !ipPoint )
					return false;
			}
		}

		if( FAILED( ipPoint->QueryCoords( &(point.X), &(point.Y) ) ) )
			return false;

		return true;
	}

	// 構成点数取得
	bool getPointCount( const IGeometryPtr& ipGeo, LONG& lCount )
	{
		lCount = 0;
		if( !ipGeo )
			return false;

		esriGeometryType esriGeoType;
		if( FAILED( ipGeo->get_GeometryType( &esriGeoType ) ) )
			return false;

		if( esriGeoType == esriGeometryPoint )
		{
			lCount = 1;
		}
		else if( esriGeoType == esriGeometryPolyline || esriGeoType == esriGeometryPolygon )
		{
			IPointCollectionPtr ipPointCol( ipGeo );
			if( FAILED( ipPointCol->get_PointCount( &lCount ) ) )
				return false;
		}

		return true;
	}

	// 線形オブジェクト？
	bool isLinearObject( const IGeometryPtr& ipGeo, bool& bResult )
	{
		bResult = false;
		if( !ipGeo )
			return false;

		esriGeometryType esriGeoType;
		if( FAILED( ipGeo->get_GeometryType( &esriGeoType ) ) )
			return false;

		if( esriGeoType == esriGeometryPolyline || esriGeoType == esriGeometryPolygon )
		{
			ISegmentCollectionPtr ipSegCol( ipGeo );
			VARIANT_BOOL vb = VARIANT_FALSE;
			if( FAILED( ipSegCol->HasNonLinearSegments( &vb ) ) )
				return false;

			if( VARIANT_FALSE == vb)
				bResult = true;
			else
				bResult = false;
		}

		return true;
	}

	// 長さ取得
	bool getLength( const IGeometryPtr& ipGeo, double& dLen )
	{
		dLen = -1.0;
		if( !ipGeo )
			return false;

		esriGeometryType esriGeoType;
		if( FAILED( ipGeo->get_GeometryType( &esriGeoType ) ) )
			return false;

		switch( esriGeoType )
		{
		case esriGeometryPolyline:
			if( FAILED( ((IPolylinePtr)ipGeo)->get_Length( &dLen ) ) )
				return false;
			break;
		case esriGeometryPolygon:
			{
				// 境界を取得
				ITopologicalOperatorPtr ipTopo( ipGeo );
				IGeometryPtr ipBoundaryGeo;
				if( FAILED( ipTopo->get_Boundary( &ipBoundaryGeo ) ) || !ipBoundaryGeo )
					return false;
				if( FAILED( ((IPolylinePtr)ipBoundaryGeo)->get_Length( &dLen ) ) )
					return false;
			}			
			break;
		default:
			break;
		}

		return true;
	}

	// 面積取得
	bool getArea( const IGeometryPtr& ipGeo, double& dArea )
	{
		dArea = -1.0;
		if( !ipGeo )
			return false;

		esriGeometryType esriGeoType;
		if( FAILED( ipGeo->get_GeometryType( &esriGeoType ) ) )
			return false;

		if( esriGeometryPolygon == esriGeoType )
			if( FAILED( ((IAreaPtr)ipGeo)->get_Area(&dArea) ) )
				return false;

		return true;
	}
}

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

#include <AheGeometryOp.h>

namespace Util
{
	IGeometryPtr getBuffer( const IGeometryPtr& geo, double meter )
	{
		// m → 度変換
		IUnitConverterPtr unitConv( CLSID_UnitConverter );
		double dConvRange( 0.0 );
		unitConv->ConvertUnits( meter, esriMeters, esriDecimalDegrees, &dConvRange );

		// 形状をバッファリング
		ITopologicalOperatorPtr ipTopo( geo );
		IGeometryPtr bufferGeo;
		ipTopo->Buffer( dConvRange, &bufferGeo );

		// 空間参照付与
		ISpatialReferencePtr ipSpRef;
		geo->get_SpatialReference( &ipSpRef );
		bufferGeo->putref_SpatialReference( ipSpRef );

		return bufferGeo;
	}

	bool getNearestDistance( const IFeaturePtr& ipBase, const IFeaturePtr& ipComp, double& nearestDistance )
	{
		// 比較先の空間参照取得
		ISpatialReferencePtr ipSpRef = ipComp->GetShapeCopy()->GetSpatialReference();

		// 比較先の構成点でループ
		IPointCollectionPtr ipPointCol( ipComp->GetShapeCopy() );
		for( int i = 0; i < ipPointCol->GetPointCount(); ++i )
		{
			IPointPtr ipPoint = ipPointCol->GetPoint( i );

			// 比較先に対し、比較元の構成点からの最短地点を取得
			IPointPtr ipNearestPoint;
			if( FAILED( ( (IProximityOperatorPtr)ipBase->GetShapeCopy() )->ReturnNearestPoint(
				ipPoint, esriSegmentExtension::esriNoExtension, &ipNearestPoint ) ) || !ipNearestPoint )
				return false;
			ipNearestPoint->putref_SpatialReference( ipSpRef );

			// 暫定の最短距離よりも短ければ更新
			double distance = AheGetMeterLength( ipPoint, ipNearestPoint );
			if( nearestDistance > distance )
				nearestDistance = distance;
		}

		return true;
	}

	void getXY( const IFeaturePtr& ipFeature, double& x, double& y, const ISpatialReferencePtr& ipSpRef /*= nullptr*/ )
	{
		IPointCollectionPtr ipPointCol( ipFeature->GetShapeCopy() );
		if( ipSpRef )
			IGeometryPtr( ipPointCol )->putref_SpatialReference( ipSpRef );

		// 始点からポイント取得
		IPointPtr ipPoint = ipPointCol->GetPoint( 0 );

		// 始終点の中間（ポリライン上）にポイントをズラしてみる
		IConstructPointPtr ipConstructPoint( ipPoint );
		ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipFeature->GetShapeCopy(), esriExtendAtFrom, 0.5, VARIANT_TRUE );

		// 中点の経緯度を取得
		ipPoint->QueryCoords( &x, &y );
	}

	void getGid2Feature( const std::vector<IFeaturePtr>& features, const sindy::CFieldMap& fieldMap, std::map<CString, IFeaturePtr>& ret )
	{
		long index = fieldMap.HasField( sindy::schema::adam_v2_global_ipc_feature::kGlobalID );
		if( -1 == index )
			return;

		for( const auto& feature : features )
		{
			CComVariant vaGid = feature->GetValue( index );
			if( vaGid.vt != VT_NULL )
				ret.emplace( CString( vaGid.bstrVal ), feature );
		}
	}
}
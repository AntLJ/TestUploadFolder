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
		// m �� �x�ϊ�
		IUnitConverterPtr unitConv( CLSID_UnitConverter );
		double dConvRange( 0.0 );
		unitConv->ConvertUnits( meter, esriMeters, esriDecimalDegrees, &dConvRange );

		// �`����o�b�t�@�����O
		ITopologicalOperatorPtr ipTopo( geo );
		IGeometryPtr bufferGeo;
		ipTopo->Buffer( dConvRange, &bufferGeo );

		// ��ԎQ�ƕt�^
		ISpatialReferencePtr ipSpRef;
		geo->get_SpatialReference( &ipSpRef );
		bufferGeo->putref_SpatialReference( ipSpRef );

		return bufferGeo;
	}

	bool getNearestDistance( const IFeaturePtr& ipBase, const IFeaturePtr& ipComp, double& nearestDistance )
	{
		// ��r��̋�ԎQ�Ǝ擾
		ISpatialReferencePtr ipSpRef = ipComp->GetShapeCopy()->GetSpatialReference();

		// ��r��̍\���_�Ń��[�v
		IPointCollectionPtr ipPointCol( ipComp->GetShapeCopy() );
		for( int i = 0; i < ipPointCol->GetPointCount(); ++i )
		{
			IPointPtr ipPoint = ipPointCol->GetPoint( i );

			// ��r��ɑ΂��A��r���̍\���_����̍ŒZ�n�_���擾
			IPointPtr ipNearestPoint;
			if( FAILED( ( (IProximityOperatorPtr)ipBase->GetShapeCopy() )->ReturnNearestPoint(
				ipPoint, esriSegmentExtension::esriNoExtension, &ipNearestPoint ) ) || !ipNearestPoint )
				return false;
			ipNearestPoint->putref_SpatialReference( ipSpRef );

			// �b��̍ŒZ���������Z����΍X�V
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

		// �n�_����|�C���g�擾
		IPointPtr ipPoint = ipPointCol->GetPoint( 0 );

		// �n�I�_�̒��ԁi�|�����C����j�Ƀ|�C���g���Y�����Ă݂�
		IConstructPointPtr ipConstructPoint( ipPoint );
		ipConstructPoint->ConstructAlong( (ICurvePtr)(IPolylinePtr)ipFeature->GetShapeCopy(), esriExtendAtFrom, 0.5, VARIANT_TRUE );

		// ���_�̌o�ܓx���擾
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
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
#include "CheckMain.h"
#include <sindy/workspace.h>

CCheckMain::CCheckMain(void)
{
}

CCheckMain::~CCheckMain(void)
{
}

int CCheckMain::Run(const CParamCtrl& cParam)
{
	int iErrFlag = 0;
	IFeatureClassPtr ipFeatureClass = GetFeatureClass( cParam.GetDB(), cParam.GetFeatureClass() );
	if( !ipFeatureClass )
	{
		std::cerr << "[ERROR] failed GetFeatureClass." << std::endl;
		return 1;
	}

	std::cout << "対象の検索" << std::endl;

	IQueryFilterPtr ipAllFilter = AheInitQueryFilter( NULL, NULL, NULL );
	IFeatureCursorPtr ipCursor;
	HRESULT hr = ipFeatureClass->Search( ipAllFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
		return 1;

	// 既にチェックしたかどうか保持する
	std::map<long, bool> mapCheckFeature;

	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		long lOID = 0;
		ipFeature->get_OID( &lOID );
		mapCheckFeature[ lOID ] = false;
	}

	std::cout << "チェック開始" << std::endl;

	long lTotalCnt = mapCheckFeature.size();
	long lCnt = 1;
	long lErrCntArea = 0;
	long lWrnCntArea = 0;
	long lErrCntSelf = 0;
	long lErrCntMulti = 0;
	long lErrCntClear = 0;
	for( std::map<long, bool>::iterator it = mapCheckFeature.begin(); it != mapCheckFeature.end(); ++it, ++lCnt )
	{
		printf( "%07d / %07d\r", lCnt, lTotalCnt );

		// チェックするフィーチャのOID
		long lOID = it->first;
		IFeaturePtr ipFeature;
		ipFeatureClass->GetFeature( lOID, &ipFeature );

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );

		// 面積0チェック
		IAreaPtr ipArea( ipGeom );
		if( !ipArea )
		{
			std::cerr << "[ERROR] not Polygon( " << lOID << " )" << std::endl;
			iErrFlag = 1;
			continue;
		}
		double dArea = 0;
		ipArea->get_Area( &dArea );
		if( dArea <= 0 )
		{
			++lErrCntArea;
			std::cerr << "[ERROR] Area is 0( " << lOID << " )" << std::endl;
			iErrFlag = 1;
			continue;
		}

		// 自己交差チェック
		if( !CheckCrossMyself( lOID, ipGeom ) )
		{
			++lErrCntSelf;
			iErrFlag = 1;
		}

		// マルチポリゴンチェック
		if( cParam.GetMultiP() )
		{
			if( !CheckMultiPolygon( lOID, ipGeom) )
			{
				++lErrCntMulti;
				iErrFlag = 1;
			}
		}

		// 隙間チェック
		if( cParam.GetClearance() )
		{
			if( !CheckClearance( ipFeatureClass, lOID, ipGeom ) )
			{
				++lErrCntClear;
				iErrFlag = 1;
			}
		}
	}

	std::cerr << "--- ポリゴンチェック数	: " << lCnt-1 << " ---" << std::endl;
	std::cerr << "面積チェックエラー	: " << lErrCntArea << " ( " << lWrnCntArea << " )" << std::endl;
	std::cerr << "自己交差チェックエラー	: " << lErrCntSelf << std::endl;
	if( cParam.GetMultiP() )
		std::cerr << "マルチポリゴンエラー	: " << lErrCntMulti << std::endl;
	if( cParam.GetClearance() )
		std::cerr << "隙間チェックエラー	: " << lErrCntClear << std::endl;

	if (iErrFlag == 1)
	{
		return 3;
	} 
	else
	{
		return 0;
	}
}

IFeatureClassPtr CCheckMain::GetFeatureClass(const CString& strDB, const CString& strFeatureClass)
{
	if( strDB.IsEmpty() || strFeatureClass.IsEmpty() )
		return NULL;

	IWorkspacePtr ipWorkspace = sindy::create_workspace( strDB );
	if( !ipWorkspace )
		return false;
	
	IFeatureWorkspacePtr ipFeatureWorkspace( ipWorkspace );
	IFeatureClassPtr ipFeatureClass;
	ipFeatureWorkspace->OpenFeatureClass( CComBSTR( strFeatureClass ), &ipFeatureClass );

	return ipFeatureClass;
}

bool CCheckMain::CheckCrossMyself(long lOID, const IGeometryPtr& ipGeom)
{
	ISegmentCollectionPtr ipSegCol( ipGeom );
	if( !ipSegCol )
	{
		std::cerr << "[ERROR] failed get SegmentCollection( " << lOID << " )" << std::endl;
		return false;
	}

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );
	long lSegCnt = 0;
	ipSegCol->get_SegmentCount( &lSegCnt );
	for( long l=0; l<lSegCnt; ++l )
	{
		_ISegmentPtr ipBaseSegment;
		ipSegCol->get_Segment( l, &ipBaseSegment );

		ISegmentCollectionPtr ipBasePoly( CLSID_Polyline );
		((IGeometryPtr)ipBasePoly)->putref_SpatialReference( ipSpRef );
		ipBasePoly->AddSegment( ipBaseSegment );

		for( long j=l+1; j<lSegCnt; ++j )
		{
			_ISegmentPtr ipTargetSegment;
			ipSegCol->get_Segment( j, &ipTargetSegment );

			ISegmentCollectionPtr ipTargetPoly( CLSID_Polyline );
			((IGeometryPtr)ipTargetPoly)->putref_SpatialReference( ipSpRef );
			ipTargetPoly->AddSegment( ipTargetSegment );

			if( AheIsCross( (IGeometryPtr)ipBasePoly, (IGeometryPtr)ipTargetPoly ) )
			{
				std::cerr << "[ERROR] found selfintersection ( " << lOID << " )" << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool CCheckMain::CheckMultiPolygon(long lOID, const IGeometryPtr& ipGeom)
{
	long lCount = 0;
	((IPolygonPtr)ipGeom)->get_ExteriorRingCount( &lCount );
	if( lCount != 1 )
	{
		std::cerr << "[ERROR] found MultiPolygon. Exterior count is " << lCount << "( " << lOID << " )" << std::endl;
		return false;
	}

	return true;
}

bool CCheckMain::CheckClearance(IFeatureClassPtr ipFeatureClass, long lOID, const IGeometryPtr& ipGeom)
{
	ISpatialFilterPtr ipSpFilter = AheGetSpatialFilter( ipGeom, ipFeatureClass/*, esriSpatialRelRelation*/ );
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );

	IFeatureCursorPtr ipFeatureCursor;
	ipFeatureClass->Search( ipSpFilter, VARIANT_FALSE, &ipFeatureCursor );
	if( !ipFeatureCursor )
		return false;

	IGeometryPtr ipAllPolygon = ipGeom;
	IFeaturePtr ipFeature;
	// 接触しているポリゴンで1つのポリゴンを作成する
	while( S_OK == ipFeatureCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		IGeometryPtr ipSearchGeom;
		ipFeature->get_ShapeCopy( &ipSearchGeom );

		long lUnionOID = 0;
		ipFeature->get_OID( &lUnionOID );
		if( lOID == lUnionOID )
			continue;

		// 自分よりもオブジェクトIDが小さい場合はチェックを除外
		if( lOID > lUnionOID )
		{
			VARIANT_BOOL vBool = VARIANT_FALSE;
			((IRelationalOperatorPtr)ipGeom)->Overlaps( ipSearchGeom, &vBool );
			if( vBool )
			{
				std::cerr << "[ERROR] found overlaps polygon( " << lOID << ", " << lUnionOID << " )" << std::endl;
				return false;
			}
		}

		IGeometryPtr ipRetGeom;
		((ITopologicalOperatorPtr)ipAllPolygon)->Union( ipSearchGeom, &ipRetGeom );
		ipAllPolygon = ipRetGeom;
	}

	IGeometryBagPtr ipExteriorBag;
	((IPolygon4Ptr)ipAllPolygon)->get_ExteriorRingBag( &ipExteriorBag );
	if( !ipExteriorBag )
		return false;

	IGeometryCollectionPtr ipExteriorCol( ipExteriorBag );
	long lExtCnt = 0;
	ipExteriorCol->get_GeometryCount( &lExtCnt );
	for( long l=0; l<lExtCnt; ++l )
	{
		IGeometryPtr ipExtRing;
		ipExteriorCol->get_Geometry( l, &ipExtRing );

		IGeometryBagPtr ipInteriorBag;
		((IPolygon4Ptr)ipAllPolygon)->get_InteriorRingBag( (IRingPtr)ipExtRing, &ipInteriorBag );
		if( !ipInteriorBag )
			continue;
		IGeometryCollectionPtr ipInteriorCol( ipInteriorBag );
		long lInteriorCnt = 0;
		ipInteriorCol->get_GeometryCount( &lInteriorCnt );
		for( long j=0; j<lInteriorCnt; ++j )
		{
			IGeometryPtr ipInteriorRing;
			ipInteriorCol->get_Geometry( j, &ipInteriorRing );

			((IRingPtr)ipInteriorRing)->ReverseOrientation();

			IGeometryCollectionPtr ipPolygon( CLSID_Polygon );
			((IPolygonPtr)ipPolygon)->putref_SpatialReference( ipSpRef );
			ipPolygon->AddGeometry( ipInteriorRing );

			VARIANT_BOOL vBool = VARIANT_TRUE;
			IGeometryPtr ipIntersect;
			((ITopologicalOperatorPtr)ipGeom)->Intersect( (IGeometryPtr)ipPolygon, esriGeometry1Dimension, &ipIntersect );
			if( ipIntersect )
			{
				long lIntersect = 0;
				ipIntersect->get_IsEmpty( &vBool );
				IPointCollectionPtr ipPointCol( ipIntersect );
				ipPointCol->get_PointCount( &lIntersect );
				if( 1 < lIntersect )
				{
					std::cerr << "[ERROR] There is a hole around this polygon( " << lOID << " )" << std::endl;

#if _DEBUG
					IPointPtr ipPoint;
					CString strMsg;
					for( long k=0; k<lIntersect; ++k )
					{
						ipPointCol->get_Point( k, &ipPoint );
						double dx = 0;
						double dy = 0;
						ipPoint->get_X( &dx );
						ipPoint->get_Y( &dy );
						strMsg.Format( _T("P%02d( %f, %f )"), k, dx, dy );
						std::cerr << CT2CA( strMsg ) << std::endl;
					}
#endif
					return false;
				}
			}
		}
	}
	return true;
}

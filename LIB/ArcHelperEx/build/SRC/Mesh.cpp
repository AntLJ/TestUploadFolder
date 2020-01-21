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

// Mesh.cpp: CMesh クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mesh.h"
#include "AheGlobals.h"
#include "AheGeometryOp.h"
#include "AheMeshUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMesh::CMesh()
{

}

CMesh::~CMesh()
{

}

void CMesh::SetSpatialReference( ISpatialReferencePtr ipSpRel )
{
	m_ipSpRel = ipSpRel;
}
IEnvelopePtr CMesh::GetMeshExtent( unsigned int iMeshCode )
{
	if( iMeshCode < 0 ) return NULL;

	ATLASSERT( m_ipSpRel != NULL );

	// メッシュの四隅を取得
	DOUBLE dXMax, dYMax, dXMin, dYMin;

	AheMESH2LL( iMeshCode, 0, 0, dXMin, dYMin );
	AheMESH2LL( iMeshCode, 1000000, 1000000, dXMax, dYMax );

	IEnvelopePtr ipEnv( CLSID_Envelope );

	ipEnv->PutCoords( dXMin, dYMin, dXMax, dYMax );
	ipEnv->putref_SpatialReference( m_ipSpRel );
	ipEnv->SnapToSpatialReference();

	return ipEnv;
}
IGeometryCollectionPtr CMesh::GetMeshesPolygon()
{
	IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
	for( std::map<UINT, IEnvelopePtr>::iterator it = m_mapMeshList.begin(); it != m_mapMeshList.end(); it++ )
	{
		if( it->second != NULL )
		{
			ipGeomCol->AddGeometry( EnvelopeToPolygon( it->second ) );
		}
	}

/*	ITopologicalOperatorPtr ipTopo( ipGeomCol );
	if( ipTopo == NULL ) return NULL;

	ipTopo->Simplify();*/

	return ipGeomCol;
}
void CMesh::AddMesh( UINT iMeshCode )
{
	// リストに追加
	m_mapMeshList[iMeshCode] = GetMeshExtent( iMeshCode );
}
void CMesh::AddMesh( DOUBLE dx, DOUBLE dy )
{
	UINT iMeshCode = GetMeshCode( dx, dy );
	
	// まだ追加されていないメッシュなら追加
	if( Exist( iMeshCode ) == FALSE ) AddMesh( iMeshCode );
}
void CMesh::AddMesh( IPointPtr ipPoint )
{
	UINT iMeshCode = GetMeshCode( ipPoint );

	// まだ追加されていないメッシュなら追加
	if( Exist( iMeshCode ) == FALSE ) AddMesh( iMeshCode );
}
void CMesh::AddMesh( IGeometryPtr ipGeom )
{
	if( ipGeom == NULL ) return;
	IPointCollectionPtr ipPointCol( ipGeom );

//	LONG lPointCount;

	if( m_ipSpRel == NULL )
	{
		ISpatialReferencePtr ipSpRel;
		ipGeom->get_SpatialReference( &ipSpRel );	// 空間参照取得
		SetSpatialReference( ipSpRel );
	}

	// ポイントの場合
	if( ipPointCol == NULL )
	{
		IPointPtr ipPoint( ipGeom );
		if( ipPoint == NULL ) return;

		AddMesh( ipPoint );
	}
	// ポイント以外
	else {
/*		ipPointCol->get_PointCount( &lPointCount );
		for( LONG i = 0; i < lPointCount; i++ )
		{
			IPointPtr ipPoint;
			
			ipPointCol->get_Point( i, &ipPoint );
			AddMesh( ipPoint );
		}*/
		// [bug 2338] メッシュをたくさん跨ぐ場合、構成点のないメッシュ
		// が追加されないので、ジオメトリのエリアに存在するメッシュを全て
		// 追加するように修正
		// ジオメトリのエリアを取得
		IEnvelopePtr ipEnv;
		ipGeom->get_Envelope( &ipEnv );
		// 4隅が存在するメッシュ取得
		DOUBLE dXMin = 0.0f, dXMax = 0.0f, dYMin = 0.0f, dYMax = 0.0f;
		ipEnv->QueryCoords( &dXMin, &dYMin, &dXMax, &dYMax );
		UINT nLLMeshCode = GetMeshCode( dXMin, dYMin );
		UINT nLUMeshCode = GetMeshCode( dXMin, dYMax );
		UINT nRLMeshCode = GetMeshCode( dXMax, dYMin );
		UINT nRUMeshCode = GetMeshCode( dXMax, dYMax );
		std::list<UINT> listMesh;
		// 隙間がないようにメッシュリスト作成
		for( UINT i = nLLMeshCode;; i = INC2NDMESH_Y(i) )
		{
			for( UINT j = i;; j = INC2NDMESH_X(j) )
			{
				ATLTRACE(_T("CMesh::AddMesh() : push_back( %d )\n"), j );
				if( Exist( j ) == FALSE ) AddMesh( j );
				if( j == nRLMeshCode ) 
				{
					nRLMeshCode = INC2NDMESH_Y( nRLMeshCode );
					break;
				}
			}
			if( i == nLUMeshCode )
				break; 
		}
	}
}
void CMesh::DeleteMesh( UINT iMeshCode )
{
	ATLASSERT(FALSE);
}
void CMesh::DeleteMesh( DOUBLE dx, DOUBLE dy )
{
	ATLASSERT(FALSE);
}
void CMesh::DeleteMesh( IPointPtr ipPoint )
{
	ATLASSERT(FALSE);
}
BOOL CMesh::Exist( UINT iMeshCode )
{
	IEnvelopePtr ipEnv( m_mapMeshList[iMeshCode] );

	if( ipEnv == NULL ) 
		return FALSE;
	else
		return TRUE;

	return FALSE;
}
BOOL CMesh::Exist( DOUBLE dx, DOUBLE dy )
{
	return Exist( GetMeshCode( dx, dy ) );
}
unsigned int CMesh::GetMeshCode( DOUBLE dx, DOUBLE dy )
{
	INT ix = 0, iy = 0 ;

	unsigned int iMeshCode = AheLL2MESH( dx, dy, 2, ix, iy );

	return iMeshCode;
}
unsigned int CMesh::GetMeshCode( IPointPtr ipPoint )
{
	DOUBLE dx, dy;

	if( ipPoint != NULL )
	{
		ipPoint->QueryCoords( &dx, &dy );
		
		return GetMeshCode( dx, dy );
	} 
	else 
		return -1;
}
UINT CMesh::GetExistMeshCode( IGeometryPtr ipGeom )
{
	if ( ipGeom )
	{
		AddMesh( ipGeom );

		esriGeometryType geomType = esriGeometryAny;
		ipGeom->get_GeometryType( &geomType );

		for( std::map<UINT, IEnvelopePtr>::iterator it = m_mapMeshList.begin(); it != m_mapMeshList.end(); ++it )
		{
			// [Bug 7833]2次メッシュ境界にリンク列作成の際にLQにメッシュコードが入らない
			switch ( geomType )
			{
				case esriGeometryPoint:
				case esriGeometryMultipoint:
					// ポイントは今までどおり
					if( AheIsContain( it->second, ipGeom, m_ipSpRel ) )
						return it->first;
				default:
					{
						IAreaPtr ipArea( ipGeom );
						IPointPtr ipPoint;

						if ( ipArea )
							ipArea->get_Centroid( &ipPoint );
						else
						{
							// 中点で判定する。
							ICurvePtr ipCurve( ipGeom );

							if ( ipCurve )
							{
								double dLen = 0.0f;

								ipPoint.CreateInstance( CLSID_Point );
								ipPoint->putref_SpatialReference( m_ipSpRel ); // 空間参照付与
								ipCurve->get_Length( &dLen );
								ipCurve->QueryPoint( esriNoExtension, dLen/2.0f, VARIANT_FALSE, ipPoint );
							}
						}

						if ( ipPoint )
						{
							if( AheIsContain( it->second, ipPoint, m_ipSpRel ) )
								return it->first;
						}
					}
					break;
			}
		}
	}

	ATLASSERT( FALSE );
	return -1;
}

IGeometryPtr CMesh::GetIntersectPointsByShape( IGeometryPtr ipGeom )
{
	AddMesh( ipGeom );

	// ポリラインの場合は始点・終点を取得しておく
	IPolylinePtr ipPoly( ipGeom );
	IPointPtr ipFromPoint, ipToPoint;
	if( ipPoly )
	{
		ipPoly->get_FromPoint( &ipFromPoint );
		ipPoly->get_ToPoint( &ipToPoint );
	}

	IPointCollectionPtr ipPointCol( CLSID_Multipoint );
	((IGeometryPtr)ipPointCol)->putref_SpatialReference( AheGetSpatialReference( ipGeom ) );  // 空間参照付与

	for( std::map<UINT, IEnvelopePtr>::iterator it = m_mapMeshList.begin(); it != m_mapMeshList.end(); it++ )
	{
/*		IGeometryPtr ipRetGeom;
		ITopologicalOperatorPtr ipTopo( EnvelopeToPolygon( it->second ) );

		if( ipTopo == NULL )
		{
			ATLASSERT( FALSE );
			continue;
		}

		ipTopo->Intersect( ipGeom, esriGeometry0Dimension, &ipRetGeom );*/
		IGeometryPtr ipRetGeom = AheGetCrossOrTouchPoints( EnvelopeToPolygon( it->second ), ipGeom );

		// スプリット形状の始点・終点と一致していないなら追加
		// こうしないと2次メッシュ線に接する道路を動かしたときにもポイントが取得されてしまう
		if( ipRetGeom != NULL )
		{
			esriGeometryType type;

			ipRetGeom->get_GeometryType( &type );
			
			// マルチポイントの場合
			if( type == esriGeometryMultipoint )
			{
				IPointCollectionPtr ipIntersectCol( ipRetGeom );
				LONG lCount;

				ipIntersectCol->get_PointCount( &lCount );
				for( LONG i = 0; i < lCount; i++ )
				{
					IPointPtr ipPoint;
					ipIntersectCol->get_Point( i, &ipPoint );

					// チェックして追加
					VARIANT_BOOL vaIsEqualToPoint = VARIANT_FALSE, vaIsEqualFromPoint = VARIANT_FALSE;
					if( ipFromPoint != NULL && ipToPoint != NULL )
					{
						IRelationalOperatorPtr ipRel( ipPoint );

						ipRel->Equals( ipFromPoint, &vaIsEqualFromPoint );
						ipRel->Equals( ipToPoint, &vaIsEqualToPoint );
					}

					if( vaIsEqualToPoint == VARIANT_TRUE || vaIsEqualFromPoint == VARIANT_TRUE )
						continue;
					else
						ipPointCol->AddPoint( ipPoint );
				}
			}
			// ポイントの場合
			else if( type == esriGeometryPoint )
			{
				IPointPtr ipPoint( ipRetGeom );

				// チェックして追加
				VARIANT_BOOL vaIsEqualToPoint = VARIANT_FALSE, vaIsEqualFromPoint = VARIANT_FALSE;
				if( ipFromPoint != NULL && ipToPoint != NULL )
				{

					IRelationalOperatorPtr ipRel( ipPoint );

					ipRel->Equals( ipFromPoint, &vaIsEqualFromPoint );
					ipRel->Equals( ipToPoint, &vaIsEqualToPoint );
				}

				if( vaIsEqualToPoint == VARIANT_TRUE || vaIsEqualFromPoint == VARIANT_TRUE )
					continue;
				else
					ipPointCol->AddPoint( ipPoint );
			}
		}
	}

	// 重複点を取り除く
	ITopologicalOperatorPtr ipTopoMultiPoint( ipPointCol );
	ipTopoMultiPoint->Simplify();

#if 1
	// でばぐ
	LONG lCount;
	ipPointCol->get_PointCount( &lCount );
//	ATLTRACE(_T("CMesh::GetIntersectPointsByShape : 総ポイント数：%d\n"), lCount );
#endif

	IGeometryPtr ipOutGeom( ipPointCol );
	ipOutGeom->putref_SpatialReference( m_ipSpRel );
	ipOutGeom->SnapToSpatialReference();

	return ipOutGeom;
}
_ISetPtr CMesh::SplitShapeByMesh( IGeometryPtr ipGeom )
{
	AddMesh( ipGeom );

	_ISetPtr ipSet( CLSID_Set );
	ISpatialReferencePtr ipSpRef( AheGetSpatialReference( ipGeom ) );

	if( ipSet == NULL ) return NULL;

	for( std::map<UINT, IEnvelopePtr>::iterator it = m_mapMeshList.begin(); it != m_mapMeshList.end(); it++ )
	{
		IGeometryPtr ipRetGeom;
		IClonePtr ipClone;
		
		// コピー
		((IClonePtr)ipGeom)->Clone( &ipClone );
		IGeometryPtr ipGeomCopy( ipClone );
		ITopologicalOperatorPtr ipTopo( ipGeomCopy );

		ATLASSERT( ipTopo != NULL );

		// 2次メッシュと交差するかテスト
		IRelationalOperatorPtr ipRel( (IGeometryPtr)ipTopo );
		VARIANT_BOOL vaIsCross, vaIsOverlap;

		ipRel->Crosses( it->second, &vaIsCross );
		ipRel->Overlaps( it->second, &vaIsOverlap );

		// 交差するならクリップ
		if( vaIsCross == VARIANT_TRUE || vaIsOverlap == VARIANT_TRUE )
		{
			if( ipTopo->Clip( it->second ) == S_OK ) 
			{
				// パートが作成されたことを想定して各パートに分解
				IGeometryCollectionPtr ipGeomCol( ipTopo );
				LONG lGeometryCount;

				ipGeomCol->get_GeometryCount( &lGeometryCount );
				for( LONG i = 0; i < lGeometryCount; i++ )
				{
					IGeometryPtr ipPartGeom;

					ipGeomCol->get_Geometry( i, &ipPartGeom );
					IPathPtr ipPath( ipPartGeom );
					IRingPtr ipRing( ipPartGeom );

					// ポリライン
					if( ipPath != NULL )
					{
						IGeometryCollectionPtr ipPolylineGeomCol( CLSID_Polyline );
						((IGeometryPtr)ipPolylineGeomCol)->putref_SpatialReference( ipSpRef ); // 空間参照付与
						ipPolylineGeomCol->AddGeometry( ipPath );
						ipSet->Add( (IGeometryPtr)ipPolylineGeomCol );
					} 
					// ポリゴン
					else if( ipRing != NULL )
					{
						IGeometryCollectionPtr ipPolygonGeomCol( CLSID_Polygon );
						((IGeometryPtr)ipPolygonGeomCol)->putref_SpatialReference( ipSpRef ); // 空間参照付与
						ipPolygonGeomCol->AddGeometry( ipRing );
						ipSet->Add( (IGeometryPtr)ipPolygonGeomCol );
					}
					else
						ATLASSERT(FALSE);
				}
			}
		}
	}

#if _DEBUG
	// でばぐ
	LONG lCount;
	ipSet->get_Count( &lCount );
#endif

	return ipSet;
}
BOOL CMesh::IsTouches( IGeometryPtr ipGeom, std::list<UINT>& listMesh  )
{
	if( ipGeom == NULL ) return FALSE;

	BOOL bRet = FALSE;

	AddMesh( ipGeom );

	for( std::map<UINT, IEnvelopePtr>::iterator it = m_mapMeshList.begin(); it != m_mapMeshList.end(); it++ )
	{
		if( AheIsTouch( ipGeom, (IGeometryPtr)it->second, m_ipSpRel ) == TRUE )
		{
			listMesh.push_back( it->first );
			bRet = TRUE;
		}
	}

	return bRet;
}
IPolygonPtr CMesh::EnvelopeToPolygon( IEnvelopePtr ipEnv )
{
	// TODO: AheEnvelope2Polygonで良いのでは？
	// 引数チェック
	if( ! ipEnv )
	{
		_ASSERTE( ipEnv );
		return NULL;
	}

	ISpatialReferencePtr ipSpRel( AheGetSpatialReference( ipEnv ) );
	IPolygonPtr ipPolygon( CLSID_Polygon );
	ipPolygon->putref_SpatialReference( ipSpRel ); // 空間参照付与
	IPointPtr ipPt1(CLSID_Point), ipPt2(CLSID_Point), ipPt3(CLSID_Point), ipPt4(CLSID_Point);
	ipPt1->putref_SpatialReference( ipSpRel ); // 空間参照付与
	ipPt2->putref_SpatialReference( ipSpRel ); // 空間参照付与
	ipPt3->putref_SpatialReference( ipSpRel ); // 空間参照付与
	ipPt4->putref_SpatialReference( ipSpRel ); // 空間参照付与
	DOUBLE dXMin, dYMin, dXMax, dYMax;

	ipEnv->QueryCoords( &dXMin, &dYMin, &dXMax, &dYMax );
	ipPt1->PutCoords( dXMin, dYMin );
	ipPt1->SnapToSpatialReference();
	ipPt2->PutCoords( dXMin, dYMax );
	ipPt2->SnapToSpatialReference();
	ipPt3->PutCoords( dXMax, dYMax );
	ipPt3->SnapToSpatialReference();
	ipPt4->PutCoords( dXMax, dYMin );
	ipPt4->SnapToSpatialReference();

	IPointCollectionPtr ipPolyPointCol( ipPolygon );
	ipPolyPointCol->AddPoint( ipPt1 );
	ipPolyPointCol->AddPoint( ipPt2 );
	ipPolyPointCol->AddPoint( ipPt3 );
	ipPolyPointCol->AddPoint( ipPt4 );

	ipPolygon->SnapToSpatialReference();
	ipPolygon->Close();

	return ipPolygon;
}

UINT CMesh::INC2NDMESH_X(UINT mesh){ return AheINCMESH_X(mesh); }
UINT CMesh::INC2NDMESH_Y(UINT mesh){ return AheINCMESH_Y(mesh); }
UINT CMesh::DEC2NDMESH_X(UINT mesh){ return AheDECMESH_X(mesh); }
UINT CMesh::DEC2NDMESH_Y(UINT mesh){ return AheDECMESH_Y(mesh); }

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
* @file TargetFeature.cpp
*
* @brief フィーチャクラス群の実装ファイル
*/
#include "stdafx.h"

#include "TargetFeature.h"
#include "AheDraw.h"
#include "AheGlobalsX.h"
#include "ErrObj.h"
#include "type_convert.h"

using namespace sindy::schema;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTargetItem::CTargetItem( void) ///: m_lNewObjectID(-1)
{
}

CTargetItem::CTargetItem( IFeaturePtr ipFeature ) ///: m_lNewObjectID(-1)
{
	m_ipFeature = ipFeature;
///	m_lNewObjectID = lNewObjectID;
}

CTargetItem::~CTargetItem( void)
{
}

bool CTargetItem::operator==( const CTargetItem& cTargetItem ) const
{
	if( ! ( m_ipFeature || cTargetItem.m_ipFeature ) )
		return true;
	if( ! ( m_ipFeature && cTargetItem.m_ipFeature ) )
		return false;

	if( HasOID() && cTargetItem.HasOID() )
	{
		return ( 
			( GetOID() == cTargetItem.GetOID() ) && 
			( GetOwnerDotClassName().CompareNoCase( cTargetItem.GetOwnerDotClassName() ) == 0 ) &&
			( (IUnknown*)(IUnknownPtr)GetWorkspace() == (IUnknown*)(IUnknownPtr)cTargetItem.GetWorkspace() )
			//( GetVersionName().CompareNoCase( cTargetItem.GetVersionName() ) == 0 ) )
			);
	}	
	return false;
}

bool CTargetItem::operator<( const CTargetItem& cTargetItem ) const
{
	if( ! ( m_ipFeature && cTargetItem.m_ipFeature ) )
		return (IFeature*)m_ipFeature < (IFeature*)cTargetItem.m_ipFeature;

	if( HasOID() && cTargetItem.HasOID() )
	{
		if( (IUnknown*)(IUnknownPtr)GetWorkspace() < (IUnknown*)(IUnknownPtr)cTargetItem.GetWorkspace() )
			return true;
		else if( (IUnknown*)(IUnknownPtr)GetWorkspace() == (IUnknown*)(IUnknownPtr)cTargetItem.GetWorkspace() )
		{
			CString cThisName( GetOwnerDotClassName() ), cTargetName( cTargetItem.GetOwnerDotClassName() );
			if( cThisName < cTargetName )
				return true;
			else if( cThisName == cTargetName )
				return GetOID() < cTargetItem.GetOID();
		}
	}
	return false;
}

BOOL CTargetItem::HasOID() const 
{
	if( m_ipFeature == NULL )
	{
		_ASSERTE( m_ipFeature != NULL );
		return FALSE;
	}

	VARIANT_BOOL vbHasOID = VARIANT_FALSE;
	m_ipFeature->get_HasOID( &vbHasOID );
	
	return ( vbHasOID == VARIANT_TRUE ? TRUE : FALSE );
}

LONG CTargetItem::GetOID() const
{
	LONG lOID = -1;
	if( m_ipFeature != NULL && HasOID() )
	{
		m_ipFeature->get_OID( &lOID );

/*		// CreateFeatureBuffer で作成されたものは OID が -1 なので、そのときは m_lNewObjectID を返す
		if( lOID == -1 ) lOID = m_lNewObjectID;*/
	}

	_ASSERTE( lOID > 0 );	// OBJECTID は 1 以上でなければならない

	return lOID;
}
IWorkspacePtr CTargetItem::GetWorkspace() const
{
	return AheGetWorkspace( m_ipFeature );
}

CString CTargetItem::GetVersionName() const
{
	IVersionPtr ipVersion( AheGetWorkspace( m_ipFeature ) );
	if( ipVersion )
	{
		CComBSTR bstrVersionName;
		ipVersion->get_VersionName( &bstrVersionName );
		return CString(bstrVersionName);
	}
	return NULL;
}

IPointPtr CTargetItem::GetFromPoint()
{
	IPolylinePtr ipPoly( GetShape() );

	if( ipPoly == NULL ) return NULL;

	IPointPtr ipPoint;
	ipPoly->get_FromPoint( &ipPoint );
	_ASSERT_EXPR( AheGetSpatialReference(ipPoint), _T("Fromポイントの空間参照がNULL") );
	_ASSERT_EXPR( AheGetSpatialReference(ipPoly), _T("ポリラインの空間参照もNULL") );

	return ipPoint;
}

IPointPtr CTargetItem::GetToPoint()
{
	IPolylinePtr ipPoly( GetShape() );

	if( ipPoly == NULL ) return NULL;

	IPointPtr ipPoint;
	ipPoly->get_ToPoint( &ipPoint );
	return ipPoint;
}

esriGeometryType CTargetItem::GetGeometryType() const
{
	esriGeometryType emGTResult;
	IGeometryPtr piGeometry;

	m_ipFeature->get_Shape( &piGeometry);
	piGeometry->get_GeometryType( &emGTResult);

	return emGTResult;
}

// 編集完了
BOOL CTargetItem::Store()
{
	ATLASSERT( NULL != m_ipFeature );

	// 編集されているかどうかチェック
	IFeatureChangesPtr ipFeatureChanges( m_ipFeature );
	if( ipFeatureChanges != NULL )
	{
		VARIANT_BOOL vbChanged = VARIANT_FALSE;
		ipFeatureChanges->get_ShapeChanged( &vbChanged );
		if( vbChanged == VARIANT_TRUE )
		{
			// Simplify と SnapToSpatialReference をする
			// もともと関数の入り口にあったが、これをすると形状が変更されてしまうことがあるため
			// 変更していないのに保存されてしまう場合があり、bug 5264の原因となっているためここに移動（bug 5331）
			AheSnapToSpatialReferenceAndForceSimplify( GetShape(), GetSpatialReference() );
			ATLTRACE(_T("CTargetItem::Store() : LAYER=%s OID=%d : 形状が変更されているため、保存します\n"), GetFeatureClassName(), GetOID() );
			return SUCCEEDED( m_ipFeature->Store() );
		}
		else {
			IRowChangesPtr ipRowChanges( m_ipFeature );
			if( ipRowChanges != NULL )
			{
				LONG lFieldCount = -1;
				IFieldsPtr ipFields;
				m_ipFeature->get_Fields( &ipFields );
				ipFields->get_FieldCount( &lFieldCount );
				for( LONG i = 0; i < lFieldCount; i++ )
				{
					VARIANT_BOOL vbValueChanged = VARIANT_FALSE;
					ipRowChanges->get_ValueChanged( i, &vbValueChanged );
					if( vbValueChanged == VARIANT_TRUE )
					{
						ATLTRACE(_T("CTargetItem::Store() : LAYER=%s OID=%d : 属性が変更されているため、保存します\n"), GetFeatureClassName(), GetOID() );
						return SUCCEEDED( m_ipFeature->Store() );
					}
				}
			}
		}
	}
	// ここまできたら、保存する必要なし
	ATLTRACE(_T("CTargetItem::Store() : LAYER=%s OID=%d : 編集されていないようなので、保存しませんでした\n"), GetFeatureClassName(), GetOID() );
	return TRUE;
}

BOOL CTargetItem::SnapToSpatialReference( ISpatialReferencePtr piSpatialReference/* = NULL*/ )
{
	ISpatialReferencePtr piBaseSpatialReference;

	if( m_ipFeature == NULL )
	{
		ATLASSERT( m_ipFeature != NULL );
		return FALSE;
	}

	if( piSpatialReference == NULL )
	{
		IObjectClassPtr ipObjectClass;
		m_ipFeature->get_Class( &ipObjectClass );
		IGeoDatasetPtr ipDataset( ipObjectClass );
		if( ipDataset != NULL )
			ipDataset->get_SpatialReference( &piBaseSpatialReference );
	}
	else
	{
		piBaseSpatialReference = piSpatialReference;
	}

	if( piBaseSpatialReference != NULL )
	{
		IGeometryPtr piGeometry( GetShape() );
		if( piGeometry != NULL )
		{
			if( SUCCEEDED( piGeometry->putref_SpatialReference( piBaseSpatialReference)))
			{
				if( SUCCEEDED( piGeometry->SnapToSpatialReference()))
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

// 指定分のサイズ拡張
BOOL CTargetItem::Buffer( double dbDistance, IGeometry** ppiGeometry)
{
	IGeometryPtr piGeometryBase;

	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeometryBase)))
	{
		ITopologicalOperatorPtr piTopo;
		if( SUCCEEDED( piGeometryBase->QueryInterface( &piTopo)))
		{
			IGeometryPtr piGeometryDst;
			if( SUCCEEDED( piTopo->Buffer( dbDistance, &piGeometryDst)))
			{
				if( NULL == ppiGeometry)
				{
					if( SUCCEEDED( m_ipFeature->putref_Shape( piGeometryDst)))
					{
						return TRUE;
					}
				}
				else
				{
					( *ppiGeometry) = piGeometryDst;
					( *ppiGeometry)->AddRef();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTargetItem::Difference( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry)
{
	IGeometryPtr piGeometryBase;

	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeometryBase)))
	{
		ITopologicalOperatorPtr piTopo;
		if( SUCCEEDED( piGeometryBase->QueryInterface( &piTopo)))
		{
			IGeometryPtr piGeometryDst;
			if( SUCCEEDED( piTopo->Difference( piGeometrySrc, &piGeometryDst)))
			{
				if( NULL == ppiGeometry)
				{
					if( SUCCEEDED( m_ipFeature->putref_Shape( piGeometryDst)))
					{
						return TRUE;
					}
				}
				else
				{
					( *ppiGeometry) = piGeometryDst;
					( *ppiGeometry)->AddRef();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTargetItem::Intersect( IGeometryPtr piGeometrySrc, esriGeometryDimension resultDimension, IGeometry** ppiGeometry)
{
	IGeometryPtr piGeometryBase;

	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeometryBase)))
	{
		ITopologicalOperatorPtr piTopo;
		if( SUCCEEDED( piGeometryBase->QueryInterface( &piTopo)))
		{
			IGeometryPtr piGeometryDst;
			if( SUCCEEDED( piTopo->Intersect( piGeometrySrc, resultDimension, &piGeometryDst)))
			{
				if( NULL == ppiGeometry)
				{
					if( SUCCEEDED( m_ipFeature->putref_Shape( piGeometryDst)))
					{
						return TRUE;
					}
				}
				else
				{
					( *ppiGeometry) = piGeometryDst;
					( *ppiGeometry)->AddRef();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTargetItem::Union( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry)
{
	IGeometryPtr piGeometryBase;

	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeometryBase)))
	{
		ITopologicalOperatorPtr piTopo;
		if( SUCCEEDED( piGeometryBase->QueryInterface( &piTopo)))
		{
			IGeometryPtr piGeometryDst;
			if( SUCCEEDED( piTopo->Union( piGeometrySrc, &piGeometryDst)))
			{
				if( NULL == ppiGeometry)
				{
					if( SUCCEEDED( m_ipFeature->putref_Shape( piGeometryDst)))
					{
						return TRUE;
					}
				}
				else
				{
					( *ppiGeometry) = piGeometryDst;
					( *ppiGeometry)->AddRef();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTargetItem::AddNewPoint( IGeometryPtr piGeometry, LONG lIndex, IPointPtr piNewPoint)
{
	IGeometryPtr piCollection;
	//IGeometryCollectionPtr piGeomColn;
	IPathPtr piPath;
	IPointCollectionPtr piPointColn;

	//piGeomColn = piGeometry;
	//piGeomColn->get_Geometry( 0, &piCollection);
	piCollection = piGeometry;
	piPath = piCollection;
	piPointColn = piPath;

	piPointColn = piGeometry;


	VARIANT vtBefore;
	vtBefore.vt = VT_I4;
	vtBefore.intVal = lIndex;
	return SUCCEEDED( piPointColn->AddPoint( piNewPoint, &vtBefore));
}

BOOL CTargetItem::CreatePolygon( double dbBufferSize, IPolygon** ppiPolygon)
{
	//AheFeature2Polygon( m_ipFeature, dbBufferSize, ppiPolygon);
	//return ( NULL != *ppiPolygon);
	IGeometryPtr piGeom;
	ITopologicalOperatorPtr piPolyTopo;
	IPolygonPtr piPolygon;

	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeom)))
	{
		piPolyTopo = piGeom;
		piPolyTopo->Buffer( dbBufferSize, &piGeom);
		piPolygon = piGeom;
		*ppiPolygon = piPolygon;
		(*ppiPolygon)->AddRef();

		return TRUE;
	}
	return FALSE;
}

BOOL CTargetItem::GetEnvelope( IEnvelope** ppiEnv)
{
	IGeometryPtr piGeom;
	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeom)))
	{
		IEnvelopePtr piEnve;
		if( SUCCEEDED( piGeom->get_Envelope( &piEnve)))
		{
			(*ppiEnv) = piEnve;
			(*ppiEnv)->AddRef();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTargetItem::IsPECLink()
{
	if( m_ipFeature == NULL ) 
	{
		ATLASSERT( m_ipFeature != NULL );
		return FALSE;
	}
	CComVariant vaValue = AheGetAttribute( m_ipFeature, _T("ROADCLASS_C") );
	if( vaValue.vt == VT_I4 && 
		( vaValue.lVal == 401/*駐車場構内道路リンク（PEC）*/ || vaValue.lVal == 402/* マッチング用リンク（PEC）*/ ) )
		return TRUE;

	return FALSE;
}

// 指定のジオメトリと交差する？
BOOL CTargetItem::IsCrosses( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsCross( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定のジオメトリを含む？
BOOL CTargetItem::IsWithin( IGeometryPtr piGeom)
{
//	return AheIsWithin( piGeom, m_ipFeature);
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsWithin( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定のジオメトリを含む？
BOOL CTargetItem::IsWithin2( IGeometryPtr piGeom)
{
//	return AheIsWithin( piGeom, m_ipFeature);
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsWithin2( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定のジオメトリに触れている？
BOOL CTargetItem::IsTouches( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsTouch( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定ジオメトリに一致しない
BOOL CTargetItem::IsDisjoint( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsDisjoint( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定ジオメトリに等しい
BOOL CTargetItem::IsEquals( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsEqual( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定ジオメトリと重なる
BOOL CTargetItem::IsOverlaps( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;
	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsOverlap( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定ジオメトリを含む？
BOOL CTargetItem::IsContains( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;

	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsContain( piDistGeom, piGeom );
	}
	return FALSE;
}

// 指定ジオメトリを含む？
BOOL CTargetItem::IsContains2( IGeometryPtr piGeom)
{
	IGeometryPtr piDistGeom;

	if( SUCCEEDED( m_ipFeature->get_Shape( &piDistGeom)))
	{
		return AheIsContain2( piDistGeom, piGeom );
	}
	return FALSE;
}

// ２字メッシュ区画辺ノード？
BOOL CTargetItem::IsBorderNode( void)
{
	CComVariant cVar;
	switch ( AheGetNetWorkType( GetFeatureClassName() ) )
	{
	case sindyeNetworkRoad:
		cVar = AheGetAttribute( m_ipFeature, road_node::kNodeClass );
		return bool2BOOL( VT_I4 == cVar.vt && sindyeOnMesh == cVar.lVal );
		break;
	case sindyeNetworkWalk:
		cVar = AheGetAttribute( m_ipFeature, walk_node::kNodeClass );
		return bool2BOOL( VT_I4 == cVar.vt && walk_node::node_class::kOutline == cVar.lVal );
		break;
	case sindyeNetworkSubway:
		cVar = AheGetAttribute( m_ipFeature, sj::subway_node::kNodeClassC );
		return bool2BOOL( VT_I4 == cVar.vt && sj::subway_node::node_class::kOutline == cVar.lVal );
		break;
	case sindyeNetworkRailway:
		cVar = AheGetAttribute( m_ipFeature, sj::railway_node::kNodeClass_C );
		return bool2BOOL( VT_I4 == cVar.vt && sj::railway_node::node_class::kOutline == cVar.lVal );
		break;
	case sindyeNetworkLane:		// ADAMは2次メッシュ対象外
	case sindyeNetworkUpDown:	// ADAMは2次メッシュ対象外
	case sindyeNetworkCompart:	// ADAMは2次メッシュ対象外
	case sindyeNetworkBorder:	// ADAMは2次メッシュ対象外
	case sindyeNotNetwork:
	default:
		break;
	}
	return FALSE;
}

// ２字メッシュ区画辺PECノード？
BOOL CTargetItem::IsBorderPECNode( void)
{
	if( GetFeatureClassName().CompareNoCase( NODE_TABLE_NAME ) == 0 )
	{
		CComVariant cVar = AheGetAttribute( m_ipFeature, NODECLASS_FIELD );
		return ( VT_I4 == cVar.vt && sindyeOnMeshPEC == cVar.lVal );
	}
	return FALSE;
}

/*int CTargetItem::GetLayerName( LPTSTR lpszTableName, int nSize)
{
	int nResult = 0;

	IFeatureLayerPtr ipFeatureLayer;
	if( SUCCEEDED( m_piLayer->QueryInterface( &ipFeatureLayer)))
	{
		IFeatureClassPtr ipFeatureClass;
		if( SUCCEEDED( ipFeatureLayer->get_FeatureClass( &ipFeatureClass)))
		{
			IDatasetPtr piDataset;
			if( SUCCEEDED( ipFeatureClass->QueryInterface( &piDataset)))
			{
				BSTR bstrLayerName = NULL;
				piDataset->get_Name( &bstrLayerName);
				if( NULL != bstrLayerName)
				{
					USES_CONVERSION;
					LPCTSTR lpcsz = OLE2T( bstrLayerName);
					/// テーブル名が ****.**** の場合、最初の ****. は削除
					for( int nIndex = 0; nIndex < lstrlen( lpcsz); nIndex++)
					{
						if( TEXT('.') == lpcsz[ nIndex])
						{
							lpcsz = &( lpcsz[ nIndex+1]);
							break;
						}
					}
					if( NULL != lpszTableName)
					{
						lstrcpyn( lpszTableName, lpcsz, nSize);
					}
					nResult = lstrlen( lpcsz) + 1;

					::SysFreeString( bstrLayerName);
				}
			}
		}
	}
	return nResult;
}*/

BOOL CTargetItem::SymmetricDifference( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry)
{
	IGeometryPtr			piGeometryBase;

	if( SUCCEEDED( m_ipFeature->get_ShapeCopy( &piGeometryBase) ) ) {
		ITopologicalOperatorPtr 	piTopo;

		if( SUCCEEDED( piGeometryBase->QueryInterface( &piTopo) ) ) {
			IGeometryPtr 				piGeometryDst;

			piTopo->Simplify();
			if( SUCCEEDED( piTopo->SymmetricDifference( piGeometrySrc, &piGeometryDst) ) ) {
				if( NULL == ppiGeometry) {
					if( SUCCEEDED( m_ipFeature->putref_Shape( piGeometryDst) ) ) {
						return TRUE;
					}
				} else {
					( *ppiGeometry) = piGeometryDst;
					( *ppiGeometry)->AddRef();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTargetItem::GetPolyline( IGeometryPtr piGeometrySrc, IPolyline** ppiPolyline)
{
	return AheGeometry2Polyline(piGeometrySrc, ppiPolyline);
}

CComVariant CTargetItem::GetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName ) const
{
	CComVariant			vaValue;

	vaValue = AheGetAttribute( ipFeature, lpcszFieldName );
	return vaValue;
}

BOOL CTargetItem::SetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName, CComVariant vaValue )
{
	return AheSetAttribute( ipFeature, lpcszFieldName, vaValue );
}

void CTargetItem::DrawFeatureXOR( IFeaturePtr ipFeature, IScreenDisplayPtr ipScreen, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop)
{
	AheDrawFeatureXOR( ipScreen, ipFeature, dbBufferSize, bDrawVertex, bDoStartStop );
}

LONG CTargetItem::HitTestVertexIndex( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance)
{
	return AheHitTestVertexIndex( ipFeature, ipPoint, dTorelance );
}

LONG CTargetItem::GetVertexCount()
{
	LONG lVertexCount = -1;
	IPointCollectionPtr	ipPointCol( GetPointCollection() );

	if( ipPointCol != NULL )
		ipPointCol->get_PointCount( &lVertexCount );

	return lVertexCount;
}

BOOL CTargetItem::IsEditableItem( _IRowPtr ipFeature, sindyeEditType type ) const
{
	return AheIsEditableFeature( ipFeature, NULL, type );
}

// 形状チェック
BOOL CTargetItem::IsSafeGeometry( CErrObjContainer& errs, BOOL bCheckRoad/* = TRUE*/, BOOL bIsCheckAngle/* = TRUE*/, ISiNDYRule3Ptr ipRule/* = NULL*/ )
{
	//return ::AheIsSafeFeature( ipMap, m_ipFeature, m_piLayer, check, m_strErrMessage, bCheckRoad );
	// 形状に変更があるもののみSimplifyをかける（bug 3519）
	return ::AheIsSafeFeature( m_ipFeature, errs, bCheckRoad, bIsCheckAngle, ipRule );
}

bool CTargetItem::HasZ()
{
	return AheGeoHasZ(GetShape());
}


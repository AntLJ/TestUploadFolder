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
#include "SmartShape.h"

CSmartShape::CSmartShape(void)
{
}

CSmartShape::~CSmartShape(void)
{
}

bool CSmartShape::SetGeometry(const IGeometryPtr& ipGeom)
{
	esriGeometryType esriType = esriGeometryLine;
	ipGeom->get_GeometryType( &esriType );
	if( esriGeometryPolygon != esriType )
	{
		std::cerr << "[ERROR] no support esriGeometryType : " << esriType << std::endl;
		return false;
	}

	IPolygon4Ptr ipPolygon4( ipGeom );
	if( !ipPolygon4 )
	{
		std::cerr << "[ERROR] IPolygon4Ptr is null." << std::endl;
		return false;
	}

	IEnvelopePtr ipEnv;
	ipGeom->get_Envelope( &ipEnv );
	if( ipEnv )
		SetEnvelope( ipEnv );
	else {
		std::cerr << "[ERROR] failed GetEnvelope." << std::endl;
		return false;
	}

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );

	IGeometryBagPtr ipGeomOuterBag;
	ipPolygon4->get_ExteriorRingBag( &ipGeomOuterBag );
	if( !ipGeomOuterBag )
	{
		std::cerr << "[ERROR] failed ExteriorRingBag." << std::endl;
		return false;
	}
	IGeometryCollectionPtr ipOuterCol( ipGeomOuterBag );
	if( !ipOuterCol )
	{
		std::cerr << "[ERROR] failed IGeometryCollecton." << std::endl;
		return false;
	}
	// マルチポリゴン非対応
	IGeometryPtr ipOuterRing;
	ipOuterCol->get_Geometry( 0, &ipOuterRing );
	if( !ipOuterRing )
	{
		std::cerr << "[ERROR] failed outer ring." << std::endl;
		return false;
	}
	m_cOuter.SetOffset( m_dAllowOffset );
	m_cOuter.SetBaseSegment( (IRingPtr)ipOuterRing );

	// インナーループ取得
	IGeometryBagPtr ipGeomInnerBag;
	ipPolygon4->get_InteriorRingBag( (IRingPtr)ipOuterRing, &ipGeomInnerBag );
	if( !ipGeomInnerBag )
	{
		std::cerr << "[ERROR] failed InteriorRingBag." << std::endl;
		return false;
	}
	IGeometryCollectionPtr ipInnerCol( ipGeomInnerBag );
	if( !ipInnerCol )
	{
		std::cerr << "[ERROR] failed IGeometryCollecton." << std::endl;
		return false;
	}
	long lCntRing = 0;
	ipInnerCol->get_GeometryCount( &lCntRing );
	for( long l=0; l<lCntRing; ++l )
	{
		IGeometryPtr ipInnerRing;
		ipInnerCol->get_Geometry( l, &ipInnerRing );
		// インナーループは、リバースする
		((IRingPtr)ipInnerRing)->ReverseOrientation();
		CSmartSegment cSmartSeg;
		cSmartSeg.SetOffset( m_dAllowOffset );
		cSmartSeg.SetBaseSegment( (IRingPtr)ipInnerRing );
		m_listInner.push_back( cSmartSeg );
	}

	return true;
}

bool CSmartShape::ExecShareLine(CSmartShape& cSmart)
{
	// 高速化のため、Envelopeを使って包含関係を見る
	m_lInclude = CheckCompEnvelope( cSmart );
	if( 1 != m_lInclude )
	{
		// ターゲットのアウターと共通部分があるかチェック
		if( _ExecShareLine( cSmart.GetOuterSegment(), cSmart, true ) )
			return true;

		for( std::list<CSmartSegment>::iterator it = cSmart.GetInnerSegments().begin(); it != cSmart.GetInnerSegments().end(); ++it )
		{
			// ターゲットのインナーと共通部分があるかチェック
			if( _ExecShareLine( *it, cSmart, false ) )
				return true;
		}
	}
	else
	{
		// ターゲットポリゴンがベースを包含している場合は
		// ターゲットポリゴンのインナーとベースのアウターが接している可能性が高いため
		// そのチェックを先に行う
		for( std::list<CSmartSegment>::iterator it = cSmart.GetInnerSegments().begin(); it != cSmart.GetInnerSegments().end(); ++it )
		{
			// ターゲットのインナーと共通部分があるかチェック
			if( _ExecShareLine( *it, cSmart, false ) )
				return true;
		}

		// ターゲットのアウターと共通部分があるかチェック
		if( _ExecShareLine( cSmart.GetOuterSegment(), cSmart, true ) )
			return true;
	}

	return false;
}

bool CSmartShape::_ExecShareLine(CSmartSegment& cSmartSegment, CSmartShape& cSmartTarget, bool bIsOuter)
{
	// ターゲットがアウター且つベースが包含している場合は
	// ベースのインナーと共有部分がある可能性が高い
	if( bIsOuter && 1 == m_lInclude )
	{
		for( std::list<CSmartSegment>::iterator it = m_listInner.begin(); it != m_listInner.end(); ++it )
		{
			// パスをリバースする必要がない
			if( __ExecShareLine( *it, cSmartSegment, cSmartTarget, false ) )
				return true;
		}
		// ベースアウターとのチェック
		if( __ExecShareLine( m_cOuter, cSmartSegment, cSmartTarget, bIsOuter ) )
			return true;
	}
	else
	{
		// ベースアウターとのチェック
		if( __ExecShareLine( m_cOuter, cSmartSegment, cSmartTarget, bIsOuter ) )
			return true;

		// お互いがインナーで共有部分はあるはずがない
		if( !bIsOuter )
			return false;

		for( std::list<CSmartSegment>::iterator it = m_listInner.begin(); it != m_listInner.end(); ++it )
		{
			// パスをリバースする必要がない
			if( __ExecShareLine( *it, cSmartSegment, cSmartTarget, false ) )
				return true;
		}
	}

	return false;
}

bool CSmartShape::__ExecShareLine(CSmartSegment& cBase, CSmartSegment& cSegmentTarget, CSmartShape& cSmartTarget, bool bIsOuter)
{
	IPolylinePtr ipBasePolyline = cBase.GetBaseSegment();
	IPolylinePtr ipTargetPolyline = cSegmentTarget.GetBaseSegment();
	IGeometryPtr ipDifferenceBase = ipBasePolyline;
	IGeometryPtr ipDifferenceTarget = ipTargetPolyline;

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipBasePolyline );
	IGeometryPtr ipShareGeom;
	((ITopologicalOperatorPtr)ipBasePolyline)->Intersect( ipTargetPolyline, esriGeometry1Dimension, &ipShareGeom );

	IGeometryCollectionPtr ipGeomCollection( ipShareGeom );
	if( !ipGeomCollection )
		return false;

	long lGeomPart = 0;
	ipGeomCollection->get_GeometryCount( &lGeomPart );
	if( 1 > lGeomPart )
		return false;

	for( long l = 0; l<lGeomPart; ++l )
	{
		IGeometryPtr ipGeomPath;
		ipGeomCollection->get_Geometry( l, &ipGeomPath );

		esriGeometryType esriType = esriGeometryPoint;
		ipGeomPath->get_GeometryType( &esriType );

		long lPathCnt = 0;
		((IPointCollectionPtr)ipGeomPath)->get_PointCount( &lPathCnt );
		if( lPathCnt < 2 )
			continue;

		// 自己交差する形状かどうか先にチェックする
		// 自己交差しない場合はtrue
		IClonePtr ipClonePath = AheGetClone( ipGeomPath );
		((IPathPtr)ipClonePath)->Generalize( m_dAllowOffset );
		bool bGeneralize = ::CheckCrossMyself( ipClonePath );

		// 共有部を取り除いて、ポリラインを保存する
		IGeometryCollectionPtr ipGeomCommon( CLSID_Polyline );
		((IPolylinePtr)ipGeomCommon)->putref_SpatialReference( ipSpRef );
		ipGeomCommon->AddGeometry( ipGeomPath );

		IGeometryPtr ipTmpCommon;
		((ITopologicalOperatorPtr)ipDifferenceBase)->Difference( (IGeometryPtr)ipGeomCommon, &ipTmpCommon );
		if( ipTmpCommon )
		{
			cBase.SetBaseSegment( (IPolylinePtr)ipTmpCommon );
			ipDifferenceBase = ipTmpCommon;
		}
		((ITopologicalOperatorPtr)ipDifferenceTarget)->Difference( (IGeometryPtr)ipGeomCommon, &ipTmpCommon );
		if( ipTmpCommon )
		{
			cSegmentTarget.SetBaseSegment( (IPolylinePtr)ipTmpCommon );
			ipDifferenceTarget = ipTmpCommon;
		}

		// 間引いた形状がSmartShape単位で交差しないかチェックする
		if( bGeneralize )
		{
			if( !CheckCross( ipClonePath, ipGeomPath, bIsOuter ) )
				bGeneralize = false;
			else 
			{
				IClonePtr ipGeomPathRev = AheGetClone( ipGeomPath );
				if( bIsOuter )
				{
					((IPathPtr)ipClonePath)->ReverseOrientation();
					((IPathPtr)ipGeomPathRev)->ReverseOrientation();
				}
				if( !cSmartTarget.CheckCross( ipClonePath, ipGeomPathRev, bIsOuter ) )
					bGeneralize = false;
			}
		}

		CSmartPath cBaseSmart( cSmartTarget.GetOID(), ipGeomPath );
		cBaseSmart.SetGeneralize( bGeneralize );
		cBase.AddSharePath( cBaseSmart );

#if _DEBUG
		IPointPtr ipFrom;
		IPointPtr ipTo;
		((IPathPtr)ipGeomPath)->get_FromPoint( &ipFrom );
		((IPathPtr)ipGeomPath)->get_ToPoint( &ipTo );
		double dFromX = 0;
		double dFromY = 0;
		double dToX = 0;
		double dToY = 0;
		ipFrom->get_X( &dFromX );
		ipFrom->get_Y( &dFromY );
		ipTo->get_X( &dToX );
		ipTo->get_Y( &dToY );
		ATLTRACE( "FROM( %f,%f ), TO( %f,%f)\n", dFromX, dFromY, dToX, dToY );
#endif
		IClonePtr ipClone = AheGetClone( ipGeomPath );
		IPathPtr ipPath( ipClone );
		// お互いアウターの場合、ターゲットはリバースし向きを合わせる
		bool bReverse = false;
		if( bIsOuter )
			bReverse = true;

		CSmartPath cTargetSmart( GetOID(), ipPath );
		cTargetSmart.SetUseReverse( bReverse );
		cTargetSmart.SetGeneralize( bGeneralize );
		cSegmentTarget.AddSharePath( cTargetSmart );
#if _DEBUG
		((IPathPtr)ipPath)->get_FromPoint( &ipFrom );
		((IPathPtr)ipPath)->get_ToPoint( &ipTo );
		ipFrom->get_X( &dFromX );
		ipFrom->get_Y( &dFromY );
		ipTo->get_X( &dToX );
		ipTo->get_Y( &dToY );
		ATLTRACE( "FROM( %f,%f ), TO( %f,%f)\n", dFromX, dFromY, dToX, dToY );
#endif
	}

	return true;
}

IPolygonPtr CSmartShape::GetPolygon()
{
	// アウターループからできるポリゴン
	IPolygonPtr ipOuter = m_cOuter.GetPolygon();
	if( !ipOuter )
		return NULL;

	for( std::list<CSmartSegment>::iterator it = m_listInner.begin();  it != m_listInner.end(); ++it )
	{
		IPolygonPtr ipInner = it->GetPolygon();
		if( !ipInner )
			continue;
		IGeometryPtr ipGeom;
		((ITopologicalOperatorPtr)ipOuter)->Difference( ipInner, &ipGeom );
		if( ipGeom )
			ipOuter = ipGeom;
	}
	return ipOuter;
}

// Envelopeを比較して、処理を高速化する
// -1	: 比較元がターゲットに包含されてしまう
//  0	: 交差している
//  1	: 比較元がターゲットを包含してしまう
long CSmartShape::CheckCompEnvelope(const CSmartShape& cSmart)
{
	IEnvelopePtr ipTargetEnv = cSmart.GetEnvelope();
	if( !( m_ipEnvelope && ipTargetEnv ) )
		return true;

	IPointPtr ipBaseLeft;
	IPointPtr ipTargetLeft;
	IPointPtr ipBaseRight;
	IPointPtr ipTargetRight;

	double dBaseXmin = 0;
	double dBaseYmin = 0;
	double dTargetXmin = 0;
	double dTargetYmin = 0;
	m_ipEnvelope->get_LowerLeft( &ipBaseLeft );
	ipTargetEnv->get_LowerLeft( &ipTargetLeft );
	ipBaseLeft->get_X( &dBaseXmin );
	ipBaseLeft->get_Y( &dBaseYmin );
	ipTargetLeft->get_X( &dTargetXmin );
	ipTargetLeft->get_Y( &dTargetYmin );

	double dBaseXmax = 0;
	double dBaseYmax = 0;
	double dTargetXmax = 0;
	double dTargetYmax = 0;
	m_ipEnvelope->get_UpperRight( &ipBaseRight );
	ipTargetEnv->get_UpperRight( &ipTargetRight );
	ipBaseRight->get_X( &dBaseXmax );
	ipBaseRight->get_Y( &dBaseYmax );
	ipTargetRight->get_X( &dTargetXmax );
	ipTargetRight->get_Y( &dTargetYmax );

	// LowerLeftにて、ターゲットがX・Y方向に小さい
	if( ( dTargetXmin < dBaseXmin ) && ( dTargetYmin < dBaseYmin ) )
	{
		// UpperRightにて、ターゲットがX・Y方向に大きい
		// ターゲットが包含している
		if( ( dBaseXmax < dTargetXmax ) && ( dBaseYmax < dTargetYmax ) )
			return 1;
	}
	else if( ( dBaseXmin < dTargetXmin ) && ( dBaseYmin < dTargetYmin ) )
	{
		if( ( dTargetXmax < dBaseXmax ) && ( dTargetYmax < dBaseYmax ) )
			return -1;
	}

	return 0;
}

// 自己交差する共有ラインを取得する
bool CSmartShape::CheckCrossMyself(std::list<CSmartPath>& listSmart)
{
	m_cOuter.CheckCrossMyself( listSmart );

	for( std::list<CSmartSegment>::iterator it = m_listInner.begin(); it != m_listInner.end(); ++it )
	{
		it->CheckCrossMyself( listSmart );
	}

	return listSmart.empty();
}

bool CSmartShape::CheckCross(const IGeometryPtr& ipGeom, const IGeometryPtr& ipOrgGeom, bool bOuter)
{
	// Ringが裏返っていたら、間引かない
	if( bOuter )
	{
		if( !m_cOuter.CheckRing( ipGeom ) )
			return false;
	}

	if( !m_cOuter.CheckCross( ipGeom, ipOrgGeom ) )
		return false;

	for( std::list<CSmartSegment>::iterator it = m_listInner.begin(); it != m_listInner.end(); ++it )
	{
		if( !bOuter )
		{
			if( !it->CheckRing( ipGeom ) )
				return false;
		}
		if( !it->CheckCross( ipGeom, ipOrgGeom ) )
			return false;
	}	

	return true;
}

bool CSmartShape::SetGeneralize(bool bDo, long lOID, const CSmartPath& cSmart)
{
	if( m_cOuter.SetGeneralize( bDo, lOID, cSmart ) )
		return true;

	for( std::list<CSmartSegment>::iterator it = m_listInner.begin(); it != m_listInner.end(); ++it )
	{
		if( it->SetGeneralize( bDo, lOID, cSmart ) )
			return true;
	}
	return false;
}

bool CSmartShape::ChangeAllSegment(IFeatureClassPtr ipFeatureClass /*=NULL*/)
{
	m_cOuter.ChangeAllSegment( ipFeatureClass );

	for( std::list<CSmartSegment>::iterator it = m_listInner.begin(); it != m_listInner.end(); ++it )
	{
		if(! it->ChangeAllSegment( ipFeatureClass ))
			return false;
	}

	return true;
}


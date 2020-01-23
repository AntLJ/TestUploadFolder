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
#include "SmartSegment.h"

CSmartSegment::CSmartSegment(void)
{
}

CSmartSegment::~CSmartSegment(void)
{
}

void CSmartSegment::SetBaseSegment(const IPolylinePtr& ipPolyline)
{
	if( !ipPolyline )
		return ;

	IClonePtr ipClone = AheGetClone( ipPolyline );
	m_ipPolyline = ipPolyline;
}

void CSmartSegment::SetBaseSegment(const IRingPtr& ipRing)
{
	if( !ipRing )
		return;

	IClonePtr ipClone = AheGetClone( ipRing );
	IPointCollectionPtr ipPolyline( CLSID_Polyline );
	((IPolylinePtr)ipPolyline)->putref_SpatialReference( AheGetSpatialReference( ipRing ) );

	IPathPtr ipPath( ipClone );
	IPointCollectionPtr ipCol( ipClone );
	ipPolyline->AddPointCollection( ipCol );

	m_ipPolyline = (IPolylinePtr)ipPolyline;
}

bool CSmartSegment::ChangeAllSegment(IFeatureClassPtr ipFeatureClass /*=NULL*/)
{
	if( !m_ipPolyline )
		return true;

	IClonePtr ipPolyClone = AheGetClone( m_ipPolyline );
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( m_ipPolyline );
	IGeometryCollectionPtr ipGeomCollection( ipPolyClone );
	if( !ipGeomCollection )
	{
		std::cerr << "[ERROR] failed GeomCollection at ChangeAllSegment." << std::endl;
		return false;
	}
	
	long lGeomCnt = 0;
	ipGeomCollection->get_GeometryCount( &lGeomCnt );
	for( long l=0; l<lGeomCnt; ++l )
	{
		IGeometryPtr ipGeom;
		ipGeomCollection->get_Geometry( l, &ipGeom );

		IGeometryPtr ipDiff;
		IGeometryCollectionPtr ipDiffGeom( CLSID_Polyline );
		((IPolylinePtr)ipDiffGeom)->putref_SpatialReference( ipSpRef );
		ipDiffGeom->AddGeometry( ipGeom );
		((ITopologicalOperatorPtr)ipPolyClone)->Difference( (IGeometryPtr)ipDiffGeom, &ipDiff );
		if( ipDiff )
			ipPolyClone = ipDiff;

		CSmartPath cSmart( 0, ipGeom );
		IClonePtr ipClone = AheGetClone( ipGeom );
		((IPathPtr)ipClone)->Generalize( m_dAllowOffset );
		bool bGeneralize = ::CheckCrossMyself( ipClone );
		if( bGeneralize )
			bGeneralize = CheckCross( ipClone, ipGeom );
		if( bGeneralize && ipFeatureClass )
			bGeneralize = CheckIntersectGeom( ipFeatureClass, ipClone );

		cSmart.SetGeneralize( bGeneralize );
		m_listSharePath.push_back( cSmart );
	}

	m_ipPolyline = NULL;

	return true;
}

IPolygonPtr CSmartSegment::GetPolygon()
{
	if( !ChangeAllSegment() )
		return NULL;

	if( m_listSharePath.empty() )
		return NULL;

	std::list<CSmartPath>::iterator it = m_listSharePath.begin();
	
	IPathPtr ipMainPath = it->GetPath();
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipMainPath );
	IPointCollectionPtr ipPointCol( CLSID_Polygon );
	((IPolygonPtr)ipPointCol)->putref_SpatialReference( ipSpRef );

	IClonePtr ipClone = AheGetClone( ipMainPath );
	if( it->GetGeneralize() )
		ipMainPath->Generalize( m_dAllowOffset );
	if( it->GetUseReverse() )
		ipMainPath->ReverseOrientation();

	IPointCollectionPtr ipTmp( ipMainPath );
	long lCntPoint = 0;
	ipTmp->get_PointCount( &lCntPoint );
	for( long l=0; l<lCntPoint; ++l )
	{
		IPointPtr ipTmpPoint;
		ipTmp->get_Point( l, &ipTmpPoint );
		ipPointCol->AddPoint( ipTmpPoint );
	}

	IPointPtr ipStart;
	ipMainPath->get_FromPoint( &ipStart );
	IPointPtr ipPoint;
	ipMainPath->get_ToPoint( &ipPoint );

	m_listSharePath.erase( it );
	for( it = m_listSharePath.begin(); it != m_listSharePath.end(); )
	{
		IPathPtr ipAddPath = it->GetPath();
		IPointPtr ipFrom;

		// ���o�[�X�̕K�v������΁ATo���ڑ��_�ɂȂ�
		if( it->GetUseReverse() )
			ipAddPath->get_ToPoint( &ipFrom );
		else
			ipAddPath->get_FromPoint( &ipFrom );

		if( AheIsEqual( ipPoint, ipFrom ) )
		{
			if( it->GetGeneralize() )
				ipAddPath->Generalize( m_dAllowOffset );
			if( it->GetUseReverse() )
				ipAddPath->ReverseOrientation();

			IPointCollectionPtr ipTmp( ipAddPath );
			long lCntPoint = 0;
			ipTmp->get_PointCount( &lCntPoint );
			for( long l=1; l<lCntPoint; ++l )
			{
				IPointPtr ipTmpPoint;
				ipTmp->get_Point( l, &ipTmpPoint );
				ipPointCol->AddPoint( ipTmpPoint );
			}

			ipAddPath->get_ToPoint( &ipPoint );
			m_listSharePath.erase( it );
			it = m_listSharePath.begin();
			if( AheIsEqual( ipStart, ipPoint ) )
			{
				ipPointCol->AddPoint( ipPoint );
				break;
			}
			continue;
		}
		++it;
	}

	IPolygonPtr ipPolygon( ipPointCol );
	if( !ipPolygon )
		return NULL;

	ipPolygon->SimplifyPreserveFromTo();

	VARIANT_BOOL vBool = VARIANT_FALSE;
	ipPolygon->get_IsClosed( &vBool );
	if( vBool )
		return ipPolygon;

	return NULL;
}

bool CSmartSegment::CheckCrossMyself(std::list<CSmartPath>& listSmart)
{
	std::list<CSmartPath> listTmpSmart = m_listSharePath;

	IGeometryCollectionPtr ipGeomCollection( m_ipPolyline );
	if( ipGeomCollection )
	{
		long lGeomCnt = 0;
		ipGeomCollection->get_GeometryCount( &lGeomCnt );
		// �אڂ��Ȃ����A�܂��אڂ��������Ă��Ȃ��`��
		for( long l=0; l<lGeomCnt; ++l )
		{
			IGeometryPtr ipTmpGeom;
			ipGeomCollection->get_Geometry( l, &ipTmpGeom );
			ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipTmpGeom );
			IGeometryCollectionPtr ipBaseGeomCol( CLSID_Polyline );
			((IPolylinePtr)ipBaseGeomCol)->putref_SpatialReference( ipSpRef );
			ipBaseGeomCol->AddGeometry( ipTmpGeom );

			for( std::list<CSmartPath>::iterator it = m_listSharePath.begin(); it != m_listSharePath.end(); ++it )
			{
				if( !it->GetGeneralize() )
					continue;

				IGeometryCollectionPtr ipGeomCol( CLSID_Polyline );
				IPathPtr ipPath = AheGetClone( it->GetPath() );
				((IPolylinePtr)ipGeomCol)->putref_SpatialReference( ipSpRef );
				ipPath->Generalize( m_dAllowOffset );
				ipGeomCol->AddGeometry( ipPath );

				if( AheIsCross( (IGeometryPtr)ipBaseGeomCol, (IGeometryPtr)ipGeomCol, ipSpRef ) )
				{
					it->SetGeneralize( false );
					listSmart.push_back( *it );
				}
			}
		}
	}
	// �ePath���������Ȃ����ǂ����`�F�b�N����
	for( std::list<CSmartPath>::iterator itBase = m_listSharePath.begin(); itBase != m_listSharePath.end(); ++itBase )
	{
		IGeometryCollectionPtr ipBaseGeom( CLSID_Polyline );
		IPathPtr ipBasePath = AheGetClone( itBase->GetPath() );
		ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipBasePath );
		((IPolylinePtr)ipBaseGeom)->putref_SpatialReference( ipSpRef );
		if( itBase->GetGeneralize() )
			ipBasePath->Generalize( m_dAllowOffset );
		if( itBase->GetUseReverse() )
			ipBasePath->ReverseOrientation();

		ipBaseGeom->AddGeometry( ipBasePath );

		std::list<CSmartPath>::iterator itTarget = itBase;
		++itTarget;
		for( ; itTarget != m_listSharePath.end(); ++itTarget )
		{
			// �ǂ�����Ԉ����Ȃ��ꍇ�̓X���[
			if( !( itBase->GetGeneralize() || itTarget->GetGeneralize() ) )
				continue;

			IGeometryCollectionPtr ipTargetGeom( CLSID_Polyline );
			((IPolylinePtr)ipTargetGeom)->putref_SpatialReference( ipSpRef );
			IPathPtr ipTargetPath = AheGetClone( itTarget->GetPath() );
			if( itTarget->GetGeneralize() )
				ipTargetPath->Generalize( m_dAllowOffset );
			if( itTarget->GetUseReverse() )
				ipTargetPath->ReverseOrientation();

			ipTargetGeom->AddGeometry( ipTargetPath );

			if( AheIsCross( (IGeometryPtr)ipBaseGeom, (IGeometryPtr)ipTargetGeom, ipSpRef ) )
			{
				if( itBase->GetGeneralize() )
				{
					itBase->SetGeneralize( false );
					listSmart.push_back( *itBase );
				}
				if( itTarget->GetGeneralize() )
				{
					itTarget->SetGeneralize( false );
					listSmart.push_back( *itTarget );
				}
			}
		}
	}

	return listSmart.empty();
}

bool CSmartSegment::CheckCross(const IGeometryPtr& ipGeom, const IGeometryPtr& ipOrgGeom)
{
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );
	if( !ipSpRef )
		return false;

	IGeometryCollectionPtr ipMainGeom( CLSID_Polyline );
	((IPolylinePtr)ipMainGeom)->putref_SpatialReference( ipSpRef );
	ipMainGeom->AddGeometry( ipGeom );

	// ���C���`�󂪌������Ȃ����A��щz���ċt�ɂȂ�(�|���S�������ɂȂ�)�ꍇ���`�F�b�N���邽��
	// 1�ԖڃZ�O�����g�Ɛڑ��Z�O�����g�̊O�ς����A�������Ⴄ�ꍇ�͔�щz�����Ɣ��f����
	ISegmentCollectionPtr ipSegColBase( ipGeom );
	_ISegmentPtr ipSegBase;
	ipSegColBase->get_Segment( 0, &ipSegBase );
	ISegmentCollectionPtr ipSegColOrg( ipOrgGeom );
	_ISegmentPtr ipSegOrg;
	ipSegColOrg->get_Segment( 0, &ipSegOrg );
	//

	IGeometryCollectionPtr ipGeomCollection( m_ipPolyline );
	if( ipGeomCollection )
	{
		long lGeomCnt = 0;
		ipGeomCollection->get_GeometryCount( &lGeomCnt );
		// �אڂ��Ȃ����A�܂��אڂ��������Ă��Ȃ��`��
		for( long l=0; l<lGeomCnt; ++l )
		{
			IGeometryPtr ipTmpGeom;
			ipGeomCollection->get_Geometry( l, &ipTmpGeom );
			IGeometryCollectionPtr ipBaseGeomCol( CLSID_Polyline );
			((IPolylinePtr)ipBaseGeomCol)->putref_SpatialReference( ipSpRef );
			ipBaseGeomCol->AddGeometry( ipTmpGeom );

			if( AheIsCross( (IGeometryPtr)ipMainGeom, (IGeometryPtr)ipBaseGeomCol, ipSpRef ) )
				return false;
			// �^�[���̕������ς��ꍇ�͎��Ȍ����Ƃ݂Ȃ�
			if( !CheckTurnDir( (IPolylinePtr)ipBaseGeomCol, ipSegBase, ipSegOrg ) )
				return false;
			// �I�[�o�[���b�v�����ꍇ�ŁAEquals�ł����OK
			if( AheIsOverlap( (IGeometryPtr)ipMainGeom, (IGeometryPtr)ipBaseGeomCol, ipSpRef ) )
			{
				if( !AheIsEqual( (IGeometryPtr)ipMainGeom, (IGeometryPtr)ipBaseGeomCol, ipSpRef ) )
					return false;
			}
		}
	}
	// �ePath���������Ȃ����ǂ����`�F�b�N����
	for( std::list<CSmartPath>::iterator itBase = m_listSharePath.begin(); itBase != m_listSharePath.end(); ++itBase )
	{
		IGeometryCollectionPtr ipBaseGeom( CLSID_Polyline );
		IPathPtr ipBasePath = AheGetClone( itBase->GetPath() );
		((IPolylinePtr)ipBaseGeom)->putref_SpatialReference( ipSpRef );
		if( itBase->GetGeneralize() )
			ipBasePath->Generalize( m_dAllowOffset );
		if( itBase->GetUseReverse() )
			ipBasePath->ReverseOrientation();

		ipBaseGeom->AddGeometry( ipBasePath );

		// �`�󂪃N���X���Ă���ꍇ�͎��Ȍ���
		if( AheIsCross( (IGeometryPtr)ipMainGeom, (IGeometryPtr)ipBaseGeom, ipSpRef ) )
			return false;
		// �O�ς��g���āA�������ς��ꍇ�͗��Ԃ邽�߁A���Ȍ����Ɣ��f����
		if( !CheckTurnDir( (IPolylinePtr)ipBaseGeom, ipSegBase, ipSegOrg ) )
			return false;
		// �I�[�o�[���b�v�����ꍇ�ŁAEquals�ł����OK
		if( AheIsOverlap( (IGeometryPtr)ipMainGeom, (IGeometryPtr)ipBaseGeom, ipSpRef ) )
		{
			// ����`��ł���΍폜�ɂȂ邽�߁A�Ԉ���OK
			if( !AheIsEqual( (IGeometryPtr)ipMainGeom, (IGeometryPtr)ipBaseGeom, ipSpRef ) )
				return false;
		}
	}

	return true;
}

bool CSmartSegment::CheckIntersectGeom(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom)
{
	if( !ipFeatureClass )
		return true;

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );
	IGeometryCollectionPtr ipGeomCol( CLSID_Polyline );
	((IPolylinePtr)ipGeomCol)->putref_SpatialReference( ipSpRef );
	ipGeomCol->AddGeometry( ipGeom );
	
	ISpatialFilterPtr ipSpFilter = AheGetSpatialFilter( ipGeomCol, ipFeatureClass );
	if( !ipSpFilter )
		return false;

	IFeatureCursorPtr ipCursor;
	ipFeatureClass->Search( ipSpFilter, VARIANT_FALSE, &ipCursor );
	if( !ipCursor )
		return true;

	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		long lOID = 0;
		ipFeature->get_OID( &lOID );
		return false;
	}
	return true;
}

bool CSmartSegment::SetGeneralize(bool bDo, long lOID, const CSmartPath& cSmart)
{
	IPointPtr ipFrom;
	IPointPtr ipTo;
	cSmart.GetPath()->get_FromPoint( &ipFrom );
	cSmart.GetPath()->get_ToPoint( &ipTo );

#if _DEBUG
	double dFromX = 0;
	double dFromY = 0;
	double dToX = 0;
	double dToY = 0;
	ipFrom->get_X( &dFromX );
	ipFrom->get_Y( &dFromY );
	ipTo->get_X( &dToX );
	ipTo->get_Y( &dToY );
	ATLTRACE( "FROM( %f,%f ), TO( %f,%f )\n", dFromX, dFromY, dToX, dToY );
#endif

	for( std::list<CSmartPath>::iterator it = m_listSharePath.begin(); it != m_listSharePath.end(); ++it )
	{
		if( it->GetOID() != lOID )
			continue;

		IPointPtr ipBaseFrom;
		IPointPtr ipBaseTo;

		it->GetPath()->get_FromPoint( &ipBaseFrom );
		it->GetPath()->get_ToPoint( &ipBaseTo );

#if _DEBUG
		ipBaseFrom->get_X( &dFromX );
		ipBaseFrom->get_Y( &dFromY );
		ipBaseTo->get_X( &dToX );
		ipBaseTo->get_Y( &dToY );
		ATLTRACE( "FROM( %f,%f ), TO( %f,%f )\n", dFromX, dFromY, dToX, dToY );
#endif
		// �p�X��FromTo�͓����ꍇ�Ƌt�̏ꍇ����
		if( ( AheIsEqual( ipFrom, ipBaseFrom ) && AheIsEqual( ipBaseTo, ipTo ) )
		 ||	( AheIsEqual( ipFrom, ipBaseTo ) && AheIsEqual( ipTo, ipBaseFrom ) ) )
		{
			it->SetGeneralize( bDo );
			return true;
		}
	}
	return false;
}

// �Ԉ����O�`��Ɛڑ��Z�O�����g�̊O�ρA�Ԉ�����`��Ɛڑ��Z�O�����g�̊O�ς�
// �������Ⴄ�ꍇ�́A�t�T�C�h�ɊԈ�����Ă���̂Ō������Ă���Ɣ��f����
bool CSmartSegment::CheckTurnDir(const IPolylinePtr& ipTarget, const _ISegmentPtr& ipBaseSeg, const _ISegmentPtr& ipOrgSeg)
{
	IPointPtr ipFrom;
	ipBaseSeg->get_FromPoint( &ipFrom );

	ISegmentCollectionPtr ipTargetCol( ipTarget );
	long lSegCnt = 0;
	ipTargetCol->get_SegmentCount( &lSegCnt );
	_ISegmentPtr ipTargetSeg;

	IPointPtr ipBaseTo;
	IPointPtr ipOrgTo;
	ipBaseSeg->get_ToPoint( &ipBaseTo );
	ipOrgSeg->get_ToPoint( &ipOrgTo );
	double dxF = 0;
	double dyF = 0;
	double dx1 = 0;		
	double dy1 = 0;
	double vec1[2] = {0,0};	// �Ԉ�����̌`��x�N�g��
	double vec2[2] = {0,0}; // �Ԉ����O�̌`��x�N�g��
	double dx2 = 0;		
	double dy2 = 0;
	ipFrom->get_X( &dxF );
	ipFrom->get_Y( &dyF );
	ipBaseTo->get_X( &dx1 );
	ipBaseTo->get_Y( &dy1 );
	ipOrgTo->get_X( &dx2 );
	ipOrgTo->get_Y( &dy2 );
	vec1[0] = dx1 - dxF;
	vec1[1] = dy1 - dyF;
	vec2[0] = dx2 - dxF;
	vec2[1] = dy2 - dyF;

	double dxTarget = 0;		
	double dyTarget = 0;
	double vecTarget[2] = {0,0}; // �����Ώۂ̌`��x�N�g��
	for( long l=0; l<lSegCnt; ++l )
	{
		_ISegmentPtr ipSeg;
		ipTargetCol->get_Segment( l, &ipSeg );
		IPointPtr ipFromPoint;
		IPointPtr ipToPoint;
		ipSeg->get_FromPoint( &ipFromPoint );
		ipSeg->get_ToPoint( &ipToPoint );
		VARIANT_BOOL vBool = VARIANT_TRUE;
		((IRelationalOperatorPtr)ipFromPoint)->Equals( ipFrom, &vBool );
		if( vBool )
		{
			ipTargetSeg = ipSeg;
			ipToPoint->get_X( &dxTarget );
			ipToPoint->get_Y( &dyTarget );
			vecTarget[0] = dxTarget - dxF;
			vecTarget[1] = dyTarget - dyF;
			break;
		}
		((IRelationalOperatorPtr)ipToPoint)->Equals( ipFrom, &vBool );
		if( vBool )
		{
			ipTargetSeg = ipSeg;
			ipFromPoint->get_X( &dxTarget );
			ipFromPoint->get_Y( &dyTarget );
			vecTarget[0] = dxTarget - dxF;
			vecTarget[1] = dyTarget - dyF;
			break;
		}
	}
	if( !ipTargetSeg )
		return true;

	double dNaiseki1 = ( vec1[0] * vecTarget[0] ) + ( vec1[1] * vecTarget[1] );
	double dNaiseki2 = ( vec2[0] * vecTarget[0] ) + ( vec2[1] * vecTarget[1] );
	// ���ς����ł���΁A���������������Ă��邽�߂ɁA�O�σ`�F�b�N���K�v
	if( dNaiseki1 < 0 && dNaiseki2 < 0)
		return true;

	double dGaiseki1 = ( vec1[0] * vecTarget[1] ) - ( vecTarget[0] * vec1[1] );
	double dGaiseki2 = ( vec2[0] * vecTarget[1] ) - ( vecTarget[0] * vec2[1] );

	// �������ł����OK
	if( dGaiseki1 * dGaiseki2 >= 0 )
		return true;

	return false;
}

// �����̌`����g����Ring���쐬���A�\���̔��������
bool CSmartSegment::CheckRing(const IGeometryPtr& ipGeom)
{
	IPathPtr ipPath = ipGeom;
	VARIANT_BOOL vBool = VARIANT_FALSE;
	// ���Ă�����AOK
	ipPath->get_IsClosed( &vBool );
	if( vBool )
		return true;

	IPointPtr ipStart;
	ipPath->get_FromPoint( &ipStart );

	IPointPtr ipTo;
	ipPath->get_ToPoint( &ipTo );

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );
	IPointCollectionPtr ipMainRing( CLSID_Ring );
	((IRingPtr)ipMainRing)->putref_SpatialReference( ipSpRef );

	IPointCollectionPtr ipStartCol( ipGeom );
	long lCnt = 0;
	ipStartCol->get_PointCount( &lCnt );
	for( long l=0; l<lCnt; ++l )
	{
		IPointPtr ipPoint;
		ipStartCol->get_Point( l, &ipPoint );
		ipMainRing->AddPoint( ipPoint );
	}

	std::list<CAdapt<IPathPtr>> listPath;
	GetAllPath( listPath );

	while( !listPath.empty() )
	{
		bool bIsConnect = false;
		for( std::list<CAdapt<IPathPtr>>::iterator it = listPath.begin(); it != listPath.end(); ++it )
		{
			IPointPtr ipFP;
			IPointPtr ipTP;
			IPathPtr ipTmpPath = (IPathPtr)AheGetClone( it->m_T );
			ipTmpPath->get_FromPoint( &ipFP );
			ipTmpPath->get_ToPoint( &ipTP );
#if _DEBUG
			double dxFP = 0;
			double dyFP = 0;
			double dxTP = 0;
			double dyTP = 0;
			double dxTo = 0;
			double dyTo = 0;
			ipFP->get_X( &dxFP );
			ipFP->get_Y( &dyFP );
			ipTP->get_X( &dxTP );
			ipTP->get_Y( &dyTP );
			ipTo->get_X( &dxTo );
			ipTo->get_Y( &dyTo );
			ATLTRACE( "To( %f, %f ), FP( %f, %f ), TP( %f, %f )\n", dxTo, dyTo, dxFP, dyFP, dxTP, dyTP );
#endif
			// To���ƒ[�_���������ꍇ�́A���o�[�X����
			if( AheIsEqual( ipTo, ipTP ) )
			{
				ipTmpPath->ReverseOrientation();
				IPointPtr ipTmp = ipTP;
				ipTP = ipFP;
				ipFP = ipTmp;
			}

			if( AheIsEqual( ipTo, ipFP ) )
			{
				IPointCollectionPtr ipAddPath( ipTmpPath );
				long lCnt = 0;
				ipAddPath->get_PointCount( &lCnt );
				for( long l=0; l<lCnt; ++l )
				{
					IPointPtr ipAddP;
					ipAddPath->get_Point( l, &ipAddP );
					ipMainRing->AddPoint( ipAddP );
				}
				ipTo = ipTP;
				listPath.erase( it );
				bIsConnect = true;		// �ڑ�����p�X��������
				break;
			}
		}
		// �ڑ��p�X���Ȃ��ꍇ�́A�Ⴄ���[�v�ł���
		if( !bIsConnect )
			return true;
	}
	((IRingPtr)ipMainRing)->get_IsClosed( &vBool );
	if( !vBool )
		((IRingPtr)ipMainRing)->Close();

	((IRingPtr)ipMainRing)->get_IsExterior( &vBool );
	if( vBool )
		return true;

	return false;
}

void CSmartSegment::GetAllPath(std::list<CAdapt<IPathPtr>>& listPath)
{
	IGeometryCollectionPtr ipPolyCol( m_ipPolyline );
	if( ipPolyCol )
	{
		long lCnt = 0;
		ipPolyCol->get_GeometryCount( &lCnt );

		for( long l=0; l<lCnt; ++l )
		{
			IGeometryPtr ipGeom;
			ipPolyCol->get_Geometry( l, &ipGeom );
			listPath.push_back( (IPathPtr)ipGeom );
		}
	}

	for( std::list<CSmartPath>::iterator it = m_listSharePath.begin(); it != m_listSharePath.end(); ++it )
	{
		IPathPtr ipPath = (IPathPtr)AheGetClone( it->GetPath() );
		if( it->GetGeneralize() )
			ipPath->Generalize( m_dAllowOffset );
		if( it->GetUseReverse() )
			ipPath->ReverseOrientation();

		listPath.push_back( ipPath );
	}
}

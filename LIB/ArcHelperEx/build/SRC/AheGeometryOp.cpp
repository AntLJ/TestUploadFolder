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
#include "AheGeometryOp.h"
#include "AheGlobals.h"
#include "GlobalFunctions.h"
#include "math2.h"
#include "WKSHelper.h"
#include "AheBugEvasion.h"
#include "AheMeshUtil.h"
#include "AheGeometryConvert.h"
#include "sde_ondemand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_FAIL_GET_WKSIZE  = _T("IWkb::get_WkbSize�Ɏ��s");
	LPCTSTR ERROR_FAIL_REPAIR      = _T("�C���Ɏ��s���܂����B");
	LPCTSTR ERROR_GEO_IS_NOT_FIXED = _T("�W�I���g���̏C�����s");

#else
	LPCTSTR ERROR_FAIL_GET_WKSIZE  = _T("Failed to invoke IWkb::get_WkbSize");
	LPCTSTR ERROR_FAIL_REPAIR      = _T("Failed to be repaired.");
	LPCTSTR ERROR_GEO_IS_NOT_FIXED = _T("The geometry could not be fixed.");

#endif // SINDY_FOR_ENGLISH
}


// �G���A�|�C���g���擾����
IPointPtr AheGetAreaPoint( IGeometry* ipGeom )
{
	WKSPoint p = {-1, -1};

	IAreaPtr ipArea( ipGeom );
	IPointPtr ipPoint( ipGeom );
	if( ipArea )
		ipArea->get_LabelPoint( &ipPoint );

	if( ipPoint )
		ipPoint->QueryCoords( &p.X, &p.Y );

	return ipPoint;
}

// �n�_���擾����
IPointPtr AheGetFromPoint( IGeometry* ipGeom )
{
	ICurvePtr ipCurve( ipGeom );
	if( ! ipCurve )
	  return NULL;

	IPointPtr ret; // �߂�l
	ipCurve->get_FromPoint( &ret );
	return ret;
}

// �I�_���擾����
IPointPtr AheGetToPoint( IGeometry* ipGeom )
{
	ICurvePtr ipCurve( ipGeom );
	if( ! ipCurve )
	  return NULL;

	IPointPtr ret; // �߂�l
	ipCurve->get_ToPoint( &ret );
	return ret;
}

// �[�_���m�̐ڑ��_���擾����
IPointPtr AheGetConnectPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 )
{
	IPointPtr ipFromPoint1( AheGetFromPoint( ipGeom1 ) );
	IPointPtr ipFromPoint2( AheGetFromPoint( ipGeom2 ) );
	IPointPtr ipToPoint1( AheGetToPoint( ipGeom1 ) );
	IPointPtr ipToPoint2( AheGetToPoint( ipGeom2 ) );

	if( AheIsEqual( ipFromPoint1, ipFromPoint2 ) ||
		AheIsEqual( ipFromPoint1, ipToPoint2 ) )
		return ipFromPoint1;
	if( AheIsEqual( ipToPoint1, ipFromPoint2 ) ||
		AheIsEqual( ipToPoint1, ipToPoint2 ) )
		return ipToPoint1;

	return IPointPtr();
}

// WKSPoint���쐬����
WKSPoint MakeWKSPoint( IPoint* ipPoint )
{
	WKSPoint p = {-1.0,-1.0}; // �߂�l
	if( ipPoint )
		ipPoint->QueryCoords( &p.X, &p.Y );

	return p;
}

// WKSPoint���쐬����
WKSPoint MakeWKSPoint( const double& x, const double& y )
{
	WKSPoint p = { x, y };
	return p;
}

/////////////////////////////////////////////////////////////////////////////
// �W�I���g���ϊ���
/////////////////////////////////////////////////////////////////////////////
// IEnvelope -> IPolygon �ϊ�
IPolygonPtr AheEnvelope2Polygon( IEnvelopePtr ipEnv )
{
	// �����`�F�b�N
	if( ipEnv == NULL )
	{
		_ASSERTE( ipEnv != NULL );
		return NULL;
	}
	ISpatialReferencePtr ipSpRef;
	HRESULT hr = ipEnv->get_SpatialReference( &ipSpRef );

	IPointCollectionPtr ipPolygon( CLSID_Polygon );
	((IGeometryPtr)ipPolygon)->putref_SpatialReference( AheGetSpatialReference( ipEnv ) ); // ��ԎQ�Ƃ�t�^ 
	IPointPtr ipUpperLeft, ipUpperRight, ipLowerLeft, ipLowerRight;

	ipEnv->get_UpperLeft( &ipUpperLeft );
	ipEnv->get_UpperRight( &ipUpperRight );
	ipEnv->get_LowerLeft( &ipLowerLeft );
	ipEnv->get_LowerRight( &ipLowerRight );

	if( ipPolygon != NULL )
	{
		ipPolygon->AddPoint( ipUpperLeft );
		ipPolygon->AddPoint( ipUpperRight );
		ipPolygon->AddPoint( ipLowerRight );
		ipPolygon->AddPoint( ipLowerLeft );
		ipPolygon->AddPoint( ipUpperLeft );
	}
	
	_ASSERTE( ipPolygon != NULL );

	// [Bug 5041]ipPolygon�͋�ԎQ�Ƃ��������ĂȂ��̂ł��ĂĂ���
	IPolygonPtr ipRetPolygon( ipPolygon );
	if( ipRetPolygon != NULL )
		hr = ipRetPolygon->putref_SpatialReference( ipSpRef );
	return ipRetPolygon;
}

// �}���`�|���S������|���S���̃Z�b�g���擾����
_ISetPtr AheConvertMultiPolygon2Polygons( IPolygonPtr ipPolygon )
{
	if( ipPolygon == NULL ) return NULL;

	// ��ԎQ�Ǝ擾
	ISpatialReferencePtr ipSpRef;
	ipPolygon->get_SpatialReference( &ipSpRef );

	IPointCollectionPtr ipPointCol( ipPolygon );
	if( ipPointCol != NULL )
	{
		LONG lPointCount = -1;
		ipPointCol->get_PointCount( &lPointCount );
		WKSPoint* pPoints = new WKSPoint[lPointCount];
		ipPointCol->QueryWKSPoints( 0, lPointCount, pPoints );

		// �e�����O�ɕ�������
		std::list<RING> listRings;
		InitRing( lPointCount, pPoints, listRings );
		// ��܊֌W���`�F�b�N
		CheckExteriorInterior( pPoints, listRings );

		// �e�|���S���ɕ�������
		_ISetPtr ipRetSet( CLSID_Set );
		if( listRings.size() < 2 )
		{
			ipRetSet->Add( AheGetClone(ipPolygon) );
			delete [] pPoints;
			return ipRetSet;
		}
		else {
			for( std::list<RING>::iterator it = listRings.begin(); it != listRings.end(); it++ )
			{
				IGeometryCollectionPtr ipMultiPolygon( CLSID_Polygon );	// ����Ɋi�[����
				((IGeometryPtr)ipMultiPolygon)->putref_SpatialReference( AheGetSpatialReference( ipPolygon ) ); // ��ԎQ�ƕt�^
				// Exterior �̂�
				if( it->IsClockwise == FALSE ) continue;

				// CROSS��WITHIN�������Ă͂����Ȃ��I�I
				if( it->Cross.size() > 0 || it->Within.size() > 0 )
				{
					_ASSERTE( it->Cross.size() < 1 && it->Within.size() < 1 );
					delete [] pPoints;
					return NULL;
				}
				
				// Exterior
				LONG lExPartPointsCount = it->END - it->START + 1;
				WKSPoint* pExPartPoints = new WKSPoint[lExPartPointsCount];
				// �_����u���b�N�P�ʂŃR�s�[
				::CopyMemory( &(pExPartPoints[0]), &(pPoints[it->START]), sizeof(WKSPoint) * lExPartPointsCount );
				// �����O���쐬����
				IPointCollectionPtr ipExRingCol( CLSID_Ring );
				((IGeometryPtr)ipExRingCol)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
				ipExRingCol->SetWKSPoints( lExPartPointsCount, pExPartPoints );
				IRingPtr ipExRing( ipExRingCol );
				ipExRing->putref_SpatialReference( ipSpRef );

				// �}���`�|���S���Ƃ��Ċi�[����
				ipMultiPolygon->AddGeometry( ipExRing );

				delete [] pExPartPoints;

				// Interior
				for( std::list<SIMPLERING>::iterator itIn = it->Contain.begin(); itIn != it->Contain.end(); itIn++ )
				{
					LONG lInPartPointsCount = itIn->END - itIn->START + 1;
					WKSPoint* pInPartPoints = new WKSPoint[lInPartPointsCount];
					// �_����u���b�N�P�ʂŃR�s�[
					::CopyMemory( &(pInPartPoints[0]), &(pPoints[itIn->START]), sizeof(WKSPoint) * lInPartPointsCount );
					// �����O���쐬����
					IPointCollectionPtr ipInRingCol( CLSID_Ring );
					((IGeometryPtr)ipInRingCol)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
					ipInRingCol->SetWKSPoints( lInPartPointsCount, pInPartPoints );
					IRingPtr ipInRing( ipInRingCol );
					ipInRing->putref_SpatialReference( ipSpRef );

					// �}���`�|���S���Ƃ��Ċi�[����
					ipMultiPolygon->AddGeometry( ipInRing );

					delete [] pInPartPoints;
				}
				IPolygonPtr ipRetPolygon( ipMultiPolygon );
/*#ifdef DEBUG
				AheAddPolygonElement( NULL, ipRetPolygon, RGB(0,255,0), esriSFSSolid );
#endif // ifdef DEBUG*/
				ipRetPolygon->putref_SpatialReference( ipSpRef );
				ipRetPolygon->SimplifyPreserveFromTo();
/*#ifdef DEBUG
				AheAddPolygonElement( NULL, ipRetPolygon, RGB(255,0,0), esriSFSSolid );
#endif // ifdef DEBUG*/
				// �Z�b�g�ɒǉ�
				ipRetSet->Add( ipRetPolygon );
			}
			delete [] pPoints;
			return ipRetSet;
		}
	}
	_ASSERTE( FALSE );
	return NULL;
}	

// �|���S�����|�����C���ɕϊ�����
IGeometryPtr AheConvertPolygon2Polyline( IPolygonPtr ipPolygon )
{
	ISpatialReferencePtr ipSpRef;
	ipPolygon->get_SpatialReference( &ipSpRef );
	IPointCollectionPtr ipPointCol( ipPolygon );
	if( ipPointCol != NULL )
	{
		WKSPoint* pPoints;
		LONG lPointCount = -1;

		ipPointCol->get_PointCount( &lPointCount );
		pPoints = new WKSPoint[lPointCount];
		ipPointCol->QueryWKSPoints( 0, lPointCount, pPoints );

		ISegmentCollectionPtr ipRetSegCol( CLSID_Polyline );
		((IGeometryPtr)ipRetSegCol)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^

		// �e�����O�ɕ�������
		std::list<RING> listRings;
		InitRing( lPointCount, pPoints, listRings );

		for( std::list<RING>::iterator it = listRings.begin(); it != listRings.end(); it++ )
		{
			LONG lPartPoints = it->END - it->START + 1;
			WKSPoint* pPartPoints = new WKSPoint[lPartPoints];
			// �_����u���b�N�P�ʂŃR�s�[
			::ZeroMemory( pPartPoints, sizeof(WKSPoint) * lPartPoints );	// �������u���b�N������
			::CopyMemory( &pPartPoints[0], &pPoints[it->START], sizeof(WKSPoint) * lPartPoints );
			// �Z�O�����g�ǉ�
			IPointCollectionPtr ipPartPointCol( CLSID_Polyline );
			((IGeometryPtr)ipPartPointCol)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
			ipPartPointCol->SetWKSPoints( lPartPoints, pPartPoints );
			delete [] pPartPoints;
			pPartPoints = NULL;
			ipRetSegCol->AddSegmentCollection( (ISegmentCollectionPtr)ipPartPointCol );
		}
		IPolylinePtr ipRetPolyline( ipRetSegCol );
		ipRetPolyline->putref_SpatialReference( ipSpRef );
		try {
			ITopologicalOperator2Ptr ipTopo( ipRetPolyline );
			ipTopo->put_IsKnownSimple( VARIANT_FALSE );
			ipTopo->Simplify();
			ipRetPolyline->SimplifyNetwork();
		}
		catch(...) {
			ATLASSERT( FALSE );			
		}
		delete [] pPoints;

		return ipRetPolyline;
	}

	_ASSERTE( FALSE );
	return NULL;
}	

// IGeometry -> list<SEGMENTITEM> �ϊ�
BOOL AheGeometry2SegmentItems( IGeometryPtr ipGeometry, std::list<SEGMENTITEM>& listSegments )
{
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeometry ) );
	
	// �����`�F�b�N
	if( ipGeomCopy == NULL )
	{
		_ASSERTE( ipGeomCopy != NULL );
		return FALSE;
	}

	// �W�I���g�� -> �Z�O�����g����
	IGeometryCollectionPtr ipGeomCol( ipGeomCopy );
	LONG lGeomCount = -1;

	ipGeomCol->get_GeometryCount( &lGeomCount );
	for( LONG i = 0; i < lGeomCount; i++ )
	{
		IGeometryPtr ipPartGeom;
		ipGeomCol->get_Geometry( i, &ipPartGeom );

		esriGeometryType type;
		ipPartGeom->get_GeometryType( &type );

		// �|���S���A�|�����C���̏ꍇ�̂�
		switch( type )
		{
			case esriGeometryRing:
			case esriGeometryPath:
			case esriGeometryPolygon:
			case esriGeometryPolyline:
			{
				// �|���S���� Close ����
				IPointCollectionPtr ipPointCol( ipPartGeom );
				if( ipPointCol != NULL )
				{
					LONG lPointCount = -1;
					ipPointCol->get_PointCount( &lPointCount );
					if( type == esriGeometryPolygon || type == esriGeometryRing )
					{
						ipPointCol->RemovePoints( lPointCount - 1, 1 );
						lPointCount--;
					}
				}
				ISegmentCollectionPtr ipSegCol( ipPointCol );
				if( ipSegCol != NULL )
				{
					LONG lSegmentCount = -1;
					ipSegCol->get_SegmentCount( &lSegmentCount );
					for( LONG j = 0; j < lSegmentCount; j++ )
					{
						SEGMENTITEM item;
						
						ipSegCol->get_Segment( j, &(item.ipSegment) );
						if( item.ipSegment != NULL )
						{
							item.ipSegment->get_FromPoint( &(item.ipFromPoint) );
							item.ipSegment->get_ToPoint( &(item.ipToPoint) );
							item.ipFromPoint->QueryCoords( &(item.dFromX), &(item.dFromY) );
							item.ipToPoint->QueryCoords( &(item.dToX), &(item.dToY) );
							item.geometryType = type;
							item.lPartID = i;
							item.lSegmentID = j;
							item.bIsFirstSegment = ( j == 0 ? TRUE : FALSE );
							item.bIsLastSegment = ( j == lSegmentCount - 1 ? TRUE : FALSE );

							listSegments.push_back( item );
						}
						else
							_ASSERTE( FALSE );
					}
				}
				else
					_ASSERTE( FALSE );
			}
		}
	}

	return TRUE;
}

// ArcObjects�̋�ԎQ�ƃI�u�W�F�N�g��SDE C API�̋�ԎQ�ƃI�u�W�F�N�g�ɕϊ�����֐�
SE_COORDREF AheSpatialReference2SE_COORDREF( ISpatialReference* ipSpRef )
{
	// [bug 1809�A5125�A5152] ����̂��߂̊֐��ł��B
	// ���̊֐��̕Ԃ�l��SE_coordref_free�Ŏg�p��ɍ폜����K�v������܂��B
	// ���̊֐��͔���J�֐��ł��B
	if( ! ipSpRef )
		return NULL;

	// ��ԎQ�ƍ쐬
	SE_COORDREF coordref = NULL;
	SSE_coordref_create( &coordref );

	// �t�@�N�g���R�[�h�Z�b�g
	long lFCode = -1;
	ipSpRef->get_FactoryCode( &lFCode );
	SSE_coordref_set_by_id( coordref, lFCode );

	// XY�Z�b�g
	SE_ENVELOPE env = {0};
	ipSpRef->GetDomain( &env.minx, &env.maxx, &env.miny, &env.maxy );
	SSE_coordref_set_xy_by_envelope( coordref, &env );

	// M�Z�b�g
	double min_value = 0.0, max_value = 0.0;
	ipSpRef->GetMDomain( &min_value, &max_value );
	SSE_coordref_set_m_by_range( coordref, min_value, max_value );

	// Z�Z�b�g
	min_value = 0.0, max_value = 0.0;
	ipSpRef->GetZDomain( &min_value, &max_value );
	SSE_coordref_set_z_by_range( coordref, min_value, max_value );

	return coordref;
}

// ArcObjects�̃W�I���g���I�u�W�F�N�g��SDE C API �̃W�I���g���I�u�W�F�N�g�ɕϊ�����֐�
std::pair<SE_SHAPE,SE_COORDREF> AheGeometry2SE_SHAPE( IGeometry* ipGeom )
{
	// [bug 1809�A5125�A5152] ����̂��߂̊֐��ł��B
	// WKB���g�p���ĕϊ����s���܂��B
	// ���̊֐��̕Ԃ�l��SE_shape_free,SE_coordref_free�Ŏg�p��ɍ폜����K�v������܂��B
	// ���̊֐��͔���J�֐��ł��B

	std::pair<SE_SHAPE,SE_COORDREF> pairRet = std::pair<SE_SHAPE,SE_COORDREF>(NULL,NULL); // �Ԃ�l

	// ipGeom -> WKB
	if( ! ipGeom )
		return pairRet;

	// �t�@�N�g���R�[�h���擾
	long lFCode = -1;
	ISpatialReferencePtr ipSpRef;
	ipGeom->get_SpatialReference( &ipSpRef );
	if( ipSpRef != NULL )
		ipSpRef->get_FactoryCode( &lFCode );

	// KnownSimple��FALSE����get_WkbSize�Ɏ��s����I�I
	// �����ŏ�������̂������̂�������Ȃ����A�Ƃ肠����
	// TRUE�ɂ��Ă����i���͖����Ǝv���j
	ITopologicalOperator2Ptr ipTopo( ipGeom );
	if( ipTopo )
	{
		VARIANT_BOOL vbIsKnownSimple = VARIANT_FALSE;
		if( SUCCEEDED( ipTopo->get_IsKnownSimple( &vbIsKnownSimple ) ) )
		{
			if( ! vbIsKnownSimple )
				if( FAILED( ipTopo->put_IsKnownSimple( VARIANT_TRUE ) ) )
					_ASSERTE(false); // put_IsKnownSimple�Ɏ��s
		}
		else
			_ASSERTE(false); // get_IsKnownSimple�Ɏ��s
	}

	// Well Known Binary �̃T�C�Y�擾
	IWkbPtr ipWKB( ipGeom );
	long lBytes = 0;
	if( SUCCEEDED( ipWKB->get_WkbSize( &lBytes ) ) )
	{
		if( 0 < lBytes )
		{
			// �o�b�t�@�m��
			BYTE* pBuf = new BYTE[lBytes];
			if( pBuf )
			{
				// �o�b�t�@�ɏ�������
				if( SUCCEEDED( IWkb_ExportToWkb( ipWKB, &lBytes, pBuf ) ) )
				{
					pairRet.second = AheSpatialReference2SE_COORDREF( ipSpRef );
					SSE_shape_create( pairRet.second, &pairRet.first );
					SSE_shape_generate_from_WKB( (const char*)pBuf, pairRet.first );
				}
				delete [] pBuf;
				pBuf = NULL;
			}
		}
		else {
			//CGeometry::Trace( ipGeom, _T("lBytes == 0 !!") );
		}
	}
	else 
	{
		AheTraceGeometry( ipGeom, ERROR_FAIL_GET_WKSIZE );
		_ASSERTE(false); // IWkb::get_WkbSize�Ɏ��s
	}

	return pairRet;
}

#ifdef _USE_GEOS
geos::geom::Geometry Geometry2Geometry( IGeometry* ipGeom )
{
	// ipGeom -> WKB
	if( ! ipGeom )
		return NULL;

	geos::geom::Geometry *pGeom = NULL; // �Ԃ�l

	// �t�@�N�g���R�[�h���擾
	long lFCode = -1;
	ISpatialReferencePtr ipSpRef;
	ipGeom->get_SpatialReference( &ipSpRef );
	if( ipSpRef != NULL )
		ipSpRef->get_FactoryCode( &lFCode );

	// Well Known Binary �̃T�C�Y�擾
	IWkbPtr ipWKB( ipGeom );
	long lBytes = 0;
	if( SUCCEEDED( ipWKB->get_WkbSize( &lBytes ) ) )
	{
		if( 0 < lBytes )
		{
			// �o�b�t�@�m��
			BYTE* pBuf = new BYTE[lBytes];
			if( pBuf )
			{
				// �o�b�t�@�ɏ�������
				if( SUCCEEDED( IWkb_ExportToWkb( ipWKB, &lBytes, pBuf ) ) )
				{
					// �o�C�i���X�g���[���ɂ���
					std::string strStream;
					strStream.assign( (const char*)pBuf, lBytes );
					std::istringstream cStream( strStream, std::istringstream::in | std::istringstream::binary );

					// �쐬�����X�g���[����ǂݍ���
					geos::io::WKBReader cReader;
					pGeom = cReader.read( cStream );

					// SRID�Z�b�g
					pGeom->setSRID( lFCode );
				}
				delete [] pBuf;
				pBuf = NULL;
			}
		}
		else {
			//CGeometry::Trace( ipGeom, _T("lBytes == 0 !!") );
		}
	}
	else
		_ASSERTE(false); // IWkb::get_WkbSize�Ɏ��s

	return pGeom;
}
#endif // ifdef _USE_GEOS
/////////////////////////////////////////////////////////////////////////////
// �����A�p�x�Ȃ�
/////////////////////////////////////////////////////////////////////////////
// 3�_�̂Ȃ��p�i0<=x<=180�j�����߂�
DOUBLE AheGetAngle( IPointPtr ipPoint1, IPointPtr ipPointCenter, IPointPtr ipPoint2 )
{
	if( ipPoint1 == NULL || ipPointCenter == NULL || ipPoint2 == NULL ) return 0;

	IConstructAnglePtr	ipConstructAngle(CLSID_GeometryEnvironment);
	DOUBLE				dRad = 0.0, dAngle = 0.0;

	if ( ipConstructAngle == NULL )
		return 0;
	if( FAILED( ipConstructAngle->ConstructThreePoint( ipPoint1, ipPointCenter, ipPoint2, &dRad ) ) )
	{
		AheTraceErrorMessage( _T("AheGetAngle") );
		return 0;
	}
	dAngle = ( 180.0 * dRad ) / M_PI;
	_ASSERTE( -180.0 <= dAngle && dAngle <= 180.0 );

	return dAngle;
}

// ���C���̐ڑ��p�����߂�
DOUBLE AheGetAngle( ICurvePtr ipCurve1, ICurvePtr ipCurve2 )
{
	ISpatialReferencePtr ipSpRef;

	if( ipCurve1 != NULL )
		ipCurve1->get_SpatialReference( &ipSpRef );
	if( AheIsTouchEndPoint( ipCurve1, ipCurve2, ipSpRef ) == FALSE ) 
	{
		//ATLASSERT( FALSE );
		return 0;
	}

	IPointPtr ipFromPoint1, ipFromPoint2, ipToPoint1, ipToPoint2;
	LONG lPointCount1 = -1, lPointCount2 = -1;
	IPointPtr ipPoint1, ipPointCenter, ipPoint2;

	ipCurve1->get_FromPoint( &ipFromPoint1 );
	ipCurve1->get_ToPoint( &ipToPoint1 );
	ipCurve2->get_FromPoint( &ipFromPoint2 );
	ipCurve2->get_ToPoint( &ipToPoint2 );
	((IPointCollectionPtr)ipCurve1)->get_PointCount( &lPointCount1 );
	((IPointCollectionPtr)ipCurve2)->get_PointCount( &lPointCount2 );

	if( AheIsEqual( ipFromPoint1, ipFromPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( 1, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( 1, &ipPoint2 );
		ipPointCenter = ipFromPoint1;
	}
	else if( AheIsEqual( ipToPoint1, ipToPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( lPointCount1 - 2, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( lPointCount2 - 2, &ipPoint2 );
		ipPointCenter = ipToPoint1;
	}
	else if( AheIsEqual( ipFromPoint1, ipToPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( 1, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( lPointCount2 - 2, &ipPoint2 );
		ipPointCenter = ipFromPoint1;
	}
	else if( AheIsEqual( ipToPoint1, ipFromPoint2, ipSpRef ) )
	{
		((IPointCollectionPtr)ipCurve1)->get_Point( lPointCount1 - 2, &ipPoint1 );
		((IPointCollectionPtr)ipCurve2)->get_Point( 1, &ipPoint2 );
		ipPointCenter = ipToPoint1;
	}

	return AheGetAngle( ipPoint1, ipPointCenter, ipPoint2 );
}

// ���ς����߂�iipPoint1->ipPoint2 , ipPoint3->ipPoint4�j
DOUBLE AheGetInnerProduct( IPointPtr ipPoint1, IPointPtr ipPoint2, IPointPtr ipPoint3, IPointPtr ipPoint4 )
{
	if( ipPoint1 == NULL || ipPoint2 == NULL || ipPoint3 == NULL || ipPoint4 == NULL ) return -1;

	DOUBLE x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;
	DOUBLE x3 = 0.0, y3 = 0.0, x4 = 0.0, y4 = 0.0;

	ipPoint1->QueryCoords( &x1, &y1 );
	ipPoint2->QueryCoords( &x2, &y2 );
	ipPoint3->QueryCoords( &x3, &y3 );
	ipPoint4->QueryCoords( &x4, &y4 );

	return (x2-x1)*(x4-x3) + (y2-y1)*(y4-y3);
}

// �x�N�g���������߂�iipPoint1->ipPoint2�j
DOUBLE AheGetLength( IPointPtr ipPoint1, IPointPtr ipPoint2 )
{
	DOUBLE x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0;

	ipPoint1->QueryCoords( &x1, &y1 );
	ipPoint2->QueryCoords( &x2, &y2 );

	return sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
}

// �����im�j�����߂�
double AheGetMeterLength( IPointPtr& ipP1, IPointPtr& ipP2 )
{
	double dx1 = 0.0, dx2 = 0.0, dy1 = 0.0, dy2 = 0.0;
	ipP1->QueryCoords( &dx1, &dy1 );
	ipP2->QueryCoords( &dx2, &dy2 );
	return AheGETDIST( dx1, dy1, dx2, dy2 );
}

// ���K�����W���������߂�
double AheGetMeshXYLength( IPointPtr& ipP1, IPointPtr& ipP2 )
{
	double dx1 = 0.0, dx2 = 0.0, dy1 = 0.0, dy2 = 0.0;
	ipP1->QueryCoords( &dx1, &dy1 );
	ipP2->QueryCoords( &dx2, &dy2 );
	int x1 = 0, y1 = 0;
    int x2 = 0, y2 = 0;
	unsigned int mesh1 = AheLL2MESH( dx1, dy1, 2, x1, y1 );
	unsigned int mesh2 = AheLL2MESH( dx2, dy2, 2, x2, y2 );
	double delta_x = 0.0, delta_y = 0.0;
	AheGETDIFF(  mesh1, x1, y1, mesh2, x2, y2, delta_x, delta_y );
	return sqrt( delta_x*delta_x + delta_y*delta_y );
}
/////////////////////////////////////////////////////////////////////////////
// RelationalOperator�n
/////////////////////////////////////////////////////////////////////////////
BOOL Geometry_Relationship( SE_SHAPE pBase, SE_SHAPE pTest, sindyeSpatialRel ope )
{
	// [bug 1809�A5125�A5152] ����̂��߂̊֐��ł��B
	// WKB���g�p���ĕϊ����s���܂��B
	// ���̊֐��̕Ԃ�l��SE_shape_free�Ŏg�p��ɍ폜����K�v������܂��B
	// ���̊֐��͔���J�֐��ł��B
	// NOTE: ope�͕����w��s��

	if( ! ( pBase && pTest ) )
		return FALSE;

	switch( ope )
	{
	case sindyeSpatialRelEqual:    return SSE_shape_is_equal(       pBase, pTest ); break;
	case sindyeSpatialRelTouch:    return SSE_shape_is_touching(    pBase, pTest ); break;
	case sindyeSpatialRelWithin:   return SSE_shape_is_within(      pBase, pTest ); break;
	case sindyeSpatialRelContain:  return SSE_shape_is_containing(  pBase, pTest ); break;
	case sindyeSpatialRelCross:    return SSE_shape_is_crossing(    pBase, pTest ); break;
	case sindyeSpatialRelDisjoint: return SSE_shape_is_disjoint(    pBase, pTest ); break;
	case sindyeSpatialRelOverlap:  return SSE_shape_is_overlapping( pBase, pTest ); break;
	default: break;
	}
	return FALSE;
}

// �W�I���g���̊֘A���`�F�b�N����iSDE C API�Łj
sindyeSpatialRel AheTestGeometryRelationship_SE( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption )
{
	// [bug 1809�A5125�A5152] ����̂��߂̊֐��ł��B
	// SDE C API �̔���֐����g�p���Ĕ�����s���܂��B
	// ��LAPI�ł͔��茋�ʂ̗h�炬���������Ƃ͊m�F�ς݂ł��B
	// ���̊֐��͔���J�֐��ł��B
	sindyeSpatialRel retRel = sindyeSpatialRelNone;	// �Ԃ�l

	// Geometry -> SDE API Shape�ϊ�
	std::pair<SE_SHAPE,SE_COORDREF> pairBase = std::pair<SE_SHAPE,SE_COORDREF>(NULL,NULL);
	std::pair<SE_SHAPE,SE_COORDREF> pairTest = std::pair<SE_SHAPE,SE_COORDREF>(NULL,NULL);
	try {
		pairBase = AheGeometry2SE_SHAPE( ipBaseGeom );
		pairTest = AheGeometry2SE_SHAPE( ipTestGeom );

		_ASSERTE( pairBase.first && pairTest.first );

		if( relOption & sindyeSpatialRelEqual    ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelEqual    ) ? sindyeSpatialRelEqual    : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelTouch    ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelTouch    ) ? sindyeSpatialRelTouch    : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelWithin   ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelWithin   ) ? sindyeSpatialRelWithin   : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelContain  ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelContain  ) ? sindyeSpatialRelContain  : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelCross    ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelCross    ) ? sindyeSpatialRelCross    : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelDisjoint ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelDisjoint ) ? sindyeSpatialRelDisjoint : (sindyeSpatialRel)0 ) );
		if( relOption & sindyeSpatialRelOverlap  ) retRel = (sindyeSpatialRel)( retRel | ( Geometry_Relationship( pairBase.first, pairTest.first, sindyeSpatialRelOverlap  ) ? sindyeSpatialRelOverlap  : (sindyeSpatialRel)0 ) );
	}
	catch(...)
	{
		ATLTRACE(_T("[WARNING] AheTestGeometryRelationship_SE() : ��O����\n") );
		_ASSERTE(false);
	}

	// ��n���i�`��Ƌ�ԎQ�ƍ폜����K�v������j
	SSE_shape_free( pairBase.first );
	SSE_shape_free( pairTest.first );
	SSE_coordref_free( pairBase.second );
	SSE_coordref_free( pairTest.second );

	return retRel;
}

// �W�I���g���̊֘A���`�F�b�N����iArcObjects�Łj
sindyeSpatialRel AheTestGeometryRelationship_AO( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// [bug 1809�A5125�A5152] ����̂��߁ASDE�ڑ��̏ꍇ��_SE�̕���
	// �g�p����悤�ɂȂ�܂����B�]���āA���̊֐���SDE�ڑ�������Ȃ�
	// �ꍇ�Ɍ���g�p����܂��B
	// ���̊֐��͔���J�֐��ł��B
	sindyeSpatialRel retRel = sindyeSpatialRelNone;	// �Ԃ�l

	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipTestGeomCopy( AheGetClone( ipTestGeom ) );

	if( ( ! ipBaseGeomCopy ) || ( ! ipTestGeomCopy ) || relOption == 0 )
		return retRel;

	// ��ԎQ�Ƃ��^�����Ă����炻����Z�b�g�i���܂�ǂ��Ȃ����H�H�j
	if( ipSpRel != NULL )
	{
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRel );
		AheSnapToSpatialReference( ipTestGeomCopy, ipSpRel );
	}

	// ��ԎQ�Ƃ��Ⴄ�ꍇ�͔��肪�����Ƃł��Ȃ��̂ŁA
	// �������ׂ����ق��̋�ԎQ�Ƃ��Z�b�g����i�_��ɂ͉e���Ȃ��͂��j
	if( ! AheHasSameSpatialReference( ipBaseGeomCopy, ipTestGeomCopy ) )
	{
		ATLTRACE(_T("AheTestGeometryRelationship() : ��r����W�I���g���̋�ԎQ�Ƃ��قȂ邽�߁A�ꎞ�I�ɋ�ԎQ�Ƃ݈̂�v�����܂��i�_��ɂ͉e������܂���j�B\n"));
		ISpatialReferencePtr ipMinimumSpRef = AheGetMinimumSpatialReference( AheGetSpatialReference( ipBaseGeomCopy ), AheGetSpatialReference( ipTestGeomCopy ) );
		ipBaseGeomCopy->putref_SpatialReference( ipMinimumSpRef );
		ipTestGeomCopy->putref_SpatialReference( ipMinimumSpRef );
	}

	IRelationalOperatorPtr ipRelOpe( ipBaseGeomCopy );

	// �ڐG
	if( relOption & sindyeSpatialRelTouch )
	{
		VARIANT_BOOL vaIsTouch = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Touches( ipTestGeomCopy, &vaIsTouch ) ) )
		{
			if( vaIsTouch )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelTouch );
		}
		else
			_ASSERTE(false);
	}
	// �I�[�o�[���b�v
	if( relOption & sindyeSpatialRelOverlap )
	{
		VARIANT_BOOL vaIsOverlap = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Overlaps( ipTestGeomCopy, &vaIsOverlap ) ) )
		{
			if( vaIsOverlap )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelOverlap );
		}
		else
			_ASSERTE(false);
	}
	// ����
	if( relOption & sindyeSpatialRelCross )
	{
		VARIANT_BOOL vaIsCross = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Crosses( ipTestGeomCopy, &vaIsCross ) ) )
		{
			if( vaIsCross )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelCross );
		}
		else
			_ASSERTE(false);
	}
	// �܂܂��
	if( relOption & sindyeSpatialRelWithin )
	{
		VARIANT_BOOL vaIsWithin = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Within( ipTestGeomCopy, &vaIsWithin ) ) )
		{
			if( vaIsWithin )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelWithin );
		}
		else
			_ASSERTE(false);
	}
	// �܂�ł���
	if( relOption & sindyeSpatialRelContain )
	{ 
		VARIANT_BOOL vaIsContain = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Contains( ipTestGeomCopy, &vaIsContain ) ) )
		{
			if( vaIsContain )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelContain );
		}
		else
			_ASSERTE(false);
	}
	// ����
	if( relOption & sindyeSpatialRelDisjoint )
	{
		VARIANT_BOOL vaIsDisjoint = VARIANT_FALSE;
		if( SUCCEEDED( ipRelOpe->Disjoint( ipTestGeomCopy, &vaIsDisjoint ) ) )
		{
			if( vaIsDisjoint )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelDisjoint );
		}
		else
			_ASSERTE(false);
	}
	// ������
	if( relOption & sindyeSpatialRelEqual )
	{ 
		VARIANT_BOOL vaIsEqual = VARIANT_FALSE;
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
		if( SUCCEEDED( ipRelOpe->Equals( ipTestGeomCopy, &vaIsEqual ) ) )
		{
			if( vaIsEqual == VARIANT_TRUE )
			{
				// ���������āA�\���_��������Ă��Ă��u�������v���Ȃ�́H�H
				// ������Ă��������Ȃ��H�H���Ȃ݂ɁA����
				IPointCollectionPtr ipBaseCol( ipBaseGeomCopy ), ipTestCol( ipTestGeomCopy );
				if( ipBaseCol != NULL && ipTestCol != NULL )
				{
					LONG lBaseColCount = -1, lTestColCount = -1;
					ipBaseCol->get_PointCount( &lBaseColCount );
					ipTestCol->get_PointCount( &lTestColCount );
					if( lBaseColCount == lTestColCount )
						retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelEqual );
				}
				else
					retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelEqual );
			}
		}
#else
		// IRelationalOperator::Equals��ITopologicalOperator::Difference���g�p����̂ł��Ȃ�Â����ʂ��ł�
		// ����ł͂����̎g�p���@�ɂ͂��񂺂񍇂�Ȃ��̂�IClone::IsEqual���g�p����
		if( SUCCEEDED( ((IClonePtr)ipRelOpe)->IsEqual( (IClonePtr)ipTestGeomCopy, &vaIsEqual ) ) )
		{
			if( vaIsEqual )
				retRel = (sindyeSpatialRel)( retRel | sindyeSpatialRelEqual );
		}
		else
			_ASSERTE(false);
#endif
	}

	return retRel;
}

// �W�I���g���̊֘A���`�F�b�N����
sindyeSpatialRel AheTestGeometryRelationship( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, sindyeSpatialRel relOption, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// sdelib���g�p�ł��邩�ǂ����m�F
	// ��ԎQ�Ƃ��쐬�ł��邩�ǂ����Ŕ��f����
	// SDE�ڑ��̃��C����TOC���疳���Ȃ����i�K�ŃG���[
	// �ɂȂ邽�߁A���ʂł�����`�F�b�N����K�v������c
#ifdef _NOT_USE_SDE_RELATIONSHIP
	return AheTestGeometryRelationship_AO( ipBaseGeom, ipTestGeom, relOption, ipSpRel );
#else
	bool bCanUseSDELIB = false;
	SE_COORDREF pRef = NULL;
	if( SE_SUCCESS == SSE_coordref_create(&pRef) )
	{
		SSE_coordref_free(pRef);
		bCanUseSDELIB = true;
	}

	if( bCanUseSDELIB )
		return AheTestGeometryRelationship_SE( ipBaseGeom, ipTestGeom, relOption );
	else
		return AheTestGeometryRelationship_AO( ipBaseGeom, ipTestGeom, relOption, ipSpRel );
#endif
}

// ��{�W�I���g������r�W�I���g�����܂ނ��i����`��܂ށj
BOOL AheIsContain3( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// ��{�@�\�� AheIsContain �Ɠ����ł����AAheTestGeometryRelationship �� sindyRelNone ���Ԃ��ė����ꍇ
	// �� PointCollection �ɕϊ����Ĕ�r���܂��B
	BOOL bRet = TRUE;

	bRet = AheIsContain(ipBaseGeom, ipTestGeom, ipSpRel);

	if ( FALSE == bRet ) {
		bRet = TRUE;

		IRelationalOperatorPtr pIRelationalOperator;
		IPointCollectionPtr pIPointCollection;
		IPointPtr pIPoint;
		LONG lCount;
		VARIANT_BOOL bFlg = VARIANT_FALSE, bFlg2 = VARIANT_FALSE;

		pIPointCollection = ipTestGeom;
		pIRelationalOperator = ipBaseGeom;

		if ( pIPointCollection ) {
			pIPointCollection->get_PointCount(&lCount);

			for ( int i = 0; i < lCount; i++ ) {
				pIPointCollection->get_Point(i, &pIPoint);
				pIRelationalOperator->Touches(pIPoint, &bFlg);
				pIRelationalOperator->Contains(pIPoint, &bFlg2);

				if (( VARIANT_FALSE == bFlg ) && ( VARIANT_FALSE == bFlg2 )) { 
					bRet = FALSE;	
					break;
				}
			}
		} else {
			pIPoint = ipTestGeom;
			pIRelationalOperator->Touches(pIPoint, &bFlg);
			pIRelationalOperator->Contains(pIPoint, &bFlg2);

			if (( VARIANT_FALSE == bFlg ) && ( VARIANT_FALSE == bFlg2 )) { 
				bRet = FALSE;	
			}
		}
	}

	return bRet;
}

// ��{�W�I���g������r�W�I���g�����܂ނ��i����`��܂ށj
BOOL AheIsContain4( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr ipSpRel/* = NULL*/ )
{
	// ��{�@�\�� AheIsContain �Ɠ����ł����AipTestGeom �� �|�C���g �ȊO�̎��� �Z�O�����g ����
	// �������Ĕ�r���܂��B
	ISpatialReferencePtr 	ipSpatialReference = ipSpRel;
	IGeometryPtr			ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr 			ipTestGeomCopy( AheGetClone( ipTestGeom ) );
	esriGeometryType		esriGeomType;

	if ( AheIsContain( ipBaseGeomCopy, ipTestGeomCopy, ipSpatialReference ) == FALSE ) {
		if ( ipBaseGeomCopy == NULL || ipTestGeomCopy == NULL )
			return FALSE;

		ipTestGeomCopy->get_GeometryType( &esriGeomType );
		if ( esriGeomType != esriGeometryPoint ) {
			IGeometryCollectionPtr 	ipGeometryCollection;
			IGeometryPtr 			ipSegmentGeometry;
			LONG 					lGeomCount = -1;

			if ( ipSpatialReference != NULL ) {
				AheSnapToSpatialReference( ipBaseGeomCopy, ipSpatialReference );
				AheSnapToSpatialReference( ipTestGeomCopy, ipSpatialReference );
			} else {
				ipBaseGeomCopy->get_SpatialReference( &ipSpatialReference );
			}

			if ( AheHasSameSpatialReference( ipBaseGeomCopy, ipTestGeomCopy ) == FALSE ) {
				ipSpatialReference = AheGetMinimumSpatialReference( AheGetSpatialReference( ipBaseGeomCopy ), AheGetSpatialReference( ipTestGeomCopy ) );
				ipBaseGeomCopy->putref_SpatialReference( ipSpatialReference );
				ipTestGeomCopy->putref_SpatialReference( ipSpatialReference );
			}

			ipSegmentGeometry = ipTestGeomCopy;
			AheSnapToSpatialReferenceAndForceSimplify( ipSegmentGeometry, ipSpatialReference ); // TODO: ��߂��ق����ǂ��񂶂�Ȃ��H
			ipGeometryCollection = ipSegmentGeometry;
			if ( ipGeometryCollection == NULL )
				return FALSE;

			ISpatialReferencePtr ipTestSpRef( AheGetSpatialReference( ipTestGeomCopy ) ); // Test���̋�ԎQ��
			ipGeometryCollection->get_GeometryCount( &lGeomCount );
			for ( LONG iCnt = 0; iCnt < lGeomCount; iCnt++ ) {
				IGeometryPtr 			ipPartGeom;

				ipGeometryCollection->get_Geometry( iCnt, &ipPartGeom );

				ISegmentCollectionPtr 	ipSegmentCollection( ipPartGeom );
				LONG 					lSegCount = -1;

				if( ipSegmentCollection == NULL )
					return FALSE;

				ipSegmentCollection->get_SegmentCount( &lSegCount );
				for( LONG jCnt = 0; jCnt < lSegCount; jCnt++ ) {
					_ISegmentPtr 			ipSegment, ipSegmentCopy;
					IPolylinePtr			ipTestPolyline( CLSID_Polyline );
					ipTestPolyline->putref_SpatialReference( ipTestSpRef ); // Test���̋�ԎQ�ƕt�^
					IPointPtr				ipTestFromPoint( CLSID_Point ), ipTestToPoint( CLSID_Point );
					ipTestFromPoint->putref_SpatialReference( ipTestSpRef ); // Test���̋�ԎQ�ƕt�^
					ipTestToPoint->putref_SpatialReference( ipTestSpRef ); // Test���̋�ԎQ�ƕt�^
					DOUBLE					dbTestFromX = 0.0f, dbTestFromY = 0.0f, dbTestToX = 0.0f, dbTestToY = 0.0f;

					ipSegmentCollection->get_Segment( jCnt, &ipSegment );
					if( ipSegment == NULL )
						return FALSE;

					ipSegmentCopy = AheGetClone( (_ISegmentPtr)ipSegment );
					if ( ipSegmentCopy == NULL )
						return FALSE;

					ipTestFromPoint->PutCoords( 0.0f, 0.0f );
					ipTestToPoint->PutCoords( 0.0f, 0.0f );

					if ( FAILED( ipSegmentCopy->QueryFromPoint( ipTestFromPoint ) ) || FAILED( ipSegmentCopy->QueryToPoint( ipTestToPoint ) ) )
						return FALSE;
					ipTestFromPoint->QueryCoords( &dbTestFromX, &dbTestFromY );
					ipTestToPoint->QueryCoords( &dbTestToX, &dbTestToY );

					ipTestPolyline->SetEmpty();
					((IPointCollectionPtr)ipTestPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestFromPoint ) );
					((IPointCollectionPtr)ipTestPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestToPoint ) );

					AheSnapToSpatialReferenceAndForceSimplify( ipTestPolyline, ipSpatialReference ); // TODO: ��߂��ق����ǂ��񂶂�Ȃ��H�H

					if ( AheIsContain( ipBaseGeomCopy, ipTestPolyline, ipSpatialReference ) == FALSE ) {
						if ( AheIsWithin( ipTestGeomCopy, ipBaseGeomCopy, ipSpatialReference ) == FALSE )
							return FALSE;
					}
				}
			}
		} else {
			if ( AheIsContain3( ipBaseGeomCopy, ipTestGeomCopy, ipSpatialReference ) == FALSE )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL AheIsEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr /*ipSpRel = NULL*/ ){
	// Equal����ɂ��Ă�IClone::IsEqual���g�p�ł��邱�Ƃ��������Ă���̂ŁA
	// ������g�p����
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return FALSE;

	VARIANT_BOOL vbRet = VARIANT_FALSE;
	esriGeometryType emTypeBase = esriGeometryAny;
	esriGeometryType emTypeTest = esriGeometryAny;
	ipBaseGeom->get_GeometryType( &emTypeBase );
	ipTestGeom->get_GeometryType( &emTypeTest );

	// ��r�Ώۂ�esriGeometryPoint�̎��̂݁A�ʓr����
	if( esriGeometryPoint == emTypeBase && esriGeometryPoint == emTypeTest )
	{
		return AheIsDBEqual( (IPointPtr)ipBaseGeom, (IPointPtr)ipTestGeom );
	}

	IRelationalOperatorPtr ipRelOpe(ipBaseGeom);
	if( SUCCEEDED( ipRelOpe->Equals( ipTestGeom, &vbRet ) ) )
	{
		if( vbRet )
		{
			// ���������āA�\���_��������Ă��Ă��u�������v���Ȃ�́H�H
			// ������Ă��������Ȃ��H�H���Ȃ݂ɁA����
			IPointCollectionPtr ipBaseCol( ipBaseGeom ), ipTestCol( ipTestGeom );
			if( ipBaseCol && ipTestCol )
			{
				LONG lBaseColCount = -1, lTestColCount = -1;
				ipBaseCol->get_PointCount( &lBaseColCount );
				ipTestCol->get_PointCount( &lTestColCount );
				if( lBaseColCount != lTestColCount )
					vbRet = VARIANT_FALSE;
			}
		}
	}

	return ( vbRet ) ? TRUE : FALSE;
}

// �f�[�^�x�[�X���Ɋi�[�����ꍇ�ɓ���_���ǂ��������؂���
BOOL AheIsDBEqual( IPointPtr ipP1, IPointPtr ipP2 )
{
	if( ! ( ipP1 && ipP2  ) )
	{
		_ASSERT_EXPR( ipP1 && ipP2, _T("�n���ꂽ�|�C���g�̂ǂ��炩��NULL") );
		return false;
	}

	ISpatialReferencePtr ipSpRef;
	ipP1->get_SpatialReference( &ipSpRef );
	// [Bug 7881]Yank���[�h���ɉE�N���b�N�L�����Z���Łu�\���_�̌����𐳂������f�ł��܂���ł����v
	// �̃��b�Z�[�W���o��o�O�C��
	// sp1�ɋ�ԎQ�ƂȂ��ꍇ�ɁAsp2����Ƃ��Ă݂�
	if( ! ipSpRef )
		ipP2->get_SpatialReference( &ipSpRef );

	if( ! ipSpRef )
	{
		_ASSERT_EXPR( ipSpRef, _T("�������̋�ԎQ�Ƃ�NULL") );
		return false;
	}

	WKSPoint p[2] = {0.0};
	ipP1->QueryCoords( &p[0].X, &p[0].Y );
	ipP2->QueryCoords( &p[1].X, &p[1].Y );

	// �v�Z�ɕK�v�Ȃ��̂��擾
	double falseX = 0.0, falseY = 0.0, xyUnits = 0.0;
	ipSpRef->GetFalseOriginAndUnits( &falseX, &falseY, &xyUnits );

	// �ۂߏ���
	WKSPoint intp[2] = {0.0};
	intp[0].X = floor(((p[0].X - falseX) * xyUnits) + 0.5);
	intp[0].Y = floor(((p[0].Y - falseY) * xyUnits) + 0.5);
	intp[1].X = floor(((p[1].X - falseX) * xyUnits) + 0.5);
	intp[1].Y = floor(((p[1].Y - falseY) * xyUnits) + 0.5);

	return ( intp[0].X == intp[1].X && intp[0].Y == intp[1].Y );
}

BOOL AheIsExactEqual( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, ISpatialReferencePtr/* ipSpRel = NULL*/ )
{
	// IClone::IsEqual��1���j�b�g���Ɛ^�Ɣ��f����邽�߁A�_���r
	// ���s���ibug 5620,5617�j
#if 0
	// AheIsEqual�̓|�C���g��r�ő�ʂɎg�p�����\��������̂�inline��
	// �܂��AEqual����ɂ��Ă�IClone::IsEqual���g�p�ł��邱�Ƃ��������Ă���̂ŁA
	// ������g�p����
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return FALSE;

	VARIANT_BOOL vbRet = VARIANT_FALSE;
	((IClonePtr)ipBaseGeom)->IsEqual( (IClonePtr)ipTestGeom, &vbRet );
	return ( vbRet ) ? TRUE : FALSE;
#else
	// 1. �_���r���s���֌W�㓯����W�_�����A�����đ��݂��Ȃ������Ƃ�
	//    �O��ɂ��Ă���i�A�����Ă��Ȃ���Εʕ��Ȃ̂�OK�j
	// 2. �|���S����Close����Ă���K�v������

	// NULL����
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return FALSE;

	// �󔻒�
	VARIANT_BOOL vbBaseEmpty = VARIANT_FALSE, vbTestEmpty = VARIANT_FALSE;
	ipBaseGeom->get_IsEmpty( &vbBaseEmpty );
	ipTestGeom->get_IsEmpty( &vbTestEmpty );
	// �ʏ�W�I���g������ł��邱�Ƃ����҂��邱�Ƃ͂Ȃ��Ǝv����̂ŁA
	// ������ł�FALSE��Ԃ����Ƃɂ���
	if( vbBaseEmpty || vbTestEmpty )
		return FALSE;

	// �^����
	esriGeometryType emBase = esriGeometryAny, emTest = esriGeometryAny;
	ipBaseGeom->get_GeometryType( &emBase );
	ipTestGeom->get_GeometryType( &emTest );
	if( emBase != emTest )
		return FALSE;

	// �����E�G���A����
	switch( emBase )
	{
		case esriGeometryPath:
		case esriGeometryLine:
		case esriGeometryPolyline:
		{
			double dBaseLen = 0, dTestLen = 0;
			((ICurvePtr)ipBaseGeom)->get_Length( &dBaseLen );
			((ICurvePtr)ipTestGeom)->get_Length( &dTestLen );
			if( dBaseLen != dTestLen )
				return FALSE;
			break;
		}
		case esriGeometryPolygon:
		case esriGeometryRing:
		{
			double dBaseArea = 0, dTestArea = 0;
			((IAreaPtr)ipBaseGeom)->get_Area( &dBaseArea );
			((IAreaPtr)ipTestGeom)->get_Area( &dTestArea );
			if( dBaseArea != dTestArea )
				return FALSE;
			break;
		}
		default: break;
	}

	// �_�񔻒�
	switch( emBase )
	{
		case esriGeometryPoint:
		{
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			if ( AheIsEqual( ipBaseGeom, ipTestGeom ) )
#else
			double xBase = 0, yBase = 0, xTest = 0, yTest = 0;
			((IPointPtr)ipBaseGeom)->QueryCoords( &xBase, &yBase );
			((IPointPtr)ipTestGeom)->QueryCoords( &xTest, &yTest );
			if( xBase != xTest || yBase != yTest )
#endif
				return FALSE;
			break;
		}
		case esriGeometryEnvelope:
		{
			WKSEnvelope stBase = {0}, stTest = {0};
			((IEnvelopePtr)ipBaseGeom)->QueryWKSCoords( &stBase );
			((IEnvelopePtr)ipTestGeom)->QueryWKSCoords( &stTest );
			if( stBase.XMax != stTest.XMax || stBase.XMin != stTest.XMin ||
				stBase.YMax != stTest.YMax || stBase.YMin != stTest.YMin )
				return FALSE;
			break;
		}
		case esriGeometryPolygon:
		{
			// �|���S���̓����O�P�ʂŔ�r����
			IPolygonPtr ipBasePoly( ipBaseGeom );
			IPolygonPtr ipTestPoly( ipTestGeom );
			// Exterior�̔�r
			long lBaseExCount = 0, lTestExCount = 0;
			ipBasePoly->get_ExteriorRingCount( &lBaseExCount );
			ipTestPoly->get_ExteriorRingCount( &lTestExCount );
			if( 0 == lBaseExCount || 0 == lTestExCount || lBaseExCount != lTestExCount )
				return FALSE; // SetWKSPoint�ō쐬�����|���S����SimplifyPreserveFromTo�����Ȃ���0�ɂȂ�݂����c�Ȃ̂ŃX���[���Ȃ��悤�ɂ����őj�~
			IRing** ppBaseExRing = new IRing*[lBaseExCount];
			IRing** ppTestExRing = new IRing*[lTestExCount];
			ipBasePoly->QueryExteriorRings( ppBaseExRing );
			ipTestPoly->QueryExteriorRings( ppTestExRing );
			for( int i = 0; i < lBaseExCount; ++i )
			{
				bool bIsEqual = false;
				int j = 0;
				for( ; j < lTestExCount; ++j )
				{
					if( AheIsEqual( ppBaseExRing[i], ppTestExRing[j] ) )
					{
						bIsEqual = true; // ��ł��������̂����������炨���܂�
						break;
					}
				}
				if( ! bIsEqual )
				{
					for( int n = 0; n < lBaseExCount; ++n )
					{
						ppBaseExRing[n]->Release();
						ppTestExRing[n]->Release();
					}
					delete [] ppBaseExRing;
					delete [] ppTestExRing;
					return FALSE;
				}
				else {
					// Exterior�������Ȃ炻�̒���Interior���m�Ŕ�r
					long lBaseInCount = 0, lTestInCount = 0;
					ipBasePoly->get_InteriorRingCount( ppBaseExRing[i], &lBaseInCount );
					ipTestPoly->get_InteriorRingCount( ppTestExRing[j], &lTestInCount );
					if( lBaseInCount != lTestInCount )
					{
						for( int n = 0; n < lBaseExCount; ++n )
						{
							ppBaseExRing[n]->Release();
							ppTestExRing[n]->Release();
						}
						delete [] ppBaseExRing;
						delete [] ppTestExRing;
						return FALSE;
					}
					IRing** ppBaseInRing = new IRing*[lBaseInCount];
					IRing** ppTestInRing = new IRing*[lTestInCount];
					ipBasePoly->QueryInteriorRings( ppBaseExRing[i], ppBaseInRing );
					ipTestPoly->QueryInteriorRings( ppTestExRing[j], ppTestInRing );
					for( int k = 0; k < lBaseInCount; ++k )
					{
						bool bIsEqualInterior = false;
						for( int l = 0; l < lTestInCount; ++l )
						{
							if( AheIsEqual( ppBaseInRing[i], ppTestInRing[j] ) )
							{
								bIsEqualInterior = true; // ��ł��������̂����������炨���܂�
								break;
							}
						}
						if( ! bIsEqualInterior )
						{
							for( int n = 0; n < lBaseExCount; ++n )
							{
								ppBaseExRing[n]->Release();
								ppTestExRing[n]->Release();
							}
							for( int n = 0; n < lBaseInCount; ++n )
							{
								ppBaseInRing[n]->Release();
								ppTestInRing[n]->Release();
							}
							delete [] ppBaseInRing;
							delete [] ppTestInRing;
							delete [] ppBaseExRing;
							delete [] ppTestExRing;
							return FALSE;
						}
					}
					for( int n = 0; n < lBaseInCount; ++n )
					{
						ppBaseInRing[n]->Release();
						ppTestInRing[n]->Release();
					}
					delete [] ppBaseInRing;
					delete [] ppTestInRing;
				}
			}
			for( int n = 0; n < lBaseExCount; ++n )
			{
				ppBaseExRing[n]->Release();
				ppTestExRing[n]->Release();
			}
			delete [] ppBaseExRing;
			delete [] ppTestExRing;
			break;
		}
		default:
		{
			IPointCollectionPtr ipBaseCol( ipBaseGeom );
			IPointCollectionPtr ipTestCol( ipTestGeom );
			if( ! ( ipBaseCol && ipTestCol ) )
				return FALSE;
			long lBaseCount = 0, lTestCount = 0;
			ipBaseCol->get_PointCount( &lBaseCount );
			ipTestCol->get_PointCount( &lTestCount );
			if( lBaseCount != lTestCount )
				return FALSE;
			// �_��m��
			// �������A�|���S���͍ŏI�_�͊J�n�_�Əd�����邽�ߏ���
			// �֌W�ŕK��Close����Ă���K�v������
			if( esriGeometryRing == emBase ){ --lBaseCount; --lTestCount; }
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			switch( emBase )
			{
				case esriGeometryPolyline:
					for( long i = 0; i < lBaseCount; ++i ) // ������
					{
						IPointPtr ipBasePoint;
						ipBaseCol->get_Point( i, &ipBasePoint );

						IPointPtr ipTestPoint;
						ipTestCol->get_Point( i, &ipTestPoint );

						if ( ! AheIsEqual( ipBasePoint, ipTestPoint ) )
						{
							for( long j = 0, k = lBaseCount - 1; j < lBaseCount; ++j, --k ) // �t����
							{
								ipBaseCol->get_Point( j, &ipBasePoint );
								ipTestCol->get_Point( k, &ipTestPoint );

								if ( ! AheIsEqual( ipBasePoint, ipTestPoint ) )
									return FALSE;
							}
						}
					}
					break;
				case esriGeometryRing:
				{
					// ����_��������
					bool bFindSamePoint = false;
					long iStart = 0, jStart = 0;
					for( long i = 0; i < lBaseCount; ++i )
					{
						for( long j = 0; j < lTestCount; ++j )
						{
							IPointPtr ipBasePoint;
							ipBaseCol->get_Point( i, &ipBasePoint );

							IPointPtr ipTestPoint;
							ipTestCol->get_Point( j, &ipTestPoint );

							if ( AheIsEqual( ipBasePoint, ipTestPoint ) )
							{
								iStart = i;
								jStart = j;
								bFindSamePoint = true;
								break;
							}
						}
						if( bFindSamePoint )
							break;
					}
					if( ! bFindSamePoint )
						return FALSE;
					// ����������_�����r�J�n
					for( long i = iStart, j = jStart, iTotal = 0; iTotal  < lBaseCount; ++i, ++j, ++iTotal )
					{
						if( i == lBaseCount )
							i = 0;
						if( j == lTestCount )
							j = 0;

						IPointPtr ipBasePoint;
						ipBaseCol->get_Point( i, &ipBasePoint );

						IPointPtr ipTestPoint;
						ipTestCol->get_Point( j, &ipTestPoint );

						if ( ! AheIsEqual( ipBasePoint, ipTestPoint ) )
						{
							// �`��ɂ���Ăׂ͗荇��Ȃ��Ă�����_�����݂���
							// ��F�h�[�i�c�|���S���̓����ƊO������_�Őڂ��Ă���ꍇ
							// �������A�����SiNDY�ł͂��̂悤�Ȍ`��̑��݂͍���͂�
							// �]���āA�Č����͍s�킸�s��v�Ɣ��f����
							return FALSE;
						}
					}
					break;
				}
				default: 
					break;
			}
#else
			WKSPoint* pBase = new WKSPoint[lBaseCount];
			WKSPoint* pTest = new WKSPoint[lTestCount];
			ipBaseCol->QueryWKSPoints( 0, lBaseCount, pBase );
			ipTestCol->QueryWKSPoints( 0, lTestCount, pTest );
			switch( emBase )
			{
				case esriGeometryPolyline:
					for( long i = 0; i < lBaseCount; ++i ) // ������
					{
						if( pBase[i].X != pTest[i].X ||
							pBase[i].Y != pTest[i].Y )
						{
							for( long j = 0, k = lBaseCount - 1; j < lBaseCount; ++j, --k ) // �t����
							{
								if( pBase[j].X != pTest[k].X ||
									pBase[j].Y != pTest[k].Y )
								{
									delete [] pBase;
									delete [] pTest;
									return FALSE;
								}
							}
						}
					}
					break;
				case esriGeometryRing:
				{
					// ����_��������
					bool bFindSamePoint = false;
					long iStart = 0, jStart = 0;
					for( long i = 0; i < lBaseCount; ++i )
					{
						for( long j = 0; j < lTestCount; ++j )
						{
							if( pBase[i].X == pTest[j].X &&
								pBase[i].Y == pTest[j].Y )
							{
								iStart = i;
								jStart = j;
								bFindSamePoint = true;
								break;
							}
						}
						if( bFindSamePoint )
							break;
					}
					if( ! bFindSamePoint )
					{
						delete [] pBase;
						delete [] pTest;
						return FALSE;
					}
					// ����������_�����r�J�n
					for( long i = iStart, j = jStart, iTotal = 0; iTotal  < lBaseCount; ++i, ++j, ++iTotal )
					{
						if( i == lBaseCount )
							i = 0;
						if( j == lTestCount )
							j = 0;
						if( pBase[i].X != pTest[j].X ||
							pBase[i].Y != pTest[j].Y )
						{
							// �`��ɂ���Ăׂ͗荇��Ȃ��Ă�����_�����݂���
							// ��F�h�[�i�c�|���S���̓����ƊO������_�Őڂ��Ă���ꍇ
							// �������A�����SiNDY�ł͂��̂悤�Ȍ`��̑��݂͍���͂�
							// �]���āA�Č����͍s�킸�s��v�Ɣ��f����
							delete [] pBase;
							delete [] pTest;
							return FALSE;
						}
					}
					break;
				}
				default: 
					break;
			}
			delete [] pBase;
			delete [] pTest;
#endif
			break;
		}
	}
	return TRUE;
#endif // if 0
}

/////////////////////////////////////////////////////////////////////////////
// TopologicalOperator�n�֐�
/////////////////////////////////////////////////////////////////////////////
// �W�I���g���� Union �����
IGeometryPtr AheUnion( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;
	
	if( ipSpRef != NULL )
	{
		// �n���Q�Ƃ��킹
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipAddGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
	ITopologicalOperatorPtr ipTopo2( ipAddGeomCopy );
	if( ipTopo1 == NULL || ipTopo2 == NULL ) return NULL;

	ipTopo1->Simplify();
	ipTopo2->Simplify();

	IGeometryPtr ipRetGeom;
	if( SUCCEEDED( ipTopo1->Union( ipAddGeomCopy, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	else
		AheTraceErrorMessage( _T("AheUnion") );

	return ipRetGeom;
}

// �|�����C�� Union �����
IGeometryPtr AheUnionNetwork( IPolylinePtr ipBaseGeom, IPolylinePtr ipAddGeom, ISpatialReferencePtr ipSpRef )
{
	IPointCollectionPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IPointCollectionPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;
	
	// �[�_���m�̐ڐG����
	LONG lBasePointCount = -1, lAddPointCount = -1;
	ipBaseGeomCopy->get_PointCount( &lBasePointCount );
	ipAddGeomCopy->get_PointCount( &lAddPointCount );

	IPointPtr ipBaseFromPoint, ipBaseToPoint, ipAddFromPoint, ipAddToPoint;
	ipBaseGeomCopy->get_Point( 0, &ipBaseFromPoint );
	ipBaseGeomCopy->get_Point( lBasePointCount - 1, &ipBaseToPoint );
	ipAddGeomCopy->get_Point( 0, &ipAddFromPoint );
	ipAddGeomCopy->get_Point( lAddPointCount - 1, &ipAddToPoint );

	BOOL bBaseFromIsAddFrom = AheIsEqual( ipBaseFromPoint, ipAddFromPoint, ipSpRef );
	BOOL bBaseFromIsAddTo   = AheIsEqual( ipBaseFromPoint, ipAddToPoint,   ipSpRef );
	BOOL bBaseToIsAddFrom   = AheIsEqual( ipBaseToPoint,   ipAddFromPoint, ipSpRef );
	BOOL bBaseToIsAddTo     = AheIsEqual( ipBaseToPoint,   ipAddToPoint,   ipSpRef );

	// �[�_���m���ڂ��Ă��Ȃ����NULL��Ԃ�
	if( bBaseFromIsAddFrom == FALSE && bBaseFromIsAddTo == FALSE &&
		bBaseToIsAddFrom   == FALSE && bBaseToIsAddTo   == FALSE )
		return NULL;

	// �Ԃ��|�����C���̓_��쐬
	// ���̏ꍇ�ABASE�̕��������炷��
	WKSPoint* pPoints = new WKSPoint[lBasePointCount + lAddPointCount - 1];
	if( bBaseToIsAddFrom == TRUE || bBaseToIsAddTo == TRUE )
	{
		ipBaseGeomCopy->QueryWKSPoints( 0, lBasePointCount, pPoints );
		if( bBaseToIsAddTo == TRUE )
			((IPolylinePtr)ipAddGeomCopy)->ReverseOrientation();
		ipAddGeomCopy->QueryWKSPoints( 0, lAddPointCount, &(pPoints[lBasePointCount - 1]) );
	}
	else {
		if( bBaseFromIsAddFrom == TRUE )
			((IPolylinePtr)ipAddGeomCopy)->ReverseOrientation();
		ipAddGeomCopy->QueryWKSPoints( 0, lAddPointCount, pPoints );
		ipBaseGeomCopy->QueryWKSPoints( 0, lBasePointCount, &(pPoints[lAddPointCount - 1]) );
	}

	// �Ԃ��|�����C���쐬
	IPointCollectionPtr ipNewPolyline( CLSID_Polyline );
	((IGeometryPtr)ipNewPolyline)->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
	ipNewPolyline->SetWKSPoints( lBasePointCount + lAddPointCount - 1, pPoints );
	((IGeometryPtr)ipNewPolyline)->SnapToSpatialReference(); // TODO: ���Ȃ��ق��������񂶂�Ȃ��H
	((IPolylinePtr)ipNewPolyline)->SimplifyNetwork();

#ifdef DEBUG
	LONG lNewPointCount = -1;
	ipNewPolyline->get_PointCount( &lNewPointCount );
	ATLASSERT( lNewPointCount == ( lBasePointCount + lAddPointCount - 1 ) );
#endif

	delete [] pPoints;

	return ipNewPolyline;
}

// �W�I���g�����m�����̂���i�}���`�����_���p�j
IGeometryPtr AheUnionSegments( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;
	
	if( ipSpRef != NULL )
	{
		// �n���Q�Ƃ��킹
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipAddGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
	ITopologicalOperatorPtr ipTopo2( ipAddGeomCopy );
	if( ipTopo1 == NULL || ipTopo2 == NULL ) return NULL;

	ipTopo1->Simplify();
	ipTopo2->Simplify();

	// ���W�I���g���E�ǉ��W�I���g�������ꂼ��ISegmentCollection�ɂ��č��̂�����
	IPolylinePtr			ipBaseSegPolyline(ipBaseGeomCopy);
	IPolylinePtr			ipAddSegPolyline(ipAddGeomCopy);
	ISegmentCollectionPtr	ipBaseSegCol(ipBaseSegPolyline);
	ISegmentCollectionPtr	ipAddSegCol(ipAddSegPolyline);
	IPolylinePtr			ipBasePolyline2;
	IGeometryPtr			ipRetGeom;
	if (SUCCEEDED(ipBaseSegCol->AddSegmentCollection(ipAddSegCol))) {
		ipBasePolyline2 = ipBaseSegCol;
		ipRetGeom = ipBasePolyline2;
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	}
/*
	if( SUCCEEDED( ipTopo1->Union( ipAddGeomCopy, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	else
		AheTraceErrorMessage( _T("AheUnion") );
*/
	return ipRetGeom;
}

// 
IGeometryPtr AheDifference( IGeometryPtr ipBaseGeom, IGeometryPtr ipAddGeom, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipAddGeomCopy( AheGetClone( ipAddGeom ) );

	if( ipBaseGeomCopy == NULL || ipAddGeomCopy == NULL ) return NULL;

	if( ipSpRef != NULL )
	{
		// �n���Q�Ƃ��킹
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipAddGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
	ITopologicalOperatorPtr ipTopo2( ipAddGeomCopy );
	if( ipTopo1 == NULL || ipTopo2 == NULL ) return NULL;

	ipTopo1->Simplify();
	ipTopo2->Simplify();

	IGeometryPtr ipRetGeom;
	if( SUCCEEDED( ipTopo1->Difference( ipAddGeomCopy, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();
	else
		AheTraceErrorMessage( _T("AheDiffrence") );

	return ipRetGeom;
}

// �W�I���g���� Intersect �����
IGeometryPtr AheIntersect( IGeometryPtr ipBaseGeom, IGeometryPtr ipTestGeom, esriGeometryDimension enumDimension/* = esriGeometry1Dimension*/, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipBaseGeomCopy( AheGetClone( ipBaseGeom ) );
	IGeometryPtr ipTestGeomCopy( AheGetClone( ipTestGeom ) );

	if( ipBaseGeomCopy == NULL || ipTestGeomCopy == NULL ) return NULL;

	if( ipSpRef != NULL )
	{
		// �n���Q�Ƃ��킹
		AheSnapToSpatialReference( ipBaseGeomCopy, ipSpRef );
		AheSnapToSpatialReference( ipTestGeomCopy, ipSpRef );
	}

	ITopologicalOperatorPtr ipTopo1( ipBaseGeomCopy );
//	ITopologicalOperatorPtr ipTopo2( ipTestGeomCopy );
	if( ipTopo1 == NULL/* || ipTopo2 == NULL*/ ) 
	{
		ATLASSERT( ipTopo1 != NULL );
		return NULL;
	}

/*	ipTopo1->Simplify();
	ipTopo2->Simplify();*/

	// ����
	IGeometryPtr ipRetGeom;

/*	if( SUCCEEDED( ipTopo1->Intersect( ipTestGeomCopy, enumDimension, &ipRetGeom ) ) )
		((ITopologicalOperatorPtr)ipRetGeom)->Simplify();*/
/*#ifdef DEBUG
	AheTraceGeometry( ipBaseGeomCopy, _T("AheIntersect : ipBaseGeometry") );
	AheTraceGeometry( ipTestGeomCopy, _T("AheIntersect : ipTestGeometry") );
#endif*/
	if( FAILED( ipTopo1->Intersect( ipTestGeomCopy, enumDimension, &ipRetGeom ) ) )
	{
		ATLASSERT( FALSE );
		AheTraceErrorMessage( _T("AheIntersect") );
		return NULL;
	}

	return ipRetGeom;
}

/////////////////////////////////////////////////////////////////////////////
// �������Z�֐�
/////////////////////////////////////////////////////////////////////////////
// �������邢�͐ڐG���Ă���|�C���g���擾����
IGeometryPtr AheGetCrossOrTouchPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode/* = 2*/, BOOL* bIsDisjoint/* = NULL*/ )
{
#ifdef DEBUG
	DWORD DWSTART = GetTickCount();
#endif
	if( ipBaseGeometry == NULL || ipTestGeometry == NULL ) return NULL;

	if( bIsDisjoint ) *bIsDisjoint = TRUE;
/*	AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipBaseGeometry, RGB( 255, 255, 0 ), 2, esriSLSSolid, TRUE, FALSE );
	AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipTestGeometry, RGB( 0, 255, 255 ), 2, esriSLSSolid, TRUE, FALSE );
	return NULL;*/
	//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipBaseGeometry, RGB(255,255,0), 8, esriSMSCircle );
	//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipTestGeometry, RGB(255,0,255), 8, esriSMSCircle );
	// iMode : 0=Cross 1=Touch 2=Both
	sindyeSpatialRel enumSpRel;
	switch( iMode )
	{
		case 0:	 enumSpRel = sindyeSpatialRelCross; break;												// Cross
		case 1:  enumSpRel = sindyeSpatialRelTouch; break;												// Touch
		case 2:  enumSpRel = (sindyeSpatialRel)(sindyeSpatialRelCross|sindyeSpatialRelTouch); break;	// Cross and Touch
		default: enumSpRel = sindyeSpatialRelNone;	break;
	}

	// �����̋�ԎQ�Ƃׂ̍����ق����炵�����l������o��
	ISpatialReferencePtr ipBaseSpRef, ipTestSpRef, ipMinimumSpRef;
	ipBaseGeometry->get_SpatialReference( &ipBaseSpRef );
	ipTestGeometry->get_SpatialReference( &ipTestSpRef );
	ipMinimumSpRef = AheGetMinimumSpatialReference( ipBaseSpRef, ipTestSpRef );
	DOUBLE dTorelance = AheGetXYDomain1UnitLength( ipMinimumSpRef );
	DOUBLE dTorelanceMin = -dTorelance, dTorelanceMax = 1.0 + dTorelance, dTorelanceMinMax = dTorelance, dTorelanceMaxMin = 1.0 - dTorelance;

	// ��x�W�I���g���R���N�V�����ɗ��Ƃ��i�j
	IGeometryCollectionPtr ipBaseGeomColCopy( AheGetClone( ipBaseGeometry ) );
	IGeometryCollectionPtr ipTestGeomColCopy( AheGetClone( ipTestGeometry ) );

	if( ipBaseGeomColCopy == NULL || ipTestGeomColCopy == NULL )
	{
		_ASSERTE( ipBaseGeomColCopy != NULL && ipTestGeomColCopy != NULL );
		return NULL;
	}

	// �����|�C���g��T��
	IPointCollectionPtr ipIntersectPoints( CLSID_Multipoint );	// �Ԓl
	((IGeometryPtr)ipIntersectPoints)->putref_SpatialReference( ipBaseSpRef ); // �x�[�X�̋�ԎQ�Ɓi�ł����́H�j��t�^

	LONG lBaseGeomCount = -1, lTestGeomCount = -1;
	IPointPtr ipBaseFromPoint( CLSID_Point ), ipBaseToPoint( CLSID_Point ), ipTestFromPoint( CLSID_Point ), ipTestToPoint( CLSID_Point );
	// ������|�C���g�͋�ԎQ�Ƃ����킹�Ă���
	ipBaseFromPoint->putref_SpatialReference( ipBaseSpRef );
	ipBaseToPoint->putref_SpatialReference( ipBaseSpRef );
	ipTestFromPoint->putref_SpatialReference( ipTestSpRef );
	ipTestToPoint->putref_SpatialReference( ipTestSpRef );

	// �g���ĂȂ��̂�
	//IPolylinePtr ipBaseSegPolyline( CLSID_Polyline ), ipTestSegPolyline( CLSID_Polyline );
	//// ��������C���͋�ԎQ�Ƃ����킹�Ă���
	//ipBaseSegPolyline->putref_SpatialReference( ipBaseSpRef );
	//ipTestSegPolyline->putref_SpatialReference( ipTestSpRef );

	ipBaseGeomColCopy->get_GeometryCount( &lBaseGeomCount );
	ipTestGeomColCopy->get_GeometryCount( &lTestGeomCount );

	// Base �̃W�I���g�����Ƃ̃��[�v
	for( LONG lBaseGeomCounter = 0; lBaseGeomCounter != lBaseGeomCount; lBaseGeomCounter++ )
	{
		IGeometryPtr ipBaseGeomCopy;
		ipBaseGeomColCopy->get_Geometry( lBaseGeomCounter, &ipBaseGeomCopy );
		if( ipBaseGeomCopy == NULL )
		{
			_ASSERTE( ipBaseGeomCopy != NULL );
			continue;
		}
		//ISegmentCollectionPtr ipBaseSegColCopy( AheGetClone( ipBaseGeometry ) ); <- ipBaseGeometry ���ƑS���ł���H
		ISegmentCollectionPtr ipBaseSegColCopy( AheGetClone( ipBaseGeomCopy ) );
		if( ipBaseSegColCopy == NULL )
		{
			_ASSERTE( ipBaseSegColCopy != NULL );
			continue;
		}
		LONG lBaseSegColCount = -1;
		ipBaseSegColCopy->get_SegmentCount( &lBaseSegColCount );

		// Base �̃Z�O�����g���Ƃ̃��[�v
		for( LONG i = 0; i < lBaseSegColCount; i++ )
		{
			_ISegmentPtr ipBaseSegTmp, ipBaseSeg;
			DOUBLE dBaseFromX = 0.0f, dBaseFromY = 0.0f, dBaseToX = 0.0f, dBaseToY = 0.0f;

			ipBaseSegColCopy->get_Segment( i, &ipBaseSegTmp );
			ipBaseSeg = AheGetClone( (_ISegmentPtr)ipBaseSegTmp );
			if( ipBaseSeg == NULL ) continue;

			// �|�C���g������
			ipBaseFromPoint->PutCoords( 0.0f, 0.0f );
			ipBaseToPoint->PutCoords( 0.0f, 0.0f );

			if( FAILED( ipBaseSeg->QueryFromPoint( ipBaseFromPoint ) ) || FAILED( ipBaseSeg->QueryToPoint( ipBaseToPoint ) ) )
			{
				ATLASSERT( FALSE );
				AheTraceErrorMessage( _T("CEditHelper::GetCrossOrTouchPoints2()") );
				continue;
			}
			ipBaseFromPoint->QueryCoords( &dBaseFromX, &dBaseFromY );
			ipBaseToPoint->QueryCoords( &dBaseToX, &dBaseToY );

/*#ifdef DEBUG
			// �Z�O�����g�n�C���C�g����
			if( ipBaseSegPolyline != NULL )
				AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipBaseSegPolyline, RGB( 0, 255, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
			// �g���ĂȂ��̂�
			//// �Z�O�����g -> �|�����C���ϊ�
			//ipBaseSegPolyline->SetEmpty();
			//((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
			//((IPointCollectionPtr)ipBaseSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipBaseToPoint ) );
/*#ifdef DEBUG
			// �Z�O�����g���n�C���C�g���Ă݂�
			AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipBaseSegPolyline, RGB( 0, 255, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
			DOUBLE Ax = (DOUBLE)( dBaseToX - dBaseFromX );
			DOUBLE Ay = (DOUBLE)( dBaseToY - dBaseFromY );

			// Test �̃W�I���g�����Ƃ̃��[�v
			for( LONG lTestGeomCounter = 0; lTestGeomCounter != lTestGeomCount; lTestGeomCounter++ )
			{
				IGeometryPtr ipTestGeomCopy;
				ipTestGeomColCopy->get_Geometry( lTestGeomCounter, &ipTestGeomCopy );
				if( ipTestGeomColCopy == NULL )
				{
					_ASSERTE( ipTestGeomCopy != NULL );
					continue;
				}
				//ISegmentCollectionPtr ipTestSegColCopy( AheGetClone( ipTestGeometry ) );	<- ipTestGeometry ���ƑS���ł���H
				ISegmentCollectionPtr ipTestSegColCopy( AheGetClone( ipTestGeomCopy ) );
				if( ipTestSegColCopy == NULL )
				{
					_ASSERTE( ipTestSegColCopy != NULL );
					continue;
				}
				LONG lTestSegColCount = -1;
				ipTestSegColCopy->get_SegmentCount( &lTestSegColCount );

				// Test �̃Z�O�����g���Ƃ̃��[�v
				for( LONG j = 0; j < lTestSegColCount; j++ )
				{
					_ISegmentPtr ipTestSegTmp, ipTestSeg;
					DOUBLE dTestFromX = 0.0f, dTestFromY = 0.0f, dTestToX = 0.0f, dTestToY = 0.0f;

					// �R�s�[�����
					ipTestSegColCopy->get_Segment( j, &ipTestSegTmp );
					ipTestSeg = AheGetClone( (_ISegmentPtr)ipTestSegTmp );
					if( ipTestSeg == NULL ) continue;

					// �|�C���g������
					ipTestFromPoint->PutCoords( 0.0f, 0.0f );
					ipTestToPoint->PutCoords( 0.0f, 0.0f );

					if( FAILED( ipTestSeg->QueryFromPoint( ipTestFromPoint ) ) || FAILED( ipTestSeg->QueryToPoint( ipTestToPoint ) ) )
					{
						ATLASSERT( FALSE );
						AheTraceErrorMessage( _T("CEditHelper::GetCrossOrTouchPoints2()") );
						continue;
					}
					ipTestFromPoint->QueryCoords( &dTestFromX, &dTestFromY );
					ipTestToPoint->QueryCoords( &dTestToX, &dTestToY );

/*#ifdef DEBUG
					// �Z�O�����g���n�C���C�g����
					if( ipTestSegPolyline != NULL )
						AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipTestSegPolyline, RGB( 255, 0, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
					// �g���ĂȂ��̂�
					//ipTestSegPolyline->SetEmpty();
					//((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
					//((IPointCollectionPtr)ipTestSegPolyline)->AddPoint( (IPointPtr)AheGetClone( ipTestToPoint ) );
/*#ifdef DEBUG
					// �Z�O�����g���n�C���C�g���Ă݂�
					AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipTestSegPolyline, RGB( 255, 0, 0 ), 4, esriSLSSolid, TRUE, TRUE );
#endif*/
					// �Z�O�����g�P�ʂŕ��s�A�ڐG�A���͊��S�ɗ���Ă��邩�ǂ����̃`�F�b�N
					// Base �� Test �� FromTo �ŁATest �� Base �� FromTo �ŃX�v���b�g���Ă݂āA�ł����畽�s�A�ڐG�ł͂Ȃ����͗���Ă��Ȃ�
//					if( ipBaseSegPolyline != NULL && ipTestSegPolyline != NULL )
//					{
						// �[�_���m���ڂ��Ă��邩�ǂ���
						BOOL bBaseFromIsTestFrom = AheIsEqual( ipBaseFromPoint, ipTestFromPoint );
						BOOL bBaseFromIsTestTo   = AheIsEqual( ipBaseFromPoint, ipTestToPoint );
						BOOL bBaseToIsTestFrom   = AheIsEqual( ipBaseToPoint, ipTestFromPoint );
						BOOL bBaseToIsTestTo     = AheIsEqual( ipBaseToPoint, ipTestToPoint );

						// ���S��v���ǂ����i���S��v�̏ꍇ�� Touch �Ƃ݂Ȃ��j
						if( bBaseFromIsTestFrom == TRUE && bBaseFromIsTestTo == TRUE && bBaseToIsTestFrom == TRUE && bBaseToIsTestTo == TRUE )
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							if( enumSpRel & sindyeSpatialRelTouch ) 
							{
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
								ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j��v�F                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j��v�F                         : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
							}
							continue;	// ���S��v�Ȃ炱���ŏI���
						}

						// �[�_���Z�O�����g��ɑ��݂��Ă��邩�ǂ���
						VARIANT_BOOL vbBaseIsSplittedByTestFrom = ( AhePointIsOnSegment( ipBaseSeg, ipTestFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
						VARIANT_BOOL vbBaseIsSplittedByTestTo   = ( AhePointIsOnSegment( ipBaseSeg, ipTestToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
						VARIANT_BOOL vbTestIsSplittedByBaseFrom = ( AhePointIsOnSegment( ipTestSeg, ipBaseFromPoint, ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );
						VARIANT_BOOL vbTestIsSplittedByBaseTo   = ( AhePointIsOnSegment( ipTestSeg, ipBaseToPoint,   ipMinimumSpRef ) == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

						// ���s���ǂ����i���s�̏ꍇ�� Touch �Ƃ݂Ȃ��j
						if( ( bBaseFromIsTestFrom == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
							( bBaseFromIsTestTo   == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) ) ||
							( bBaseToIsTestFrom   == TRUE && ( vbBaseIsSplittedByTestTo   == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
							( bBaseToIsTestTo     == TRUE && ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) ) ||
							( bBaseFromIsTestFrom == FALSE && bBaseFromIsTestTo == FALSE && bBaseToIsTestFrom == FALSE && bBaseToIsTestTo == FALSE && 
							  ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE && vbBaseIsSplittedByTestTo == VARIANT_TRUE ) ||
							    ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE && vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ||
							    ( ( vbBaseIsSplittedByTestFrom == VARIANT_TRUE || vbBaseIsSplittedByTestTo == VARIANT_TRUE ) &&
							      ( vbTestIsSplittedByBaseFrom == VARIANT_TRUE || vbTestIsSplittedByBaseTo == VARIANT_TRUE ) ) ) ) )
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							if( enumSpRel & sindyeSpatialRelTouch ) 
							{
								// ���s�̏ꍇ
								if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipTestFromPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FBaseIsSplittedByTestFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestFromX, dTestFromY );
								}
								if( vbBaseIsSplittedByTestTo == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipTestToPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FBaseIsSplittedByTestTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dTestToX, dTestToY );
								}
								if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipBaseFromPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FTestIsSplittedByBaseFrom : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
								if( vbTestIsSplittedByBaseTo == VARIANT_TRUE ) 
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									//AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipBaseFromPoint, RGB(255, 255, 50 ), 8, esriSMSCircle );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j���s�FTestIsSplittedByBaseTo   : X=%.15lf Y=%.15lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;	// ���s�Ȃ炱���ŏI���
						}
						
						// ��O�y�ш��� Base�ATest �̃Z�O�����g�� From�ATo ���擾
						_ISegmentPtr ipPrevBaseSeg, ipNextBaseSeg, ipPrevTestSeg, ipNextTestSeg;
						IPointPtr ipPrevBaseSegFromPoint, ipPrevBaseSegToPoint, ipNextBaseSegFromPoint, ipNextBaseSegToPoint;
						IPointPtr ipPrevTestSegFromPoint, ipPrevTestSegToPoint, ipNextTestSegFromPoint, ipNextTestSegToPoint;
						if( i > 0 )
						{ 
							ipBaseSegColCopy->get_Segment( i - 1, &ipPrevBaseSeg );
							if( ipPrevBaseSeg != NULL )
							{
								ipPrevBaseSeg->get_FromPoint( &ipPrevBaseSegFromPoint );
								ipPrevBaseSeg->get_ToPoint( &ipPrevBaseSegToPoint );
							}
						}
						if( i < lBaseSegColCount - 1 )
						{
							ipBaseSegColCopy->get_Segment( i + 1, &ipNextBaseSeg );
							if( ipNextBaseSeg != NULL )
							{
								ipNextBaseSeg->get_FromPoint( &ipNextBaseSegFromPoint );
								ipNextBaseSeg->get_ToPoint( &ipNextBaseSegToPoint );
							}
						}
						if( j > 0 )
						{
							ipTestSegColCopy->get_Segment( j - 1, &ipPrevTestSeg );
							if( ipPrevTestSeg != NULL )
							{
								ipPrevTestSeg->get_FromPoint( &ipPrevTestSegFromPoint );
								ipPrevTestSeg->get_ToPoint( &ipPrevTestSegToPoint );
							}
						}
						if( j < lTestSegColCount - 1 )
						{
							ipTestSegColCopy->get_Segment( j + 1, &ipNextTestSeg );
							if( ipNextTestSeg != NULL )
							{
								ipNextTestSeg->get_FromPoint( &ipNextTestSegFromPoint );
								ipNextTestSeg->get_ToPoint( &ipNextTestSegToPoint );
							}
						}

						// �[�_���m���ڂ��Ă��邪�A��v�ł����s�ł��Ȃ��ꍇ
						if( bBaseFromIsTestFrom == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base �� From �� Test �� From �ƈ�v���Ă���Ƃ�
							//
							// 1. Base �� To           ... ipBaseToPoint
							// 2. Base �� From         ... ipBaseFromPoint
							// 3. Base �̈�O�� From  ... ipPrevBaseSegFromPoint
							// 4. Test �� To           ... ipTestToPoint
							// 5. Test �̈�O�� From  ... ipPrevTestSegFromPoint
							//
							// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
							if( ipPrevBaseSeg != NULL && ipPrevTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
								else
								{
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
							}
							// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
							}
							continue;
						}
						if( bBaseFromIsTestTo == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base �� From �� Test �� To �ƈ�v���Ă���Ƃ�
							//
							// 1. Base �� To           ... ipBaseToPoint
							// 2. Base �� From         ... ipBaseFromPoint
							// 3. Base �̈�O�� From  ... ipPrevBaseSegFromPoint
							// 4. Test �� From         ... ipTestFromPoint
							// 5. Test �̈��� To    ... ipNextTestSegToPoint
							//
							// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
							if( ipPrevBaseSeg != NULL && ipNextTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipTestFromPoint        ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseToPoint, ipBaseFromPoint, ipNextTestSegToPoint   ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								} 
								else {
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
							}
							// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
							}
							continue;
						}
						if( bBaseToIsTestFrom == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base �� To �� Test �� From �ƈ�v���Ă���Ƃ�
							//
							// 1. Base �� From         ... ipBaseFromPoint
							// 2. Base �� To           ... ipBaseToPoint
							// 3. Base �̈��� To    ... ipNextBaseSegToPoint
							// 4. Test �� To           ... ipTestToPoint
							// 5. Test �̈�O�� From  ... ipPrevTestSegFromPoint
							//
							// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
							if( ipNextBaseSeg != NULL && ipPrevTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestToPoint          ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipPrevTestSegFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
								else {
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
							}
							// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;
						}
						if( bBaseToIsTestTo == TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base �� To �� Test �� To �ƈ�v���Ă���Ƃ�
							//
							// 1. Base �� From         ... ipBaseFromPoint
							// 2. Base �� To           ... ipBaseToPoint
							// 3. Base �̈��� To    ... ipNextBaseSegToPoint
							// 4. Test �� From         ... ipTestFromPoint
							// 5. Test �̈��� To    ... ipNextTestSegToPoint
							//
							// 123 �̂Ȃ��p�̊Ԃ� 124�A125 �̊p�x����������܂��͗����Ȃ��ꍇ�� Touch�A�Е��������Ă����Е����Ȃ��ꍇ�� Cross
							if( ipNextBaseSeg != NULL && ipNextTestSeg != NULL )
							{
								DOUBLE dAngle123 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextBaseSegToPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
								DOUBLE dAngle124 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipTestFromPoint      ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
								DOUBLE dAngle125 = AheGetAngle( ipBaseFromPoint, ipBaseToPoint, ipNextTestSegToPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
								if( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								} else {
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
							}
							// Base ���� Test ���J�n���͍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�[�FX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;
						}

						// ��v�ł����s�ł��Ȃ��[�_���ڂ��Ă��Ȃ��ꍇ�ŁA�Е��̂̃Z�O�����g��ɂ����Е��� From ���� To �����݂���Ƃ�
						if( vbBaseIsSplittedByTestFrom == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base �� Test �� From �ŃX�v���b�g���ꂽ�Ƃ�
							//
							// 1. Base �� From �� To �i�ǂ���ł��ǂ��j... ipBaseFromPoint
							// 2. Test �� From                        ... ipTestFromPoint
							// 3. Test �� To                          ... ipTestToPoint
							// 4. Test �̈�O�� From                ... ipPrevTestSegFromPoint
							//
							// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
							// ��O�� Test ���擾
							if( ipPrevTestSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipTestToPoint );
								DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestFromPoint, ipPrevTestSegFromPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
									}
								}
								else
								{
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
									}
								}
							}
							// Test ���J�n�Z�O�����g�Ȃ̂ŁA Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestFromX, dTestFromY );
								}
							}
							continue;
						}
						if( vbBaseIsSplittedByTestTo   == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Base �� Test �� To �ŃX�v���b�g���ꂽ�Ƃ�
							//
							// 1. Base �� From �� To �i�ǂ���ł��ǂ��j... ipBaseFromPoint
							// 2. Test �� To                          ... ipTestToPoint
							// 3. Test �� From                        ... ipTestFromPoint
							// 4. Test �̈��� To                  ... ipNextTestSegToPoint
							//
							// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
							if( ipNextTestSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipTestFromPoint );
								DOUBLE dAngle2 = AheGetAngle( ipBaseFromPoint, ipTestToPoint, ipNextTestSegToPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
									}
								} 
								else {
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
									}
								}
							}
							// Test �̍Ō�̃Z�O�����g�̏ꍇ�� Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipTestToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dTestToX, dTestToY );
								}
							}
							continue;
						}
						if( vbTestIsSplittedByBaseFrom == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Test �� Base �� From �ŃX�v���b�g���ꂽ�Ƃ�
							//
							// 1. Test �� From �� To �i�ǂ���ł��ǂ��j... ipTestFromPoint
							// 2. Base �� From                        ... ipBaseFromPoint
							// 3. Base �� To                          ... ipBaseToPoint
							// 4. Base �̈�O�� From                ... ipPrevBaseSegFromPoint
							//
							// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
							// ��O�� Base ���擾
							if( ipPrevBaseSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipBaseToPoint );
								DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseFromPoint, ipPrevBaseSegFromPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
								else {
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i�O�̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
									}
								}
							}
							// j �� 0 �̏ꍇ�͊J�n�Z�O�����g�Ȃ̂ŁA Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseFromPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseFromX, dBaseFromY );
								}
							}
							continue;
						}
						if( vbTestIsSplittedByBaseTo   == VARIANT_TRUE ) 
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							// Test �� Base �� To �ŃX�v���b�g���ꂽ�Ƃ�
							//
							// 1. Test �� From �� To �i�ǂ���ł��ǂ��j... ipTestFromPoint
							// 2. Base �� To                          ... ipBaseToPoint
							// 3. Base �� From                        ... ipBaseFromPoint
							// 4. Base �̈��� To                  ... ipNextBaseSegToPoint
							//
							// 123 �̂Ȃ��p�� 124 �̂Ȃ��p���擾���A�ǂ���������͕��̊p�x�ł���� Touch�A�ǂ��炩�����łǂ��炩�����ł���� Cross 
							// ���� Base ���擾
							if( ipNextBaseSeg != NULL )
							{
								DOUBLE dAngle1 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipBaseFromPoint );
								DOUBLE dAngle2 = AheGetAngle( ipTestFromPoint, ipBaseToPoint, ipNextBaseSegToPoint );
								if( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) )
								{
									// �N���X�����ꍇ
//									if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) 
									if( enumSpRel & sindyeSpatialRelCross )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								} else {
									// �N���X���Ȃ������ꍇ�� Touch �Ƃ݂Ȃ��i��̃Z�O�����g���ڂ��Ă����ꍇ�� Touch �ƂȂ�j
/*									if( ( enumSpRel & sindyeSpatialRelTouch ) || 
										( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
									if( enumSpRel & sindyeSpatialRelTouch )
									{
										ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
										ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
									}
								}
							}
							// i �� Base �̍Ō�̃Z�O�����g���w���Ă����ꍇ�� Touch �Ƃ���i�ⓚ���p�j
							else {
/*								if( ( enumSpRel & sindyeSpatialRelTouch ) || 
									( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == TRUE ) )*/
								if( enumSpRel & sindyeSpatialRelTouch )
								{
									ipIntersectPoints->AddPoint( (IPointPtr)AheGetClone(ipBaseToPoint) );
									ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�[�ځFX=%lf Y=%lf S=-- T=--\n"), dBaseToX, dBaseToY );
								}
							}
							continue;
						}
//					}

					DOUBLE Bx = (DOUBLE)( dTestToX - dTestFromX );
					DOUBLE By = (DOUBLE)( dTestToY - dTestFromY );
					DOUBLE DivideBase = (DOUBLE)( (DOUBLE)( Bx * Ay ) - (DOUBLE)( By * Ax ) );
					if( DivideBase == 0.0 ) 
					{
						ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ���s�Ȃ̂Ŕ�΂��܂��B") );
						continue;	// ���s
					}

					DOUBLE t = (DOUBLE)( ( (DOUBLE)(Ax * (DOUBLE)( dTestFromY - dBaseFromY )) - (DOUBLE)(Ay * (DOUBLE)( dTestFromX - dBaseFromX )) ) / DivideBase );
//					if( dTorelanceMin <= t && t <= dTorelanceMax )
					if( 0.0 < t && t < 1.0 )
					{
						//DOUBLE s  = (DOUBLE)( (DOUBLE)(( dTestFromX + (DOUBLE)( t * Bx ) - dBaseFromX )) / Ax );	<- Base ���������� Ax �� 0 �ɂȂ��ĂƂ�ł��Ȃ����Ƃ�
						DOUBLE s = (DOUBLE)( ( (DOUBLE)(Bx * (DOUBLE)( dBaseFromY - dTestFromY )) - (DOUBLE)(By * (DOUBLE)( dBaseFromX - dTestFromX )) ) / (-DivideBase) );
						if( 0.0 < s && s < 1.0 )
						{
							if( bIsDisjoint ) *bIsDisjoint = FALSE;
							BOOL bAdd = FALSE;
							DOUBLE Px = (DOUBLE)( dBaseFromX + (DOUBLE)(s * Ax) );
							DOUBLE Py = (DOUBLE)( dBaseFromY + (DOUBLE)(s * Ay) );
/*							if( ( enumSpRel & sindyeSpatialRelTouch ) && ( ( dTorelanceMin <= t && t <= dTorelanceMinMax ) || ( dTorelanceMaxMin <= t && t <= dTorelanceMax ) ) )
							{
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iTOUCH�j�����FX=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
								bAdd = TRUE;
							} else if( enumSpRel & sindyeSpatialRelCross ) {
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�����FX=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
								bAdd = TRUE;
							}*/
//							if( ( enumSpRel & sindyeSpatialRelCross ) && bTouchSideIsCross == FALSE ) {
							if( enumSpRel & sindyeSpatialRelCross ) {
								ATLTRACE(_T("CEditHelper::GetCrossOrTouchPoints2() : ��_�iCROSS�j�����FX=%lf Y=%lf S=%lf T=%lf D=%.20lf\n"), Px, Py, s, t, DivideBase );
								bAdd = TRUE;
							}
							if( bAdd == TRUE )
							{
								IPointPtr ipPoint( CLSID_Point );
								((IGeometryPtr)ipPoint)->putref_SpatialReference( ipBaseSpRef ); // �x�[�X�̋�ԎQ�Ɓi�ł����́H�j��t�^
								ipPoint->PutCoords( Px, Py );
/*#ifdef DEBUG
								// �C���^�[�Z�N�g���n�C���C�g
								AheDrawPoint( m_cArcHelper.GetScreenDisplay(FALSE), ipPoint, FALSE, 8, 100, 0, 50, esriSMSCircle, FALSE, TRUE );
								ATLTRACE(_T("CEditHelper::SplitFeaturesByGeometry() : �C���^�[�Z�N�g���n�C���C�g�i�����K�F�j\n") );
								AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipPoint, RGB(100,0,50), 8, esriSMSCircle );
#endif*/
#ifdef DEBUG
/*								// �ق�Ƃɂ���́H
								if( AheTestGeometryRelationship( ipSegPolyline,  ipPoint, (sindyeSpatialRel)(sindyeSpatialRelContain|sindyeSpatialRelTouch) ) == 0 ||
									AheTestGeometryRelationship( ipSegPolyline2, ipPoint, (sindyeSpatialRel)(sindyeSpatialRelContain|sindyeSpatialRelTouch) ) == 0 )
								{
									AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipSegPolyline, RGB( 10, 20, 255 ), 2, esriSLSSolid, TRUE, FALSE );
									AheDrawPolyline( m_cArcHelper.GetScreenDisplay(FALSE), ipSegPolyline2, RGB( 10, 20, 0 ), 2, esriSLSSolid, TRUE, FALSE );
								}*/
/*								AheTraceGeometry( ipTestSegPolyline, _T("��r�������C���iBase�j") );
								AheTraceGeometry( ipBaseSegPolyline, _T("��r�������C���iTest�j") );
								AheTraceGeometry( ipPoint, _T("����ꂽ�|�C���g") );*/
/*								AheAddLineElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipSegPolyline, RGB(100,j + i * lTestSegColCount,50), 2, esriSLSSolid );
								AheAddLineElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipSegPolyline2, RGB(100,j + i * lTestSegColCount,50), 2, esriSLSSolid );
								AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipPoint, RGB(100,j + i * lTestSegColCount,50), 8, esriSMSCircle );*/
#endif
								ipIntersectPoints->AddPoint( ipPoint );
							}
						}
					}
				}
			}
		}
	}

	// ����Ȃ���� NULL ��Ԃ�
	if( ipIntersectPoints != NULL )
	{
		LONG lIntersectCount = -1;
		ipIntersectPoints->get_PointCount( &lIntersectCount );
		if( lIntersectCount < 1 )
			ipIntersectPoints = NULL;
	}

/*#ifdef DEBUG
	AheAddPointElement( m_cArcHelper.GetGraphicsContainer(FALSE), ipIntersectPoints, RGB(0,0,255), 8, esriSMSCircle );
#endif*/
#ifdef DEBUG
	ATLTRACE(_T("AheGetCrossOrTouchPoints() : �������� %.3lf�b\n"), (GetTickCount() - DWSTART)/1000.0 );
#endif

	return ipIntersectPoints;
}

// �|�C���g���|�����C���y�у|���S���t�B�[�`���� From �|�C���g�ł��邩�ǂ����̔���
BOOL AheIsFromPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPointGeom && ipCurveFeature ) )
		return FALSE;

	IGeometryPtr ipGeom;
	ipCurveFeature->get_Shape( &ipGeom );

	return AheIsFromPoint( (IPointPtr)ipPointGeom, (ICurvePtr)ipGeom, ipSpRef );
}

// �|�C���g���|�����C���y�у|���S���� From �|�C���g�ł��邩�ǂ����̔���
BOOL AheIsFromPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPoint && ipCurve ) )
		return FALSE;

	// From �|�C���g�擾
	IPointPtr ipFromPoint;
	ipCurve->get_FromPoint( &ipFromPoint );

	return AheIsDBEqual( ipPoint, ipFromPoint );
}

// �|�C���g���|�����C���y�у|���S���t�B�[�`���� To �|�C���g�ł��邩�ǂ����̔���
BOOL AheIsToPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPointGeom && ipCurveFeature ) )
		return FALSE;

	IGeometryPtr ipGeom;
	ipCurveFeature->get_Shape( &ipGeom );

	return AheIsToPoint( (IPointPtr)ipPointGeom, (ICurvePtr)ipGeom, ipSpRef );
}

// �|�C���g���|�����C���y�у|���S���� To �|�C���g�ł��邩�ǂ����̔���
BOOL AheIsToPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPoint && ipCurve ) )
		return FALSE;

	// To �|�C���g�擾
	IPointPtr ipToPoint;
	ipCurve->get_ToPoint( &ipToPoint );

	return AheIsDBEqual( ipPoint, ipToPoint );
}

// �|�C���g���|�����C���y�у|���S���t�B�[�`���̒[�_�ł��邩�ǂ����̔���
BOOL AheIsEndPoint( IGeometryPtr ipPointGeom, IFeaturePtr ipCurveFeature, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPointGeom && ipCurveFeature ) )
		return FALSE;

	IGeometryPtr ipGeom;
	ipCurveFeature->get_Shape( &ipGeom );

	return AheIsEndPoint( (IPointPtr)ipPointGeom, (ICurvePtr)ipGeom, ipSpRef );
}

// �|�C���g���|�����C���y�у|���S���̒[�_�ł��邩�ǂ����̔���
BOOL AheIsEndPoint( IPointPtr ipPoint, ICurvePtr ipCurve, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ! ( ipPoint && ipCurve ) )
		return FALSE;

#if 0
	return  AheIsFromPoint( ipPoint, ipCurve, ipSpRef ) || 
		AheIsToPoint( ipPoint, ipCurve, ipSpRef );
#else
	// get_FromPoint�Ȃǂ���ƁA9.3.1�ł͌`��ύX���ꂽ���ƂɂȂ�悤���ibug 7860�j
	// ���傤���Ȃ��̂ŁAHitTest���g�����Ƃŉ������
	IHitTestPtr ipHitTest( ipCurve );
	IPointPtr ipHitPoint( AheCreateClone(ipPoint) );
	double dLen = AheGetXYDomain1UnitLength(AheGetSpatialReference(ipCurve)) * 10.0, dDistance = 0.0;
	long lHitPart = 0, lHitSegment = 0;
	VARIANT_BOOL vbRightSide = VARIANT_FALSE, vbHit = VARIANT_FALSE;
	if( FAILED( ipHitTest->HitTest( ipPoint, dLen, esriGeometryPartEndpoint, ipHitPoint, &dDistance, &lHitPart, &lHitSegment, &vbRightSide, &vbHit ) ) ||
		( ! vbHit ) )
		return FALSE;

	return AheIsDBEqual( ipPoint, ipHitPoint );
#endif // if 0
}

// �W�I���g���̒[�_���m���ڐG���Ă��邩�ǂ���
BOOL AheIsTouchEndPoint( ICurvePtr ipBaseCurve, ICurvePtr ipTestCurve, ISpatialReferencePtr ipSpRef/* = NULL*/, IPoint** ipTouchPoint/* = NULL*/ )
{
	if( ipBaseCurve == NULL || ipTestCurve == NULL ) return FALSE;

	// From �|�C���g�擾
	IPointPtr ipFromPoint, ipToPoint;

	if( ipSpRef == NULL )
		ipBaseCurve->get_SpatialReference( &ipSpRef );

	ipTestCurve->get_FromPoint( &ipFromPoint );
	ipTestCurve->get_ToPoint( &ipToPoint );

	BOOL bIsTouchFromPoint = AheIsEndPoint( ipFromPoint, ipBaseCurve, ipSpRef );
	BOOL bIsTouchToPoint   = AheIsEndPoint( ipToPoint,   ipBaseCurve, ipSpRef );

	if( ipTouchPoint != NULL )
	{
		if( bIsTouchFromPoint == TRUE )
			(*ipTouchPoint) = ipFromPoint;
		else if( bIsTouchToPoint == TRUE )
			(*ipTouchPoint) = ipToPoint;

		if( (*ipTouchPoint) != NULL )
			(*ipTouchPoint)->AddRef();
	}

	if( bIsTouchFromPoint == TRUE || bIsTouchToPoint == TRUE )
		return TRUE;
	else
		return FALSE;
}

// �|�C���g���W�I���g����̍\���_���ǂ����̔���
BOOL AheIsVertexPoint( IPointPtr ipPoint, IGeometryPtr ipGeom, ISpatialReferencePtr ipSpRef/* = NULL*/, LONG* pIndex/* = NULL*/ )
{
	// TODO: ���̊֐���HitTest��臒l0�ł�����W���}�C�J�H
	if( ipPoint == NULL || ipGeom == NULL ) return FALSE;
	if( pIndex != NULL ) *pIndex = -1;

	ISpatialReferencePtr ipGeomSpRef( AheGetSpatialReference( ipGeom ) );
	IPointCollectionPtr ipPointCol( ipGeom );
	if( ipPointCol == NULL )
	{
		IPointPtr ipGeomPoint( ipGeom );
		if( ipGeomPoint == NULL ) return FALSE;

		ipPointCol.CreateInstance( CLSID_Multipoint );
		((IGeometryPtr)ipPointCol)->putref_SpatialReference( ipGeomSpRef ); // ��ԎQ�Ƃ�t�^
		ipPointCol->AddPoint( ipGeomPoint );
	}

	IPointPtr ipGeomPoint( CLSID_Point );
	ipGeomPoint->putref_SpatialReference( ipGeomSpRef ); // ��ԎQ�ƕt�^
	LONG lPointCount = -1;
	ipPointCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		ipPointCol->QueryPoint( i, ipGeomPoint );
		if( AheIsEqual( ipGeomPoint, ipPoint, ipSpRef ) == TRUE )
		{
			if( pIndex != NULL ) *pIndex = i;
			return TRUE;
		}
	}
	return FALSE;
}

// �W�I���g�������Ȍ������Ă��邩�ǂ����̔���
BOOL AheIsOneselfCross( IGeometryPtr ipGeometry, BOOL bIsNetworkFeature/* = TRUE*/ )
{
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeometry ) );
	if( ipGeomCopy == NULL ) return FALSE;

	// ��ԎQ��
	ISpatialReferencePtr ipSpRef;
	ipGeomCopy->get_SpatialReference( &ipSpRef );

	// �W�I���g�� -> �Z�O�����g����
	std::list<SEGMENTITEM> listSegments;
	AheGeometry2SegmentItems( ipGeomCopy, listSegments );

	for( std::list<SEGMENTITEM>::iterator itBase = listSegments.begin(); itBase != listSegments.end(); itBase++ )
	{
		std::list<SEGMENTITEM>::iterator itTest = itBase;
		itTest++;
		for( ; itTest != listSegments.end(); itTest++ )
		{
			// �[�_���m�̐ڐG����
			BOOL bBaseFromIsTestFrom = AheIsEqual( itBase->ipFromPoint, itTest->ipFromPoint );
			BOOL bBaseFromIsTestTo   = AheIsEqual( itBase->ipFromPoint, itTest->ipToPoint );
			BOOL bBaseToIsTestFrom   = AheIsEqual( itBase->ipToPoint, itTest->ipFromPoint );
			BOOL bBaseToIsTestTo     = AheIsEqual( itBase->ipToPoint, itTest->ipToPoint );

			// �ʃp�[�g�̏ꍇ�́A��΂ɐڐG���Ă͂����Ȃ�
			if( itBase->lPartID != itTest->lPartID )
			{
				if( bBaseFromIsTestFrom == TRUE || bBaseFromIsTestTo == TRUE || bBaseToIsTestFrom == TRUE || bBaseToIsTestTo == TRUE ) return TRUE;
			}

			// ���S��v���ǂ���
			if( ( bBaseFromIsTestFrom == TRUE && bBaseToIsTestTo == TRUE ) || ( bBaseFromIsTestTo == TRUE && bBaseToIsTestFrom == TRUE ) ) return TRUE;
			
			// �[�_���Z�O�����g��ɑ��݂��Ă��邩�ǂ���
			BOOL bTestFromIsOnBase = AhePointIsOnSegment( itBase->ipSegment, itTest->ipFromPoint, ipSpRef );
			BOOL bTestToIsOnBase   = AhePointIsOnSegment( itBase->ipSegment, itTest->ipToPoint,   ipSpRef );
			BOOL bBaseFromIsOnTest = AhePointIsOnSegment( itTest->ipSegment, itBase->ipFromPoint, ipSpRef );
			BOOL bBaseToIsOnTest   = AhePointIsOnSegment( itTest->ipSegment, itBase->ipToPoint,   ipSpRef );

			// �ʃp�[�g�̏ꍇ�́A��΂ɑ��݂��Ă͂����Ȃ�
			if( itBase->lPartID != itTest->lPartID )
			{
				if( bTestFromIsOnBase == TRUE || bTestToIsOnBase == TRUE || bBaseFromIsOnTest == TRUE || bBaseToIsOnTest == TRUE ) return TRUE;
			}

			// ���s���ǂ����i���s�̏ꍇ�� Touch �Ƃ݂Ȃ��j
			if( ( bBaseFromIsTestFrom == TRUE && ( bTestToIsOnBase   == TRUE || bBaseToIsOnTest   == TRUE ) ) ||
				( bBaseFromIsTestTo   == TRUE && ( bTestFromIsOnBase == TRUE || bBaseToIsOnTest   == TRUE ) ) ||
				( bBaseToIsTestFrom   == TRUE && ( bTestToIsOnBase   == TRUE || bBaseFromIsOnTest == TRUE ) ) ||
				( bBaseToIsTestTo     == TRUE && ( bTestFromIsOnBase == TRUE || bBaseFromIsOnTest == TRUE ) ) ||
				( bBaseFromIsTestFrom == FALSE && bBaseFromIsTestTo == FALSE && bBaseToIsTestFrom == FALSE && bBaseToIsTestTo == FALSE && 
				  ( ( bTestFromIsOnBase == VARIANT_TRUE && bTestToIsOnBase == VARIANT_TRUE ) ||
					( bBaseFromIsOnTest == VARIANT_TRUE && bBaseToIsOnTest == VARIANT_TRUE ) ||
					( ( bTestFromIsOnBase == VARIANT_TRUE || bTestToIsOnBase == VARIANT_TRUE ) &&
					  ( bBaseFromIsOnTest == VARIANT_TRUE || bBaseToIsOnTest == VARIANT_TRUE ) ) ) ) ) return TRUE;

			// �ד��m�̃Z�O�����g�̏ꍇ�͂����ŏI���
			if( itBase->lPartID == itTest->lPartID && 
				( itBase->lSegmentID == itTest->lSegmentID + 1 || itBase->lSegmentID == itTest->lSegmentID - 1 ) ) continue;

			// ��O�y�ш����擾
			std::list<SEGMENTITEM>::iterator itBasePrev = itBase, itBaseNext = itBase, itTestPrev = itTest, itTestNext = itTest;
			if( itBase->bIsLastSegment  == TRUE ) itBaseNext = listSegments.end(); else itBaseNext++;
			if( itBase->bIsFirstSegment == TRUE ) itBasePrev = listSegments.end(); else itBasePrev--;
			if( itTest->bIsLastSegment  == TRUE ) itTestNext = listSegments.end(); else itTestNext++;
			if( itTest->bIsFirstSegment == TRUE ) itTestPrev = listSegments.end(); else itTestPrev--;

			// ���H�n�̏ꍇ�̂�
			if( bIsNetworkFeature == TRUE )
			{
				// �[�_���m���ڂ��Ă��邪�A��v�ł����s�ł��Ȃ��ꍇ
				if( bBaseFromIsTestFrom == TRUE )
				{
					if( itBasePrev != listSegments.end() && itTestPrev != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itBasePrev->ipFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTest->ipToPoint       ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTestPrev->ipFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseFromIsTestTo == TRUE )
				{
					if( itBasePrev != listSegments.end() && itTestNext != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itBasePrev->ipFromPoint ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTest->ipFromPoint     ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipToPoint, itBase->ipFromPoint, itTestNext->ipToPoint   ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseToIsTestFrom == TRUE )
				{
					if( itBaseNext != listSegments.end() && itTestPrev != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itBaseNext->ipToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTest->ipToPoint       ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTestPrev->ipFromPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseToIsTestTo == TRUE )
				{
					if( itBaseNext != listSegments.end() && itTestNext != listSegments.end() )
					{
						DOUBLE dAngle123 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itBaseNext->ipToPoint   ); dAngle123 = ( dAngle123 < 0 ? 360 + dAngle123 : dAngle123 );
						DOUBLE dAngle124 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTest->ipFromPoint       ); dAngle124 = ( dAngle124 < 0 ? 360 + dAngle124 : dAngle124 );
						DOUBLE dAngle125 = AheGetAngle( itBase->ipFromPoint, itBase->ipToPoint, itTestNext->ipToPoint ); dAngle125 = ( dAngle125 < 0 ? 360 + dAngle125 : dAngle125 );
						if( ! ( ( ( dAngle123 < dAngle124 ) && ( dAngle123 > dAngle125 ) ) || ( ( dAngle123 > dAngle124 ) && ( dAngle123 < dAngle125 ) ) ) ) return TRUE;
					}
					else return TRUE;
				}

				// ��ӏ��ł��[�_���m���ڂ��Ă���ꍇ�͂����ŏI���
				if( bBaseFromIsTestFrom == TRUE || bBaseFromIsTestTo == TRUE || bBaseToIsTestFrom == TRUE || bBaseToIsTestTo == TRUE ) continue;

				// ��v�ł����s�ł��Ȃ��[�_���ڂ��Ă��Ȃ��ꍇ�ŁA�Е��̂̃Z�O�����g��ɂ����Е��� From ���� To �����݂���Ƃ�
				if( bTestFromIsOnBase == TRUE )
				{
					if( itTestPrev != listSegments.end() )
					{
						DOUBLE dAngle1 = AheGetAngle( itBase->ipFromPoint, itTest->ipFromPoint, itTest->ipToPoint );
						DOUBLE dAngle2 = AheGetAngle( itBase->ipFromPoint, itTest->ipFromPoint, itTestPrev->ipFromPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bTestToIsOnBase == TRUE )
				{
					if( itTestNext != listSegments.end() )
					{
						DOUBLE dAngle1 = AheGetAngle( itBase->ipFromPoint, itTest->ipToPoint, itTest->ipFromPoint );
						DOUBLE dAngle2 = AheGetAngle( itBase->ipFromPoint, itTest->ipToPoint, itTestNext->ipToPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseFromIsOnTest == TRUE )
				{
					if( itBasePrev != listSegments.end() )
					{
						DOUBLE dAngle1 = AheGetAngle( itTest->ipFromPoint, itBase->ipFromPoint, itBase->ipToPoint );
						DOUBLE dAngle2 = AheGetAngle( itTest->ipFromPoint, itBase->ipFromPoint, itBasePrev->ipFromPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
				if( bBaseToIsOnTest == TRUE )
				{
					if( itBaseNext != listSegments.end() ) 
					{
						DOUBLE dAngle1 = AheGetAngle( itTest->ipFromPoint, itBase->ipToPoint, itBase->ipFromPoint );
						DOUBLE dAngle2 = AheGetAngle( itTest->ipFromPoint, itBase->ipToPoint, itBaseNext->ipToPoint );
						if( ! ( ( dAngle1 > 0 && dAngle2 < 0 ) || ( dAngle1 < 0 && dAngle2 > 0 ) ) ) return TRUE;
					}
					else return TRUE;
				}
			}
			else
			{
				if( bBaseFromIsTestFrom == TRUE || bBaseFromIsTestTo == TRUE || bBaseToIsTestFrom == TRUE || bBaseToIsTestTo == TRUE ||
					bTestFromIsOnBase   == TRUE || bTestToIsOnBase   == TRUE || bBaseFromIsOnTest == TRUE || bBaseToIsOnTest == TRUE )
					return TRUE;
			}
			
			if( bIsNetworkFeature == FALSE )
			{
				// �Z�O�����g���m���������Ă��邩�ǂ���
				DOUBLE Ax = (DOUBLE)( itBase->dToX - itBase->dFromX );
				DOUBLE Ay = (DOUBLE)( itBase->dToY - itBase->dFromY );
				DOUBLE Bx = (DOUBLE)( itTest->dToX - itTest->dFromX );
				DOUBLE By = (DOUBLE)( itTest->dToY - itTest->dFromY );
				DOUBLE DivideBase = (DOUBLE)( (DOUBLE)( By * Ax ) - (DOUBLE)( Bx * Ay ) );
				// �R�̎��ȂǁA���s�̏ꍇ�����邽��
				if( DivideBase != 0.0 ) 
				{
					// t�͕����_(�����_)�̔�
					DOUBLE t = (DOUBLE)( ( (DOUBLE)(Ay * (DOUBLE)( itTest->dFromX - itBase->dFromX )) - (DOUBLE)(Ax * (DOUBLE)( itTest->dFromY - itBase->dFromY )) ) / DivideBase );
					if( 0.0 < t && t < 1.0 )
					{
						// s�͕����_�̔�
						DOUBLE s = (DOUBLE)( ( (DOUBLE)(By * (DOUBLE)( itTest->dFromX - itBase->dFromX )) - (DOUBLE)(Bx * (DOUBLE)( itTest->dFromY - itBase->dFromY )) ) / DivideBase );
						if( 0.0 < s && s < 1.0 )
							return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

// �W�I���g���ɔ����Z�O�����g�����݂��邩�ǂ����̔���
BOOL AheHasMicroSegment( IGeometryPtr ipGeometry, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bFixGeometry/* = FALSE*/ )
{
	IGeometryCollectionPtr ipGeomCol( ipGeometry );
	LONG lGeomCount = -1;

	if( ipGeomCol == NULL ) return FALSE;

	// �ŏ������̌v�Z
	if( ipSpRef == NULL )
		ipGeometry->get_SpatialReference( &ipSpRef );
	DOUBLE dMicroLen = AheGetXYDomain1UnitLength( ipSpRef ) * 4.0;	// �΂߂��l������̂�

	ipGeomCol->get_GeometryCount( &lGeomCount );
	for( LONG i = 0; i < lGeomCount; i++ )
	{
		IGeometryPtr ipPartGeom;
		ipGeomCol->get_Geometry( i, &ipPartGeom );

		ISegmentCollectionPtr ipSegCol( ipPartGeom );
		if( ipSegCol != NULL )
		{
			LONG lSegCount = -1;
			ipSegCol->get_SegmentCount( &lSegCount );
			for( LONG j = 0; j < lSegCount; j++ )
			{
				_ISegmentPtr ipSegment;
				ipSegCol->get_Segment( j, &ipSegment );
				if( ipSegment == NULL )
					return TRUE;

				DOUBLE dLen = 0.0;
				if( FAILED( ipSegment->get_Length( &dLen ) ) )
					_ASSERTE( FALSE );

				if( dLen < dMicroLen )
				{
					// �C�����[�h�Ȃ�C��
					if( bFixGeometry == TRUE )
					{
						// �Z�O�����g�C���f�b�N�X+1�̍\���_���폜
						// �������A�Ō�̃Z�O�����g�̏ꍇ�ɂ͍ŏI�_���폜����Ă��܂����߁A
						// �|���S���̏ꍇ�̓����O�������A�|�����C���̏ꍇ�͑��̃t�B�[�`���Ƃ�
						// �ڑ���������邱�ƂɂȂ�̂ŃZ�O�����g�C���f�b�N�X�̍\���_���폜����
						IPointCollectionPtr ipCol( ipPartGeom );
						if( ipCol && SUCCEEDED( ipCol->RemovePoints( ( j == lSegCount - 1 ) ? j : j + 1, 1 ) ) )
						{
							// �C���ł����������x�ŏ�����`�F�b�N
							i = j = -1;
							break;
						}
						else
						{
							AheMessageBox( ERROR_FAIL_REPAIR, ERROR_GEO_IS_NOT_FIXED, MB_OK|MB_ICONWARNING );
							return TRUE;
						}
					}
					else
						return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

// �|�C���g���Z�O�����g��ɏ���Ă��邩�ǂ���
BOOL AhePointIsOnSegment( _ISegmentPtr ipBaseSegment, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef, BOOL bExeptEndPoint/* = FALSE*/ )
{
	IPolylinePtr ipBaseSegmentCopy = ToPolyline( ipBaseSegment );
	IPointPtr ipTestPointCopy( AheGetClone( ipTestPoint ) );

	if( !ipBaseSegmentCopy || !ipTestPointCopy || !ipSpRef ) return FALSE;


	// �n���ꂽ��ԎQ�Ƃł̍ŏ����������߂�i�O���b�h�̃|�C���g�Ԃ̋����j
	double dTorelance = AheGetXYDomain1UnitLength(ipSpRef);

	if (dTorelance <= 0.0)
	{
		ATLASSERT(dTorelance > 0.0);
		return FALSE;
	}
	dTorelance *= 2.0;	// �΂ߍl��

	// �|�C���g�ƃZ�O�����g�̋������v�Z���āA��ԎQ�Ƃ̍ŏ��P�ʒ� x 2�i�΂߂̃|�C���g���l������̂Łj��蒷����� FALSE
	double dPointToSegmentLen = -1.0; 

	// ��ԎQ�Ƃ��킹�i���킹�Ȃ��ƃG���[�ɂȂ�ꍇ�L��j
	ipTestPointCopy->putref_SpatialReference(ipSpRef);
	ipBaseSegmentCopy->putref_SpatialReference(ipSpRef);

	if (FAILED(IProximityOperatorPtr(ipBaseSegmentCopy)->ReturnDistance(ipTestPointCopy, &dPointToSegmentLen)))
	{
		AheTraceErrorMessage(_T("AhePointIsOnSegment() : IProximityOperator::ReturnDistance()"));
		ATLASSERT(false);
		return FALSE;
	}
	if (dPointToSegmentLen < 0.0)
	{
		ATLASSERT(dPointToSegmentLen >= 0.0);
		return FALSE;
	}
	// �Ώۂ̃|�C���g�ƃZ�O�����g��̍ŋߖT�|�C���g�̋������ŏ��P�ʁi��2�{�j�ȏゾ���������ĂȂ��Ɣ��f
	else if (dTorelance < dPointToSegmentLen)
		return FALSE;


	// [bug 8140] �ŏ��P�ʈȓ��������ꍇ�A�Z�O�����g��̍ŋߖT�_�iReturnNearestPoint�j�ƈ�v���邩�`�F�b�N���Ă������A
	// ���ꂾ�ƃr�~���E�ɃY����ꍇ���������̂ŁA���ۂɕ������Ă��Ăł����\���_�ƈ�v���邩�`�F�b�N����
	VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
	long lNewPartIndex = -1, lNewSegmentIndex = -1;
	// �����ŁAipTestPoint �� ipBaseSegment �̒[�_�������ꍇ�� vaIsSplitHappened ��FALSE�AlNewPartIndex �͂O�܂��͂P
	// �ƂȂ�Ahr �� S_OK ���Ԃ����B���̂��Ƃ̏����Ɏx��͂Ȃ����߁A���ɉ����P�A���Ȃ��B
	// SplitAtPoint�̑�����:projectOnto��FALSE�ɂ���ƕ����ʒu��ipTestPointCopy�̈ʒu�Ɉړ����Ă��܂��̂ŁATRUE�ɕύX
	HRESULT hr = ipBaseSegmentCopy->SplitAtPoint(ipTestPointCopy, VARIANT_TRUE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex);

	// �O�̂��߁A�C���f�b�N�X�͈͂��`�F�b�N�B�Z�O�����g�̂��߂O�`�Q
	if (FAILED(hr) ||
		lNewSegmentIndex < 0 ||
		lNewSegmentIndex > 2)
	{
		AheTraceErrorMessage(_T("AhePointIsOnSegment() : SplitAtPoint()"));
		ATLASSERT(false);
		return FALSE;
	}

	// �������ꂽ�\���_
	IPointPtr ipSplitedPoint;
	IPointCollectionPtr(ipBaseSegmentCopy)->get_Point(lNewSegmentIndex, &ipSplitedPoint);

	BOOL bRet = AheIsEqual(ipSplitedPoint, ipTestPointCopy);


	// ���[�_�������ꍇ�̓`�F�b�N
	if( bRet && bExeptEndPoint )
	{
		ISpatialReferencePtr ipBaseSpRef;
		ipBaseSegmentCopy->get_SpatialReference( &ipBaseSpRef );
		IPointPtr ipEndPoint( CLSID_Point );

		ipEndPoint->putref_SpatialReference( ipBaseSpRef ); // ��ԎQ�ƕt�^
		ipBaseSegmentCopy->QueryFromPoint( ipEndPoint );
		AheSnapToSpatialReference( ipEndPoint, ipSpRef );
		BOOL bEqualFrom = AheIsEqual( ipTestPointCopy, ipEndPoint );
		ipBaseSegmentCopy->QueryToPoint( ipEndPoint );
		AheSnapToSpatialReference( ipEndPoint, ipSpRef );
		BOOL bEqualTo = AheIsEqual( ipTestPointCopy, ipEndPoint );

		if( bEqualFrom == TRUE || bEqualTo == TRUE )
			return FALSE;
	}

	return bRet;
}

// �|�C���g���W�I���g����ɏ���Ă��邩�ǂ���
BOOL AhePointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef/* = NULL*/, BOOL bExeptVertex/* = FALSE*/ )
{
	if( ipBaseGeometry == NULL || ipTestPoint == NULL ) return FALSE;

	if( ipSpRef == NULL )
		ipBaseGeometry->get_SpatialReference( &ipSpRef );

	esriGeometryType geomType;

	ipBaseGeometry->get_GeometryType( &geomType );
	switch( geomType )
	{
		case esriGeometryPoint:
			return AheIsEqual( ipBaseGeometry, ipTestPoint );
			break;
		default:
		{
			// HitTest�ň�ԋ߂��Z�O�����g�擾
			DOUBLE dTorelance = AheGetXYDomain1UnitLength( ipSpRef ) * (DOUBLE)2.0;	// �������l�͍ŏ��P��x2
			LONG lPartIndex = -1, lSegmentIndex = -1;
			if( AheTestGeometryHit( ipBaseGeometry, ipTestPoint, dTorelance, NULL, NULL, &lPartIndex, &lSegmentIndex, esriGeometryPartBoundary ) == esriGeometryPartBoundary )
			{
				_ISegmentPtr ipSegment;
				try {
					IGeometryPtr ipPartGeom;
					IGeometryCollectionPtr ipGeomCol( ipBaseGeometry );
					ipGeomCol->get_Geometry( lPartIndex, &ipPartGeom );
					((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lSegmentIndex, &ipSegment );


				}
				catch( _com_error e )
				{
					DEBUGPRINT( _T("AhePointIsOnGeometry() : ��O : %s\n"), e.ErrorMessage() );
				}

				if (ipSegment)
					return AhePointIsOnSegment(ipSegment, ipTestPoint, ipSpRef, bExeptVertex);
			}
		}
		break;
	}

	return FALSE;
}

// �x�[�X�̃W�I���g����ɂ̂��Ă���e�X�g�̃W�I���g���̍\���_��Ԃ�
IPointCollectionPtr AheGetOnGeometryPoints( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IPointCollectionPtr ipPointCol( ipTestGeometry ), ipRetPointCol;
	if( ipBaseGeometry == NULL || ipPointCol == NULL )
	{
		_ASSERTE( ipBaseGeometry != NULL && ipPointCol != NULL );
		return NULL;
	}

	IPointPtr ipPoint( CLSID_Point );
	((IGeometryPtr)ipPoint)->putref_SpatialReference( AheGetSpatialReference( ipTestGeometry ) ); // QueryPoint��Test������̂ŁATest�̋�ԎQ�Ƃ�t�^
	LONG lPointCount = -1;

	ipPointCol->get_PointCount( &lPointCount );
	for( LONG i = 0; i < lPointCount; i++ )
	{
		ipPointCol->QueryPoint( i, ipPoint );
		if( AhePointIsOnGeometry( ipBaseGeometry, ipPoint, ipSpRef ) )
		{
			if( ! ipRetPointCol )
			{
				ipRetPointCol.CreateInstance( CLSID_Multipoint );
				((IGeometryPtr)ipRetPointCol)->putref_SpatialReference( AheGetSpatialReference( ipTestGeometry ) ); // �������R��Test�̋�ԎQ�Ƃ�t�^
			}
			ipRetPointCol->AddPoint( (IPointPtr)AheGetClone( ipPoint ) );
		}
	}

	return ipRetPointCol;
}

// �x�[�X�̃W�I���g����ɒǉ��p�\���_������Ă���̂ł���΁A�\���_��ǉ�����
BOOL AheAddVertexIfOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipAddVertexPoint, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	if( ipBaseGeometry == NULL || ipAddVertexPoint == NULL )
	{
		_ASSERTE( ipBaseGeometry != NULL && ipAddVertexPoint != NULL );
		return FALSE;
	}

	if( AhePointIsOnGeometry( ipBaseGeometry, ipAddVertexPoint, ipSpRef, TRUE ) == TRUE )
	{
		IPolycurvePtr ipPolycurve( ipBaseGeometry );
		if( ipPolycurve == NULL )
		{
			_ASSERTE( ipPolycurve != NULL );
			return FALSE;
		}

		VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
		LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
		if( FAILED( ipPolycurve->SplitAtPoint( ipAddVertexPoint, VARIANT_FALSE, VARIANT_FALSE, &vaIsSplitHappened, &lNewPartIndex, &lNewSegmentIndex ) ) )
		{
			_ASSERTE( FALSE );
			AheTraceErrorMessage( _T("AheAddVertexIfOnGeometry() : SplitAtPoint ���s") );
			return FALSE;
		}
		if( vaIsSplitHappened != VARIANT_TRUE )
		{
			_ASSERTE( vaIsSplitHappened == VARIANT_TRUE );	// AhePointIsOnGemetry() �� TRUE ���A���Ă��Ă���񂾂���A�K���������Ȃ��Ƃ���
			return FALSE;
		}
		ATLTRACE(_T("AheAddVertexIfOnGeometry() : �\���_�ǉ��F SPLIT=%s NewPartIndex=%d NewSegmentIndex=%d\n"), (vaIsSplitHappened == VARIANT_TRUE ? _T("TRUE") : _T("FALSE")), lNewPartIndex, lNewSegmentIndex );

		return TRUE;
	}

	return FALSE;
}

// �x�[�X�̃W�I���g����ɒǉ��p�\���_������Ă���̂ł���΁A�\���_��ǉ�����
BOOL AheAddVertexesIfOnGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipAddVertexPoints, ISpatialReferencePtr ipSpRef/* = NULL */ )
{
	IPointCollectionPtr ipAddVertexPointCollection( ipAddVertexPoints );
	if( ! ipAddVertexPointCollection )
	{
		if( ipAddVertexPoints )
		{
			esriGeometryType type = esriGeometryAny;
			ipAddVertexPoints->get_GeometryType( &type );
			if( esriGeometryPoint == type )
			{
				ipAddVertexPointCollection.CreateInstance( CLSID_Multipoint );
				((IGeometryPtr)ipAddVertexPointCollection)->putref_SpatialReference( AheGetSpatialReference( ipAddVertexPoints ) ); // Point->Multipoint�Ȃ̂ŁAPoint�̋�ԎQ�Ƃ�t�^
				ipAddVertexPointCollection->AddPoint( (IPointPtr)ipAddVertexPoints );
			}
		}
	}

	if( !ipBaseGeometry || !ipAddVertexPointCollection )
	{
		_ASSERTE( ipBaseGeometry && ipAddVertexPointCollection );
		return NULL;
	}

	// �]���̂悤�Ƀ|�C���g1����SplitAtPoint������@�ł�1�Z�O�����g�ɕ����̃|�C���g���z�u�����ꍇ��
	// �\���_���ǉ�����Ȃ���肪���邽�߁ASplitAtPoints�ŏ�������	[bug 9790]
	IPolycurve2Ptr ipPolycurve( ipBaseGeometry );
	if( !ipPolycurve )
	{
		_ASSERTE( ipPolycurve );
		return FALSE;
	}
	
	IEnumVertexPtr ipEnumVertex;
	ipAddVertexPointCollection->get_EnumVertices(&ipEnumVertex);
	IEnumSplitPointPtr ipEnumSplitPoint;
	
	// 臒l�͋�ԎQ�Ƃ̍ŏ��P�ʂ�2�{��(AhePointIfOnGeometry�ɏ���)
	DOUBLE cutOffDistance = AheGetXYDomain1UnitLength(AheGetSpatialReference(ipAddVertexPoints)) * (DOUBLE)2.0;

	if( FAILED( ipPolycurve->SplitAtPoints( ipEnumVertex, VARIANT_FALSE, VARIANT_FALSE, cutOffDistance, &ipEnumSplitPoint ) ) )
	{
		_ASSERTE( FALSE );
		AheTraceErrorMessage( _T("AheAddVertexesIfOnGeometry() : SplitAtPoints ���s") );
		return FALSE;
	}

	VARIANT_BOOL vaIsSplitHappened = VARIANT_FALSE;
	ipEnumSplitPoint->get_SplitHappened(&vaIsSplitHappened);
	if( !vaIsSplitHappened )
	{
		return FALSE;
	}

#ifdef _DEBUG
	// �����_�̃��|�[�g
	ipEnumSplitPoint->Reset();
	IPointPtr ipPoint;
	LONG lNewPartIndex = -1, lNewVertextIndex = -1;
	while( SUCCEEDED( ipEnumSplitPoint->NextInSequence(&ipPoint, &lNewPartIndex, &lNewVertextIndex ) ) && ipPoint ){
		WKSPoint point = {};
		ipPoint->QueryCoords(&(point.X), &(point.Y));
		ATLTRACE(_T("AheAddVertexesIfOnGeometry() : �\���_�ǉ��FNewPartIndex=%d NewVertextIndex=%d (%.12f,%.12f)\n")
			, lNewPartIndex, lNewVertextIndex, point.X, point.Y );
	}
#endif

	return TRUE;
}

// �|�C���g�ړ����Ƀ��o�[�o���h���ƂȂ肤��Z�O�����g��Ԃ�
ISegmentCollectionPtr AheGetRubberBandSegment( IGeometryPtr ipGeom, IPointPtr ipSplitPoint, ISpatialReferencePtr ipSpRef/* = NULL*/ )
{
	IGeometryPtr ipGeomCopy( AheGetClone( ipGeom ) );
	ISegmentCollectionPtr ipSegmentCol( CLSID_Polyline );
	((IGeometryPtr)ipSegmentCol)->putref_SpatialReference( AheGetSpatialReference( ipGeom ) ); // ipGeom�̃Z�O�����g������̂ŁAipSpRef�ł͖���ipGeom�̋�ԎQ�Ƃ�t�^
	esriGeometryType enumGeomType;

	if( ipGeomCopy == NULL || ipSplitPoint == NULL ) return ipSegmentCol;

	if( ipSpRef == NULL )
		ipGeom->get_SpatialReference( &ipSpRef );
	ipGeom->get_GeometryType( &enumGeomType );
	ipSplitPoint->putref_SpatialReference( ipSpRef );
	ipSplitPoint->SnapToSpatialReference();

	if( !( enumGeomType == esriGeometryPolyline || enumGeomType == esriGeometryPolygon ) ) return ipSegmentCol;

	// �e�p�[�g�ɕ������
	IGeometryCollectionPtr ipGeomCol( ipGeomCopy );
	LONG lGeomCount = -1;
	ipGeomCol->get_GeometryCount( &lGeomCount );
	for( LONG i = 0; i < lGeomCount; ++i )
	{
		IGeometryPtr ipPartGeom;
		ipGeomCol->get_Geometry( i, &ipPartGeom );

		esriGeometryType geomType;
		LONG lPartPointCount = -1;
		LONG lIndex = -1;

		ipPartGeom->get_GeometryType( &geomType );
		((IPointCollectionPtr)ipPartGeom)->get_PointCount( &lPartPointCount );
		
		// �\���_���ǂ������m�F
		if( AheIsVertexPoint( ipSplitPoint, ipPartGeom, ipSpRef, &lIndex ) == TRUE )
		{
			_ISegmentPtr ipSegment;
			switch( geomType )
			{
			// Path��������ŏ��̓_��n���ăZ�O�����g���쐬
			case esriGeometryPath:
				((ISegmentCollectionPtr)ipPartGeom)->get_Segment( 0, &ipSegment );
				ipSegmentCol->AddSegment( ipSegment );
				break;

			// Ring��������\���_��͂񂾂Ƃ��Q�̃Z�O�����g�����o�[�o���h�ɂȂ�
			case esriGeometryRing:
				if( 0 <= lIndex && lIndex < lPartPointCount )
				{
					// 1�߂̃Z�O�����g
					((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lIndex, &ipSegment );
					ipSegmentCol->AddSegment( ipSegment );

					// �����ЂƂ̃Z�O�����g
					// �J�n�_�Ȃ�Ō�̓_��n���Ď擾����
					long lOtherIndex = (lIndex == 0) ? lPartPointCount - 2 : lIndex - 1;
					((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lOtherIndex, &ipSegment );
					ipSegmentCol->AddSegment( ipSegment );
				}
				break;

			default:
				break;
			}

			// [Bug 4972] 2�_�ȉ��ō\������Ă���t�B�[�`����ҏW����Ɨ�����

			//if( 0 <= lIndex && lIndex < lPartPointCount )
			//{
			//	_ISegmentPtr ipSegment1;
			//	((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lIndex, &ipSegment1 );
			//	ipSegmentCol->AddSegment( ipSegment1 );
			//}

			//// �J�n�_�̎��́A Ring �̎�������ԍŌ�������
			//LONG lOtherIndex = -1;
			//if( lIndex == 0 )
			//{
			//	if( geomType == esriGeometryRing )
			//		lOtherIndex = lPartPointCount - 2;
			//}
			//else
			//	lOtherIndex = lIndex - 1;

			//if( lOtherIndex > 0 )
			//{
			//	_ISegmentPtr ipSegment2;
			//	((ISegmentCollectionPtr)ipPartGeom)->get_Segment( lOtherIndex, &ipSegment2 );
			//	ipSegmentCol->AddSegment( ipSegment2 );
			//}
		}
		else {
			// �|�C���g���W�I���g���ɍڂ��Ă���ꍇ�Ɍ���
			if( AhePointIsOnGeometry( ipPartGeom, ipSplitPoint, ipSpRef ) == TRUE )
			{
				// IRing, IPath > IPolycurve �ϊ�
				IPolycurvePtr ipPolycurve;
				switch( geomType )
				{
					case esriGeometryPath: ipPolycurve.CreateInstance( CLSID_Polyline ); break;
					case esriGeometryRing: ipPolycurve.CreateInstance( CLSID_Polygon  ); break;
					default: _ASSERTE(FALSE);	// ���̌^������킯���Ȃ�
				}
				ipPolycurve->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^
				WKSPoint* pPoints = NULL;
				IPointCollectionPtr ipPointColTmp( ipPartGeom );
				LONG lPointCount = -1;
				ipPointColTmp->get_PointCount( &lPointCount );
				pPoints = new WKSPoint[lPointCount];
				ipPointColTmp->QueryWKSPoints( 0, lPointCount, pPoints );
				((IPointCollectionPtr)ipPolycurve)->SetWKSPoints( lPointCount, pPoints );
				delete [] pPoints;
				switch( geomType )
				{
					case esriGeometryPath: ((IPolylinePtr)ipPolycurve)->SimplifyNetwork();        break;
					case esriGeometryRing: ((IPolygonPtr )ipPolycurve)->SimplifyPreserveFromTo(); break;
					default: _ASSERTE(FALSE);	// ���̌^������킯���Ȃ�
				}
				// �؂��Ă݂�
				VARIANT_BOOL vaSplitHappened = VARIANT_FALSE;
				LONG lNewPartIndex = -1, lNewSegmentIndex = -1;
				ipPolycurve->SplitAtPoint( ipSplitPoint, VARIANT_FALSE, VARIANT_FALSE, &vaSplitHappened, &lNewPartIndex, &lNewSegmentIndex );
				
				if( vaSplitHappened == VARIANT_TRUE && lNewSegmentIndex > 0 )
				{
					_ISegmentPtr ipSegment1;
					((ISegmentCollectionPtr)ipPolycurve)->get_Segment( lNewSegmentIndex, &ipSegment1 );
					ipSegmentCol->AddSegment( ipSegment1 );

					_ISegmentPtr ipSegment2;
					((ISegmentCollectionPtr)ipPolycurve)->get_Segment( lNewSegmentIndex - 1, &ipSegment2 );
					ipSegmentCol->AddSegment( ipSegment2 );
				}
				else if( lNewSegmentIndex == 0 )
					ATLASSERT( FALSE );	// �Ȃ��Ƃ�����
			}
		}
	}

	return ipSegmentCol;
}

IGeometryPtr AheSetKnownSimple(IGeometryPtr ipGeometry)
{
	ITopologicalOperator2Ptr ipTopo2 = ipGeometry;

	if ( ipTopo2 ) {
		VARIANT_BOOL vbIsKnownSimple = VARIANT_FALSE;

		if ( SUCCEEDED( ipTopo2->get_IsKnownSimple( &vbIsKnownSimple ) ) ) {
			if ( !vbIsKnownSimple ) {
				if ( FAILED( ipTopo2->put_IsKnownSimple( VARIANT_TRUE ) ) )
					_ASSERTE(false); // put_IsKnownSimple�Ɏ��s
			}
		} else
			_ASSERTE(false); // get_IsKnownSimple�Ɏ��s
	}

	return ipGeometry;
}

IGeometryPtr AheRemoveAdjoiningPoint(IGeometryPtr ipGeometry)
{
	AheTraceGeometry( ipGeometry, _T("AheRemoveAdjoiningPoint�O()") );

	IGeometryPtr ipRetGeometry = ipGeometry;

	if ( ipRetGeometry ) {
		esriGeometryType geomType;

		ipRetGeometry->get_GeometryType( &geomType );

		IGeometryCollectionPtr ipGeomColl( ipRetGeometry );

		if ( ipGeomColl && geomType != esriGeometryMultipoint ) {
			long lGeomCount = 0;

			ipGeomColl->get_GeometryCount( &lGeomCount );

			for ( long i = 0; i < lGeomCount; ++i ) {
				IGeometryPtr ipGeom;

				ipGeomColl->get_Geometry( i, &ipGeom );
				ipGeom = AheRemoveAdjoiningPointProc( ipGeom );
			}
		} else
			ipRetGeometry = AheRemoveAdjoiningPointProc( ipRetGeometry );
	}

	AheTraceGeometry( ipRetGeometry, _T("AheRemoveAdjoiningPoint��()") );

	return ipRetGeometry;
}

IGeometryPtr AheRemoveAdjoiningPointProc(IGeometryPtr ipGeometry)
{
	AheTraceGeometry( ipGeometry, _T("AheRemoveAdjoiningPointProc�O()") );

	IGeometryPtr ipRetGeometry = ipGeometry;
	IPointCollectionPtr ipPointColl( ipRetGeometry );

	if ( ipPointColl ) {
		// �|�C���g���擾
		long lPointCount = 0;

		ipPointColl->get_PointCount( &lPointCount );

		// �����|�C���g�����鎞�̂�
		if ( lPointCount > 1 ) {
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			esriGeometryType emType = esriGeometryAny;
			ipGeometry->get_GeometryType( &emType );

			CLSID clsid = CLSID_NULL;
			switch ( emType )
			{
				case esriGeometryRing: clsid = CLSID_Ring; break;
				case esriGeometryPath: clsid = CLSID__Path; break;
				case esriGeometryPolygon: clsid = CLSID_Polygon; break;
				case esriGeometryPolyline: clsid = CLSID_Polyline; break;
				case esriGeometryLine: clsid = CLSID_Line; break;
				case esriGeometryEnvelope: clsid = CLSID_Envelope; break;
				case esriGeometryMultipoint: clsid = CLSID_Multipoint; break;
				case esriGeometryBag: clsid = CLSID_GeometryBag; break;
				default: return ipRetGeometry;
			}

			long lNewPointCount = 0;
			IPointCollectionPtr ipNewPointColl( clsid );
			((IGeometryPtr)ipNewPointColl)->putref_SpatialReference( AheGetSpatialReference( ipGeometry ) );
			for ( long i = 0; i < lPointCount; ++i ) {
				IPointPtr ipPointSrc;
				ipPointColl->get_Point( i, &ipPointSrc );

				bool bIsNoSame = false;

				if ( i+1 >= lPointCount ) {
					// �Ō�̓_�͕K���ǉ�
					bIsNoSame = true;
				} else {
					IPointPtr ipPointDest;
					ipPointColl->get_Point( i+1, &ipPointDest );

					// �ׂ荇���Ă���_���������Ȃ��H
					if ( ! AheIsEqual( ipPointSrc, ipPointDest ) )
						bIsNoSame = true;
				}

				if ( bIsNoSame ) {
					// �i�[���悤�Ƃ��Ă���_�ƐV�K�|�C���g�Q���������Ȃ��_��V�K�|�C���g�Q�ɐݒ�B
					for ( long j = 0; j < lNewPointCount; ++j ) {
						IPointPtr ipPointDest;
						if ( j+1 >= lNewPointCount ) {
							// �Ō�̓_�͈�O�̓_�Ɣ�r
							ipPointColl->get_Point( j, &ipPointDest );
						} else {
							ipPointColl->get_Point( j+1, &ipPointDest );
						}

						// �ׂ荇���Ă���_���������H
						if ( AheIsEqual( ipPointSrc, ipPointDest ) )
							bIsNoSame = false;
					}

					if ( bIsNoSame ) {
						// �V�K�|�C���g�Q�i�[
						ipNewPointColl->AddPoint( ipPointSrc );
						++lNewPointCount;
					}
				}
			}

			// 0���Ă��Ƃ͑S�������_�Ȃ͂�
			if ( lNewPointCount == 0 ) {
				IPointPtr ipPointSrc;
				ipPointColl->get_Point( 0, &ipPointSrc );
				// �n�_�i�[
				ipNewPointColl->AddPoint( ipPointSrc );
				++lNewPointCount;
			}

			ipPointColl->SetPointCollection( ipNewPointColl );
#else
			// �V�K�|�C���g�Q�̈�m��
			WKSPoint* wksNew = new WKSPoint[lPointCount];

			// �|�C���g�Q�擾
			WKSPoint* wks = new WKSPoint[lPointCount];

			ipPointColl->QueryWKSPoints( 0, lPointCount, wks );

			// �ׂ荇���Ă���_���������Ȃ��_��V�K�|�C���g�Q�ɐݒ�B
			long lNewPointCount = 0;

			for ( long i = 0; i < lPointCount; ++i ) {
				WKSPoint wksSrc={0}, wksDest={0};
				bool bIsNoSame = false;

				wksSrc = wks[i];

				if ( i+1 >= lPointCount ) {
					// �Ō�̓_�͕K���ǉ�
					bIsNoSame = true;
				} else {
					wksDest = wks[i+1];

					// �ׂ荇���Ă���_���������Ȃ��H
					if ( wksSrc.X != wksDest.X || wksSrc.Y != wksDest.Y )
						bIsNoSame = true;
				}

				if ( bIsNoSame ) {
					// �i�[���悤�Ƃ��Ă���_�ƐV�K�|�C���g�Q���������Ȃ��_��V�K�|�C���g�Q�ɐݒ�B
					for ( long j = 0; j < lNewPointCount; ++j ) {
						if ( j+1 >= lNewPointCount ) {
							// �Ō�̓_�͈�O�̓_�Ɣ�r
							wksDest = wksNew[j];
						} else {
							wksDest = wksNew[j+1];
						}

						// �ׂ荇���Ă���_���������H
						if ( wksSrc.X == wksDest.X && wksSrc.Y == wksDest.Y )
							bIsNoSame = false;
					}

					if ( bIsNoSame ) {
						// �V�K�|�C���g�Q�i�[
						wksNew[lNewPointCount] = wksSrc;
						++lNewPointCount;
					}
				}
			}

			// 0���Ă��Ƃ͑S�������_�Ȃ͂�
			if ( lNewPointCount == 0 ) {
				// �n�_�i�[
				wksNew[lNewPointCount] = wks[0];
				++lNewPointCount;
			}

			// �V�K�|�C���g�Q���i�[
			ipPointColl->SetWKSPoints( lNewPointCount, wksNew );

			// �|�C���g�Q�̈����
			delete [] wksNew;
			delete [] wks;
#endif
		}
	}

	// �Ȃ���VARIANT_FALSE�ɂȂ�̂�
	AheSetKnownSimple( ipRetGeometry );

	AheTraceGeometry( ipRetGeometry, _T("AheRemoveAdjoiningPointProc��()") );

	return ipRetGeometry;
}

/////////////////////////////////////////////////////////////////////////////
// �l�b�g���[�N�n�`�F�b�N�Ŏg�p����֐�
/////////////////////////////////////////////////////////////////////////////

// �\���_���m���ڐG���Ă��Ȃ����ǂ������ׂ�
bool AheIsTouchVertex( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom, IPointPtr& ipTouch )
{
	if( ! ( ipBaseGeom && ipTestGeom ) )
		return false;

	bool ret = false; // �߂�l
	IEnumVertexPtr ipBaseEnum, ipTestEnum;
	ipBaseGeom->get_EnumVertices( &ipBaseEnum );
	ipTestGeom->get_EnumVertices( &ipTestEnum );
	ipBaseEnum->Reset();
	long baseCount = 0, testCount = 0;
	ipBaseGeom->get_PointCount( &baseCount );
	ipTestGeom->get_PointCount( &testCount );

	long basePart = 0, baseVert = 0, baseTotal = 0;
	IPointPtr ipBaseP;
	while( S_OK == ipBaseEnum->Next( &ipBaseP, &basePart, &baseVert ) )
	{
		if( ! ipBaseP )
			break;
		++baseTotal;

		ipTestEnum->Reset();
		long testPart = 0, testVert = 0, testTotal = 0;
		IPointPtr ipTestP;
		while( S_OK == ipTestEnum->Next( &ipTestP, &testPart, &testVert ) )
		{
			if( ! ipTestP )
				break;
			++testTotal;

			// �l�b�g���[�N�n�I�u�W�F�N�g���m�̍\���_���m�̐ڐG����Ɏg�p�ibug 8463 ���[�h�`�F�b�N2320�j
			// �n�_���m�A�I�_���m�͔�΂�
			// �}���`�p�[�g�̏ꍇ�͂��̊֐��̎g�p�Ӌ`����O���̂ŁA�����
			if( 0 == basePart && 0 == baseVert && 0 == testPart && 0 == testVert || // �����n�_
				baseCount == baseTotal && testCount == testTotal ||                 // �����I�_
				0 == basePart && 0 == baseVert && testCount == testTotal ||         // base�n�_test�I�_
				0 == testPart && 0 == testVert && baseCount == baseTotal )          // base�I�_test�n�_
				continue;

			if( AheIsDBEqual( ipBaseP, ipTestP ) )
			{
				ipTouch = AheCreateClone( ipBaseP );
				return true;
			}
		}
	}
	return false;
}

// �[�_���ڐG���Ă���`��̑��Z�O�����g�Ƃ̋�����Ԃ��܂��B
double AheGetConnectSegmentCrack( IPointCollectionPtr& ipBaseGeom, IPointCollectionPtr& ipTestGeom )
{
	double ret = -1.0; // �߂�l

	if( ! ( ipBaseGeom && ipTestGeom ) )
		return ret;

	IPointPtr ipBaseFrom, ipBaseTo, ipTestFrom, ipTestTo, ipTestPoint;
	long baseCount = 0, testCount = 0;
	ipBaseGeom->get_PointCount( &baseCount );
	ipTestGeom->get_PointCount( &testCount );
	if( ! ( baseCount > 0 && testCount > 0 ) )
		return ret;

	ipBaseGeom->get_Point( 0, &ipBaseFrom );
	ipBaseGeom->get_Point( baseCount -1, &ipBaseTo );
	ipTestGeom->get_Point( 0, &ipTestFrom );
	ipTestGeom->get_Point( testCount -1, &ipTestTo );
	if( AheIsDBEqual( ipBaseFrom, ipTestFrom ) ||
		AheIsDBEqual( ipBaseTo,   ipTestFrom ) )
		ipTestGeom->get_Point( 1, &ipTestPoint );
	else if( AheIsDBEqual( ipBaseFrom, ipTestTo ) ||
		AheIsDBEqual( ipBaseTo, ipTestTo ) )
		ipTestGeom->get_Point( testCount -2, &ipTestPoint );
	else
		return ret; // �ڐG���Ă��Ȃ�

	// �`�F�b�N�d�l�ł͖@�������Ə����Ă��邪�A�����ł͒P����
	// �Z�O�����g�[�_�ƃ��C���̋������o���Ă���B
	// ����́A�@���������o���̂��ʓ|�Ȃ̂ƁAbase �� test ��
	// ����ւ��邱�Ƃł�������̃`�F�b�N�ł͕K���@���ƂȂ邽
	// �߂ł���B
	IProximityOperatorPtr ipProx( ipBaseGeom );
	IPointPtr ipNearPoint;
	ipProx->ReturnNearestPoint( ipTestPoint, esriNoExtension, &ipNearPoint );

	double dx1 = 0.0, dy1 = 0.0, dx2 = 0.0, dy2 = 0.0;
	ipTestPoint->QueryCoords( &dx1, &dy1 );
	ipNearPoint->QueryCoords( &dx2, &dy2 );
	ret = AheGETDIST( dx1, dy1, dx2, dy2 );

	return ret*100.0;
}

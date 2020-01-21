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
 * @file AheGeometryConvert.cpp
 * @brief <b>�W�I���g���R���o�[�g�n�����t�@�C��</b>\n
 * @author �R���e���c�{���Z�p�J�������암�J���O���[�v �Ð�M�G
 * ���̊֐��Q�́ASiNDYLib��GeometryConvert.cpp���Q�l�ɍ��܂����B
 * @version $Id$
 */
#include "stdafx.h"
#include "AheGeometryConvert.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �W�I���g������}���`�|�C���g�𐶐�����
IGeometryPtr ToMultipoint( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Multipoint );
	IPointCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryPoint:
			// Point�AMultiPoint�̓|�C���g���擾����ꍇ�ɃR�s�[�ł͂Ȃ��Q�Ƃ�Ԃ�
			// ���̊֐����̂ǂ�����Ă΂�邩�킩��Ȃ����߁A�|�C���g�̏ꍇ��Clone���쐬���������ǂ�
			ipRetCol->AddPoint( (IPointPtr)AheCreateClone( (IUnknownPtr)ipGeom ) );
			break;
		case esriGeometryPolygon:
		case esriGeometryRing:
		case esriGeometryEnvelope:
		case esriGeometryPolyline:
		case esriGeometryPath:
		{
			IPointCollectionPtr ipCol( ipGeom );
			long lPointCount = 0;
			WKSPoint* pPoints = NULL;
			if( SUCCEEDED( hr = ipCol->get_PointCount( &lPointCount ) ) )
			{
				if( NULL != ( pPoints = new WKSPoint[lPointCount] ) )
				{
					if( SUCCEEDED( hr = ipCol->QueryWKSPoints( 0, lPointCount, pPoints ) ) )
						ATLVERIFY( SUCCEEDED( hr = ipRetCol->SetWKSPoints( lPointCount, pPoints ) ) );
					else
						_ASSERTE( SUCCEEDED(hr) );
				}
				else
					_ASSERTE( false );
			}
			else
				_ASSERTE( SUCCEEDED(hr) );

			if( pPoints ) 
				delete [] pPoints;

			break;
		}
		case esriGeometryMultipoint: 
			ipRet = AheCreateClone( ipGeom ); 
			break;
		default:
			_ASSERTE(false); // ���T�|�[�g
			break;
	}

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// �W�I���g������|�����C���𐶐�����
IGeometryPtr ToPolyline( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Polyline );
	IGeometryCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryPath:
		case esriGeometryRing:
			ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( (IGeometryPtr)AheCreateClone( (IUnknownPtr)ipGeom ) ) ) );
			break;
		case esriGeometryEnvelope:
			ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToPath( ipGeom ) ) ) );
			break;
		case esriGeometryPolygon:
		{
			IGeometryCollectionPtr ipGeomCol( ipGeom );
			long lGeomCount = 0;
			if( SUCCEEDED( hr = ipGeomCol->get_GeometryCount( &lGeomCount ) ) )
			{
				for( long i = 0; i < lGeomCount; ++i )
				{
					IGeometryPtr ipPart;
					if( SUCCEEDED( hr = ipGeomCol->get_Geometry( i, &ipPart ) ) )
						ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToPath( ipPart ) ) ) );
					else
						_ASSERTE( SUCCEEDED(hr) );
				}
			}
			else
				_ASSERTE( SUCCEEDED(hr) );
			break;
		}
		case esriGeometryLine:
		case esriGeometryCircularArc:
			ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToPath( ipGeom ) ) ) );
			break;
		case esriGeometryPolyline:
			ipRet = AheCreateClone( ipGeom ); 
			break;
		default:
			_ASSERTE( false ); // ���T�|�[�g
			break;
	}

	// KnownSimple�ȃW�I���g�����ϊ����KnownSimple����Ȃ��Ȃ�̂ŁA������
	// KnownSimple�ɂ��Ă��iKnownSimple����Ȃ���WKB�֘A�̃��\�b�h�����s�H����炵���j
	((ITopologicalOperator2Ptr)ipRet)->put_IsKnownSimple( /*vbIsKnownSimple*/VARIANT_TRUE );

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// �W�I���g������p�X�𐶐�����
IGeometryPtr ToPath( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	switch( emType )
	{
		case esriGeometryRing: // Ring -> Path
		{
			// ���� Ring -> Path �ɕϊ��ł�����̂� ICurve::GetSubCurve ��������Ȃ��c
			double dLen = 0.0;
			ICurvePtr ipCurve( ipGeom );
			ipCurve->get_Length( &dLen );
			ICurvePtr ipPath;
			if( SUCCEEDED( hr = ipCurve->GetSubcurve( 0.0, dLen, VARIANT_FALSE, &ipPath ) ) )
				ipRet = ipPath;
			else
				_ASSERTE( SUCCEEDED(hr) );
			break;
		}
		case esriGeometryPath: // Path -> Path
			ipRet = AheCreateClone( ipGeom );
			break;
		default:
			ipRet.CreateInstance( CLSID__Path );
			IPointCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
			ipRet->putref_SpatialReference( ipSpRef );

			switch( emType )
			{
				case esriGeometryEnvelope: // Envelope -> Path
				{
					IEnvelopePtr ipEnv( ipGeom );
					double XMin = 0.0, YMin = 0.0, XMax = 0.0, YMax = 0.0;
					if( SUCCEEDED( hr = ipEnv->QueryCoords( &XMin, &YMin, &XMax, &YMax ) ) )
					{
						WKSPoint* pPoints = new WKSPoint[5];
						if( pPoints )
						{
							pPoints[0].X = XMin; pPoints[0].Y = YMin;
							pPoints[1].X = XMin; pPoints[1].Y = YMax;
							pPoints[2].X = XMax; pPoints[2].Y = YMax;
							pPoints[3].X = XMax; pPoints[3].Y = YMin;
							pPoints[4] = pPoints[0];
							ATLVERIFY( SUCCEEDED( hr = ipRetCol->SetWKSPoints( 5, pPoints ) ));
						}
						else
							_ASSERTE( pPoints );

						if( pPoints ) 
							delete [] pPoints;	// ��n��
					}
					else
						_ASSERTE( SUCCEEDED(hr) );

					break;
				}
				case esriGeometryLine: // Line -> Path
				case esriGeometryCircularArc: // CircularArc -> Path
					ATLVERIFY( SUCCEEDED( hr = ((ISegmentCollectionPtr)ipRet)->AddSegment( (_ISegmentPtr)AheCreateClone( ipGeom ) ) ) );
					break;
				default: 
					_ASSERTE(false); // ���T�|�[�g
					break;
			}
			break;
	}

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
		
}

// �W�I���g������|���S���𐶐�����
IGeometryPtr ToPolygon( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;

	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Polygon );
	IGeometryCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryRing:	ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( (IGeometryPtr)AheCreateClone( (IUnknownPtr)ipGeom ) ) ) ); break;
		case esriGeometryEnvelope: ATLVERIFY( SUCCEEDED( hr = ipRetCol->AddGeometry( ToRing( ipGeom ) ) ) ); break;
		case esriGeometryPolygon: ipRet = AheCreateClone( ipGeom ); break;
		default: _ASSERTE( false ); break; // ���T�|�[�g
	}

	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// �W�I���g�����烊���O�𐶐�����
IGeometryPtr ToRing( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l

	if( ! ipGeom )
		return ipRet;

	HRESULT hr = S_OK;
	ISpatialReferencePtr ipSpRef;
	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );
	ipGeom->get_SpatialReference( &ipSpRef );

	ipRet.CreateInstance( CLSID_Ring );
	IPointCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
	ipRet->putref_SpatialReference( ipSpRef );

	switch( emType )
	{
		case esriGeometryEnvelope:
		{
			IEnvelopePtr ipEnv( ipGeom );
			double XMin = 0.0, YMin = 0.0, XMax = 0.0, YMax = 0.0;
			if( SUCCEEDED( hr = ipEnv->QueryCoords( &XMin, &YMin, &XMax, &YMax ) ) )
			{
				WKSPoint* pPoints = new WKSPoint[5];
				if( pPoints )
				{
					pPoints[0].X = XMin; pPoints[0].Y = YMin;
					pPoints[1].X = XMin; pPoints[1].Y = YMax;
					pPoints[2].X = XMax; pPoints[2].Y = YMax;
					pPoints[3].X = XMax; pPoints[3].Y = YMin;
					pPoints[4] = pPoints[0];
					ATLVERIFY( SUCCEEDED( hr = ipRetCol->SetWKSPoints( 5, pPoints ) ) );
				}
				else
					_ASSERTE( pPoints );

				if( pPoints ) 
					delete [] pPoints;	// ��n��
			}
			break;
		}
		case esriGeometryRing: ipRet = AheCreateClone( ipGeom ); break;
		default: _ASSERTE( false ); break; // ���T�|�[�g
	}
	return ( SUCCEEDED(hr) ) ? ipRet : NULL;
}

// �W�I���g������^����ꂽ�����̃W�I���g���𐶐�����
IGeometryPtr Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim )
{
	IGeometryPtr ipRet;	// �Ԃ�l

	if( ! ipGeom )
		return ipRet;

	esriGeometryType emType = esriGeometryNull;
	ipGeom->get_GeometryType( &emType );

	switch( emGeomDim )
	{
		case esriGeometry0Dimension:
			switch( emType )
			{
				case esriGeometryPolygon:
				case esriGeometryPolyline:
				case esriGeometryRing:
				case esriGeometryPath:
				case esriGeometryEnvelope:
				case esriGeometryPoint:
					ipRet = ToMultipoint( ipGeom );
					break;
				default: _ASSERTE( false ); break; // ���T�|�[�g
			}
			break;
		case esriGeometry1Dimension:
			switch( emType )
			{
				case esriGeometryPolygon:
				case esriGeometryRing:
				case esriGeometryEnvelope:
				case esriGeometryPath:
					ipRet = ToPolyline( ipGeom );
					break;
				default: _ASSERTE( false ); break; // ���T�|�[�g
			}
			break;
		case esriGeometry2Dimension:
			switch( emType )
			{
				case esriGeometryRing:
				case esriGeometryEnvelope:
					ipRet = ToPolygon( ipGeom );
					break;
				default: _ASSERTE( false ); break; // ���T�|�[�g
			}
			break;
		default: _ASSERTE( false ); break; // ���T�|�[�g
	}
	return ipRet;
}

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
 * @file GeometryConvert.cpp
 * @brief <b>CGeometryBase�N���X�R���o�[�g�n�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "GeometryBase.h"
#include "sindymacroutil.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;
// �W�I���g������}���`�|�C���g�𐶐�����
IGeometryPtr CGeometryBase::ToMultipoint( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		ISpatialReferencePtr ipSpRef;
		esriGeometryType emType = esriGeometryNull;
		ipGeom->get_GeometryType( &emType );
		ipGeom->get_SpatialReference( &ipSpRef );

		LOGASSERTEERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Multipoint ) ), sindyErr_COMCreateInstanceFailed )
		{
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
					IPointCollectionPtr ipCol;
					LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
					{
						long lPointCount = 0;
						WKSPoint* pPoints = NULL;
						LOGASSERTEERR_IF( SUCCEEDED( ipCol->get_PointCount( &lPointCount ) ) &&
									   NULL != ( pPoints = new WKSPoint[lPointCount] ) &&
									   SUCCEEDED( ipCol->QueryWKSPoints( 0, lPointCount, pPoints ) ) &&
									   SUCCEEDED( ipRetCol->SetWKSPoints( lPointCount, pPoints ) ), sindyErr_GeometryFunctionFailed );
						if( pPoints ) delete [] pPoints;
					}
					break;
				}
				case esriGeometryMultipoint: ipRet = AheCreateClone( ipGeom ); break;
				default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
			}
		}			
	}
	return ( sindyErr_NoErr == emErr ) ? ipRet : NULL;
}

// �W�I���g������|�����C���𐶐�����
IGeometryPtr CGeometryBase::ToPolyline( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		ISpatialReferencePtr ipSpRef;
		esriGeometryType emType = esriGeometryNull;
		ipGeom->get_GeometryType( &emType );
		ipGeom->get_SpatialReference( &ipSpRef );

		LOGASSERTEERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Polyline ) ), sindyErr_COMCreateInstanceFailed )
		{
			IGeometryCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
			ipRet->putref_SpatialReference( ipSpRef );

			switch( emType )
			{
				case esriGeometryPath:
				case esriGeometryRing:
					LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->AddGeometry( (IGeometryPtr)AheCreateClone( (IUnknownPtr)ipGeom ) ) ), sindyErr_GeometryFunctionFailed );
					break;
				case esriGeometryEnvelope:
					LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->AddGeometry( ToPath( ipGeom ) ) ), sindyErr_GeometryFunctionFailed );
					break;
				case esriGeometryPolygon:
				{
					IGeometryCollectionPtr ipGeomCol;
					LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IGeometryCollection, (void**)&ipGeomCol ) ), sindyErr_COMQueryInterfaceFailed )
					{
						long lGeomCount = 0;
						LOGASSERTEERR_IF( SUCCEEDED( ipGeomCol->get_GeometryCount( &lGeomCount ) ), sindyErr_GeometryFunctionFailed )
						{
							for( long i = 0; i < lGeomCount; ++i )
							{
								IGeometryPtr ipPart;
								LOGASSERTEERR_IF( SUCCEEDED( ipGeomCol->get_Geometry( i, &ipPart ) ) &&
												  SUCCEEDED( ipRetCol->AddGeometry( ToPath( ipPart ) ) ), sindyErr_GeometryFunctionFailed );
							}
						}
					}
					break;
				}
				case esriGeometryLine:
					LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->AddGeometry( ToPath( ipGeom ) ) ), sindyErr_GeometryFunctionFailed );
					break;
				case esriGeometryPolyline: ipRet = AheCreateClone( ipGeom ); break;
				default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
			}

			// KnownSimple�ȃW�I���g�����ϊ����KnownSimple����Ȃ��Ȃ�̂ŁA������
			// KnownSimple�ɂ��Ă��iKnownSimple����Ȃ���WKB�֘A�̃��\�b�h�����s�H����炵���j
/*			VARIANT_BOOL vbIsKnownSimple = VARIANT_FALSE;
			ITopologicalOperatorPtr ipTopo( ipGeom );
			if( ipTopo != NULL )
				ipTopo->get_IsKnownSimple( &vbIsKnownSimple );
			if( vbIsKnownSimple )*/
				((ITopologicalOperator2Ptr)ipRet)->put_IsKnownSimple( /*vbIsKnownSimple*/VARIANT_TRUE );

		}
	}
	return ( sindyErr_NoErr == emErr ) ? ipRet : NULL;
}

// �W�I���g������p�X�𐶐�����
IGeometryPtr CGeometryBase::ToPath( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
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
				LOGASSERTEERR_IF( SUCCEEDED( ipCurve->GetSubcurve( 0.0, dLen, VARIANT_FALSE, &ipPath ) ), sindyErr_GeometryFunctionFailed )
				{
					ipRet = ipPath;
				}
				break;
			}
			case esriGeometryPath: // Path -> Path
				ipRet = AheCreateClone( ipGeom ); break;
				break;
			default:
				LOGASSERTEERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Path ) ), sindyErr_COMCreateInstanceFailed )
				{
					IPointCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
					ipRet->putref_SpatialReference( ipSpRef );

					switch( emType )
					{
						case esriGeometryEnvelope: // Envelope -> Path
						{
							IEnvelopePtr ipEnv;
							LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IEnvelope, (void**)&ipEnv ) ), sindyErr_COMQueryInterfaceFailed )
							{
								double XMin, YMin, XMax, YMax;
								LOGASSERTEERR_IF( SUCCEEDED( ipEnv->QueryCoords( &XMin, &YMin, &XMax, &YMax ) ), sindyErr_GeometryFunctionFailed )
								{
									WKSPoint* pPoints = new WKSPoint[5];
									LOGASSERTEERR_IF( pPoints, sindyErr_MemoryAllocationFailed )
									{
										pPoints[0].X = XMin; pPoints[0].Y = YMin;
										pPoints[1].X = XMin; pPoints[1].Y = YMax;
										pPoints[2].X = XMax; pPoints[2].Y = YMax;
										pPoints[3].X = XMax; pPoints[3].Y = YMin;
										pPoints[4] = pPoints[0];
										LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->SetWKSPoints( 5, pPoints ) ), sindyErr_GeometryFunctionFailed );
									}
									if( pPoints ) delete [] pPoints;	// ��n��
								}
							}
							break;
						}
						case esriGeometryLine: // Line -> Path
							LOGASSERTEERR_IF( SUCCEEDED( ((ISegmentCollectionPtr)ipRet)->AddSegment( (_ISegmentPtr)AheCreateClone( ipGeom ) ) ), sindyErr_GeometryFunctionFailed );
							break;
						default: 
							LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported );
							break;

					}
				}
				break;
		}
	}
	return ( sindyErr_NoErr == emErr ) ? ipRet : NULL;
		
}

// �W�I���g������|���S���𐶐�����
IGeometryPtr CGeometryBase::ToPolygon( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		ISpatialReferencePtr ipSpRef;
		esriGeometryType emType = esriGeometryNull;
		ipGeom->get_GeometryType( &emType );
		ipGeom->get_SpatialReference( &ipSpRef );

		LOGASSERTEERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Polygon ) ), sindyErr_COMCreateInstanceFailed )
		{
			IGeometryCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
			ipRet->putref_SpatialReference( ipSpRef );

			switch( emType )
			{
				case esriGeometryRing:	LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->AddGeometry( (IGeometryPtr)AheCreateClone( (IUnknownPtr)ipGeom ) ) ), sindyErr_GeometryFunctionFailed ); break;
				case esriGeometryEnvelope: LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->AddGeometry( ToRing( ipGeom ) ) ), sindyErr_GeometryFunctionFailed ); break;
				case esriGeometryPolygon: ipRet = AheCreateClone( ipGeom ); break;
				default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
			}
		}
	}
	return ( sindyErr_NoErr == emErr ) ? ipRet : NULL;
}

// �W�I���g�����烊���O�𐶐�����
IGeometryPtr CGeometryBase::ToRing( IGeometry* ipGeom )
{
	IGeometryPtr ipRet;	// �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		ISpatialReferencePtr ipSpRef;
		esriGeometryType emType = esriGeometryNull;
		ipGeom->get_GeometryType( &emType );
		ipGeom->get_SpatialReference( &ipSpRef );

		LOGASSERTEERR_IF( SUCCEEDED( ipRet.CreateInstance( CLSID_Ring ) ), sindyErr_COMCreateInstanceFailed )
		{
			IPointCollectionPtr ipRetCol( ipRet );	// ���s���邱�Ƃ͖����͂��Ȃ̂ŃG���[�`�F�b�N���Ȃ��i�ʓ|�����j
			ipRet->putref_SpatialReference( ipSpRef );

			switch( emType )
			{
				case esriGeometryEnvelope:
				{
					IEnvelopePtr ipEnv;
					LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IEnvelope, (void**)&ipEnv ) ), sindyErr_COMQueryInterfaceFailed )
					{
						double XMin, YMin, XMax, YMax;
						LOGASSERTEERR_IF( SUCCEEDED( ipEnv->QueryCoords( &XMin, &YMin, &XMax, &YMax ) ), sindyErr_GeometryFunctionFailed )
						{
							WKSPoint* pPoints = new WKSPoint[5];
							LOGASSERTEERR_IF( pPoints, sindyErr_MemoryAllocationFailed )
							{
								pPoints[0].X = XMin; pPoints[0].Y = YMin;
								pPoints[1].X = XMin; pPoints[1].Y = YMax;
								pPoints[2].X = XMax; pPoints[2].Y = YMax;
								pPoints[3].X = XMax; pPoints[3].Y = YMin;
								pPoints[4] = pPoints[0];
								LOGASSERTEERR_IF( SUCCEEDED( ipRetCol->SetWKSPoints( 5, pPoints ) ), sindyErr_GeometryFunctionFailed );
							}
							if( pPoints ) delete [] pPoints;	// ��n��
						}
					}
					break;
				}
				case esriGeometryRing: ipRet = AheCreateClone( ipGeom ); break;
				default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
			}
		}			
	}
	return ( sindyErr_NoErr == emErr ) ? ipRet : NULL;
}

// �W�I���g������^����ꂽ�����̃W�I���g���𐶐�����
IGeometryPtr CGeometryBase::Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim )
{
	IGeometryPtr ipRet;	// �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
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
					default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
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
					default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
				}
				break;
			case esriGeometry2Dimension:
				switch( emType )
				{
					case esriGeometryRing:
					case esriGeometryEnvelope:
						ipRet = ToPolygon( ipGeom );
						break;
					default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
				}
				break;
			default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
		}
	}
	return ( sindyErr_NoErr == emErr ) ? ipRet : NULL;
}

} // sindy

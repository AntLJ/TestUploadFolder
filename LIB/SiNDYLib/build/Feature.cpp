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
 * @file Feature.cpp
 * @brief <b>CFeature�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "Feature.h"
#include "sindycomutil.h"
#include "util.h"
#include "ErrorObject.h"
#include "RuleModel.h"
#include "Table.h"
#include "define.h"
#include "GlobalFunctions.h"
#include "AheGeometryOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR FAILED_TO_MOVE_FEATURE   = _T("�t�B�[�`���ړ��Ɏ��s");
	LPCTSTR FAILED_TO_SPLIT_AT_POINT = _T("SplitAtPoints�Ɏ��s");
#else
	LPCTSTR FAILED_TO_MOVE_FEATURE   = _T("Failed to move feature");
	LPCTSTR FAILED_TO_SPLIT_AT_POINT = _T("Failed to SplitAtPoints");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace ::sindy::errorcode;

CFeature::operator IFeature*() const
{
	IFeaturePtr ipFeature;
	QUERYINTERFACE( p, IID_IFeature, (void**)&ipFeature );
	return ipFeature;
}

IObjectClassPtr CFeature::GetClass() const
{
	IObjectClassPtr ipObjectClass;
	QUERYINVOKEMETHOD( IFeature, IID_IFeature,  get_Class( &ipObjectClass ) );
	return ipObjectClass;
}


IEnvelopePtr CFeature::GetExtent() const
{
	IEnvelopePtr ipEnv = NULL;
	QUERYINVOKEMETHOD( IFeature, IID_IFeature, get_Extent( &ipEnv ) );
	return ipEnv;
}

esriFeatureType CFeature::GetFeatureType() const
{
	esriFeatureType type = (esriFeatureType)-1;
	QUERYINVOKEMETHOD( IFeature, IID_IFeature, get_FeatureType( &type ) );
	return type;
}

// �^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��
sindyErrCode CFeature::Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjects& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;
	
	// �`����ړ�����
	if( sindyErr_NoErr != ( emErr = GetShape()->Move( cGeom, pairMove, emMode ) ) )
		cErrs.push_back( CErrorObject( *this, CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), FAILED_TO_MOVE_FEATURE ) ) );
#ifdef _DEBUG2
	else {
		// �{���ɕύX���ꂽ���ǂ����̊m�F
		schema::ipc_table::update_type::ECode emCode;
		if( ! ( Changed( &emCode ) && emCode == schema::ipc_table::update_type::kShapeUpdated ) )
		{
			_ASSERTE( Changed( &emCode ) && emCode == schema::ipc_table::update_type::kShapeUpdated );
			cErrInfos.push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), _T("CGeometry::Move()�ōX�V��������܂���ł���") ) );
		}
	}
#endif // ifdef _DEBUG2
	// �֘A�I�u�W�F�N�g�̈ړ�
	CContainer::Move( cGeom, pairMove, emMode, cErrs );

	return cErrs.GetDefaultErrCode();
}

sindyErrCode CFeature::Merge( const CGeometry& cGeom, CErrorObjects& /*cErrInfos*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;

	if ( GetShape() ) {
		// �}�[�W����̌`���Intersect�����`��̃|�C���g����1�ȏ�H
		CGeometry cIntersect = GetShape()->IntersectPoints( cGeom );

		if ( (IGeometry*)cIntersect && !cIntersect.IsEmpty() && cIntersect.GetPointCount() > 0 ) {
			switch ( GetShape()->GetGeometryType() ) {
				case esriGeometryPolyline:
				case esriGeometryPath:
					{
						CGeometry cFrom = GetShape()->GetPoint( 0 );
						CGeometry cTo = GetShape()->GetPoint( GetShape()->GetPointCount() - 1 );

						// �������Ⴄ���͍�����
						if ( cFrom.Equals( cGeom.GetPoint( 0 ) ) || cTo.Equals( cGeom.GetPoint( cGeom.GetPointCount() - 1 ) ) )
							((ICurvePtr)(IGeometry*)cGeom)->ReverseOrientation();

						emErr = GetShape()->Union( cGeom );	// �`���UNION
					}
					break;
				case esriGeometryMultipoint:
				case esriGeometryPolygon:
				case esriGeometryRing:
					emErr = GetShape()->Union( cGeom );	// �`���UNION
					break;
				default:
					_ASSERT(false); break;
			}
		}
	} else
		_ASSERTE(false);

	return emErr;
}

sindyErrCode CFeature::Erase( const CGeometry& cGeom, CErrorObjects& /*cErrInfos*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;

	if ( GetShape() ) {
		esriGeometryType emType = GetShape()->GetGeometryType();
		CGeometry cIntersect = GetShape()->IntersectPoints( cGeom );

		if ( (IGeometry*)cIntersect && !cIntersect.IsEmpty() ) {
			switch ( emType ) {
				case esriGeometryPoint:
					Delete();
					break;
				case esriGeometryPolyline:
					if ( GetShape()->GetPointCount() - cIntersect.GetPointCount() <= 1 )
						Delete();
					else
						emErr = GetShape()->Erase( cIntersect );
					break;
				case esriGeometryPolygon:
					{
						IGeometryCollectionPtr ipGeomColl( (IGeometry*)*GetShape() );
						bool bIsDel = false;

						// �p�[�g���ɕ����ď�������̂��ɏ���
						long i = 0;
						for ( CGeometry::part_iterator itPart = GetShape()->part_begin(); itPart != GetShape()->part_end(); ++itPart, ++i ) {
							CGeometry cPart( *itPart );

							cIntersect = cPart.IntersectPoints( cGeom );

							if ( (IGeometry*)cIntersect && ! cIntersect.IsEmpty() ) {
								long nInterCnt = cIntersect.GetPointCount();

								// cIntersect�Ɏn�I�_(����)�����邩�H
								CGeometry cFront = cPart.GetPoint( 0 );
								long nFBCnt = 0;

								for ( CGeometry::point_iterator itPt = cIntersect.point_begin(); itPt != cIntersect.point_end(); ++itPt ) {
									if ( cFront.Equals( *itPt ) )
										++nFBCnt;
								}

								if ( nFBCnt == 2 )	
									--nInterCnt;	// �n�I�_(����)�����鎞�̓f�N�������g���Ƃ�

								if ( cPart.GetPointCount() - nInterCnt <= 3 ) {
									if ( i == 0 ) {	// Exterior�̎��͍폜
										Delete();
										bIsDel = true;
										break;
									} else {
										// Interior��
										ipGeomColl->RemoveGeometries( i, 1 );
									}
								}
							}
						}

						if ( !bIsDel ) {
							// �p�[�g���ɏ���������̌`��ŏ���
							CGeometry cPart( (IGeometryPtr)ipGeomColl );

							cIntersect = cPart.IntersectPoints( cGeom );

							if ( (IGeometry*)cIntersect && cIntersect.GetPointCount() > 0 )
								// �\���_�폜
								emErr = cPart.Erase( cIntersect );

							// �`�󂪂Ȃ��Ȃ����Ƃ��͍폜
							if( ! (IGeometry*)cPart || cPart.IsEmpty() )
								Delete();
						}
					}
					break;
				default:
					_ASSERTE(false);
			}
		}
	} else
		_ASSERTE(false);

	return emErr;
}

sindyErrCode CFeature::Split( const CGeometry& cGeom, CGeometry& cSplittedGeoms, CGeometry& cSplittedPoints, CErrorObjects& /*cErrInfos*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;

	if ( GetShape() ) {
		// �R�s�[�擾
		IGeometryPtr ipCopyOrg = AheSetKnownSimple( AheCreateClone( GetShapeCopy() ) );
		IGeometryPtr ipCopyGeom = AheSetKnownSimple( AheCreateClone( (IGeometry*)cGeom ) );

		switch ( GetShape()->GetGeometryType() ) {
			case esriGeometryPolyline:
				{
					CGeometry cSplitGeom;	// �����`��

					switch ( cGeom.GetGeometryType() ) {
						case esriGeometryPoint:
						case esriGeometryMultipoint:
							cSplitGeom = ipCopyGeom;
							break;
						case esriGeometryPolyline:
						case esriGeometryPolygon:
							cSplitGeom = AheGetCrossOrTouchPoints( ipCopyOrg, ipCopyGeom );
							break;
						default:
							break;
					}

					if ( (IGeometry*)cSplitGeom && cSplitGeom.GetPointCount() > 0 ) {	// ��_����H
						bool bSplit = true;
						long nCount = 0;

						((IPointCollectionPtr)ipCopyOrg)->get_PointCount( &nCount );

						// 2�_�����Ȃ�����FROM/TO��Split���悤�Ƃ������̓X���[
						if ( nCount == 2 ) {
							CGeometry cFrom = GetShape()->GetPoint( 0 );
							CGeometry cTo = GetShape()->GetPoint( GetShape()->GetPointCount() - 1 );

							for ( CGeometry::point_iterator itPt = cSplitGeom.point_begin(); itPt != cSplitGeom.point_end(); ++itPt ) {
								if ( cFrom.Equals( *itPt ) || cTo.Equals( *itPt ) ) {
									bSplit = false;
									break;
								}
							}
						}

						if ( bSplit ) {
							IGeometryCollectionPtr ipGeomColl( CLSID_GeometryBag );
							((IGeometryPtr)ipGeomColl)->putref_SpatialReference( GetShape()->GetSpatialReference() );
							IPointCollectionPtr ipSplittedPoints;

							// Split
							CGeometry cGeoms = (IGeometryPtr)AheCreateClone( _SplitPolyline( cSplitGeom, &ipSplittedPoints ) );

							// �R�s�[�����Ȃ��ƁA�v���W�F�N�V�����̂Ƃ��ł�������B
							for ( CGeometry::const_part_iterator itPart = cGeoms.part_begin(), itPartEnd = cGeoms.part_end(); itPart != itPartEnd; ++itPart ) {
								CGeometry cNewGeom = (IGeometryPtr)AheCreateClone( (*itPart) );

								ipGeomColl->AddGeometry( cNewGeom );
							}

							long lCount = 0;

							ipGeomColl->get_GeometryCount( &lCount );
							if ( lCount > 0 ) {
								cSplittedGeoms = (IGeometryPtr)ipGeomColl;
								cSplittedPoints = (IGeometryPtr)ipSplittedPoints;
							}
						}
					}
				}
				break;
			case esriGeometryPolygon:
				{
					switch ( cGeom.GetGeometryType() ) {
						case esriGeometryPolyline:
							cSplittedGeoms = (IGeometryPtr)AheCreateClone( _SplitPolygon( (CGeometry)ipCopyOrg, (CGeometry)ipCopyGeom ) );
							break;
						default:
							break;
					}

				}
				break;
			default:
				emErr = sindyErr_GeometryFunctionFailed;
				break;
		}
	} else
		emErr = sindyErr_GeometryFunctionFailed;

	return emErr;
}

IGeometryPtr CFeature::_SplitPolyline( const CGeometry& cGeom, IPointCollection** ipSplittedPoints )
{
	IGeometryPtr ipSplittedGeoms;

	if( (IGeometry*)cGeom ) {
		esriGeometryType enumSplitterPointsType = cGeom.GetGeometryType();
		IPointCollectionPtr ipPointCol( (IGeometry*)cGeom );

		// cGeom �̃W�I���g���^�C�v���|�C���g�̏ꍇ�A IPointCollection �ɂ͕ϊ��ł��Ȃ�
		if ( ( ! ipPointCol ) && enumSplitterPointsType == esriGeometryPoint )
		{
			ipPointCol.CreateInstance( CLSID_Multipoint );

			((IGeometryPtr)ipPointCol)->putref_SpatialReference( cGeom.GetSpatialReference() );
			ipPointCol->AddPoint( (IPointPtr)(IGeometry*)cGeom );
		}

		// ipPointCol �̃|�C���g����0�̏ꍇ�̓��^�[��
		long lCount = 0;

		ipPointCol->get_PointCount( &lCount );
		if ( lCount > 0 ) {
			bool bIsSplitted = false;	// �������ꂽ���ǂ���

			// ���̌`��̒n���Q�Ǝ擾
			ISpatialReferencePtr ipSpRef = GetShape()->GetSpatialReference();

			// ���̌`��R�s�[
			IPolylinePtr ipCopyPolyline( AheCreateClone( (IUnknownPtr)GetShapeCopy() ) );

			// ���̌`��� FROM�ATO �|�C���g���L���i�X�v���b�g��̌������X�v���b�g�O�ƈ�v�����邽�߁j
			IPointPtr ipFromPoint, ipToPoint;

			ipCopyPolyline->get_FromPoint( &ipFromPoint );
			ipCopyPolyline->get_ToPoint( &ipToPoint );

			//
			IEnumVertexPtr ipEnumVertex;

			ipPointCol->get_EnumVertices( &ipEnumVertex );

			if ( ipEnumVertex ) {
				ipEnumVertex->Reset();

				IPolycurve2Ptr ipPolycurve2 = ipCopyPolyline;

				if ( ipPolycurve2 ) {
					IEnumSplitPointPtr ipEnumSplit;

					// �X�v���b�g
					SUCCEEDED_IF( ipPolycurve2->SplitAtPoints( ipEnumVertex, VARIANT_FALSE, VARIANT_TRUE, AheGetXYDomain1UnitLength( ipSpRef ), &ipEnumSplit ), sindyErr_GeometryFunctionFailed, _T("%s"), FAILED_TO_SPLIT_AT_POINT )
					{
						if ( ipEnumSplit )
						{
							ipEnumSplit->Reset();
							VARIANT_BOOL vbIsSplit = VARIANT_FALSE;
							ipEnumSplit->get_SplitHappened( &vbIsSplit );

							if ( vbIsSplit )
							{
								// �X�v���b�g�_
								IPointCollectionPtr ipPointColl( CLSID_Multipoint );
								((IGeometryPtr)ipPointColl)->putref_SpatialReference( ipSpRef );

								int lCnt = 0;

								IPointPtr ipPoint;
								long outPartIndex = 0, vertexIndex = 0;
								while ( SUCCEEDED( ipEnumSplit->Next( &ipPoint, &outPartIndex, &vertexIndex ) ) )
								{
									if ( !ipPoint )
										break;

									// �����_���L��
									ipPointColl->AddPoint( (IPointPtr)AheCreateClone(ipPoint) );

									++lCnt;
								}

								if ( lCnt > 0 ) {
									if ( ipSplittedPoints ) {
										(*ipSplittedPoints) = ipPointColl;
										if ( (*ipSplittedPoints) )
											(*ipSplittedPoints)->AddRef();
									}
									bIsSplitted = true;
								}
							}
						}
					}
				}
			}

			// �X�v���b�g�ł��Ȃ������炻�̂܂܃��^�[��
			if ( bIsSplitted ) {
				// IPath -> IPolyline �ϊ�
				IGeometryCollectionPtr ipGeomCol( AheSetKnownSimple( ipCopyPolyline ) );

				if ( ipGeomCol ) {
					long lPartCount = -1;

					ipGeomCol->get_GeometryCount( &lPartCount );

					IGeometryCollectionPtr ipSplittedGeomColl( CLSID_GeometryBag );

					((IGeometryPtr)ipSplittedGeomColl)->putref_SpatialReference( ipSpRef );

					for( int i = 0; i < lPartCount; ++i ) {
						IGeometryCollectionPtr ipNewGeomCol( CLSID_Polyline );
						IPolylinePtr ipPoly( ipNewGeomCol );

						ipPoly->putref_SpatialReference( ipSpRef ); // ��ԎQ�ƕt�^

						IGeometryPtr ipPartGeom;

						ipGeomCol->get_Geometry( i, &ipPartGeom );

						IPathPtr ipPath( ipPartGeom );

						ipNewGeomCol->AddGeometry( ipPath );

						ipSplittedGeomColl->AddGeometry( (IGeometryPtr)AheCreateClone( AheSetKnownSimple( ipPoly ) ) );
					}

					long lCount = 0;

					ipSplittedGeomColl->get_GeometryCount( &lCount );
					if ( lCount > 0 )
						ipSplittedGeoms = ipSplittedGeomColl;
				}
			}
		}
	}

	return ipSplittedGeoms;
}

IGeometryPtr CFeature::_SplitPolygon( const CGeometry& cGeom, const CGeometry& cSplitterGeom )
{
	IGeometryPtr ipSplittedGeoms;

	IPolygonPtr ipPolygonCopy( AheCreateClone( (IGeometry*)cGeom ) );
	ITopologicalOperatorPtr ipTopo( ipPolygonCopy );

	if ( ipTopo && (IGeometry*)cSplitterGeom ) {
		// �n���Q�Ƃ��킹
		ISpatialReferencePtr ipSpRef = cGeom.GetSpatialReference();
		IPolylinePtr ipSplitterPolyline( AheCreateClone( (IGeometry*)cSplitterGeom ) );

		ipSplitterPolyline->putref_SpatialReference( ipSpRef );
		ipSplitterPolyline->SnapToSpatialReference();

		IGeometryPtr ipLeftGeom, ipRightGeom;

		if ( ! AheIsOverlap2( AheConvertPolygon2Polyline(ipPolygonCopy), ipSplitterPolyline, ipSpRef ) ) {
			try
			{
				if ( SUCCEEDED( ipTopo->Cut( ipSplitterPolyline, &ipLeftGeom, &ipRightGeom ) ) ) {
					if ( ipLeftGeom && ipRightGeom ) {
						// �X�v���b�g�����������悤�Ɍ����Ă����ۂ͍\���_��0�̏ꍇ������̂ŁA�`�F�b�N�i�ȁ[��[�Ł[��[�j
						IPointCollectionPtr ipPointCol( ipLeftGeom );
						long lPointCount = -1;

						ipPointCol->get_PointCount( &lPointCount );

						if ( lPointCount > 0 ) {
							ipPointCol = ipRightGeom;
							lPointCount = -1;
							ipPointCol->get_PointCount( &lPointCount );

							if ( lPointCount > 0 ) {
								// [bug 2189] �X�v���b�g��Ƀh�[�i�c�ɂȂ�ꍇ�́AInterior���E���ɂȂ��Ă���ꍇ�L��I�I
								IPolygonPtr ipPolygonTmp( ipLeftGeom );

								if ( ipPolygonTmp )
									ipPolygonTmp->SimplifyPreserveFromTo();

								ipPolygonTmp = ipRightGeom;
								if ( ipPolygonTmp )
									ipPolygonTmp->SimplifyPreserveFromTo();

								IGeometryCollectionPtr ipSplittedGeomColl( CLSID_GeometryBag );

								((IGeometryPtr)ipSplittedGeomColl)->putref_SpatialReference( ipSpRef );

								// �}���`�p�[�g�ɂȂ��Ă��邩������Ȃ��̂ŁA�p�[�g����
								IUnknownPtr ipUnk;

								_ISetPtr ipLeftSet = AheConvertMultiPolygon2Polygons( ipLeftGeom );
								if ( ipLeftSet ) {
									ipLeftSet->Reset();

									while ( SUCCEEDED( ipLeftSet->Next( &ipUnk ) ) ) {
										if ( ! ipUnk )
											break;

										IPolygonPtr ipPolygonTmp( ipUnk );
										if ( ipPolygonTmp )
											ipSplittedGeomColl->AddGeometry( (IGeometryPtr)AheCreateClone(ipPolygonTmp) );
									}
								}

								_ISetPtr ipRightSet = AheConvertMultiPolygon2Polygons( ipRightGeom );
								if ( ipRightSet ) {
									ipRightSet->Reset();

									while ( SUCCEEDED( ipRightSet->Next( &ipUnk ) ) ) {
										if ( ! ipUnk )
											break;

										IPolygonPtr ipPolygonTmp( ipUnk );
										if ( ipPolygonTmp )
											ipSplittedGeomColl->AddGeometry( (IGeometryPtr)AheCreateClone(ipPolygonTmp) );
									}
								}

								long lCount = 0;

								ipSplittedGeomColl->get_GeometryCount( &lCount );
								if ( lCount > 0 )
									ipSplittedGeoms = ipSplittedGeomColl;
							}
						}
					}
				}
			}
			catch(...)
			{
			}
		}
	}

	return ipSplittedGeoms;
}

// ���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�
IEnvelopePtr CFeature::GetEnvelope( bool bOriginal/* = true*/, bool bChanged/* = true*/, bool bFeatureChangedOriginal/* = true*/ ) const
{
	IEnvelopePtr ipRet;

	// CRow���ł̕ύX�`���`
	if( bChanged && m_pGeom )
		ipRet = m_pGeom->GetEnvelope();

	// �I���W�i��
	if( bOriginal )
	{
		IFeaturePtr ipFeat( p );
		if( ipFeat != NULL )
		{
			IEnvelopePtr ipEnv;
			IGeometryPtr ipGeom;
			ipFeat->get_Shape( &ipGeom );
			if( ipGeom != NULL )
			{
				ipGeom->get_Envelope( &ipEnv );
				if( ipEnv != NULL )
				{
					if( ipRet != NULL )
					{
						LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed );
					}
					else
						ipRet = ipEnv;
				}
			}
		}
	}

	// IFeatureChanged::OriginalShape
	if( bFeatureChangedOriginal )
	{
		IFeatureChangesPtr ipChanged( p );
		if( ipChanged != NULL )
		{
			VARIANT_BOOL vbChanged = VARIANT_FALSE;
			ipChanged->get_ShapeChanged( &vbChanged );
			if( vbChanged )
			{
				IEnvelopePtr ipEnv;
				IGeometryPtr ipGeom;
				ipChanged->get_OriginalShape( &ipGeom );
				if( ipGeom != NULL )
				{
					ipGeom->get_Envelope( &ipEnv );
					if( ipEnv != NULL )
					{
						if( ipRet != NULL )
						{
							LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed );
						}
						else
							ipRet = ipEnv;
					}
				}
			}
		}
	}
	// �֘A�I�u�W�F�N�g
	IEnvelopePtr ipExtendEnv( CContainer::GetEnvelope( bOriginal, bChanged, bFeatureChangedOriginal ) );

	// UNION
	if( ipRet )
	{
		if( ipExtendEnv )
			ipRet->Union( ipExtendEnv );
	}
	else
		ipRet = ipExtendEnv;

	return ipRet;
}

// ��ԎQ�Ƃ��擾����
ISpatialReferencePtr CFeature::GetSpatialReference() const
{
	ISpatialReferencePtr ipRet; // �Ԃ�l

	IGeoDatasetPtr ipDataset( GetClass() );
	LOGASSERTE_IF( ipDataset, sindyErr_ClassMemberMissing )
	{
		SUCCEEDED_IF( ipDataset->get_SpatialReference( &ipRet ), sindyErr_COMFunctionFailed, NULL, NULL )
		{
		}
	}
	return ipRet;
}

} // sindy

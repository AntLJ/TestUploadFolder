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
 * @file Geometry.cpp
 * @brief <b>CGeometry�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "sindycomutil.h"
#include "GlobalFunctions.h"
#include "util.h"
#include "EnumSplitPoint.h"
#include <boost/shared_ptr.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR SIMPLIFY_PERFORMED_SINCE_ARG1_NOT_SIMPLE = _T("%s : Simplify is performed since the argument 1 is not Simple. \n");
	LPCTSTR SIMPLIFY_PERFORMED_SINCE_ARG2_NOT_SIMPLE = _T("%s : Simplify is performed since the argument 2 is not Simple. \n");
	LPCTSTR INFO_ADDED_SHAPE_POINT                   = _T("%s : The added shape point:PART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
	LPCTSTR ADD_SHAPE_POINT_PART_INDEX               = _T("%s : Shape point addition PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
	LPCTSTR ADD_SHAPE_POINT_PART_INDEX_COUNT         = _T("%s : Shape point addition PART[%d] INDEX[%d] COUNT[%d]\n");
#else
	LPCTSTR SIMPLIFY_PERFORMED_SINCE_ARG1_NOT_SIMPLE = _T("%s : �����P��Simple�ł͂Ȃ����߁ASimplify�����s���܂�\n");
	LPCTSTR SIMPLIFY_PERFORMED_SINCE_ARG2_NOT_SIMPLE = _T("%s : �����Q��Simple�ł͖������߁ASimplify�����s���܂�\n");
	LPCTSTR INFO_ADDED_SHAPE_POINT                   = _T("%s : �ǉ����ꂽ�\���_�FPART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
	LPCTSTR ADD_SHAPE_POINT_PART_INDEX               = _T("%s : �\���_�ǉ� PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
	LPCTSTR ADD_SHAPE_POINT_PART_INDEX_COUNT         = _T("%s : �\���_�ǉ� PART[%d] INDEX[%d] COUNT[%d]\n");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

/// Move���\�b�h���ł̈ړ��⏕�p�N���X
class _AddPoint
{
	friend class _AddPoint;
public:
	bool operator < ( const _AddPoint& c1 ) const { return Before > c1.Before; }
	_AddPoint(){ init(); }
	_AddPoint( IPoint* p, long index ){ init(); Point = p; Index = index; }
	virtual ~_AddPoint(){}
	void init() { Point = NULL; Index = -1; Before = true; }
	IPointPtr Point;
	long Index;
	bool Before;
};

// CGeometry���R�s�[����
void CopyCGeometry( CGeometry& dest, const CGeometry& src )
{
	dest.Init();
	dest = (IGeometry*)src;
	dest.m_ipCache = ( src.m_ipCache ) ? (IGeometryPtr)AheCreateClone( src.m_ipCache ) : NULL;
}

esriGeometryDimension CGeometry::GetDimension() const
{
	esriGeometryDimension emRet = (esriGeometryDimension)-1;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Dimension( &emRet ) ), sindyErr_COMFunctionFailed );
	}
	return emRet;
}

IEnvelopePtr CGeometry::GetEnvelope() const
{
	IEnvelopePtr ipRet;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Envelope( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

esriGeometryType CGeometry::GetGeometryType() const
{
	esriGeometryType emRet = (esriGeometryType)-1;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_GeometryType( &emRet ) ), sindyErr_COMFunctionFailed );
	}
	return emRet;
}

bool CGeometry::IsEmpty() const
{
	VARIANT_BOOL vbIsEmpty = VARIANT_FALSE;
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_IsEmpty( &vbIsEmpty ) ), sindyErr_COMFunctionFailed );
	}
	return VB2bool(vbIsEmpty);
}

sindyErrCode CGeometry::Project( ISpatialReference* newReferenceSystem )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->Project( newReferenceSystem ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometry::QueryEnvelope( IEnvelope* outEnvelope ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->QueryEnvelope( outEnvelope ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometry::SetEmpty()
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SetEmpty() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometry::SnapToSpatialReference()
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SnapToSpatialReference() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

ISpatialReferencePtr CGeometry::GetSpatialReference() const
{
	ISpatialReferencePtr ipRet;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_SpatialReference( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

sindyErrCode CGeometry::PutRefSpatialReference( ISpatialReference* spatialRef )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->putref_SpatialReference( spatialRef ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

double CGeometry::GetLength() const
{
	double dRet = 0.0; // �Ԃ�l

	ICurvePtr ipCurve( (IGeometry*)*this );
	if( ipCurve )
	{
		SUCCEEDED_IF( ipCurve->get_Length( &dRet ), sindyErr_GeometryFunctionFailed, _T("%s"), NULL )
		{
		}
	}

	return dRet;
}

// �^����ꂽ�W�I���g���Ƃ̋������v�Z����
double CGeometry::ReturnDistance( IGeometry* ipGeom ) const
{
	double dRet = -1;	// �Ԃ�l

	IProximityOperatorPtr ipProx;
	LOGASSERTE_IF( ipGeom, sindyErr_ArgIsNull )
	{
		LOGASSERTE_IF( NULL != (IGeometry*)(*this), sindyErr_ClassMemberMissing )
		{
			LOGASSERT_IF( SUCCEEDED( ((IGeometry*)(*this))->QueryInterface( IID_IProximityOperator, (void**)&ipProx ) ), sindyErr_COMQueryInterfaceFailed )
			{
				LOGASSERT_IF( SUCCEEDED( ipProx->ReturnDistance( ipGeom, &dRet ) ), sindyErr_GeometryFunctionFailed );
			}
		}
	}

	return dRet;
}

// �������g��UNION����
sindyErrCode CGeometry::Union( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	if ( IsCached() )
		m_ipCache = Union( (IGeometry*)*this, ipGeom );
	else
		*this = Union( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometry::Union( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
{
	IGeometryPtr ipRet; // �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( ipGeom1 && ( ! ipGeom2 ) )
		ipRet = AheCreateClone( ipGeom1 );
	else if( ipGeom2 && ( ! ipGeom1 ) )
		ipRet = AheCreateClone( ipGeom2 );
	else {
		try {
			ITopologicalOperatorPtr ipTopo( ipGeom1 ), ipTopo2( ipGeom2 );
			LOGASSERTEERR_IF( NULL != ipTopo, sindyErr_COMInterfaceIsNull )
			{
#ifdef _DEBUG
				CString strHead;
				strHead.Format(_T("%s : geom1"), __func__);
				CGeometry::Trace( ipGeom1, strHead );
				strHead.Format(_T("%s : geom2"), __func__);
				CGeometry::Trace( ipGeom2, strHead );
#endif // ifdef _DEBUG
				// Simple ���ǂ������`�F�b�N
				VARIANT_BOOL vbIsSimple1 = VARIANT_FALSE, vbIsSimple2 = VARIANT_FALSE;
				LOGASSERTEERR_IF( SUCCEEDED( ipTopo->get_IsSimple( &vbIsSimple1 ) ) &&
					NULL != ipTopo2 &&
					SUCCEEDED( ipTopo2->get_IsSimple( &vbIsSimple2 ) ), sindyErr_GeometryFunctionFailed )
				{
					if( vbIsSimple1 && vbIsSimple2 )
					{
					}
					else {
						// Simple �ł͂Ȃ��ꍇ Union �����s����̂� Simplify �����s����
						if( ! vbIsSimple1 )
						{
							TRACEMESSAGE(SIMPLIFY_PERFORMED_SINCE_ARG1_NOT_SIMPLE, __func__ );
							LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed );
						}
						if( ! vbIsSimple2 )
						{
							TRACEMESSAGE(SIMPLIFY_PERFORMED_SINCE_ARG2_NOT_SIMPLE, __func__ );
							LOGASSERTEERR_IF( SUCCEEDED( ipTopo2->Simplify() ), sindyErr_GeometryFunctionFailed );
						}
					}

					if( sindyErr_NoErr == emErr )
					{
						IGeometryPtr ipOut;
						LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Union( ipGeom2, &ipOut ) ), sindyErr_GeometryFunctionFailed )
						{
							ipRet = ipOut;
#ifdef _DEBUG
							CString strHead;
							strHead.Format(_T("%s : Unioned"), __func__);
							CGeometry::Trace( ipRet, strHead );
#endif // ifdef _DEBUG
						}
					}
				}
			}
		}
		catch(...)
		{
			emErr = sindyErr_ThrowException;
			ERRORLOG(emErr);
			SASSERTE(false);
		}
	}

	if( pErr )
		*pErr = emErr;

	return ipRet;
}

// �������g��SymmetricDifference����
sindyErrCode CGeometry::SymmetricDifference( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	if ( IsCached() )
		m_ipCache = SymmetricDifference( (IGeometry*)*this, ipGeom );
	else
		*this = SymmetricDifference( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometry::SymmetricDifference( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
{
	IGeometryPtr ipRet; // �Ԃ�l
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( ipGeom1 && ( ! ipGeom2 ) )
		ipRet = AheCreateClone( ipGeom1 );
	else if( ipGeom2 && ( ! ipGeom1 ) )
		ipRet = AheCreateClone( ipGeom2 );
	else {
		try {
			ITopologicalOperator2Ptr ipTopo( ipGeom1 ), ipTopo2( ipGeom2 );
			LOGASSERTEERR_IF( NULL != ipTopo, sindyErr_COMInterfaceIsNull )
			{
#ifdef _DEBUG
				CString strHead;
				strHead.Format(_T("%s : geom1"), __func__);
				CGeometry::Trace( ipGeom1, strHead );
				strHead.Format(_T("%s : geom2"), __func__);
				CGeometry::Trace( ipGeom2, strHead );
#endif // ifdef _DEBUG
				// Simple ���ǂ������`�F�b�N
				VARIANT_BOOL vbIsSimple1 = VARIANT_FALSE, vbIsSimple2 = VARIANT_FALSE;
				LOGASSERTEERR_IF( SUCCEEDED( ipTopo->get_IsSimple( &vbIsSimple1 ) ) &&
					NULL != ipTopo2 &&
					SUCCEEDED( ipTopo2->get_IsSimple( &vbIsSimple2 ) ), sindyErr_GeometryFunctionFailed )
				{
					if( vbIsSimple1 && vbIsSimple2 )
					{
					}
					else {
						// Simple �ł͂Ȃ��ꍇ Union �����s����̂� Simplify �����s����
						if( ! vbIsSimple1 )
						{
							TRACEMESSAGE(SIMPLIFY_PERFORMED_SINCE_ARG1_NOT_SIMPLE, __func__ );
							LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed );
						}
						if( ! vbIsSimple2 )
						{
							TRACEMESSAGE(SIMPLIFY_PERFORMED_SINCE_ARG2_NOT_SIMPLE, __func__ );
							LOGASSERTEERR_IF( SUCCEEDED( ipTopo2->Simplify() ), sindyErr_GeometryFunctionFailed );
						}
					}

					if( sindyErr_NoErr == emErr )
					{
						IGeometryPtr ipOut;
						LOGASSERTEERR_IF( SUCCEEDED( ipTopo->SymmetricDifference( ipGeom2, &ipOut ) ), sindyErr_GeometryFunctionFailed )
						{
							ipRet = ipOut;
#ifdef _DEBUG
							CString strHead;
							strHead.Format(_T("%s : SymmetricDifferenced"), __func__);
							CGeometry::Trace( ipRet, strHead );
#endif // ifdef _DEBUG
						}
					}
				}
			}
		}
		catch(...)
		{
			emErr = sindyErr_ThrowException;
			ERRORLOG(emErr);
			SASSERTE(false);
		}
	}

	if( pErr )
		*pErr = emErr;

	return ipRet;
}

// �W�I���g���̈ړ��������s��
sindyErrCode CGeometry::Move( IGeometry* ipQueryGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	bool bMoveAll = true;	// �N�G���`��NULL�Ȃ�S�����s�ړ��Ȃ̂ōŏ���true

	Trace( _T("CGeometry::Move() : �ړ��O") );

	CGeometry cQueryGeom( ipQueryGeom );
	if( cQueryGeom != NULL )
	{
		bMoveAll = false;

		// �ړ����[�h��WholeIntersect�̏ꍇ��Disjoint�Ŕ�r����
		if( sindyeMove_WholeIntersect == emMode && ( ! Disjoint( cQueryGeom ) ) )
			bMoveAll = true;
		else {
			switch( cQueryGeom.GetGeometryType() )
			{
				case esriGeometryPoint:
					emErr = MoveVertex( (IPointPtr)(IGeometry*)cQueryGeom, pairMove );
					break;
				case esriGeometryPolygon:
				case esriGeometryEnvelope:
				case esriGeometryRing:
					if( Within( cQueryGeom ) )
						bMoveAll = true;
					else {
						switch( emMode )
						{
							case sindyeMove_Point:
								// ���̏ꍇ�A�����ɂ���܂łɃN�G���`�󂪃|�C���g�ɂȂ��Ă���K�v������
								LOGASSERTEERR_IF( sindyeMove_Point != emMode, sindyErr_AlgorithmFailed );
								break;
							case sindyeMove_SegmentIntersect:
							case sindyeMove_SegmentWithin:
								emErr = MoveVertex( ipQueryGeom, pairMove );
								break;
							case sindyeMove_SegmentAddPointBorderIntersect:
							case sindyeMove_SegmentAddPointBorderWithin:
							case sindyeMove_SegmentAddPointsBorder:
								// �����ړ�����̂̓|�C���g�ȊO
								if( GetGeometryType() != esriGeometryPoint )
								{
									// �����ړ�����
									// ����������_�Ŏ擾
									IGeometryPtr ipIntersectPoint = Intersect( cQueryGeom, esriGeometry0Dimension );
									IPointCollectionPtr ipCol;
									IEnumVertexPtr ipVertex;
									LOGASSERTEERR_IF( NULL != ipIntersectPoint &&
													  SUCCEEDED( ipIntersectPoint->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ) &&
													  SUCCEEDED( ipCol->get_EnumVertices( &ipVertex ) ), sindyErr_GeometryFunctionFailed )
									{
										std::map<long, std::list<_AddPoint> > mapVertIndex;
										CEnumSplitPoint cSplitPoint;
										double dLenTmp = GetLength(); // �ǂ̂��炢����Ă��ǂ�����臒l�i��{�I�ɂ� OnGeometry �Ȃ̂ŁA�K���j
										LOGASSERTEERR_IF( sindyErr_NoErr == ( emErr = Split( ipVertex, dLenTmp, (IEnumSplitPoint**)cSplitPoint.Release() ) ), emErr )
										{
											cSplitPoint.Reset();
											for( CEnumSplitPoint::const_iterator it = cSplitPoint.begin(); it != cSplitPoint.end(); ++it )
											{
												double x, y;
												(*it)->QueryCoords( &x, &y );
												TRACEMESSAGE(INFO_ADDED_SHAPE_POINT, __func__, it.part(), it.index(), x, y );
												mapVertIndex[it.part()].push_back( _AddPoint( *it, it.index() ) );
											}
										}
										// �ǉ��ł����p�[�g�ƃC���f�b�N�X�ԍ�����ړ����ׂ��\���_������o��
										// �Ⴂ�C���f�b�N�X�ԍ����Ƀ\�[�g�����ꍇ�An�Ԗڂ�n+1�Ԗڂ̃C���f�b�N�X�ԍ�
										// �̊Ԃ̍\���_�́A�S�ăN�G���`��̊O�A���͓��ƂȂ�̂ŊԂ̍\���_�̈�ɑ�
										// ���ē��O��������邾���ŁA������x�܂Ƃ߂Ĕ��肷�邱�Ƃ��ł���
										// �܂��A���̕��@�ł͑S�Ă̍\���_�𔻒肷��K�v�������̂ō������ɂȂ���c�͂�
										for( std::map<long,std::list<_AddPoint> >::iterator it = mapVertIndex.begin(); it != mapVertIndex.end(); ++it ) it->second.sort();	// �܂��͊e�p�[�g���Ń\�[�g���Ă���
										for( std::map<long,std::list<_AddPoint> >::iterator it = mapVertIndex.begin(); it != mapVertIndex.end(); ++it )
										{
											bool bIsFirst = true, bIsLast = false;
											std::list<_AddPoint>::iterator itIndexBefore = it->second.end();
											std::list<_AddPoint>::iterator itIndex       = it->second.begin();
											while( 1 )
											{
												long lStart = -1, lEnd = -1;
												if( bIsFirst )
												{
													lStart = 0;
													lEnd = itIndex->Index;
												}
												else if( bIsLast )
												{
													lStart = itIndexBefore->Index + 1;
													lEnd = GetPointCount();
												}
												else {
													lStart = itIndexBefore->Index + 1;
													lEnd = itIndex->Index;
												}
												bool bIsInside = false;

												// �ǉ��\���_�Ԃɍ\���_������Ȃ��ꍇ�� lStart �� lEnd �������ɂȂ�
												// ���̏ꍇ�Afor ���[�v���ŏ����ł��Ȃ��̂Ő�ɏ���
												if( lStart == lEnd && it->second.end() != itIndexBefore ) itIndexBefore->Before = false;

												for( long i = lStart; i < lEnd; ++i )
												{
													if( i == lStart )
													{
														if( cQueryGeom.Contains( GetPoint(i) ) )
															bIsInside = true;
														
														if( it->second.end() != itIndexBefore )
														{
															if( bIsInside )
																itIndexBefore->Before = false;	// ��ł���ɍ\���_��ǉ����ړ����邽��
														}
													}
													if( bIsInside )
													{
														LOGASSERTE_IF( sindyErr_NoErr == ( emErr = MoveVertex( GetPoint(i), pairMove ) ), sindyErr_GeometryFunctionFailed );	// �C���f�b�N�X�Ŏw��ł��������ǂ��񂶂�Ȃ��H
													}
												}
												if( bIsLast ) break;
												itIndexBefore = itIndex++;	// �C���N�������g����O�� itIndex �� itIndexBefore �ɃR�s�[���Ă���AitIndex ���C���N�������g
												if( itIndex == it->second.end() ) bIsLast = true;
												if( bIsFirst ) bIsFirst = false;
											}
										}
										// �{���Ȃ�\���_�ǉ��|�C���g�ɂ�2�\���_��ǉ������ׂ������A�����܂ł̏����ł͈�����ǉ�����Ă��Ȃ�
										// ���R��2����A���IPolycurve::SplitAtPoint�œ���\���_���쐬�ł��Ȃ����ƁA�������2�̂����̈��
										// �͈ړ��ΏۂƂ��Ȃ���΂Ȃ�Ȃ����A��x�ɒǉ����Ă��܂��Ƃǂ���𓮂�������悢���킩��Ȃ��Ȃ邽�߂ł���B
										// �����ŁA��ɒǉ������_���L�����Ă����A��L�̍����ړ��̍ۂ̔���Ő�ɒǉ������_�̑O��ǂ���ɒǉ�����΂悢����
										// ���ׂĂ����A�����Ă����Œǉ����Ĉړ�����Ƃ����������s���Ă���i�����ƃX�}�[�g�ȕ��@����H�j
										for( std::map<long,std::list<_AddPoint> >::iterator it = mapVertIndex.begin(); it != mapVertIndex.end(); ++it )
										{
											for( std::list<_AddPoint>::iterator itIndex = it->second.begin(); itIndex != it->second.end(); ++itIndex )
											{
												CPoint cMovedPoint( (IPointPtr)AheCreateClone( itIndex->Point ) );	// �R�s�[�����_�������Ă���킯�ł͂Ȃ��̂ŃR�s�[�����
												cMovedPoint.Move( pairMove );
												LOGASSERTE_IF( sindyErr_NoErr == ( emErr = AddVertex( cMovedPoint, it->first, itIndex->Index, itIndex->Before ) ), sindyErr_GeometryFunctionFailed )
												{
													// �\���_��ǉ��������Ƃɂ��A�ێ����Ă����C���f�b�N�X�������
													// ���̂��߁A�Ȍ�̃C���f�b�N�X�ԍ����C������K�v������
													std::list<_AddPoint>::iterator itIndex2 = itIndex;
													for( ++itIndex2; itIndex2 != it->second.end(); ++itIndex2 )
													{
														if( itIndex2->Index >= itIndex->Index )
															itIndex2->Index++;
													}
			//										LOGASSERTE_IF( sindyErr_NoErr == ( emErr = MoveVertex( itIndex->Point, pairMove ) ), sindyErr_GeometryFunctionFailed ); <- �ǉ�����_���R�s�[���ē������Ă���ǉ�����悤�ɕύX
												}
											}
										}
									}
								}
								break;
						}
					}
					break;
				default:
					ERRORLOG( emErr = sindyErr_GeometryNotSupported ); break;
			}
		}
	}
	if( bMoveAll )
	{
		// �S�����s�ړ�
		ITransform2DPtr ipTrans;
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->QueryInterface( IID_ITransform2D, (void**)&ipTrans ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERTEERR_IF( SUCCEEDED( ipTrans->Move( pairMove.first, pairMove.second ) ), sindyErr_GeometryFunctionFailed );
		}
	}

	return emErr;
}

struct HITINFO {
	long part;
	long index;
	esriGeometryHitPartType type;
};
	
// �\���_�̈ړ��������s��
sindyErrCode CGeometry::MoveVertex( IGeometry* ipQueryGeometry, const std::pair<double,double>& pairMove )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		CGeometry cQueryGeometry = ipQueryGeometry;
		LOGASSERTEERR_IF( cQueryGeometry != NULL, sindyErr_ArgIsNull )
		{
			for( point_iterator it = point_begin(); it != point_end(); ++it )
			{
				bool bMove = false; // �ړ����邩�ǂ���
				switch( cQueryGeometry.GetGeometryType() )
				{
					case esriGeometryPoint: bMove = cQueryGeometry.Equals( *it );   break; // �N�G���`�󂪃|�C���g�̏ꍇ��Equal�Ŕ�r�ibug 5548�j
					default:                bMove = cQueryGeometry.Contains( *it ); break; // ����ȊO��Contains�Ŕ�r
				}
				if( bMove )
				{
					LOGASSERTE_IF( sindyErr_NoErr == it.move( pairMove ), sindyErr_GeometryFunctionFailed )
					{
					//	TRACEMESSAGE(_T("%s : �\���_�ړ��FPART[%d] INDEX[%d] X=%.20lf(����%.20lf) Y=%.20lf(����%.20lf)\n"), __func__, lPart, lIndex, cPoint.GetX(), pairMove.first, cPoint.GetY(), pairMove.second );
					}
				}
			}
		}
	}
	return emErr;
}

// �\���_��ǉ�����
bool CGeometry::AddVertex( IPoint* ipPoint, long* pAddPart/* = NULL*/, long* pAddIndex/* = NULL*/ )
{
	bool bRet = false;	// �Ԃ�l

	// ������
	if( pAddPart ) *pAddPart = -1;
	if( pAddIndex ) *pAddIndex = -1;

	LOGASSERTE_IF( ipPoint, sindyErr_ArgIsNull )
	{
		switch( GetGeometryType() )
		{
			case esriGeometryPoint: LOGASSERTE_IF( GetGeometryType() != esriGeometryPoint, sindyErr_AlgorithmFailed ); break;
			case esriGeometryPolygon:
			case esriGeometryPolyline:
			case esriGeometryRing:
			case esriGeometryPath:
			{
				IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
				LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_GeometryNotFound )
				{
					IPolycurvePtr ipCurve;
					LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPolycurve, (void**)&ipCurve ) ), sindyErr_COMQueryInterfaceFailed )
					{
						VARIANT_BOOL vb = VARIANT_FALSE;
						long lPart = -1, lIndex = -1;
						LOGASSERTE_IF( SUCCEEDED( ipCurve->SplitAtPoint( ipPoint, -1/*VARIANT_TRUE*/, VARIANT_FALSE, &vb, &lPart, &lIndex ) ), sindyErr_GeometryFunctionFailed )
						{
							bRet = VB2bool(vb);
							if( pAddPart ) *pAddPart = lPart;
							if( pAddIndex ) *pAddIndex = lIndex;
							if( bRet )
							{
								IPointPtr ipPointTmp;
								double dX, dY;
								((IPointCollectionPtr)ipCurve)->get_Point( lIndex, &ipPointTmp );
								ipPointTmp->QueryCoords( &dX, &dY );
								TRACEMESSAGE(ADD_SHAPE_POINT_PART_INDEX, __func__, lPart, lIndex, dX, dY );
							}
						}
					}
				}
				break;
			}
			default: LOGASSERTE_IF( false, sindyErr_GeometryNotSupported ); break;
		}
	}

	return bRet;
}

// �\���_��ǉ�����
errorcode::sindyErrCode CGeometry::AddVertex( IPoint* ipPoint, long lPart, long lIndex, bool bAddBefore/* = false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipPoint, sindyErr_ArgIsNull )
	{
		LOGASSERTEERR_IF( ! ( lPart < 0 || lIndex < 0 ), sindyErr_ArgLimitOver )
		{
			IGeometryPtr ipGeom = (IGeometry*)*this;	// Polygon,Polyline�̏ꍇ�͌�Ńp�[�g�P�ʂɓ���ւ�����̂ŃX�}�[�g�|�C���^��
			switch( GetGeometryType() )
			{
				case esriGeometryPoint: LOGASSERTEERR_IF( GetGeometryType() != esriGeometryPoint, sindyErr_AlgorithmFailed ); break;
				case esriGeometryPolygon:
				case esriGeometryPolyline:
				case esriGeometryRing:
				case esriGeometryPath:
					LOGASSERTEERR_IF( NULL != ipGeom, sindyErr_GeometryNotFound )
					{
						IPointCollectionPtr ipCol;
						LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
						{
							LOGASSERTEERR_IF( SUCCEEDED( ipCol->InsertPoints( ( ( bAddBefore ) ? lIndex : lIndex + 1 ), 1, &ipPoint ) ), sindyErr_GeometryFunctionFailed )
							{
								long lPoint;
								ipCol->get_PointCount( &lPoint );
								TRACEMESSAGE(ADD_SHAPE_POINT_PART_INDEX_COUNT, __func__, lPart, ( bAddBefore ) ? lIndex : lIndex+1, lPoint );
							}
						}
					}
					break;
				default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
			}
		}
	}

	return emErr;
}

// �W�I���g���̍폜�������s��
sindyErrCode CGeometry::Erase( IGeometry* ipQueryGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipQueryGeom, sindyErr_ArgIsNull )
	{
		switch ( GetGeometryType() ) {
			case esriGeometryPoint:
			case esriGeometryPolygon:
			case esriGeometryRing:
			case esriGeometryMultipoint:
			case esriGeometryPolyline:
			case esriGeometryPath:
				{
					CGeometry cQueryGeom( ipQueryGeom );

					// ��v����|�C���g���폜
					for ( point_iterator itQuery = cQueryGeom.point_begin(); itQuery != cQueryGeom.point_end(); ++itQuery ) {
						emErr = EraseVertex( *itQuery );
						if ( emErr != sindyErr_NoErr )	// �G���[�͒��~
							break;
					}
				}
				break;
			default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
		}
	}

	return emErr;
}

// �\���_�̍폜�������s��
sindyErrCode CGeometry::EraseVertex( IPoint* ipQueryPoint )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( ipQueryPoint, sindyErr_ArgIsNull )
	{
		switch ( GetGeometryType() ) {
			case esriGeometryPoint:
				if ( Equals( ipQueryPoint ) )	// ��v�������ɂ���B
					SetEmpty();
				break;
			case esriGeometryPolygon:
			case esriGeometryRing:
				{
					long HitIndex = 0, PartIndex = 0;

					// �q�b�g�H
					if ( HitTest( ipQueryPoint, 0.0, esriGeometryPartVertex, 0, 0, &PartIndex, &HitIndex ) ) {
						IGeometryCollectionPtr ipGeomColl( (IGeometry*)*this );

						// �q�b�g�����p�[�g�̓_�Q����폜
						long i = 0;
						for ( CGeometry::part_iterator itPart = part_begin(); itPart != part_end(); ++itPart, ++i ) {
							if ( i == PartIndex ) {
								CGeometry cPart( *itPart );

								if ( cPart.GetPointCount() - 1 <= 3 ) {
									if ( i != 0 )
										ipGeomColl->RemoveGeometries( i, 1 );
									else
										SetEmpty();
								} else {
									// �n�_���폜�����H
									if ( cPart.GetPoint( 0 ).Equals( ipQueryPoint ) ) {
										// [Bug 6021]�h�[�i�b�c�|���S���̊O���̎n�_���\���_�폜�ŃG���[�ɂȂ�Ȃ�
										// �n�_�������_�Q��˂����݂Ȃ����āA�Ō�̓_���n�_�̎��̓_�ɍX�V
										long nPartCnt = cPart.GetPointCount();
										WKSPoint* point_wks = new WKSPoint[nPartCnt];

										// �n�_�������_�Q�����Ȃ���
										((IPointCollectionPtr)(IGeometry*)cPart)->QueryWKSPoints( 1, nPartCnt - 1, point_wks );
										((IPointCollectionPtr)(IGeometry*)cPart)->SetWKSPoints( nPartCnt - 1, point_wks );
										delete [] point_wks;
										point_wks = NULL;

										// �Ō�̓_���n�_�ɍX�V
										((IPointCollectionPtr)(IGeometry*)cPart)->UpdatePoint( nPartCnt - 2, cPart.GetPoint( 0 ) );
									} else
										((IPointCollectionPtr)(IGeometry*)cPart)->RemovePoints( HitIndex, 1 );

									// [Bug 6021]�h�[�i�b�c�|���S���̊O���̎n�_���\���_�폜�ŃG���[�ɂȂ�Ȃ�
									// �ꉞClose���Ƃ�
									switch ( GetGeometryType() ) {
										case esriGeometryPolygon:
											{
												IPolygonPtr ipPolygon = (IGeometry*)cPart;
												if ( ipPolygon )
													ipPolygon->Close();
											}
											break;
										case esriGeometryRing:
											{
												IRingPtr ipRing = (IGeometry*)cPart;
												if ( ipRing )
													ipRing->Close();
											}
											break;
									}
								}

								break;
							}
						}
					}

					if ( GetPointCount() <= 3 ) // 3�_�ȉ�
						SetEmpty();
				}
				break;
			case esriGeometryMultipoint:
			case esriGeometryPolyline:
			case esriGeometryPath:
				{
					// ���C���n�͂���ȂɃ|�C���g���������Ȃ�����̂ŁA���ׂẴ|�C���g�Ɣ�r
					for ( point_iterator itBase = point_begin(); itBase != point_end(); ) {
						// ��v����|�C���g���폜
						if ( CGeometry( *itBase ).Equals( ipQueryPoint ) ) {
							itBase.erase();
							itBase = point_begin();	// �ŏ������蒼��
							continue;
						}

						++itBase;
					}

					switch ( GetPointCount() ) {
						case 0:
							SetEmpty();
							break;
						case 1:
							*this = GetPoint(0);
							break;
					}
				}
				break;
			default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
		}
	}

	return emErr;
}

// �W�I���g���̕����������s��
sindyErrCode CGeometry::Split( IGeometry* /*ipQueryGeom*/, _ISet** /*ipOtherGeometries = NULL*/, IPoint* /*ipTakeOverPoint = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	emErr = sindyErr_NoImplimentation;
	LOGASSERTE(false,emErr);

	return emErr;
}

// �|�����C����_�ŕ�������
bool CGeometry::Split( IPoint* splitPoint, long* newPartIndex/* = NULL*/, long* hitIndex/* = NULL*/ )
{
	bool bRet = false;	// �Ԃ�l

	// ������
	if( newPartIndex ) *newPartIndex = -1;
	if( hitIndex ) *hitIndex = -1;

	switch( GetGeometryType() )
	{
		case esriGeometryPolyline:
			LOGASSERTE_IF( splitPoint, sindyErr_ArgIsNull )
			{
				IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
				LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
				{
					IPolycurvePtr ipCurve;
					LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPolycurve, (void**)&ipCurve ) ), sindyErr_COMQueryInterfaceFailed )
					{
						VARIANT_BOOL vbSplitHappened = VARIANT_FALSE;
						long lNewPartIndex = -1, lNewSegmentIndex = -1;
						LOGASSERTE_IF( SUCCEEDED( ipCurve->SplitAtPoint( splitPoint, VARIANT_FALSE, -1/*VARIANT_TRUE*/, &vbSplitHappened, &lNewPartIndex, &lNewSegmentIndex ) ), sindyErr_GeometryFunctionFailed )
						{
							bRet = VB2bool(vbSplitHappened);
							if( newPartIndex ) *newPartIndex = lNewPartIndex;
							if( hitIndex ) *hitIndex = lNewSegmentIndex;
						}
					}
				}
			}
			break;
		default:
			LOGASSERTE_IF( esriGeometryPolyline == GetGeometryType(), sindyErr_GeometryNotSupported );
			break;
	}
	return bRet;
}

// �|�����C����_�ŕ�������
sindyErrCode CGeometry::Split( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo/* = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	switch( GetGeometryType() )
	{
		case esriGeometryPolyline:
			LOGASSERTEERR_IF( splitPoints, sindyErr_ArgIsNull )
			{
				IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
				LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
				{
					IPolycurve2Ptr ipCurve;
					LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPolycurve2, (void**)&ipCurve ) ), sindyErr_COMQueryInterfaceFailed )
					{
						try {
							IEnumSplitPointPtr ipSplitInfo;
							LOGASSERTEERR_IF( SUCCEEDED( ipCurve->SplitAtPoints( splitPoints, VARIANT_FALSE, -1/*VARIANT_TRUE*/, cutoffDistance, ( splitInfo ) ? splitInfo : &ipSplitInfo ) ), sindyErr_GeometryFunctionFailed );
						} catch(...) {
							// �|�C���g��������Ȃ��ꍇ��SplitAtPoints�ŗ�O���o��iESRI���� by furuakwa.�j
							// �����ɓ����Ă��Ȃ��悤�ɏ㗬�ŉ�����邱��
							Trace( (IGeometry*)*this, (LPCTSTR)__func__ );
							LOGASSERTEERR_IF( false, sindyErr_ThrowException );
						}
					}
				}
			}
			break;
		default:
			LOGASSERTEERR_IF( esriGeometryPolyline == GetGeometryType(), sindyErr_GeometryNotSupported );
			break;
	}
		
	return emErr;
}

IGeometryPtr CGeometry::Intersect( IGeometry* ipGeom, esriGeometryDimension emGeomDim ) const
{
	IGeometryPtr ipRet;	// �Ԃ�l

	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		esriGeometryType emType = esriGeometryNull, emThisType = GetGeometryType();
		ipGeom->get_GeometryType( &emType );

		// �ʎ����̂Ƃ��ł��������v�Z�ł���悤�ɓK�X�ϊ�����
		IGeometryPtr ipConvertTarget;
//		esriGeometryDimension emConvertDimension = (esriGeometryDimension)-1;
		// esriGeometry0Dimension
		//		Polyline & Polygon  ->  Polyline & Polyline
		//		Polyline & Envelope ->  Polyline & Polyline
		//      Polygon  & Envelope ->  Polyline & Polyline
		// �{���͂����Ƃ��邯�ǁA�K�v�ɂȂ�����������悤 by furukawa.
		IGeometryPtr ipThis( (IGeometry*)*this ), ipInGeom( ipGeom );

		switch( emGeomDim )
		{
			case esriGeometry0Dimension:
				switch( emThisType )
				{
					case esriGeometryPolygon:
					case esriGeometryEnvelope:
						ipThis = Convert( ipThis, esriGeometry1Dimension );
						break;
					case esriGeometryPolyline:
						break;
					default:
						emErr = sindyErr_GeometryNotSupported;
						break;
				}
				switch( emType )
				{
					case esriGeometryPolygon:
					case esriGeometryEnvelope:
						ipInGeom = Convert( ipInGeom, esriGeometry1Dimension );
						break;
					case esriGeometryPolyline:
						break;
					default:
						emErr = sindyErr_GeometryNotSupported;
						break;
				}
				break;
			case esriGeometry1Dimension:
				// Polyline��Polygon�̏ꍇ�́A������������IntersectMultidimension���g�p�ł���̂�������Ȃ����A
				// ���ׂĂȂ��̂łƂ肠����Polyline�ɂ��킹��
				if( emType == esriGeometryPolyline && emThisType == esriGeometryPolygon )
					ipThis = Convert( ipThis, esriGeometry1Dimension );
				else if( emType == esriGeometryPolygon && emThisType == esriGeometryPolyline )
					ipInGeom = Convert( ipInGeom, esriGeometry1Dimension );
				else if( ! ( ( emType == esriGeometryPolyline && emThisType == esriGeometryPolyline ) ||
					         ( emType == esriGeometryPolygon  && emThisType == esriGeometryPolygon  ) ) )
					emErr = sindyErr_GeometryNotSupported;
				break;
			case esriGeometry2Dimension:
				if( ! ( emType == esriGeometryPolygon && emThisType == esriGeometryPolygon ) )
					emErr = sindyErr_GeometryNotSupported;
				break;
			default:
				emErr = sindyErr_GeometryNotSupported;
				break;
		}

		LOGASSERTE_IF( sindyErr_NoErr == emErr, emErr )
		{
			ITopologicalOperatorPtr ipTopo;
			LOGASSERTE_IF( SUCCEEDED( ipThis->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
			{
		/*			// KnownSimple���ǂ����m�F
				VARIANT_BOOL vbIsKnownSimpleBase = VARIANT_FALSE, vbIsKnownSimpleIn = VARIANT_FALSE;
				ipTopo->get_IsKnownSimple( &vbIsKnownSimpleBase );
				((ITopologicalOperatorPtr)ipInGeom)->get_IsKnownSimple( &vbIsKnownSimpleIn );	// Intersect�͓����������m�ł��̂ŁA���RITopologicalOperator�C���^�[�t�F�[�X�͎����Ă���͂�
		*/
				LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Intersect( ipInGeom, emGeomDim, &ipRet ) ), sindyErr_GeometryFunctionFailed )
				{
					// Intersect��̌`��̓V���v���ł͂Ȃ����Ƃɒ��ӁI�I
		/*				if( emGeomDim > esriGeometry0Dimension )
					{
						// Intersect��̌`��KnownSimple���ǂ����m�F
						VARIANT_BOOL vbIsKnownSimpleIntersect = VARIANT_FALSE;
						((ITopologicalOperatorPtr)ipRet)->get_IsKnownSimple( &vbIsKnownSimple );
*/
				}
			}
		}
	}
	
	return ipRet;
}

IGeometryPtr CGeometry::IntersectPoints( IGeometry* ipGeom ) const
{
	IGeometryPtr ipRet;	// �Ԃ�l

	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		// Multipoint�ɂ��Ƃ��Č`����̃|�C���g���o
		CGeometry cGeomMulti( (IGeometry*)ToMultipoint( (IGeometry*)*this ) );

		if ( (IPointPtr)ipGeom ) {
			for ( point_iterator itBase = cGeomMulti.point_begin(); itBase != cGeomMulti.point_end(); ++itBase ) {
				// ��v����|�C���g?
				if ( CGeometry( *itBase ).Equals( ipGeom ) ) {
					ipRet = (IGeometry*)*itBase;
					break;
				}
			}
		} else {
			// �`����ɂ���\���_���擾
			ITopologicalOperator2Ptr ipTopo = ipGeom;
			IGeometryPtr ipIntersectGeom;

			if ( ipTopo )
				ipTopo->IntersectMultidimension( cGeomMulti, &ipIntersectGeom );

			if ( ipIntersectGeom ) {	// �`����Ƀ|�C���g����H
				// �`��ƌ������Ă���|�C���g���܂܂�Ă���̂ŁA��������O����
				ipTopo = ipIntersectGeom;
				if ( ipTopo )
					ipTopo->Intersect( cGeomMulti, esriGeometry0Dimension, &ipIntersectGeom );

				if ( ipIntersectGeom ) {	// �`����Ƀ|�C���g����H
					CGeometry cRetGeom = ipIntersectGeom;

					if ( !cRetGeom.IsEmpty() ) {	// �`�󂠂�H
						switch ( cRetGeom.GetPointCount() ) {
							case 0:
								cRetGeom.SetEmpty();	// ��ɂ��Ă���
								break;
							case 1:
								cRetGeom = cRetGeom.GetPoint( 0 );	// �|�C���g����1�̎��̓|�C���g�ɂ���B
								break;
							default:
								break;
						}

						ipRet = (IGeometry*)cRetGeom;
					}
				}
			}
		}
	}

	return ipRet;
}

IGeometryPtr CGeometry::Buffer( const double& Distance ) const
{
	IGeometryPtr ipRet;	// �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		switch( GetGeometryType() )
		{
			case esriGeometryEnvelope:
			{
				IEnvelopePtr ipEnv = (IEnvelopePtr)AheCreateClone( ipGeom );
				LOGASSERTE_IF( NULL != ipEnv, sindyErr_COMFunctionFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipEnv->Expand( Distance, Distance, VARIANT_FALSE ) ), sindyErr_GeometryFunctionFailed )
						ipRet = ipEnv;
				}
				break;
			}
			default:
			{
				ITopologicalOperatorPtr ipTopo;
				LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_ITopologicalOperator, (void**)&ipTopo ) ), sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipTopo->Buffer( Distance, &ipRet ) ), sindyErr_GeometryFunctionFailed );
				}
				break;
			}
		}
	}
	return ipRet;
}

// �W�I���g���̋�`���擾����
sindyErrCode CGeometry::GetEnvelope( double& dXMin, double& dYMin, double& dXMax, double& dYMax ) const
{
	if( NULL != m_ipCache )
	{
		IEnvelopePtr ipEnv = GetEnvelope();
		if( ipEnv != NULL )
		{
			if( SUCCEEDED( ipEnv->QueryCoords( &dXMin, &dYMin, &dXMax, &dYMax ) ) )
				return sindyErr_NoErr;
		}
	}

	return sindyErr_COMFunctionFailed;
}

// �����o�ϐ�������������
void CGeometry::Init()
{
	m_ipCache = NULL;
}

bool CGeometry::HitTest( IPoint* QueryPoint, const double& searchRadius, esriGeometryHitPartType geometryPart, IPoint* hitPoint/* = NULL*/, double* hitDistance/* = NULL*/, long* hitPartIndex/* = NULL*/, long* hitSegmentIndex/* = NULL*/, bool* bRightSide/* = NULL*/ ) const
{
	bool bRet = false;	// �Ԃ�l

	// ������
	if( hitDistance     ) *hitDistance = -1.0;
	if( hitPartIndex    ) *hitPartIndex = -1;
	if( hitSegmentIndex ) *hitSegmentIndex = -1;
	if( bRightSide      ) *bRightSide = false;

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( QueryPoint, sindyErr_ArgIsNull )
		{
			IHitTestPtr ipHitTest;
			LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IHitTest, (void**)&ipHitTest ) ), sindyErr_COMQueryInterfaceFailed )
			{
				IPointPtr ipHitPoint = ( hitPoint ) ? hitPoint : AheCreateClone(QueryPoint);
				double dHitDistance = -1.0;
				long lHitPartIndex = -1, lHitSegmentIndex = -1;
				VARIANT_BOOL vbRightSide = VARIANT_FALSE, vbHit = VARIANT_FALSE;
				
				LOGASSERTE_IF( SUCCEEDED( ipHitTest->HitTest( QueryPoint, searchRadius, geometryPart, ipHitPoint, &dHitDistance, &lHitPartIndex, &lHitSegmentIndex, &vbRightSide, &vbHit ) ), sindyErr_GeometryFunctionFailed )
				{
					bRet = VB2bool(vbHit);
					if( hitDistance ) *hitDistance = dHitDistance;
					if( hitPartIndex ) *hitPartIndex =lHitPartIndex;
					if( hitSegmentIndex ) *hitSegmentIndex = lHitSegmentIndex;
					if( bRightSide ) *bRightSide = VB2bool(vbRightSide);
				}
			}
		}
	}
	return bRet;
}

// �W�I���g���p�[�g�̎Q�Ƃ�Ԃ�
IGeometryPtr CGeometry::GetPart( long Index ) const
{
	IGeometryPtr ipRet;	// �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( GetPartCount() > Index, sindyErr_ArgLimitOver )
		{
			switch( GetGeometryType() )
			{
				case esriGeometryPoint: ipRet = (IGeometry*)*this; break;
				default:
				{
					IGeometryCollectionPtr ipCol;
					LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IGeometryCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
					{
						LOGASSERTE_IF( SUCCEEDED( ipCol->get_Geometry( Index, &ipRet ) ), sindyErr_GeometryFunctionFailed );
					}
					break;
				}
			}
		}
	}
	return ipRet;	
}

long CGeometry::GetPartCount() const
{
	long lRet = 0; // �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		switch( GetGeometryType() )
		{
			case esriGeometryPoint: lRet = 1; break;
			default:
			{
				IGeometryCollectionPtr ipCol;
				LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IGeometryCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipCol->get_GeometryCount( &lRet ) ), sindyErr_GeometryFunctionFailed );
				}
				break;
			}
		}
	}
	return lRet;
}

// �W�I���g���̃L���b�V�����쐬����
sindyErrCode CGeometry::CreateCache( bool bDetatch/* = false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( IsCached() )
		m_ipCache = NULL;

	if( NULL == ( m_ipCache = AheCreateClone( p ) ) )
		emErr = sindyErr_FunctionFailed;
	else {
	}

	// �؂藣��
	if( bDetatch )
		Release();

	return emErr;
}

#ifdef _DEBUG
// �_����g���[�X����
void CGeometry::Trace( IGeometry* ipGeom, LPCTSTR lpcszHeader/* = NULL*/ )
{
//	AheTraceSpatialReference( ipGeom, lpcszHeader );
	AheTraceGeometry( ipGeom, lpcszHeader );
}
#endif // ifdef _DEBUG

} // sindy

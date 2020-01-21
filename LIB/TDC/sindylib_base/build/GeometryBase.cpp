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
 * @brief <b>CGeometryBase�N���X�����t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include <boost/shared_ptr.hpp>
#include "type_convert.h"
#include "GeometryBase.h"
#include "sindycomutil.h"
#include "GlobalFunctions.h"
#include "sindymacroutil.h"
#include "EnumSplitPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT1_NOT_SIMPLE[] = _T("%s : �����P��Simple�ł͂Ȃ����߁ASimplify�����s���܂�\n");
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT2_NOT_SIMPLE[] = _T("%s : �����Q��Simple�ł͖������߁ASimplify�����s���܂�\n");
const static _TCHAR SHAPEPOINT_ADDED_INFO[]                  = _T("%s : �ǉ����ꂽ�\���_�FPART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO[]               = _T("%s : �\���_�ǉ� PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO2[]              = _T("%s : �\���_�ǉ� PART[%d] INDEX[%d] COUNT[%d]\n");
#else
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT1_NOT_SIMPLE[] = _T("Simplify is performed since the argument 1 is not Simple. \n");
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT2_NOT_SIMPLE[] = _T("Simplify is performed since the argument 2 is not Simple. \n");
const static _TCHAR SHAPEPOINT_ADDED_INFO[]                  = _T("%s : The added shape point:PART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO[]               = _T("%s : Shape point addition PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO2[]              = _T("%s : Shape point addition PART[%d] INDEX[%d] COUNT[%d]\n");
#endif	// SINDY_FOR_ENGLISH

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

// CGeometryBase���R�s�[����
IGeometryPtr CGeometryBase::Clone() const
{
	if( p )
		return AheCreateClone( p );

	return IGeometryPtr();
}

esriGeometryDimension CGeometryBase::GetDimension() const
{
	esriGeometryDimension emRet = (esriGeometryDimension)-1;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Dimension( &emRet ) ), sindyErr_COMFunctionFailed );
	}
	return emRet;
}

IEnvelopePtr CGeometryBase::GetEnvelope() const
{
	IEnvelopePtr ipRet;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Envelope( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

esriGeometryType CGeometryBase::GetGeometryType() const
{
	esriGeometryType emRet = (esriGeometryType)-1;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_GeometryType( &emRet ) ), sindyErr_COMFunctionFailed );
	}
	return emRet;
}

bool CGeometryBase::IsEmpty() const
{
	VARIANT_BOOL vbIsEmpty = VARIANT_FALSE;
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_IsEmpty( &vbIsEmpty ) ), sindyErr_COMFunctionFailed );
	}
	return VB2bool(vbIsEmpty);
}

sindyErrCode CGeometryBase::Project( ISpatialReference* newReferenceSystem )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->Project( newReferenceSystem ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometryBase::QueryEnvelope( IEnvelope* outEnvelope ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->QueryEnvelope( outEnvelope ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometryBase::SetEmpty()
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SetEmpty() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometryBase::SnapToSpatialReference()
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SnapToSpatialReference() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

ISpatialReferencePtr CGeometryBase::GetSpatialReference() const
{
	ISpatialReferencePtr ipRet;	// �Ԃ�l
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_SpatialReference( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

sindyErrCode CGeometryBase::PutRefSpatialReference( ISpatialReference* spatialRef )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->putref_SpatialReference( spatialRef ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

double CGeometryBase::GetLength() const
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
double CGeometryBase::ReturnDistance( IGeometry* ipGeom ) const
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
sindyErrCode CGeometryBase::Union( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	*this = Union( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometryBase::Union( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
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
				CGeometryBase::Trace( ipGeom1, strHead );
				strHead.Format(_T("%s : geom2"), __func__);
				CGeometryBase::Trace( ipGeom2, strHead );
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
							TRACEMESSAGE( SIMPLIFY_PERFOMED_ARGUMENT1_NOT_SIMPLE, __func__ );
							LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed );
						}
						if( ! vbIsSimple2 )
						{
							TRACEMESSAGE( SIMPLIFY_PERFOMED_ARGUMENT2_NOT_SIMPLE, __func__ );
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
							CGeometryBase::Trace( ipRet, strHead );
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
sindyErrCode CGeometryBase::SymmetricDifference( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
	*this = SymmetricDifference( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometryBase::SymmetricDifference( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
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
				CGeometryBase::Trace( ipGeom1, strHead );
				strHead.Format(_T("%s : geom2"), __func__);
				CGeometryBase::Trace( ipGeom2, strHead );
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
							TRACEMESSAGE( SIMPLIFY_PERFOMED_ARGUMENT1_NOT_SIMPLE, __func__ );
							LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Simplify() ), sindyErr_GeometryFunctionFailed );
						}
						if( ! vbIsSimple2 )
						{
							TRACEMESSAGE( SIMPLIFY_PERFOMED_ARGUMENT2_NOT_SIMPLE, __func__ );
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
							CGeometryBase::Trace( ipRet, strHead );
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
sindyErrCode CGeometryBase::Move( IGeometry* ipQueryGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	bool bMoveAll = true;	// �N�G���`��NULL�Ȃ�S�����s�ړ��Ȃ̂ōŏ���true

	Trace( _T("CGeometryBase::Move() : �ړ��O") );

	CGeometryBase cQueryGeom( ipQueryGeom );
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
												TRACEMESSAGE( SHAPEPOINT_ADDED_INFO, __func__, it.part(), it.index(), x, y );
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
sindyErrCode CGeometryBase::MoveVertex( IGeometry* ipQueryGeometry, const std::pair<double,double>& pairMove )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		CGeometryBase cQueryGeometry = ipQueryGeometry;
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
bool CGeometryBase::AddVertex( IPoint* ipPoint, long* pAddPart/* = NULL*/, long* pAddIndex/* = NULL*/ )
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
								TRACEMESSAGE( SHAPEPOINT_ADDITION_INFO, __func__, lPart, lIndex, dX, dY );
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
errorcode::sindyErrCode CGeometryBase::AddVertex( IPoint* ipPoint, long lPart, long lIndex, bool bAddBefore/* = false*/ )
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
								TRACEMESSAGE( SHAPEPOINT_ADDITION_INFO2, __func__, lPart, ( bAddBefore ) ? lIndex : lIndex+1, lPoint );
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
sindyErrCode CGeometryBase::Erase( IGeometry* ipQueryGeom )
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
					CGeometryBase cQueryGeom( ipQueryGeom );

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
sindyErrCode CGeometryBase::EraseVertex( IPoint* ipQueryPoint )
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
						for ( CGeometryBase::part_iterator itPart = part_begin(); itPart != part_end(); ++itPart, ++i ) {
							if ( i == PartIndex ) {
								CGeometryBase cPart( *itPart );

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
										boost::shared_ptr<WKSPoint> p = (boost::shared_ptr<WKSPoint>)new WKSPoint[nPartCnt];

										// �n�_�������_�Q�����Ȃ���
										((IPointCollectionPtr)(IGeometry*)cPart)->QueryWKSPoints( 1, nPartCnt - 1, p.get() );
										((IPointCollectionPtr)(IGeometry*)cPart)->SetWKSPoints( nPartCnt - 1, p.get() );

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
						if ( CGeometryBase( *itBase ).Equals( ipQueryPoint ) ) {
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

/** 
 * @brief �}���`�p�[�g�ȃ|�����C�������ꂼ��̃p�[�g�ō�蒼��
 * @param line [in] �����`��
 * @return �������ꂽ�W�I���g���̃Z�b�g�i0���̏ꍇ����j
 */
_ISetPtr MultiPolyline2Polylines( IPolyline* line )
{
	if( ! line )
		return NULL;
	
	_ISetPtr ret( CLSID_Set ); // �߂�l

	ISpatialReferencePtr ref;
	((IGeometryPtr)line)->get_SpatialReference( &ref );

	CGeometryBase cLine(line);
	for( CGeometryBase::part_iterator it = cLine.part_begin(); it != cLine.part_end(); ++it )
	{
		IGeometryCollectionPtr col( CLSID_Polyline );
		((IGeometryPtr)col)->putref_SpatialReference(ref);
		col->AddGeometry( *it );
		ret->Add(col);
	}

	return ret;
}

/**
 * @brief �}���`�|���S�����V���O���|���S���ɕ�������
 * @param poly [in] �����`��
 * @return �������ꂽ�W�I���g���̃Z�b�g�i0���̏ꍇ����j
 */
_ISetPtr MultiPolygon2Polygons( IPolygon4* poly )
{
	if( ! poly )
		return NULL;

	ISpatialReferencePtr ref;
	((IGeometryPtr)poly)->get_SpatialReference(&ref);

	_ISetPtr ret( CLSID_Set ); // �߂�l

	// exterior�ł΂炷
	IGeometryBagPtr bag;
	if( FAILED( poly->get_ExteriorRingBag( &bag ) ) )
	{
		// IsKnownSimple=VARIANT_FALSE�̏ꍇ�Aget_ExteriorRingBag�����s����
		// ���傤���Ȃ��̂ŁASimplifyPreserveFromTo�������čă`�������W����
		VARIANT_BOOL isKnownSimple = VARIANT_FALSE;
		((ITopologicalOperatorPtr)poly)->get_IsKnownSimple( &isKnownSimple );

		if( !isKnownSimple )
		{
			poly->SimplifyPreserveFromTo();
			if( FAILED( poly->get_ExteriorRingBag( &bag ) ) )
			{
				CGeometryBase::Trace( (IGeometryPtr)poly, _T("get_ExteriorRingBag���s") );
				ret->Add( poly ); // �ǂ����Ă��_���ȏꍇ�͂��̂܂ܕԂ�
			}
		}
		else {
			CGeometryBase::Trace( (IGeometryPtr)poly, _T("get_ExteriorRingBag���s") );
			ret->Add(poly); // �킩��Ȃ��̂łƂ肠�������̂܂ܕԂ�
		}
	}
	CGeometryBase cBag(bag);
	for( CGeometryBase::part_iterator it = cBag.part_begin(); it != cBag.part_end(); ++it )
	{
		IGeometryCollectionPtr newPoly( CLSID_Polygon );
		((IGeometryPtr)newPoly)->putref_SpatialReference(ref);
		newPoly->AddGeometry( *it );
		// exterior�Ɋ܂܂��interior��T���ă|���S�����č\�z
		IGeometryBagPtr bag2;
		poly->get_InteriorRingBag( (IRingPtr)*it, &bag2 );
		if( bag2 )
		{
			CGeometryBase cBag2(bag2);
			for( CGeometryBase::part_iterator it2 = cBag2.part_begin(); it2 != cBag2.part_end(); ++it2 )
				newPoly->AddGeometry( *it2 );
		}
		ret->Add( newPoly );
	}
	return ret;
}

void MergeSet( _ISetPtr set1, _ISetPtr set2 )
{
	if( ! ( set1 && set2 ) )
		return;

	long count = 0;
	set2->get_Count( &count );
	set2->Reset();
	for( long i = 0; i < count; ++i )
	{
		IUnknownPtr unk;
		set2->Next(&unk);
		set1->Add(unk);
	}
}

/// �W�I���g�����󂩂ǂ������肷��
bool IsEmpty( IGeometryPtr geom )
{
	if( !geom )
		return true;

	VARIANT_BOOL isEmpty = VARIANT_FALSE;
	geom->get_IsEmpty( &isEmpty );
	if( isEmpty )
		return true;

	bool ret = true;
	esriGeometryType emType = esriGeometryAny;
	geom->get_GeometryType( &emType );
	switch( emType )
	{
	case esriGeometryPolyline:
	case esriGeometryPath:
		{
			IPolycurvePtr curve(geom);
			double len = 0;
			curve->get_Length( &len );
			if( len > 0.0 )
				ret = false;
			break;
		}
	case esriGeometryBag:
		{
			long count = 0;
			IGeometryCollectionPtr col(geom);
			col->get_GeometryCount(&count);
			for( long i = 0; i < count; ++i )
			{
				IGeometryPtr part;
				col->get_Geometry( i, &part );
				if( !IsEmpty(part) )
					return false;
			}
			break;
		}
	default:
		{
			// �ʐώ�����͖̂ʐςŔ��f
			// ����ȊO�� not empty�ŕԂ�
			IAreaPtr area(geom);
			if( area )
			{
				double dArea = 0;
				area->get_Area(&dArea);
				if( dArea != 0.0 )
					ret = false;
			}
			else
				ret = false;
			break;
		}
	}
	return ret;
}

/**
 * @brief �}���`�p�[�g�Ȍ`����V���O���p�[�g�Ȍ`��ɕ�������
 * 
 * @note �h�[�i�c�|���S���̓h�[�i�c�̂܂܂ł�
 * @param geom [in] �����`��
 * @return �������ꂽ�W�I���g���̃Z�b�g�i0���̏ꍇ����j
 */
_ISetPtr MultiGeometry2Geometries( IGeometry* geom )
{
	if( ! geom )
		return NULL;

	if( IsEmpty(geom) )
		return NULL;

	esriGeometryType emType = esriGeometryAny;
	geom->get_GeometryType( &emType );
	switch( emType )
	{
	case esriGeometryPolyline:
		return MultiPolyline2Polylines( (IPolylinePtr)geom );
	case esriGeometryPolygon:
		return MultiPolygon2Polygons( (IPolygon4Ptr)geom );
	case esriGeometryBag:
		{
			_ISetPtr ret( CLSID_Set );
			long count = 0;
			IGeometryCollectionPtr col(geom);
			col->get_GeometryCount(&count);
			for( long i = 0; i < count; ++i )
			{
				IGeometryPtr part;
				col->get_Geometry( i, &part );
				// GeometryBag�̏ꍇ�͍ċA�������K�v
				_ISetPtr set = MultiGeometry2Geometries( (IGeometryPtr)AheCreateClone(part) );
				MergeSet( ret, set ); // ret <- set
			}
			return ret;
		}
		break;
	default:
		break;
	}
	return NULL;
}

// �W�I���g���̕����������s��
sindyErrCode CGeometryBase::Split( IGeometry* ipQueryGeom, _ISet** ipOtherGeometries/* = NULL*/, IPoint* /*ipTakeOverPoint = NULL*/ )
{
	ITopologicalOperatorPtr topo((IGeometry*)*this);
	if( ! topo )
		return sindyErr_NoErr;

	// �����`��
	// �|�����C���łȂ��ꍇ�̓|�����C���ɕϊ�
	CGeometryBase cutGeom( ipQueryGeom );
	if( cutGeom.GetGeometryType() != esriGeometryPolyline )
		cutGeom = ToPolyline(ipQueryGeom);
	if( 0 >= cutGeom.GetLength() )
	{
		_ASSERTE( 0 < cutGeom.GetLength() );
		return sindyErr_ArgIsConsistency; // �|�����C���ɕϊ��ł��Ȃ�����
	}

	_ISetPtr ipRetSet( CLSID_Set ); // ������̌`�󂪓���
	ITopologicalOperator4Ptr topo4( (IGeometry*)*this );
	if( ! topo )
	{
		// �������̕������@�̏ꍇ�A�h�[�i�c�|���S�������܂������ł��Ȃ�
		// �Ⴆ�΁A���|���S���ɂȂ�悤�ȕ������̏ꍇ�ɁA�������̃|���S��
		// �͍쐬����邪�A�h�[�i�c�����͒��������Ȃ�
		// �Ȃ̂ŁAITopologicalOperator4���g����ꍇ�͂��������g��
		//
		// ����
		IGeometryPtr left, right;
		try {
			if( FAILED( topo->Cut( (IPolylinePtr)(IGeometry*)cutGeom, &left, &right ) ) )
			{ 
				_ASSERT_EXPR( false, GETCOMERRORSTRING() ); // �����Ɏ��s
				return sindyErr_GeometryFunctionFailed; // �W�I���g�����Z���s
			}
		}
		catch( _com_error e )
		{
			_ASSERT_EXPR( false, e.ErrorMessage() ); // ���炭�A0�Ԗڂ̍\���_���܂ނ悤�ȕ��������悤�Ƃ���Əo��l�q�iEditHelper.cpp SplitPolygonByPolyline�j 
			return sindyErr_GeometryFunctionFailed;
		}

		// ������̌`�����̃Z�b�g�Ɋi�[
		CGeometryBase cLeft(left), cRight(right);
		if( 0 < cLeft.GetPointCount() )
		{
			_ISetPtr ipSet( MultiGeometry2Geometries(left) );
			if( ipSet )
				MergeSet( ipRetSet, ipSet );
		}
		if( 0 < cRight.GetPointCount() )
		{
			_ISetPtr ipSet( MultiGeometry2Geometries(right) );
			if( ipSet )
				MergeSet( ipRetSet, ipSet );
		}
	}
	else {
		IGeometryCollectionPtr col;
		if( FAILED( topo4->Cut2( (IPolylinePtr)(IGeometry*)cutGeom, &col ) ) )
		{
//			_ASSERT_EXPR( false, GETCOMERRORSTRING() ); // �� �P���ɕ����ł��Ȃ��ꍇ�ɂ��o��̂ŁA�o���Ȃ�
			//return sindyErr_GeometryFunctionFailed; // �W�I���g�����Z���s
		}
		if( col )
		{
			long count = 0;
			col->get_GeometryCount( &count );
			for( long i = 0; i < count; ++i )
			{
				IGeometryPtr geom;
				col->get_Geometry( i, &geom );
				ipRetSet->Add( geom );
			}
		}
	}
	
	// �����p���`������߂�
	// TODO: ������
	long count = 0;
	ipRetSet->get_Count( &count );
	if( 0 >= count )
	{
		_ASSERTE( 0 < count ); // �K����͎c��Ȃ��Ƃ܂���
		return sindyErr_GeometryFunctionFailed;
	}
	ipRetSet->Reset();
	IUnknownPtr unk;
	ipRetSet->Next(&unk);
	ipRetSet->Remove(unk); // �Z�b�g����͍폜
	*this = (IGeometryPtr)unk;

	if( ipOtherGeometries )
		*ipOtherGeometries = ipRetSet.Detach();

	return sindyErr_NoErr;
}

// �|�����C����_�ŕ�������
bool CGeometryBase::Split( IPoint* splitPoint, long* newPartIndex/* = NULL*/, long* hitIndex/* = NULL*/ )
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
sindyErrCode CGeometryBase::Split( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo/* = NULL*/ )
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

IGeometryPtr CGeometryBase::Intersect( IGeometry* ipGeom, esriGeometryDimension emGeomDim ) const
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

// Simplify�����s����
bool CGeometryBase::Simplify( bool force/* = false */ )
{
	IGeometryPtr geom( (IGeometry*)*this );
	if( ! geom )
		return false;

	switch( GetGeometryType() )
	{
	case esriGeometryPolygon:
	case esriGeometryPolyline:
		if( force )
			((ITopologicalOperator2Ptr)geom)->put_IsKnownSimple( VARIANT_FALSE );
		if( FAILED( ((ITopologicalOperatorPtr)geom)->Simplify() ) )
			return false;
		break;
	default:
		return false;
		break;
	}
	return true;
}

IGeometryPtr CGeometryBase::IntersectPoints( IGeometry* ipGeom ) const
{
	IGeometryPtr ipRet;	// �Ԃ�l

	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		// Multipoint�ɂ��Ƃ��Č`����̃|�C���g���o
		CGeometryBase cGeomMulti( (IGeometry*)ToMultipoint( (IGeometry*)*this ) );

		if ( (IPointPtr)ipGeom ) {
			for ( point_iterator itBase = cGeomMulti.point_begin(); itBase != cGeomMulti.point_end(); ++itBase ) {
				// ��v����|�C���g?
				if ( CGeometryBase( *itBase ).Equals( ipGeom ) ) {
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
					CGeometryBase cRetGeom = ipIntersectGeom;

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

IGeometryPtr CGeometryBase::Buffer( const double& Distance ) const
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
sindyErrCode CGeometryBase::GetEnvelope( double& dXMin, double& dYMin, double& dXMax, double& dYMax ) const
{
	IEnvelopePtr ipEnv( GetEnvelope() );
	if( ipEnv )
	{
		if( FAILED( ipEnv->QueryCoords( &dXMin, &dYMin, &dXMax, &dYMax ) ) )
			return sindyErr_COMFunctionFailed;
	}
	else
		return sindyErr_ClassMemberMissing;

	return sindyErr_NoErr;
}

/*// �����o�ϐ�������������
void CGeometryBase::Init()
{
	m_ipCache = NULL;
}*/

bool CGeometryBase::HitTest( IPoint* QueryPoint, const double& searchRadius, esriGeometryHitPartType geometryPart, IPoint* hitPoint/* = NULL*/, double* hitDistance/* = NULL*/, long* hitPartIndex/* = NULL*/, long* hitSegmentIndex/* = NULL*/, bool* bRightSide/* = NULL*/ ) const
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
IGeometryPtr CGeometryBase::GetPart( long Index ) const
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

long CGeometryBase::GetPartCount() const
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

#ifdef _DEBUG
// �_����g���[�X����
void CGeometryBase::Trace( IGeometry* ipGeom, LPCTSTR lpcszHeader/* = NULL*/ )
{
//	AheTraceSpatialReference( ipGeom, lpcszHeader );
	AheTraceGeometry( ipGeom, lpcszHeader );
}
#endif // ifdef _DEBUG

} // sindy

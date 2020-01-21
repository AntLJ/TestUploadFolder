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
 * @brief <b>CGeometryBaseクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
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
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT1_NOT_SIMPLE[] = _T("%s : 引数１がSimpleではないため、Simplifyを実行します\n");
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT2_NOT_SIMPLE[] = _T("%s : 引数２がSimpleでは無いため、Simplifyを実行します\n");
const static _TCHAR SHAPEPOINT_ADDED_INFO[]                  = _T("%s : 追加された構成点：PART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO[]               = _T("%s : 構成点追加 PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO2[]              = _T("%s : 構成点追加 PART[%d] INDEX[%d] COUNT[%d]\n");
#else
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT1_NOT_SIMPLE[] = _T("Simplify is performed since the argument 1 is not Simple. \n");
const static _TCHAR SIMPLIFY_PERFOMED_ARGUMENT2_NOT_SIMPLE[] = _T("Simplify is performed since the argument 2 is not Simple. \n");
const static _TCHAR SHAPEPOINT_ADDED_INFO[]                  = _T("%s : The added shape point:PART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO[]               = _T("%s : Shape point addition PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
const static _TCHAR SHAPEPOINT_ADDITION_INFO2[]              = _T("%s : Shape point addition PART[%d] INDEX[%d] COUNT[%d]\n");
#endif	// SINDY_FOR_ENGLISH

namespace sindy {
using namespace errorcode;

/// Moveメソッド内での移動補助用クラス
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

// CGeometryBaseをコピーする
IGeometryPtr CGeometryBase::Clone() const
{
	if( p )
		return AheCreateClone( p );

	return IGeometryPtr();
}

esriGeometryDimension CGeometryBase::GetDimension() const
{
	esriGeometryDimension emRet = (esriGeometryDimension)-1;	// 返り値
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Dimension( &emRet ) ), sindyErr_COMFunctionFailed );
	}
	return emRet;
}

IEnvelopePtr CGeometryBase::GetEnvelope() const
{
	IEnvelopePtr ipRet;	// 返り値
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Envelope( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

esriGeometryType CGeometryBase::GetGeometryType() const
{
	esriGeometryType emRet = (esriGeometryType)-1;	// 返り値
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
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->Project( newReferenceSystem ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometryBase::QueryEnvelope( IEnvelope* outEnvelope ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->QueryEnvelope( outEnvelope ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometryBase::SetEmpty()
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SetEmpty() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometryBase::SnapToSpatialReference()
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SnapToSpatialReference() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

ISpatialReferencePtr CGeometryBase::GetSpatialReference() const
{
	ISpatialReferencePtr ipRet;	// 返り値
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_SpatialReference( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

sindyErrCode CGeometryBase::PutRefSpatialReference( ISpatialReference* spatialRef )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->putref_SpatialReference( spatialRef ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

double CGeometryBase::GetLength() const
{
	double dRet = 0.0; // 返り値

	ICurvePtr ipCurve( (IGeometry*)*this );
	if( ipCurve )
	{
		SUCCEEDED_IF( ipCurve->get_Length( &dRet ), sindyErr_GeometryFunctionFailed, _T("%s"), NULL )
		{
		}
	}

	return dRet;
}

// 与えられたジオメトリとの距離を計算する
double CGeometryBase::ReturnDistance( IGeometry* ipGeom ) const
{
	double dRet = -1;	// 返り値

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

// 自分自身にUNIONする
sindyErrCode CGeometryBase::Union( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	*this = Union( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometryBase::Union( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
{
	IGeometryPtr ipRet; // 返り値
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

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
				// Simple かどうかをチェック
				VARIANT_BOOL vbIsSimple1 = VARIANT_FALSE, vbIsSimple2 = VARIANT_FALSE;
				LOGASSERTEERR_IF( SUCCEEDED( ipTopo->get_IsSimple( &vbIsSimple1 ) ) &&
					NULL != ipTopo2 &&
					SUCCEEDED( ipTopo2->get_IsSimple( &vbIsSimple2 ) ), sindyErr_GeometryFunctionFailed )
				{
					if( vbIsSimple1 && vbIsSimple2 )
					{
					}
					else {
						// Simple ではない場合 Union が失敗するので Simplify を実行する
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

// 自分自身にSymmetricDifferenceする
sindyErrCode CGeometryBase::SymmetricDifference( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	*this = SymmetricDifference( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometryBase::SymmetricDifference( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
{
	IGeometryPtr ipRet; // 返り値
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

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
				// Simple かどうかをチェック
				VARIANT_BOOL vbIsSimple1 = VARIANT_FALSE, vbIsSimple2 = VARIANT_FALSE;
				LOGASSERTEERR_IF( SUCCEEDED( ipTopo->get_IsSimple( &vbIsSimple1 ) ) &&
					NULL != ipTopo2 &&
					SUCCEEDED( ipTopo2->get_IsSimple( &vbIsSimple2 ) ), sindyErr_GeometryFunctionFailed )
				{
					if( vbIsSimple1 && vbIsSimple2 )
					{
					}
					else {
						// Simple ではない場合 Union が失敗するので Simplify を実行する
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

// ジオメトリの移動処理を行う
sindyErrCode CGeometryBase::Move( IGeometry* ipQueryGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	bool bMoveAll = true;	// クエリ形状がNULLなら全部平行移動なので最初はtrue

	Trace( _T("CGeometryBase::Move() : 移動前") );

	CGeometryBase cQueryGeom( ipQueryGeom );
	if( cQueryGeom != NULL )
	{
		bMoveAll = false;

		// 移動モードがWholeIntersectの場合はDisjointで比較する
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
								// この場合、ここにくるまでにクエリ形状がポイントになっている必要がある
								LOGASSERTEERR_IF( sindyeMove_Point != emMode, sindyErr_AlgorithmFailed );
								break;
							case sindyeMove_SegmentIntersect:
							case sindyeMove_SegmentWithin:
								emErr = MoveVertex( ipQueryGeom, pairMove );
								break;
							case sindyeMove_SegmentAddPointBorderIntersect:
							case sindyeMove_SegmentAddPointBorderWithin:
							case sindyeMove_SegmentAddPointsBorder:
								// 部分移動するのはポイント以外
								if( GetGeometryType() != esriGeometryPoint )
								{
									// 部分移動処理
									// 交差部分を点で取得
									IGeometryPtr ipIntersectPoint = Intersect( cQueryGeom, esriGeometry0Dimension );
									IPointCollectionPtr ipCol;
									IEnumVertexPtr ipVertex;
									LOGASSERTEERR_IF( NULL != ipIntersectPoint &&
													  SUCCEEDED( ipIntersectPoint->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ) &&
													  SUCCEEDED( ipCol->get_EnumVertices( &ipVertex ) ), sindyErr_GeometryFunctionFailed )
									{
										std::map<long, std::list<_AddPoint> > mapVertIndex;
										CEnumSplitPoint cSplitPoint;
										double dLenTmp = GetLength(); // どのぐらい離れても良いかの閾値（基本的には OnGeometry なので、適当）
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
										// 追加できたパートとインデックス番号から移動すべき構成点を割り出す
										// 若いインデックス番号順にソートした場合、n番目とn+1番目のインデックス番号
										// の間の構成点は、全てクエリ形状の外、又は内となるので間の構成点の一つに対
										// して内外判定をするだけで、ある程度まとめて判定することができる
										// また、この方法では全ての構成点を判定する必要が無いので高速化につながる…はず
										for( std::map<long,std::list<_AddPoint> >::iterator it = mapVertIndex.begin(); it != mapVertIndex.end(); ++it ) it->second.sort();	// まずは各パート内でソートしておく
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

												// 追加構成点間に構成点が一つもない場合は lStart と lEnd が同じになる
												// その場合、for ループ内で処理できないので先に処理
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
																itIndexBefore->Before = false;	// 後でさらに構成点を追加＆移動するため
														}
													}
													if( bIsInside )
													{
														LOGASSERTE_IF( sindyErr_NoErr == ( emErr = MoveVertex( GetPoint(i), pairMove ) ), sindyErr_GeometryFunctionFailed );	// インデックスで指定できた方が良いんじゃない？
													}
												}
												if( bIsLast ) break;
												itIndexBefore = itIndex++;	// インクリメントする前の itIndex を itIndexBefore にコピーしてから、itIndex をインクリメント
												if( itIndex == it->second.end() ) bIsLast = true;
												if( bIsFirst ) bIsFirst = false;
											}
										}
										// 本当なら構成点追加ポイントには2つ構成点を追加されるべきだが、ここまでの処理では一つしか追加されていない
										// 理由は2つあり、一つはIPolycurve::SplitAtPointで同一構成点を作成できないこと、もう一つは2つのうちの一つ
										// は移動対象としなければならないが、一度に追加してしまうとどちらを動かしたらよいかわからなくなるためである。
										// そこで、先に追加した点を記憶しておき、上記の高速移動の際の判定で先に追加した点の前後どちらに追加すればよいかを
										// 調べておき、そしてここで追加して移動するという処理を行っている（もっとスマートな方法ある？）
										for( std::map<long,std::list<_AddPoint> >::iterator it = mapVertIndex.begin(); it != mapVertIndex.end(); ++it )
										{
											for( std::list<_AddPoint>::iterator itIndex = it->second.begin(); itIndex != it->second.end(); ++itIndex )
											{
												CPoint cMovedPoint( (IPointPtr)AheCreateClone( itIndex->Point ) );	// コピーした点が入っているわけではないのでコピーを取る
												cMovedPoint.Move( pairMove );
												LOGASSERTE_IF( sindyErr_NoErr == ( emErr = AddVertex( cMovedPoint, it->first, itIndex->Index, itIndex->Before ) ), sindyErr_GeometryFunctionFailed )
												{
													// 構成点を追加したことにより、保持していたインデックスがずれる
													// そのため、以後のインデックス番号を修正する必要がある
													std::list<_AddPoint>::iterator itIndex2 = itIndex;
													for( ++itIndex2; itIndex2 != it->second.end(); ++itIndex2 )
													{
														if( itIndex2->Index >= itIndex->Index )
															itIndex2->Index++;
													}
			//										LOGASSERTE_IF( sindyErr_NoErr == ( emErr = MoveVertex( itIndex->Point, pairMove ) ), sindyErr_GeometryFunctionFailed ); <- 追加する点をコピーして動かしてから追加するように変更
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
		// 全部平行移動
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
	
// 構成点の移動処理を行う
sindyErrCode CGeometryBase::MoveVertex( IGeometry* ipQueryGeometry, const std::pair<double,double>& pairMove )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		CGeometryBase cQueryGeometry = ipQueryGeometry;
		LOGASSERTEERR_IF( cQueryGeometry != NULL, sindyErr_ArgIsNull )
		{
			for( point_iterator it = point_begin(); it != point_end(); ++it )
			{
				bool bMove = false; // 移動するかどうか
				switch( cQueryGeometry.GetGeometryType() )
				{
					case esriGeometryPoint: bMove = cQueryGeometry.Equals( *it );   break; // クエリ形状がポイントの場合はEqualで比較（bug 5548）
					default:                bMove = cQueryGeometry.Contains( *it ); break; // それ以外はContainsで比較
				}
				if( bMove )
				{
					LOGASSERTE_IF( sindyErr_NoErr == it.move( pairMove ), sindyErr_GeometryFunctionFailed )
					{
					//	TRACEMESSAGE(_T("%s : 構成点移動：PART[%d] INDEX[%d] X=%.20lf(差分%.20lf) Y=%.20lf(差分%.20lf)\n"), __func__, lPart, lIndex, cPoint.GetX(), pairMove.first, cPoint.GetY(), pairMove.second );
					}
				}
			}
		}
	}
	return emErr;
}

// 構成点を追加する
bool CGeometryBase::AddVertex( IPoint* ipPoint, long* pAddPart/* = NULL*/, long* pAddIndex/* = NULL*/ )
{
	bool bRet = false;	// 返り値

	// 初期化
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

// 構成点を追加する
errorcode::sindyErrCode CGeometryBase::AddVertex( IPoint* ipPoint, long lPart, long lIndex, bool bAddBefore/* = false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( ipPoint, sindyErr_ArgIsNull )
	{
		LOGASSERTEERR_IF( ! ( lPart < 0 || lIndex < 0 ), sindyErr_ArgLimitOver )
		{
			IGeometryPtr ipGeom = (IGeometry*)*this;	// Polygon,Polylineの場合は後でパート単位に入れ替えられるのでスマートポインタで
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

// ジオメトリの削除処理を行う
sindyErrCode CGeometryBase::Erase( IGeometry* ipQueryGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

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

					// 一致するポイントを削除
					for ( point_iterator itQuery = cQueryGeom.point_begin(); itQuery != cQueryGeom.point_end(); ++itQuery ) {
						emErr = EraseVertex( *itQuery );
						if ( emErr != sindyErr_NoErr )	// エラーは中止
							break;
					}
				}
				break;
			default: LOGASSERTEERR_IF( false, sindyErr_GeometryNotSupported ); break;
		}
	}

	return emErr;
}

// 構成点の削除処理を行う
sindyErrCode CGeometryBase::EraseVertex( IPoint* ipQueryPoint )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	LOGASSERTEERR_IF( ipQueryPoint, sindyErr_ArgIsNull )
	{
		switch ( GetGeometryType() ) {
			case esriGeometryPoint:
				if ( Equals( ipQueryPoint ) )	// 一致したら空にする。
					SetEmpty();
				break;
			case esriGeometryPolygon:
			case esriGeometryRing:
				{
					long HitIndex = 0, PartIndex = 0;

					// ヒット？
					if ( HitTest( ipQueryPoint, 0.0, esriGeometryPartVertex, 0, 0, &PartIndex, &HitIndex ) ) {
						IGeometryCollectionPtr ipGeomColl( (IGeometry*)*this );

						// ヒットしたパートの点群から削除
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
									// 始点が削除される？
									if ( cPart.GetPoint( 0 ).Equals( ipQueryPoint ) ) {
										// [Bug 6021]ドーナッツポリゴンの外側の始点を構成点削除でエラーにならない
										// 始点を除く点群を突っ込みなおして、最後の点を始点の次の点に更新
										long nPartCnt = cPart.GetPointCount();
										boost::shared_ptr<WKSPoint> p = (boost::shared_ptr<WKSPoint>)new WKSPoint[nPartCnt];

										// 始点を除く点群を入れなおし
										((IPointCollectionPtr)(IGeometry*)cPart)->QueryWKSPoints( 1, nPartCnt - 1, p.get() );
										((IPointCollectionPtr)(IGeometry*)cPart)->SetWKSPoints( nPartCnt - 1, p.get() );

										// 最後の点を始点に更新
										((IPointCollectionPtr)(IGeometry*)cPart)->UpdatePoint( nPartCnt - 2, cPart.GetPoint( 0 ) );
									} else
										((IPointCollectionPtr)(IGeometry*)cPart)->RemovePoints( HitIndex, 1 );

									// [Bug 6021]ドーナッツポリゴンの外側の始点を構成点削除でエラーにならない
									// 一応Closeしとく
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

					if ( GetPointCount() <= 3 ) // 3点以下
						SetEmpty();
				}
				break;
			case esriGeometryMultipoint:
			case esriGeometryPolyline:
			case esriGeometryPath:
				{
					// ライン系はそんなにポイント数が多くない･･･ので、すべてのポイントと比較
					for ( point_iterator itBase = point_begin(); itBase != point_end(); ) {
						// 一致するポイントを削除
						if ( CGeometryBase( *itBase ).Equals( ipQueryPoint ) ) {
							itBase.erase();
							itBase = point_begin();	// 最初からやり直し
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
 * @brief マルチパートなポリラインをそれぞれのパートで作り直す
 * @param line [in] 分解形状
 * @return 分解されたジオメトリのセット（0件の場合あり）
 */
_ISetPtr MultiPolyline2Polylines( IPolyline* line )
{
	if( ! line )
		return NULL;
	
	_ISetPtr ret( CLSID_Set ); // 戻り値

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
 * @brief マルチポリゴンをシングルポリゴンに分解する
 * @param poly [in] 分解形状
 * @return 分解されたジオメトリのセット（0件の場合あり）
 */
_ISetPtr MultiPolygon2Polygons( IPolygon4* poly )
{
	if( ! poly )
		return NULL;

	ISpatialReferencePtr ref;
	((IGeometryPtr)poly)->get_SpatialReference(&ref);

	_ISetPtr ret( CLSID_Set ); // 戻り値

	// exteriorでばらす
	IGeometryBagPtr bag;
	if( FAILED( poly->get_ExteriorRingBag( &bag ) ) )
	{
		// IsKnownSimple=VARIANT_FALSEの場合、get_ExteriorRingBagが失敗する
		// しょうがないので、SimplifyPreserveFromToをかけて再チャレンジする
		VARIANT_BOOL isKnownSimple = VARIANT_FALSE;
		((ITopologicalOperatorPtr)poly)->get_IsKnownSimple( &isKnownSimple );

		if( !isKnownSimple )
		{
			poly->SimplifyPreserveFromTo();
			if( FAILED( poly->get_ExteriorRingBag( &bag ) ) )
			{
				CGeometryBase::Trace( (IGeometryPtr)poly, _T("get_ExteriorRingBag失敗") );
				ret->Add( poly ); // どうしてもダメな場合はそのまま返す
			}
		}
		else {
			CGeometryBase::Trace( (IGeometryPtr)poly, _T("get_ExteriorRingBag失敗") );
			ret->Add(poly); // わからないのでとりあえずそのまま返す
		}
	}
	CGeometryBase cBag(bag);
	for( CGeometryBase::part_iterator it = cBag.part_begin(); it != cBag.part_end(); ++it )
	{
		IGeometryCollectionPtr newPoly( CLSID_Polygon );
		((IGeometryPtr)newPoly)->putref_SpatialReference(ref);
		newPoly->AddGeometry( *it );
		// exteriorに含まれるinteriorを探してポリゴンを再構築
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

/// ジオメトリが空かどうか判定する
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
			// 面積取れるものは面積で判断
			// それ以外は not emptyで返す
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
 * @brief マルチパートな形状をシングルパートな形状に分解する
 * 
 * @note ドーナツポリゴンはドーナツのままです
 * @param geom [in] 分解形状
 * @return 分解されたジオメトリのセット（0件の場合あり）
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
				// GeometryBagの場合は再帰処理が必要
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

// ジオメトリの分割処理を行う
sindyErrCode CGeometryBase::Split( IGeometry* ipQueryGeom, _ISet** ipOtherGeometries/* = NULL*/, IPoint* /*ipTakeOverPoint = NULL*/ )
{
	ITopologicalOperatorPtr topo((IGeometry*)*this);
	if( ! topo )
		return sindyErr_NoErr;

	// 分割形状
	// ポリラインでない場合はポリラインに変換
	CGeometryBase cutGeom( ipQueryGeom );
	if( cutGeom.GetGeometryType() != esriGeometryPolyline )
		cutGeom = ToPolyline(ipQueryGeom);
	if( 0 >= cutGeom.GetLength() )
	{
		_ASSERTE( 0 < cutGeom.GetLength() );
		return sindyErr_ArgIsConsistency; // ポリラインに変換できなかった
	}

	_ISetPtr ipRetSet( CLSID_Set ); // 分割後の形状が入る
	ITopologicalOperator4Ptr topo4( (IGeometry*)*this );
	if( ! topo )
	{
		// こっちの分割方法の場合、ドーナツポリゴンがうまく分割できない
		// 例えば、島ポリゴンになるような分割線の場合に、島部分のポリゴン
		// は作成されるが、ドーナツ部分は中が抜けない
		// なので、ITopologicalOperator4が使える場合はそっちを使う
		//
		// 分割
		IGeometryPtr left, right;
		try {
			if( FAILED( topo->Cut( (IPolylinePtr)(IGeometry*)cutGeom, &left, &right ) ) )
			{ 
				_ASSERT_EXPR( false, GETCOMERRORSTRING() ); // 分割に失敗
				return sindyErr_GeometryFunctionFailed; // ジオメトリ演算失敗
			}
		}
		catch( _com_error e )
		{
			_ASSERT_EXPR( false, e.ErrorMessage() ); // 恐らく、0番目の構成点を含むような分割をしようとすると出る様子（EditHelper.cpp SplitPolygonByPolyline） 
			return sindyErr_GeometryFunctionFailed;
		}

		// 分割後の形状を一つのセットに格納
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
//			_ASSERT_EXPR( false, GETCOMERRORSTRING() ); // ← 単純に分割できない場合にも出るので、出さない
			//return sindyErr_GeometryFunctionFailed; // ジオメトリ演算失敗
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
	
	// 引き継ぐ形状を決める
	// TODO: 未実装
	long count = 0;
	ipRetSet->get_Count( &count );
	if( 0 >= count )
	{
		_ASSERTE( 0 < count ); // 必ず一つは残らないとまずい
		return sindyErr_GeometryFunctionFailed;
	}
	ipRetSet->Reset();
	IUnknownPtr unk;
	ipRetSet->Next(&unk);
	ipRetSet->Remove(unk); // セットからは削除
	*this = (IGeometryPtr)unk;

	if( ipOtherGeometries )
		*ipOtherGeometries = ipRetSet.Detach();

	return sindyErr_NoErr;
}

// ポリラインを点で分割する
bool CGeometryBase::Split( IPoint* splitPoint, long* newPartIndex/* = NULL*/, long* hitIndex/* = NULL*/ )
{
	bool bRet = false;	// 返り値

	// 初期化
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

// ポリラインを点で分割する
sindyErrCode CGeometryBase::Split( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo/* = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

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
							// ポイントが一つしかない場合はSplitAtPointsで例外が出る（ESRI未報告 by furuakwa.）
							// ここに入ってこないように上流で回避すること
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
	IGeometryPtr ipRet;	// 返り値

	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		esriGeometryType emType = esriGeometryNull, emThisType = GetGeometryType();
		ipGeom->get_GeometryType( &emType );

		// 別次元のときでも正しく計算できるように適宜変換する
		IGeometryPtr ipConvertTarget;
//		esriGeometryDimension emConvertDimension = (esriGeometryDimension)-1;
		// esriGeometry0Dimension
		//		Polyline & Polygon  ->  Polyline & Polyline
		//		Polyline & Envelope ->  Polyline & Polyline
		//      Polygon  & Envelope ->  Polyline & Polyline
		// 本当はもっとあるけど、必要になったら実装しよう by furukawa.
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
				// PolylineとPolygonの場合は、もしかしたらIntersectMultidimensionが使用できるのかもしれないが、
				// 調べてないのでとりあえずPolylineにあわせる
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
		/*			// KnownSimpleかどうか確認
				VARIANT_BOOL vbIsKnownSimpleBase = VARIANT_FALSE, vbIsKnownSimpleIn = VARIANT_FALSE;
				ipTopo->get_IsKnownSimple( &vbIsKnownSimpleBase );
				((ITopologicalOperatorPtr)ipInGeom)->get_IsKnownSimple( &vbIsKnownSimpleIn );	// Intersectは同じ次元同士でやるので、当然ITopologicalOperatorインターフェースは持っているはず
		*/
				LOGASSERTEERR_IF( SUCCEEDED( ipTopo->Intersect( ipInGeom, emGeomDim, &ipRet ) ), sindyErr_GeometryFunctionFailed )
				{
					// Intersect後の形状はシンプルではないことに注意！！
		/*				if( emGeomDim > esriGeometry0Dimension )
					{
						// Intersect後の形状がKnownSimpleかどうか確認
						VARIANT_BOOL vbIsKnownSimpleIntersect = VARIANT_FALSE;
						((ITopologicalOperatorPtr)ipRet)->get_IsKnownSimple( &vbIsKnownSimple );
*/
				}
			}
		}
	}
	
	return ipRet;
}

// Simplifyを実行する
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
	IGeometryPtr ipRet;	// 返り値

	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		// Multipointにおとして形状内のポイント抽出
		CGeometryBase cGeomMulti( (IGeometry*)ToMultipoint( (IGeometry*)*this ) );

		if ( (IPointPtr)ipGeom ) {
			for ( point_iterator itBase = cGeomMulti.point_begin(); itBase != cGeomMulti.point_end(); ++itBase ) {
				// 一致するポイント?
				if ( CGeometryBase( *itBase ).Equals( ipGeom ) ) {
					ipRet = (IGeometry*)*itBase;
					break;
				}
			}
		} else {
			// 形状内にある構成点を取得
			ITopologicalOperator2Ptr ipTopo = ipGeom;
			IGeometryPtr ipIntersectGeom;

			if ( ipTopo )
				ipTopo->IntersectMultidimension( cGeomMulti, &ipIntersectGeom );

			if ( ipIntersectGeom ) {	// 形状内にポイントあり？
				// 形状と交差しているポイントが含まれているので、それを除外する
				ipTopo = ipIntersectGeom;
				if ( ipTopo )
					ipTopo->Intersect( cGeomMulti, esriGeometry0Dimension, &ipIntersectGeom );

				if ( ipIntersectGeom ) {	// 形状内にポイントあり？
					CGeometryBase cRetGeom = ipIntersectGeom;

					if ( !cRetGeom.IsEmpty() ) {	// 形状あり？
						switch ( cRetGeom.GetPointCount() ) {
							case 0:
								cRetGeom.SetEmpty();	// 空にしておく
								break;
							case 1:
								cRetGeom = cRetGeom.GetPoint( 0 );	// ポイント数が1の時はポイントにする。
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
	IGeometryPtr ipRet;	// 返り値

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

// ジオメトリの矩形を取得する
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

/*// メンバ変数を初期化する
void CGeometryBase::Init()
{
	m_ipCache = NULL;
}*/

bool CGeometryBase::HitTest( IPoint* QueryPoint, const double& searchRadius, esriGeometryHitPartType geometryPart, IPoint* hitPoint/* = NULL*/, double* hitDistance/* = NULL*/, long* hitPartIndex/* = NULL*/, long* hitSegmentIndex/* = NULL*/, bool* bRightSide/* = NULL*/ ) const
{
	bool bRet = false;	// 返り値

	// 初期化
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

// ジオメトリパートの参照を返す
IGeometryPtr CGeometryBase::GetPart( long Index ) const
{
	IGeometryPtr ipRet;	// 返り値

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
	long lRet = 0; // 返り値

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
// 点列をトレースする
void CGeometryBase::Trace( IGeometry* ipGeom, LPCTSTR lpcszHeader/* = NULL*/ )
{
//	AheTraceSpatialReference( ipGeom, lpcszHeader );
	AheTraceGeometry( ipGeom, lpcszHeader );
}
#endif // ifdef _DEBUG

} // sindy

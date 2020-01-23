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
 * @brief <b>CGeometryクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
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
	LPCTSTR SIMPLIFY_PERFORMED_SINCE_ARG1_NOT_SIMPLE = _T("%s : 引数１がSimpleではないため、Simplifyを実行します\n");
	LPCTSTR SIMPLIFY_PERFORMED_SINCE_ARG2_NOT_SIMPLE = _T("%s : 引数２がSimpleでは無いため、Simplifyを実行します\n");
	LPCTSTR INFO_ADDED_SHAPE_POINT                   = _T("%s : 追加された構成点：PART[%d] INDEX[%d] X=%.20lf Y=%.20lf\n");
	LPCTSTR ADD_SHAPE_POINT_PART_INDEX               = _T("%s : 構成点追加 PART[%d] INDEX[%d] X=%.20lf Y=%20.lf\n");
	LPCTSTR ADD_SHAPE_POINT_PART_INDEX_COUNT         = _T("%s : 構成点追加 PART[%d] INDEX[%d] COUNT[%d]\n");
#endif // SINDY_FOR_ENGLISH
} // namespace

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

// CGeometryをコピーする
void CopyCGeometry( CGeometry& dest, const CGeometry& src )
{
	dest.Init();
	dest = (IGeometry*)src;
	dest.m_ipCache = ( src.m_ipCache ) ? (IGeometryPtr)AheCreateClone( src.m_ipCache ) : NULL;
}

esriGeometryDimension CGeometry::GetDimension() const
{
	esriGeometryDimension emRet = (esriGeometryDimension)-1;	// 返り値
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Dimension( &emRet ) ), sindyErr_COMFunctionFailed );
	}
	return emRet;
}

IEnvelopePtr CGeometry::GetEnvelope() const
{
	IEnvelopePtr ipRet;	// 返り値
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_Envelope( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

esriGeometryType CGeometry::GetGeometryType() const
{
	esriGeometryType emRet = (esriGeometryType)-1;	// 返り値
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
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->Project( newReferenceSystem ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometry::QueryEnvelope( IEnvelope* outEnvelope ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->QueryEnvelope( outEnvelope ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometry::SetEmpty()
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SetEmpty() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

sindyErrCode CGeometry::SnapToSpatialReference()
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->SnapToSpatialReference() ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

ISpatialReferencePtr CGeometry::GetSpatialReference() const
{
	ISpatialReferencePtr ipRet;	// 返り値
	LOGASSERTE_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTE_IF( SUCCEEDED( ((IGeometry*)*this)->get_SpatialReference( &ipRet ) ), sindyErr_COMFunctionFailed );
	}
	return ipRet;
}

sindyErrCode CGeometry::PutRefSpatialReference( ISpatialReference* spatialRef )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	LOGASSERTEERR_IF( (IGeometry*)*this, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)*this)->putref_SpatialReference( spatialRef ) ), sindyErr_COMFunctionFailed );
	}
	return emErr;
}

double CGeometry::GetLength() const
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
double CGeometry::ReturnDistance( IGeometry* ipGeom ) const
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
sindyErrCode CGeometry::Union( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	if ( IsCached() )
		m_ipCache = Union( (IGeometry*)*this, ipGeom );
	else
		*this = Union( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometry::Union( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
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
				CGeometry::Trace( ipGeom1, strHead );
				strHead.Format(_T("%s : geom2"), __func__);
				CGeometry::Trace( ipGeom2, strHead );
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

// 自分自身にSymmetricDifferenceする
sindyErrCode CGeometry::SymmetricDifference( IGeometry* ipGeom )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
	if ( IsCached() )
		m_ipCache = SymmetricDifference( (IGeometry*)*this, ipGeom );
	else
		*this = SymmetricDifference( (IGeometry*)*this, ipGeom );
	return emErr;
}

IGeometryPtr CGeometry::SymmetricDifference( IGeometry* ipGeom1,IGeometry* ipGeom2, sindyErrCode* pErr/* = NULL*/ )
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
				CGeometry::Trace( ipGeom1, strHead );
				strHead.Format(_T("%s : geom2"), __func__);
				CGeometry::Trace( ipGeom2, strHead );
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

// ジオメトリの移動処理を行う
sindyErrCode CGeometry::Move( IGeometry* ipQueryGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	bool bMoveAll = true;	// クエリ形状がNULLなら全部平行移動なので最初はtrue

	Trace( _T("CGeometry::Move() : 移動前") );

	CGeometry cQueryGeom( ipQueryGeom );
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
												TRACEMESSAGE(INFO_ADDED_SHAPE_POINT, __func__, it.part(), it.index(), x, y );
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
sindyErrCode CGeometry::MoveVertex( IGeometry* ipQueryGeometry, const std::pair<double,double>& pairMove )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		CGeometry cQueryGeometry = ipQueryGeometry;
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
bool CGeometry::AddVertex( IPoint* ipPoint, long* pAddPart/* = NULL*/, long* pAddIndex/* = NULL*/ )
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

// 構成点を追加する
errorcode::sindyErrCode CGeometry::AddVertex( IPoint* ipPoint, long lPart, long lIndex, bool bAddBefore/* = false*/ )
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

// ジオメトリの削除処理を行う
sindyErrCode CGeometry::Erase( IGeometry* ipQueryGeom )
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
					CGeometry cQueryGeom( ipQueryGeom );

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
sindyErrCode CGeometry::EraseVertex( IPoint* ipQueryPoint )
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
						for ( CGeometry::part_iterator itPart = part_begin(); itPart != part_end(); ++itPart, ++i ) {
							if ( i == PartIndex ) {
								CGeometry cPart( *itPart );

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
										WKSPoint* point_wks = new WKSPoint[nPartCnt];

										// 始点を除く点群を入れなおし
										((IPointCollectionPtr)(IGeometry*)cPart)->QueryWKSPoints( 1, nPartCnt - 1, point_wks );
										((IPointCollectionPtr)(IGeometry*)cPart)->SetWKSPoints( nPartCnt - 1, point_wks );
										delete [] point_wks;
										point_wks = NULL;

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
						if ( CGeometry( *itBase ).Equals( ipQueryPoint ) ) {
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

// ジオメトリの分割処理を行う
sindyErrCode CGeometry::Split( IGeometry* /*ipQueryGeom*/, _ISet** /*ipOtherGeometries = NULL*/, IPoint* /*ipTakeOverPoint = NULL*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	emErr = sindyErr_NoImplimentation;
	LOGASSERTE(false,emErr);

	return emErr;
}

// ポリラインを点で分割する
bool CGeometry::Split( IPoint* splitPoint, long* newPartIndex/* = NULL*/, long* hitIndex/* = NULL*/ )
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
sindyErrCode CGeometry::Split( IEnumVertex* splitPoints, const double& cutoffDistance, IEnumSplitPoint** splitInfo/* = NULL*/ )
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

IGeometryPtr CGeometry::Intersect( IGeometry* ipGeom, esriGeometryDimension emGeomDim ) const
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

IGeometryPtr CGeometry::IntersectPoints( IGeometry* ipGeom ) const
{
	IGeometryPtr ipRet;	// 返り値

	sindyErrCode emErr = sindyErr_NoErr;

	LOGASSERTEERR_IF( ipGeom, sindyErr_ArgIsNull )
	{
		// Multipointにおとして形状内のポイント抽出
		CGeometry cGeomMulti( (IGeometry*)ToMultipoint( (IGeometry*)*this ) );

		if ( (IPointPtr)ipGeom ) {
			for ( point_iterator itBase = cGeomMulti.point_begin(); itBase != cGeomMulti.point_end(); ++itBase ) {
				// 一致するポイント?
				if ( CGeometry( *itBase ).Equals( ipGeom ) ) {
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
					CGeometry cRetGeom = ipIntersectGeom;

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

IGeometryPtr CGeometry::Buffer( const double& Distance ) const
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

// メンバ変数を初期化する
void CGeometry::Init()
{
	m_ipCache = NULL;
}

bool CGeometry::HitTest( IPoint* QueryPoint, const double& searchRadius, esriGeometryHitPartType geometryPart, IPoint* hitPoint/* = NULL*/, double* hitDistance/* = NULL*/, long* hitPartIndex/* = NULL*/, long* hitSegmentIndex/* = NULL*/, bool* bRightSide/* = NULL*/ ) const
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
IGeometryPtr CGeometry::GetPart( long Index ) const
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

long CGeometry::GetPartCount() const
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

// ジオメトリのキャッシュを作成する
sindyErrCode CGeometry::CreateCache( bool bDetatch/* = false*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( IsCached() )
		m_ipCache = NULL;

	if( NULL == ( m_ipCache = AheCreateClone( p ) ) )
		emErr = sindyErr_FunctionFailed;
	else {
	}

	// 切り離し
	if( bDetatch )
		Release();

	return emErr;
}

#ifdef _DEBUG
// 点列をトレースする
void CGeometry::Trace( IGeometry* ipGeom, LPCTSTR lpcszHeader/* = NULL*/ )
{
//	AheTraceSpatialReference( ipGeom, lpcszHeader );
	AheTraceGeometry( ipGeom, lpcszHeader );
}
#endif // ifdef _DEBUG

} // sindy

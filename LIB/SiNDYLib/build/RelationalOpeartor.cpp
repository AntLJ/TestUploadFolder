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
 * @brief <b>CGeometryクラスコンバート系実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "util.h"
#include "sindycomutil.h"
#include "AheGeometryOp.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR CATCH_EXCEPTION           = _T("%s : An exception has been detected.\n");
	LPCTSTR FAILED_GEO_CONTAION       = _T("CGeometry::Contains() : Failure");
	LPCTSTR DISJOINT_SECOND_CHALLENGE = _T("IRelationalOperator::Disjoint : SecondChallenge");
	LPCTSTR EQUALS_SECOND_CHALLENGE   = _T("IRelationalOperator::Equals : SecondChallenge");
	LPCTSTR OVERLAPS_SECOND_CHALLENGE = _T("IRelationalOperator::Overlaps : SecondChallenge");
#else
	LPCTSTR CATCH_EXCEPTION           = _T("%s : 例外をキャッチしました\n");
	LPCTSTR FAILED_GEO_CONTAION       = _T("CGeometry::Contains() : 失敗");
	LPCTSTR DISJOINT_SECOND_CHALLENGE = _T("IRelationalOperator::Disjoint : SecondChallenge を行います");
	LPCTSTR EQUALS_SECOND_CHALLENGE   = _T("IRelationalOperator::Equals : SecondChallenge を行います");
	LPCTSTR OVERLAPS_SECOND_CHALLENGE = _T("IRelationalOperator::Overlaps : SecondChallenge を行います");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

#define RELATIONSHIP_MACRO( func ) \
	VARIANT_BOOL vb = VARIANT_FALSE; \
	try { \
		if( IsCached() ) \
		{ \
			IGeometry* p = m_ipCache; \
			QUERYINVOKEMETHOD( IRelationalOperator, IID_IRelationalOperator, func( ipGeom, &vb ) ); \
			if( FAILED(hr) ) \
			{ \
				ERRORLOG(sindyErr_GeometryFunctionFailed); \
				SASSERT( SUCCEEDED(hr) ); \
			} \
		} \
		else { \
			QUERYINVOKEMETHOD( IRelationalOperator, IID_IRelationalOperator, func( ipGeom, &vb ) ); \
			if( FAILED(hr) ) \
			{ \
				ERRORLOG(sindyErr_GeometryFunctionFailed); \
				SASSERT( SUCCEEDED(hr) ); \
			} \
		} \
	} \
	catch(...) { \
		ERRORLOG(sindyErr_GeometryFunctionFailed); \
		TRACEMESSAGE( CATCH_EXCEPTION, __func__ ); \
		SASSERT(false); \
	} \
	return VB2bool(vb);

extern FILE* g_Errlog;

bool _Equals( IGeometry* ipBase, IGeometry* ipTest )
{
	return BOOL2bool( AheIsEqual( ipBase, ipTest ) );
}

bool _Contains( IGeometry* ipBase, IGeometry* ipTest )
{
	bool bRet = false;	// 返り値

	LOGASSERTE_IF( ipBase && ipTest, sindyErr_ArgIsNull )
	{
		// ポリライン＆ポリラインの場合は、IRelationalOperator::Containsのバグ回避（bug 4895、bug 4806）及び、
		// 自己交差オブジェクトの場合の回避（bug 5126）の為に点列比較とする
		esriGeometryType emTypeBase = esriGeometryAny, emTypeTest = esriGeometryAny;
		ipBase->get_GeometryType( &emTypeBase );
		ipTest->get_GeometryType( &emTypeTest );
		if( emTypeBase == esriGeometryPolyline && emTypeTest == esriGeometryPolyline )
		{
			// 1. 順、逆両方向比較する（bug 5240)
			// 2. ipBaseがリング形状の場合は考慮しない
			// 3. Contains開始点と同じ座標を持つ点がContains開始点より前にあった場合の事を考えたくないのでその場合falseとする
			for( int i = 0; i < 2; ++i )
			{
				// 一回目は順方向、二回目は逆方向
				PointIterator itBase = PointIterator( ipBase );
				PointIterator itTest = PointIterator( ipTest, ( 0 == i ) ? true : false );
				for( ; itBase != PointIterator(); ++itBase )
				{
					if( _Equals( *itBase, *itTest ) )
					{
						bRet = true;
						break;
					}
				}

				// itBaseが最終点だった場合の対策
				if( PointIterator() == ++itBase )
					bRet = false;

				// 次の点からは一緒にインクリメントしながら比較（itBaseは上で既に一つ進めてるので初回進めるのはitTestのみ）
				for( ++itTest; itTest != PointIterator(); ++itBase, ++itTest )
				{
					// itBaseがitTestより先に終わってしまった場合の対策
					if( itBase == PointIterator() )
					{
						bRet = false;
						break;	
					}

					if( ! _Equals( *itBase, *itTest ) )
					{
						bRet = false;
						break;
					}
				}
				if( bRet )
					break;
			}
		}
		// どちらかがポリラインではない場合は通常ルーチンで判定する
		else {
			IRelationalOperatorPtr ipRelOpe;
			SUCCEEDED_IF( ipBase->QueryInterface( IID_IRelationalOperator, (void**)&ipRelOpe ), sindyErr_COMQueryInterfaceFailed, NULL )
			{
				VARIANT_BOOL vb = VARIANT_FALSE;
				SUCCEEDED_IF( ipRelOpe->Contains( ipTest, &vb ), sindyErr_GeometryFunctionFailed, _T("%s"), FAILED_GEO_CONTAION )
				{
					bRet = VB2bool(vb);
				}
			}
		}
	}
  	return bRet;
}

bool AheGeometryContains( IGeometry* ipBase, IGeometry* ipTest )
{
	bool bRet = false; // 返り値

	if( ipBase == NULL || ipTest == NULL )
		return bRet;

	IGeometryPtr ipThis( ipBase ), ipInGeom( ipTest );
	esriGeometryType emType = esriGeometryNull, emThisType = esriGeometryNull;
	ipThis->get_GeometryType( &emThisType );
	ipInGeom->get_GeometryType( &emType );
	
	// Ring、Path、Lineは上位の形状に変換
	if( esriGeometryRing == emType ) 
		ipInGeom = CGeometry::ToPolygon( ipInGeom );
	else if( esriGeometryPath == emType ||
		     esriGeometryLine == emType )
		ipInGeom = CGeometry::ToPolyline( ipInGeom );

	if( esriGeometryRing == emThisType )
		ipThis = CGeometry::ToPolygon( ipThis );
	else if( esriGeometryPath == emThisType ||
		     esriGeometryLine == emThisType )
		ipThis = CGeometry::ToPolyline( ipThis );

	return _Contains( ipThis, ipInGeom );
}

bool CGeometry::Contains( IGeometry* ipGeom ) const
{
	return AheGeometryContains( (IGeometry*)(*this), ipGeom );
}

bool CGeometry::Crosses( IGeometry* ipGeom ) const
{
	RELATIONSHIP_MACRO( Crosses );
}

bool CGeometry::Disjoint( IGeometry* ipGeom ) const
{
#if 0
	RELATIONSHIP_MACRO( Disjoint );
#else
	VARIANT_BOOL vb = VARIANT_FALSE;	// 判定値
	HRESULT hr = S_OK;
	IRelationalOperatorPtr ipRelOpe = ( IsCached() ) ? m_ipCache : p;
	ITopologicalOperator2Ptr ipTopo;	// KnownSimple 一時操作用
	bool bSecondChallenge = false, bSetBaseKnownSimple = false, bSetTargetKnownSimple = false;

RELATIONAL_OPERATION:
	try {
		LOGASSERTE_IF( NULL != ipRelOpe, sindyErr_ClassMemberMissing )
		{
			if( FAILED( hr = ipRelOpe->Disjoint( ipGeom, &vb ) ) )
			{
				ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Disjoint : 判定処理に失敗しました );
				goto ERROR_HANDLE;
			}
		}
	}
	catch(...) {
		ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Disjoint : 例外をキャッチしました );
		if( ! bSecondChallenge )
		{
			// KnownSimple が FALSE だと判定できない？？
			// なので、ここで一時的に TRUE にして再度判定
			VARIANT_BOOL vbKnownSimple = VARIANT_FALSE;
			ITopologicalOperator2Ptr ipTopo( ipRelOpe );
			ipTopo->get_IsKnownSimple( &vbKnownSimple );
			if( ! vbKnownSimple )
			{
				bSecondChallenge = true;
				ipTopo->put_IsKnownSimple( /*VARIANT_TRUE*/-1 );
				bSetBaseKnownSimple = true;
			}
			ipTopo = ipGeom;
			ipTopo->get_IsKnownSimple( &vbKnownSimple );
			if( ! vbKnownSimple )
			{
				bSecondChallenge = true;
				ipTopo->put_IsKnownSimple( /*VARIANT_TRUE*/-1 );
				bSetTargetKnownSimple = true;
			}
			if( bSetBaseKnownSimple || bSetTargetKnownSimple )
			{
				TRACEMESSAGE(_T("%s\n"), DISJOINT_SECOND_CHALLENGE );
				goto RELATIONAL_OPERATION;
			}
			else
				goto ERROR_HANDLE;
		}
		else {
			ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Disjoint : SecondChallenge にも失敗しました );
			goto ERROR_HANDLE;
		}
	}
	goto RETURN_END;

ERROR_HANDLE:
	CGeometry::Trace( (IGeometryPtr)ipRelOpe, _T("比較元ジオメトリ") );
	CGeometry::Trace( ipGeom, _T("比較先ジオメトリ") );

	SASSERTE(hr);

RETURN_END:
	// KnownSimple を一時的にセットしたなら、ここで
	// 元の値に戻す
	if( bSetBaseKnownSimple )
	{
		ipTopo = ipRelOpe;
		ipTopo->put_IsKnownSimple( VARIANT_FALSE );
	}
	if( bSetTargetKnownSimple )
	{
		ipTopo = ipGeom;
		ipTopo->put_IsKnownSimple( VARIANT_FALSE );
	}

	return VB2bool(vb);
#endif // if 0
}

bool CGeometry::Equals( IGeometry* ipGeom, bool bIgnoreOrient/* = false*/ ) const
{
#if 0
	RELATIONSHIP_MACRO( Equals );
#else
	IGeometryPtr ipThis = (IGeometry*)(*this);
	IGeometryPtr ipTest = ipGeom;

	// 必要なメンバチェック
	if( ipThis == NULL || ipTest == NULL )
		return false;

	bool bRet = false; // 返り値

	// 型を取得
	esriGeometryType emThisType = esriGeometryAny, emTestType = esriGeometryAny;
	ipThis->get_GeometryType( &emThisType );
	ipTest->get_GeometryType( &emTestType );
	// 型が違う場合は比較できない
	if( emThisType != emTestType )
		return false;

	switch( emThisType )
	{
		case esriGeometryLine:
		{
			// 面倒なので、FROMとTOしか比較しない
			_ISegmentPtr ipThisSeg( ipThis ), ipTestSeg( ipTest );
			if( ipThisSeg != NULL && ipTestSeg != NULL )
			{
				IPointPtr ipToThis, ipFromThis, ipToTest, ipFromTest;
				ipThisSeg->get_ToPoint( &ipToThis );
				ipThisSeg->get_FromPoint( &ipFromThis );
				ipTestSeg->get_ToPoint( &ipToTest );
				ipTestSeg->get_FromPoint( &ipFromTest );
				VARIANT_BOOL vbEqualsTo = VARIANT_FALSE, vbEqualsFrom = VARIANT_FALSE;
				((IRelationalOperatorPtr)ipToThis)->Equals( ipToTest, &vbEqualsTo );
				((IRelationalOperatorPtr)ipFromThis)->Equals( ipFromTest, &vbEqualsFrom );
				if( vbEqualsTo && vbEqualsFrom )
					bRet = true;
				else {
					if( bIgnoreOrient )
					{
						((IRelationalOperatorPtr)ipToThis)->Equals( ipFromTest, &vbEqualsTo );
						((IRelationalOperatorPtr)ipFromThis)->Equals( ipToTest, &vbEqualsFrom );
						bRet = ( vbEqualsTo && vbEqualsFrom );
					}
				}
			}
			break;
		}
		case esriGeometryPath:
		case esriGeometryRing:
		{
			// PathとRingはPolygonとPolylineに変換する必要がある
			if( esriGeometryPath )
			{
				ipThis = ToPolyline( ipThis );
				ipTest = ToPolyline( ipTest );
			}
			else {
				ipThis = ToPolygon( ipThis );
				ipTest = ToPolygon( ipTest );
			}
			// breakせずに、そのまま下を実行
		}
		case esriGeometryPoint:
		case esriGeometryPolyline:
		case esriGeometryPolygon:
		case esriGeometryEnvelope:
		case esriGeometryBag:
		case esriGeometryMultipoint:
		{
			if( bIgnoreOrient )
			{
				// 今のところ、向きを無視できるのはセグメントのみしか実装していない
				// ので、その警告を出す
				ERRORLOG2( sindyErr_AlgorithmFailed, IRelationalOperator::Equals : bIgnoreOrientはSegmentの場合のみ有効です );
			}
			VARIANT_BOOL vb = VARIANT_FALSE;	// 判定値
			HRESULT hr = S_OK;

//#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
//			IRelationalOperatorPtr ipRelOpe = ipThis;
//#else
			IClonePtr ipRelOpe( ipThis );
//#endif // ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			ITopologicalOperator2Ptr ipTopo;	// KnownSimple 一時操作用
			bool bSecondChallenge = false, bSetBaseKnownSimple = false, bSetTargetKnownSimple = false;

EQUALS_OPERATION:
			try {
				LOGASSERTE_IF( NULL != ipRelOpe, sindyErr_ClassMemberMissing )
				{
//#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
//					if( FAILED( hr = ipRelOpe->Equals( ipTest, &vb ) ) )
//#else
					if( FAILED( hr = ipRelOpe->IsEqual( (IClonePtr)ipTest, &vb ) ) )
//#endif // ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
					{
						ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Equals : 判定処理に失敗しました );
						goto EQUALS_ERROR_HANDLE;
					}

				}
			}
			catch(...) {
				ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Equals : 例外をキャッチしました );
				if( ! bSecondChallenge )
				{
					// KnownSimple が FALSE だと判定できない？？
					// なので、ここで一時的に TRUE にして再度判定
					VARIANT_BOOL vbKnownSimple = VARIANT_FALSE;
					ITopologicalOperator2Ptr ipTopo( ipRelOpe );
					ipTopo->get_IsKnownSimple( &vbKnownSimple );
					if( ! vbKnownSimple )
					{
						bSecondChallenge = true;
						ipTopo->put_IsKnownSimple( /*VARIANT_TRUE*/-1 );
						bSetBaseKnownSimple = true;
					}
					ipTopo = ipTest;
					ipTopo->get_IsKnownSimple( &vbKnownSimple );
					if( ! vbKnownSimple )
					{
						bSecondChallenge = true;
						ipTopo->put_IsKnownSimple( /*VARIANT_TRUE*/-1 );
						bSetTargetKnownSimple = true;
					}
					if( bSetBaseKnownSimple || bSetTargetKnownSimple )
					{
						TRACEMESSAGE(_T("%s\n"), EQUALS_SECOND_CHALLENGE );
						goto EQUALS_OPERATION;
					}
					else
						goto EQUALS_ERROR_HANDLE;
				}
				else {
					ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Equals : SecondChallenge にも失敗しました );
					goto EQUALS_ERROR_HANDLE;
				}
			}
			goto EQUALS_END;

EQUALS_ERROR_HANDLE:
			CGeometry::Trace( (IGeometryPtr)ipRelOpe, _T("比較元ジオメトリ") );
			CGeometry::Trace( ipTest, _T("比較先ジオメトリ") );

			SASSERTE(hr);

EQUALS_END:
			// KnownSimple を一時的にセットしたなら、ここで
			// 元の値に戻す
			if( bSetBaseKnownSimple )
			{
				ipTopo = ipRelOpe;
				ipTopo->put_IsKnownSimple( VARIANT_FALSE );
			}
			if( bSetTargetKnownSimple )
			{
				ipTopo = ipTest;
				ipTopo->put_IsKnownSimple( VARIANT_FALSE );
			}

			bRet = VB2bool(vb);

			break;
		}
		default:
			ERRORLOG2( sindyErr_GeometryNotSupported, CGeometry::Euqals : 渡された形状の型は現在サポートされていません );
			break;
}

	return bRet;
#endif // if 0
}

// 点列比較で同じかどうか判定する
bool CGeometry::EqualsVertex( IGeometry* ipGeom ) const
{
	IGeometryPtr ipThis = (IGeometry*)(*this);
	IGeometryPtr ipTest = ipGeom;

	// 必要なメンバチェック
	if( ! ( ipThis && ipTest ) )
		return false;

	bool bRet = false; // 返り値

	// 型を取得
	esriGeometryType emThisType = esriGeometryAny, emTestType = esriGeometryAny;
	ipThis->get_GeometryType( &emThisType );
	ipTest->get_GeometryType( &emTestType );
	// 型が違う場合は比較できない
	if( emThisType != emTestType )
		return false;

	switch( emThisType )
	{
		case esriGeometryPoint:
		{
#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			return _Equals( ipThis, ipTest );
#else
			double dx1 = 0.0, dx2 = 0.0, dy1 = 0.0, dy2 = 0.0;
			((IPointPtr)ipThis)->QueryCoords( &dx1, &dy1 );
			((IPointPtr)ipTest)->QueryCoords( &dx2, &dy2 );
			return ( dx1 == dx2 && dy1 == dy2 );
#endif
			break;
		}
		default:
		{
			IPointCollectionPtr ipThisCol( ipThis );
			IPointCollectionPtr ipTestCol( ipTest );
			if( ! ( ipThisCol && ipTestCol ) )
			{
				_ASSERT_EXPR( ipThisCol && ipTestCol, _T("CGeoemtry::EqualsVertex : IPointCollectionインターフェースを持たない形状は対象外です") ); // 面倒だし需要ないと思うので。Equals使ってください。
				return false;
			}
			long lThisCount = 0, lTestCount = 0;
			ipThisCol->get_PointCount( &lThisCount );
			ipTestCol->get_PointCount( &lTestCount );
			if( lThisCount != lTestCount )
				return false;

#ifdef _RELATIONALOPERATOR_EQUALS_BUG_FIXED
			for( long i = 0; i < lThisCount; ++i )
			{
				IPointPtr ipThisPoint, ipTestoint;
				ipThisCol->get_Point( i, &ipThisPoint );
				ipTestCol->get_Point( i, &ipTestoint );

				if( !_Equals( ipThisPoint, ipTestoint ) )
					return false;
			}
#else
			WKSPoint* pThisPoints = new WKSPoint[lThisCount];
			WKSPoint* pTestPoints = new WKSPoint[lTestCount];

			ipThisCol->QueryWKSPoints( 0, lThisCount, pThisPoints );
			ipTestCol->QueryWKSPoints( 0, lTestCount, pTestPoints );

			for( long i = 0; i < lThisCount; ++i )
			{
				if( pThisPoints[i].X != pTestPoints[i].X ||
					pThisPoints[i].Y != pTestPoints[i].Y )
				{
					delete [] pThisPoints;
					delete [] pTestPoints;

					return false;
				}
			}
			delete [] pThisPoints;
			delete [] pTestPoints;
#endif
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool CGeometry::Overlaps( IGeometry* ipGeom ) const
{
#if 0
	RELATIONSHIP_MACRO( Overlaps );
#else
	VARIANT_BOOL vb = VARIANT_FALSE;	// 判定値
	HRESULT hr = S_OK;
	IRelationalOperatorPtr ipRelOpe = (IGeometry*)(*this);
	ITopologicalOperator2Ptr ipTopo;	// KnownSimple 一時操作用
	bool bSecondChallenge = false, bSetBaseKnownSimple = false, bSetTargetKnownSimple = false;

RELATIONAL_OPERATION:
	try {
		LOGASSERTE_IF( NULL != ipRelOpe, sindyErr_ClassMemberMissing )
		{
			if( FAILED( hr = ipRelOpe->Overlaps( ipGeom, &vb ) ) )
			{
				ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Overlaps : 判定処理に失敗しました );
				goto ERROR_HANDLE;
			}
		}
	}
	catch(...) {
		ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Overlaps : 例外をキャッチしました );
		if( ! bSecondChallenge )
		{
			// KnownSimple が FALSE だと判定できない？？
			// なので、ここで一時的に TRUE にして再度判定
			VARIANT_BOOL vbKnownSimple = VARIANT_FALSE;
			ITopologicalOperator2Ptr ipTopo( ipRelOpe );
			ipTopo->get_IsKnownSimple( &vbKnownSimple );
			if( ! vbKnownSimple )
			{
				bSecondChallenge = true;
				ipTopo->put_IsKnownSimple( /*VARIANT_TRUE*/-1 );
				bSetBaseKnownSimple = true;
			}
			ipTopo = ipGeom;
			ipTopo->get_IsKnownSimple( &vbKnownSimple );
			if( ! vbKnownSimple )
			{
				bSecondChallenge = true;
				ipTopo->put_IsKnownSimple( /*VARIANT_TRUE*/-1 );
				bSetTargetKnownSimple = true;
			}
			if( bSetBaseKnownSimple || bSetTargetKnownSimple )
			{
				TRACEMESSAGE(_T("%s\n"), OVERLAPS_SECOND_CHALLENGE );
				goto RELATIONAL_OPERATION;
			}
			else
				goto ERROR_HANDLE;
		}
		else {
			ERRORLOG2(sindyErr_GeometryFunctionFailed, IRelationalOperator::Overlaps : SecondChallenge にも失敗しました );
			goto ERROR_HANDLE;
		}
	}
	goto RETURN_END;

ERROR_HANDLE:
	CGeometry::Trace( (IGeometryPtr)ipRelOpe, _T("CGeometry::Overlaps : 比較元ジオメトリ") );
	CGeometry::Trace( ipGeom, _T("CGeometry::Overlaps : 比較先ジオメトリ") );

	SASSERTE(hr);

RETURN_END:
	// KnownSimple を一時的にセットしたなら、ここで
	// 元の値に戻す
	if( bSetBaseKnownSimple )
	{
		ipTopo = ipRelOpe;
		ipTopo->put_IsKnownSimple( VARIANT_FALSE );
	}
	if( bSetTargetKnownSimple )
	{
		ipTopo = ipGeom;
		ipTopo->put_IsKnownSimple( VARIANT_FALSE );
	}

	return VB2bool(vb);
#endif // if 0
}

bool CGeometry::Touches( IGeometry* ipGeom ) const
{
	RELATIONSHIP_MACRO( Touches );
}

bool CGeometry::Within( IGeometry* ipGeom ) const
{
	//RELATIONSHIP_MACRO( Within );
	return AheGeometryContains( ipGeom, (IGeometry*)(*this) );
}

} // sindy

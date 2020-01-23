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
 * @file SegmentCollection.cpp
 * CGeometryクラスISegmentCollectionインターフェース実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "EnumSegment.h"
#include "util.h"
#include "sindycomutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR DELETE_DUPLI_SEGMENT = _T("%s : Delete segment that is a duplicate : INDEX[%d]\n");
#else
	LPCTSTR DELETE_DUPLI_SEGMENT = _T("%s : 重複セグメント削除 : INDEX[%d]\n");
#endif // SINDY_FOR_ENGLISH
} // namespace

namespace sindy {
using namespace errorcode;

// セグメント数を返す
long CGeometry::GetSegmentCount() const
{
	long lRet = -1;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		ISegmentCollectionPtr ipCol;
		LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_ISegmentCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERTE_IF( SUCCEEDED( ipCol->get_SegmentCount( &lRet ) ), sindyErr_GeometryFunctionFailed );
		}
	}
	return lRet;
}

// セグメントを取得する
_ISegmentPtr CGeometry::GetSegment( long index ) const
{
	_ISegmentPtr ipRet;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		ISegmentCollectionPtr ipCol;
		LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_ISegmentCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERTE_IF( SUCCEEDED( ipCol->get_Segment( index, &ipRet ) ), sindyErr_GeometryFunctionFailed );
		}
	}
	return ipRet;
}

sindyErrCode CGeometry::AddSegment( _ISegment* inSegment, long lInsertIndex/* = -1*/, bool bAddAfter/* = true*/ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTEERR_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		LOGASSERTEERR_IF( inSegment, sindyErr_ArgIsNull )
		{
			ISegmentCollectionPtr ipCol;
			LOGASSERTEERR_IF( SUCCEEDED( ipGeom->QueryInterface( IID_ISegmentCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
			{
				if( 0 > lInsertIndex )
				{
					LOGASSERTEERR_IF( SUCCEEDED( ipCol->AddSegment( inSegment ) ), sindyErr_GeometryFunctionFailed );
				}
				else {
					VARIANT va;
					VariantInit(&va);
					va.lVal = lInsertIndex;
					va.vt = VT_I4;
					LOGASSERTEERR_IF( SUCCEEDED( ipCol->AddSegment( inSegment, ( bAddAfter ) ? &vtMissing : &va, ( bAddAfter ) ? &va : &vtMissing ) ), sindyErr_GeometryFunctionFailed );
				}
			}
		}
	}
	return emErr;
}

CGeometry::const_segment_iterator CGeometry::segment_begin( IEnvelope* ipQuery/* = NULL*/ ) const
{
	CEnumSegment cSegs;

	ISegmentCollectionPtr ipSegCol( (IGeometryPtr)(IGeometry*)*this );
	if( ipSegCol )
	{
		if( ipQuery )
		{
			ISpatialIndexPtr ipSpatialIndex( ipSegCol );
			if( ipSpatialIndex )
			{
				// put_AllowIndexingをTRUEにしないと、get_IndexedEnumSegmentsで取得できないのでTRUEにしとく
				VARIANT_BOOL vbAllow = VARIANT_FALSE;
				ipSpatialIndex->get_AllowIndexing( &vbAllow );
				if ( ! vbAllow )
					ipSpatialIndex->put_AllowIndexing( VARIANT_TRUE );
				// ipQueryに関係するSegmentを取得する（パート番号やセグメント番号はそのままくる）
				ipSegCol->get_IndexedEnumSegments( ipQuery, cSegs.Release() );
				ipSpatialIndex->put_AllowIndexing( vbAllow );	// 元に戻しとく
			}
			else
				ipSegCol->get_EnumSegments( cSegs.Release() );
		} 
		else
			ipSegCol->get_EnumSegments( cSegs.Release() );

		cSegs.Reset();
		++cSegs;
	}
	return cSegs;
}

CGeometry::const_segment_iterator CGeometry::segment_end() const
{
	return CEnumSegment();
}

// 重複セグメントを削除する
sindyErrCode CGeometry::segment_unique()
{
	sindyErrCode emErr = sindyErr_NoErr;

	ISegmentCollectionPtr ipCol = (IGeometry*)*this;
	if( NULL != ipCol )	// 無くてもエラーにはしない
	{
		long lCount = 0;
		LOGASSERTEERR_IF( SUCCEEDED( ipCol->get_SegmentCount( &lCount ) ), sindyErr_GeometryFunctionFailed )
		{
			long lIndex = 0;
			while( lIndex < lCount && sindyErr_NoErr == emErr )
			{
				_ISegmentPtr ipSeg;
				LOGASSERTEERR_IF( SUCCEEDED( ipCol->get_Segment( lIndex, &ipSeg ) ), sindyErr_GeometryFunctionFailed )
				{
					CGeometry cSeg( ipSeg );
					long lIndex2 = lIndex + 1;
					while( lIndex < lCount && lIndex2 < lCount && sindyErr_NoErr == emErr )
					{
						_ISegmentPtr ipSeg2;
						LOGASSERTEERR_IF( SUCCEEDED( ipCol->get_Segment( lIndex2, &ipSeg2 ) ), sindyErr_GeometryFunctionFailed )
						{
							if( cSeg.Equals( ipSeg2, true ) ) // 向きは無視する
							{
								LOGASSERTEERR_IF( SUCCEEDED( ipCol->RemoveSegments( lIndex2, 1, VARIANT_FALSE ) ), sindyErr_GeometryFunctionFailed )
								{
									TRACEMESSAGE(DELETE_DUPLI_SEGMENT, __func__, lIndex2 );
									--lCount;
									continue;
								}
							}
						}
						++lIndex2;
					}
				}
				++lIndex;
			}
		}
	}
	return emErr;
}

} // sindy

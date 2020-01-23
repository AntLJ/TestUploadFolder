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
 * @file PointCollection.cpp
 * CGeometryクラスIPointCollectionインターフェース実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Geometry.h"
#include "util.h"
#include "sindycomutil.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// ジオメトリの総構成点数を取得する
long CGeometry::GetPointCount() const
{
	long lCount = -1;	// 返り値
	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_GeometryNotFound )
	{
		// ポイント以外はコレクションに落とす
		if( esriGeometryPoint != GetGeometryType() )
		{
			IPointCollectionPtr ipCol;
			LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
			{
				LOGASSERTE_IF( SUCCEEDED( ipCol->get_PointCount( &lCount ) ), sindyErr_GeometryFunctionFailed );
			}
		}
		// ポイントは常に1
		else lCount = 1;
	}

	return lCount;
}

// ジオメトリの構成点を取得する
CPoint CGeometry::GetPoint( long index ) const
{
	IPointPtr ipPoint;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;
	IPointCollectionPtr ipCol = ipGeom;

	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_GeometryNotFound )
	{
		switch( GetGeometryType() )
		{
			case esriGeometryPoint:
				LOGASSERTE_IF( 0 == index, sindyErr_ArgLimitOver )
				{
					// IPointCollection::get_Point()はコピーを作成するのでそれに動作をあわせる
					ipPoint = AheCreateClone( ipGeom );
				}
				break;
			case esriGeometryMultipoint:
			{
				// IPointCollection::get_Point()はMultipointの場合は参照を返すのでそれに動作をあわせる
				IPointPtr ipTmpPoint;
				LOGASSERTE_IF( NULL != ipCol, sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipCol->get_Point( index, &ipTmpPoint ) ), sindyErr_GeometryNotFound ); // 原因は違うかもしれない
				}
				ipPoint = AheCreateClone( ipTmpPoint );
				break;
			}
			default:
				LOGASSERTE_IF( NULL != ipCol, sindyErr_COMQueryInterfaceFailed )
				{
					LOGASSERTE_IF( SUCCEEDED( ipCol->get_Point( index, &ipPoint ) ), sindyErr_GeometryPointNotFound ); // 原因は違うかもしれない
				}
				break;
		}
	}

	return CPoint(ipPoint);
}

// ジオメトリの構成点を取得する
CPoint CGeometry::GetPoint( long part, long index ) const
{
	IPointPtr ipPoint;	// 返り値

	LOGASSERTE_IF( GetPartCount() > part, sindyErr_ArgLimitOver )
	{
		if( part == 0 )
			return GetPoint( index );
		else {
			CGeometry cGeom = GetPart( part );
			return cGeom.GetPoint( index );
		}
	}
	LOGASSERTE_IF( false, sindyErr_AlgorithmFailed );	// 途中で帰るのでここにきてはいけない
	return CPoint();
}

// 構成点のイテレータを取得する
IEnumVertexPtr CGeometry::GetEnumVertex() const
{
	IEnumVertexPtr ipRet;	// 返り値

	IGeometryPtr ipGeom = (IGeometryPtr)(IGeometry*)*this;

	LOGASSERTE_IF( (IGeometry*)ipGeom, sindyErr_ClassMemberMissing )
	{
		IPointCollectionPtr ipCol;
		LOGASSERTE_IF( SUCCEEDED( ipGeom->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
		{
			LOGASSERTE_IF( SUCCEEDED( ipCol->get_EnumVertices( &ipRet ) ), sindyErr_GeometryFunctionFailed );
		}
	}
	return ipRet;
}

sindyErrCode CGeometry::UpdatePoint( long i, IPoint* p )
{
	sindyErrCode emErr = sindyErr_NoErr; // 返り値

	point_iterator it = point_begin();
	if( sindyErr_NoErr == ( emErr = it.advance( i ) ) )
		return it.update( p );

	return emErr;
}

} // sindy

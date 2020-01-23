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
 * @file EnumVertex.cpp
 * CEnumVertexクラス実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "EnumVertex.h"
#include "util.h"
#include "sindycomutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

bool CEnumVertex::IsLastInPart() const
{
	VARIANT_BOOL vb = VARIANT_FALSE;	// 返り値
	INVOKEMETHOD( IsLastInPart( &vb ) );
	return VB2bool(vb);
}

IPointPtr CEnumVertex::Next( long* outPartIndex/* = NULL*/, long* vertexIndex/* = NULL*/ ) const
{
	IPointPtr ipPoint;	// 返り値
	long lPart = -1, lIndex = -1;
	INVOKEMETHOD( Next( &ipPoint, &lPart, &lIndex ) );
	if( outPartIndex ) *outPartIndex = lPart;
	if( vertexIndex  ) *vertexIndex  = lIndex;
	return ipPoint;
}

IPointPtr CEnumVertex::NextInPart( long* OutVertexIndex/* = NULL*/ ) const
{
	IPointPtr ipPoint;	// 返り値
	long lIndex = -1;
	INVOKEMETHOD( NextInPart( &ipPoint, &lIndex ) );
	if( OutVertexIndex ) *OutVertexIndex = lIndex;
	return ipPoint;
}

IPointPtr CEnumVertex::Previous( long* outPartIndex/* = NULL*/, long* vertexIndex/* = NULL*/ ) const
{
	IPointPtr ipPoint;	// 返り値
	long lPart = -1, lIndex = -1;
	INVOKEMETHOD( Previous( &ipPoint, &lPart, &lIndex ) );
	if( outPartIndex ) *outPartIndex = lPart;
	if( vertexIndex  ) *vertexIndex  = lIndex;
	return ipPoint;
}

IPointPtr CEnumVertex::QueryNext( IPoint* Vertex, long* outPartIndex/* = NULL*/, long* vertexIndex/* = NULL*/ ) const
{
	LOGASSERTE_IF( Vertex, sindyErr_ArgIsNull )
	{
		long lPart = -1, lIndex = -1;
		INVOKEMETHOD( QueryNext( Vertex, &lPart, &lIndex ) );
		if( outPartIndex ) *outPartIndex = lPart;
		if( vertexIndex  ) *vertexIndex  = lIndex;
	}
	return Vertex;
}

IPointPtr CEnumVertex::QueryNextInPart( IPoint* Vertex, long* OutVertexIndex/* = NULL*/ ) const
{
	LOGASSERTE_IF( Vertex, sindyErr_ArgIsNull )
	{
		long lIndex = -1;
		INVOKEMETHOD( QueryNextInPart( Vertex, &lIndex ) );
		if( OutVertexIndex ) *OutVertexIndex = lIndex;
	}
	return Vertex;
}

IPointPtr CEnumVertex::QueryPrevious( IPoint* Vertex, long* outPartIndex/* = NULL*/, long* vertexIndex/* = NULL*/ ) const
{
	LOGASSERTE_IF( Vertex, sindyErr_ArgIsNull )
	{
		long lPart = -1, lIndex = -1;
		INVOKEMETHOD( QueryPrevious( Vertex, &lPart, &lIndex ) );
		if( outPartIndex ) *outPartIndex = lPart;
		if( vertexIndex  ) *vertexIndex  = lIndex;
	}
	return Vertex;
}

errorcode::sindyErrCode CEnumVertex::Reset() const
{
	INVOKEMETHOD( Reset() );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

errorcode::sindyErrCode CEnumVertex::ResetToEnd() const
{
	INVOKEMETHOD( ResetToEnd() );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

errorcode::sindyErrCode CEnumVertex::SetAt( long iPart, long iVertex ) const
{
	INVOKEMETHOD( SetAt( iPart, iVertex ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

errorcode::sindyErrCode CEnumVertex::Skip( long numVertices ) const
{
	INVOKEMETHOD( Skip( numVertices ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

errorcode::sindyErrCode CEnumVertex::SetX( const double& X )
{
	INVOKEMETHOD( put_X( X ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

errorcode::sindyErrCode CEnumVertex::SetY( const double& Y )
{
	INVOKEMETHOD( put_Y( Y ) );
	return (SUCCEEDED(hr)) ? sindyErr_NoErr : sindyErr_COMFunctionFailed;
}

} // sindy


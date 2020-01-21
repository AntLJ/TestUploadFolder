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
 * @file EnumVertex.h
 * CEnumVertexクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#ifndef _ENUMVERTEX_H_
#define _ENUMVERTEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinLib/ComWrapper.h>
#include "ErrorCode.h"

namespace sindy {

#pragma warning(push)
#pragma warning(disable: 4275)
class SINDYLIB_API CEnumVertex : public CComWrapper<IEnumVertex>
{
public:
	explicit CEnumVertex(){}
	virtual ~CEnumVertex(){}
	explicit CEnumVertex( IEnumVertex* lp ){ *this = lp; }
	CEnumVertex& operator=( IEnumVertex* lp ){ AtlComPtrAssign((IUnknown**)&p, lp ); if( p ) Reset(); return *this; }
	operator IEnumVertex*() const{ return p; }
	
	bool IsLastInPart() const;
	IPointPtr Next( long* outPartIndex = NULL, long* vertexIndex = NULL ) const;
	IPointPtr NextInPart( long* OutVertexIndex = NULL ) const;
	IPointPtr Previous( long* outPartIndex = NULL, long* vertexIndex = NULL ) const;
	IPointPtr QueryNext( IPoint* Vertex, long* outPartIndex = NULL, long* vertexIndex = NULL ) const;
	IPointPtr QueryNextInPart( IPoint* Vertex, long* OutVertexIndex = NULL ) const;
	IPointPtr QueryPrevious( IPoint* Vertex, long* outPartIndex = NULL, long* vertexIndex = NULL ) const;
	errorcode::sindyErrCode Reset() const;
	errorcode::sindyErrCode ResetToEnd() const;
	errorcode::sindyErrCode SetAt( long iPart, long iVertex ) const;
	errorcode::sindyErrCode Skip( long numVertices ) const;
	errorcode::sindyErrCode SetX( const double& X );
	errorcode::sindyErrCode SetY( const double& Y );
};
#pragma warning(pop)
} // sindy

#endif // ifndef _ENUMVERTEX_H_

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

#include "stdafx.h"
#include "GlobalFunc.h"

#include <crd_cnv/coord_converter.h>

static crd_cnv g_cCrdCnv;

namespace gf
{
	// IPoint ¨ ƒƒbƒVƒ…‚w‚x
	bool ConvPoint2MXY( IPointPtr ipPoint, CMeshXY* pMXY )
	{
		if( ipPoint )
		{
			WKSPoint point;
			ipPoint->QueryCoords( &point.X, &point.Y );
			return 0 == g_cCrdCnv.LLtoMesh( point.X, point.Y, 64, &pMXY->m_nMesh, &pMXY->m_nX, &pMXY->m_nY );
		}
		else
			return false;		
	}

	// ƒƒbƒVƒ…XY ¨ IPoint
	IPointPtr CreateMXYPoint( const CMeshXY& cMXY )
	{
		WKSPoint point;
		if( g_cCrdCnv.MeshtoLL(cMXY.m_nMesh, cMXY.m_nX, cMXY.m_nY, &point.X, &point.Y) ==  0 )
		{
			IPointPtr ipPoint( CLSID_Point );
			ipPoint->PutCoords( point.X, point.Y );
			return ipPoint;
		}
		else
			return NULL;
	}
}

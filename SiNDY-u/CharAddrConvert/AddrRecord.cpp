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

#include "StdAfx.h"
#include "AddrRecord.h"

// 位置情報セット
void CMeshXY::SetPos( IPointPtr ipPoint )
{
	static crd_cnv g_cCrdCnv;
	if( ipPoint )
	{
		d_lonlat p;
		ipPoint->QueryCoords( &p.lon, &p.lat );
		g_cCrdCnv.LLtoMesh( p.lon, p.lat, 2, &m_nMeshCode, &m_nX, &m_nY );
	}
}

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

// GeomProc.h: CGeomProc クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "stdafx.h"

class CGeomProc
{
public:
	CGeomProc();
	virtual ~CGeomProc();

public:
	bool getGeom2PointVec( const IGeometryPtr ipGeometry, std::vector< WKSPoint >& rPointVec );
	bool getPoint( const IGeometryPtr ipGeometry, WKSPoint& rWKSPoint );

	bool judgePointSame( WKSPoint cWKSPoint1, WKSPoint cWKSPoint2 );
	bool judgePointVecSame( std::vector<WKSPoint>& rWKSPointVec1, std::vector<WKSPoint>& rWKSPointVec2 );
};

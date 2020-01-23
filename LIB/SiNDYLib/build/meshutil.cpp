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
 * @file meshutil.cpp
 * @brief メッシュ操作用外部関数実装ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "meshutil.h"
#include "AheMeshUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 実体はArcHelperExのAheMeshUtil.[h|cpp]に移動
void MESH2LL( unsigned int lMeshCode, int x, int y, double& lon, double& lat ){ AheMESH2LL( lMeshCode, x, y, lon, lat ); }
void MESH2ENV( unsigned int lMeshCode, double& dMinLon, double& dMinLat, double& dMaxLon, double& dMaxLat ){ AheMESH2ENV( lMeshCode, dMinLon, dMinLat, dMaxLon, dMaxLat ); }
IEnvelopePtr MESH2ENV( unsigned int lMeshCode, ISpatialReference* ipSpRef ){ return AheMESH2ENV( lMeshCode, ipSpRef ); }
IGeometryPtr MESH2POLY( unsigned int lMeshCode, ISpatialReference* ipSpRef ){ return AheMESH2POLY( lMeshCode, ipSpRef ); }
unsigned int LL2MESH( const double& dLon, const double& dLat, int level, int& x, int& y, int no_mill/* = 0*/ ){ return AheLL2MESH( dLon, dLat, level, x, y, no_mill ); }
unsigned int LL2MESH1st( const double& dLon, const double& dLat, int& x, int& y, int no_mill/* = 0*/ ){ return AheLL2MESH1st( dLon, dLat, x, y, no_mill ); }
unsigned int LL2MESH2nd( const double& dLon, const double& dLat, int& x, int& y, int no_mill/* = 0*/ ){ return AheLL2MESH2nd( dLon, dLat, x, y, no_mill ); }
unsigned int LL2MESHcity( const double& dLon, const double& dLat, int& x, int& y, int no_mill/* = 0*/ ){ return AheLL2MESHcity( dLon, dLat, x, y, no_mill ); }
unsigned int Point2MESH( IPoint* ipPoint, int level, int& x, int& y, int no_mill/* = 0*/ ){ return AhePoint2MESH( ipPoint, level, x, y, no_mill ); }
unsigned int INCMESH_X( unsigned int code ){ return AheINCMESH_X(code); }
unsigned int INCMESH_Y( unsigned int code ){ return AheINCMESH_Y(code); }
unsigned int DECMESH_X( unsigned int code ){ return AheDECMESH_X(code); }
unsigned int DECMESH_Y( unsigned int code ){ return AheDECMESH_Y(code); }
unsigned int MESH2MESH1ST( unsigned int code ){ return AheMESH2MESH1ST( code ); }

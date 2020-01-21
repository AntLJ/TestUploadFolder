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
#include <ArcHelperEx/AheMeshUtil.h>
#include <crd_cnv/crd_cnv.h>
#include <TDC/useful_headers/registry_util.h>
#include <boost/logic/tribool.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

crd_cnv g_cnv; //!< メッシュコードコンバータ

namespace ahe_reg_key{
	const CString WorldMeshMode = _T("WorldMeshMode");
}

// 国際都市地図モードかどうか
bool AheIsWorldMeshMode()
{
	static boost::tribool bWorldMeshMode = boost::indeterminate;

	// レジストリは1度だけ読みに行く
	if( boost::indeterminate( bWorldMeshMode ) )
	{
		bWorldMeshMode = (bool)uh::reg_util::read( ahe_reg_key::WorldMeshMode, REGPATH_SINDY_E_COMMON, false );
	}
	return bWorldMeshMode;
}

// メッシュコード＋正規化XYから緯度経度に変換する
void AheMESH2LL( unsigned int lMeshCode, int x, int y, double& lon, double& lat )
{
	if( AheIsWorldMeshMode() )
	{
		int level = 1;
		if( lMeshCode > 999999 )
			level = 2;
		if( lMeshCode > 99999999 )
			level = 64;
		g_cnv.MeshtoLL_World( lMeshCode, x, y, level, crd_cnv::SJ_ORIGIN, &lon, &lat );
	}
	else
		g_cnv.MeshtoLL( lMeshCode, x, y, &lon, &lat );
}

// メッシュコードから緯度経度に変換する
void AheMESH2ENV( unsigned int lMeshCode, double& dMinLon, double& dMinLat, double& dMaxLon, double& dMaxLat )
{
	AheMESH2LL( lMeshCode,       0,       0, dMinLon, dMinLat );
	AheMESH2LL( lMeshCode, 1000000, 1000000, dMaxLon, dMaxLat );
}


// 緯度経度からメッシュコード＋正規化XYに変換する
unsigned int AheLL2MESH( const double& dLon, const double& dLat, int level, int& x, int& y, int no_mill/* = 0*/ )
{
	unsigned int nMeshCode = 0;

	if( AheIsWorldMeshMode() )
		g_cnv.LLtoMesh_World( dLon, dLat, level, crd_cnv::SJ_ORIGIN, &nMeshCode, &x, &y, no_mill );
	else
	{
		int nTmpMeshCode = -1;
		g_cnv.LLtoMesh( dLon, dLat, level, &nTmpMeshCode, &x, &y, no_mill );
		nMeshCode = nTmpMeshCode;
	}

	return nMeshCode;
}

// 緯度経度から一次メッシュコード＋正規化XYに変換する
unsigned int AheLL2MESH1st( const double& dLon, const double& dLat, int& x, int& y, int no_mill/* = 0*/ )
{
	return AheLL2MESH( dLon, dLat, 1, x, y, no_mill );
}

// 緯度経度から二次メッシュコード＋正規化XYに変換する
unsigned int AheLL2MESH2nd( const double& dLon, const double& dLat, int& x, int& y, int no_mill/* = 0*/ )
{
	return AheLL2MESH( dLon, dLat, 2, x, y, no_mill );
}

// 緯度経度から都市地図メッシュコード＋正規化XYに変換する
unsigned int AheLL2MESHcity( const double& dLon, const double& dLat, int& x, int& y, int no_mill/* = 0*/ )
{
	return AheLL2MESH( dLon, dLat, 64, x, y, no_mill );
}

// 渡されたメッシュコードから（x,y）離れたメッシュコードを求める
unsigned int AheGETMESHCODE( unsigned int code, int xdist, int ydist )
{
	if( AheIsWorldMeshMode() )
		return g_cnv.GetMeshCode_SJ( code, xdist, ydist );
	else
		return g_cnv.GetMeshCode( code, xdist, ydist );
}

unsigned int AheINCMESH_X( unsigned int code ){ return AheGETMESHCODE( code,  1,  0 ); }
unsigned int AheINCMESH_Y( unsigned int code ){ return AheGETMESHCODE( code,  0,  1 ); }
unsigned int AheDECMESH_X( unsigned int code ){ return AheGETMESHCODE( code, -1,  0 ); }
unsigned int AheDECMESH_Y( unsigned int code ){ return AheGETMESHCODE( code,  0, -1 ); }

// 2次メッシュ、都市地図メッシュから1次メッシュコードを返す
unsigned int AheMESH2MESH1ST( unsigned int code )
{
	unsigned int ret = -1;

	if( AheIsWorldMeshMode() )
	{
		switch( g_cnv.GetMeshDigit( code ) )
		{
			case 6: ret = code; break;
			case 8: ret = (unsigned int)(code/100); break;
			case 10: ret = (unsigned int)(code/10000); break;
			default: _ASSERTE(false);	// おかしい
		}
	}
	else
	{
		switch( g_cnv.GetMeshDigit( code ) )
		{
			case 4: ret = code; break;
			case 6: ret = (int)(code/100); break;
			case 8: ret = (int)(code/10000); break;
			default: _ASSERTE(false);	// おかしい
		}
	}
	return ret;
}

// 2点間の距離（m）を求める
double AheGETDIST( const double& lon1, const double& lat1, const double& lon2, const double& lat2 )
{
	if( AheIsWorldMeshMode() )
		return g_cnv.GetDist_JGD2000( lon1, lat1, lon2, lat2 );
	else	
		return g_cnv.GetDist( lon1, lat1, lon2, lat2 );
}

// 2地点の経緯度から、2点間の距離 + 距離(X,Y)mを求める
double AheGETDISTXY( const double& lon1, const double& lat1, const double& lon2, const double& lat2, double& distX, double& distY)
{
	if( AheIsWorldMeshMode() )
		return g_cnv.GetDistXY_JGD2000(lon1, lat1, lon2, lat2, &distX, &distY);
	else
		return g_cnv.GetDistXY(lon1, lat1, lon2, lat2, &distX, &distY);
}

// 経緯度とそこから経度方向にX(m),緯度方向にY(m)離れたところの経緯度を求める
int AheGETLLPT( const double& lon1, const double& lat1, const double& distX, const double& distY, double& lon2, double& lat2)
{
	if( AheIsWorldMeshMode() )
		return g_cnv.GetLLPt_JGD2000(lon1, lat1, distX, distY, &lon2, &lat2);
	else
		return g_cnv.GetLLPt(lon1, lat1, distX, distY, &lon2, &lat2);
}

// 2点間（メッシュXY）の差分を求める
int AheGETDIFF( unsigned int mesh1, int x1, int y1, unsigned int mesh2, int x2, int y2, double& dx, double& dy )
{
	// TODO:crd_cnv::GetNrmDiffが国際都市地図メッシュには対応していない
	return g_cnv.GetNrmDiff( mesh1, x1, y1, mesh2, x2, y2, &dx, &dy );
}

// 経緯度から最も近い１９座標系の系とＸＹを返す
void AheLL2XY19( double lon, double lat, coord19 *sys_xy )
{
	g_cnv.LLtoXY19( lon, lat, sys_xy );
}
// １９座標系のＸＹから経緯度へ変換
void AheXY192LL( int axis, double x, double y, d_lonlat *lonlat )
{
	g_cnv.XY19toLL( axis, x, y, lonlat );
}

// ゾーンを求める
int AheLL2UTM( double lon, double lat, double *dX, double *dY )
{
	int nZone = g_cnv.GetUTMZone( lon, lat, dX, dY );
	return nZone;
}

// 緯度経度系からUTM（世界測地系）へ変換
void AheLLtoUTM_WGS84( int nZone,  double lon, double lat, double *dX, double *dY )
{
	g_cnv.LLtoUTM_WGS84( nZone, lon, lat, dX, dY );
}

// UTM座標から緯度経度系（世界測地系）へ変換
void AheUTM2LL_WGS84( int nZone, double x, double y, double *dX, double *dY )
{
	g_cnv.UTMtoLL_WGS84( nZone, x, y, dX, dY );
}

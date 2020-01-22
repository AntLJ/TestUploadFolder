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

// MeshPos.cpp: CMeshPos クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshPos.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CMeshPos::~CMeshPos()
{
}
//---------------------------------------------------------------------
//	２次メッシュコード（Ｘ:Ｙ）から、指定距離にあるポイントの経緯度を取得
//---------------------------------------------------------------------
bool CMeshPos::SetLeftDownPos(int cMeshcode,int cX, int cY,int cDist)
{
	int	aMeshcode = cMeshcode;
	int	aMoveX	= cX - cDist;						//最南西ポイントを取得
	int	aMoveY	= cY - cDist;
	double aLon=.0, aLat =.0;

	eX = aMoveX;
	eY = aMoveY;
	MeshtoLL( cMeshcode, aMoveX, aMoveY, &aLon, &aLat);

	//移動座標取得
	if(( aMoveX < 0 )&&( aMoveY < 0 )){		
		eMesh = GetNextMeshCode(cMeshcode,-1,-1);	//隣接メッシュコード＋座標取得
		eX    = MILLION - abs(aMoveX);
		eY    = MILLION - abs(aMoveY);
		MeshtoLL( eMesh, eX, eY, &aLon, &aLat);
	}
	else if( aMoveX < 0 ){		
		eMesh = GetNextMeshCode(cMeshcode,-1,0);	//隣接メッシュコード＋座標取得
		eX    = MILLION - abs(aMoveX);
		MeshtoLL( eMesh, eX, aMoveY, &aLon, &aLat);
	}
	else if( aMoveY < 0 ){		
		eMesh = GetNextMeshCode(cMeshcode,0,-1);	//隣接メッシュコード＋座標取得
		eY    = MILLION - abs(aMoveY);
		MeshtoLL( eMesh, aMoveX, eY, &aLon, &aLat);
	}
	return true;
}
//---------------------------------------------------------------------
//	２次メッシュコード（Ｘ:Ｙ）から、指定距離にあるポイントの経緯度を取得
//---------------------------------------------------------------------
bool CMeshPos::SetRightTopPos(int cMeshcode, int cX, int cY,int cDist)
{
	int	aMeshcode = cMeshcode;
	int	aMoveX = cX + cDist;						//最北東ポイントを取得
	int	aMoveY = cY + cDist;
	double aLon=.0, aLat =.0;

	eX = aMoveX;
	eY = aMoveY;
	MeshtoLL( cMeshcode, aMoveX, aMoveY, &aLon, &aLat);

	//移動座標取得
	if(( aMoveX > MILLION )&&( aMoveY > MILLION )){		
		eMesh = GetNextMeshCode(cMeshcode,1,1);		//隣接メッシュコード＋座標取得
		eX    = aMoveX-MILLION;
		eY    = aMoveY-MILLION;
		MeshtoLL( eMesh, eX, eY, &aLon, &aLat);
	}
	else if( aMoveX > MILLION ){
		eMesh = GetNextMeshCode(cMeshcode,1,0);		//隣接メッシュコード＋座標取得
		eX    = aMoveX-MILLION;
		MeshtoLL( eMesh, eX, aMoveY, &aLon, &aLat);
	}
	else if( aMoveY > MILLION ){
		eMesh = GetNextMeshCode(cMeshcode,0,1);		//隣接メッシュコード＋座標取得
		eY    = aMoveY-MILLION;
		MeshtoLL( eMesh, aMoveX, eY, &aLon, &aLat);
	}
	return true;
}
//---------------------------------------------------------------------
//	メッシュ + (X,Y)から緯度経度へ変換
//	@retval	0	成功
//	@retval -1	失敗
//---------------------------------------------------------------------
bool CMeshPos::MeshtoLL( int		cMeshcode,	// ２次メッシュコード
					     int		cX,			// 正規化座標X
					     int		cY,			// 正規化座標Y
					     double		*cLon,		// 経度
					     double		*cLat)		// 緯度
{
	int		atai[6];
	double	wid1	= 1.0,	hei1	= 2.0 / 3.0;
	double	wid2	= 0.0;
	double	wid64	= 0.0;
	double	hei2	= 0.0;
	double	hei64	= 0.0;

	wid2	= 7.0 / 60.0 + 30.0 / 3600.0; 
	hei2	= 5.0 / 60.0;
	wid64	= wid2 / 8.0;
	hei64	= hei2 / 8.0;

	for(int i=0; i<6; i++){
		atai[i]	= 0;
	}
	// ２次メッシュコード
	for(int i=0; i<2; i++){
		atai[i]	   = cMeshcode % 10;
		cMeshcode /= 10;
	}
	atai[2]	= cMeshcode % 100;
	atai[3]	= cMeshcode / 100;
	*cLat	= atai[3] / 1.5   + hei2 * atai[1] + cY * hei2 / D_MILLION;
	*cLon	= atai[2] + 100.0 + wid2 * atai[0] + cX * wid2 / D_MILLION;

	setLon(*cLon);
	setLat(*cLat);

	return true;
}
//---------------------------------------------------------------------
//	緯度経度から、メッシュコード + (x,y) を求める
//	@retval	0	成功
//	@retval -1	失敗
//---------------------------------------------------------------------
int	CMeshPos::LLtoMesh	(	double		lon,		// 緯度
							double		lat,		// 経度
							int			level,		// メッシュのレベル (1,2,64)
							int			*meshcode,	// メッシュコード
							int			*x,			// 正規化Ｘ
							int			*y)			// 正規化Ｙ
{
	double	wid1		= 1.0,	hei1		= 2.0 / 3.0;
	double	wid2		= 0.0,	hei2		= 0.0;
	double	wid64		= 0.0,	hei64		= 0.0;
	double	lat_amari1	= 0.0,	lon_amari1	= 0.0;
	double	lat_amari2	= 0.0,	lon_amari2	= 0.0;
	double	lat_amari3	= 0.0,	lon_amari3	= 0.0;
	double	x_step_1	= 0.0,	y_step_1	= 0.0;
	double	x_step_2	= 0.0,	y_step_2	= 0.0;
	double	x_step_3	= 0.0,	y_step_3	= 0.0;
	int		up_1st		= 0,	low_1st		= 0;
	int		up_2nd		= 0,	low_2nd		= 0;
	int		up_64		= 0,	low_64		= 0;

	x_step_1	= wid1 / D_MILLION;
	y_step_1	= hei1 / D_MILLION;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	x_step_2	= wid2 / D_MILLION;
	y_step_2	= hei2 / D_MILLION;

	wid64		= wid2 / 8.0;
	hei64		= hei2 / 8.0;

	x_step_3	= wid64 / D_MILLION;
	y_step_3	= hei64 / D_MILLION;
	
	up_1st		= (int)(lat * 1.5);
	low_1st		= (int)(lon - 100.0);

	lat_amari1	= lat - (up_1st / 1.5);
	lon_amari1	= lon - (100.0 + low_1st);

	switch(level)
	{
		case	1:
			*meshcode	= up_1st * 100 + low_1st;
			*x			= (int)(lon_amari1 / x_step_1 + 0.5);
			*y			= (int)(lat_amari1 / y_step_1 + 0.5);
			return(0);
		case	2:
			up_2nd		= (int)(lat_amari1 / hei2);
			low_2nd		= (int)(lon_amari1 / wid2);
			lat_amari2	= lat_amari1 - up_2nd * hei2;
			lon_amari2	= lon_amari1 - low_2nd * wid2;
			*meshcode	= up_1st * 10000 + low_1st * 100 +
							up_2nd * 10 + low_2nd;
			*x			= (int)(lon_amari2 / x_step_2 + 0.5);
			*y			= (int)(lat_amari2 / y_step_2 + 0.5);
			return(0);
		case	64:
			up_2nd		= (int)(lat_amari1 / hei2);
			low_2nd		= (int)(lon_amari1 / wid2);
			lat_amari2	= lat_amari1 - up_2nd * hei2;
			lon_amari2	= lon_amari1 - low_2nd * wid2;
			up_64		= (int)(lat_amari2 / hei64);
			low_64		= (int)(lon_amari2 / wid64);
			lat_amari3	= lat_amari2 - up_64 * hei64;
			lon_amari3	= lon_amari2 - low_64 * wid64;
			*meshcode	= up_1st * MILLION + low_1st * 10000 +
							up_2nd * 1000 + low_2nd * 100 +
							up_64 * 10 + low_64;
			*x			= (int)(lon_amari3 / x_step_3 + 0.5);
			*y			= (int)(lat_amari3 / y_step_3 + 0.5);
			return(0);
		default:
			break;
	}
	return(-1);
}
//---------------------------------------------------------------------
//	1/256秒単位の緯度経度から、通常の緯度経度へ変換
//---------------------------------------------------------------------
void	CMeshPos::LL256toLL	(	int		lon256,		// 経度
								int		lat256,		// 緯度
								double	*lon,		// 経度
								double	*lat)		// 緯度
{
	*lat	= (lat256 / 3600.0 / 256.0);
	*lon	= (lon256 / 3600.0 / 256.0);
}
//---------------------------------------------------------------------
// 経緯度
//---------------------------------------------------------------------
CMeshPos::CMeshPos(double cLon, double cLat, int cMeshCode )
{
	eMesh = cMeshCode;
	mUnitSize  = 1000000;
	CMapDPoint	aCorner = GetMeshCorner( cMeshCode );

	if( ( cMeshCode / 10000000 ) != 0 ){		// CityMap
		eX = (int)((cLon -aCorner.eX) / ((7.5/60.0)/8.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / ((5.0/60.0)/8.0) * (double)getUnitSize());
	}
	else if( ( cMeshCode / 100000 ) != 0 ){		// Base
		eX = (int)((cLon -aCorner.eX) / (7.5/60.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / (5.0/60.0) * (double)getUnitSize());
	}
	else if( ( cMeshCode / 1000 ) != 0 ){		// MIDDLE
		eX = (int)((cLon -aCorner.eX) / (120.0/60.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / (80.0/60.0) * (double)getUnitSize());
	}
	else {	// Top
		eX = (int)((cLon -aCorner.eX) / (1920.0/60.0) * (double)getUnitSize());
		eY = (int)((cLat -aCorner.eY) / (1280.0/60.0) * (double)getUnitSize());
	}
}
//---------------------------------------------------------------------
//	２次メッシュ区画の南西の経緯度を求める
//---------------------------------------------------------------------
CMapDPoint	CMeshPos::GetMeshCorner( int cMeshcode )
{
	CMapDPoint	aCorner;

	int	aBaseMesh = cMeshcode / 100;		// Base
	int aCity = cMeshcode % 100;			// City RowCol
	int	aWork1,aWork2,aWork3,aWork4;

	if( ( cMeshcode / 10000000 ) != 0 ){	// CityMap
		int	aBaseMesh = cMeshcode / 100;	// Base
		int aCity = cMeshcode % 100;		// City RowCol
		aWork1 = aBaseMesh / 10000;
		aWork2 = (aBaseMesh / 100) % 100;
		aWork3 = (aBaseMesh / 10) % 10;
		aWork4 = aBaseMesh % 10;
		aCorner.eX = (double) (aWork2+100) + (double) aWork4 / 8.0;
		aCorner.eY = (double) (8*aWork1+aWork3) / 12.0;
		aCorner.eX += (double) (((7.5/60.0)/8.0)*(aCity%10));
		aCorner.eY += (double) (((5/60.0)/8.0)*(aCity/10));

	}
	else if( ( cMeshcode / 100000 ) != 0 ){	// Base
		aWork1 = cMeshcode / 10000;
		aWork2 = (cMeshcode / 100) % 100;
		aWork3 = (cMeshcode / 10) % 10;
		aWork4 = cMeshcode % 10;
		aCorner.eX = (double) (aWork2+100) + (double) aWork4 / 8.0;
		aCorner.eY = (double) (8*aWork1+aWork3) / 12.0;
	}
	else if( ( cMeshcode / 1000 ) != 0 ){	// MIDDLE
		aWork1 = cMeshcode / 100;
		aWork2 = cMeshcode % 100;
		aCorner.eX = (double) (aWork2+100) ;
		aCorner.eY = (double) (8*aWork1) / 12.0;
	}
	else{									// Top
		switch (cMeshcode){
			case 0:							// 00
				aCorner.eX = 104.0;
				aCorner.eY = 14.0;
				break;
			case 1:							// 01
				aCorner.eX = 136.0;
				aCorner.eY = 14.0;
				break;
			case 10:						// 10
				aCorner.eX = 104.0;
				aCorner.eY = 35.333333333333;
				break;
			case 11:						// 11
				aCorner.eX = 136.0;
				aCorner.eY = 35.333333333333;
				break;
		}
	}
	return aCorner;
}
//---------------------------------------------------------------------
//	原点メッシュから(x,y)離れた所のメッシュコードを取得
//	@return		成功：メッシュコード
//	@return		失敗： -1
// 		+-----+-----+-----+
// 		|-1,0 | 0,1 | 1,1 |
// 		+-----+-----+-----+
// 		|-1,0 | 0,0 | 1,0 |
// 		+-----+-----+-----+
// 		|-1,-1| 0,-1| 1,-1|
// 		+-----+-----+-----+ ..(X,Y)の入力引数仕様
//---------------------------------------------------------------------
int CMeshPos::GetNextMeshCode(int meshcode,	//基準メッシュコード
							  int xdist,	// 原点メッシュからX方向へ何メッシュ離れているか
							  int ydist)	// 原点メッシュからY方向へ何メッシュ離れているか
{
	#define		MILLION			1000000
	int	meshunit	= 0;
	int	roop_count	= 0;
	int	num1[6]		= {0,0,0,0,0,0};
	int	dist[2]		= {0,0};
	int	retmesh		= 0;
	int	tmp1		= 0;
	int	tmp2		= 0;

	dist[0]			= xdist;
	dist[1]			= ydist;
	int mc			= meshcode;
	int	digit		= 6;	// メッシュコードの桁数

	switch( digit ){
		case 4:	meshunit=1; roop_count=0; break;	// １次メッシュ
		case 6: meshunit=2;	roop_count=2; break;	// ２次メッシュ
		case 8: meshunit=64;roop_count=4; break;	// ８桁メッシュ
		default: return(-1);
	}
	// 桁をばらばらにする(59 41 4 1 7 7 ←分解)
	for(int i = 0; i < roop_count; i++){
		num1[i]	= mc % 10;
		mc /= 10;
	}
	num1[roop_count]	= mc % 100;
	num1[roop_count+1]	= mc / 100;
	int	j = 0;
	if(meshunit == 64) {			// j = 0 ... 左右方向に離れたメッシュコード					
		for(j = 0; j < 2; j++) {	// j = 1 ... 上下方向に離れたメッシュコード
			num1[j]	+= dist[j];
			if(num1[j] > 7) {
				num1[j+2] += num1[j] / 8;			// 桁の操り上げ
				num1[j]	  %= 8;
				if(num1[j+2] > 7) {
					num1[j+4] += num1[j+2] / 8;		// 桁の操り上げ
					num1[j+2] %= 8;
				}
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);		// 桁の繰下げ
				num1[j]		=  7 - (-num1[j] % 8);
				if(num1[j+2] < 0) {
					num1[j+2]-= 7;
					num1[j+4]-= (-num1[j+2] / 8);	// 桁の繰下げ
					num1[j+2]= 7 - (-num1[j+2] % 8);
				}
			}
		}
		retmesh	= num1[5] * MILLION	+ num1[4] * 10000 + 
				  num1[3] * 1000+ num1[2] * 100 +
			      num1[1] * 10	+ num1[0];
	} else	
	// ２次メッシュ用
	if(meshunit == 2) {				// j = 0 ... 左右方向に離れたメッシュコード,		
		for(j = 0; j < 2; j++) {	// j = 1 ... 上下方向に離れたメッシュコード	
			num1[j] += dist[j];
			if(num1[j] > 7) {
				num1[j+2]	+= num1[j] / 8;		// 桁の繰上げ
				num1[j]		%= 8;
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);	// 桁の繰下げ
				num1[j]		=  7 - (-num1[j] % 8);
			}
		}
		retmesh	= num1[3] * 10000 + num1[2] * 100 +
					num1[1] * 10  + num1[0];
	} else
	if(meshunit == 1) { // １次メッシュの場合
		// 左右に離れたメッシュコード
		num1[0] += dist[0];
		num1[1] += dist[1];
		retmesh	= num1[1] * 100 + num1[0];

	} else {
		return(-1);
	}
	return(retmesh);
}
//---------------------------------------------------------------------
//	２地点の緯度経度から、２点間の距離を求める(単位: m)
//	@return		２地点間の距離
//---------------------------------------------------------------------
double	CMeshPos::GetDist(double	eLon1,		// 経度１
						  double	eLat1,		// 緯度１
						  double	eLon2,		// 経度２
						  double	eLat2)		// 緯度２
{
	double aFaiRad		= (eLat2 + eLat1) * V_PI / 180.0 / 2.0;
	double aDeltaX		= (eLon2 - eLon1) * V_PI / 180.0;
	double aDeltaY		= (eLat2 - eLat1) * V_PI / 180.0;
	double aSinVal		= sin(aFaiRad);
	double aCosVal		= cos(aFaiRad);

	double D	= 1.0 - EA2 * aSinVal * aSinVal;
	double M	= 6334834.0 / sqrt(D * D * D);
	double N	= 6377397.0 / sqrt(D);

	double aDistY	= M * aDeltaY;
	double aDistX	= N * aCosVal * aDeltaX;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}
//---------------------------------------------------------------------
//	２地点の緯度経度から、２点間の距離 + 距離(X,Y)mを求める
//	@return		２地点間の距離
//---------------------------------------------------------------------
double	CMeshPos::GetDistXY	(	double		eLon1,		// 経度１
								double		eLat1,		// 緯度１
								double		eLon2,		// 経度２
								double		eLat2,		// 緯度２
								double		*cDist_X,	// 距離X(m)
								double		*cDist_Y)	// 距離Y(m)
{
	double aFaiRad		= (eLat2 + eLat1) * V_PI / 180.0 / 2.0;
	double aDeltaX		= (eLon2 - eLon1) * V_PI / 180.0;
	double aDeltaY		= (eLat2 - eLat1) * V_PI / 180.0;

	double aSinVal		= sin(aFaiRad);
	double aCosVal		= cos(aFaiRad);

	double D			= 1.0 - EA2 * aSinVal * aSinVal;
	double M			= 6334834.0 / sqrt(D * D * D);
	double N			= 6377397.0 / sqrt(D);

	double aDistY		= M * aDeltaY;
	double aDistX		= N * aCosVal * aDeltaX;

	*cDist_X	= aDistX;
	*cDist_Y	= aDistY;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}
//---------------------------------------------------------------------
// 	緯度経度とそこから経度方向にXm,緯度方向にYm離れたところの緯度経度を取得
//---------------------------------------------------------------------
void CMeshPos::GetLLPnt(double		eLon1,		// 経度１
						double		eLat1,		// 緯度１
						double		cDist_X,	// 距離Ｘ(m)
						double		cDist_Y,	// 距離Ｙ(m)
						double		*eLon2,		// 経度２
						double		*eLat2)		// 緯度２
{
	double  cur_lon = 0.0;
	double  cur_lat = 0.0;
	double	min_lon	= 0.0;
	double	min_lat	= 0.0;
	double	max_lon	= 0.0;
	double	max_lat	= 0.0;
	double	d_x1	= 0.0;
	double	d_y1	= 0.0;
	double	d_x2	= 0.0;
	double	d_y2	= 0.0;

	if(cDist_X < 0){
		min_lon	= -180.0;
		max_lon	= eLon1;
	}
	else{
		min_lon	= eLon1;
		max_lon	= 180.0;
	}
	if(cDist_Y < 0){
		min_lat	= -90.0;
		max_lat	= eLat1;
	}
	else{
		min_lat	= eLat1;
		max_lat	= 90;
	}
	double dist	= sqrt(cDist_X * cDist_X + cDist_Y * cDist_Y);
	// X方向
	while(1){
		cur_lon	= (max_lon + min_lon) / 2.0;
		cur_lat	= (max_lat + min_lat) / 2.0;
		double t_dist	= GetDistXY(eLon1, eLat1, cur_lon, cur_lat, &d_x1, &d_y1);
		if(fabs(t_dist - dist) > 0.01){
			if(d_x1 - cDist_X > 0)
				max_lon	= (max_lon + min_lon) / 2.0;
			else
				min_lon	= (max_lon + min_lon) / 2.0;
			if(d_y1 - cDist_Y > 0)
				max_lat	= (max_lat + min_lat) / 2.0;
			else
				min_lat	= (max_lat + min_lat) / 2.0;
		}
		else{
			break;
		}
	}
	*eLon2	= cur_lon;
	*eLat2	= cur_lat;
}
//------------------------------[ end of file ]------------------------------

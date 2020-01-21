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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//------------------------------------------------------------
/*
	１０進から度、分、秒へ変換
*/
//------------------------------------------------------------
void	crd_cnv::
DECtoDMS	(	double		lonlat,			// 経緯度（１０進）
				dms			*mes_pt)		// 変換後の測点の経緯度（度、分、秒）
{
	int			deg,	min;
	double		sec;
	double		tmp[3];

	deg			= lonlat;
	tmp[0]		= (lonlat - deg) * 60.0;
	min			= tmp[0];
/*	if(tmp[0]	>= 60.0)
	{
		// 桁上げ
		min		-= 60.0;
		deg++;
	}
*/	tmp[1]		= (tmp[0] - min) * 60.0;
	sec			= tmp[1];
/*	if(tmp[1]	>= 60.0)
	{
		// 桁上げ
		sec		-= 60.0;
		min++;
	}
*/	mes_pt->deg	= deg;
	mes_pt->min	= min;
	mes_pt->sec	= sec;
}

//------------------------------------------------------------
/*
	１０進から度、分、秒へ変換
	@return	構造体 dms へのポインタ
*/
//------------------------------------------------------------
dms*	crd_cnv::
DECtoDMS	(	double	lonlat	)	// 経緯度（１０進）
{
	static	dms	mes_pt;
	DECtoDMS(lonlat, &mes_pt);

	return(&mes_pt);
}

//------------------------------------------------------------
/*
	度、分、秒から１０進へ変換 2000.03.25
*/
//------------------------------------------------------------
void	crd_cnv::
DMStoDEC	(	dms		mes_pt,		// 経緯度（１０進）
				double	*lonlat)	// 変換後の測点の経緯度（度、分、秒）
{
	*lonlat	= mes_pt.deg + mes_pt.min / 60.0 + mes_pt.sec / 3600.0;
}

//------------------------------------------------------------
/*
	度、分、秒から１０進へ変換 2000.03.25
	@return	doubleの緯度および経度値
*/
//------------------------------------------------------------
double	crd_cnv::
DMStoDEC	(	dms		mes_pt	)	// 経緯度（１０進）
{
	double	lonlat	= 0.0;
	lonlat	= mes_pt.deg + mes_pt.min / 60.0 + mes_pt.sec / 3600.0;
	return(lonlat);
}

//---------------------------------------------------------------------
/*
	２地点の経緯度から、２点間の距離 + 距離(X,Y)mを求める
	注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
	@return		２地点間の距離
*/
//---------------------------------------------------------------------
double	crd_cnv::
GetDistXY_Core	(	double		Ra,			///< [in] 地球楕円体の長半径（メートル）
					double		ea2,		///< [in] 第一離心率の2乗
					double		eLon1,		// 経度１
					double		eLat1,		// 緯度１
					double		eLon2,		// 経度２
					double		eLat2,		// 緯度２
					double		*cDist_X,	// 距離X(m)
					double		*cDist_Y)	// 距離Y(m)
{
	double		aFaiRad;
	double		aDeltaX;
	double		aDeltaY;
	double		aSinVal;
	double		aCosVal;
	double		D;
	double		M;
	double		N;
	double		aDistX;
	double		aDistY;

	aFaiRad		= (eLat2 + eLat1) * M_PI / 180.0 / 2.0;
	aDeltaX		= (eLon2 - eLon1) * M_PI / 180.0;
	aDeltaY		= (eLat2 - eLat1) * M_PI / 180.0;

	aSinVal		= sin(aFaiRad);
	aCosVal		= cos(aFaiRad);

	D			= 1.0 - ea2 * aSinVal * aSinVal;
	M			= (Ra * (1.0 - ea2)) / sqrt(D * D * D);
	N			= Ra / sqrt(D);

	aDistY		= M * aDeltaY;
	aDistX		= N * aCosVal * aDeltaX;

	*cDist_X	= aDistX;
	*cDist_Y	= aDistY;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}

//------------------------------------------------------------
/*
	２地点の経緯度から、２点間の距離を求める(単位: m)
	注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
	@return		２地点間の距離
	※ Coreを作成したが、こちらは修正してよいか確認してから
　　　 修正する 2010.10.22
*/
//------------------------------------------------------------
double	crd_cnv::
GetDist	(	double		eLon1,		// 経度１
			double		eLat1,		// 緯度１
			double		eLon2,		// 経度２
			double		eLat2)		// 緯度２
{
	double		aFaiRad;
	double		aDeltaX;
	double		aDeltaY;
	double		aSinVal;
	double		aCosVal;
	double		D;
	double		M;
	double		N;
	double		aDistX;
	double		aDistY;

	aFaiRad		= (eLat2 + eLat1) * M_PI / 180.0 / 2.0;
	aDeltaX		= (eLon2 - eLon1) * M_PI / 180.0;
	aDeltaY		= (eLat2 - eLat1) * M_PI / 180.0;

	aSinVal		= sin(aFaiRad);
	aCosVal		= cos(aFaiRad);

	D			= 1.0 - EA2 * aSinVal * aSinVal;
	M			= 6334834.0 / sqrt(D * D * D);	// 子午線曲率半径
	N			= 6377397.0 / sqrt(D);			// 卯酉線曲率半径

	aDistY		= M * aDeltaY;
	aDistX		= N * aCosVal * aDeltaX;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}

//------------------------------------------------------------
/*
	２地点の経緯度から、２点間の距離を求める(単位: m)
	注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
	@return		２地点間の距離
*/
//------------------------------------------------------------
double	crd_cnv::
GetDist	(	d_lonlat	lonlat1,		// 経緯度１
			d_lonlat	lonlat2)		// 経緯度２
{
	double	dist	= 0.0;
	dist	= GetDist(lonlat1.lon, lonlat1.lat, lonlat2.lon, lonlat2.lat);
	return(dist);
}

//------------------------------------------------------------
/*
	２地点の経緯度から、２点間の距離を求める(単位: m)（世界測地系版）
	注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
	@return		２地点間の距離
*/
//------------------------------------------------------------
double	crd_cnv::
GetDist_JGD2000	(	double		eLon1,		// 経度１
					double		eLat1,		// 緯度１
					double		eLon2,		// 経度２
					double		eLat2)		// 緯度２
{
	double	dist_x = 0.0, dist_y = 0.0;

	return ( GetDistXY_Core	( GRS80_RA, GRS80_EA2, eLon1, eLat1, eLon2, eLat2, &dist_x, &dist_y ) );
}

//---------------------------------------------------------------------
/*
	２地点のメッシュ+(x,y)から、２点間の距離を求める（単位: m) 2001.07.23
	@return		２地点間の距離
*/
//---------------------------------------------------------------------
double	crd_cnv::
GetMeshDist(	int			c_mesh1,	// メッシュコード1
				int			c_x1,		// X座標1
				int			c_y1,		// Y座標1
				int			c_mesh2,	// メッシュコード2
				int			c_x2,		// X座標2
				int			c_y2	)	// Y座標2
{
	double		lon1	= 0.0, lat1		= 0.0;
	double		lon2	= 0.0, lat2		= 0.0;
	double		dist	= 0.0;

	if(MeshtoLL(c_mesh1, c_x1, c_y1, &lon1, &lat1) == -1) {
		return	(-1.0);
	}
	if(MeshtoLL(c_mesh2, c_x2, c_y2, &lon2, &lat2) == -1) {
		return	(-1.0);
	}
	dist	= GetDist(lon1, lat1, lon2, lat2);
	return	(dist);
}

//---------------------------------------------------------------------
/*
	２地点の経緯度から、２点間の距離 + 距離(X,Y)mを求める
	注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
	@return		２地点間の距離
*/
//---------------------------------------------------------------------
double	crd_cnv::
GetDistXY	(	double		eLon1,		// 経度１
				double		eLat1,		// 緯度１
				double		eLon2,		// 経度２
				double		eLat2,		// 緯度２
				double		*cDist_X,	// 距離X(m)
				double		*cDist_Y)	// 距離Y(m)
{
	double		aFaiRad;
	double		aDeltaX;
	double		aDeltaY;
	double		aSinVal;
	double		aCosVal;
	double		D;
	double		M;
	double		N;
	double		aDistX;
	double		aDistY;

	aFaiRad		= (eLat2 + eLat1) * M_PI / 180.0 / 2.0;
	aDeltaX		= (eLon2 - eLon1) * M_PI / 180.0;
	aDeltaY		= (eLat2 - eLat1) * M_PI / 180.0;

	aSinVal		= sin(aFaiRad);
	aCosVal		= cos(aFaiRad);

	D			= 1.0 - EA2 * aSinVal * aSinVal;
	M			= 6334834.0 / sqrt(D * D * D);
	N			= 6377397.0 / sqrt(D);

	aDistY		= M * aDeltaY;
	aDistX		= N * aCosVal * aDeltaX;

	*cDist_X	= aDistX;
	*cDist_Y	= aDistY;

	return sqrt(aDistX * aDistX + aDistY * aDistY);
}

//---------------------------------------------------------------------
/*
	２地点の経緯度から、２点間の距離 + 距離(X,Y)mを求める（世界測地系版）
	注：zclから計算式はほとんど全部引用させてもらっている。(class ZLonLat)
	@return		２地点間の距離
*/
//---------------------------------------------------------------------
double	crd_cnv::
GetDistXY_JGD2000	(	double		eLon1,		///< [in]  経度1
						double		eLat1,		///< [in]  緯度1
						double		eLon2,		///< [in]  経度2
						double		eLat2,		///< [in]  緯度2
						double		*cDist_X,	///< [out]  距離X(m)
						double		*cDist_Y)	///< [out]  距離Y(m)
{
	return ( GetDistXY_Core	( GRS80_RA, GRS80_EA2, eLon1, eLat1, eLon2, eLat2, cDist_X, cDist_Y) );
}

//-2001.2.20-----------------------------------------------------------
/*
	メッシュ + (X,Y)から経緯度へ変換
	※メッシュの桁数がいっしょのときのみ
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetNrmDiff(	int		c_mesh1,	// メッシュコード
		    int		c_x1,		// 正規化座標Ｘ
		    int		c_y1,		// 正規化座標Ｙ
		    int		c_mesh2,	// メッシュコード
		    int		c_x2,		// 正規化座標Ｘ
		    int		c_y2,		// 正規化座標Ｙ
			double	*c_dx,		// 正規化座標差分X
			double	*c_dy)		// 正規化座標差分Y
{
	int		dig1		= GetMeshDigit(c_mesh1);
	int		dig2		= GetMeshDigit(c_mesh2);
	if(dig1 != dig2) { // 桁がそろわない
		return(-1);
	}

	double	lon_1		= 0.0;
	double	lat_1		= 0.0;
	double	lon_2		= 0.0;
	double	lat_2		= 0.0;
	double	lon_diff	= 0.0;	// 経度差分
	double	lat_diff	= 0.0;	// 緯度差分

	double	wid1	= 1.0,	hei1	= 2.0 / 3.0;
	double	wid2	= 0.0,	hei2	= 0.0;
	double	wid64	= 0.0,	hei64	= 0.0;

	wid2	= 7.0 / 60.0 + 30.0 / 3600.0; 
	hei2	= 5.0 / 60.0;
	wid64	= wid2 / 8.0;
	hei64	= hei2 / 8.0;

	if(MeshtoLL(c_mesh1, c_x1, c_y1, &lon_1, &lat_1) != 0) {
		return(-1);
	}
	if(MeshtoLL(c_mesh2, c_x2, c_y2, &lon_2, &lat_2) != 0) {
		return(-1);
	}

	// 差分を取る
	lon_diff	= lon_2 - lon_1;
	lat_diff	= lat_2 - lat_1;

	switch(dig1)
	{
		case	4:
			*c_dx	= lon_diff * D_MILLION / wid1 + 0.5;
			*c_dy	= lat_diff * D_MILLION / hei1 + 0.5;
			break;
		case	6:
			*c_dx	= lon_diff * D_MILLION / wid2 + 0.5;
			*c_dy	= lat_diff * D_MILLION / hei2 + 0.5;
			break;
		case	8:
			*c_dx	= lon_diff * D_MILLION / wid64 + 0.5;
			*c_dy	= lat_diff * D_MILLION / hei64 + 0.5;
			break;
		default:
			return(-1);
	}
	return(0);
}

//-2001.2.20-----------------------------------------------------------
/*
	メッシュ+(x,y) の2てんの正規化ポイントでの差分を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetNrmDiff2(int		c_mesh1,	// メッシュコード
		    int		c_x1,		// 正規化座標Ｘ
		    int		c_y1,		// 正規化座標Ｙ
		    int		c_mesh2,	// メッシュコード
		    int		c_x2,		// 正規化座標Ｘ
		    int		c_y2,		// 正規化座標Ｙ
			double	*c_dx,		// 正規化座標距離X
			double	*c_dy)		// 正規化座標距離Y
{
	int		dig1		= GetMeshDigit(c_mesh1);
	int		dig2		= GetMeshDigit(c_mesh2);
	int		tmp_mesh1	= c_mesh1;
	int		tmp_mesh2	= c_mesh2;
	double	diff_x		= 0.0;
	double	diff_y		= 0.0;

	if(dig1 != dig2) { // 桁がそろわない
		return(-1);
	}

	int		atai_1[6], atai_2[6];	// メッシュをばらばらにする
	int		i;
	for(i = 0; i < 6; i++)
	{
		atai_1[i]	= 0;
		atai_2[i]	= 0;
	}

	if(dig1 == 4) { // １次メッシュコードの時
		for(i = 0; i < 2; i++)
		{
			atai_1[i]	=  tmp_mesh1 % 100;
			atai_2[i]	=  tmp_mesh2 % 100;
			tmp_mesh1	/= 100;
			tmp_mesh2	/= 100;
		}
		diff_y	= (atai_2[1] - atai_1[1]) * D_MILLION + (c_y2 - c_y1);	// 上２桁
		diff_x	= (atai_2[0] - atai_1[0]) * D_MILLION + (c_x2 - c_x1);	// 下２桁
	}
	else
	if(dig1 == 6) { // ２次メッシュコードの時
		for(i = 0; i < 2; i++)
		{
			atai_1[i]	=  tmp_mesh1 % 10;
			atai_2[i]	=  tmp_mesh2 % 10;
			tmp_mesh1	/= 10;
			tmp_mesh2	/= 10;
		}
		atai_1[2]		=  tmp_mesh1 % 100;
		atai_2[2]		=  tmp_mesh2 % 100;
		atai_1[3]		=  tmp_mesh1 / 100;
		atai_2[3]		=  tmp_mesh2 / 100;
		diff_y	= (atai_2[3] - atai_1[3]) * D_MILLION * 8 +
				  (atai_2[1] - atai_1[1]) * D_MILLION + 
				  (c_y2 - c_y1);		// 上２桁
		diff_x	= (atai_2[2] - atai_1[2]) * D_MILLION * 8 +
				  (atai_2[0] - atai_1[0]) * D_MILLION + 
				  (c_x2 - c_x1);		// 下２桁
	}
	else
	if(dig1 == 8) // 1/64メッシュコードの時
	{
		for(i = 0; i < 4; i++)
		{
			atai_1[i]	=  tmp_mesh1 % 10;
			atai_2[i]	=  tmp_mesh2 % 10;
			tmp_mesh1	/= 10;
			tmp_mesh2	/= 10;
		}
		atai_1[4]		=  tmp_mesh1 % 100;
		atai_2[4]		=  tmp_mesh2 % 100;
		atai_1[5]		=  tmp_mesh1 / 100;
		atai_2[5]		=  tmp_mesh2 / 100;
		diff_y	= (atai_2[5] - atai_1[5]) * D_MILLION * 64 + 
				  (atai_2[3] - atai_1[3]) * D_MILLION * 8 +
				  (atai_2[1] - atai_1[1]) * D_MILLION +
				  (c_y2 - c_y1);		// 上２桁
		diff_x	= (atai_2[4] - atai_1[4]) * D_MILLION * 64 +
				  (atai_2[2] - atai_1[2]) * D_MILLION * 8 +
				  (atai_2[0] - atai_1[0]) * D_MILLION +
				  (c_x2 - c_x1);		// 下２桁
	}
	else
	{
		return(-1);
	}
	*c_dx	= diff_x;
	*c_dy	= diff_y;
	return(0);
}

//---------------------------------------------------------------------
/*
	メッシュ + (X,Y)から経緯度へ変換
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoLL	(	int		meshcode,		// メッシュコード
				int		x,				// 正規化座標X
				int		y,				// 正規化座標Y
				double	*lon,			// 経度
				double	*lat)			// 緯度
{
	int	a_iRet	= -1;
	
	a_iRet	= MeshtoLL2_Core( meshcode, (double)x, (double)y, lon, lat, ORIGIN_JPN.lon, ORIGIN_JPN.lat );

	return ( a_iRet );
}

//---------------------------------------------------------------------
/*
	メッシュ + (X,Y)から経緯度へ変換(double版)
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoLL2	(	int		meshcode,		// メッシュコード
				double	x,				// 正規化座標X
				double	y,				// 正規化座標Y
				double	*lon,			// 経度
				double	*lat)			// 緯度
{
	int	a_iRet	= -1;
	
	a_iRet	= MeshtoLL2_Core( meshcode, x, y, lon, lat, ORIGIN_JPN.lon, ORIGIN_JPN.lat  );

	return ( a_iRet );
}

//---------------------------------------------------------------------
/*	2006.09.06 統一版
	メッシュ + (X,Y)から経緯度へ変換(double版)
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoLL2_Core(	unsigned int	meshcode,		// メッシュコード
				double			x,				// 正規化座標X
				double			y,				// 正規化座標Y
				double			*lon,			// 経度
				double			*lat,			// 緯度
				double			offset_lon,		// メッシュ原点のオフセット値（経度）
				double			offset_lat )	// メッシュ原点のオフセット値（緯度）

{

	char	a_cMeshStr[16];

	memset	( a_cMeshStr, '\0', 16 );

	sprintf	( a_cMeshStr, "%.2d", meshcode );	// 2006.10.28 トップマップのため修正

	int	a_iRet	= -1;

	a_iRet	= MeshtoLL2_Core(a_cMeshStr, x, y, lon, lat, offset_lon, offset_lat );

	return	( a_iRet );

}

//---------------------------------------------------------------------
/*	2006.09.07 統一版
	メッシュ(文字列) + (X,Y)から経緯度へ変換(double版)
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoLL2_Core(	char*	meshcode,		// メッシュコード
				double	x,				// 正規化座標X
				double	y,				// 正規化座標Y
				double	*lon,			// 経度
				double	*lat,			// 緯度
				double	offset_lon,		// メッシュ原点のオフセット値（経度）
				double	offset_lat )	// メッシュ原点のオフセット値（緯度）
{
	int		atai[6];
	int		i;
	double	wid1	= 1.0,	hei1	= 2.0 / 3.0;
	double	wid2	= 0.0;
	double	wid64	= 0.0;
	double	hei2	= 0.0;
	double	hei64	= 0.0;
	double	widtop	= 0.0;	// 2003.12.22
	double	heitop	= 0.0;	// 2003.12.22

	char	a_cTmpBuf[24];
	char*	a_cTmpStr[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

	int origin = -1; // 原点判別用 2011.12.12

	// 原点を判別
	if( ORIGIN_JPN.lon  == offset_lon && ORIGIN_JPN.lat == offset_lat )
		origin = JPN_ORIGIN;
	else if( ORIGIN_SJ.lon == offset_lon && ORIGIN_SJ.lat == offset_lat )
		origin = SJ_ORIGIN;

	memset	( a_cTmpBuf, '\0', 24 );

	wid2	= 7.0 / 60.0 + 30.0 / 3600.0; 
	hei2	= 5.0 / 60.0;

	if( SJ_ORIGIN == origin ) {	
		wid64	= wid2 / 4.0;
		hei64	= hei2 / 4.0;
	} else {
		wid64	= wid2 / 8.0;
		hei64	= hei2 / 8.0;
	}

	widtop	= 32.0;
	heitop	= 21.0 + 1.0 / 3.0;	// 21*60+20

	// 初期化
	for(i = 0; i < 6; i++)
	{
		atai[i]		= 0;
		a_cTmpStr[i]= a_cTmpBuf+(i*4);
	}

	// それぞれのレベルの桁数
	int digit_top = ( SJ_ORIGIN == origin ) ? -1 : 2;
	int digit_1st = ( SJ_ORIGIN == origin ) ?  6 : 4;
	int digit_2nd = ( SJ_ORIGIN == origin ) ?  8 : 6;
	int digit_3rd = ( SJ_ORIGIN == origin ) ? 10 : 8;
	int digit_1st_half = digit_1st / 2;

	int	digit	= GetMeshDigit(meshcode);	// 2001.03.21

	if(digit == digit_top) {			// トップマップコードの時

		strncpy ( a_cTmpStr[1], meshcode, 1);	// 上1桁
		strncpy ( a_cTmpStr[0], meshcode+1, 1);	// 下1桁

		// 2006.10.27 式が間違っているので修正
		atai[1]	= atoi(a_cTmpStr[1]);
		atai[0]	= atoi(a_cTmpStr[0]);

		// 日本原点以外は対応しないことにする 2006.09.11
		if( JPN_ORIGIN != origin )
			return ( -1 );
		*lat	=	14.0 + (atai[1] * heitop) + y * heitop / D_MILLION;
		*lon	=	104.0 + (atai[0] * widtop) + x * widtop / D_MILLION;
	} else
	if(digit == digit_1st) {			// １次メッシュコードの時

		strncpy ( a_cTmpStr[1], meshcode,					digit_1st_half);	// 上2桁
		strncpy ( a_cTmpStr[0], meshcode+digit_1st_half,	digit_1st_half);	// 下2桁

		for( i = 1; i >= 0; i-- )
		{
			atai[i]	= atoi(a_cTmpStr[i]);
		}
		if( SJ_ORIGIN == origin )
			atai[1] -= 100; // 新国際都市地図は上3桁は100スタート 2011.12.19

		*lat	= offset_lat + atai[1] / 1.5 + y * hei1 / D_MILLION;
		*lon	= offset_lon + atai[0] + x * wid1 / D_MILLION;
	} else
	if(digit == digit_2nd) {			// ２次メッシュコードの時

		strncpy ( a_cTmpStr[3], meshcode,					digit_1st_half);	// 1次メッシュ上2桁
		strncpy ( a_cTmpStr[2], meshcode+digit_1st_half,	digit_1st_half);	// 1次メッシュ下2桁
		strncpy ( a_cTmpStr[1], meshcode+digit_1st,						 1);	// 2次メッシュ上1桁
		strncpy ( a_cTmpStr[0], meshcode+digit_1st+1,					 1);	// 2次メッシュ下1桁

		for( i = 3; i >= 0; i-- )
		{
			atai[i]	= atoi(a_cTmpStr[i]);
		}
		if( SJ_ORIGIN == origin )
			atai[3] -= 100; // 新国際都市地図は上3桁は100スタート 2011.12.19
	
		*lat	= offset_lat + atai[3] / 1.5 + hei2 * atai[1] + y * hei2 / D_MILLION;
		*lon	= offset_lon + atai[2] + wid2 * atai[0] + x * wid2 / D_MILLION;
	} else
	if(digit == digit_3rd) { // 1/64メッシュコードの時

		strncpy ( a_cTmpStr[5], meshcode,					digit_1st_half);	// 1次メッシュ上2桁
		strncpy ( a_cTmpStr[4], meshcode+digit_1st_half,	digit_1st_half);	// 1次メッシュ下2桁
		strncpy ( a_cTmpStr[3], meshcode+digit_1st,						 1);	// 2次メッシュ上1桁
		strncpy ( a_cTmpStr[2], meshcode+digit_1st+1,					 1);	// 2次メッシュ下1桁
		strncpy ( a_cTmpStr[1], meshcode+digit_1st+2,					 1);	// 8桁メッシュ上1桁
		strncpy ( a_cTmpStr[0], meshcode+digit_1st+3,					 1);	// 8桁メッシュ下1桁

		for( i = 5; i >= 0; i-- )
		{
			atai[i]	= atoi(a_cTmpStr[i]);
		}
		if( SJ_ORIGIN == origin )
			atai[5] -= 100; // 新国際都市地図は上3桁は100スタート 2011.12.19

		*lat	= offset_lat + atai[5] / 1.5 + hei2 * atai[3] + hei64 * atai[1]
				+ y * hei64 / D_MILLION;
		*lon	= offset_lon + atai[4] + wid2 * atai[2] + wid64 * atai[0]
				+ x * wid64 / D_MILLION;
	} else {
		return(-1);
	}
	return(0);
}

//---------------------------------------------------------------------
/*
	メッシュ + (X,Y)から1/256秒単位の経緯度へ変換
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoLL256	(	int		meshcode,	// メッシュコード
				int		x,			// 正規化座標Ｘ
				int		y,			// 正規化座標Ｙ
				int		*lon256,	// 経度
				int		*lat256)	// 緯度
{
	double	lat	= 0.0;
	double	lon	= 0.0;

	int		ret	= 0;
	ret		= MeshtoLL(meshcode, x, y, &lon, &lat);
	if(ret == -1)
	{
		return(-1);
	}

	*lat256	= (int)(lat * 3600.0 * 256.0 + 0.5);
	*lon256	= (int)(lon * 3600.0 * 256.0 + 0.5);
	return(0);
}

//---------------------------------------------------------------------
/*
	１０進小数点経緯度から1/256秒単位の経緯度へ変換
	経度、緯度の入力がintになっていたのを修正 2008.01.21
*/
//---------------------------------------------------------------------
void	crd_cnv::
LLtoLL256	(	double	lon,		///< 経度
				double	lat,		///< 緯度
				int		*lon256,	///< 経度(1/256単位)
				int		*lat256)	///< 緯度(1/256単位)
{
	*lat256	= (int)(lat * 3600 * 256.0 + 0.5);
	*lon256	= (int)(lon * 3600 * 256.0 + 0.5);
}

//---------------------------------------------------------------------
/*
	1/256秒単位の経緯度から、通常の経緯度へ変換
*/
//---------------------------------------------------------------------
void	crd_cnv::
LL256toLL	(	int		lon256,		// 経度
				int		lat256,		// 緯度
				double	*lon,		// 経度
				double	*lat)		// 緯度
{
	*lat	= (lat256 / 3600.0 / 256.0);
	*lon	= (lon256 / 3600.0 / 256.0);
}

//---------------------------------------------------------------------
/*
	メッシュの角から角の距離を求める
*/
//---------------------------------------------------------------------
void	crd_cnv::
CtoCDist	(	int		meshcode,	// メッシュコード
				double	*udist,		// メッシュの上辺距離
				double	*ddist,		// メッシュの下辺距離
				double	*ldist,		// メッシュの左辺距離
				double	*rdist)		// メッシュの右辺距離
{
	double	ld_lat	= 0.0, ld_lon	= 0.0;
	double	lu_lat	= 0.0, lu_lon	= 0.0;
	double	rd_lat	= 0.0, rd_lon	= 0.0;
	double	ru_lat	= 0.0, ru_lon	= 0.0;
	MeshtoLL(meshcode, 0, 0, &ld_lon, &ld_lat);
	MeshtoLL(meshcode, 0, MILLION, &lu_lon, &lu_lat);
	MeshtoLL(meshcode, MILLION, 0, &rd_lon, &rd_lat);
	MeshtoLL(meshcode, MILLION, MILLION, &ru_lon, &ru_lat);
	
	*udist = GetDist(lu_lon, lu_lat, ru_lon, ru_lat);
	*ddist = GetDist(ld_lon, ld_lat, rd_lon, rd_lat);
	*ldist = GetDist(ld_lon, ld_lat, lu_lon, lu_lat);
	*rdist = GetDist(rd_lon, rd_lat, ru_lon, ru_lat);
}

//---------------------------------------------------------------------
/*
	原点メッシュから(x,y)離れた所のメッシュコードを求める
	2001.3.20 マイナス方向でのバグを修正
	@return		成功：メッシュコード \n
	@return		失敗： -1
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetMeshCode(	int		meshcode,	// メッシュコード
				int		xdist,		// 原点メッシュからX方向へ何メッシュ離れているか
				int		ydist)		// 原点メッシュからY方向へ何メッシュ離れているか
{

	int		meshunit	= 0;
	int		roop_count	= 0;	// 2001.03.21

	int		mc			= 0;				// ８桁メッシュコード
	int		num1[6]		= {0,0,0,0,0,0};	// ８桁ばらばらにする
	int		dist[2]		= {0,0};			// 2001.03.21
	int		retmesh		= 0;
	int		tmp1		= 0;
	int		tmp2		= 0;

	dist[0]				= xdist;
	dist[1]				= ydist;

	mc		= meshcode;

	int		digit		= GetMeshDigit(mc);	// 2001.03.21

	if(digit == 4) {		// １次メッシュ
		meshunit	= 1;
		roop_count	= 0;
	} else
	if(digit == 6) {		// ２次メッシュ
		meshunit	= 2;
		roop_count	= 2;
	} else
	if(digit == 8) {		// ８桁メッシュ
		meshunit	= 64;
		roop_count	= 4;
	} else {
		return(-1);
	}

	// 桁をばらばらにする(59 41 4 1 7 7 のようにする)
	int	i;
	for(i = 0; i < roop_count; i++)
	{
		num1[i]	= mc % 10;
		mc		/= 10;
	}
	num1[roop_count]	= mc % 100;
	num1[roop_count+1]	= mc / 100;

	int	j = 0;
	if(meshunit == 64) {
		// j = 0 ... 左右方向に離れたメッシュコード
		// j = 1 ... 上下方向に離れたメッシュコード
		for(j = 0; j < 2; j++) {
			num1[j]	+= dist[j];
			if(num1[j] > 7) {
				num1[j+2]	+= num1[j] / 8;			// 桁の操り上げ
				num1[j]		%= 8;
				if(num1[j+2] > 7) {
					num1[j+4]	+= num1[j+2] / 8;	// 桁の操り上げ
					num1[j+2]	%= 8;
				}
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);		// 桁の繰下げ
				num1[j]		=  7 - (-num1[j] % 8);
				if(num1[j+2] < 0) {
					num1[j+2]	-= 7;
					num1[j+4]	-= (-num1[j+2] / 8);// 桁の繰下げ
					num1[j+2]	= 7 - (-num1[j+2] % 8);
				}
			}
		}

		retmesh	= num1[5] * MILLION	+ num1[4] * 10000 + 
					num1[3] * 1000	+ num1[2] * 100 +
					num1[1] * 10	+ num1[0];
	} else	
	if(meshunit == 2) { // ２次メッシュ用
		// j = 0 ... 左右方向に離れたメッシュコード, j = 1 ... 上下方向に離れたメッシュコード
		for(j = 0; j < 2; j++) {
			num1[j] += dist[j];
			if(num1[j] > 7) {
				num1[j+2]	+= num1[j] / 8;				// 桁の繰上げ
				num1[j]		%= 8;
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);		// 桁の繰下げ
				num1[j]		=  7 - (-num1[j] % 8);
			}
		}

		retmesh	= num1[3] * 10000	+ num1[2] * 100 +
					num1[1] * 10	+ num1[0];
	} else
	if(meshunit == 1) { // １次メッシュの場合
		// 左右に離れたメッシュコード
		num1[0] += dist[0];
		num1[1] += dist[1];

		retmesh	= num1[1] * 100 + num1[0];

	} else {
		//fprintf(stderr, "メッシュ単位の指定 %d は間違いです。\n", meshunit);
		return(-1);
	}
	return(retmesh);
}

//---------------------------------------------------------------------
/*
	原点メッシュから(x,y)離れた所のメッシュコードを求める（新国際都市地図版） 2011.12.09
	@return		成功：メッシュコード \n
	@return		失敗： -1
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetMeshCode_SJ(	unsigned int	meshcode,	// メッシュコード
				int				xdist,		// 原点メッシュからX方向へ何メッシュ離れているか
				int				ydist)		// 原点メッシュからY方向へ何メッシュ離れているか
{

	int				meshunit	= 0;
	int				roop_count	= 0;	// 2001.03.21

	unsigned int	mc			= 0;				// メッシュコード
	int				num1[8]		= {0,0,0,0,0,0,0,0};// 桁をばらばらにする
	int				dist[2]		= {0,0};			// 2001.03.21
	int				retmesh		= 0;
	int				tmp1		= 0;
	int				tmp2		= 0;

	dist[0]	= xdist;
	dist[1]	= ydist;

	mc = meshcode;

	int	digit = GetMeshDigit(mc);	// 2001.03.21

	if(digit == 6) {		// １次メッシュ
		meshunit	= 1;
		roop_count	= 0;
	} else
	if(digit == 8) {		// ２次メッシュ
		meshunit	= 2;
		roop_count	= 2;
	} else
	if(digit == 10) {		// ３次メッシュ
		meshunit	= 64;
		roop_count	= 4;
	} else {
		return(-1);
	}

	// 桁をばらばらにする（例：「5941416701」の場合、594 141 6 7 0 1とし、
	// num1には、num1[0]=1, num1[1]=0, num1[2]=7, num1[3]=6, num1[4]=141, num1[5]=594と格納される）
	for(int i = 0; i < roop_count; i++)
	{
		num1[i]	= mc % 10;
		mc		/= 10;
	}

	num1[roop_count]	= mc % 1000;
	num1[roop_count+1]	= mc / 1000;

	if(meshunit == 64) {
		// j = 0 ... 左右方向に離れたメッシュコード
		// j = 1 ... 上下方向に離れたメッシュコード
		for(int j = 0; j < 2; j++) {
			num1[j]	+= dist[j];
			if(num1[j] > 3) {
				num1[j+2]	+= num1[j] / 4;			// 桁の操り上げ
				num1[j]		%= 4;
				if(num1[j+2] > 7) {
					num1[j+4]	+= num1[j+2] / 8;	// 桁の操り上げ
					num1[j+2]	%= 8;
				}
			} else
			if(num1[j] < 0) {
				num1[j]		-= 3;
				num1[j+2]	-= (-num1[j] / 4);		// 桁の繰下げ
				num1[j]		=  3 - (-num1[j] % 4);
				if(num1[j+2] < 0) {
					num1[j+2]	-= 7;
					num1[j+4]	-= (-num1[j+2] / 8);// 桁の繰下げ
					num1[j+2]	= 7 - (-num1[j+2] % 8);
				}
			}
		}

		retmesh	= num1[5] * 10000000	+ num1[4] * 10000 + 
					num1[3] * 1000	+ num1[2] * 100 +
					num1[1] * 10	+ num1[0];
	} else	
	if(meshunit == 2) { // ２次メッシュ用
		// j = 0 ... 左右方向に離れたメッシュコード, j = 1 ... 上下方向に離れたメッシュコード
		for(int j = 0; j < 2; j++) {
			num1[j] += dist[j];
			if(num1[j] > 7) {
				num1[j+2]	+= num1[j] / 8;				// 桁の繰上げ
				num1[j]		%= 8;
			} else
			if(num1[j] < 0) {
				num1[j]		-= 7;
				num1[j+2]	-= (-num1[j] / 8);		// 桁の繰下げ
				num1[j]		=  7 - (-num1[j] % 8);
			}
		}

		retmesh	= num1[3] * 100000	+ num1[2] * 100 +
					num1[1] * 10	+ num1[0];
	} else
	if(meshunit == 1) { // １次メッシュの場合
		// 左右に離れたメッシュコード
		num1[0] += dist[0];
		num1[1] += dist[1];

		retmesh	= num1[1] * 1000 + num1[0];

	} else {
		//fprintf(stderr, "メッシュ単位の指定 %d は間違いです。\n", meshunit);
		return(-1);
	}
	return(retmesh);
}

//---------------------------------------------------------------------
/*
	経緯度から、メッシュコード + (x,y) を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh	(	double		lon,		// 経度
				double		lat,		// 緯度
				int			level,		// メッシュのレベル (0,1,2,64)
				int			*meshcode,	// メッシュコード
				int			*x,			// 正規化Ｘ
				int			*y,			// 正規化Ｙ
				int			no_mill	)	// XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, (unsigned int *)meshcode, x, y, ORIGIN_JPN.lon, ORIGIN_JPN.lat, no_mill );

	return	( a_iRet );
}

/** ------------------------------------------------------------------
	経緯度から、メッシュコード + (x,y) を求める
	(世界共通版(日本も)) 2006.09.06
	@retval	0	成功
	@retval -1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh_Core(	double			lon,			///< 経度
				double			lat,			///< 緯度
				int				level,			///< メッシュのレベル (1,2,64)
				unsigned int	*meshcode,		///< メッシュコード
				int				*x,				///< 正規化Ｘ
				int				*y,				///< 正規化Ｙ
				double			offset_lon,		///< メッシュ原点のオフセット値（経度）
				double			offset_lat, 	///< メッシュ原点のオフセット値（緯度）
				int				no_mill	)		///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す） 
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
	// topmap用に追加
	double	lat_amari0	= 0.0,	lon_amari0	= 0.0;
	int		up_0		= 0,	low_0		= 0;
	double	widtop		= 32.0;								// 2003.12.22
	double	heitop		= 21.0 + 1.0 / 3.0;	// 21*60+20;	// 2003.12.22
	double	x_step_0	= 0.0,	y_step_0	= 0.0;
	int origin = -1; // 原点判別用 2011.12.09

	// 原点を判別
	if( ORIGIN_JPN.lon  == offset_lon && ORIGIN_JPN.lat == offset_lat )
		origin = JPN_ORIGIN;
	else if( ORIGIN_SJ.lon == offset_lon && ORIGIN_SJ.lat == offset_lat )
		origin = SJ_ORIGIN;

	x_step_0	= widtop / D_MILLION;	// for topmap
	y_step_0	= heitop / D_MILLION;	// for topmap

	x_step_1	= wid1 / D_MILLION;
	y_step_1	= hei1 / D_MILLION;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	x_step_2	= wid2 / D_MILLION;
	y_step_2	= hei2 / D_MILLION;

	if( SJ_ORIGIN == origin ) {
		wid64		= wid2 / 4.0;
		hei64		= hei2 / 4.0;
	} else {
		wid64		= wid2 / 8.0;
		hei64		= hei2 / 8.0;
	}

	x_step_3	= wid64 / D_MILLION;
	y_step_3	= hei64 / D_MILLION;
	
	up_1st		= (lat - offset_lat) * 1.5;	// 2006.09.06
	low_1st		= lon - offset_lon;			// 2006.09.06

	lat_amari1	= lat - (up_1st / 1.5) - offset_lat;	// 2006.09.06
	lon_amari1	= lon - (low_1st + offset_lon);

	switch(level)
	{
		case	0:	// トップマップ
			// 日本原点以外は対応しないことにする 2006.09.11
			if( JPN_ORIGIN == origin ) {
				return ( -1 );
			}
			// 2006.10.28 値がおかしかったので修正
			up_0		= (lat - 14.0) / heitop;
			low_0		= (lon - 104.0) / widtop;
			lat_amari0	= lat - (up_0 * heitop + 14.0); 
			lon_amari0	= lon - (low_0 * widtop + 104.0);
			*meshcode	= up_0 * 10 + low_0;
			*x			= lon_amari0 / x_step_0 + 0.5;
			*y			= lat_amari0 / y_step_0 + 0.5;
			break;
		case	1:
			if( SJ_ORIGIN == origin ) // 新世界測地系対応 2011.12.09
				*meshcode	= ( up_1st + 100 ) * 1000 + low_1st; // 一次メッシュ上3桁は100スタート 2011.12.19
			else
				*meshcode	= up_1st * 100 + low_1st;
			*x			= lon_amari1 / x_step_1 + 0.5;
			*y			= lat_amari1 / y_step_1 + 0.5;
			break;
		case	2:
			up_2nd		= lat_amari1 / hei2;
			low_2nd		= lon_amari1 / wid2;
			lat_amari2	= lat_amari1 - up_2nd * hei2;
			lon_amari2	= lon_amari1 - low_2nd * wid2;
			if( SJ_ORIGIN == origin ) // 新世界測地系対応 2011.12.09
				*meshcode	= ( up_1st + 100 ) * 100000 + low_1st * 100 + up_2nd * 10 + low_2nd; // 一次メッシュ上3桁は100スタート 2011.12.19
			else
				*meshcode	= up_1st * 10000 + low_1st * 100 + up_2nd * 10 + low_2nd;
			*x			= lon_amari2 / x_step_2 + 0.5;
			*y			= lat_amari2 / y_step_2 + 0.5;
			break;
		case	64:
			up_2nd		= lat_amari1 / hei2;
			low_2nd		= lon_amari1 / wid2;
			lat_amari2	= lat_amari1 - up_2nd * hei2;
			lon_amari2	= lon_amari1 - low_2nd * wid2;
			up_64		= lat_amari2 / hei64;
			low_64		= lon_amari2 / wid64;
			lat_amari3	= lat_amari2 - up_64 * hei64;
			lon_amari3	= lon_amari2 - low_64 * wid64;
			if( SJ_ORIGIN == origin ) // 新世界測地系対応 2011.12.09
				*meshcode	= ( up_1st + 100 ) * 10000000 + low_1st * 10000 + up_2nd * 1000 + low_2nd * 100 + up_64 * 10 + low_64; // 一次メッシュ上3桁は100スタート 2011.12.19
			else
				*meshcode	= up_1st * MILLION + low_1st * 10000 + up_2nd * 1000 + low_2nd * 100 + up_64 * 10 + low_64;
			*x			= lon_amari3 / x_step_3 + 0.5;
			*y			= lat_amari3 / y_step_3 + 0.5;
			break;
		default:
			break;
	}
	if( level == 1 || level == 2 || level == 64 ) {
		if( no_mill == 1 ) {
			int	xflag = 0, yflag = 0;
			if( *x == MILLION ) {
				xflag	= 1;
				*x		= 0;
			}
			if( *y == MILLION ) {
				yflag	= 1;
				*y		= 0;
			}
			if( SJ_ORIGIN == origin )
				*meshcode	= GetMeshCode_SJ(*meshcode, xflag, yflag );
			else
				*meshcode	= GetMeshCode(*meshcode, xflag, yflag );
		}
		return(0);
	}else
	if( level == 0 ) {	// 2006.09.08 トップが無いので追加
		return(0);
	}
	return(-1);
}

/** ------------------------------------------------------------------
	経緯度から、メッシュコード(文字列) + (x,y) を求める
	(世界共通版(日本も)) 2006.09.08
	@retval	0	成功
	@retval -1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh_Core(	double	lon,			///< 経度
				double	lat,			///< 緯度
				int		level,			///< メッシュのレベル (0,1,2,64)
				char*	meshcode,		///< メッシュコード
				int		*x,				///< 正規化Ｘ
				int		*y,				///< 正規化Ｙ
				double	offset_lon,		///< メッシュ原点のオフセット値（経度）
				double	offset_lat,		///< メッシュ原点のオフセット値（緯度）
				int		no_mill	)		///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す） 
{
	char	a_cMeshStr[16];

	memset	( a_cMeshStr, '\0', 16 );

	int	a_iRet			= -1;
	unsigned int	a_iTmpMeshCode	= 0;

	a_iRet	=	LLtoMesh_Core( lon, lat,	level, &a_iTmpMeshCode, x, y, offset_lon, offset_lat, no_mill );

	if( a_iRet == -1 ) {
		return ( a_iRet );
	}
	if( meshcode == NULL ) {
		return	( -1 );
	}

	switch (level)
	{
	case	0:
		sprintf	( a_cMeshStr, "%.2d", a_iTmpMeshCode );
		break;
	case	1:
		sprintf	( a_cMeshStr, "%.4d", a_iTmpMeshCode );
		break;
	case	2:
		sprintf	( a_cMeshStr, "%.6d", a_iTmpMeshCode );
		break;
	case	64:
		sprintf	( a_cMeshStr, "%.8d", a_iTmpMeshCode );
		break;
	default:
		return	( -1 );
	}
	// メッシュコード（文字列）のコピー
	strcpy	( meshcode, a_cMeshStr );

	return	( a_iRet );
}
		
//---------------------------------------------------------------------
/*
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh2	(	double		c_lon,			// 経度
				double		c_lat,			// 緯度
				int			c_meshcode,		// メッシュコード
				double		*c_x,			// 正規化Ｘ
				double		*c_y)			// 正規化Ｙ
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_JPN.lon, ORIGIN_JPN.lat );

	return ( a_iRet );
}

//---------------------------------------------------------------------
/*
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh2	(	double		c_lon,			// 経度
				double		c_lat,			// 緯度
				int			c_meshcode,		// メッシュコード
				int			*c_x,			// 正規化Ｘ
				int			*c_y)			// 正規化Ｙ
{
	double	tmp_x	= 0.0;
	double	tmp_y	= 0.0;
	if( LLtoMesh2 ( c_lon, c_lat, c_meshcode, &tmp_x, &tmp_y ) == -1) {
		return	(-1);
	}
	*c_x	= tmp_x + 0.5;
	*c_y	= tmp_y + 0.5;
	return	(0);
}

/** ------------------------------------------------------------------
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める 2006.09.07
	@retval	0	成功
	@retval -1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh2_Core(	double			c_lon,				///< 経度
				double			c_lat,				///< 緯度
				unsigned int	c_meshcode,			///< メッシュコード
				double			*c_x,				///< 正規化Ｘ
				double			*c_y,				///< 正規化Ｙ
				double			offset_lon,			///< メッシュ原点のオフセット値（経度）
				double			offset_lat	)		///< メッシュ原点のオフセット値（緯度）
{

	char	a_cMeshStr[16];

	memset	( a_cMeshStr, '\0', 16 );

	sprintf	( a_cMeshStr, "%.2d", c_meshcode );	// 2006.10.28 トップマップのため修正

	int	a_iRet	= -1;

	a_iRet	= LLtoMesh2_Core( c_lon, c_lat, a_cMeshStr, c_x, c_y, offset_lon, offset_lat );
	
	return	( a_iRet );
}

/** ------------------------------------------------------------------
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコード(文字列)にたいして、正規化座標値を求める 2006.09.08
	@retval	0	成功
	@retval -1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh2_Core(	double	c_lon,			///< 経度
				double	c_lat,			///< 緯度
				char*	c_meshcode,		///< メッシュコード
				double	*c_x,			///< 正規化Ｘ
				double	*c_y,			///< 正規化Ｙ
				double	offset_lon,		///< メッシュ原点のオフセット値（経度）
				double	offset_lat	)	///< メッシュ原点のオフセット値（緯度）
{
	double	wid1		= 1.0,	hei1		= 2.0 / 3.0;
	double	wid2		= 0.0,	hei2		= 0.0;
	double	wid64		= 0.0,	hei64		= 0.0;
	double	lat_diff	= 0.0,	lon_diff	= 0.0;
	double	x_step_1	= 0.0,	y_step_1	= 0.0;
	double	x_step_2	= 0.0,	y_step_2	= 0.0;
	double	x_step_3	= 0.0,	y_step_3	= 0.0;

	// トップマップ用 2003.01.05
	double	widtop		= 32.0;								// 2003.12.22
	double	heitop		= 21.0 + 1.0 / 3.0;	// 21*60+20;	// 2003.12.22
	double	x_step_0	= 0.0,	y_step_0	= 0.0;
	// トップマップ用ここまで

	double	ori_lon		= 0.0,	ori_lat		= 0.0;	// 左下原点経緯度

	int		digit	= 0;

	// メッシュの桁数を獲得
	digit	= GetMeshDigit( c_meshcode );
	if( digit == -1) {
		return	(-1);
	}

	// トップマップ用 2003.01.05
	if( digit == 2 ) {
		x_step_0	= widtop / D_MILLION;
		y_step_0	= heitop / D_MILLION;
	}
	// トップマップ用ここまで

	int	a_iRet	= -1;

	a_iRet	= MeshtoLL2_Core	( c_meshcode, 0.0, 0.0, &ori_lon, &ori_lat, offset_lon, offset_lat );
	if( a_iRet == -1 ) {
		return	(-1);
	}

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
	
	lon_diff	= c_lon - ori_lon;
	lat_diff	= c_lat - ori_lat;

	switch( digit )
	{
		// トップマップ用 2003.01.05
		case	2:
			*c_x			= lon_diff / x_step_0;
			*c_y			= lat_diff / y_step_0;
			return(0);
		// トップマップ用ここまで
		case	4:
			*c_x			= lon_diff / x_step_1;
			*c_y			= lat_diff / y_step_1;
			return(0);
		case	6:
			*c_x			= lon_diff / x_step_2;
			*c_y			= lat_diff / y_step_2;
			return(0);
		case	8:
			*c_x			= lon_diff / x_step_3;
			*c_y			= lat_diff / y_step_3;
			return(0);
		default:
			return(-1);
	}
}

//---------------------------------------------------------------------
/*
	○次メッシュの(x,y)から、×次メッシュの(x,y)に変換
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoMesh	(	int		s_mesh,		// 元メッシュコード
				int		s_x,		// 正規化Ｘ
				int		s_y,		// 正規化Ｙ
				int		level,		// メッシュの次数 (1,2,64)
				int		*d_mesh,	// 変換後メッシュコード
				int		*d_x,		// 正規化Ｘ
				int		*d_y)		// 正規化Ｙ
{
	double	lat	= 0.0,	lon	= 0.0;
	if(MeshtoLL(s_mesh, s_x, s_y, &lon, &lat) != 0)
	{
		return(-1);
	}
	if(LLtoMesh(lon, lat, level, d_mesh, d_x, d_y) != 0)
	{
		return(-1);
	}
	return(0);
}

//---------------------------------------------------------------------
/*
	○次メッシュの(x,y)から、×次メッシュの(x,y)に変換 2000.10.06
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
MeshtoMesh2	(	int		s_mesh,		// 元メッシュコード
				int		s_x,		// 正規化Ｘ
				int		s_y,		// 正規化Ｙ
				int		level,		// メッシュの次数 (1,2,64)
				int		*d_mesh,	// 変換後メッシュコード
				int		*d_x,		// 正規化Ｘ
				int		*d_y)		// 正規化Ｙ
{
	int		i;
	int		atai[6];
	int		digit		= 0;	// 桁数
	int		ret			= 0;
	int		fact		= 8;
	int		tmp_mesh 	= 0, tmp_x	= 0, tmp_y	= 0;
	int		move_x		= 0, move_y	= 0;	// メッシュの移動量

	// 注意:内部では、メッシュを、整数として扱う

	// 初期化
	for(i = 0; i < 6; i++)
	{
		atai[i]	= 0;
	}

	digit	= GetMeshDigit(s_mesh);
	if(digit == 4 || digit == 6 || digit == 8) {
		int work_x = 0, work_y = 0;
		// 全部一度都市地図メッシュにする
		if( digit == 4 ) {
			work_x		= s_x * 64;
			work_y		= s_y * 64;
			tmp_mesh	= s_mesh * 10000;
		}else 
		if( digit == 6 ) {
			work_x		= s_x * 8;
			work_y		= s_y * 8;
			tmp_mesh	= s_mesh * 100;
		}else {
			work_x		= s_x;
			work_y		= s_y;
			tmp_mesh	= s_mesh;
		}
		int		x_shift	= work_x / MILLION;
		int		y_shift	= work_y / MILLION;

		// ここで整合性のとれた8桁になる
		tmp_x	= work_x % MILLION;
		tmp_y	= work_y % MILLION;

		if( tmp_x < 0 ) {
			tmp_x	+= MILLION;
			x_shift--;
		}
		if( tmp_y < 0 ) {
			tmp_y	+= MILLION;
			y_shift--;
		}

		tmp_mesh= GetMeshCode(tmp_mesh, x_shift, y_shift);

		// レベル別に変換
		if( level == 1 ) {
			*d_mesh	= tmp_mesh / 10000;
			tmp_x	= MILLION / 8 * ((tmp_mesh % 1000) / 100) + MILLION / 64 * (tmp_mesh % 10) + tmp_x / 64;
			tmp_y	= MILLION / 8 * ((tmp_mesh % 10000) / 1000 ) + MILLION / 64 * (tmp_mesh % 100 / 10) + tmp_y / 64;
		}else
		if( level == 2 ) {
			*d_mesh	= tmp_mesh / 100;
			tmp_x	= MILLION / 8 * (tmp_mesh % 10) + tmp_x / 8;
			tmp_y	= MILLION / 8 * (tmp_mesh % 100 / 10) + tmp_y / 8;
		}else {
			*d_mesh= tmp_mesh;
			tmp_x	= tmp_x;
			tmp_y	= tmp_y;
		}
		*d_x	= tmp_x;
		*d_y	= tmp_y;
	}else {
		ret		= -1;
	}
	return(ret);
}

//---------------------------------------------------------------------
/*
	メッシュコードの桁数判定 2000.4.6
	// 2003.10.22 トップマップに対応
	// 2011.12.09 新国際都市地図に対応
	@return		成功 = コードの桁数 2,4,6,8,10 \n	
	@return		失敗 = -1
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetMeshDigit	(	unsigned int	s_mesh)		// 元メッシュコード
{
	if( 0 <= s_mesh && s_mesh < 100)	// 2004.01.15 追加
	{
		return(2);
	}
	else
	if(    1000 <= s_mesh && s_mesh < 10000)
	{
		return(4);
	}
	else
	if(  100000 <= s_mesh && s_mesh < 1000000)
	{
		return(6);
	}
	else
	if(10000000 <= s_mesh && s_mesh < 100000000)
	{
		return(8);
	}
	else
	if(1000000000 <= s_mesh && s_mesh < 10000000000) // 2011.12.09 追加
	{
		return(10);
	}
	return(-1);
}

/** ------------------------------------------------------------------
	メッシュコードの桁数判定(文字列用) 2006.9.7
	// 2011.12.09 新国際都市地図に対応
	@return		成功 = コードの桁数 2,4,6,8,10 \n
	@return		失敗 = -1
--------------------------------------------------------------------*/
int	crd_cnv::
GetMeshDigit	(	char*	s_mesh	)	///< 元メッシュコード
{
	int	a_iRet	= -1;
	int	a_iLen	= 0;

	if( s_mesh == NULL ) {
		return ( -1 );	
	}else {
		a_iLen	= strlen ( s_mesh );
		switch ( a_iLen )
		{
		case	2:
		case	4:
		case	6:
		case	8:
		case	10: // 2011.12.09 追加
			return	( a_iLen );
		default:
			return	( -1 );
		}
	}
}

//---------------------------------------------------------------------
/*
	経緯度とそこから経度方向にXm,緯度方向にYm離れたところの経緯度を求める \n
	(近似値) 2000.4.13
*/
//---------------------------------------------------------------------
/*void	crd_cnv::
GetLLPt	(	double		eLon1,		// 経度１
			double		eLat1,		// 緯度１
			double		cDistX,		// 経度方向(m)
			double		cDistY,		// 緯度方向(m)
			double		*eLon2,		// 経度２
			double		*eLat2)		// 緯度２
{
	double		aFaiRad;
	double		aDeltaX;
	double		aDeltaY;
	double		aSinVal;
	double		aCosVal;
	double		D;
	double		M;
	double		N;

	// 
	aFaiRad		= eLat1 * M_PI / 180.0;
	aSinVal		= sin(aFaiRad);
	aCosVal		= cos(aFaiRad);
	D			= 1.0 - EA2 * aSinVal * aSinVal;
	M			= 6334834.0 / sqrt(D * D * D);
	N			= 6377397.0 / sqrt(D);
	aDeltaX		= cDistX / (N * aCosVal);
	aDeltaY		= cDistY / M;

	*eLon2		= eLon1 + aDeltaX * 180.0 / M_PI;
	*eLat2		= eLat1 + aDeltaY * 180.0 / M_PI;
}
*/

//---------------------------------------------------------------------
/*
	経緯度とそこから経度方向にXm,緯度方向にYm離れたところの経緯度を求める \n
	2000.4.13
	※収束しないケースがあるとの報告があったので収束回数の制限を設定(50回)
	　計算内で一番近い結果になった経緯度を返すように変更。
	@return		収束までの回数 \n
	@return		-1 = 制限回数内に収束しない
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetLLPt(	double		eLon1,		// 経度１
			double		eLat1,		// 緯度１
			double		cDist_X,	// 距離Ｘ(m)
			double		cDist_Y,	// 距離Ｙ(m)
			double		*eLon2,		// 経度２
			double		*eLat2)		// 緯度２
{
	return	GetLLPt_Core(6377397.0, EA2, eLon1, eLat1, cDist_X, cDist_Y, 0, eLon2, eLat2);
}

//---------------------------------------------------------------------
/*
	経緯度とそこから経度方向にXm,緯度方向にYm離れたところの経緯度を求める（世界測地系版）
	※収束しないケースがあるとの報告があったので収束回数の制限を設定(50回)
	　計算内で一番近い結果になった経緯度を返すように変更。
	@return		収束までの回数
	@return		-1 = 制限回数内に収束しない
*/
//---------------------------------------------------------------------
int crd_cnv::
	GetLLPt_JGD2000(double		eLon1,		// 経度１
					double		eLat1,		// 緯度１
					double		cDist_X,	// 距離Ｘ(m)
					double		cDist_Y,	// 距離Ｙ(m)
					double		*eLon2,		// 経度２
					double		*eLat2)		// 緯度２
{
	return	GetLLPt_Core(GRS80_RA, GRS80_EA2, eLon1, eLat1, cDist_X, cDist_Y, 1, eLon2, eLat2);
}

//---------------------------------------------------------------------
/*
	経緯度とそこから経度方向にXm,緯度方向にYm離れたところの経緯度を求める（統一版）
	※収束しないケースがあるとの報告があったので収束回数の制限を設定(50回)
	　計算内で一番近い結果になった経緯度を返すように変更。
	@return		収束までの回数 \n
	@return		-1 = 制限回数内に収束しない
*/
//---------------------------------------------------------------------
int	crd_cnv::
GetLLPt_Core(	double		ra,			// 地球楕円体の長半径（m）
				double		ea2,		// 第一離心率の2乗
				double		eLon1,		// 経度１
				double		eLat1,		// 緯度１
				double		cDist_X,	// 距離Ｘ(m)
				double		cDist_Y,	// 距離Ｙ(m)
				int			mode,		// 変換モード(0=日本測地系,1=世界測地系)
				double		*eLon2,		// 経度２
				double		*eLat2)		// 緯度２
{
	double	cur_lon	= 0.0, cur_lat	= 0.0;
	double	min_lon	= 0.0, min_lat	= 0.0;
	double	max_lon	= 0.0, max_lat	= 0.0;
	double	d_x1	= 0.0, d_y1	= 0.0;
	double	d_x2	= 0.0,d_y2	= 0.0;
	double	t_dist	= 0.0;
	double	n_lon	= 0.0;	double	n_lat	= 0.0;	// 一番近いところ
	int		count	= 0;
	bool	flag	= true;

	if(cDist_Y < 0)	{
		min_lat	= -90.0;
		max_lat	= eLat1;
	} else {
		min_lat	= eLat1;
		max_lat	= 90;
	}

	// まず、緯度方向の探索
	while(1)
	{
		if( count >= 50 ) {
			flag	= false;
			break;	// 収束しない場合は抜ける
		}
		cur_lat	= (max_lat + min_lat) / 2.0;
		if( mode == 0 )
			t_dist = GetDistXY(eLon1, eLat1, eLon1, cur_lat, &d_x1, &d_y1);
		else
			t_dist= GetDistXY_Core(ra, ea2, eLon1, eLat1, eLon1, cur_lat, &d_x1, &d_y1);

		if(fabs(d_y1 - cDist_Y) > 0.001) {
			if(d_y1 - cDist_Y > 0)
				max_lat	= cur_lat;
			else
				min_lat	= cur_lat;
		} else {
			*eLat2 = cur_lat;
			break;
		}
		count++;
	}

	// 次に、経度を計算
	double aFaiRad		= ( eLat1 + *eLat2 ) * M_PI / 180.0 / 2.0;
	double aSinVal		= sin(aFaiRad);
	double aCosVal		= cos(aFaiRad);

	double D			= 1.0 - ea2 * aSinVal * aSinVal;
	double N			= ra / sqrt(D);

	// cosが0、すなわち極点の場合はX距離が0mであるときに限り、与えられた値を返す
	if( aCosVal == 0.0 ){
		if( cDist_X = 0.0 ){
			*eLon2 = eLon1;
		}
		else{
			// それ以外はNG
			flag = false;
		}
	} else{
		*eLon2 = eLon1 + cDist_X / ( N * aCosVal ) * 180.0 / M_PI;
	}
	// 結果がNGであれば、返す座標は手がかりにもならないため元の座標に戻す
	// ※NGな場合は、極点に行ってしまう場合と地球を1/4周離れた距離(約1万km)を指定された場合です
	// 　半分ずつ近づけていく方法では、そんなに離れた距離には近づかないと思います。
	// 　地球1週分の距離で割った余りにすれば、出せると思いますが、サポートしなくても良い気がします。
	if(!flag){
		*eLon2 = eLon1;
		*eLat2 = eLat1;
	}

	return (flag ? count : -1);


}

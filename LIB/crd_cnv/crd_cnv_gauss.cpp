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

// ドイツのガウス・クリューゲル座標系用
#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//------------------------------------------------------------
/**
	ガウス座標系の表を作るメンバ関数
*/
//------------------------------------------------------------
void	crd_cnv::
Make_Gauss_tbl	(	void	)
{
	d_lonlat	origin_gauss[3] = 
					{{6.0,	0.0},	// ゾーン２
					 {9.0,	0.0},	// ゾーン３
					 {12.0,	0.0}};	// ゾーン４

	// ガウス座標原点のラジアンの表を作成する
	int	i;
	for (i = 0; i < 3; i++)
	{
		Rad_Origin_Gauss[i].lat	= (origin_gauss[i].lat) * M_PI / 180.0;
		Rad_Origin_Gauss[i].lon	= (origin_gauss[i].lon) * M_PI / 180.0;
	}
}	


//------------------------------------------------------------
/**
	経緯度からガウス座標系のＸＹへ変換
	@return	構造体 xy_coord へのポインタ
*/
//------------------------------------------------------------
xy_coord*	crd_cnv::
LLtoXYGs	(	int			axis,	///< 該当するゾーンの番号
				double		lon,	///< 経度
				double		lat)	///< 緯度
{

	double			R;

	double			d_Ramda,	t;
	double			N_2,		N_4;
	double			Phi;
	double			p, q, r, s, u;
	double			x, y, d_x;
	static xy_coord	coord;

	Make_Gauss_tbl();			// ガウス座標系のゾーンテーブルを作成

	d_Ramda	= lon * M_PI / 180.0 - Rad_Origin_Gauss[axis - 2].lon;
	Phi		= lat * M_PI / 180.0;	// lat = φ

	N_2		= EB2 * pow(cos(Phi), 2.0);						// n*n
	N_4		= pow(N_2, 2.0);								// N_2*N_2
	R		= RA / sqrt(1.0 - EA2 * pow(sin(Phi), 2.0));	// R = a
	t		= tan(Phi);

	p		= pow(cos(Phi), 2.0) * (5.0 - t*t + 9.0 * N_2 + 4.0 * N_4)/12.0;
	q		= pow(cos(Phi), 4.0) * (61.0 - 58.0 * t*t + pow(t, 4.0) + 270.0 * N_2 - 330.0 * t*t * N_2)/360.0;
	r		= pow(cos(Phi), 2.0) * (1.0 - t*t + N_2)/6.0;
	s		= pow(cos(Phi), 4.0) * (5.0 - 18.0 * t*t + pow(t, 4.0) + 14.0 * N_2 - 58.0 * t*t * N_2)/120.0;
	u		= pow(cos(Phi), 6.0) * (61.0 - 479.0 * t*t + 179.0 * pow(t, 4.0) - pow(t, 6.0))/5040.0;

	x		= RA * (1-EA2) * (G_A * Phi - G_B * sin(2.0*Phi) + G_C * sin(4.0*Phi) - G_D * sin(6.0*Phi));
	d_x		= pow(d_Ramda, 2.0) * R * sin(Phi) * cos(Phi) * (1.0 + p * pow(d_Ramda, 2.0) + q * pow(d_Ramda, 4.0))/2.0;
	y		= d_Ramda * R * cos(Phi) * (1.0 + r * pow(d_Ramda, 2.0) + s * pow(d_Ramda, 4.0) + u * pow(d_Ramda, 6.0));

	coord.x	= x + d_x;
	coord.y	= y + 500000 + axis * 1000000;
	return(&coord);
}

//------------------------------------------------------------
/**
	経緯度からガウス座標系のＸＹへ変換		2000.9.19
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXYGs	(	int			axis,	///< 該当する座標系の番号
				double		lon,	///< 経度
				double		lat,	///< 緯度
				double		*x,		///< ガウス座標系のＸ
				double		*y)		///< ガウス座標系のＹ
{
	xy_coord	*xy;	// 19座標系のＸＹ

	xy	= LLtoXYGs(axis, lon, lat);
	*x	= xy->x;
	*y	= xy->y;
}

//------------------------------------------------------------
/**
	ガウス座標系のＸＹから経緯度へ変換
*/
//------------------------------------------------------------
void 	crd_cnv::
XYGstoLL	(	int			axis,		///< 該当する座標系の番号
				double		x,			///< 北方向の座標値
				double		y,			///< 東方向の座標値
				d_lonlat	*lonlat)	///< 10進で表した経緯度(ex:130.23432342....)
{
	double	Phi, d_Phi, Ramda0, Ramda, d_Ramda;
	double	Phi1, Phi0;
	double	Phi_n,	Phi_c;
	double	t;
	double	_p, _q, _r, _s, _u;
	double	N_2, N_4, R;

	Make_Gauss_tbl();	// ガウス座標系のテーブルを作成

	Phi0	= x / (RA * (1.0 - EA2) * G_A);
	y		= y -  500000.0 - axis * 1000000.0;

	Ramda0	= Rad_Origin_Gauss[axis - 2].lon;

	Phi_c	= Phi0;

	// 垂足緯度を求める
	while(1)
	{
		Phi_n	= Phi0 + (sin(2.0*Phi_c) * G_B - sin(4.0*Phi_c) * G_C + sin(6.0*Phi_c) * G_D) / G_A;

		if(fabs(Phi_n - Phi_c) < 1.0e-12)
		{
			break;
		}
		Phi_c	= Phi_n;
	}
	Phi1	= Phi_n;

	t		= tan(Phi1);

	N_2		= EB2 * pow(cos(Phi1), 2.0);					// n*n
	N_4		= pow(N_2, 2.0);								// N_2*N_2
	R		= RA / sqrt(1.0 - EA2 * pow(sin(Phi1), 2.0));	// R = a
	t		= tan(Phi1);

	_p		= (5.0 + 3.0 * t*t + N_2 - 9.0 * t*t * N_4) /
				(12.0*pow(R, 2.0));
	_q		= (61.0 + 90.0 * t*t + 45.0 * pow(t, 4.0) + 107.0 * N_2 - 162.0 * t*t * N_2 -45.0 * pow(t, 4.0) * N_2) /
				(360.0*pow(R, 4.0));
	_r		= (1.0 + 2.0 * t*t + N_2) / (6.0*pow(R, 2.0));
	_s		= (5.0 + 28.0 * t*t + 24.0 * pow(t, 4.0) + 6.0 * N_2 + 8.0 * t*t * N_2) / (120.0*pow(R, 4.0));
	_u		= (61.0 + 662.0 * t*t + 1320.0 * pow(t, 4.0) + 720.0 * pow(t, 6.0)) / (5040.0*pow(R, 6.0));

	d_Phi	= y*y * t * (1.0 + N_2) * (1.0 - _p * y*y + _q * pow(y, 4.0)) / (2.0 * R*R);
	d_Ramda	= y * (1.0 - _r * y*y + _s * pow(y, 4.0) - _u * pow(y, 6.0)) / (R * cos(Phi1));

	Phi		= Phi1 - d_Phi;

	Ramda	= Ramda0 + d_Ramda;

	lonlat->lat	= Phi	/ (M_PI/180.0);
	lonlat->lon = Ramda	/ (M_PI/180.0);
}

//------------------------------------------------------------
/**
	ガウス座標系のＸＹから経緯度へ変換 \n
*/
//------------------------------------------------------------
void 	crd_cnv::
XYGstoLL	(	int			axis,	///< 該当する座標系の番号
				double		X,		///< 北方向の座標値
				double		Y,		///< 東方向の座標値
				double		*lon,	///< 経度
				double		*lat)	///< 緯度
{
	d_lonlat	lonlat;

	XYGstoLL(axis, X, Y, &lonlat);

	*lon	= lonlat.lon;
	*lat	= lonlat.lat;
}

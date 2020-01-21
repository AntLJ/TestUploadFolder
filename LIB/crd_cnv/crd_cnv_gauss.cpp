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

// �h�C�c�̃K�E�X�E�N�����[�Q�����W�n�p
#include "crd_cnv.h"
#include "crd_cnv_defines.h"

//------------------------------------------------------------
/**
	�K�E�X���W�n�̕\����郁���o�֐�
*/
//------------------------------------------------------------
void	crd_cnv::
Make_Gauss_tbl	(	void	)
{
	d_lonlat	origin_gauss[3] = 
					{{6.0,	0.0},	// �]�[���Q
					 {9.0,	0.0},	// �]�[���R
					 {12.0,	0.0}};	// �]�[���S

	// �K�E�X���W���_�̃��W�A���̕\���쐬����
	int	i;
	for (i = 0; i < 3; i++)
	{
		Rad_Origin_Gauss[i].lat	= (origin_gauss[i].lat) * M_PI / 180.0;
		Rad_Origin_Gauss[i].lon	= (origin_gauss[i].lon) * M_PI / 180.0;
	}
}	


//------------------------------------------------------------
/**
	�o�ܓx����K�E�X���W�n�̂w�x�֕ϊ�
	@return	�\���� xy_coord �ւ̃|�C���^
*/
//------------------------------------------------------------
xy_coord*	crd_cnv::
LLtoXYGs	(	int			axis,	///< �Y������]�[���̔ԍ�
				double		lon,	///< �o�x
				double		lat)	///< �ܓx
{

	double			R;

	double			d_Ramda,	t;
	double			N_2,		N_4;
	double			Phi;
	double			p, q, r, s, u;
	double			x, y, d_x;
	static xy_coord	coord;

	Make_Gauss_tbl();			// �K�E�X���W�n�̃]�[���e�[�u�����쐬

	d_Ramda	= lon * M_PI / 180.0 - Rad_Origin_Gauss[axis - 2].lon;
	Phi		= lat * M_PI / 180.0;	// lat = ��

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
	�o�ܓx����K�E�X���W�n�̂w�x�֕ϊ�		2000.9.19
*/
//------------------------------------------------------------
void	crd_cnv::
LLtoXYGs	(	int			axis,	///< �Y��������W�n�̔ԍ�
				double		lon,	///< �o�x
				double		lat,	///< �ܓx
				double		*x,		///< �K�E�X���W�n�̂w
				double		*y)		///< �K�E�X���W�n�̂x
{
	xy_coord	*xy;	// 19���W�n�̂w�x

	xy	= LLtoXYGs(axis, lon, lat);
	*x	= xy->x;
	*y	= xy->y;
}

//------------------------------------------------------------
/**
	�K�E�X���W�n�̂w�x����o�ܓx�֕ϊ�
*/
//------------------------------------------------------------
void 	crd_cnv::
XYGstoLL	(	int			axis,		///< �Y��������W�n�̔ԍ�
				double		x,			///< �k�����̍��W�l
				double		y,			///< �������̍��W�l
				d_lonlat	*lonlat)	///< 10�i�ŕ\�����o�ܓx(ex:130.23432342....)
{
	double	Phi, d_Phi, Ramda0, Ramda, d_Ramda;
	double	Phi1, Phi0;
	double	Phi_n,	Phi_c;
	double	t;
	double	_p, _q, _r, _s, _u;
	double	N_2, N_4, R;

	Make_Gauss_tbl();	// �K�E�X���W�n�̃e�[�u�����쐬

	Phi0	= x / (RA * (1.0 - EA2) * G_A);
	y		= y -  500000.0 - axis * 1000000.0;

	Ramda0	= Rad_Origin_Gauss[axis - 2].lon;

	Phi_c	= Phi0;

	// �����ܓx�����߂�
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
	�K�E�X���W�n�̂w�x����o�ܓx�֕ϊ� \n
*/
//------------------------------------------------------------
void 	crd_cnv::
XYGstoLL	(	int			axis,	///< �Y��������W�n�̔ԍ�
				double		X,		///< �k�����̍��W�l
				double		Y,		///< �������̍��W�l
				double		*lon,	///< �o�x
				double		*lat)	///< �ܓx
{
	d_lonlat	lonlat;

	XYGstoLL(axis, X, Y, &lonlat);

	*lon	= lonlat.lon;
	*lat	= lonlat.lat;
}

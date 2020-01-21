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

#include "crd_cnv.h"
#include "crd_cnv_defines.h"


//---------------------------------------------------------------------
//               UTM �ϊ��֘A
//---------------------------------------------------------------------

//---------------------------------------------------------------------
/*
	���o����AUTM���W�̃]�[���ԍ��𓾂� \n
	�����E�l���ǂ����邩��������Ɩ�� \n
	  2001.08.28
	@return  �]�[���ԍ�: �Ώ۔͈͊O \n
	@return  -1 : �Ώ۔͈͊O
*/
//---------------------------------------------------------------------
int		crd_cnv::
GetUTMZone	(	double	c_lon,			// ���o?�x
				double	c_lat,			// �ܓx(�_�~�[)
				double	*c_orig_lon,	// ���_�o�x
				double	*c_orig_lat	)	// ���_�ܓx
{
	int		zone	= -1;
	if(!(-180.0 <= c_lon && c_lon < 180.0)) {
		*c_orig_lon		= 0.0;
		*c_orig_lat		= 0.0;
		return	(zone);	// �ΏۊO
	}
	if(-180.0 <= c_lon && c_lon < 0.0) {	// ���o180�x�`0�x
		zone		= (180.0 + c_lon)/ 6 + 1;
		*c_orig_lon	= -180.0 + (zone * 6.0) - 3.0;
	}else
	if(0.0 <= c_lon && c_lon < 180.0) {		// ���o0�x����180�x
		zone		= 31 + c_lon / 6;
		*c_orig_lon	= ((zone - 30) * 6.0) - 3.0;	// 2009.05.25 �C��
	}
	// �ܓx�̒l�͏��0�x
	*c_orig_lat	= 0.0;
	return	(zone);
}

//---------------------------------------------------------------------
/*
	UTM�̃]�[���ԍ����猴�_�o�ܓx�𓾂� \n
	�����E�l���ǂ����邩��������Ɩ�� \n
	  2001.08.28
	@retval	0	�]�[���ԍ�����������
	@retval	-1	�]�[���ԍ���������Ȃ�
*/
//---------------------------------------------------------------------
int		crd_cnv::
GetUTMOrigLL	(	int		c_zone,			// �]�[���ԍ�
					double	*c_orig_lon,	// ���_�o�x
					double	*c_orig_lat	)	// ���_�ܓx
{
	*c_orig_lon	= 0.0;
	*c_orig_lat	= 0.0;

	if(1 <= c_zone && c_zone <= 30) {
		*c_orig_lon	= -180.0 + (c_zone * 6.0) - 3.0;
	}else
	if(30 < c_zone && c_zone <= 59) {
		*c_orig_lon	= ((c_zone - 30) * 6.0) - 3.0;
	}else {
		return	(-1);
	}
	return	(0);
}

//---------------------------------------------------------------------
/*
	UTM���W�n�̂w�x����o�ܓx(Bessel)�֕ϊ� \n
	2001.08.28
	@retval	0	����
	@retval	-1 	���s
*/
//---------------------------------------------------------------------
int 	crd_cnv::
UTMtoLL	(	int		c_zone,		// UTM�̃]�[���ԍ�
			double	x,			// �k�����̍��W�l
			double	y,			// �������̍��W�l
			double	*c_lon,		// �o�x(decimal)
			double	*c_lat)		// �ܓx(decimal)
{
	return UTMtoLL_Core( RA, EA2, EB2, c_zone, x, y, c_lon, c_lat ); // Bessel�ȉ~��
}

//---------------------------------------------------------------------
/*
	UTM���W�n�̂w�x����o�ܓx(WGS84)�֕ϊ� \n
	2001.08.28
	@retval	0	����
	@retval	-1 	���s
*/
//---------------------------------------------------------------------
int 	crd_cnv::
UTMtoLL_WGS84	(
		    int		c_zone,		// UTM�̃]�[���ԍ�
			double	x,			// �k�����̍��W�l
			double	y,			// �������̍��W�l
			double	*c_lon,		// �o�x(decimal)
			double	*c_lat)		// �ܓx(decimal)
{
	return UTMtoLL_Core( WGS84_RA, WGS84_EA2, WGS84_EB2, c_zone, x, y, c_lon, c_lat ); // WGS84
}

//---------------------------------------------------------------------
/*
	UTM���W�n�̂w�x����o�ܓx�֕ϊ� \n
	2001.08.28
	@retval	0	����
	@retval	-1 	���s
*/
//---------------------------------------------------------------------
int 	crd_cnv::
UTMtoLL_Core(
			 double ra,			// �n���ȉ~�̂̒����a�i���[�g���j
			 double ea2,		// ��ꗣ�S����2��
			 double eb2,		// ��񗣐S����2��
			 int	c_zone,		// UTM�̃]�[���ԍ�
			 double	x,			// �k�����̍��W�l
			 double	y,			// �������̍��W�l
			 double	*c_lon,		// �o�x(decimal)
			 double	*c_lat)		// �ܓx(decimal)
{
	double	Rad_Phi,	Rad_Ramda0,	Rad_Ramda;
	double	e_d2/*,	f*/;
	double	Rad_Phi1,	e2,	Rad_Phi0;
	double	Rad_Phi_n,	Rad_Phi_c,	Gunma2;
	double	t1,	a1,	p;
	double	UTM_M0	= 0.9996;

	double	tmp_lon	= 0.0;
	double	tmp_lat	= 0.0;

	y	-= 500000.0;	// 2001.08.29 UTM�́AY�����ɃI�t�Z�b�g 500000.0

	if(GetUTMOrigLL(c_zone, &tmp_lon, &tmp_lat) == -1) {
		return	(-1);
	}

	 // ���_�o�ܓx�̎��o��
	Rad_Phi0		= tmp_lat * M_PI / 180.0;

	Rad_Ramda0		= tmp_lon * M_PI / 180.0;

//	f				= 1.0 / 299.152813;

	e2				= ea2;				// ��P���S��
	e_d2			= eb2;				// ��Q���S��

	Rad_Phi_c		= Rad_Phi0;

	// �����ܓx�����߂�
	while(1)
	{
		Rad_Phi_n	= Rad_Phi_c
					- ((M_Arc_Len_Core(ra, e2, Rad_Phi_c) - M_Arc_Len_Core(ra, e2, Rad_Phi0) - (x / UTM_M0)) / ra * (1.0 - e2))
					* pow((1.0 - e2 * pow(sin(Rad_Phi_c), 2.0)), 1.5);

		if(fabs(Rad_Phi_n - Rad_Phi_c) < 1.0e-12)
		{
			break;
		}
		Rad_Phi_c	= Rad_Phi_n;
	}
	Rad_Phi1	= Rad_Phi_n;

	t1		= tan(Rad_Phi1);
	Gunma2	= e_d2 * pow(cos(Rad_Phi1), 2.0);
	a1		= ra / sqrt(1.0 - e2 * pow(sin(Rad_Phi1), 2.0));
	p		= y / UTM_M0;

	double p2, p4, t2, t4, a2, a4;
	p2		= pow(p,	2.0);
	p4		= pow(p,	4.0);
	t2		= pow(t1,	2.0);
	t4		= pow(t1,	4.0);
	a2		= pow(a1,	2.0);
	a4		= pow(a1,	4.0);

	Rad_Phi		= Rad_Phi1
				- (p2 * (1.0 + Gunma2) * t1) / (2.0 * a2)
				* (1.0 - p2 / (12.0 * a2)
				* (5.0 + 3.0 * t2 + Gunma2 - 9.0 * t2 * Gunma2)
				+ p4 / (360.0 * a4) 
				* (61.0 + 90.0 * t2 + 45.0 * t4));

	Rad_Ramda	= Rad_Ramda0
				+ p / (a1 * cos(Rad_Phi1))
				* (1.0 - p2 / (6.0 * a2)
				* (1.0 + 2.0 * t2 + Gunma2)
				+ p4 / (120.0 * a4)
				* (5.0 + 28.0 * t2 + 24.0 * t4));

	*c_lat	= Rad_Phi	/ (M_PI/180.0);
	*c_lon	= Rad_Ramda	/ (M_PI/180.0);
	return	(0);
}

//---------------------------------------------------------------------
/*
	�o�ܓx(Bessel)����UTM���W�ɂ���
	@retval	0	����
	@retval	-1 	���s
*/
//---------------------------------------------------------------------
int		crd_cnv::
LLtoUTM(	
			 int	c_zone,		// �]�[���ԍ�
			 double	lon,		// �o�x
			 double	lat,		// �ܓx
			 double	*c_x,		// UTM���W�n��X���W�l
			 double	*c_y)		// UTM���W�n��Y���W�l
{
	return LLtoUTM_Core( RA, EA2, EB2, c_zone, lon, lat, c_x, c_y );
}

//---------------------------------------------------------------------
/*
	�o�ܓx(WGS84)����UTM���W�ɂ���
	@retval	0	����
	@retval	-1 	���s
*/
//---------------------------------------------------------------------
int		crd_cnv::
LLtoUTM_WGS84(	
			 int	c_zone,		// �]�[���ԍ�
			 double	lon,		// �o�x
			 double	lat,		// �ܓx
			 double	*c_x,		// UTM���W�n��X���W�l
			 double	*c_y)		// UTM���W�n��Y���W�l
{
	return LLtoUTM_Core( WGS84_RA, WGS84_EA2, WGS84_EB2, c_zone, lon, lat, c_x, c_y );
}

//---------------------------------------------------------------------
/*
	�o�ܓx����UTM���W�ɂ���
	@retval	0	����
	@retval	-1 	���s
*/
//---------------------------------------------------------------------
int		crd_cnv::
LLtoUTM_Core(	
			 double ra,			// �n���ȉ~�̂̒����a�i���[�g���j
			 double ea2,		// ��ꗣ�S����2��
			 double eb2,		// ��񗣐S����2��
			 int	c_zone,		// �]�[���ԍ�
			 double	lon,		// �o�x
			 double	lat,		// �ܓx
			 double	*c_x,		// UTM���W�n��X���W�l
			 double	*c_y)		// UTM���W�n��Y���W�l
{
	double			d_lon,	Tau;
	double			Eta2,	Eta4;
	double			N,		Phi;
	double			DPhi,	t;
	double			x,		y;
	double			tmp_lon	= 0.0;
	double			tmp_lat	= 0.0;
	double			pi_lon	= 0.0;
	double			pi_lat	= 0.0;
	double	UTM_M0	= 0.9996;

	if(GetUTMOrigLL(c_zone, &tmp_lon, &tmp_lat) == -1) {
		return	(-1);
	}

	pi_lon	= tmp_lon * M_PI / 180.0;
	pi_lat	= tmp_lat * M_PI / 180.0;

	d_lon	= lon * M_PI / 180.0 - pi_lon;
	lat		= lat * M_PI / 180.0;

	Tau		= tan(lat);
	Eta2	= eb2 * pow(cos(lat), 2.0);
	Eta4	= pow(Eta2, 2.0);
	N		= ra / sqrt(1.0 - ea2 * pow(sin(lat), 2.0));
	Phi		= pi_lat;
	DPhi	= lat - pi_lat;
	t		= tan(lat);

	x		= M_Arc_Len_Core(ra, ea2, lat) - M_Arc_Len_Core(ra, ea2, pi_lat)
	  		+ N * pow(d_lon, 2.0) * sin(lat) * cos(lat) / 2.0
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((5.0 - pow(t, 2.0) + 9.0 * Eta2 + 4.0 * Eta4) / 12.0
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (61.0 - 58.0 * pow(t, 2.0) + pow(t, 4.0) + 270.0 * Eta2
			- 330.0 * pow(t, 2.0) * Eta2) / 30.0));

	y		= N * d_lon * cos(lat)
			* (1.0 + pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* ((1.0 - pow(t, 2.0) + Eta2)
			+ pow(d_lon, 2.0) * pow(cos(lat), 2.0)
			* (5.0 - 18.0 * pow(t, 2.0) + pow(t, 4.0)
			+ 14.0 * Eta2 -58.0 * pow(t, 2.0) * Eta2) / 20.0) / 6.0);

	*c_x		= x * UTM_M0;
	*c_y		= y * UTM_M0 + 500000.0;	// 2001.08.29 UTM�́AY�����ɃI�t�Z�b�g 500000.0
	return(0);
}

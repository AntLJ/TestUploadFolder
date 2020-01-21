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

//-2001.2.28----------------------------------------------------------
/**
	���b�V�� + (X,Y)����o�ܓx�֕ϊ�(EU������)
	@retval	0	����
	@retval	-1	���s
*/
//--------------------------------------------------------------------
int	crd_cnv::
MeshtoLL_EU(	unsigned int	meshcode,	///< ���b�V���R�[�h
				int				x,			///< ���K�����W�w
				int				y,			///< ���K�����W�x
				double			*lon,		///< �o�x
				double			*lat)		///< �ܓx
{
	int	a_iRet	= -1;

	// �����[���b�p
	a_iRet	= MeshtoLL2_Core	( meshcode, (double)x, (double)y, lon, lat, ORIGIN_EU.lon, ORIGIN_EU.lat );
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
	@retval	0	����
	@retval	-1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_EU(	double			lon,		///< �o�x
				double			lat,		///< �ܓx
				int				level,		///< ���b�V���̃��x�� (1,2,64)
				unsigned int	*meshcode,	///< ���b�V���R�[�h
				int				*x,			///< ���K���w
				int				*y,			///< ���K���x
				int				no_mill	)	///< XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, meshcode, x, y, ORIGIN_EU.lon, ORIGIN_EU.lat, no_mill );
	
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_EU2	(	double			c_lon,			///< �o�x
					double			c_lat,			///< �ܓx
					unsigned int	c_meshcode,		///< ���b�V���R�[�h
					double			*c_x,			///< ���K���w
					double			*c_y	)		///< ���K���x
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_EU.lon, ORIGIN_EU.lat );

	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_EU2	(	double			c_lon,			///< �o�x
					double			c_lat,			///< �ܓx
					unsigned int	c_meshcode,		///< ���b�V���R�[�h
					int				*c_x,			///< ���K���w
					int				*c_y	)		///< ���K���x
{
	double	tmp_x	= 0.0;
	double	tmp_y	= 0.0;
	if( LLtoMesh_EU2 ( c_lon, c_lat, c_meshcode, &tmp_x, &tmp_y ) == -1) {
		return	(-1);
	}
	*c_x	= tmp_x + 0.5;
	*c_y	= tmp_y + 0.5;
	return	(0);
}

//-2001.2.28----------------------------------------------------------
/**
	���b�V�� + (X,Y)����o�ܓx�֕ϊ�(NA������)
	@retval	0	����
	@retval	-1	���s
*/
//--------------------------------------------------------------------
int	crd_cnv::
MeshtoLL_NA(	unsigned int	meshcode,	///< ���b�V���R�[�h
				int				x,			///< ���K�����W�w
				int				y,			///< ���K�����W�x
				double			*lon,		///< �o�x
				double			*lat)		///< �ܓx
{
	int	a_iRet	= -1;

	// �k�A�����J
	a_iRet	= MeshtoLL2_Core	( meshcode, (double)x, (double)y, lon, lat, ORIGIN_NA.lon, ORIGIN_NA.lat );
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
	@retval	0	����
	@retval	-1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_NA(	double			lon,		///< �o�x
				double			lat,		///< �ܓx
				int				level,		///< ���b�V���̃��x�� (1,2,64)
				unsigned int	*meshcode,	///< ���b�V���R�[�h
				int				*x,			///< ���K���w
				int				*y,			///< ���K���x
				int				no_mill	)	///< XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, meshcode, x, y, ORIGIN_NA.lon, ORIGIN_NA.lat, no_mill );
	
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_NA2(	double			c_lon,			///< �o�x
				double			c_lat,			///< �ܓx
				unsigned int	c_meshcode,		///< ���b�V���R�[�h
				double			*c_x,			///< ���K���w
				double			*c_y	)		///< ���K���x
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core ( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_NA.lon, ORIGIN_NA.lat );

	return ( a_iRet );
}

/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
*/
int	crd_cnv::
LLtoMesh_NA2(	double			c_lon,			///< �o�x
				double			c_lat,			///< �ܓx
				unsigned int	c_meshcode,		///< ���b�V���R�[�h
				int				*c_x,			///< ���K���w
				int				*c_y	)		///< ���K���x
{
	double	tmp_x	= 0.0;
	double	tmp_y	= 0.0;
	if( LLtoMesh_NA2 ( c_lon, c_lat, c_meshcode, &tmp_x, &tmp_y ) == -1) {
		return	(-1);
	}
	*c_x	= tmp_x + 0.5;
	*c_y	= tmp_y + 0.5;
	return	(0);
}

//-2005.12.22---------------------------------------------------------
/**
	���b�V�� + (X,Y)����o�ܓx�֕ϊ�(�A���X�J�E�n���C��)
	@retval	0	����
	@retval	-1	���s
*/
//--------------------------------------------------------------------
int	crd_cnv::
MeshtoLL_AH(	unsigned int	meshcode,	///< ���b�V���R�[�h
				int				x,			///< ���K�����W�w
				int				y,			///< ���K�����W�x
				double			*lon,		///< �o�x
				double			*lat)		///< �ܓx
{
	int	a_iRet	= -1;

	// �A���X�J�E�n���C
	a_iRet	= MeshtoLL2_Core	( meshcode, (double)x, (double)y, lon, lat, ORIGIN_AH.lon, ORIGIN_AH.lat );
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂� (�A���X�J�E�n���C��)
	@retval	0	����
	@retval	-1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_AH(	double			lon,		///< �o�x
				double			lat,		///< �ܓx
				int				level,		///< ���b�V���̃��x�� (1,2,64)
				unsigned int	*meshcode,	///< ���b�V���R�[�h
				int				*x,			///< ���K���w
				int				*y,			///< ���K���x
				int				no_mill	)	///< XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, meshcode, x, y, ORIGIN_AH.lon, ORIGIN_AH.lat, no_mill );
	
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_AH2(	double			c_lon,			///< �o�x
				double			c_lat,			///< �ܓx
				unsigned int	c_meshcode,		///< ���b�V���R�[�h
				double			*c_x,			///< ���K���w
				double			*c_y	)		///< ���K���x
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_AH.lon, ORIGIN_AH.lat );

	return ( a_iRet );
}

/**
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
*/
int	crd_cnv::
LLtoMesh_AH2(	double			c_lon,			///< �o�x
				double			c_lat,			///< �ܓx
				unsigned int	c_meshcode,		///< ���b�V���R�[�h
				int				*c_x,			///< ���K���w
				int				*c_y	)		///< ���K���x
{
	double	tmp_x	= 0.0;
	double	tmp_y	= 0.0;
	if( LLtoMesh_AH2 ( c_lon, c_lat, c_meshcode, &tmp_x, &tmp_y ) == -1) {
		return	(-1);
	}
	*c_x	= tmp_x + 0.5;
	*c_y	= tmp_y + 0.5;
	return	(0);
}

/*------------------------------------------------------------------
// �������牺�̃f�[�^�́A����̍��ۓs�s�n�}�Ή��̂��߂ƁA�@�\�̓���
// ��ړI�Ƃ��ĐV�K�ɂ����������́B����͂�����̋@�\���C�����Ă���
// �`�őΉ� 2006.09.06
------------------------------------------------------------------*/

/** ------------------------------------------------------------------
	���b�V�� + (X,Y)����o�ܓx�֕ϊ�(���E���ʔ�(���{��)) 2006.09.06
	@retval	0	����
	@retval	-1	���s
--------------------------------------------------------------------*/
int	crd_cnv::
MeshtoLL_World(	unsigned int	c_meshcode,		///< ���b�V���R�[�h
				int				c_x,			///< ���K�����W�w
				int				c_y,			///< ���K�����W�x
				int				c_level,		///< ���b�V���̃��x�� (1,2,64)
				int				c_origin_code,	///< ���_�R�[�h
				double			*c_lon,			///< �o�x
				double			*c_lat	)		///< �ܓx
{
	char	a_cMeshStr[18];
	char	a_cTmpStr[18];

	memset	( a_cMeshStr, '\0', 18 );
	memset	( a_cTmpStr, '\0', 18 );

	// ���̂܂ܕϊ������Ƃ��̃��b�V���R�[�h�̌������m�F
	sprintf	( a_cTmpStr, "%.2u", c_meshcode );	// 2006.10.28 �g�b�v�}�b�v�̂��ߏC��

	// c_level �ɂ���āA���b�V���R�[�h�������Ƃ��Ĉ��������߂�
	switch (c_level)
	{
	case	0:
		if( strlen( a_cTmpStr ) > 2 ) return ( -1 );
		sprintf	( a_cMeshStr, "%.2d", c_meshcode );
		break;
	case	1:
		if( SJ_ORIGIN == c_origin_code ) {
			if( strlen( a_cTmpStr ) > 6 ) return ( -1 ); // 2011.12.08 �V���ۓs�s�n�}�Ή�
			sprintf	( a_cMeshStr, "%.6u", c_meshcode );
		}
		else {
			if( strlen( a_cTmpStr ) > 4 ) return ( -1 );
			sprintf	( a_cMeshStr, "%.4d", c_meshcode );
		}
		break;
	case	2:
		if( SJ_ORIGIN == c_origin_code ) {
			if( strlen( a_cTmpStr ) > 8 ) return ( -1 ); // 2011.12.12 �V���ۓs�s�n�}�Ή�
			sprintf	( a_cMeshStr, "%.8u", c_meshcode );
		}
		else {
			if( strlen( a_cTmpStr ) > 6 ) return ( -1 );
			sprintf	( a_cMeshStr, "%.6d", c_meshcode );
		}
		break;
	case	64:
		if( SJ_ORIGIN == c_origin_code ) {
			if( strlen( a_cTmpStr ) > 10 ) return ( -1 ); // 2011.12.12 �V���ۓs�s�n�}�Ή�
			sprintf	( a_cMeshStr, "%.10u", c_meshcode );
		}
		else {
			if( strlen( a_cTmpStr ) > 8 ) return ( -1 );
			sprintf	( a_cMeshStr, "%.8d", c_meshcode );
		}
		break;
	default:
		return	( -1 );
	}

	int	a_iRet			= -1;

	switch ( c_origin_code )
	{
	case	JPN_ORIGIN:	// ���{
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_JPN.lon, ORIGIN_JPN.lat );
		break;
	case	EU_ORIGIN:	// �����[���b�p
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_EU.lon, ORIGIN_EU.lat );
		break;
	case	NA_ORIGIN:	// �k�A�����J
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_NA.lon, ORIGIN_NA.lat );
		break;
	case	AH_ORIGIN:	// �A���X�J�E�n���C
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_AH.lon, ORIGIN_AH.lat );
		break;
	case	SEAA_ORIGIN:// ����A�W�A�E�I�[�X�g�����A (�V�K�ǉ� 2006.09.07)
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_SEAA.lon, ORIGIN_SEAA.lat );
		break;
	case	SA_ORIGIN:	// ��A�����J (�V�K�ǉ� 2008.12.01)
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_SA.lon, ORIGIN_SA.lat );
		break;
	case	SJ_ORIGIN:	// �V���ۓs�s�n�}�i�V�K�ǉ� 2011.12.08�j
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_SJ.lon, ORIGIN_SJ.lat  );
		break;
	default:
		break;
	}	
	return	( a_iRet );
}

/** ------------------------------------------------------------------
    �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
	(���E���ʔ�(���{��)) 2006.09.08
    @retval 0   ����
    @retval -1  ���s
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh_World(		double			c_lon,			///< �o�x
					double			c_lat,			///< �ܓx
					int				c_level,		///< ���b�V���̃��x�� (1,2,64)
					int				c_origin_code,	///< ���_�R�[�h
					unsigned int	*c_meshcode,	///< ���b�V���R�[�h
					int				*c_x,			///< ���K���w
					int				*c_y,			///< ���K���x
					int				c_no_mill	)	///< XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06 
{
	int	a_iRet	= -1;

	switch ( c_origin_code )
	{
	case	JPN_ORIGIN:	// ���{
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_JPN.lon, ORIGIN_JPN.lat, c_no_mill );
		break;
	case	EU_ORIGIN:	// �����[���b�p
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_EU.lon, ORIGIN_EU.lat, c_no_mill  );
		break;
	case	NA_ORIGIN:	// �k�A�����J
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_NA.lon, ORIGIN_NA.lat, c_no_mill  );
		break;
	case	AH_ORIGIN:	// �A���X�J�E�n���C
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_AH.lon, ORIGIN_AH.lat, c_no_mill  );
		break;
	case	SEAA_ORIGIN:// ����A�W�A�E�I�[�X�g�����A (�V�K�ǉ� 2006.09.07)
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_SEAA.lon, ORIGIN_SEAA.lat, c_no_mill  );
		break;
	case	SA_ORIGIN:	// ��A�����J (�V�K�ǉ� 2008.12.01)
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_SA.lon, ORIGIN_SA.lat, c_no_mill  );
		break;
	case	SJ_ORIGIN:	// �V���ۓs�s�n�}�i�V�K�ǉ� 2011.12.08�j
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat,  c_level, c_meshcode, c_x, c_y, ORIGIN_SJ.lon, ORIGIN_SJ.lat, c_no_mill  );
		break;
	default:
		break;
	}
	return	( a_iRet );
}

/** ------------------------------------------------------------------
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	(���E���ʔ�(���{��)) 2006.09.06
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh2_World(	double			c_lon,			///< �o�x
					double			c_lat,			///< �ܓx
					unsigned int	c_meshcode,		///< ���b�V���R�[�h
					int				c_level,		///< ���b�V���̃��x�� (1,2,64)
					int				c_origin_code,	///< ���_�R�[�h
					double			*c_x,			///< ���K���w
					double			*c_y	)		///< ���K���x
{

	char	a_cMeshStr[18];
	char	a_cTmpStr[18];

	memset	( a_cMeshStr, '\0', 18 );
	memset	( a_cTmpStr, '\0', 18 );

	// ���̂܂ܕϊ������Ƃ��̃��b�V���R�[�h�̌������m�F
	sprintf	( a_cTmpStr, "%.2d", c_meshcode );	// 2006.10.28 �g�b�v�}�b�v�̂��ߏC��

	// c_level �ɂ���āA���b�V���R�[�h�������Ƃ��Ĉ��������߂�
	switch (c_level)
	{
	case	0:
		if( strlen( a_cTmpStr ) > 2 ) return ( -1 );
		sprintf	( a_cMeshStr, "%.2d", c_meshcode );
		break;
	case	1:
		if( SJ_ORIGIN == c_origin_code ){
			if( strlen( a_cTmpStr ) > 6 ) return ( -1 ); // 2011.12.08 �V���ۓs�s�n�}�Ή�
			sprintf	( a_cMeshStr, "%.6d", c_meshcode );
		}
		else {
			if( strlen( a_cTmpStr ) > 4 ) return ( -1 );
			sprintf	( a_cMeshStr, "%.4d", c_meshcode );
		}
		break;
	case	2:
		if( strlen( a_cTmpStr ) > 6 ) return ( -1 );
		sprintf	( a_cMeshStr, "%.6d", c_meshcode );
		break;
	case	64:
		if( strlen( a_cTmpStr ) > 8 ) return ( -1 );
		sprintf	( a_cMeshStr, "%.8d", c_meshcode );
		break;
	default:
		return	( -1 );
	}

	int	a_iRet	= -1;

	switch ( c_origin_code )
	{
	case	JPN_ORIGIN:	// ���{
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_JPN.lon, ORIGIN_JPN.lat );
		break;
	case	EU_ORIGIN:	// �����[���b�p
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_EU.lon, ORIGIN_EU.lat  );
		break;
	case	NA_ORIGIN:	// �k�A�����J
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_NA.lon, ORIGIN_NA.lat  );
		break;
	case	AH_ORIGIN:	// �A���X�J�E�n���C
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_AH.lon, ORIGIN_AH.lat  );
		break;
	case	SEAA_ORIGIN:// ����A�W�A�E�I�[�X�g�����A (�V�K�ǉ� 2006.09.07)
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat,  a_cMeshStr, c_x, c_y, ORIGIN_SEAA.lon, ORIGIN_SEAA.lat  );
		break;
	case	SA_ORIGIN:	// ����A�W�A�E�I�[�X�g�����A (�V�K�ǉ� 2008.12.01)
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat,  a_cMeshStr, c_x, c_y, ORIGIN_SA.lon, ORIGIN_SA.lat  );
		break;
	case SJ_ORIGIN:		// �V���ۓs�s�n�}�i�V�K�ǉ� 2011.12.08�j
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat,  a_cMeshStr, c_x, c_y, ORIGIN_SJ.lon, ORIGIN_SJ.lat  );
		break;
	default:
		break;
	}
	return	( a_iRet );
}

/** ------------------------------------------------------------------
	�o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
	(���E���ʔ�(���{��)) 2006.09.12
	�w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
	@retval	0	����
	@retval -1	���s
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh2_World(	double			c_lon,			///< �o�x
					double			c_lat,			///< �ܓx
					unsigned int	c_meshcode,		///< ���b�V���R�[�h
					int				c_level,		///< ���b�V���̃��x�� (1,2,64)
					int				c_origin_code,	///< ���_�R�[�h
					int				*c_x,			///< ���K���w
					int				*c_y	)		///< ���K���x
{
	double	tmp_x	= 0.0;
	double	tmp_y	= 0.0;
	if( LLtoMesh2_World ( c_lon, c_lat, c_meshcode, c_level, c_origin_code, &tmp_x, &tmp_y ) == -1) {
		return	(-1);
	}
	*c_x	= tmp_x + 0.5;
	*c_y	= tmp_y + 0.5;
	return	(0);
}

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
	メッシュ + (X,Y)から経緯度へ変換(EU諸国版)
	@retval	0	成功
	@retval	-1	失敗
*/
//--------------------------------------------------------------------
int	crd_cnv::
MeshtoLL_EU(	unsigned int	meshcode,	///< メッシュコード
				int				x,			///< 正規化座標Ｘ
				int				y,			///< 正規化座標Ｙ
				double			*lon,		///< 経度
				double			*lat)		///< 緯度
{
	int	a_iRet	= -1;

	// 東ヨーロッパ
	a_iRet	= MeshtoLL2_Core	( meshcode, (double)x, (double)y, lon, lat, ORIGIN_EU.lon, ORIGIN_EU.lat );
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める
	@retval	0	成功
	@retval	-1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_EU(	double			lon,		///< 経度
				double			lat,		///< 緯度
				int				level,		///< メッシュのレベル (1,2,64)
				unsigned int	*meshcode,	///< メッシュコード
				int				*x,			///< 正規化Ｘ
				int				*y,			///< 正規化Ｙ
				int				no_mill	)	///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, meshcode, x, y, ORIGIN_EU.lon, ORIGIN_EU.lat, no_mill );
	
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_EU2	(	double			c_lon,			///< 経度
					double			c_lat,			///< 緯度
					unsigned int	c_meshcode,		///< メッシュコード
					double			*c_x,			///< 正規化Ｘ
					double			*c_y	)		///< 正規化Ｙ
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_EU.lon, ORIGIN_EU.lat );

	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_EU2	(	double			c_lon,			///< 経度
					double			c_lat,			///< 緯度
					unsigned int	c_meshcode,		///< メッシュコード
					int				*c_x,			///< 正規化Ｘ
					int				*c_y	)		///< 正規化Ｙ
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
	メッシュ + (X,Y)から経緯度へ変換(NA諸国版)
	@retval	0	成功
	@retval	-1	失敗
*/
//--------------------------------------------------------------------
int	crd_cnv::
MeshtoLL_NA(	unsigned int	meshcode,	///< メッシュコード
				int				x,			///< 正規化座標Ｘ
				int				y,			///< 正規化座標Ｙ
				double			*lon,		///< 経度
				double			*lat)		///< 緯度
{
	int	a_iRet	= -1;

	// 北アメリカ
	a_iRet	= MeshtoLL2_Core	( meshcode, (double)x, (double)y, lon, lat, ORIGIN_NA.lon, ORIGIN_NA.lat );
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める
	@retval	0	成功
	@retval	-1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_NA(	double			lon,		///< 経度
				double			lat,		///< 緯度
				int				level,		///< メッシュのレベル (1,2,64)
				unsigned int	*meshcode,	///< メッシュコード
				int				*x,			///< 正規化Ｘ
				int				*y,			///< 正規化Ｙ
				int				no_mill	)	///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, meshcode, x, y, ORIGIN_NA.lon, ORIGIN_NA.lat, no_mill );
	
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_NA2(	double			c_lon,			///< 経度
				double			c_lat,			///< 緯度
				unsigned int	c_meshcode,		///< メッシュコード
				double			*c_x,			///< 正規化Ｘ
				double			*c_y	)		///< 正規化Ｙ
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core ( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_NA.lon, ORIGIN_NA.lat );

	return ( a_iRet );
}

/**
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
int	crd_cnv::
LLtoMesh_NA2(	double			c_lon,			///< 経度
				double			c_lat,			///< 緯度
				unsigned int	c_meshcode,		///< メッシュコード
				int				*c_x,			///< 正規化Ｘ
				int				*c_y	)		///< 正規化Ｙ
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
	メッシュ + (X,Y)から経緯度へ変換(アラスカ・ハワイ版)
	@retval	0	成功
	@retval	-1	失敗
*/
//--------------------------------------------------------------------
int	crd_cnv::
MeshtoLL_AH(	unsigned int	meshcode,	///< メッシュコード
				int				x,			///< 正規化座標Ｘ
				int				y,			///< 正規化座標Ｙ
				double			*lon,		///< 経度
				double			*lat)		///< 緯度
{
	int	a_iRet	= -1;

	// アラスカ・ハワイ
	a_iRet	= MeshtoLL2_Core	( meshcode, (double)x, (double)y, lon, lat, ORIGIN_AH.lon, ORIGIN_AH.lat );
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める (アラスカ・ハワイ版)
	@retval	0	成功
	@retval	-1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_AH(	double			lon,		///< 経度
				double			lat,		///< 緯度
				int				level,		///< メッシュのレベル (1,2,64)
				unsigned int	*meshcode,	///< メッシュコード
				int				*x,			///< 正規化Ｘ
				int				*y,			///< 正規化Ｙ
				int				no_mill	)	///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06
{
	int	a_iRet	= -1;

	a_iRet	= LLtoMesh_Core(	lon, lat, level, meshcode, x, y, ORIGIN_AH.lon, ORIGIN_AH.lat, no_mill );
	
	return ( a_iRet );
}

//---------------------------------------------------------------------
/**
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
//---------------------------------------------------------------------
int	crd_cnv::
LLtoMesh_AH2(	double			c_lon,			///< 経度
				double			c_lat,			///< 緯度
				unsigned int	c_meshcode,		///< メッシュコード
				double			*c_x,			///< 正規化Ｘ
				double			*c_y	)		///< 正規化Ｙ
{
	int	a_iRet	= -1;
	
	a_iRet	= LLtoMesh2_Core( c_lon, c_lat, c_meshcode, c_x, c_y, ORIGIN_AH.lon, ORIGIN_AH.lat );

	return ( a_iRet );
}

/**
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
*/
int	crd_cnv::
LLtoMesh_AH2(	double			c_lon,			///< 経度
				double			c_lat,			///< 緯度
				unsigned int	c_meshcode,		///< メッシュコード
				int				*c_x,			///< 正規化Ｘ
				int				*c_y	)		///< 正規化Ｙ
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
// ここから下のデータは、今後の国際都市地図対応のためと、機能の統一
// を目的として新規についかしたもの。今後はこちらの機能を修正していく
// 形で対応 2006.09.06
------------------------------------------------------------------*/

/** ------------------------------------------------------------------
	メッシュ + (X,Y)から経緯度へ変換(世界共通版(日本も)) 2006.09.06
	@retval	0	成功
	@retval	-1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
MeshtoLL_World(	unsigned int	c_meshcode,		///< メッシュコード
				int				c_x,			///< 正規化座標Ｘ
				int				c_y,			///< 正規化座標Ｙ
				int				c_level,		///< メッシュのレベル (1,2,64)
				int				c_origin_code,	///< 原点コード
				double			*c_lon,			///< 経度
				double			*c_lat	)		///< 緯度
{
	char	a_cMeshStr[18];
	char	a_cTmpStr[18];

	memset	( a_cMeshStr, '\0', 18 );
	memset	( a_cTmpStr, '\0', 18 );

	// そのまま変換したときのメッシュコードの桁数を確認
	sprintf	( a_cTmpStr, "%.2u", c_meshcode );	// 2006.10.28 トップマップのため修正

	// c_level によって、メッシュコードを何桁として扱うか決める
	switch (c_level)
	{
	case	0:
		if( strlen( a_cTmpStr ) > 2 ) return ( -1 );
		sprintf	( a_cMeshStr, "%.2d", c_meshcode );
		break;
	case	1:
		if( SJ_ORIGIN == c_origin_code ) {
			if( strlen( a_cTmpStr ) > 6 ) return ( -1 ); // 2011.12.08 新国際都市地図対応
			sprintf	( a_cMeshStr, "%.6u", c_meshcode );
		}
		else {
			if( strlen( a_cTmpStr ) > 4 ) return ( -1 );
			sprintf	( a_cMeshStr, "%.4d", c_meshcode );
		}
		break;
	case	2:
		if( SJ_ORIGIN == c_origin_code ) {
			if( strlen( a_cTmpStr ) > 8 ) return ( -1 ); // 2011.12.12 新国際都市地図対応
			sprintf	( a_cMeshStr, "%.8u", c_meshcode );
		}
		else {
			if( strlen( a_cTmpStr ) > 6 ) return ( -1 );
			sprintf	( a_cMeshStr, "%.6d", c_meshcode );
		}
		break;
	case	64:
		if( SJ_ORIGIN == c_origin_code ) {
			if( strlen( a_cTmpStr ) > 10 ) return ( -1 ); // 2011.12.12 新国際都市地図対応
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
	case	JPN_ORIGIN:	// 日本
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_JPN.lon, ORIGIN_JPN.lat );
		break;
	case	EU_ORIGIN:	// 東ヨーロッパ
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_EU.lon, ORIGIN_EU.lat );
		break;
	case	NA_ORIGIN:	// 北アメリカ
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_NA.lon, ORIGIN_NA.lat );
		break;
	case	AH_ORIGIN:	// アラスカ・ハワイ
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_AH.lon, ORIGIN_AH.lat );
		break;
	case	SEAA_ORIGIN:// 東南アジア・オーストラリア (新規追加 2006.09.07)
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_SEAA.lon, ORIGIN_SEAA.lat );
		break;
	case	SA_ORIGIN:	// 南アメリカ (新規追加 2008.12.01)
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_SA.lon, ORIGIN_SA.lat );
		break;
	case	SJ_ORIGIN:	// 新国際都市地図（新規追加 2011.12.08）
		a_iRet	= MeshtoLL2_Core	( a_cMeshStr, (double)c_x, (double)c_y, c_lon, c_lat, ORIGIN_SJ.lon, ORIGIN_SJ.lat  );
		break;
	default:
		break;
	}	
	return	( a_iRet );
}

/** ------------------------------------------------------------------
    経緯度から、メッシュコード + (x,y) を求める
	(世界共通版(日本も)) 2006.09.08
    @retval 0   成功
    @retval -1  失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh_World(		double			c_lon,			///< 経度
					double			c_lat,			///< 緯度
					int				c_level,		///< メッシュのレベル (1,2,64)
					int				c_origin_code,	///< 原点コード
					unsigned int	*c_meshcode,	///< メッシュコード
					int				*c_x,			///< 正規化Ｘ
					int				*c_y,			///< 正規化Ｙ
					int				c_no_mill	)	///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す）2006.09.06 
{
	int	a_iRet	= -1;

	switch ( c_origin_code )
	{
	case	JPN_ORIGIN:	// 日本
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_JPN.lon, ORIGIN_JPN.lat, c_no_mill );
		break;
	case	EU_ORIGIN:	// 東ヨーロッパ
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_EU.lon, ORIGIN_EU.lat, c_no_mill  );
		break;
	case	NA_ORIGIN:	// 北アメリカ
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_NA.lon, ORIGIN_NA.lat, c_no_mill  );
		break;
	case	AH_ORIGIN:	// アラスカ・ハワイ
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_AH.lon, ORIGIN_AH.lat, c_no_mill  );
		break;
	case	SEAA_ORIGIN:// 東南アジア・オーストラリア (新規追加 2006.09.07)
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_SEAA.lon, ORIGIN_SEAA.lat, c_no_mill  );
		break;
	case	SA_ORIGIN:	// 南アメリカ (新規追加 2008.12.01)
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat, c_level, c_meshcode, c_x, c_y, ORIGIN_SA.lon, ORIGIN_SA.lat, c_no_mill  );
		break;
	case	SJ_ORIGIN:	// 新国際都市地図（新規追加 2011.12.08）
		a_iRet	= LLtoMesh_Core	( c_lon, c_lat,  c_level, c_meshcode, c_x, c_y, ORIGIN_SJ.lon, ORIGIN_SJ.lat, c_no_mill  );
		break;
	default:
		break;
	}
	return	( a_iRet );
}

/** ------------------------------------------------------------------
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	(世界共通版(日本も)) 2006.09.06
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh2_World(	double			c_lon,			///< 経度
					double			c_lat,			///< 緯度
					unsigned int	c_meshcode,		///< メッシュコード
					int				c_level,		///< メッシュのレベル (1,2,64)
					int				c_origin_code,	///< 原点コード
					double			*c_x,			///< 正規化Ｘ
					double			*c_y	)		///< 正規化Ｙ
{

	char	a_cMeshStr[18];
	char	a_cTmpStr[18];

	memset	( a_cMeshStr, '\0', 18 );
	memset	( a_cTmpStr, '\0', 18 );

	// そのまま変換したときのメッシュコードの桁数を確認
	sprintf	( a_cTmpStr, "%.2d", c_meshcode );	// 2006.10.28 トップマップのため修正

	// c_level によって、メッシュコードを何桁として扱うか決める
	switch (c_level)
	{
	case	0:
		if( strlen( a_cTmpStr ) > 2 ) return ( -1 );
		sprintf	( a_cMeshStr, "%.2d", c_meshcode );
		break;
	case	1:
		if( SJ_ORIGIN == c_origin_code ){
			if( strlen( a_cTmpStr ) > 6 ) return ( -1 ); // 2011.12.08 新国際都市地図対応
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
	case	JPN_ORIGIN:	// 日本
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_JPN.lon, ORIGIN_JPN.lat );
		break;
	case	EU_ORIGIN:	// 東ヨーロッパ
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_EU.lon, ORIGIN_EU.lat  );
		break;
	case	NA_ORIGIN:	// 北アメリカ
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_NA.lon, ORIGIN_NA.lat  );
		break;
	case	AH_ORIGIN:	// アラスカ・ハワイ
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat, a_cMeshStr, c_x, c_y, ORIGIN_AH.lon, ORIGIN_AH.lat  );
		break;
	case	SEAA_ORIGIN:// 東南アジア・オーストラリア (新規追加 2006.09.07)
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat,  a_cMeshStr, c_x, c_y, ORIGIN_SEAA.lon, ORIGIN_SEAA.lat  );
		break;
	case	SA_ORIGIN:	// 東南アジア・オーストラリア (新規追加 2008.12.01)
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat,  a_cMeshStr, c_x, c_y, ORIGIN_SA.lon, ORIGIN_SA.lat  );
		break;
	case SJ_ORIGIN:		// 新国際都市地図（新規追加 2011.12.08）
		a_iRet	= LLtoMesh2_Core	( c_lon, c_lat,  a_cMeshStr, c_x, c_y, ORIGIN_SJ.lon, ORIGIN_SJ.lat  );
		break;
	default:
		break;
	}
	return	( a_iRet );
}

/** ------------------------------------------------------------------
	経緯度から、メッシュコード + (x,y) を求める(メッシュ指定版)
	(世界共通版(日本も)) 2006.09.12
	指定したメッシュコードにたいして、正規化座標値を求める
	@retval	0	成功
	@retval -1	失敗
--------------------------------------------------------------------*/
int	crd_cnv::
LLtoMesh2_World(	double			c_lon,			///< 経度
					double			c_lat,			///< 緯度
					unsigned int	c_meshcode,		///< メッシュコード
					int				c_level,		///< メッシュのレベル (1,2,64)
					int				c_origin_code,	///< 原点コード
					int				*c_x,			///< 正規化Ｘ
					int				*c_y	)		///< 正規化Ｙ
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

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

//---------------------------------------------------------------------
// IPCのTokyo97エリアのメッシュかどうかを調べる
//	@return	Tokyo97エリア以外 == false
//	@return	Tokyo97エリア == true
//---------------------------------------------------------------------
bool	crd_cnv::
Is_IPC_Tokyo97_Area(	int		c_iMeshCode	)	///< 2次メッシュコード
{
	std::set<int, std::less<int> >::iterator	a_iIndx;
	a_iIndx	= m_siTokyo97_Area.find ( c_iMeshCode );	// 2次メッシュコードを入れて確認
	if( a_iIndx != m_siTokyo97_Area.end() ) {
		return	( true );
	}
	return	( false );
}

//---------------------------------------------------------------------
// JGD2000パラメータファイルを読み込み、セットの構造に落とす
//	@return	bool
//---------------------------------------------------------------------
bool	crd_cnv::
JGD2000_RP_Init( char*			c_fname )			///< 読み込みファイル名
{
	const int	LINEMAX	= 512;
	FILE	*rfp	= NULL;
	char	r_buf[LINEMAX];
	int		max_field	= 0;	// 2000.11.30

	// ファイルオープン
	rfp		= fopen(c_fname, "r");
	if(rfp == NULL)
	{
		fprintf(stderr, "ファイルオープンエラー <%s> ！！\n", c_fname);
		return	( false );
	}

	memset(r_buf, '\0', LINEMAX);	// バッファの初期化

	int	a_iCurLine	= 0;
	for(; fgets(r_buf, LINEMAX, rfp) != NULL; a_iCurLine++)
	{
		param_info	r_data;		// 読み込みデータ

		if( a_iCurLine < 2 ) {	// TKY2JGD.parの先頭2行を飛ばす
			continue;
		}

		if( r_buf[0] == '#' || r_buf[0] == '\n' ) {
			continue;
		}

		int	a_ilen	= strlen(r_buf);
		if( r_buf[a_ilen-1] == '\n' ) {
			r_buf[a_ilen-1] = '\0';
		}

		char	*delim		= "\t, ";		// タブ、コンマ、スペース区切り
		char	*field[3]	= { NULL, NULL,	NULL };	// フィールドの文字列のポインタ

		field[0]	= strtok(r_buf, delim);	// 座標系情報
		field[1]	= strtok(NULL, delim);		// 座標情報１(メッシュコード)
		field[2]	= strtok(NULL, delim);		// 座標情報２(X)

		// フィールドの読み取りチェック
		int	i;
		for(i = 0; i < 3; i++)
		{
			if(field[i] == NULL) {
				return	( false );
			}
		}

		r_data.e_imeshcode	= atoi ( field[0] );
		r_data.e_dDlat		= atof ( field[1] );
		r_data.e_dDlon		= atof ( field[2] );

		// 秒から度に変換しておく
		double	a_dTmp;
		a_dTmp	= r_data.e_dDlat / 3600.0;
		r_data.e_dDlat	= a_dTmp;

		a_dTmp	= r_data.e_dDlon / 3600.0;
		r_data.e_dDlon	= a_dTmp;

		m_spParam.insert(r_data);	// データ格納

		memset(r_buf, '\0', LINEMAX);	// バッファの初期化
	}
	fclose	( rfp );
	m_bJGD2000_RP_Inited	= true;	// 初期化済み
	return	( true );
}

//---------------------------------------------------------------------
// 経緯度から、3次メッシュコード + (x,y) を求める
//	@return	無し
//---------------------------------------------------------------------
void	crd_cnv::
LLto3rdMesh(	double	lon,			///< 経度
				double	lat,			///< 緯度
				int		*meshcode,		///< メッシュコード
				int		*x,				///< 正規化Ｘ
				int		*y,				///< 正規化Ｙ
				int		no_mill	)		///< XY座標が1000000になることを許容するか（0=許容,1=許容しない=隣のメッシュの0として返す） 
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
	double	widtop		= 32.0;
	double	heitop		= 21.0 + 1.0 / 3.0;	// 21*60+20;
	double	x_step_0	= 0.0,	y_step_0	= 0.0;

	double	offset_lon	= 100.0;		// メッシュ原点のオフセット値（経度）
	double	offset_lat	= 0.0; 			// メッシュ原点のオフセット値（緯度）

	x_step_0	= widtop / D_MILLION;	// for topmap
	y_step_0	= heitop / D_MILLION;	// for topmap

	x_step_1	= wid1 / D_MILLION;
	y_step_1	= hei1 / D_MILLION;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	x_step_2	= wid2 / D_MILLION;
	y_step_2	= hei2 / D_MILLION;

	wid64		= wid2 / 10.0;
	hei64		= hei2 / 10.0;

	x_step_3	= wid64 / D_MILLION;
	y_step_3	= hei64 / D_MILLION;
	
	up_1st		= (lat - offset_lat) * 1.5;	// 2006.09.06
	low_1st		= lon - offset_lon;			// 2006.09.06

	lat_amari1	= lat - (up_1st / 1.5) - offset_lat;	// 2006.09.06
	lon_amari1	= lon - (low_1st + offset_lon);

	up_2nd		= lat_amari1 / hei2;
	low_2nd		= lon_amari1 / wid2;
	lat_amari2	= lat_amari1 - up_2nd * hei2;
	lon_amari2	= lon_amari1 - low_2nd * wid2;
	up_64		= lat_amari2 / hei64;
	low_64		= lon_amari2 / wid64;
	lat_amari3	= lat_amari2 - up_64 * hei64;
	lon_amari3	= lon_amari2 - low_64 * wid64;
	*meshcode	= up_1st * MILLION + low_1st * 10000 +
					up_2nd * 1000 + low_2nd * 100 +
					up_64 * 10 + low_64;
	*x			= lon_amari3 / x_step_3 + 0.5;
	*y			= lat_amari3 / y_step_3 + 0.5;

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
		*meshcode	= Get3rdMeshCode(*meshcode, xflag, yflag );
	}
}

//---------------------------------------------------------------------
// 3次メッシュコード + (x,y) から、経緯度を求める 2007.09.13
//	@return	無し
//---------------------------------------------------------------------
bool	crd_cnv::
Mesh3rdtoLL(	int		meshcode,		///< メッシュコード
				int		x,				///< 正規化Ｘ
				int		y,				///< 正規化Ｙ
				double*	lon,			///< 経度
				double*	lat	)			///< 緯度
{
	int		atai[6];
	int		i;
	double	wid1	= 1.0,	hei1	= 2.0 / 3.0;
	double	wid2	= 0.0;
	double	wid100	= 0.0;
	double	hei2	= 0.0;
	double	hei100	= 0.0;
	double	widtop	= 0.0;
	double	heitop	= 0.0;

	char	a_cTmpBuf[18];
	char*	a_cTmpStr[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

	memset	( a_cTmpBuf, '\0', 18 );

	wid2	= 7.0 / 60.0 + 30.0 / 3600.0; 
	hei2	= 5.0 / 60.0;
	wid100	= wid2 / 10.0;
	hei100	= hei2 / 10.0;

	widtop	= 32.0;
	heitop	= 21.0 + 1.0 / 3.0;	// 21*60+20

	// 初期化
	for(i = 0; i < 6; i++)
	{
		atai[i]		= 0;
		a_cTmpStr[i]= a_cTmpBuf+(i*3);
	}

	int	digit	= GetMeshDigit(meshcode);


	if(digit == 8) { // 1/64メッシュコードの時

		char	a_cpMeshStr[9];
		sprintf	( a_cpMeshStr, "%d", meshcode );

		strncpy ( a_cTmpStr[5], a_cpMeshStr, 2);	// 1次メッシュ上2桁
		strncpy ( a_cTmpStr[4], a_cpMeshStr+2, 2);	// 1次メッシュ下2桁
		strncpy ( a_cTmpStr[3], a_cpMeshStr+4, 1);	// 2次メッシュ上1桁
		strncpy ( a_cTmpStr[2], a_cpMeshStr+5, 1);	// 2次メッシュ下1桁
		strncpy ( a_cTmpStr[1], a_cpMeshStr+6, 1);	// 8桁メッシュ上1桁
		strncpy ( a_cTmpStr[0], a_cpMeshStr+7, 1);	// 8桁メッシュ下1桁

		for( i = 5; i >= 0; i-- )
		{
			atai[i]	= atoi(a_cTmpStr[i]);
		}

		*lat	= atai[5] / 1.5 + hei2 * atai[3] + hei100 * atai[1]
				+ y * hei100 / D_MILLION;
		*lon	= 100.0 + atai[4] + wid2 * atai[2] + wid100 * atai[0]
				+ x * wid100 / D_MILLION;
	} else {
		return(false);
	}
	return(true);
}

//---------------------------------------------------------------------
//	原点メッシュから(x,y)離れた所のメッシュコードを求める
//	@return		成功：メッシュコード \n
//	@return		失敗： -1
//---------------------------------------------------------------------
int		crd_cnv::
Get3rdMeshCode(	int		meshcode,	///< メッシュコード
				int		xdist,		///< 原点メッシュからX方向へ何メッシュ離れているか
				int		ydist	)	///< 原点メッシュからY方向へ何メッシュ離れているか
{

	int		meshunit	= 0;
	int		roop_count	= 0;

	int		mc			= 0;				// ８桁メッシュコード
	int		num1[6]		= {0,0,0,0,0,0};	// ８桁ばらばらにする
	int		dist[2]		= {0,0};
	int		retmesh		= 0;
	int		tmp1		= 0;
	int		tmp2		= 0;

	dist[0]				= xdist;
	dist[1]				= ydist;

	mc		= meshcode;

	meshunit	= 64;
	roop_count	= 4;

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
			if(num1[j] > 9) {		// 3次メッシュの処理
				num1[j+2]	+= num1[j] / 10;		// 桁の操り上げ
				num1[j]		%= 10;
				if(num1[j+2] > 7) {	// 2次メッシュの処理
					num1[j+4]	+= num1[j+2] / 8;	// 桁の操り上げ
					num1[j+2]	%= 8;
				}
			} else
			if(num1[j] < 0) {		// 3次メッシュの処理
				num1[j]		-= 9;
				num1[j+2]	-= (-num1[j] / 10);		// 桁の繰下げ
				num1[j]		=  9 - (-num1[j] % 10);
				if(num1[j+2] < 0) {	// 2次メッシュの処理
					num1[j+2]	-= 7;
					num1[j+4]	-= (-num1[j+2] / 8);// 桁の繰下げ
					num1[j+2]	= 7 - (-num1[j+2] % 8);
				}
			}
		}

		retmesh	= num1[5] * MILLION	+ num1[4] * 10000 + 
					num1[3] * 1000	+ num1[2] * 100 +
					num1[1] * 10	+ num1[0];
	}
	return(retmesh);
}

//---------------------------------------------------------------------
//	バイリニア補間で値を求めてくる
//	@return		計算値
//---------------------------------------------------------------------
double	crd_cnv::
Bilinear(	double	c_dLU,		///< 左上の値
			double	c_dRU,		///< 右上の値
			double	c_dLD,		///< 左下の値
			double	c_dRD,		///< 右下の値
			double	c_dPtX,		///< 矩形内のX方向位置(0.0<=X<=1.0)
			double	c_dPtY )	///< 矩形内のY方向位置(0.0<=Y<=1.0)
{
	double	a_dValue = 0.0;

	// 2009.02.04 飛田幹男著「世界測地系と座標変換」の解説を元に実装
	a_dValue	= (1.0 - c_dPtX) * (1.0 - c_dPtY) * c_dLD +
					c_dPtY * (1.0 - c_dPtX) * c_dLU +
					c_dPtX * (1.0 - c_dPtY) * c_dRD + c_dPtX * c_dPtY * c_dRU;					

	return	( a_dValue );
}

//---------------------------------------------------------------------
// パラメータテーブルから左下、左上、右上、右下のパラメータを取得する
//	@return	失敗 == 0
//	@return	パラメータの範囲内で変換 == 1
//	@return	パラメータの範囲外で変換(精度が範囲内より甘い) == 2
//---------------------------------------------------------------------
bool		crd_cnv::
GetParam(	int			c_iMeshCode,	///< メッシュコード[IN]
			param_info*	c_pLD_Prm,		///< 左下のパラメータ[OUT]
			param_info*	c_pLU_Prm,		///< 左上のパラメータ[OUT]
			param_info*	c_pRU_Prm,		///< 右上のパラメータ[OUT]
			param_info*	c_pRD_Prm,		///< 右下のパラメータ[OUT]
			bool		c_bIPC_Flag	)	///< IPC固有変換をするかどうか[IN] true=IPC固有変換 false=世間一般対応変換
{
	// 3次メッシュから、左上、右下、右上の3次メッシュを算出する
	int	a_iLU3rdMesh = 0, a_iRD3rdMesh = 0, a_iRU3rdMesh = 0;

	a_iLU3rdMesh	= Get3rdMeshCode ( c_iMeshCode, 0, 1 );
	a_iRD3rdMesh	= Get3rdMeshCode ( c_iMeshCode, 1, 0 );
	a_iRU3rdMesh	= Get3rdMeshCode ( c_iMeshCode, 1, 1 );

	// パラメータテーブルから、経緯度の差分値を取得してくる
	int j = 0;
	int	a_iGetFailFlag	= 0;
	for( j = 0; j < 4; j++ )
	{
		param_info		a_pKey;	// サーチ用のキー
		switch ( j )
		{
		case	0:	a_pKey.e_imeshcode = c_iMeshCode; break;
		case	1:	a_pKey.e_imeshcode = a_iLU3rdMesh; break;
		case	2:	a_pKey.e_imeshcode = a_iRD3rdMesh; break;
		case	3:	a_pKey.e_imeshcode = a_iRU3rdMesh; break;
		}

		std::set<param_info, std::less<param_info> >::iterator	a_iIndx;
		a_iIndx	= m_spParam.find ( a_pKey );
		if( a_iIndx == m_spParam.end() ) {
			a_iGetFailFlag	= 1;
			continue;
		}

		//----------------------------------------------
		// もっとも近いポイントを取ってくる
		//----------------------------------------------

		//------------------------------------------------------------------------------------
		param_info		a_pFoundParam	= *a_iIndx;	// 検索された地域パラメータ
		double			a_dBefore_Lon = 0.0, a_dBefore_Lat = 0.0; // 日本測地系の経緯度
		double			a_dJGD2000_Lon = 0.0, a_dJGD2000_Lat = 0.0, a_dJGD2000_He = 0.0;	// JGD2000の経緯度、楕円体高(m)
		// 2次メッシュコードに変換し、パラメータのエリアを判別 2009.10.20 追加
		if( Is_IPC_Tokyo97_Area( a_pKey.e_imeshcode / 100 ) && c_bIPC_Flag == true ) {
			// 四隅の経緯度を求める
			switch ( j )
			{
			case	0:	Mesh3rdtoLL(a_pKey.e_imeshcode,       0,       0, &a_dBefore_Lon, &a_dBefore_Lat); break;
			case	1:	Mesh3rdtoLL(a_pKey.e_imeshcode,       0, MILLION, &a_dBefore_Lon, &a_dBefore_Lat); break;
			case	2:	Mesh3rdtoLL(a_pKey.e_imeshcode, MILLION,       0, &a_dBefore_Lon, &a_dBefore_Lat); break;
			case	3:	Mesh3rdtoLL(a_pKey.e_imeshcode, MILLION, MILLION, &a_dBefore_Lon, &a_dBefore_Lat); break;
			}
			// 求めた経緯度を3パラメータで世界測地系に変換する
			TOKYOtoJGD2000	( a_dBefore_Lon, a_dBefore_Lat, 0.0, &a_dJGD2000_Lon, &a_dJGD2000_Lat, &a_dJGD2000_He );
			// 地域パラメータの値から、該当ポイントを地域パラメータで変換した経緯度と、
			// 3パラメータで変換した経緯度の差分値を差し引く
			a_pFoundParam.e_dDlon	= a_dJGD2000_Lon - a_dBefore_Lon;
			a_pFoundParam.e_dDlat	= a_dJGD2000_Lat - a_dBefore_Lat;
		}
		//------------------------------------------------------------------------------------

		switch ( j )
		{
		case	0:	*c_pLD_Prm = a_pFoundParam; break;
		case	1:	*c_pLU_Prm = a_pFoundParam; break;
		case	2:	*c_pRD_Prm = a_pFoundParam; break;
		case	3:	*c_pRU_Prm = a_pFoundParam; break;
		}
	}
	if ( a_iGetFailFlag == 1 ) {
		return	( false );
	}else {
		return	( true );
	}
}

//---------------------------------------------------------------------
/** 
* @brief パラメータの範囲をチェックする
* @return	範囲内 == true
* @return	範囲外 == false
*/
//---------------------------------------------------------------------
bool		crd_cnv::
ChkParamRange(	long	c_lLD3rdMesh,			///< [in] 左下の3次メッシュコード
				long	c_lRU3rdMesh,			///< [in] 右上の3次メッシュコード
				long	c_lTgt3rdMesh	)		///< [in] ターゲットの3次メッシュコード
{
	double	a_dLD_Lon = 0.0, a_dLD_Lat = 0.0;
	double	a_dRU_Lon = 0.0, a_dRU_Lat = 0.0;
	double	a_dTgt_Lon= 0.0, a_dTgt_Lat= 0.0;
	// 一番近いパラメータのある点
	Mesh3rdtoLL(c_lLD3rdMesh, 0, 0, &a_dLD_Lon, &a_dLD_Lat);
	Mesh3rdtoLL(c_lRU3rdMesh, 0, 0, &a_dRU_Lon, &a_dRU_Lat);
	Mesh3rdtoLL(c_lTgt3rdMesh, 0, 0, &a_dTgt_Lon, &a_dTgt_Lat);
	if( a_dTgt_Lon < a_dLD_Lon ||	// 左にはみ出す
		a_dTgt_Lon > a_dRU_Lon ||	// 右にはみ出す
		a_dTgt_Lat < a_dLD_Lat ||	// 下にはみ出す
		a_dTgt_Lat > a_dRU_Lat )	// 上にはみ出す
	{
		return	( false );	// 範囲外
	}
	return	( true );	// 範囲内
}

//---------------------------------------------------------------------
/**  2010.01.19 追加テスト
* @brief パラメータテーブルから距離的に一番近いパラメータを取得する(暫定)
* @return	失敗 == 0
* @return	パラメータの範囲内で変換 == 1
* @return	パラメータの範囲外で変換(精度が範囲内より甘い) == 2
*/
//---------------------------------------------------------------------
bool	crd_cnv::
GetNearestParam(	double		c_dLon,			///< [in]   経度
					double		c_dLat,			///< [in]   緯度
					param_info*	c_pNearest_Prm,	///< [out]  最近点のパラメータ
					bool		c_bIPC_Flag	)	///< [in]	IPC固有変換をするかどうか true=IPC固有変換 false=世間一般対応変換
{
	//---------------------------------------------------------------------------------
	// 経緯度を3次メッシュコードに
	int	a_iMesh3rd = 0, a_iX = 0, a_iY = 0;
	LLto3rdMesh	( c_dLon, c_dLat, &a_iMesh3rd, &a_iX, &a_iY, 1 );

	double	a_dLon = 0.0, a_dLat = 0.0;
	param_info	a_pNearest;
	
	// 地域パラメータ外のメッシュの場合は、既に最近傍パラメータを取得済みか確認する
	std::map<int, std::set<param_info, std::less<param_info>>>::const_iterator nearPram = m_outCode2nearCodes.find(a_iMesh3rd);
	if(m_outCode2nearCodes.end() == nearPram){
		// 最近傍パラメータを取得していなければ計算する
		std::set<param_info, std::less<param_info> >	a_pTmpParamSet;	// 最近傍取得用のパラメータリスト
		std::set<param_info, std::less<param_info> >::iterator	a_iBeginIndx, a_iEndIndx, a_iTmpIndx;
		int i = 0;
		for(i = 1;i < 1600; ) // iが320でおよそ1次メッシュの範囲
		{
			int	a_iLD_MeshCode = 0, a_iRU_MeshCode = 0;
			param_info	a_pLDKey, a_pRUKey;
			a_iLD_MeshCode	= Get3rdMeshCode( a_iMesh3rd, -1*i, -1*i );	// 左下のメッシュコードを作成
			a_iRU_MeshCode	= Get3rdMeshCode( a_iMesh3rd, 1*i, 1*i );	// 左下のメッシュコードを作成
			a_pLDKey.e_imeshcode	= a_iLD_MeshCode;
			a_pRUKey.e_imeshcode	= a_iRU_MeshCode;
			a_iBeginIndx	= m_spParam.lower_bound(a_pLDKey);
			a_iEndIndx		= m_spParam.upper_bound(a_pRUKey);
			// 範囲チェックをやる
			for( a_iTmpIndx = a_iBeginIndx; a_iTmpIndx != a_iEndIndx; a_iTmpIndx++ )
			{
				if( ChkParamRange( a_iLD_MeshCode, a_iRU_MeshCode, (*a_iTmpIndx).e_imeshcode ) ) {
					// パラメータリストに追加
					a_pTmpParamSet.insert ( *a_iTmpIndx );
				}
			}
			// パラメータが見つかったら抜ける（どこかしらで見つかるはず）
			if( a_pTmpParamSet.size() > 0 ) {
				break;
			}
			if( i < 10 ) {
				i++;
			}else {
				i+=40;
			}
		}

		//---------------------------------------------------------------------------------
		std::set<param_info, std::less<param_info> >::iterator	a_iIndx, a_iBegin, a_iEnd;
		// 1次メッシュ単位に無い場合は、全検索する
		if( a_pTmpParamSet.size() == 0 ) {
			a_iBegin	= m_spParam.begin();
			a_iEnd		= m_spParam.end();
		}else {
			a_iBegin	= a_pTmpParamSet.begin();
			a_iEnd		= a_pTmpParamSet.end();
		}

		// 次回以降のために近傍パラメータを確保するためのコンテナ
		std::vector<std::pair<double,param_info>> tmpNearParam;

		double	a_dDist= -1.0;
		for( a_iIndx = a_iBegin; a_iIndx != a_iEnd; a_iIndx++ )
		{
			param_info	a_pCur	= *a_iIndx;
			// 距離を計算
			// パラメータのメッシュコードを緯度、経度に変換し、距離を計算
			a_dLon = 0.0;	a_dLat = 0.0;
			Mesh3rdtoLL(a_pCur.e_imeshcode, 0, 0, &a_dLon, &a_dLat);
			double	a_dTmpDist = 0.0;
			a_dTmpDist	= sqrt(pow((c_dLon - a_dLon),2.0) + pow((c_dLat - a_dLat),2.0 ));
			if( a_dDist < 0.0 ){
				a_dDist = a_dTmpDist;
				a_pNearest	= a_pCur;
			} else {
				if( a_dTmpDist < a_dDist ) {
					a_dDist	= a_dTmpDist;
					a_pNearest	= a_pCur;
				}
			}
			tmpNearParam.push_back(std::make_pair(a_dTmpDist,a_pCur));
		}
		// 次回以降のために、パラメータ外のメッシュと最近傍パラメータの対応は確保しておく
		// 近傍パラメータは、指定メッシュの周り8つ分あれば十分
		std::sort(tmpNearParam.begin(),tmpNearParam.end());
		for(long l = 0; l < tmpNearParam.size(); ++l){
			m_outCode2nearCodes[a_iMesh3rd].insert(tmpNearParam[l].second);
			if(8 == l)
				break;
		}
	} else {
		double	a_dDist= -1.0;
		std::set<param_info>::const_iterator a_iIndx = nearPram->second.begin();
		std::set<param_info>::const_iterator a_iEnd = nearPram->second.end();
		for( ; a_iIndx != a_iEnd; ++a_iIndx )
		{
			param_info	a_pCur	= *a_iIndx;
			// 距離を計算
			// パラメータのメッシュコードを緯度、経度に変換し、距離を計算
			a_dLon = 0.0;	a_dLat = 0.0;
			Mesh3rdtoLL(a_pCur.e_imeshcode, 0, 0, &a_dLon, &a_dLat);
			double	a_dTmpDist = 0.0;
			a_dTmpDist	= sqrt(pow((c_dLon - a_dLon),2.0) + pow((c_dLat - a_dLat),2.0 ));
			if( a_dDist < 0.0 ){
				a_dDist = a_dTmpDist;
				a_pNearest	= a_pCur;
			} else {
				if( a_dTmpDist < a_dDist ) {
					a_dDist	= a_dTmpDist;
					a_pNearest	= a_pCur;
				}
			}
		}
	}

	// 一番近いパラメータのある点
	Mesh3rdtoLL(a_pNearest.e_imeshcode, 0, 0, &a_dLon, &a_dLat);

	//------------------------------------------------------------------------------------
	double			a_dBefore_Lon = 0.0, a_dBefore_Lat = 0.0; // 日本測地系の経緯度
	double			a_dJGD2000_Lon = 0.0, a_dJGD2000_Lat = 0.0, a_dJGD2000_He = 0.0;	// JGD2000の経緯度、楕円体高(m)
	// 2次メッシュコードに変換し、パラメータのエリアを判別 2009.10.20 追加
	if( Is_IPC_Tokyo97_Area( a_pNearest.e_imeshcode / 100 ) && c_bIPC_Flag == true ) {
		// 四隅の経緯度を求める
		Mesh3rdtoLL(a_pNearest.e_imeshcode,       0,       0, &a_dBefore_Lon, &a_dBefore_Lat);
		// 求めた経緯度を3パラメータで世界測地系に変換する
		TOKYOtoJGD2000	( a_dBefore_Lon, a_dBefore_Lat, 0.0, &a_dJGD2000_Lon, &a_dJGD2000_Lat, &a_dJGD2000_He );
		// 地域パラメータの値から、該当ポイントを地域パラメータで変換した経緯度と、
		// 3パラメータで変換した経緯度の差分値を差し引く
		a_pNearest.e_dDlon	= a_dJGD2000_Lon - a_dBefore_Lon;
		a_pNearest.e_dDlat	= a_dJGD2000_Lat - a_dBefore_Lat;
	}
	//------------------------------------------------------------------------------------

	// 3パラメータで変換
	double	a_dAfter_3prm_Lon = 0.0, a_dAfter_3prm_Lat = 0.0, a_dAfter_He = 0.0;
	TOKYOtoJGD2000	( a_dLon, a_dLat, 0.0, &a_dAfter_3prm_Lon, &a_dAfter_3prm_Lat, &a_dAfter_He );
	// 地域パラメータで変換した値
	double	a_dAfter_Rprm_Lon = 0.0, a_dAfter_Rprm_Lat = 0.0;
	a_dAfter_Rprm_Lon		= a_dLon + a_pNearest.e_dDlon;
	a_dAfter_Rprm_Lat		= a_dLat + a_pNearest.e_dDlat;

	// 着目点を3パラメータで変換
	double	a_dTmp_3prm_Lon = 0.0, a_dTmp_3prm_Lat = 0.0;
	TOKYOtoJGD2000	( c_dLon, c_dLat, 0.0, &a_dTmp_3prm_Lon, &a_dTmp_3prm_Lat, &a_dAfter_He );

	// 暫定パラメータを計算
	a_pNearest.e_dDlon	= (a_dTmp_3prm_Lon - c_dLon) + (a_dAfter_Rprm_Lon - a_dAfter_3prm_Lon);
	a_pNearest.e_dDlat	= (a_dTmp_3prm_Lat - c_dLat) + (a_dAfter_Rprm_Lat - a_dAfter_3prm_Lat);

	*c_pNearest_Prm	= a_pNearest;
	return	( true );
}

//---------------------------------------------------------------------
// 経緯度の測地系相互変換
//	@return	パラメータの範囲内で変換 == 1
//	@return	パラメータの範囲外で変換(精度が範囲内より甘い) == 2
//---------------------------------------------------------------------
int		crd_cnv::
LLConv(	double	c_dBefore_Lon,	///< 変換前経度
		double	c_dBefore_Lat,	///< 変換前緯度
		double*	c_dAfter_Lon,	///< 変換後経度
		double*	c_dAfter_Lat,	///< 変換元緯度
		int		c_iMode,		///< 変換モード（0=Tokyo->JGD2000への変換 1=JGD2000->Tokyoへの変換）
		bool	c_bIPC_Flag	)	///< IPC固有変換をするかどうか true=IPC固有変換 false=世間一般対応変換
{

	int	a_iMeshCode = 0, a_iX = 0, a_iY = 0;
	double	a_dGTmpLon = 0.0, a_dGTmpLat = 0.0;

	// 経緯度から3次メッシュコードに変換する
	if( c_iMode == 0 ) {
		LLto3rdMesh( c_dBefore_Lon, c_dBefore_Lat, &a_iMeshCode, &a_iX, &a_iY, 1 );
	}else {
		int	a_iTmpMeshCode = 0, a_iTmpX = 0, a_iTmpY = 0;
		double	a_dTmpLon = 0.0, a_dTmpLat = 0.0;
		a_dTmpLon	= c_dBefore_Lon + (12.0/3600.0);
		a_dTmpLat	= c_dBefore_Lat - (12.0/3600.0);
		// 仮の旧日本測地系に変換
		LLto3rdMesh( a_dTmpLon, a_dTmpLat, &a_iTmpMeshCode, &a_iTmpX, &a_iTmpY, 1 );
		
		// tmpの値からテンポラリの差分値を求める
		param_info		a_pTmpLD, a_pTmpLU, a_pTmpRU, a_pTmpRD;	// パラメータから取得した値
		if( GetParam( a_iTmpMeshCode, &a_pTmpLD, &a_pTmpLU, &a_pTmpRU, &a_pTmpRD, c_bIPC_Flag ) ) {

			// tmpの値からバイリニア補間で値を求める
			// 取得した値からバイリニア補間で該当地点の値を求める
			double	a_dTmpPtX = 0.0, a_dTmpPtY = 0.0;
			double	a_dDTmpLon = 0.0, a_dDTmpLat = 0.0;

			a_dTmpPtX	= a_iTmpX / D_MILLION;
			a_dTmpPtY	= a_iTmpY / D_MILLION;

			// 仮の旧日本測地系から得られたパラメータを使用し、より正確な旧日本測地系の位置を取り出し、その位置から
			// パラメータを割り出す
			a_dDTmpLon	= Bilinear ( a_pTmpLU.e_dDlon, a_pTmpRU.e_dDlon, a_pTmpLD.e_dDlon, a_pTmpRD.e_dDlon, a_dTmpPtX, a_dTmpPtY );
			a_dDTmpLat	= Bilinear ( a_pTmpLU.e_dDlat, a_pTmpRU.e_dDlat, a_pTmpLD.e_dDlat, a_pTmpRD.e_dDlat, a_dTmpPtX, a_dTmpPtY );

			LLto3rdMesh( c_dBefore_Lon - a_dDTmpLon, c_dBefore_Lat - a_dDTmpLat, &a_iMeshCode, &a_iX, &a_iY, 1 );

		}else {
			// 一番近いところのパラメータを取得する
			param_info		a_pNearest;
			GetNearestParam( a_dTmpLon, a_dTmpLat, &a_pNearest, c_bIPC_Flag );
			a_dGTmpLon	= a_pNearest.e_dDlon;
			a_dGTmpLat	= a_pNearest.e_dDlat;
		//	return	( false );
		}
	}
	// 2009.01.28 IPC固有の変換対応を行うかどうかの判定を行う
	// Tokyo97対応をしたところについては強制的に3パラメータ計算で変換するようにしている。
//	bool	a_bExec_IPC_Only	= false;
//	if( c_bIPC_Flag == true ) {
//		a_bExec_IPC_Only	= Is_IPC_Tokyo97_Area( a_iMeshCode / 100 );	// 2次メッシュコードを入れて確認
//	}

	// パラメータテーブルから、経緯度の差分値を取得してくる
	param_info		a_pLD, a_pLU, a_pRU, a_pRD;	// パラメータから取得した値

	double	a_dDLon = 0.0, a_dDLat = 0.0;
	if( GetParam( a_iMeshCode, &a_pLD, &a_pLU, &a_pRU, &a_pRD, c_bIPC_Flag ) ) {

		// 取得した値からバイリニア補間で該当地点の値を求める
		double	a_dPtX = 0.0, a_dPtY = 0.0;

		a_dPtX	= a_iX / D_MILLION;
		a_dPtY	= a_iY / D_MILLION;

		a_dDLon	= Bilinear ( a_pLU.e_dDlon, a_pRU.e_dDlon, a_pLD.e_dDlon, a_pRD.e_dDlon, a_dPtX, a_dPtY );
		a_dDLat	= Bilinear ( a_pLU.e_dDlat, a_pRU.e_dDlat, a_pLD.e_dDlat, a_pRD.e_dDlat, a_dPtX, a_dPtY );
	}else {
		param_info		a_pNearest;
		GetNearestParam( c_dBefore_Lon - a_dGTmpLon, c_dBefore_Lat - a_dGTmpLat, &a_pNearest, c_bIPC_Flag );
		a_dDLon	= a_pNearest.e_dDlon;
		a_dDLat	= a_pNearest.e_dDlat;
	}
	// 差分値と元の経緯度からJGD2000での経緯度を求める
	if( c_iMode == 0 ) {
		*c_dAfter_Lon	= c_dBefore_Lon + a_dDLon;
		*c_dAfter_Lat	= c_dBefore_Lat + a_dDLat;
	}else {
		*c_dAfter_Lon	= c_dBefore_Lon - a_dDLon;
		*c_dAfter_Lat	= c_dBefore_Lat - a_dDLat;
	}
	return	( CONVED_REGIONPRM );
}
//-----------------------------------------------------------------------------

//=======================================================================
//	下記6つのメンバ関数、BLHtoXYZ(), XYZtoBLH(), XYZtoXYZ()	は、
//  [Bug 5755] の要望により追加 2008.01.21
//	※【補足事項】
//	この6つの測地系変換関数は、飛田 幹夫氏著書、「世界測地系と座標変換」
//  で解説されている手法を元に作成されています。
//=======================================================================

//---------------------------------------------------------------------
// 地理座標値から三次元直交座標値への変換 2007.05.09 追加
// (B,L,He)->(X,Y,Z)
//	@return	無し
//---------------------------------------------------------------------
void	crd_cnv::
BLHtoXYZ(	double	c_dA,			///< 回転楕円体長半径(m)
			double	c_dInvF,		///< 扁平率fの逆数1/f
			double	c_dLon,			///< 経度
			double	c_dLat,			///< 緯度
			double	c_dHe,			///< 楕円体高(m)
			double*	c_dX,			///< 原点からのX軸方向の距離(m)[OUT]
			double*	c_dY,			///< 原点からのY軸方向の距離(m)[OUT]
			double*	c_dZ	)		///< 原点からのZ軸方向の距離(m)[OUT]
{
//	const double	a_dA	= 6377397.155;				// ベッセル楕円体長半径(m)
	const double	a_dF	= 1.0/c_dInvF;				// 扁平率
	double			a_dE2	= a_dF*(2.0 - a_dF);		// 離心率の２乗
	double			a_dN	= 0.0;						// 卯酉線曲率半径(m)

	double			a_dRad_Lon = 0.0, a_dRad_Lat = 0.0;	// ラジアンに直した経度、緯度

	// 経緯度をラジアンに直す
	a_dRad_Lon	= c_dLon * M_PI / 180.0;
	a_dRad_Lat	= c_dLat * M_PI / 180.0;

	a_dN	= c_dA / sqrt(1.0 - (a_dE2)*pow(sin(a_dRad_Lat), 2.0));

	*c_dX	= (a_dN + c_dHe) * cos(a_dRad_Lat) * cos(a_dRad_Lon);
	*c_dY	= (a_dN + c_dHe) * cos(a_dRad_Lat) * sin(a_dRad_Lon);
	*c_dZ	= (a_dN * (1.0 - a_dE2) + c_dHe) * sin(a_dRad_Lat);
}

//---------------------------------------------------------------------
// 三次元直交座標値から地理座標値への変換 2007.05.09 追加
// (X,Y,Z)->(B,L,He)
//	@return	無し
//---------------------------------------------------------------------
void	crd_cnv::
XYZtoBLH(	double	c_dA,			///< 回転楕円体長半径(m)
			double	c_dInvF,		///< 扁平率fの逆数1/f
			double	c_dX,			///< 原点からのX軸方向の距離(m)
			double	c_dY,			///< 原点からのY軸方向の距離(m)
			double	c_dZ,			///< 原点からのZ軸方向の距離(m)
			double*	c_dLon,			///< 経度[OUT]
			double*	c_dLat,			///< 緯度[OUT]
			double*	c_dHe	)		///< 楕円体高(m)[OUT]
{
//	const double	a_dA	= 6378137.0;				// GRS80楕円体長半径(m)
	const double	a_dF	= 1.0/c_dInvF;				// 扁平率
	double			a_dE2	= a_dF*(2.0 - a_dF);		// 離心率の２乗
	double			a_dN	= 0.0;						// 卯酉線曲率半径(m)

	double			a_dRad_Lon = 0.0, a_dRad_Lat = 0.0;	// ラジアンに直した経度、緯度

	double			a_dOut_Lon = 0.0, a_dOut_Lat = 0.0, a_dOut_He = 0.0;

	// 作業用変数
	double			a_dp = 0.0, a_dr = 0.0, a_du = 0.0;

	a_dp			= sqrt ( c_dX*c_dX + c_dY*c_dY );
	a_dr			= sqrt ( a_dp*a_dp + c_dZ*c_dZ );

	a_du			= atan ( c_dZ * ((1.0 - a_dF) + (a_dE2 * c_dA)/a_dr) / a_dp );

	a_dOut_Lat		= atan ( (c_dZ * (1.0 - a_dF) + (a_dE2 * c_dA) * pow(sin(a_du), 3.0)) /
								((1.0 - a_dF) * (a_dp - a_dE2 * c_dA * pow(cos(a_du), 3.0)) )) * 180.0 / M_PI;

	// 2011.2.10 atan()だと、-PI/2<=atan(x)<=PI/2 までの範囲なのでatan2()、-PI<=atan2(y,x)<=PI 
	// を使用するように修正。
	a_dOut_Lon		= atan2 ( c_dY, c_dX ) * 180.0 / M_PI;

	// 緯度をラジアンに変換しておく
	a_dRad_Lat		= a_dOut_Lat * M_PI / 180.0;

	a_dOut_He		= a_dp * cos(a_dRad_Lat) + c_dZ * sin(a_dRad_Lat) - c_dA * sqrt( (1.0 - a_dE2 * pow(sin(a_dRad_Lat), 2.0)) );

	*c_dLon			= a_dOut_Lon;
	*c_dLat			= a_dOut_Lat;
	*c_dHe			= a_dOut_He;
}

//---------------------------------------------------------------------
// 三次元直交座標間での座標変換 2007.05.09 追加
//	@return	無し
//---------------------------------------------------------------------
void	crd_cnv::
XYZtoXYZ(	param_7	c_pParam7,		///< 変換に使用する7パラメータ
			double	c_dXi,			///< 原点からのX軸方向の距離(m)(入力)
			double	c_dYi,			///< 原点からのY軸方向の距離(m)(入力)
			double	c_dZi,			///< 原点からのZ軸方向の距離(m)(入力)
			double*	c_dXo,			///< 原点からのX軸方向の距離(m)(出力)[OUT]
			double*	c_dYo,			///< 原点からのY軸方向の距離(m)(出力)[OUT]
			double*	c_dZo	)		///< 原点からのZ軸方向の距離(m)(出力)[OUT]
{
	*c_dXo	= c_dXi + (c_pParam7.T1 / 100.0) + (c_pParam7.D * 1e-8) * c_dXi
				- (c_pParam7.R3*0.001*M_PI*c_dYi/(180.0*3600.0)) + (c_pParam7.R2*0.001*M_PI*c_dZi/(180.0*3600.0));

	*c_dYo	= c_dYi + (c_pParam7.T2 / 100.0) + (c_pParam7.R3*0.001*M_PI*c_dXi/(180.0*3600.0))
				+ (c_pParam7.D * 1e-8) * c_dYi - (c_pParam7.R1*0.001*M_PI*c_dZi/(180.0*3600.0));

	*c_dZo	= c_dZi + (c_pParam7.T3 / 100.0) - (c_pParam7.R2*0.001*M_PI*c_dXi/(180.0*3600.0))
				+ (c_pParam7.R1*0.001*M_PI*c_dYi/(180.0*3600.0)) + (c_pParam7.D * 1e-8) * c_dZi;
}

//---------------------------------------------------------------------
// 経緯度の測地系相互変換
//	@return	無し
//---------------------------------------------------------------------
void	crd_cnv::
BLHtoBLH(	double	c_dBefore_A,	///< 変換前座標系の準拠する楕円体長半径(m)
			double	c_dBefore_InvF,	///< 変換前座標系の準拠する楕円体の扁平率
			double	c_dAfter_A,		///< 変換前座標系の準拠する楕円体長半径(m)
			double	c_dAfter_InvF,	///< 変換前座標系の準拠する楕円体の扁平率
			param_7	c_pBtoAParam7,	///< 変換に使用する7パラメータ
			double	c_dBefore_Lon,	///< 変換前経度
			double	c_dBefore_Lat,	///< 変換前緯度
			double	c_dBefore_He,	///< 変換前楕円体高(m)
			double*	c_dAfter_Lon,	///< 変換後経度[OUT]
			double*	c_dAfter_Lat,	///< 変換後緯度[OUT]
			double*	c_dAfter_He )	///< 変換後楕円体高(m)[OUT]
{

	// テンポラリのX,Y,Z(m)座標値
	double	a_dXi = 0.0, a_dYi = 0.0, a_dZi = 0.0;
	double	a_dXo = 0.0, a_dYo = 0.0, a_dZo = 0.0;

	// 変換後の値
	double	a_dTmp_Lon = 0.0, a_dTmp_Lat = 0.0;
	double	a_dHo = 0.0;

	int	a_iRetMode	= 2;

	BLHtoXYZ(c_dBefore_A, c_dBefore_InvF, c_dBefore_Lon, c_dBefore_Lat, c_dBefore_He, &a_dXi, &a_dYi, &a_dZi );

	XYZtoXYZ(c_pBtoAParam7, a_dXi, a_dYi, a_dZi, &a_dXo, &a_dYo, &a_dZo);

	XYZtoBLH(c_dAfter_A, c_dAfter_InvF, a_dXo, a_dYo, a_dZo, &a_dTmp_Lon, &a_dTmp_Lat, &a_dHo);

	*c_dAfter_Lon	= a_dTmp_Lon;	// 2011.2.10 修正
	*c_dAfter_Lat	= a_dTmp_Lat;
	*c_dAfter_He	= a_dHo;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------
// Tokyo->JGD2000への座標変換
//	@return 無し
//---------------------------------------------------------------------
void	crd_cnv::
TOKYOtoJGD2000	(	double	c_dBefore_Lon,	///< 変換前経度
					double	c_dBefore_Lat,	///< 変換前緯度
					double	c_dBefore_He,	///< 変換前楕円体高(m)
					double*	c_dAfter_Lon,	///< 変換後経度
					double*	c_dAfter_Lat,	///< 変換元緯度
					double*	c_dAfter_He )	///< 変換後楕円体高(m)[OUT]
{
	double	a_dBefore_A = 0.0, a_dAfter_A = 0.0;
	double	a_dBefore_InvF = 0.0, a_dAfter_InvF = 0.0;
	param_7	a_pParam;

	// Tokyo->JGD2000 変換用パラメータ設定
	//=============================================================
	a_dBefore_A		= RA;				// ベッセル楕円体
	a_dBefore_InvF	= BESSEL_INV_F;		// ベッセル楕円体の扁平率の逆数

	a_dAfter_A		= GRS80_RA;			// GRS80楕円体
	a_dAfter_InvF	= GRS80_INV_F;		// GRS80楕円体の扁平率の逆数

	a_pParam.T1	= -14641.40;
	a_pParam.T2	= 50733.70;
	a_pParam.T3	= 68050.70;
	a_pParam.D	= 0.00;
	a_pParam.R1	= 0.00;
	a_pParam.R2	= 0.00;
	a_pParam.R3	= 0.00;
	//=============================================================

	BLHtoBLH( a_dBefore_A, a_dBefore_InvF, a_dAfter_A, a_dAfter_InvF, 
			a_pParam, c_dBefore_Lon, c_dBefore_Lat, c_dBefore_He, c_dAfter_Lon, c_dAfter_Lat, c_dAfter_He);
}

//---------------------------------------------------------------------
// JGD2000->Tokyoへの座標変換
//	@return 無し
//---------------------------------------------------------------------
void	crd_cnv::
JGD2000toTOKYO	(	double	c_dBefore_Lon,	///< 変換前経度
					double	c_dBefore_Lat,	///< 変換前緯度
					double	c_dBefore_He,	///< 変換前楕円体高(m)
					double*	c_dAfter_Lon,	///< 変換後経度
					double*	c_dAfter_Lat,	///< 変換元緯度
					double*	c_dAfter_He )	///< 変換後楕円体高(m)[OUT]
{
	double	a_dBefore_A = 0.0, a_dAfter_A = 0.0;
	double	a_dBefore_InvF = 0.0, a_dAfter_InvF = 0.0;
	param_7	a_pParam;

	// JGD2000->Tokyo 変換用パラメータ設定
	//=============================================================
	a_dBefore_A		= GRS80_RA;			// GRS80楕円体
	a_dBefore_InvF	= GRS80_INV_F;		// GRS80楕円体の扁平率の逆数

	a_dAfter_A		= RA;				// ベッセル楕円体
	a_dAfter_InvF	= BESSEL_INV_F;		// ベッセル楕円体の扁平率の逆数

	a_pParam.T1	= 14641.40;
	a_pParam.T2	= -50733.70;
	a_pParam.T3	= -68050.70;
	a_pParam.D	= 0.00;
	a_pParam.R1	= 0.00;
	a_pParam.R2	= 0.00;
	a_pParam.R3	= 0.00;
	//=============================================================

	BLHtoBLH( a_dBefore_A, a_dBefore_InvF, a_dAfter_A, a_dAfter_InvF, 
			a_pParam, c_dBefore_Lon, c_dBefore_Lat, c_dBefore_He, c_dAfter_Lon, c_dAfter_Lat, c_dAfter_He);
}

//---------------------------------------------------------------------
// 地域毎パラメータを使用したTokyo->JGD2000への座標変換
//	@return 0  = 変換エラー（未初期化で使用した場合等）
//  @return	1  = 地域毎のパラメータで変換された場合3パラメータで変換された場合
//  @return 2  = 3パラメータで変換された場合
//---------------------------------------------------------------------
int	crd_cnv::
TOKYOtoJGD2000_RP	(	double	c_dBefore_Lon,		///< 変換前経度[IN]
						double	c_dBefore_Lat,		///< 変換前緯度[IN]
						double*	c_dAfter_Lon,		///< 変換後経度[OUT]
						double*	c_dAfter_Lat,		///< 変換元緯度[OUT]
						bool	c_bIPC_Flag	)		///< IPC固有変換をするかどうかtrue=IPC固有変換 false=世間一般対応変換[IN]
{
	if( m_bJGD2000_RP_Inited == false )	return	(JGD2000_RP_UNINITED);
	int	a_iRet	= 0;
	a_iRet	= LLConv( c_dBefore_Lon, c_dBefore_Lat, c_dAfter_Lon, c_dAfter_Lat, 0, c_bIPC_Flag );
	return	( a_iRet );
}

//---------------------------------------------------------------------
// 地域毎パラメータを使用したJGD2000->Tokyoへの座標変換
//	@return 0  = 変換エラー（未初期化で使用した場合等）
//  @return	1  = 地域毎のパラメータで変換された場合3パラメータで変換された場合
//  @return 2  = 3パラメータで変換された場合
//---------------------------------------------------------------------
int	crd_cnv::
JGD2000toTOKYO_RP	(	double	c_dBefore_Lon,		///< 変換前経度[IN]
						double	c_dBefore_Lat,		///< 変換前緯度[IN]
						double*	c_dAfter_Lon,		///< 変換後経度[OUT]
						double*	c_dAfter_Lat,		///< 変換元緯度[OUT]
						bool	c_bIPC_Flag	)		///< IPC固有変換をするかどうかtrue=IPC固有変換 false=世間一般対応変換[IN]
{
	if( m_bJGD2000_RP_Inited == false )	return	(JGD2000_RP_UNINITED);
	int	a_iRet	= 0;
	a_iRet	= LLConv( c_dBefore_Lon, c_dBefore_Lat, c_dAfter_Lon, c_dAfter_Lat, 1, c_bIPC_Flag );
	return	( a_iRet );
}

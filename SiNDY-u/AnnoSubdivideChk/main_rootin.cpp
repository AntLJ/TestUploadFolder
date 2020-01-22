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

#include "stdafx.h"
#include "main_rootin.h"
#include <poly_class.h>
#include <atlstr.h>
#include <WinLib/VersionInfo.h>

using namespace sindy::schema;

/** ---------------------------------------------------------------------------
//	指定の長さから何度に当たるかを返す
//	@return	度単位での距離
-----------------------------------------------------------------------------*/
double	main_rootin::
Get_Degree_Range (	int		c_iMeshCode,		///< メッシュコード 
					double	c_dMeter_Range )	///< メートル単位の範囲
{
	crd_cnv		a_cCrd;
	double		a_dMeshWid	= 0.0;
	double		a_dMeshHei	= 0.0;
	double		a_dLd_Lon	= 0.0;	// 左下緯度
	double		a_dLd_Lat	= 0.0;	// 左下経度
	double		a_dRd_Lon	= 0.0;	// 左下緯度
	double		a_dRd_Lat	= 0.0;	// 左下経度

	a_dMeshWid	= a_cCrd.GetMeshDist ( c_iMeshCode, 0, 0, c_iMeshCode, 1000000, 0 );
	a_dMeshHei	= a_cCrd.GetMeshDist ( c_iMeshCode, 0, 0, c_iMeshCode, 0, 1000000 );

	a_cCrd.MeshtoLL ( c_iMeshCode, 0, 0, &a_dLd_Lon, &a_dLd_Lat );
	a_cCrd.MeshtoLL	( c_iMeshCode, 1000000, 0, &a_dRd_Lon, &a_dRd_Lat );

	return(	(a_dRd_Lon - a_dLd_Lon) * c_dMeter_Range / a_dMeshWid );

}

/** ---------------------------------------------------------------------------
//	指定の長さから何度に当たるかを返す
//	@return	度単位での距離
-----------------------------------------------------------------------------*/
double	main_rootin::
Get_Degree_Range_Dev (	int		c_iMeshCode,		///< メッシュコード
						int		c_iScale,			///< スケール
						int		c_iDev_Dist )		///< デバイス座標点値での距離
{
	crd_cnv		a_cCrd;
	double		a_dMeshWid	= 0.0;
	double		a_dMeshHei	= 0.0;
	double		a_dLd_Lon	= 0.0;	// 左下緯度
	double		a_dLd_Lat	= 0.0;	// 左下経度
	double		a_dRd_Lon	= 0.0;	// 左下緯度
	double		a_dRd_Lat	= 0.0;	// 左下経度

	int	devrange_x[4];
	int	devrange_y[4];
	int i = 0, j = 0;
	for(i = 0, j = 1; i < 4; i++, j*=2)
	{
		devrange_x[i]	=	320 * j;
		devrange_y[i]	=	240 * j;
	}

	a_cCrd.MeshtoLL ( c_iMeshCode, 0, 0, &a_dLd_Lon, &a_dLd_Lat );
	if( a_cCrd.GetMeshDigit(c_iMeshCode) == 4 ) {
		a_cCrd.MeshtoLL	( c_iMeshCode, 2000000, 0, &a_dRd_Lon, &a_dRd_Lat );
	}else {
		a_cCrd.MeshtoLL	( c_iMeshCode, 1000000, 0, &a_dRd_Lon, &a_dRd_Lat );
	}
	// 経度方向での距離
	return(	(a_dRd_Lon - a_dLd_Lon) * c_iDev_Dist / devrange_x[c_iScale-1] );
}

/** ---------------------------------------------------------------------------
//	指定の長さから何度に当たるかを返す
-----------------------------------------------------------------------------*/
void	main_rootin::
Get_Degree_Range_Dev (	int		c_iMeshCode,		///< メッシュコード[IN]
						int		c_iScale,			///< スケール[IN]
						int		c_iDev_Dist, 		///< デバイス座標点値での距離[IN]
						double*	c_cpRetXDeg,		///< X方向の距離[OUT]
						double*	c_cpRetYDeg )		///< Y方向の距離[OUT]
{
	crd_cnv		a_cCrd;
	double		a_dMeshWid	= 0.0;
	double		a_dMeshHei	= 0.0;
	double		a_dLd_Lon	= 0.0;	// 左下緯度
	double		a_dLd_Lat	= 0.0;	// 左下経度
	double		a_dRu_Lon	= 0.0;	// 左下緯度
	double		a_dRu_Lat	= 0.0;	// 左下経度

	int	devrange_x[4];
	int	devrange_y[4];
	int i = 0, j = 0;
	for(i = 0, j = 1; i < 4; i++, j*=2)
	{
		devrange_x[i]	=	320 * j;
		devrange_y[i]	=	240 * j;
	}

	a_cCrd.MeshtoLL ( c_iMeshCode, 0, 0, &a_dLd_Lon, &a_dLd_Lat );
	if( a_cCrd.GetMeshDigit(c_iMeshCode) == 4 ) {
		a_cCrd.MeshtoLL	( c_iMeshCode, 2000000, 2000000, &a_dRu_Lon, &a_dRu_Lat );
	}else {
		a_cCrd.MeshtoLL	( c_iMeshCode, 1000000, 1000000, &a_dRu_Lon, &a_dRu_Lat );
	}
	// 経度方向での距離
	*c_cpRetXDeg	= (a_dRu_Lon - a_dLd_Lon) * c_iDev_Dist / devrange_x[c_iScale-1] ;
	*c_cpRetYDeg	= (a_dRu_Lat - a_dLd_Lat) * c_iDev_Dist / devrange_y[c_iScale-1] ;
}

/** ---------------------------------------------------------------------------
//	一度デバイス座標に変換し、その後緯度経度に戻す
//	@return	IPointPtr
-----------------------------------------------------------------------------*/
IPointPtr	main_rootin::
get_Device_Pt	(	int				c_iMeshCode,
					IPointPtr		c_ipPoint,
					int				c_iScale	)	///< スケール
{
	crd_cnv	crd;

	int	devrange_x[4];
	int	devrange_y[4];
	int i = 0, j = 0;
	for(i = 0, j = 1; i < 4; i++, j*=2)
	{
		devrange_x[i]	=	320 * j;
		devrange_y[i]	=	240 * j;
	}
	double		a_dTmp_X	= 0, a_dTmp_Y	= 0;
//	Device_Pt	a_mDev_Pt;		// 現デバイス座標
	int			a_iNorm_X	= 0;
	int			a_iNorm_Y	= 0;

	double	a_dX = 0.0, a_dY = 0.0;
	c_ipPoint->get_X ( &a_dX );
	c_ipPoint->get_Y ( &a_dY );

	// 正規化座標に一度変換する(メッシュコード指定)
	crd.LLtoMesh2	( a_dX, a_dY, c_iMeshCode, &a_iNorm_X, &a_iNorm_Y);

	int	a_iMesh_Digit	= crd.GetMeshDigit ( c_iMeshCode );
	if( a_iMesh_Digit == 4 ) {
		a_iNorm_X /= 2;
		a_iNorm_Y /= 2;
	}

	// デバイス座標の計算（四捨五入する）
	a_dTmp_X		= (double)devrange_x[c_iScale-1] * a_iNorm_X / 1000000.0 + 0.5;
	a_dTmp_Y		= (double)devrange_y[c_iScale-1] * a_iNorm_Y / 1000000.0 + 0.5;
//	a_mDev_Pt.e_X	= (long)a_dTmp_X;
//	a_mDev_Pt.e_Y	= (long)a_dTmp_Y;
	// 計算誤差のためか、INデータと+1ptの差があるので、-1ptしておく
	int	a_iDev_X	= 0;
	int a_iDev_Y	= 0;

	a_iDev_X	= (long)a_dTmp_X-1;	// 2002.7.18
	a_iDev_Y	= (long)a_dTmp_Y-1;	// 2002.7.18

	// 座標を緯度経度に戻す
	double	a_dAfter_X	= 0.0;
	double	a_dAfter_Y	= 0.0;
	double	a_dAfter_N_X= 0;
	double	a_dAfter_N_Y= 0;
	int		a_iTmp_X	= 0;
	int		a_iTmp_Y	= 0;

	a_dAfter_N_X	= (double)a_iDev_X * 1000000.0 / devrange_x[c_iScale-1] - 0.5;
	a_dAfter_N_Y	= (double)a_iDev_Y * 1000000.0 / devrange_y[c_iScale-1] - 0.5;
	a_iTmp_X	= (int)a_dAfter_N_X;
	a_iTmp_Y	= (int)a_dAfter_N_Y;
	if( a_iMesh_Digit == 4 ) {
		a_iTmp_X *= 2;
		a_iTmp_Y *= 2;
	}
	crd.MeshtoLL ( c_iMeshCode, a_iTmp_X, a_iTmp_Y, &a_dAfter_X, &a_dAfter_Y );

	IPointPtr	a_ipRet_Pt(CLSID_Point);
	a_ipRet_Pt->put_X ( a_dAfter_X );
	a_ipRet_Pt->put_Y ( a_dAfter_Y );

	return		( a_ipRet_Pt );
}

/** ---------------------------------------------------------------------------
//	真位置も含めた表示位置を全て取り出す
-----------------------------------------------------------------------------*/
bool	main_rootin::
Get_Disp_Pts	(	int						c_iMeshCode,		// メッシュコード
				//	int						c_iAttr_Code,		// 属性コード
					Feature_All_Attrs&		c_fFAtt,			// 注記真位置の情報
					msi_handle_ws*			c_mAny_Note_Line_WS,// 該当ラインの検索用
					vector<disp_pt_info>&	c_vPoints )
{
	//--------------------------------------------------------------------
	// 真位置の情報
	disp_pt_info			a_dTruePtInfo;
	a_dTruePtInfo.e_iOid	= c_fFAtt.Get_Obj_ID();
	a_dTruePtInfo.e_ipPt	= (IPointPtr)c_fFAtt.Ret_Geo();
	if( m_iDigit == 8 ) {
		a_dTruePtInfo.e_iScale	= 3;	// 都市地図はスケール３
	}else {
		a_dTruePtInfo.e_iScale	= 4;	// 仮
	}
	a_dTruePtInfo.e_iStat	= 1;		// 真位置です

	c_vPoints.push_back	( a_dTruePtInfo );	// データ追加
	//--------------------------------------------------------------------

	// 注記ラインが不要の場合は抜ける
	if( eExe_Flag & ON_DEL_DISP_LINE_MESSAGE ) {
		return	( true );
	}

	Feature_Attr	a_fAttr;
	long			a_lAttr_Code	= 0;
	a_fAttr			= c_fFAtt.Get_Attr ( annotation::kAnnoCode );
	a_lAttr_Code	= a_fAttr.e_ccvAttr.intVal;

	// フェリーは表示位置は要らない
//	if( a_lAttr_Code == 203105 ||	// フェリー航路名称又は到着地名称
//		a_lAttr_Code == 203106 ) {	// その他旅客路線航路名称又は到着地名称
//		return ( true );
//	}

	string	a_sWhereStr;
	char	a_cpStr[32];
	int		a_iDispFlag[4];
	sprintf	( a_cpStr, "%s = %d", disp_line::kAnnoID, a_dTruePtInfo.e_iOid );	// Where句作成
	a_sWhereStr	= a_cpStr;
	int	a_iStart	= 0, a_iEnd	= 4;
	if( m_iDigit == 8 ) {
		a_iStart		= 2;
		a_iEnd			= 3;
		a_iDispFlag[2]	= 1;
	}else {
		Feature_Attr	a_fDispSc[4];
		int	i = 0;
		std::map<int,const _TCHAR*> SC_Disptypes;
		SC_Disptypes[0] = annotation::kSC1DispType;
		SC_Disptypes[1] = annotation::kSC2DispType;
		SC_Disptypes[2] = annotation::kSC3DispType;
		SC_Disptypes[3] = annotation::kSC4DispType;
		for( i = 0; i < 4; i++)
		{
			a_fDispSc[i]	= c_fFAtt.Get_Attr ( SC_Disptypes[i] );
			a_iDispFlag[i]	= a_fDispSc[i].e_ccvAttr.intVal;
		}
	}

	IFeatureCursorPtr	a_ipAny_Note_Line_Cursor;
	int	a_iRoop	= 0;
	for( a_iRoop = a_iStart; a_iRoop < a_iEnd; a_iRoop++ )
	{
		if( a_iDispFlag[a_iRoop] == 0 ) continue;

		a_ipAny_Note_Line_Cursor	= c_mAny_Note_Line_WS[a_iRoop].Search_Where ( a_sWhereStr, RET_FCURSOR );
		if ( a_ipAny_Note_Line_Cursor == NULL ) {
			e_sPrnInfo.Prn_Info_Std ( "注記ラインフィーチャの取得に失敗", stderr );
			continue;
		}else {
		// ラインデータのループ
			IFeaturePtr		a_ipAny_Note_Line_Feat;
			for (;;)
			{
				if( a_ipAny_Note_Line_Cursor->NextFeature ( &a_ipAny_Note_Line_Feat ) == S_OK ) {
					disp_pt_info	a_dCurPtInfo;
					a_dCurPtInfo.e_iScale	= a_iRoop + 1;	// スケールを入れる
					// オブジェクトIDの取得
					a_ipAny_Note_Line_Feat->get_OID ( (long*)&a_dCurPtInfo.e_iOid );
					//　ジオメトリの取得
					IGeometryPtr	a_ipCurPtGeo;
					a_ipAny_Note_Line_Feat->get_Shape ( &a_ipCurPtGeo );
				//	a_dCurPtInfo.e_ipPt	= Get_Center_Pt( a_ipCurPtGeo );
					a_dCurPtInfo.e_ipPt	= Get_Center_Pt2( a_ipCurPtGeo, c_iMeshCode, a_dCurPtInfo.e_iScale );	// 記号のサイズは１６で固定
					c_vPoints.push_back ( a_dCurPtInfo );
				}else {
					break;
				}
			}
		}
	}
	
	return	( true );
}

/** ---------------------------------------------------------------------------
//	ラインから記号中心位置にあたる点を取り出す
-----------------------------------------------------------------------------*/
IPointPtr	main_rootin::
Get_Center_Pt	(	IGeometryPtr	c_ipGeo )	// ジオメトリ
{
	IPointPtr			a_ipRetPt(CLSID_Point);
	IPolylinePtr		a_ipPolyline(c_ipGeo);
	IPointCollectionPtr	a_ipPts(a_ipPolyline);

	IPointPtr	a_ipPt[2];
	double	a_dX[2] = {0.0, 0.0};
	double	a_dY[2] = {0.0, 0.0};
	int i = 0;
	for( i = 0; i < 2;i++ )
	{
		a_ipPts->get_Point ( i, &a_ipPt[i] );
		a_ipPt[i]->get_X ( &a_dX[i] );
		a_ipPt[i]->get_Y ( &a_dY[i] );
	}
	
	double	a_dXDiff	= (a_dX[1] - a_dX[0]) / 2.0;
	a_ipRetPt->put_X ( a_dX[0] + a_dXDiff );
	a_ipRetPt->put_Y ( a_dY[0] + a_dXDiff );
	return ( a_ipRetPt );
}

/** ---------------------------------------------------------------------------
//	ラインから記号中心位置にあたる点を取り出す
-----------------------------------------------------------------------------*/
IPointPtr	main_rootin::
Get_Center_Pt2	(	IGeometryPtr	c_ipGeo, 		///< ジオメトリ
					int				c_iMeshCode,	///< メッシュコード
					int				c_iScale )		///< スケール
//					int				c_iFontSize )	///< フォントサイズ
{
	IPointPtr			a_ipRetPt(CLSID_Point);
	IPolylinePtr		a_ipPolyline(c_ipGeo);
	IPointCollectionPtr	a_ipPts(a_ipPolyline);

	double	a_dXDiff = 0.0, a_dYDiff = 0.0;

//	Get_Degree_Range_Dev ( c_iMeshCode, c_iScale, c_iFontSize, &a_dXDiff, &a_dYDiff );
///** 2006.04.18 コメントアウト
	Get_Degree_Range_Dev ( c_iMeshCode, c_iScale, 16, &a_dXDiff, &a_dYDiff );
	// 計算誤差のためか、INデータとの間に+1ptの差ができるので、-1.0ptしておく 2006.08.10
	a_dXDiff	= a_dXDiff * (7.5-1.0) / 16.0;
	a_dYDiff	= a_dYDiff * (8.5-1.0) / 16.0;
//*/

/* 2006.08.09 コメントアウト
	Get_Degree_Range_Dev ( c_iMeshCode, c_iScale, 1, &a_dXDiff, &a_dYDiff );
	a_dXDiff	*= 8.0;
	a_dYDiff	*= 8.0;
*/
	IPointPtr	a_ipPt;
	double	a_dX = 0.0;
	double	a_dY = 0.0;
	a_ipPts->get_Point ( 0, &a_ipPt ); //　始点を取る
	a_ipPt->get_X ( &a_dX );
	a_ipPt->get_Y ( &a_dY );
	
	a_ipRetPt->put_X ( a_dX + a_dXDiff );
	a_ipRetPt->put_Y ( a_dY + a_dYDiff );
	return ( a_ipRetPt );
}

/** ---------------------------------------------------------------------------
	ポイントとアークの距離を求める
	@return	double	距離
-----------------------------------------------------------------------------*/
double	main_rootin::
Get_Dist	(	IPointPtr&				c_ipPt,			///< ポイント[IN]
				const IGeometryPtr&			c_ipLine	)	///< ライン[IN]
{
	//-------------------------------------------------------------------------
	// 距離用
	//-------------------------------------------------------------------------
	Dpos		a_dPt;	// ポイントの座標値
	a_dPt.xpos	= 0.0, a_dPt.ypos	= 0.0;

	c_ipPt->get_X ( &a_dPt.xpos );
	c_ipPt->get_Y ( &a_dPt.ypos );
	//-------------------------------------------------------------------------
	// ラインのデータを取得
	//-------------------------------------------------------------------------
	poly_cls			a_pLine;
	IPointCollectionPtr	a_ipLine_Pts;
	a_ipLine_Pts		= c_ipLine;
	
	// 全部の点の距離を求めて、リストにする
	long	a_lCount	= 0;
	a_ipLine_Pts->get_PointCount ( &a_lCount );

	int	roop_1	= 0;
	for(roop_1 = 0; roop_1 < a_lCount; roop_1++)
	{
		IPointPtr	a_ipCurPt;
		a_ipLine_Pts->get_Point	( roop_1, &a_ipCurPt );
	
		Dpos	a_dTmpPt;	// XYをそのまま使う

		a_ipCurPt->get_X	( &a_dTmpPt.xpos );
		a_ipCurPt->get_Y	( &a_dTmpPt.ypos );
		a_pLine.e_Pts.push_back ( a_dTmpPt );
	}
	double	a_dDist	= 0.0;
	a_dDist	= a_pLine.get_pt_arc_dist( a_dPt );	// この距離の求め方はOK！Get_Poly_Distは問題あり
	return	( a_dDist );
}

/** ---------------------------------------------------------------------------
//	SJISへの変換
//	@return	変換後文字列
-----------------------------------------------------------------------------*/
string	main_rootin::
Str_to_SJIS		(	CComBSTR*	c_CStr	)	// 文字列(Unicode)
{	
	string		ret_str;

#ifndef	_ARC_9_1_
	USES_CONVERSION;
	if (*c_CStr != 0) {
		ret_str = OLE2T(*c_CStr);
	}
#else
	long		length	= c_CStr->Length();

	char*		tmp_moji	= new char[length+1];

	memset		(tmp_moji, '\0', length+1);
	_bstr_t		a_bTmp ( *c_CStr, false );
//	wchar_t*	tmp_wide	= OLE2W(*c_CStr);
	wchar_t*	tmp_wide	= (wchar_t *)a_bTmp;

	if(tmp_wide == NULL) {
		ret_str	= "";
	}else {
		if(wcstombs( tmp_moji, tmp_wide, length+1 ) == -1){
			ret_str	= "";
		}else {
			ret_str	= tmp_moji;
		}
	}
	delete [] tmp_moji;
#endif	// endof _ARC_9_1_
	return	(ret_str);
}

/** ---------------------------------------------------------------------------
//	エラーログファイルをオープンする
//	@return	成功	true
//	@return	失敗	false
-----------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_cpLog_File == NULL ) {
		e_fErr_Fp	= stderr;
	}else {
		e_fErr_Fp	= fopen ( e_cpLog_File, "w" );
		if( e_fErr_Fp == NULL ) {
			fprintf	( stderr, "エラーログファイル<%s>がオープンできない[main_rootin]\n", e_cpLog_File );
			return	( false );
		}
	}

	return	( true );
}

/** ---------------------------------------------------------------------------
//	エラーログのヘッダを出力する
//	@return	無し
-----------------------------------------------------------------------------*/
void	main_rootin::
Output_Log_Header	( long		c_lDigit	)	///< メッシュ桁数[IN]
{
	fprintf	( e_fErr_Fp,		"#SINDYSTDLOG\n" );	// 2007.03
	fprintf	( e_fErr_Fp,		"#ツールバージョン          : <%s>\n", e_sVersion.c_str() );
	if( eExe_Flag & ON_PGDB_MODE ) {
		fprintf	( e_fErr_Fp,	"#接続情報(PGDB)            : <%s>\n",				e_cpPGDB_File );
	}else {
		fprintf	( e_fErr_Fp,	"#接続情報(DB_MAP)          : <%s/%s/%s/%s/%s>\n",	e_pTool_Init.get_UserName_Map(), e_pTool_Init.get_Password_Map(),
																					e_pTool_Init.get_Version_Map(),	e_pTool_Init.get_Instance_Map(),
																					e_pTool_Init.get_Server_Map() );
		fprintf	( e_fErr_Fp,	"#接続情報(DB_BASE)         : <%s/%s/%s/%s/%s>\n",	e_pTool_Init.get_UserName_Base(), e_pTool_Init.get_Password_Base(),
																					e_pTool_Init.get_Version_Base(), e_pTool_Init.get_Instance_Base(),
																					e_pTool_Init.get_Server_Base() );
	}
	fprintf	( e_fErr_Fp,		"#鉄道レイヤ                : <%s>\n", m_sAny_Railway_Layer.c_str() );
	fprintf	( e_fErr_Fp,		"#注記真位置レイヤ          : <%s>\n", m_sAny_Note_Layer.c_str() );
	if( c_lDigit == 8 ) {
			fprintf	( e_fErr_Fp,"#注記表示ラインレイヤ      : <%s>\n", m_sAny_Note_Line_Layer[2].c_str() );
	}else {
		int i = 0;
		for ( i = 0; i < 4; i++ )
		{
			fprintf	( e_fErr_Fp,"#注記表示ラインレイヤ%d     : <%s>\n", i+1, m_sAny_Note_Line_Layer[i].c_str() );
		}
	}
	fprintf	( e_fErr_Fp,		"#鉄道検索範囲              : <%0.0fpt>\n", e_dDev_Dist );
}

#define	OPTION_MAX	14
#define	BMAX		512

/** ---------------------------------------------------------------------------
//	引数チェック
//	@return	成功	true
//	@return	成功	false
-----------------------------------------------------------------------------*/
bool	main_rootin::
arg_check	(	int		argc,		// 引数個数
				char	**argv	)	// 引数列
{
	int		a_optcount[OPTION_MAX];	// オプションの重複のカウント
	int		i;
	int		a_exeflag	= 0;

	//-----------------------------------------------------------------------
	// ツールバージョン表示 2008.09.05
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	e_sVersion	= (char*)(_bstr_t(a_csVar));
	//-----------------------------------------------------------------------

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// 引数文字列数のチェック
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			fprintf	( stderr, "引数文字列が長すぎます!!\n" );
			return	( false );
		}
	}

	if(argc < 3) {
		return( false );
	}

//	e_cpLog_File		= argv[4];	///< ログデータ出力パス

	// オプションがなにも無いとき
	if( argc == 1 ) {
		return	( true );
	}

	// オプションだから、今のところ使用しない
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) {
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];		///< ログファイル
			a_exeflag		|= ON_LOGFILE;
			a_optcount[0]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-m") == 0) { // 2002.9.24
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-v") == 0) { // 2002.9.24
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[2]++;
			i+=1;
			return	( false );	// 仮にこうする？
		} else
		if(strcmp(argv[i], "-w") == 0) {		//	ワーニング距離指定 2002.9.26
		//	if(argc < i+2) return( false );
		//	e_Warn_Dist		= atof(argv[i+1]);
		//	a_optcount[3]++;
		//	i+=2;
		} else
		if(strcmp(argv[i], "-e") == 0) {		//	エラー距離指定 2002.9.26
		//	if(argc < i+2) return( false );
		//	e_Err_Dist		= atof(argv[i+1]);
		//	a_optcount[4]++;
		//	i+=2;
		} else
		if(strcmp(argv[i], "-M") == 0) { // 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag		|= ON_MONO_MESH;
			e_MeshCode		= atoi(argv[i+1]);	
			a_optcount[5]++;
			i+=2;
		} else 
		if(strcmp(argv[i], "-i") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_INIT_FILE;
			e_cpInit_File	= argv[i+1];	
			a_optcount[6]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-r") == 0) {		//	ワーニング距離指定 2002.9.26
			if(argc < i+2) return( false );
			e_dDev_Dist		= atof(argv[i+1]);
			a_optcount[7]++;
			i+=2;
		} else
	//	if(strcmp(argv[i], "-N") == 0) {		// 道路番号だけチェックの場合		// 2004.2.3
	//		//if(argc < i+1) return( false );
	//		a_exeflag		|= ON_ROADNUM_ONLY;
	//		a_optcount[8]++;
	//		i+=1;
	//	} else 
	//	if(strcmp(argv[i], "-F") == 0) {		// フェリー関係だけチェックの場合	// 2004.2.3
	//		//if(argc < i+1) return( false );
	//		a_exeflag		|= ON_FELLY_ONLY;
	//		a_optcount[9]++;
	//		i+=1;
	//	} else
		if(strcmp(argv[i], "-pgdb") == 0) {	/// 2007.02.23
			if(argc < i+2) return( false );
			e_cpPGDB_File	= argv[i+1];
			a_exeflag		|= ON_PGDB_MODE;
			a_optcount[10]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-addmsg") == 0) {		//	細分化不要のものについてもログを出力
			a_exeflag		|= ON_OUT_FUYOU_MESSAGE;
			a_optcount[11]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-ddsplmsg") == 0) {	//	表示ラインについてのメッセージを出力しない
			a_exeflag		|= ON_DEL_DISP_LINE_MESSAGE;
			a_optcount[12]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-nearest") == 0) {	//	一番近い路線とのみチェックを行う
			a_exeflag		|= ON_NEAREST_CHK;
			a_optcount[13]++;
			i+=1;
		} else {
			fprintf	(stderr, "オプション %s は間違ってます!!\n", argv[i]);
			return	( false );
		}
	}

	// 行政コードリストの指定と、メッシュコードリストの指定が同時に出来ないようにチェック
	if( (a_exeflag & ON_MESH) && (a_exeflag & ON_MONO_MESH) ) {
		fprintf	(stderr, "オプション '-m','-M',は同時に指定できません!!\n");
		return	( false );
	}else {
		if( !(a_exeflag & ON_MESH || a_exeflag & ON_MONO_MESH) ) {	// メッシュコードリストを読み込んでのチェックの場合
			fprintf	(stderr, "オプション '-m','-M',のどちらかを指定してください\n");
			return	( false );
		}
	}


	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			fprintf	(stderr, "オプションの指定が重複しています!!\n");
			return	( false );
		}
	}
	eExe_Flag	= a_exeflag;

	int	a_iInit_Ret	= 0;			// 2007.11.07
	if( a_exeflag & ON_INIT_FILE ) {// 2007.11.07
		a_iInit_Ret	= e_pTool_Init.read_init_file ( Ret_Init_File() );
	}else {
		a_iInit_Ret	= e_pTool_Init.read_init_env ( );
	}
	if( a_iInit_Ret != 0 ) {
		fprintf ( stderr, "初期化ファイル<%s>のオープンに失敗しました\n", Ret_Init_File() );
		return	( false );
	}

//	return		( a_exeflag );
	return		( true );

}

/** ---------------------------------------------------------------------------
//	ツールの使用法の表示
-----------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
			"=== 【鉄道路線注記細分化チェックツール】(%s) ===\n"
				"<usage>\n"
				"%s [オプション...]\n"
				"\t[オプション...]\n"
				"\t      '-r'               : デバイス座標での範囲を指定（指定ポイント以内であればOKとする）\n"
				"\t                         : ※デフォルト値は16ptとしている\n"
				"\t      '-i'               : 設定ファイル名\n"
				"\t      '-i'               : 設定ファイルを指定\n"
				"\t      '-l'               : ログファイル名を指定\n"
				"\t      '-m'               : メッシュコードリストファイルを指定\n"
				"\t      '-M'               : メッシュコードを指定（2次、8桁メッシュ）\n"
				"\t      '-pgdb'            : PGDBデータに対してのチェック\n"
				"\t                           ※SiNDY-C用の機能でないことに注意。\n"
				"\t      '-addmsg'          : 細分化不要のものについてもログを出力\n"
				"\t      '-ddsplmsg'        : 表示ラインについてのメッセージを出力しない\n"
				"\t      '-nearest'         : 一番近い路線とのみチェックを行う\n"
				"\t      '-update'			: ※未実装\n"
				"\t      '-v'               : バージョン情報表示\n"
				"\t[環境変数]\n"
				"\t  <共通>\n"
				"\t      DB_MAP                : <ユーザ名>/<パスワード>/<バージョン>/<インスタンス>/<サーバ>\n"
				"\t      DB_BASE               : <ユーザ名>/<パスワード>/<バージョン>/<インスタンス>/<サーバ>\n"
				"\t      FC_CITYMESH           : 都市地図メッシュFC\n"
				"\t      FC_BASEMESH           : 中縮尺メッシュFC\n"
				"\t      FC_MIDDLEMESH         : ミドルメッシュFC\n"
				"\t  <8桁メッシュ指定時>\n"
				"\t      FC_CITY_ANNOTATION    : 都市地図注記真位置FC\n"
				"\t      FC_CITY_DISP_LINE     : 都市地図注記ラインFC\n"
				"\t      FC_CITY_RAILWAY       : 都市地図鉄道ラインFC\n"
				"\t  <2次メッシュ指定時>\n"
				"\t      FC_BASE_ANNOTATION    : ベース注記真位置FC\n"
				"\t      FC_B_SC1DISP_LINE     : ベース注記S1表示ラインFC\n"
				"\t      FC_B_SC2DISP_LINE     : ベース注記S2表示ラインFC\n"
				"\t      FC_B_SC3DISP_LINE     : ベース注記S3表示ラインFC\n"
				"\t      FC_B_SC4DISP_LINE     : ベース注記S4表示ラインFC\n"
				"\t      FC_BASE_RAILWAY       : ベース鉄道ラインFC\n"
				"\t  <middleメッシュ指定時>\n"
				"\t      FC_MIDDLE_ANNOTATION  : ミドル注記真位置FC\n"
				"\t      FC_M_SC1DISP_LINE     : ミドル注記S1表示ラインFC\n"
				"\t      FC_M_SC2DISP_LINE     : ミドル注記S2表示ラインFC\n"
				"\t      FC_M_SC3DISP_LINE     : ミドル注記S3表示ラインFC\n"
				"\t      FC_M_SC4DISP_LINE     : ミドル注記S4表示ラインFC\n"
				"\t  ※上記の環境変数での設定は'-i'オプションでも同様に設定できます\n"
			//	"\t      '-w'               : ワーニング距離指定(単位:km)\n"
			//	"\t      '-e'               : エラー距離指定(単位:km)\n"
			, e_sVersion.c_str(), argv[0]);
}

/** ---------------------------------------------------------------------------
//	バージョン情報の表示
//	@return		無し
-----------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	fprintf(	stdout,
			"=== 【鉄道路線注記細分化チェックツール】(%s) ===\n"
				"\t<バージョン情報>\n"
				"\t2008.09.16     0.9.0.1            ・新規作成（試用バージョン）\n"
				"\t2008.09.16     0.9.1.2            ・[Bug 6409]への対応。（真位置の情報を追加）\n"
				"\t2008.10.17     1.0.0.3            ・正式公開版ツール\n"
			 	, e_sVersion.c_str());
}

/** ---------------------------------------------------------------------------
//	メッシュ矩形をDBから獲得するメソッドの初期化 \
//	※get_mesh_rect をやる前に必ず呼ぶ
-----------------------------------------------------------------------------*/
bool	main_rootin::
get_mesh_init	(	string			c_sMeshLayer,		// メッシュレイヤ名
					IWorkspacePtr	c_ipWorkSpace	)	// 指定するワークスペース
{
	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// フィーチャワークスペース

	// レイヤのオープン
	char*	a_Layer_Name		= &c_sMeshLayer[0];		// こんなんでいいのか？？2002/4/30

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(a_Layer_Name), &e_ipF_Class )) ) {
		fprintf	(stderr, "レイヤ <%s> のオープンに失敗\n", a_Layer_Name);
		return	(false);
	}else {
		fprintf(stdout,	"<%s> レイヤをオープン\n", a_Layer_Name);
	}
	
	return	( true );
}

/** ---------------------------------------------------------------------------
//	メッシュ矩形をGeoDBから取得する \
//	※GeoDBへの接続が確立されていることが前提
-----------------------------------------------------------------------------*/
IEnvelopePtr	main_rootin::
get_mesh_rect	( int	c_iMeshCode	)	///< メッシュコード
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );

	char		a_tmpstr[30];	//　ここはなんとかしたい気がする

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	if(a_iDigit == 4) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に 2007.09.03 修正
		m_iDigit	= 4;
	}else
	if(a_iDigit == 6) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に
		m_iDigit	= 6;
	}else {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に 2007.09.03 修正
		m_iDigit	= 8;
	}
	_bstr_t		a_Where_Str ( a_tmpstr );
//	a_Where_Str	= T2BSTR(_T(a_tmpstr));		// stringをBSTRに変換

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// 空間検索フィルタ
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// 属性検索優先
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where句の設定
	
	// クリップ矩形作成
	IEnvelopePtr	a_ipMesh_Rect;	// メッシュ矩形
	a_ipMesh_Rect	= NULL;

	// サーチしてくる
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		fprintf	(stderr, "データの属性検索に失敗\n");
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;
		long					a_lSp_InitFlag	= 0;	// 空間参照を初期化したかどうかのフラグ

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// データをひとつ取り出す
				break;
			}
				
			IGeometryPtr		a_Clip_Shp;		// こいつにデータをコピーしてクリップする

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// フィーチャから
				fprintf	(stderr, "シェイプのコピーに失敗\n");
				continue;
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				fprintf	( stderr, "該当メッシュ<%d>の矩形がみつかりませんでした\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}
		}
	}
	return	( a_ipMesh_Rect );
}

/** ---------------------------------------------------------------------------
//	指定ポイントを中心に、指定したX,Y方向の幅を持った矩形を作成
-----------------------------------------------------------------------------*/
IEnvelopePtr	main_rootin::
make_rect	(	IPointPtr&		c_ipPt,				///< 中心ポイント
				double			c_dDegX,			///< X方向幅
				double			c_dDegY	)			///< Y方向幅
{
	IEnvelopePtr	a_ipRetRect (CLSID_Envelope);

	double	a_dCenter_X = 0.0, a_dCenter_Y = 0.0;

	c_ipPt->get_X ( &a_dCenter_X );
	c_ipPt->get_Y ( &a_dCenter_Y );

	ISpatialReferencePtr	a_ipSpRef;
	if( FAILED (c_ipPt->get_SpatialReference ( &a_ipSpRef ) )) {
		return	( NULL );
	}

	// 空間参照合わせ
	if( FAILED ( a_ipRetRect->putref_SpatialReference ( a_ipSpRef ) )) {
		return	( NULL );
	}

	// 座標値付与
	a_ipRetRect->put_XMin ( a_dCenter_X - (c_dDegX) );
	a_ipRetRect->put_XMax ( a_dCenter_X + (c_dDegX) );
	a_ipRetRect->put_YMin ( a_dCenter_Y - (c_dDegY) );
	a_ipRetRect->put_YMax ( a_dCenter_Y + (c_dDegY) );

	return	( a_ipRetRect );

}

/** ---------------------------------------------------------------------------
//	DBへの接続オープン
//	@return	true	成功
//	@return	false	失敗
-----------------------------------------------------------------------------*/
bool	main_rootin::
DB_Open	(	IWorkspacePtr&			c_ipWorkspace_Map,			
			IWorkspacePtr&			c_ipWorkspace_Base )
{
	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	fprintf	(	stdout,
				_T(	"DB_MAP 接続プロパティ：%s/%s/%s/%s/%s\n"),
				e_pTool_Init.get_UserName_Map(), 
				e_pTool_Init.get_Password_Map(),
				e_pTool_Init.get_Version_Map(),
				e_pTool_Init.get_Instance_Map(),
				e_pTool_Init.get_Server_Map() );

	fprintf	(	stdout,
				_T(	"DB_BASE 接続プロパティ：%s/%s/%s/%s/%s\n"),
				e_pTool_Init.get_UserName_Base(), 
				e_pTool_Init.get_Password_Base(),
				e_pTool_Init.get_Version_Base(),
				e_pTool_Init.get_Instance_Base(),
				e_pTool_Init.get_Server_Base() );

	CString connectStrMap, connectStrBase;
		connectStrMap.Format(_T("%s/%s/%s/%s/%s"),
		e_pTool_Init.get_UserName_Map(), e_pTool_Init.get_Password_Map(),
		e_pTool_Init.get_Version_Map(), e_pTool_Init.get_Instance_Map(), e_pTool_Init.get_Server_Map());

	connectStrBase.Format(_T("%s/%s/%s/%s/%s"),
		e_pTool_Init.get_UserName_Base(), e_pTool_Init.get_Password_Base(),
		e_pTool_Init.get_Version_Base(), e_pTool_Init.get_Instance_Base(), e_pTool_Init.get_Server_Base());

	c_ipWorkspace_Map = sindy::create_workspace(connectStrMap);
	c_ipWorkspace_Base = sindy::create_workspace(connectStrBase);

	// DBへ接続
	fprintf ( stdout, "都市地図系ＤＢ接続中..." );
	if( !c_ipWorkspace_Map ) {
		fprintf ( stderr, "都市地図系ＤＢに接続出来なかった\n" );
		return	( false );
	}else {
		fprintf ( stdout, "都市地図系ＤＢ接続完了\n" );
	}
	
	fprintf ( stdout, "中縮尺系ＤＢ接続中..." );
	if( !c_ipWorkspace_Base ) {
		fprintf ( stderr, "中縮尺系ＤＢに接続出来なかった\n", stderr );
		return	( false );
	}else {
		fprintf ( stdout, "接続完了\n" );
	}
	return	( true );
}

/** ---------------------------------------------------------------------------
//	チェックの初期化処理
-----------------------------------------------------------------------------*/
bool	main_rootin::
check_init	( void )
{
	// エラーログファイルのオープン
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}

	if( eExe_Flag & ON_PGDB_MODE ) {
		// パーソナルジオDB用です
		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );					// 接続プロパティー格納用
		IWorkspacePtr			ipWorkspace;

		// カレント直下にオープンしてみる
		_bstr_t	a_bsTmpFName	= e_cpPGDB_File;
		ipWorkspaceFactory->OpenFromFile( a_bsTmpFName, 0, &ipWorkspace );
		if( ipWorkspace == NULL )	{
		//	e_sPrnInfo.Prn_Info_Std ( "パーソナルジオDBがオープンできませんでした\n", stderr ); 
			fprintf	( stderr, "パーソナルジオDBがオープンできませんでした\n" );
			return	( false );
		}else {
			fprintf	( stdout, "接続完了\n" );
		//	e_sPrnInfo.Prn_Info_Std ("接続完了", stdout );
		}
		printf(_T("\n"));

		m_ipWSpace_Map	= ipWorkspace;
		m_ipWSpace_Base	= ipWorkspace;

	}else {
		// 都市地図系用接続
		IWorkspacePtr			ipWorkspace_Map;
		// 中縮尺系用接続
		IWorkspacePtr			ipWorkspace_Base;

		if( !DB_Open (ipWorkspace_Map, ipWorkspace_Base) ) {
			return	( false );
		}

		ATLASSERT( ipWorkspace_Map != NULL );
		ATLASSERT( ipWorkspace_Base != NULL );

		m_ipWSpace_Map  = ipWorkspace_Map;
		m_ipWSpace_Base = ipWorkspace_Base;
	}

	// エラーメッセージ用
	char	a_cpErrMsgBuf[512];

	// チェックメッシュリストの読み込み
	char*	a_cpList_File	= NULL;		//	リストファイル名
	int		a_iMDigit	= 0;

	if( eExe_Flag & ON_MESH ) {
		a_cpList_File	= Ret_Mesh_File ( );
		if( a_cpList_File == NULL ) {
			e_sPrnInfo.Prn_Info_Std ( "メッシュコードリストファイル名がとりだせませんでした\n" , stderr );
			return	( false );
		}	
		if( m_mChk_List.read_mesh_list ( a_cpList_File ) != 0 ) {	// ここでリストを読み込む
			sprintf	( a_cpErrMsgBuf, "メッシュコードリストファイル<%s>の読み込みに失敗\n", a_cpList_File );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			return	( false );
		}
	}else {
		if( m_mChk_List.read_mesh_list ( e_MeshCode ) != 0 ) {	// ここでリストを読み込む
			sprintf	( a_cpErrMsgBuf, "メッシュコードリストファイル<%s>の読み込みに失敗\n", a_cpList_File );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			return	( false );
		}
	}

	char*		a_cpMeshLayer	= NULL;

	a_iMDigit	= m_mChk_List.get_mesh_digit ();
	
	switch	( a_iMDigit )
	{
	case	8:	a_cpMeshLayer	= e_pTool_Init.get_City_Mesh ();	break;
	case	6:	a_cpMeshLayer	= e_pTool_Init.get_Base_Mesh();		break;
	case	4:	a_cpMeshLayer	= e_pTool_Init.get_Middle_Mesh();	break;
	default:
		fprintf	( stderr, "リストのメッシュ桁数には対応していません[check_exe]\n");
		return	( false );
	}

	if( !get_mesh_init( a_cpMeshLayer , m_ipWSpace_Map ) ) {	// メッシュのデータはどこのDBでも参照できるはず・・・
		e_sPrnInfo.Prn_Info_Std ( "メッシュレイヤの獲得の初期化に失敗[check_exe]\n" , stderr );
		return	( false );
	}
	
	switch	( a_iMDigit )
	{
	case	8:
		m_sAny_Railway_Layer		= e_pTool_Init.get_City_RailWay();
		m_sAny_Note_Layer			= e_pTool_Init.get_City_Note();
		m_sAny_Note_Line_Layer[2]	= e_pTool_Init.get_City_Note_Line();
		break;
	case	6:
		m_sAny_Railway_Layer		= e_pTool_Init.get_Base_Railway();
		m_sAny_Note_Layer			= e_pTool_Init.get_Base_Note();
		m_sAny_Note_Line_Layer[0]	= e_pTool_Init.get_Base_Note_Line1();
		m_sAny_Note_Line_Layer[1]	= e_pTool_Init.get_Base_Note_Line2();
		m_sAny_Note_Line_Layer[2]	= e_pTool_Init.get_Base_Note_Line3();
		m_sAny_Note_Line_Layer[3]	= e_pTool_Init.get_Base_Note_Line4();
		break;
	case	4:
		m_sAny_Railway_Layer		= e_pTool_Init.get_Base_Railway();
		m_sAny_Note_Layer			= e_pTool_Init.get_Middle_Note();
		m_sAny_Note_Line_Layer[0]	= e_pTool_Init.get_Middle_Note_Line1();
		m_sAny_Note_Line_Layer[1]	= e_pTool_Init.get_Middle_Note_Line2();
		m_sAny_Note_Line_Layer[2]	= e_pTool_Init.get_Middle_Note_Line3();
		m_sAny_Note_Line_Layer[3]	= e_pTool_Init.get_Middle_Note_Line4();
		break;
	default:
		e_sPrnInfo.Prn_Info_Std ( "桁数が間違い", stderr );
		return	( false );
	}

	if( !m_mAny_Railway_WS.Init ( m_ipWSpace_Base, (char*)m_sAny_Railway_Layer.c_str() ) ) {
		sprintf	( a_cpErrMsgBuf, "鉄道レイヤ<%s>の読み込みに失敗\n", m_sAny_Railway_Layer.c_str() );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
		return	( false );
	}

	if( !m_mAny_Note_WS.Init ( m_ipWSpace_Map, (char*)m_sAny_Note_Layer.c_str() ) ) {
		sprintf	( a_cpErrMsgBuf, "注記レイヤ<%s>の読み込みに失敗\n", m_sAny_Note_Layer.c_str() );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
		return	( false );	
	}
	
	if( a_iMDigit == 8 ) {
		if( !m_mAny_Note_Line_WS[2].Init ( m_ipWSpace_Map, (char*)m_sAny_Note_Line_Layer[2].c_str() ) ) {
			sprintf	( a_cpErrMsgBuf, "注記ラインレイヤ<%s>の読み込みに失敗\n", m_sAny_Note_Line_Layer[2] );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			return	( false );	
		}
	}else {
		for( int i = 0; i < 4; i++ )
		{
			if( !m_mAny_Note_Line_WS[i].Init ( m_ipWSpace_Map, (char*)m_sAny_Note_Line_Layer[i].c_str() ) ) {
				sprintf	( a_cpErrMsgBuf, "注記ラインレイヤ<%s>の読み込みに失敗\n", m_sAny_Note_Line_Layer[i].c_str() );
				e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
				return	( false );	
			}
		}
	}
	// ログのヘッダ情報を出力
	Output_Log_Header( a_iMDigit );
	return	( true );
}

/** ---------------------------------------------------------------------------
//	読み込んだデータをセットに落とす
//	@return	データ数
-----------------------------------------------------------------------------*/
int		main_rootin::
Get_Data_info (	IFeatureCursorPtr&	c_ipCursor,		///< チェック元カーソル[IN]
				ms_data_info&		c_ipGetData )	///< チェック元データのセット[OUT]
{
	int	a_iCount	= 0;
	// データをマルチセットのデータに落とす
	if( c_ipCursor != NULL ) {
		IFeaturePtr	a_ipFeature;
		// 検索された注記のループ
		for(;;)
		{
			data_info	a_dCur_Data;
			if( c_ipCursor->NextFeature ( &a_ipFeature ) == S_OK ) {

				Feature_All_Attrs	a_fAttrs;
				a_fAttrs.Init		( a_ipFeature );
				// オブジェクトID
			//	Feature_Attr			a_fObjID, a_fMeshCode, a_fPrim_ID, a_fAttr, a_fRP_F;
			//	a_fObjID				= a_fChk_Src_Attrs.Get_Attr ( "OBJECTID" );
			//	a_fAttr					= a_fChk_Src_Attrs.Get_Attr ( c_sAttrFName );
			//	a_dChk_Data.e_iAttr		= a_fAttr.e_ccvAttr.intVal;
			
				a_dCur_Data.m_iOID		= a_fAttrs.Get_Obj_ID();
				a_dCur_Data.m_ipGeo		= a_fAttrs.Ret_Geo();

				a_dCur_Data.m_faaAttr	= a_fAttrs;

				c_ipGetData.insert ( a_dCur_Data );
				a_iCount++;
			}else {
				break;
			}
		}
	}
	return	( a_iCount );
}

/*
 * @brief     地上の鉄道が細分化可能かを判定し、可能であるならABLE_SUBDIVIDEを返す
 * @note      Anno_Railway_Chk内で使用
 * @param[in] targetAttr チェック対象路線の鉄道種別
 * @param[in] rightAttr  路線と整合がとれている場合の鉄道種別
 * @param[in] underFlag  対象の鉄道が地上にあるか、地下にあるか
 * @retval    ABLE_SUBDIVIDE     細分化可能
 * @retval    RAIL_INCONSISTENT  路線と不整合
 * @retval    NONE_SUBDIVIDE     細分化の必要なし
*/
main_rootin::ERROR_MESSAGE_CODE checkRailwayClass_Ground(long targetAttr, long rightAttr, long underFlag )
{
	if( targetAttr == rightAttr ) //種別の整合が取れているなら、細分化できるか判定
	{
		if( underFlag == 1 )
			return main_rootin::ERROR_MESSAGE_CODE::ABLE_SUBDIVIDE;  // 細分化可能
	}
	else return main_rootin::ERROR_MESSAGE_CODE::RAIL_INCONSISTENT;  // 路線と不整合

	return main_rootin::ERROR_MESSAGE_CODE::NONE_SUBDIVIDE;          //細分化の必要なし
};

/*
 * @brief     地下フラグの状態と注記種別の整合が取れているか判定し、地上、地下の状態が反対になっているなら、GROUND_INCONSISTENTを返す
 * @note      Anno_Railway_Chk内で使用
 * @param[in] targetAttr チェック対象路線の鉄道種別
 * @param[in] rightAttr  路線と整合がとれている場合の鉄道種別
 * @param[in] underFlag  対象の鉄道が地上にあるか、地下にあるか
 * @retval    GROUND_INCONSISTENT 地上地下が反対
 * @retval    RAIL_INCONSISTENT   路線と不整合
 * @retval    NONE_SUBDIVIDE      整合は取れている
*/
main_rootin::ERROR_MESSAGE_CODE checkRailwayClass_Under(long targetAttr, long rightAttr, long underFlag )
{
	if( targetAttr == rightAttr)
	{
		if( underFlag == 0 )
			return main_rootin::ERROR_MESSAGE_CODE::GROUND_INCONSISTENT; // 地上地下が反対
	}
	else
		return main_rootin::ERROR_MESSAGE_CODE::RAIL_INCONSISTENT;       // 路線と不整合

	return main_rootin::ERROR_MESSAGE_CODE::NONE_SUBDIVIDE;              // 整合は取れている
};


/** ---------------------------------------------------------------------------
//	注記と鉄道の属性を見てチェック
//	@return	データ数
-----------------------------------------------------------------------------*/
void	main_rootin::
Anno_Railway_Chk (	data_info&				c_dAnnoData,		///< 注記データ[IN]
					disp_pt_info&			c_dCurDat,			///< 表示位置データ[IN]
					ms_data_info&			c_mRailway_Data	)	///< 検索で得られた鉄道データ[IN]
{
	using namespace sindy::schema::city_railway;
	namespace anno_code = annotation::annotation_code;

	// 注記属性取得
	Feature_Attr	a_fAnno_Attr = c_dAnnoData.m_faaAttr.Get_Attr ( annotation::kAnnoCode );
	long			a_lAnno_Attr = a_fAnno_Attr.e_ccvAttr.intVal;
	Feature_Attr	a_fAnno_Str  = c_dAnnoData.m_faaAttr.Get_Attr ( annotation::kNameString1 );
	string			a_sAnno_Str  = Str_to_SJIS(	(CComBSTR*)&a_fAnno_Str.e_ccvAttr.bstrVal );

	char	a_cpErrMsgBuf[512];

	// 距離順にソート
	map<double, data_info, less<double> >	a_dDist_Sorted_Railway;
	ims_data_info	a_iIndx;
	for( a_iIndx = c_mRailway_Data.begin(); a_iIndx != c_mRailway_Data.end(); a_iIndx++ )
	{
		// 距離を求める
		double	a_dDist	= 0.0;
		a_dDist	= Get_Dist ( c_dCurDat.e_ipPt, a_iIndx->m_ipGeo );
		a_dDist_Sorted_Railway.insert ( map<double, data_info, less<double> >::value_type ( a_dDist, *a_iIndx ) );
	}

	// ソートしたデータでループさせる
	map<double, data_info, less<double> >::iterator	a_iSIndx;
	for( a_iSIndx = a_dDist_Sorted_Railway.begin(); a_iSIndx != a_dDist_Sorted_Railway.end(); a_iSIndx++ )
	{
		data_info a_dRailway = (*a_iSIndx).second;

		// 距離を求める
		double a_dDist = 0.0;
		a_dDist	= Get_Dist ( c_dCurDat.e_ipPt, a_dRailway.m_ipGeo );

		// 鉄道属性取得
		//都市地図メッシュが対象か否かによって、フィールド名を変える
		Feature_Attr	a_fRailway_Attr = a_dRailway.m_faaAttr.Get_Attr( m_iDigit==8 ?
			city_railway::kRailwayClass : base_railway::kRailwayClass );
		long			a_lRailway_Attr = a_fRailway_Attr.e_ccvAttr.intVal;

		//都市地図メッシュが対象か否かによって、フィールド名を変える
		Feature_Attr	a_fUnderGround	= a_dRailway.m_faaAttr.Get_Attr ( m_iDigit==8 ?
			city_railway::kUnderGround : base_railway::kUnderGround );
		///0:地上 1:地下
		long			a_lUnderGround	= a_fUnderGround.e_ccvAttr.intVal;

		/// エラー状態
		ERROR_MESSAGE_CODE a_lErr_Stat = NONE_SUBDIVIDE;

		if( m_iDigit != 8 ) {//ベース注記、あるいは、ミドル注記用のチェック
			// 属性でチェック
			switch ( a_lAnno_Attr )
			{
			case anno_code::kShinkansenLineName:  // 1101101 新幹線路線名（地上）
			case anno_code::kJRLineName:          // 1101102 ＪＲ路線名（地上）
			case anno_code::kQuasiPublicLineName: // 1101103 第３セクター路線名（地上）
			case anno_code::kPrivateLineName:     // 1101104 私鉄路線名（地上）
			case anno_code::kSubwayLineName:      // 1101106 地下鉄路線名（地上）
			case anno_code::kMonorailLineName:    // 1101107 モノレール路線名（地上）
			case anno_code::kShinkotsuLineName:   // 1101108 新交通路線名（地上）
				if( a_lRailway_Attr == (a_lAnno_Attr-1101100) ) {// 各路線種別との比較
					if( a_lUnderGround == 1 ) {
						a_lErr_Stat	= ABLE_SUBDIVIDE;	// 細分化可能
					}
				}else {
					a_lErr_Stat	= RAIL_INCONSISTENT;		// 路線と不整合
				}
				break;
			case anno_code::kShinkansenSubLineName:  // 1101201 新幹線路線名（地下）
			case anno_code::kJRSubLineName:          // 1101202 ＪＲ路線名（地下）
			case anno_code::kQuasiPublicSubLineName: // 1101203 第３セクター路線名（地下）
			case anno_code::kPrivateSubLineName:     // 1101204 私鉄路線名（地下）
			case anno_code::kSubwaySubLineName:      // 1101206 地下鉄路線名（地下）
			case anno_code::kMonorailSubLineName:    // 1101207 モノレール路線名（地下）
			case anno_code::kShinkotsuSubLineName:   // 1101208 新交通路線名（地下）
				if( a_lRailway_Attr == (a_lAnno_Attr-1101200) ) {// 各路線種別との比較
					if( a_lUnderGround == 0 ) {
						a_lErr_Stat	= GROUND_INCONSISTENT;	// 地上地下が反対
					}
				}else {
					a_lErr_Stat	= RAIL_INCONSISTENT;		// 路線と不整合
				}
				break;
			default:
				a_lErr_Stat	= NON_SUBJECT;	// 対象外種別
				break;
			}
		}else {	// 都市地図
			// 属性でチェック
			switch ( a_lAnno_Attr )
			{
			//地上に関する判定
			case anno_code::kShinkansenLineName:  // 1101101 新幹線路線名（地上）
			case anno_code::kJRLineName:          // 1101102 ＪＲ路線名（地上）
			case anno_code::kQuasiPublicLineName: // 1101103 第３セクター路線名（地上）
			case anno_code::kPrivateLineName:     // 1101104 私鉄路線名（地上）
				//鉄道種別が普通鉄道で地下にあるなら細分化可能
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kNormalRailway, a_lUnderGround);
				break;
			case anno_code::kSubwayLineName:      // 1101106 地下鉄路線名（地上）
				//鉄道種別が地下鉄で地下にあるなら細分化可能
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kSubway, a_lUnderGround);
				break;
			case anno_code::kMonorailLineName:    // 1101107 モノレール路線名（地上）
				//鉄道種別がモノレールで地下にあるなら細分化可能
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kMonorail, a_lUnderGround);
				break;
			case anno_code::kShinkotsuLineName:   // 1101108 新交通路線名（地上）
				//鉄道種別が新交通システムで地下にあるなら細分化可能
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kUniversalTraffic, a_lUnderGround);
				break;
			case anno_code::kCableCarLineName:    // 1101110 ケーブルカー路線名
				//鉄道種別が新交通ケーブルカーで地下にあるなら細分化可能
				a_lErr_Stat = checkRailwayClass_Ground(a_lRailway_Attr, railway_class::kCableCar, a_lUnderGround);
				break;
			//以下、地下に関する判定
			case anno_code::kShinkansenSubLineName:  // 1101201 新幹線路線名（地下）
			case anno_code::kJRSubLineName:          // 1101202 ＪＲ路線名（地下）
			case anno_code::kQuasiPublicSubLineName: // 1101203 第３セクター路線名（地下）
			case anno_code::kPrivateSubLineName:     // 1101204 私鉄路線名（地下）
				//鉄道種別が普通鉄道で地上にあるなら地上地下が逆
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kNormalRailway, a_lUnderGround);
				break;
			case anno_code::kSubwaySubLineName:      // 1101206 地下鉄路線名（地下）
				//鉄道種別が地下鉄で地上にあるなら地上地下が逆
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kSubway, a_lUnderGround);
				break;
			case anno_code::kMonorailSubLineName:    // 1101207 モノレール路線名（地下）
				//鉄道種別がモノレールで地上にあるなら地上地下が逆
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kMonorail, a_lUnderGround);
				break;
			case anno_code::kShinkotsuSubLineName:   // 1101208 新交通路線名（地下）
				//鉄道種別が新交通システムで地上にあるなら地上地下が逆
				a_lErr_Stat	= checkRailwayClass_Under(a_lRailway_Attr, railway_class::kUniversalTraffic, a_lUnderGround);
				break;
			default:
				a_lErr_Stat	= NON_SUBJECT; // 対象外種別
				break;
			}
		}

		//-----------------------------------------------------------------------
		// エラー出力
		// <注記属性値>\t<注記属性名称>\t<鉄道属性値>\t<鉄道属性名称>\t<真位置か？>\t<スケール>メッセージ

		e_sPrnInfo.Set_OID  ( c_dCurDat.e_iOid );
		if( c_dCurDat.e_iStat == 1 ) {
			e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Layer.c_str() );
		}else {
			e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Line_Layer[c_dCurDat.e_iScale-1].c_str() );
		}

		// 属性名称取得(都市地図は鉄道属性*10+地下フラグで検索)

		long a_lRailKey = 0;
		if( m_iDigit == 8 ) {
			a_lRailKey = a_lRailway_Attr*10+a_lUnderGround;
		}else {
			a_lRailKey = a_lRailway_Attr;
		}

		string a_sAnnoAttr, a_sRailAttr;
		map<long, string, less<long> >::iterator a_iNameIndx;
		a_iNameIndx	= m_mAttrStrTbl.find ( a_lAnno_Attr );		// 注記属性名
		if( a_iNameIndx !=  m_mAttrStrTbl.end() ) {
			a_sAnnoAttr	= (*a_iNameIndx).second;
		}
		a_iNameIndx	= m_mAttrStrTbl.find ( a_lRailKey );		// 鉄道属性名
		if( a_iNameIndx !=  m_mAttrStrTbl.end() ) {
			a_sRailAttr	= (*a_iNameIndx).second;
		}

		string a_sErrMsg;

		switch (a_lErr_Stat)
		{
		case NONE_SUBDIVIDE:      //!< 細分化の必要無し
			a_sErrMsg = "細分化の必要なし";
			break;
		case ABLE_SUBDIVIDE:      //!< 細分化可能
			a_sErrMsg = "細分化可能";
			break;
		case RAIL_INCONSISTENT:   //!< 路線と不整合
			a_sErrMsg = "路線と不整合";
			break;
		case GROUND_INCONSISTENT: //!< 地上地下が反対
			a_sErrMsg = "地上地下が反対";
			break;
		default:                  //!< 対象外種別
			a_sErrMsg = "対象外種別";
			break;
		}

		string	a_sStat	= "真位置";
		if( c_dCurDat.e_iStat == 0 ) a_sStat = "表示位置";

		sprintf	( a_cpErrMsgBuf, "%s\t%s\tS%d\t%d\t%s\t%d\t%s\t%d\t%s\t%d\t%0.10f",
			a_sErrMsg.c_str(), a_sStat.c_str(), c_dCurDat.e_iScale, c_dAnnoData.m_iOID, a_sAnno_Str.c_str(), a_lAnno_Attr, a_sAnnoAttr.c_str(), a_lRailway_Attr, a_sRailAttr.c_str(), a_dRailway.m_iOID, a_dDist ); 


		// 細分化の必要なしも表示するかの判別
		if( !(eExe_Flag & ON_OUT_FUYOU_MESSAGE) ) {
			if( a_lErr_Stat == NONE_SUBDIVIDE ) continue;
		}
		e_sPrnInfo.Set_Err_Msg ( a_cpErrMsgBuf );
		e_sPrnInfo.Prn_SiNDY_Err ( 0, NULL, NULL );

		// 一番近いところだけやったら終了
		if( eExe_Flag & ON_NEAREST_CHK ) {
			break;
		}
		//-----------------------------------------------------------------------
	}

}

/** ---------------------------------------------------------------------------
//	チェック実行部
//	※内部でエラーファイルの処理等も行う
//	@return	成功	true
//	@return 失敗	false
-----------------------------------------------------------------------------*/
bool	main_rootin::
check_exe	( void )
{
	namespace anno_code = sindy::schema::annotation::annotation_code;

	//HRESULT				hr;

	// チェックの初期化
	if( !check_init() ) {
		return	( false );
	}

	// エラーログ用の設定
	e_sPrnInfo.Set_Layer_Name ( (char*)m_sAny_Note_Layer.c_str() );

	//--------------------------------------------------------------------------------------

	// キャッシュ上にとってくる
	// オンメモリテスト 2002.6.24
	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Map	( m_ipWSpace_Map );
	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Base	( m_ipWSpace_Base );

	//	1101101	新幹線路線名（地上）
	//	1101102	ＪＲ路線名（地上）
	//	1101103	第３セクター路線名（地上）
	//	1101104	私鉄路線名（地上）
	//	1101106	地下鉄路線名（地上）
	//	1101107	モノレール路線名（地上）
	//	1101108	新交通路線名（地上）
	//	1101201	新幹線路線名（地下）
	//	1101202	ＪＲ路線名（地下）
	//	1101203	第３セクター路線名（地下）
	//	1101204	私鉄路線名（地下）
	//	1101206	地下鉄路線名（地下）
	//	1101207	モノレール路線名（地下）
	//	1101208	新交通路線名（地下）
	// メッシュ矩形で注記データをサーチする
	CString a_sNote_Where;	// 内部固定値
//	if( m_iDigit == 8 ) {
	a_sNote_Where.Format(_T("((%s >= %ld AND %s <= %ld) OR (%s >= %ld AND %s <= %ld)) AND %s <> %ld AND %s <> %ld"),
		annotation::kAnnoCode, anno_code::kShinkansenLineName,
		annotation::kAnnoCode, anno_code::kShinkotsuLineName,
		annotation::kAnnoCode, anno_code::kShinkansenSubLineName,
		annotation::kAnnoCode, anno_code::kShinkotsuSubLineName,
		annotation::kAnnoCode, anno_code::kStreetCarLineName,
		annotation::kAnnoCode, 1101205); // コード値1101205の注記種別は存在しない 2014/03/20現在

	char	a_cpErrMsgBuf[512];

	int		a_iCur_Mesh	= 0;
	// メッシュ単位の処理
	for( a_iCur_Mesh = m_mChk_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = m_mChk_List.get_mesh_next() )
	{
		e_sPrnInfo.Set_Mesh		( a_iCur_Mesh );
		e_sPrnInfo.Set_Err_Level( smp_sindy_err_info::EC_ERROR );

		// メッシュごとにキャッシュマネージャをクリアしておく
	//	hr	= a_ipSpCacheMgr_Map->EmptyCache( );
	//	if( FAILED ( hr ) ) {
	//		a_sPrnInfo.Prn_Light_Msg ( 2, "都市地図系データ用キャッシュマネージャをクリアできませんでした", stderr );
	//		continue;
	//	}

		sprintf	( a_cpErrMsgBuf, "メッシュコード [%d] チェック中\n", a_iCur_Mesh );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stdout );

		// メッシュ矩形の取り出し
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// メッシュ矩形が無い場合
			sprintf	( a_cpErrMsgBuf, "メッシュコード<%d>の矩形が見つかりませんでした\n", a_iCur_Mesh );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			continue;
		}
		
		IEnvelopePtr	a_ipRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// 空間キャッシュマネージャにメッシュ内のデータをとりだす
	//	a_ipSpCacheMgr_Map->FillCache( a_ipRect );
	//	a_ipSpCacheMgr_Base->FillCache( a_ipRect );

		IFeatureCursorPtr	a_ipAny_Note_Cursor;
		a_ipAny_Note_Cursor	= m_mAny_Note_WS.Search_Feature( a_ipMeshRect, esriSpatialRelIntersects, (LPCTSTR)a_sNote_Where, RET_COUNT|RET_FCURSOR );
		if ( a_ipAny_Note_Cursor == NULL ) {
			sprintf	( a_cpErrMsgBuf, "<%d>メッシュ矩形でデータが取れません\n", a_iCur_Mesh );
			e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stderr );
			continue;
		}

		//-----------<チェック進行度表示>--------------------
		long		i			= 0;
		long		a_lTCount	= 0;
		a_lTCount	= m_mAny_Note_WS.Get_Data_Count ();
		sprintf	( a_cpErrMsgBuf, "メッシュコード [%d] 総データ件数 %d 件\n", a_iCur_Mesh, a_lTCount );
		e_sPrnInfo.Prn_Info_Std ( a_cpErrMsgBuf, stdout );
		//--------------------------------------------------

		ms_data_info	a_mAnnotation_Data;
		long			a_lCount	= 0;

		// データをコンテナにする
		a_lCount		= Get_Data_info ( a_ipAny_Note_Cursor, a_mAnnotation_Data );

		//--------------------------------------------------------------------
		e_sPrnInfo.Set_Mesh			( a_iCur_Mesh );
		e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR );
		e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Layer.c_str() );
		//--------------------------------------------------------------------

		ims_data_info	a_iAnnoIndx;	// 注記真位置のインデックス
		for( a_iAnnoIndx = a_mAnnotation_Data.begin(); a_iAnnoIndx != a_mAnnotation_Data.end(); a_iAnnoIndx++ )
		{
			data_info		a_dAnnoData	= *a_iAnnoIndx;

			Feature_Attr	a_fStr_1	= a_dAnnoData.m_faaAttr.Get_Attr ( annotation::kNameString1 );
			_bstr_t			a_bsAnno_Str= a_fStr_1.e_ccvAttr.bstrVal;
			Feature_Attr	a_fAttr_Code= a_dAnnoData.m_faaAttr.Get_Attr ( annotation::kAnnoCode );
			long			a_lAttr_Code= a_fAttr_Code.e_ccvAttr.intVal;

			// ログ要OIDのセット
			e_sPrnInfo.Set_OID		( a_dAnnoData.m_iOID );

			// ここで真位置からデバイス座標を計算し、緯度経度に戻しポイントを作成してチェックをかける
		//	IGeometryPtr	a_ipPtGeo;
		//	if( FAILED (a_ipAny_Note_Feat->get_Shape ( &a_ipPtGeo )) ) {
		//		e_sPrnInfo.Prn_Info_Std ( "シェイプの取得に失敗\n", stderr );
		//		continue;
		//	}
			
			// 注記真位置から、表示ラインも含めたチェック用の先頭ポイントを取り出す
			vector<disp_pt_info>	a_vChk_Pts;
			if( !Get_Disp_Pts	( a_iCur_Mesh, a_dAnnoData.m_faaAttr, m_mAny_Note_Line_WS, a_vChk_Pts ) ) {
				e_sPrnInfo.Prn_Info_Std ( "表示ラインの情報取得に失敗\n", stderr );
				continue;
			}
			
			vector<disp_pt_info>::iterator	a_viIndx;
			for( a_viIndx = a_vChk_Pts.begin(); a_viIndx != a_vChk_Pts.end(); a_viIndx++ )
			{
				disp_pt_info	a_dCurDat	= *a_viIndx;

				// デバイス座標から緯度経度にする
				// サーチ用のポイントを作成
				IPointPtr	a_ipPoint;
				a_ipPoint	= a_dCurDat.e_ipPt;	// ポイント取り出し
			//	a_ipPoint	= 	get_Device_Pt	( a_iCur_Mesh, (IPointPtr)a_ipPtGeo, 4 );	// スケール４でチェック
		
				e_sPrnInfo.Set_OID		( a_dCurDat.e_iOid );
				if( a_dCurDat.e_iStat == 1 ) {
					e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Layer.c_str() );
				}else {
					e_sPrnInfo.Set_Layer_Name	( (char*)m_sAny_Note_Line_Layer[a_dCurDat.e_iScale-1].c_str() );
				}

				//-------------------------------------------------------------------
				//　道路関係のチェック
				//-------------------------------------------------------------------
				// 属性で切り分ける
				IFeatureCursorPtr			a_ipRailway_Intersect_Cursor;

				//----------------------------------------------------------------------------------
				// ここでジオメトリバッファを作成する
				//----------------------------------------------------------------------------------
				double		a_dDegree	= 0.0;
				a_dDegree	= Get_Degree_Range_Dev ( a_iCur_Mesh, a_dCurDat.e_iScale, (int)Get_Dev_Dist() );

				IGeometryPtr			a_ipBold_Search_Geo;
			//	ITopologicalOperatorPtr	a_ipBold_Line ( a_ipPoint );	// 2006.06.08
			//	if ( FAILED ( a_ipBold_Line->Buffer ( a_dDegree , &a_ipBold_Search_Geo ) ) ) {
			//		e_sPrnInfo.Set_Err_Msg("ジオメトリバッファが作成できない");
			//		e_sPrnInfo.Prn_SiNDY_Err( 2, NULL, stderr );
			//		continue;
			//	}

				// 真ロジック
				double	a_dDeg_X = 0.0, a_dDeg_Y = 0.0;

				Get_Degree_Range_Dev ( a_iCur_Mesh, a_dCurDat.e_iScale, (int)Get_Dev_Dist(), &a_dDeg_X, &a_dDeg_Y );

				IEnvelopePtr	a_ipTmpRect;
				a_ipTmpRect		= make_rect	(a_ipPoint,	a_dDeg_X, a_dDeg_Y );
				a_ipBold_Search_Geo	= a_ipTmpRect;

				//----------------------------------------------------------------------------------
				int	a_iInterSect_Count	= 0;
				a_ipRailway_Intersect_Cursor	= m_mAny_Railway_WS.Search_Feature ( a_ipBold_Search_Geo, esriSpatialRelIntersects, "", RET_COUNT|RET_FCURSOR );
				a_iInterSect_Count				= m_mAny_Railway_WS.Get_Data_Count();						

				char	a_cpTmpStr[512];
				if( a_ipRailway_Intersect_Cursor == NULL ) {
					e_sPrnInfo.Set_Err_Msg ( "鉄道路線データの検索に失敗しています" );
					e_sPrnInfo.Prn_SiNDY_Err ( 0, NULL, NULL );
				}else {
					if( a_iInterSect_Count == 0 ) {
					//	sprintf( a_cpTmpStr, "%s\tスケール\t%d\t該当する鉄道路線が見つかりません", (char*)a_sStat.c_str(), a_dCurDat.e_iScale );
						Feature_Attr	a_fAnno_Str  = a_dAnnoData.m_faaAttr.Get_Attr ( "NAME_STRING1" );
						string			a_sAnno_Str  = Str_to_SJIS(	(CComBSTR*)&a_fAnno_Str.e_ccvAttr.bstrVal );
						string			a_sStat	= "真位置";
						if( a_dCurDat.e_iStat == 0 ) a_sStat = "表示位置";

						string	a_sAnnoAttr;
						map<long, string, less<long> >::iterator	a_iNameIndx;
						a_iNameIndx	= m_mAttrStrTbl.find ( a_lAttr_Code );		// 注記属性名
						if( a_iNameIndx !=  m_mAttrStrTbl.end() ) {
							a_sAnnoAttr	= (*a_iNameIndx).second;
						}

						sprintf	( a_cpTmpStr, "該当する鉄道路線が見つかりません\t%s\tS%d\t%d\t%s\t%d\t%s\t\t\t\t%0.10f",
							a_sStat.c_str(), a_dCurDat.e_iScale, a_dAnnoData.m_iOID, a_sAnno_Str.c_str(), a_lAttr_Code, a_sAnnoAttr.c_str(), 0.0 ); 

						e_sPrnInfo.Set_Err_Msg ( a_cpTmpStr );							
						e_sPrnInfo.Prn_SiNDY_Err ( 0, NULL, NULL );
					}else {
						ms_data_info	a_mRailway_Data;	// データ
						long			a_lRail_Count	= 0;// コンテナ
						// データをコンテナにする
						a_lRail_Count	= Get_Data_info ( a_ipRailway_Intersect_Cursor, a_mRailway_Data );
						// 注記と鉄道の属性を見てチェック
						Anno_Railway_Chk ( a_dAnnoData, a_dCurDat, a_mRailway_Data );
					}		
				}
			}
			//-------------------------------------------------------------------
			if( i > 0 && (i % 100) == 0) {	// 
				// カウント100件ごとにメッセージを表示
				fprintf( stdout, "メッシュコード [%d]  %d 件中　%d 件 チェック終了 \n", a_iCur_Mesh, a_lTCount, i );
			}
			//-------------------------------------------------------------------
		}
		fprintf	( stdout, "メッシュコード [%d] チェック終了\n", a_iCur_Mesh );
	}

	return	( true );
}

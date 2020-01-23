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

#include "MSiGet_Parts_Info.h"


/**
	エラープリント
	@return	無し
*/
void	msi_get_parts_info::
Error_Print	(	char*	c_cpErr_Code,		///< エラーコード
				char*	c_cpErr_Str		)	///< 出力エラー文字列
{
	fprintf	( e_fpLog,	"0\t%s\t%d\t\t\tERROR\t%s\t%s\t%d\n",
					
						&e_sLayer_Name[0],	// レイヤ名称
						e_lObj_ID,			// オブジェクトID
						c_cpErr_Code,		// エラーコード
						c_cpErr_Str,
						e_iMeshCode );		// メッシュコード 2003.6.23		// 出力エラー文字列						
}

/**
	初期化(このときに情報を取り出す)
	※ジオメトリのクローンを作成して分解する
	@return	なし
*/
bool	msi_get_parts_info::
Init(	IGeometryPtr	c_ipGeo,		///< ジオメトリ
		long			c_lObj_ID,		///< オブジェクトID
		string			c_sLayer_Name,	///< レイヤ名称
		int				c_iMeshCode,	///< メッシュコード
		FILE*			c_fpLog,		///< ログファイル用
		int				c_iMode	)		///< 再構成モードの設定（デフォルトは再構成しない）
{
	//　queryでリングを取った後は、元の構造が壊れるようなので、コピーしておき、元は
	//　壊さないようにする。
	IClonePtr	ipClone;
	((IClonePtr)c_ipGeo)->Clone( &ipClone );
	
	e_ipGeo			= (IGeometryPtr)ipClone;

	e_lObj_ID		= c_lObj_ID;
	e_sLayer_Name	= c_sLayer_Name;

	e_iMeshCode		= c_iMeshCode;
	e_fpLog			= c_fpLog;

	HRESULT			hr;

	if( FAILED(	c_ipGeo->get_GeometryType( &e_eGeoType )) ) {
		Error_Print ( "Init_01", "ジオメトリタイプがとれません" );
		return		( false );
	}

	long			col_num	= 0;	// コレクションの数

	e_ipGeoCol		= e_ipGeo;		// ジオメトリコレクションに入れてみる
	
	if( FAILED( e_ipGeoCol->get_GeometryCount( &col_num )) ) {
		Error_Print ( "Init_02", "ジオメトリカウントがとれません" );
		return		( false );
	}
#ifdef	_CHECK_
	if( col_num >= 2 ) {
		printf("col_num = %d\n", col_num );
	}
#endif
	if( e_eGeoType != esriGeometryPolygon ) {	// ポリゴン以外のとき

		e_lPart_Num		= col_num;		// パートの数は、そのままコレクションの数
		e_lSubPart_Max	= 1;			// サブパートの最大値は1とする

	}else {		// ポリゴンの場合

		// ここで既にInitされていたときは、解放して新たにメモリ確保
		Pre_Init_Chk ();
	
		e_ipPolygon		= e_ipGeo;

		// 2003.08.21追加 ---------------------------------------------------------
		IPointCollectionPtr		a_ipTmpPts( e_ipPolygon );

		long	a_ipPtsCount = 0;	// 総補間点数

		hr	= a_ipTmpPts->get_PointCount ( &a_ipPtsCount );
		if ( FAILED( hr ) ) {
			Error_Print ( "Init_02", "get_PointCountに失敗" );
			return		( false );
		}

		//debug -----------------------
		double	tmpx, tmpy;
		IPointPtr	a_ipTmpPt;
		a_ipTmpPts->get_Point ( 0, &a_ipTmpPt );
		a_ipTmpPt->get_X( &tmpx );
		a_ipTmpPt->get_Y( &tmpy );		

		//--------------------------------
	
		e_wpPoints	= new WKSPoint[a_ipPtsCount];	// 点列の確保
		hr	= a_ipTmpPts->QueryWKSPoints ( 0, a_ipPtsCount, e_wpPoints );
		if ( FAILED( hr ) ){
			Error_Print ( "Init_02", "WKSPoint取得に失敗" );
			return		( false );
		}

		// 初期化する
		InitRing( a_ipPtsCount, e_wpPoints, e_lRings );
		// 点列の包含関係をとってくる
		CheckExteriorInterior( e_wpPoints, e_lRings );
		if( c_iMode == GPI_RECONFIG_MODE ) {	// ポリゴンの状態を見てここで再構成する
			if( Reconfig_Polygon() ) {		// ポリゴン再構成　2003.10.25
				Error_Print ( "RECONFIG", "ポリゴンの状態が正常でないので再構成します" );
			}
		}
		// アウターリング数の取得
		list<RING>::iterator	a_iIndx;
		for( a_iIndx = e_lRings.begin(); a_iIndx != e_lRings.end(); a_iIndx++ )
		{
			e_vChk_Rings.push_back(*a_iIndx);	//チェック用に放り込む（リストの最初から順に並ぶことになる）
		}
		int	a_iPoly_Number	= 0;
		for( a_iIndx = e_lRings.begin(); a_iIndx != e_lRings.end(); a_iIndx++, a_iPoly_Number++ )
		{
			RING	a_rOuter	= *a_iIndx;
			if( a_rOuter.IsClockwise == TRUE ) { // 2003.10.01
				e_vOuter.push_back( a_rOuter );	// 
				e_lPart_Num++;

				// 最大サブパート数を得る
				int a_iInner_Cnt = 0;
				a_iInner_Cnt	= Inner_Ring_Chk ( a_iPoly_Number );
				if( a_iInner_Cnt > e_lSubPart_Max ) {
					e_lSubPart_Max	= a_iInner_Cnt;
				}
			}
		}

#ifdef	_CHECK_
		if( e_lPart_Num > 2 ) {
			printf("pnum = %d\n", e_lPart_Num);
		}
#endif

		// <ここまで>---------------------------------------------------------------
		e_lSubPart_Max++;	// 外側の分1加える
	}
	return	( true );
}


/**
	リングが内包する中抜きリング（サブパート）のチェック用
	※基本的にInit内だけで使用
	※自分が右回りポリゴンであることが前提
	@return	中抜きリング数
*/
int		msi_get_parts_info::
Inner_Ring_Chk(	int	c_iIndx	)	///< RINGポリゴンのインデックス
{
	int	a_iInner_Ring_Num	= 0;	// 内包リング数

	list<SIMPLERING>::iterator	a_iIndx, a_iBegin, a_iEnd;
	a_iBegin	= e_vChk_Rings[c_iIndx].Contain.begin();
	a_iEnd		= e_vChk_Rings[c_iIndx].Contain.end();
	// 自分が内包しているポリゴンについて
	for( a_iIndx = a_iBegin; a_iIndx != a_iEnd; a_iIndx++ )
	{
		SIMPLERING	a_sRing	= *a_iIndx;
		if( e_vChk_Rings[a_sRing.INDX].IsClockwise == FALSE ) {	// 左回り
			if( Is_Inner_Ring ( e_vChk_Rings[c_iIndx], e_vChk_Rings[a_sRing.INDX] ) ) {
				a_iInner_Ring_Num++;
			}
		}
	}
	return ( a_iInner_Ring_Num );
}

/** 2003.10.03
	リングに対して内側リングが直の中抜きかどうかをチェックする
	※Init後に使用
	@return	bool
*/
bool	msi_get_parts_info::
Is_Inner_Ring(		RING&	c_rOuter,	///< 外側ポリゴン
					RING&	c_rInner )	///< 内側ポリゴン
{
	
	// c_rInnerのWithinポリゴンが、c_rOuterのWithinポリゴン＋c_rOuterを含めば直接の中抜きリング
	int	a_iWithin_Diff	= c_rInner.Within.size() - c_rOuter.Within.size();
	if( a_iWithin_Diff != 1 ) {	// 直ではないので
		return	( false );
	}
	if( c_rInner.IsClockwise == TRUE ) {	// 内側が右回りだったら対象外
		return	( false );
	}
	
	int	a_iFind_Mine		= 0;	// 内側のポリゴンの外部ポリゴンには自分が入っていることが前提
	int	a_iSame_Outer_Count	= 0;	// この値がc_rOuter.Within.size()と同じであればよい
	list<SIMPLERING>::iterator	a_iInner_Indx, a_iOuter_Indx;
	for( a_iInner_Indx = c_rInner.Within.begin(); a_iInner_Indx != c_rInner.Within.end(); a_iInner_Indx++ )
	{
		if( a_iInner_Indx->INDX == c_rOuter.NUM ) {	// 自分が入っている
			a_iFind_Mine	= 1;
		}
		for( a_iOuter_Indx = c_rOuter.Within.begin(); a_iOuter_Indx != c_rOuter.Within.end(); a_iOuter_Indx++ )
		{
			if( a_iInner_Indx->INDX == a_iOuter_Indx->INDX ) {
				a_iSame_Outer_Count++;
				break;
			}
		}
	}

	if( a_iFind_Mine == 1 && (unsigned)a_iSame_Outer_Count == c_rOuter.Within.size() ) {
		return	( true );
	}
	return ( false );
}

/**　2003.10.25
	ポリゴンの状態を見て再構成する（他パートが自分の１レベル内包ポリゴンの場合に行う）
	@return	TRUE	再構成処理があったばあい
	@return	FALSE	再構成処理が無かった場合
*/
bool	msi_get_parts_info::
Reconfig_Polygon( void )
{
	//この内部では、既に取得された情報
	//	list<RING>	e_lRings;		///< 2003.08.21
	//	WKSPoint*	e_wpPoints;		///< 2003.08.21
	// の状態をチェックし、上記の2メンバについて更新処理を行うものとする
	// 具体的には、IsClockWiseの情報を変更し、該当する部分の点列を反転させる

	bool	a_bRet	= FALSE;
	list<RING>::iterator	a_liIndx;
	int	i = 0;
	for( a_liIndx = e_lRings.begin(); a_liIndx != e_lRings.end(); a_liIndx++ )
	{
		RING	a_rCurRing	= *a_liIndx;
		// 自分が含むポリゴンがなく、自分を含むポリゴンが一つしかなく、しかも右回りの場合のみ（実質怪しいのはこの状態だけ？）
		if( a_rCurRing.IsClockwise == TRUE && 
			a_rCurRing.Contain.size() == 0 &&
			a_rCurRing.Within.size() == 1 )
		{
			a_liIndx->IsClockwise	= FALSE;	// 左回りにする
			a_liIndx->XMIN			= a_liIndx->START + ( a_liIndx->END - a_liIndx->XMIN );

			int	a_iSize	= a_rCurRing.END - a_rCurRing.START + 1;

			WKSPoint*	a_wWorkPts	= new WKSPoint[a_iSize];
			// テンポラリにコピーしておく
			
			int	j = 0;
			for( j = 0; j < a_iSize; j++ )
			{
				a_wWorkPts[j]	= e_wpPoints[a_rCurRing.START+j];	// 書き換え
			}
			for( j = 0; j < a_iSize; j++)
			{
				e_wpPoints[a_rCurRing.START+j]	= a_wWorkPts[(a_iSize - 1) - j];	// 書き換え
			}
			delete [] a_wWorkPts;
			a_bRet	= TRUE;	// 再構成処理あり
		}
	}
	return	( a_bRet );
}

/**
	パート数の獲得
	@return	パート数
*/
long	msi_get_parts_info::
Get_Part_Num	( void )
{
	return	( e_lPart_Num );
}

/**
	サブパート数の獲得
	※もし、指定パートが存在しない場合は、'0'を返す
	@return	サブパート数
*/
long	msi_get_parts_info::
Get_SubPart_Num	(	long	c_lPart_Num	)	// 指定パート番号(１オリジン)
{
	long	a_lSubPart_Num	= 0;

	if(c_lPart_Num > e_lPart_Num || c_lPart_Num < 1) {
		Error_Print ( "Get_SubPart_Num_01", "指定パートのサブパート数はとりだせません" );
		return		( 0 );
	}

	if( e_eGeoType != esriGeometryPolygon ) {	// ポリゴン以外のとき

		return	( 1 );				// ポリゴン以外は、パート==サブパートの考えかたなので、'1'とする

	}else {		// ポリゴンの場合
	
		RING	a_rPart;
		a_rPart	= e_vOuter[c_lPart_Num-1];
		
		a_lSubPart_Num	= Inner_Ring_Chk( a_rPart.NUM );	// 2003.10.02
		a_lSubPart_Num++;
	}
	return	( a_lSubPart_Num );	
}

/**
	サブパート数の最大数
	※あまり使用頻度が高くなさそうなので、こういう処理にしておく
	@return サブパート数の最大値
*/
long	msi_get_parts_info::
Get_SubPart_Max	( void )
{
	return	( e_lSubPart_Max );
}


/**
	点列を得る
	ポリゴンについては右回りで取得できればよいという前提　\
	内部でクローンを作成し、そのオブジェクトについてポイントを返す　\
	そのようにしないと、クラスで保持している図形について書き換えられるおそれがある
	@return IPointCollection点列
	※失敗した場合はNULL
*/
IPointCollectionPtr	msi_get_parts_info::
Get_Points	(	long		c_lPart_Num,		///< パート番号
				long		c_lSubPart_Num	)	///< サブパート番号
{
	HRESULT	hr;
	IGeometryPtr		a_ipSubPart_Geo;	// サブパートのジオメトリ

	if( c_lPart_Num > e_lPart_Num || c_lPart_Num < 1) {
		Error_Print ( "Get_Points_01", "指定パートの点列はとりだせません" );
		return		( NULL );
	}

	// ここでサブパートに該当するシェイプをとってくる
	if( e_eGeoType != esriGeometryPolygon ) {	// ポリゴン以外だった場合

		if(FAILED(e_ipGeoCol->get_Geometry(c_lPart_Num-1, &a_ipSubPart_Geo)) ) {
			Error_Print ( "Get_Points_02", "コレクションからジオメトリの取り出しに失敗" );
			return		( NULL );
		}

	}else {	// ポリゴンの場合

		IPointCollectionPtr		a_ipSp_Pts(CLSID_Ring);

		if(c_lSubPart_Num == 1) {	// サブパートが１の時は、外側のリングを返す	
		
			// 2003.8.21 
			RING	a_rOuter		= e_vOuter[c_lPart_Num-1];
			if( FAILED ( a_ipSp_Pts->AddWKSPoints ( (a_rOuter.END - a_rOuter.START)+1, ( e_wpPoints + a_rOuter.START )) )) {
				Error_Print ( "Get_Points_02", "AddWKSPointsに失敗" );
				return		( NULL );		
			}
			a_ipSubPart_Geo			= a_ipSp_Pts;
		}else {

			// 2003.8.21 
			RING	a_rOuter		= e_vOuter[c_lPart_Num-1];
			int		a_iAddCount		= 0;

			// 2003.10.03 指定サブパート番号にあたる中抜きポリゴンをとってくる
			SIMPLERING	a_rInner;
			int		a_iSkip_Count	= 1;
			list<SIMPLERING>::iterator	a_ipInner_Indx;
			for( a_ipInner_Indx = a_rOuter.Contain.begin(); a_ipInner_Indx != a_rOuter.Contain.end(); a_ipInner_Indx++ )
			{
				// 直接の中抜き以外は飛ばす
				if( !Is_Inner_Ring ( a_rOuter, e_vChk_Rings[a_ipInner_Indx->INDX] ) ) {
					continue;
				}else {
					a_iSkip_Count++;
					if( a_iSkip_Count == c_lSubPart_Num ) {
						a_rInner	= *a_ipInner_Indx;
						break;
					}
				}
			}
			// -----------------------------------------

			// 内側の個数を得る
			int			a_iRev_Size	= (a_rInner.END - a_rInner.START)+1;
			WKSPoint*	a_wRev_Pts	= new WKSPoint[a_iRev_Size];
			// 点列を反時計回りに
			int	i = 0, j = 0;
			for(i = 0, j = a_rInner.END; i < a_iRev_Size; i++, j-- )
			{
				a_wRev_Pts[i]	= e_wpPoints[j];
			}
			hr	= a_ipSp_Pts->AddWKSPoints ( a_iRev_Size, a_wRev_Pts );
			delete [] a_wRev_Pts;
			if( FAILED ( hr ) ) {
				Error_Print ( "Get_Points_02", "AddWKSPointsに失敗" );
				return		( NULL );		
			}
			a_ipSubPart_Geo			= a_ipSp_Pts;
		}
	}

	// クローンを作成して、それから取り出した点列を返す
	IClonePtr	a_ipClone;
	if( FAILED ( ((IClonePtr)a_ipSubPart_Geo)->Clone( &a_ipClone ) ) ){
		Error_Print ( "Get_Points_03", "ジオメトリのクローンに失敗" );
		return		( NULL );
	}
	IPointCollectionPtr		a_ipPts_Tmp;	

	a_ipPts_Tmp	= (IGeometryPtr)a_ipClone;	// 獲得したサブパート点列を格納
	return		( a_ipPts_Tmp );
}

/**
	シェイプの指定パート、サブパートから、ジオメトリを返す
	※返すジオメトリのタイプは、元ジオメトリによる。このメンバ関数も、やはり \
	　内部でクローンを作成してそれを返す形にしないと、外部で書き換えられる可能性　\
	　がある。
	@return	true　成功
	@return false 失敗
*/
IGeometryPtr	msi_get_parts_info::
Get_Geo_from_Part_Subpart	(	long	c_lPart_Num,		///< パート番号
								long	c_lSubPart_Num	)	///< サブパート番号
{
	HRESULT	hr;

	IGeometryPtr		a_ipSubPart_Geo;	// サブパートのジオメトリ

	if(c_lPart_Num > e_lPart_Num) {
		Error_Print ( "Get_Geo_from_Part_Subpart_01", "指定パートの点列はとりだせません" );
		return		( NULL );
	}

	// ここでサブパートに該当するシェイプをとってくる
	if( e_eGeoType != esriGeometryPolygon ) {	// ポリゴン以外だった場合

		if( e_eGeoType != esriGeometryPolyline ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "未対応のジオメトリタイプ" );
			return		( NULL );
		}

		IPolylinePtr		a_ipTmp_Line ( CLSID_Polyline );
		IPointCollectionPtr	a_ipLine_Pts_Tmp ( a_ipTmp_Line );
		hr	= a_ipLine_Pts_Tmp ->AddPointCollection ( Get_Points ( c_lPart_Num,	c_lSubPart_Num ) );	///< サブパート番号
		if( FAILED ( hr ) ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "点列の追加ができない" );
			return		( NULL );			
		}
		
		if( a_ipLine_Pts_Tmp == NULL ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "点列の取得ができない" );
			return		( NULL );
		}

		a_ipSubPart_Geo	= a_ipTmp_Line;

	}else
	if( e_eGeoType == esriGeometryPolygon ) {	// ポリゴンの場合

		IPointCollectionPtr	a_ipLine_Pts_Tmp;
		a_ipLine_Pts_Tmp	= Get_Points ( c_lPart_Num,	c_lSubPart_Num );	///< サブパート番号
		if( a_ipLine_Pts_Tmp == NULL ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "点列の取得ができない" );
			return		( NULL );
		}

		IPolygonPtr			a_ipTmp_Poly ( CLSID_Polygon );
		IPointCollectionPtr	a_tpTmpPts( a_ipTmp_Poly );

		// 点列を追加する
		if( FAILED (a_tpTmpPts->AddPointCollection ( a_ipLine_Pts_Tmp )) ) {
			Error_Print ( "Get_Geo_from_Part_Subpart_02", "点列の追加ができない" );
			return		( NULL );
		}
		a_ipSubPart_Geo	= a_ipTmp_Poly;

	}

	// クローンを作成して、それから取り出した点列を返す
	IClonePtr	a_ipClone;
	if( FAILED ( ((IClonePtr)a_ipSubPart_Geo)->Clone( &a_ipClone ) ) ){
		Error_Print ( "Get_Geo_from_Part_Subpart_03", "コレクションからジオメトリの取り出しに失敗" );
		return		( NULL );
	}

	IGeometryPtr			a_ipRet_Geo;
	a_ipRet_Geo				= (IGeometryPtr)a_ipClone;
	
	return	( a_ipRet_Geo );	// 獲得したサブパートを格納
}

/**
	指定パートのジオメトリを取り出す
	※返すジオメトリのタイプは、元ジオメトリによる。ここでは、Get_Geo_from_Part_Subpart \
	　Get_Part_Polygonでクローンを作成するので、そのまま返す
	@return 指定パートのジオメトリ
*/
IGeometryPtr		msi_get_parts_info::
Get_Geo_from_Part	(	long	c_lPart_Num	)	///< パート番号
{

	IGeometryPtr		a_ipPart_Geo;			// パートのジオメトリ

	if( e_eGeoType != esriGeometryPolygon ) {	// ポリゴン以外だった場合
		a_ipPart_Geo	= Get_Geo_from_Part_Subpart	( c_lPart_Num, 1 );
		if( a_ipPart_Geo == NULL ){
			return	( NULL );
		}

	}else {	// ポリゴンの場合

		IPolygonPtr		a_ipTmpPoly;
		a_ipTmpPoly		= Get_Part_Polygon ( c_lPart_Num );
		if( a_ipTmpPoly == NULL ) {
			return	( NULL );
		}
		a_ipPart_Geo	= a_ipTmpPoly;

	}
	return	( a_ipPart_Geo );
}


/**
	指定パートのポリゴンを取り出す（ポリゴンの場合）
	※与えられたジオメトリがポリゴンである場合のみ。もしライン等で使用した場合
	　には、NULLを返す
	　内部でクローンを作成してからポリゴンを返す。
	@return 指定パートのポリゴン
*/
IPolygonPtr		msi_get_parts_info::
Get_Part_Polygon	(	long	c_lPart_Num	)	///< パート番号
{

	HRESULT	hr;

	// データタイプが正しいかの判定
	if( e_eGeoType != esriGeometryPolygon ) {
		Error_Print ( "Get_Part_Polygon_01", "このメソッドはポリゴン以外には非対応です" );
		return	( NULL );
	}
	// パート番号が正しいかの判定
	if(c_lPart_Num > Get_Part_Num()) {
		Error_Print ( "Get_Part_Polygon_02", "指定パート番号が不正です" );
		return	( NULL );
	}


	IPolygonPtr				a_ipNewPolygon( CLSID_Polygon );
	IGeometryCollectionPtr	a_ipNewColl( a_ipNewPolygon );

	// 指定パートの外側リングを取り出す 2003.08.25
	RING					a_irOuter_Ring	= e_vOuter[c_lPart_Num-1];
	
	IRingPtr				a_ipOuter_Ring( CLSID_Ring );
	IPointCollectionPtr		a_ipPoints( a_ipOuter_Ring );

	// 外側点列を追加
	hr	= a_ipPoints->AddWKSPoints ( (a_irOuter_Ring.END - a_irOuter_Ring.START)+1, ( e_wpPoints + a_irOuter_Ring.START )) ;
	if( FAILED ( hr ) ) {
		Error_Print ( "Get_Part_Polygon_04", "指定パートのリングが追加できない" );
		return		( NULL );
	}

	hr	= a_ipNewColl->AddGeometry ( (IGeometryPtr)a_ipOuter_Ring );
	if( FAILED ( hr ) ) {
		Error_Print ( "Get_Part_Polygon_04", "指定パートのリングが追加できない" );
		return		( NULL );
	}

	// 内側点列を追加（点列を反時計回りにする）
	list<SIMPLERING>::iterator	a_ipInner_Indx;
	for( a_ipInner_Indx = a_irOuter_Ring.Contain.begin(); a_ipInner_Indx != a_irOuter_Ring.Contain.end(); a_ipInner_Indx++ )
	{
		// 直接の中抜き以外は飛ばす
		if( !Is_Inner_Ring ( a_irOuter_Ring, e_vChk_Rings[a_ipInner_Indx->INDX] ) ) {
			continue;
		}

		SIMPLERING	a_sCur_Ring	= *a_ipInner_Indx;
		int			a_iRev_Size	= (a_sCur_Ring.END - a_sCur_Ring.START)+1;

		IRingPtr	a_ipInner_Ring ( CLSID_Ring );

		IPointCollectionPtr	a_ipInnerPts ( a_ipInner_Ring );

		hr	= a_ipInnerPts->AddWKSPoints ( (a_sCur_Ring.END - a_sCur_Ring.START)+1, ( e_wpPoints + a_sCur_Ring.START )) ;
		if( FAILED ( hr ) ) {
			Error_Print ( "Get_Part_Polygon_04", "サブパートのリングが追加できない" );
			continue;
		}

		hr	= a_ipNewColl->AddGeometry ( (IGeometryPtr)a_ipInner_Ring );
		if( FAILED ( hr ) ) {
			Error_Print ( "Get_Part_Polygon_04", "サブパートのリングが追加できない" );
			continue;
		}

	}
#ifdef	_DEBUG	// ここが元凶？？？はずしたら道路の中抜きがおかしい問題も、街区の中抜きがおかしい問題も消えた・・・
//	ITopologicalOperator2Ptr	a_ipTopo;		// 論理オペレータ
//	a_ipTopo			= a_ipNewPolygon;
//	hr					= a_ipTopo->put_IsKnownSimple ( VARIANT_FALSE );
//	if( FAILED ( hr ) ) {
//		Error_Print ( "Get_Part_Polygon_07", "ポリゴンが単純化できませんでした" );
//		return		( NULL );
//	}
//	hr					= a_ipNewPolygon->SimplifyPreserveFromTo();	// 2003.06.13 修正
//	if( FAILED ( hr ) ) {
///		Error_Print ( "Get_Part_Polygon_07", "ポリゴンが単純化できませんでした" );
//		return		( NULL );
//	}
#endif	
	return	( a_ipNewPolygon );
}


/**	
	クラスに格納されているオブジェクトの情報を出力する
	※Initした後でないとだめ！
	@return bool
*/
bool	msi_get_parts_info::
Print_Obj_Info		( void )
{
	// パート情報出力
	fprintf ( e_fpLog, "パート数 = %d : 最大サブパート数 = %d\n", e_lPart_Num, e_lSubPart_Max );
	
	int	i = 0;
	for(i = 1; i <= e_lPart_Num; i++)
	{
		long			a_lSubNum	= 0;
		a_lSubNum		= Get_SubPart_Num ( i );
		fprintf			( e_fpLog, "パート番号 = <%d> : サブパート数 = <%d>\n", i, a_lSubNum);
		IGeometryPtr	a_ipGeo;
		int j = 0;
		for( j = 1; j <= a_lSubNum; j++)
		{
			a_ipGeo			= Get_Geo_from_Part_Subpart ( i, j );
			
			IPointCollectionPtr		a_ipCur_Pts(a_ipGeo);
			
			long	a_lPointCount	= 0;
			if( FAILED(a_ipCur_Pts->get_PointCount ( &a_lPointCount )) ) {
				return ( false );
			}	
			fprintf ( e_fpLog ,"総ポイント数 = %d\n", a_lPointCount);
			long	k = 0;
			for(k = 0; k < a_lPointCount; k++)
			{
				IPointPtr	a_ipPt;
				if ( FAILED( a_ipCur_Pts->get_Point ( k, &a_ipPt )) ) {
					return ( false );
				}
				double		x = 0.0, y = 0.0;
				a_ipPt->get_X ( &x );
				a_ipPt->get_Y ( &y );
				fprintf ( e_fpLog,	"パート = <%d> :　サブパート = <%d> count = <%d> (%0.10f, %0.10f)\n",
									i, j, k, x, y);
			}
		}
	}
	return ( true );
}

/**	
	クラスに格納されているオブジェクトの情報を出力する
	クラスに格納されているオブジェクトの情報を出力する(元点列）
	※Initした後でないとだめ！
	@return bool
*/
bool	msi_get_parts_info::
Print_Obj_Info2		( void )
{
	// パート情報出力
	fprintf ( e_fpLog, "リング数 = %d : 最大サブパート数 = %d\n", e_vChk_Rings.size(), e_lSubPart_Max );
	
	int	i = 0;
	for(i = 0; i < e_vChk_Rings.size(); i++)
	{

		long			a_lSubNum	= 0;
	//	a_lSubNum		= Get_SubPart_Num ( i+1 );
		fprintf			( e_fpLog, "リング番号 = <%d> : NUM=<%d> Contain = <%d> rot= <%s>\n",
									i, e_vChk_Rings[i].NUM, e_vChk_Rings[i].Contain.size(), e_vChk_Rings[i].IsClockwise?"右回り":"左回り");
			int	j = 0;
			list<SIMPLERING>::iterator	a_iIndx	= e_vChk_Rings[i].Contain.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Contain.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : インデックス= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"右回り":"左回り");
			}

		fprintf			( e_fpLog, "                   : Within  = <%d>\n", e_vChk_Rings[i].Within.size());
			a_iIndx	= e_vChk_Rings[i].Within.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Within.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : インデックス= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"右回り":"左回り");
			}
		fprintf			( e_fpLog, "                   : Cross   = <%d>\n", e_vChk_Rings[i].Cross.size());
			a_iIndx	= e_vChk_Rings[i].Cross.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Cross.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : インデックス= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"右回り":"左回り");
			}
		fprintf			( e_fpLog, "                   : Disjoint= <%d>\n", e_vChk_Rings[i].Disjoint.size());
			a_iIndx	= e_vChk_Rings[i].Disjoint.begin();
			for(j = 0; a_iIndx != e_vChk_Rings[i].Disjoint.end(); j++, a_iIndx++ )
			{
				SIMPLERING	tmp	= *a_iIndx;
				fprintf	( e_fpLog, "                   : インデックス= <%d> b= <%d> e=<%d> rot= <%s>\n", 
					tmp.INDX, tmp.START, tmp.END, e_vChk_Rings[tmp.INDX].IsClockwise?"右回り":"左回り");
			}
	}
	return ( true );
}

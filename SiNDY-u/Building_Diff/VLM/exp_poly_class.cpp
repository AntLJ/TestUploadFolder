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

#include "exp_poly_class.h"

/**
------------------------------------------------
	ジオメトリの点列を確保する
	@return	成功	WKSPoint点列へのポインタ
	@return	失敗	NULLポインタ
------------------------------------------------
*/
WKSPoint*	exp_poly_class::
Get_GeoPts( IGeometryPtr	c_ipGeo, 		///< ジオメトリ[IN]
		    long&			c_lCount )		///< 補間点数[OUT]
{
	HRESULT					hr;
	IPointCollectionPtr		a_ipPtsArray( c_ipGeo );

	long					a_lCount	= 0;
	a_ipPtsArray->get_PointCount( &a_lCount );
	if( a_lCount == 0 ) {
		c_lCount	= 0;
		return	( NULL );
	}

	WKSPoint*	a_wpPoints;
	a_wpPoints	= new WKSPoint[a_lCount];	// 点列の確保
	hr	= a_ipPtsArray->QueryWKSPoints ( 0, a_lCount, a_wpPoints );
	if ( FAILED( hr ) ){
		c_lCount	= 0;
		return		( NULL );
	}
	c_lCount	= a_lCount;
	return	( a_wpPoints );
}

/**
------------------------------------------------
	ジオメトリをばらけた点列に落とし込む	
	@return	0	成功
	@return -1	失敗
------------------------------------------------
*/
int		exp_poly_class::
Set_GeotoPts( IGeometryPtr	c_ipGeo, 	///< ジオメトリ[IN]
			  int			c_iID )		///< ID[IN]
{

	long		a_lCount	= 0;
	WKSPoint*	a_wpPoints	= NULL;
	a_wpPoints	= Get_GeoPts ( c_ipGeo, a_lCount );
	if( a_wpPoints == NULL ) {
		return	( -1 );
	}

	//　点列のコピー
	int i = 0;
	for(i = 0; i < a_lCount; i++)
	{
		Dpos2		a_dPt;

		a_dPt.xpos	= a_wpPoints[i].X;
		a_dPt.ypos	= a_wpPoints[i].Y;
		a_dPt.e_indx= c_iID;
	
		// アーク列を追加
		e_sExpPts.insert ( a_dPt );
	}
	delete [] a_wpPoints;
	return	( 0 );
}

/**
------------------------------------------------
	ジオメトリをばらけたアーク列（ベクタ）に落とし込む	
	@return	0	成功
	@return -1	失敗
------------------------------------------------
*/
int		exp_poly_class::
Set_GeotoArcs( IGeometryPtr	c_ipGeo, 	///< ジオメトリ[IN]
			   int			c_iID )		///< ID[IN]
{
	long		a_lCount	= 0;
	WKSPoint*	a_wpPoints	= NULL;
	a_wpPoints	= Get_GeoPts ( c_ipGeo, a_lCount );
	if( a_wpPoints == NULL ) {
		return	( -1 );
	}

	//　点列のコピー
	int i = 0;
	for(i = 0; i < a_lCount-1; i++)
	{
		Dpos		a_dBegin;
		Dpos		a_dEnd;

		a_dBegin.xpos	= a_wpPoints[i].X;
		a_dBegin.ypos	= a_wpPoints[i].Y;
		a_dEnd.xpos		= a_wpPoints[i+1].X;
		a_dEnd.ypos		= a_wpPoints[i+1].Y;

		line_seg2		cur_arc;
		cur_arc.e_Head	= a_dBegin;
		cur_arc.e_Tail	= a_dEnd;
		cur_arc.e_iIndx	= c_iID;
		// アーク列を追加
		e_vExpArcs.push_back(cur_arc);
	//	e_msExpArcs.insert(cur_arc);
	}
	delete [] a_wpPoints;
	return	( 0 );
}

/**
------------------------------------------------
	アークに最も近い点を取り出す
	@return	0	取り出し成功
	@return	1	該当データ無し
------------------------------------------------
*/
int		exp_poly_class::
Get_NearestPt( line_seg2		c_lSeg,			///< アークの線分[IN]
			   double			c_lDeg, 		///< 閾値（度）[IN}
			   vector<Dpos2>&	c_msErrPts )	///< エラーとなる点のコンテナ[OUT]
{
	Dpos2	a_dMinPt, a_dMaxPt;
	// 最大値、最小値の取り出し
	if( c_lSeg.e_Head.xpos < c_lSeg.e_Tail.xpos ) {
		a_dMinPt.xpos	= c_lSeg.e_Head.xpos;
		a_dMaxPt.xpos	= c_lSeg.e_Tail.xpos;
	}else {
		a_dMinPt.xpos	= c_lSeg.e_Tail.xpos;
		a_dMaxPt.xpos	= c_lSeg.e_Head.xpos;
	}
	if( c_lSeg.e_Head.ypos < c_lSeg.e_Tail.ypos ) {
		a_dMinPt.ypos	= c_lSeg.e_Head.ypos;
		a_dMaxPt.ypos	= c_lSeg.e_Tail.ypos;
	}else {
		a_dMinPt.ypos	= c_lSeg.e_Tail.ypos;
		a_dMaxPt.ypos	= c_lSeg.e_Head.ypos;
	}
	a_dMinPt.e_indx	= 0;
	a_dMaxPt.e_indx	= 1000000000;

	// 閾値を最小、最大値に加味する
	a_dMinPt.xpos -= c_lDeg;
	a_dMinPt.ypos -= c_lDeg;
	a_dMaxPt.xpos += c_lDeg;
	a_dMaxPt.ypos += c_lDeg;

	multiset<Dpos2>::iterator	a_iBegin_Indx, a_iEnd_Indx, a_iIndx, a_iChk;

	a_iBegin_Indx	= e_sExpPts.lower_bound ( a_dMinPt );
	a_iEnd_Indx		= e_sExpPts.upper_bound ( a_dMaxPt );
	// エラーポイントのクリア
	c_msErrPts.clear();
int cnt=0;
	for( a_iIndx = a_iBegin_Indx; ; a_iIndx++,cnt++ )
	{
		Dpos2	a_dCurPt	= *a_iIndx;
		// 点とアークの距離計算の結果、閾値以下ならエラーポイントとして追加する
		double	a_dDist		= 0;
		Dpos tmp; tmp.xpos = a_dCurPt.xpos; tmp.ypos = a_dCurPt.ypos;
		a_dDist	= c_lSeg.get_pt_seg_dist ( a_dCurPt );
	//	a_dDist	= c_lSeg.get_pt_line_dist ( tmp );
	//	if( a_dDist != 0.0 && a_dDist <= c_lDeg ) {
		if( a_dDist <= c_lDeg ) {	
			c_msErrPts.push_back ( a_dCurPt );
		}
		if( a_iIndx == a_iEnd_Indx ) break;
	}
printf("match = %d\n", cnt );
	if( c_msErrPts.size() > 0 ) {
		return ( 0 );
	}
	return ( -1 );
}

/**
	点に最も近いアークを取り出す
	@return	0	取り出し成功
	@return	1	該当データ無し
*/
int		exp_poly_class::
Get_NearestArcs(	Dpos2				c_lPt,			///< 点[IN]
					double				c_lDeg,			///< 閾値（度）[IN}
					vector<line_seg2>&	c_msErrPts	)	///< エラーとなるアークのセット[OUT]
{
	line_seg2	a_dBegin, a_dEnd;
	// 最大値、最小値の取り出し
	Dpos2	a_dBPt, a_dEPt;
	a_dBPt	= c_lPt;
	a_dEPt	= c_lPt;
	a_dBPt.xpos	-= c_lDeg;
	a_dBPt.ypos	-= c_lDeg;
	a_dEPt.xpos	+= c_lDeg;
	a_dEPt.ypos	+= c_lDeg;

	a_dBegin.Data_Set	( a_dBPt, a_dBPt );
	a_dEnd.Data_Set		( a_dEPt, a_dEPt );
	a_dBegin.e_iIndx	= 1000000000;
	a_dEnd.e_iIndx		= 0;

	// 閾値を最小、最大値に加味する

/*	multiset<line_seg2>::iterator	a_iBegin_Indx, a_iEnd_Indx, a_iIndx;
//	vector<line_seg2>::iterator	a_iIndx;
	a_iBegin_Indx	= e_msExpArcs.lower_bound ( a_dBegin );
	a_iEnd_Indx		= e_msExpArcs.upper_bound ( a_dEnd );

	// エラーポイントのクリア
	c_msErrPts.clear();

//	for( a_iIndx = a_iBegin_Indx; a_iIndx != a_iEnd_Indx; a_iIndx++ )
	for( a_iIndx = a_iBegin_Indx; ; a_iIndx++ )
//	for( a_iIndx = e_vExpArcs.begin();a_iIndx != e_vExpArcs.end() ; a_iIndx++ )
	{
		line_seg2	a_dCurSeg	= *a_iIndx;
		// 点とアークの距離計算の結果、閾値以下ならエラーポイントとして追加する
		double	a_dDist		= 0;
		a_dDist	= a_dCurSeg.get_pt_seg_dist ( c_lPt );
		if( a_dDist != 0.0 && a_dDist <= c_lDeg ) {
			if( a_dDist == 0.0 ) {
			printf( "%0.10f\n", a_dDist );
			}
			c_msErrPts.push_back ( a_dCurSeg );
			break;
		}
		if( a_iIndx == a_iEnd_Indx ) {
			break;
		}
	}
	if( c_msErrPts.size() > 0 ) {
		return ( 0 );
	}*/
	return ( -1 );
}

double		line_seg2::
get_pt_seg_dist	(	Dpos2		c_src_pt	)	// 距離を求める点
{
	double	dist	= 0.0;

	dvec	v1, v2;			// ベクタその１、その２
	dvec	vs1, vs2;		// 線分構成点で構成されるベクタ
	double	nai_1	= 0;	// 内積その１
	double	nai_2	= 0;	// 内積その２

	Dpos	a_dPos;
	a_dPos.xpos	= c_src_pt.xpos;
	a_dPos.ypos	= c_src_pt.ypos;

	v1.get_vec	(e_Head, a_dPos);	// 座標点１と着目点のベクトル
	v2.get_vec	(e_Tail, a_dPos);	// 座標点２と着目点のベクトル
	vs1.get_vec	(e_Head, e_Tail);		// 座標点１と座標点２のベクトル
	vs2.get_vec	(e_Tail, e_Head);		// 座標点２と座標点１のベクトル

	nai_1	= v1.get_naiseki	( vs1 );	// 内積計算
	nai_2	= v2.get_naiseki	( vs2 );	//  〃

	if(nai_1 > 0.0 && nai_2 > 0.0) {		// なす角が両方とも鋭角(線分の間に直交点がくる)
		line_seg	cur_seg(e_Head, e_Tail);
		dist		= cur_seg.get_pt_line_dist(a_dPos);
	}else {	// どっちかが鈍角になる(線分の間に直交点がこない)
		double	td1	= 0.0;
		double	td2	= 0.0;
		td1		= a_dPos.Get_Dist( e_Head );
		td2		= a_dPos.Get_Dist( e_Tail );
		if(td1 < td2) {
			dist	= td1;
		}else {
			dist	= td2;
		}
	}
	return(dist);
}

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
#include "poly_class.h"

//--------------------------------------------------------------------
//	Vector GetVector(Point pt0, Point pt1)
//		output:	与えられた始点と終点の座標から作成されたベクトル
//
//		function:
//				与えられた２点を結ぶベクトルを作成する
//--------------------------------------------------------------------
void	dvec::
get_vec		(	Dpos		pt0,	//	ベクトルの始点の座標
				Dpos		pt1)	//	ベクトルの終点の座標
{
	double	x_diff	= (double)pt1.xpos - (double)pt0.xpos;
	double	y_diff	= (double)pt1.ypos - (double)pt0.ypos;

	// 零ベクトルへの対応
	if( x_diff == 0.0 ) {
		x	= x_diff;
	}
	if( y_diff == 0.0 ) {
		y	= y_diff;
	}
	// 単位ベクトルにする
	x = x_diff / sqrt(x_diff*x_diff + y_diff*y_diff);
	y = y_diff / sqrt(x_diff*x_diff + y_diff*y_diff); 
}

//--------------------------------------------------------------------
// 2001.10.11
//	ベクトルの内積の値を求める(符号だけ)
// なす角が90度 = 0
// なす角が鋭角 = 0以上
// なす角が鈍角 = 負値
//--------------------------------------------------------------------
double		dvec::
get_naiseki	(	dvec		c_Vec	)	//	ベクトルその２
{
	//int		ret	= v0.x * c_Vec.x + v0.y * c_Vec.y;	// 内積計算
	double		ret	= (double)x * (double)c_Vec.x + (double)y * (double)c_Vec.y;	// 内積計算
	return	(ret);
    /*	2002.03.19コメントアウト
	if(ret != 0.0)
        return ((ret) < 0.0)?-1:1;
    else	// ret == 0
        return 0;
	*/
}

//--------------------------------------------------------------------
//		output:	２つの２次元ベクトルの外積の値
//
//		function:
//				ベクトルの外積の値を求める(符号だけ)
//--------------------------------------------------------------------
int		dvec::
get_gaiseki	(	dvec	c_Vec)	//	vector
{
    double dRet = (double)x*(double)c_Vec.y - (double)y*(double)c_Vec.x;
    if(dRet != 0.0)
        return ((dRet) < 0.0)?-1:1;
    else
        return 0;
}

/*
	２つのベクトルのなす角を求める
	@return	角度
*/
double		dvec::
get_deg(	dvec	c_Vec	)
{
	double	n_atai	= 0.0;	// 内積の値
	int		g_atai	= 0;	// 外積の値
	double	deg		= 0.0;	// 角度
	double	v0		= 0.0;
	double	v1		= 0.0;
	v0		= fabs(sqrt(x*x+y*y));
	v1		= fabs(sqrt(c_Vec.x*c_Vec.x+c_Vec.y*c_Vec.y));
	n_atai	= get_naiseki	(c_Vec);
	g_atai	= get_gaiseki	(c_Vec);

	double	yogen	= 0.0;
	double	bunbo	= v0*v1;
	if(bunbo == 0.0 && n_atai == 0.0) {
		yogen	= 0.0;
	}else {
		yogen	= n_atai / bunbo;
	}

	double	ret_yogen	= 0.0;
	if( fabs(yogen - 1.0) < 1.0e-10 ) { // ダブルでの計算だと誤差がでるのでこうする
		ret_yogen	= 0.0;
	}else {
		ret_yogen	= (double)acos(yogen) * 180.0;
	}

	deg		= ret_yogen / 3.14159265358979;
	
	if(g_atai > 0) {
		deg *= -1.0;
	}
	return	(deg);
}

//--------------------------------------------------------------------
//		output:	0:		線分が交差しない
//				1:		線分が交差する
//		function:
//				線分の交差判定を行なう
//		algorithm:
//				線分１、２各々について互いに他の線分のどちら側にあるかを調べ
//				両方とも異なる側にあれば交差していることになる
//--------------------------------------------------------------------
int		line_seg::
Cross_Seg	(	line_seg	c_Seg	)	//	対象線分
{
	dvec	 v0, v1, v2;

	v0.get_vec(e_Head, e_Tail);
	v1.get_vec(e_Head, c_Seg.e_Head);
	v2.get_vec(e_Head, c_Seg.e_Tail);
	if(v0.get_gaiseki(v1)*v0.get_gaiseki(v2) > 0) return(0);

	v0.get_vec(c_Seg.e_Head, c_Seg.e_Tail);
	v1.get_vec(c_Seg.e_Head, e_Head);
	v2.get_vec(c_Seg.e_Head, e_Tail);
	if(v0.get_gaiseki(v1)*v0.get_gaiseki(v2) > 0) return(0);

	if(((e_Head.xpos>e_Tail.xpos)?e_Head.xpos:e_Tail.xpos) <
		((c_Seg.e_Head.xpos<c_Seg.e_Tail.xpos)?c_Seg.e_Head.xpos:c_Seg.e_Tail.xpos)) return(0);
	if(((e_Head.ypos>e_Tail.ypos)?e_Head.ypos:e_Tail.ypos) <
		((c_Seg.e_Head.ypos<c_Seg.e_Tail.ypos)?c_Seg.e_Head.ypos:c_Seg.e_Tail.ypos)) return(0);
	if(((e_Head.xpos<e_Tail.xpos)?e_Head.xpos:e_Tail.xpos) >
		((c_Seg.e_Head.xpos>c_Seg.e_Tail.xpos)?c_Seg.e_Head.xpos:c_Seg.e_Tail.xpos)) return(0);
	if(((e_Head.ypos<e_Tail.ypos)?e_Head.ypos:e_Tail.ypos) >
		((c_Seg.e_Head.ypos>c_Seg.e_Tail.ypos)?c_Seg.e_Head.ypos:c_Seg.e_Tail.ypos)) return(0);

	return(1);
}

//--------------------------------------------------------------------
// 2001.10.03 PointsDiet で使ってたものを改造
// ２点 e_Head,e_Tailを通る直線と点c_src_ptの距離を求める
// 返り値	距離(正規化座標値)
//--------------------------------------------------------------------
double		line_seg::
get_pt_line_dist	(	Dpos		c_Pt	)	// 距離を求める点
{
	double	dist	= 0.0;
	double	a		= 0.0;
	double	b		= 0.0;
	double	c		= 0.0;

	if(e_Head.xpos == e_Tail.xpos) {
		dist	= fabs(e_Head.xpos - c_Pt.xpos);
	} else {
		a		= ((double)e_Tail.ypos - (double)e_Head.ypos) / ((double)e_Tail.xpos - (double)e_Head.xpos);
		b		= -1.0;
		c		= 0.5 * (((double)e_Head.ypos + (double)e_Tail.ypos) - 
					((double)e_Tail.ypos - (double)e_Head.ypos) * 
					((double)e_Head.xpos + (double)e_Tail.xpos) / ((double)e_Tail.xpos - (double)e_Head.xpos));
		dist	= fabs(a * (double)c_Pt.xpos + b * (double)c_Pt.ypos + c) / sqrt(a * a + b * b);
	}
	return(dist);
}

//--------------------------------------------------------------------
// 2001.10.11
// ２点 e_Head,e_Tailで構成される線分と点c_src_ptの距離を求める
// 返り値	距離(正規化座標値)
//--------------------------------------------------------------------
double		line_seg::
get_pt_seg_dist	(	Dpos		c_src_pt	)	// 距離を求める点
{
	double	dist	= 0.0;

	dvec	v1, v2;			// ベクタその１、その２
	dvec	vs1, vs2;		// 線分構成点で構成されるベクタ
	double	nai_1	= 0;	// 内積その１
	double	nai_2	= 0;	// 内積その２

	v1.get_vec	(e_Head, c_src_pt);	// 座標点１と着目点のベクトル
	v2.get_vec	(e_Tail, c_src_pt);	// 座標点２と着目点のベクトル
	vs1.get_vec	(e_Head, e_Tail);		// 座標点１と座標点２のベクトル
	vs2.get_vec	(e_Tail, e_Head);		// 座標点２と座標点１のベクトル

	nai_1	= v1.get_naiseki	( vs1 );	// 内積計算
	nai_2	= v2.get_naiseki	( vs2 );	//  〃

	// 2008.09.12 判定ミスしていたので修正
	if(nai_1 >= 0 && nai_2 >= 0) {		// なす角が両方とも鋭角(線分の間に直交点がくる)
		line_seg	cur_seg(e_Head, e_Tail);
		dist		= cur_seg.get_pt_line_dist(c_src_pt);
	}else {	// どっちかが鈍角になる(線分の間に直交点がこない)
		double	td1	= 0.0;
		double	td2	= 0.0;
		td1		= c_src_pt.Get_Dist( e_Head );
		td2		= c_src_pt.Get_Dist( e_Tail );
		if(td1 < td2) {
			dist	= td1;
		}else {
			dist	= td2;
		}
	}
	return(dist);
}

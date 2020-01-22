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

// この内部のソースは、借り物がほとんどです・・
#include "stdafx.h"
#include "poly_class.h"

//--------------------------------------------------------------------
//		output:	0:  点は多角形の外側にある
//				1:  点は多角形の内側にある
//				-1: 点は多角形の境界上にある
//		function:
//				与えられた点が多角形に対してどこにあるかを調べる
//--------------------------------------------------------------------
int		poly_cls::
innerpolygon	(	Dpos	c_Pt	)	//	調べる点の座標
{
	int			i, ncross = 0;
//	int			maxy, miny;
	double		maxy, miny;
//	int			xint;
	double		xint;
	int			count	= e_Pts.size();

	for(i=0 ; i<count-1 ; ++i) {
		if(e_Pts[i].ypos == c_Pt.ypos) {
			if(e_Pts[i].xpos == c_Pt.xpos)
				return(-1);
			if(e_Pts[i].ypos == e_Pts[i+1].ypos) {
				if(e_Pts[i].xpos < e_Pts[i+1].xpos) {
					if((c_Pt.xpos>=e_Pts[i].xpos) && (c_Pt.xpos<=e_Pts[i+1].xpos)) return(-1);
				} else {
					if((c_Pt.xpos<=e_Pts[i].xpos) && (c_Pt.xpos>=e_Pts[i+1].xpos)) return(-1);
				}
			}
		}
	}

	for(i=0 ; i<count-1 ; ++i) {
		if(e_Pts[i].ypos < e_Pts[i+1].ypos) {
			maxy = e_Pts[i+1].ypos;
			miny = e_Pts[i].ypos;
		} else {
			maxy = e_Pts[i].ypos;
			miny = e_Pts[i+1].ypos;
		}
		if((e_Pts[i].ypos != e_Pts[i+1].ypos) &&
		  ((c_Pt.xpos<=e_Pts[i].xpos) || (c_Pt.xpos<=e_Pts[i+1].xpos)) &&
		  ((c_Pt.ypos<=maxy) && (c_Pt.ypos>=miny)) &&
		  (c_Pt.ypos != maxy)) {
			xint = e_Pts[i].xpos + (long) (c_Pt.ypos-e_Pts[i].ypos) * 
					(long) (e_Pts[i+1].xpos-e_Pts[i].xpos) / 
					(long) (e_Pts[i+1].ypos-e_Pts[i].ypos);
			if(c_Pt.xpos == xint) return(1);
			if(c_Pt.xpos < xint) ++ncross;
		}
	}

	return(ncross % 2);
}

//--------------------------------------------------------------------
// ポリゴン内包チェック
//		output:	0:  ポリゴンは内包されない
//				1:  ポリゴンは内包される
//		function:
//				ポリゴン１の内部にポリゴン２が内包されるかどうか
//--------------------------------------------------------------------
int		poly_cls::
innerpolycheck	(	poly_cls	c_Poly	)	// 対象点列
{
//	int	inner_cnt	= 0;
	int	count2		= c_Poly.e_Pts.size();

	// とりあえず全ての点が内包されるかのチェック
	int	i;
	for(i = 0; i < count2; i++)
	{
		if(innerpolygon(c_Poly.e_Pts[i]) == 0)
			return(0);
	}
	// 点列が交差してたらはみでている
	if(crossline( c_Poly ) == 0) {
		return(1);	// ポリゴンは内包される
	}else {
		return(0);
	}
}

//--------------------------------------------------------------------
//	int CrossLine(PointSet *line1, PointSet *line2)
//		output:	２つの折れ線の交点の個数
//		function:
//				２つの折れ線の交点の個数を求める
//		algorithm:
//				２つの折れ線を構成する線分すべての組み合わせにたいして CrossSegment
// 			により交差判定を行ない交差している場合の数を求める
//--------------------------------------------------------------------
int		poly_cls::
crossline(	poly_cls	c_Poly	)
{
	int			i, j;
	int			count;
	int			count1	= e_Pts.size();
	int			count2	= c_Poly.e_Pts.size();
	count = 0;
	for(i = 0 ; i < count1-1 ; i++)
	{
		for(j = 0 ; j < count2-1; j++)
		{
			Dpos		s1	= e_Pts[i];
			Dpos		e1	= e_Pts[i+1];
			Dpos		s2	= c_Poly.e_Pts[j];
			Dpos		e2	= c_Poly.e_Pts[j+1];
			line_seg	ls_1	(s1, e1);
			line_seg	ls_2	(s2, e2);

			count += ls_1.Cross_Seg(ls_2);
		}
	}

	return(count);
}

//-2000.5.30----------------------------------------------------------
// ポリゴンの面積
//--------------------------------------------------------------------
/*
double get_real_area	(	Dpos	*e_Pts,		// 点列
							int		c_count,	// 補間点数
							int		c_meshcode)	// メッシュコード
{
	double		lon		= 0.0;
	double		lat		= 0.0;
	double		xratio	= 0.0;
	double		yratio	= 0.0;

	crd_cnv		crd;

	crd.MeshtoLL(c_meshcode, 0, 0, &lon, &lat);
	xratio	= RA * sin(M_PI / 2.0 - lat * M_PI / 180.0) * 2.0 * M_PI / 23040.0 / 1000000.0; // 23040 = 360*64
	yratio	= RA * 2.0 * M_PI / 34560.0 / 1000000.0;
	return(calc_area(e_Pts, c_count, xratio, yratio));
}
*/

//-2000.5.30----------------------------------------------------------
// ポリゴンの面積
//--------------------------------------------------------------------
double		poly_cls::
calc_area	(	double	c_xratio,	// x方向比率?
				double	c_yratio)	// y方向比率?
{
	double	S		= 0.0;
	int		i;
	int		s		= 0;
	int		e		= 0;
	int		count	= e_Pts.size();
	for(i = 0; i < count - 1; i++)
	{
		s	= (i == 0) ? count-2: i-1;
		e	= (i == count-2) ? 0: i+1;
		S	+=((e_Pts[s].xpos - e_Pts[e].xpos) * c_xratio * e_Pts[i].ypos * c_yratio);
	}
	S /= (2.0);
	return(fabs(S));
}

//--------------------------------------------------------------------
// ポリゴンの代表点を探す
//--------------------------------------------------------------------
int		poly_cls::
Find_Center	(	Dpos	*RprPt)	// 代表点
{
    Crect	rect;
    int		iRet;
//  int		DivX, DivY;
	double	DivX, DivY;
//  int		BaseX, BaseY;
	double	BaseX, BaseY;
	int		i, j;
    int		DivCount	= 1;
//	int		count		= e_Pts.size();
    
    if((iRet = getouterrect( &rect ))){
        BaseX	= rect.xmin;
        BaseY	= rect.ymin;
        DivX	= rect.xmax - rect.xmin;
        DivY	= rect.ymax - rect.ymin;
        iRet = 0;

//      while(iRet != 1 && DivCount < 1024 && DivX != 0 && DivY != 0){
		while(iRet != 1 && DivCount < 1024 && DivX != 0.0 && DivY != 0.0){

            for(i = 0; iRet != 1 && i < DivCount; i++){
                rect.ymin	= BaseY + DivY * i;		// 1999.6.24 X->Y
                rect.ymax	= rect.ymin + DivY;
                for(j = 0; iRet != 1 && j < DivCount; j++){
                    rect.xmin	= BaseX + DivX * j;
                    rect.xmax	= rect.xmin + DivX;
                    iRet = iscenterin( &rect, RprPt );
                }
            }
            DivX /= 2;
            DivY /= 2;
            DivCount *= 2;	// 1999.6.24 4->2
        }
		if(iRet == 0){
			RprPt->xpos	= (double)e_Pts[0].xpos;
			RprPt->ypos	= (double)e_Pts[0].ypos;
		}
    }
    return iRet;
}

//--------------------------------------------------------------------
// 矩形の中心がポリゴン内かどうかのチェック
//--------------------------------------------------------------------
int		poly_cls::
iscenterin	(	Crect	*rect,	// チェック対象矩形
				Dpos	*RprPt)	// 代表点
{
    int iRet;
	Dpos tmp;
//	int	count	= e_Pts.size();

    RprPt->xpos	= (double)(rect->xmin + rect->xmax) / 2.0;
    RprPt->ypos	= (double)(rect->ymin + rect->ymax) / 2.0;
	tmp.xpos	= (int)RprPt->xpos;
	tmp.ypos	= (int)RprPt->ypos;
    if(innerpolygon( tmp ) == 1)
        iRet = 1;
    else
        iRet = 0;
    
    return iRet;
}

//--------------------------------------------------------------------
//	ポリゴン点列から、外接矩形の獲得
//--------------------------------------------------------------------
int		poly_cls::
getouterrect(	Crect	*rect)	//	獲得矩形
{
    int i;
    int iRet	= 0;
	int	count	= e_Pts.size(); 
    if(count > 2){
		rect->xmin	= e_Pts[0].xpos;
		rect->ymin	= e_Pts[0].ypos;
		rect->xmax	= e_Pts[0].xpos;
		rect->ymax	= e_Pts[0].ypos;
        for(i = 1; i < count; i++){
            if(e_Pts[i].xpos < rect->xmin)
                rect->xmin	= e_Pts[i].xpos;
            else if(e_Pts[i].xpos > rect->xmax)
                rect->xmax	= e_Pts[i].xpos;
            if(e_Pts[i].ypos < rect->ymin)
                rect->ymin	= e_Pts[i].ypos;
            else if(e_Pts[i].ypos > rect->ymax)
                rect->ymax	= e_Pts[i].ypos;
        }
        iRet = 1;
    }
    return iRet;
}

//--------------------------------------------------------------------
// 2001.10.03 PointsDiet で使ってたものを改造
// 点c_Ptと、Dpos *e_Ptsで表されるラインの距離を求める
// 返り値	距離(正規化座標値)
//--------------------------------------------------------------------
double		poly_cls::
get_pt_arc_dist	(	Dpos		c_Pt	)	// 距離を求める点
{
	double	dist		= 0.0;
	double	cur_dist	= 0.0;
	int		i			= 0;
	int		count		= e_Pts.size();

	if(count == 1) {
		dist	= sqrt(pow(2.0, (e_Pts[0].xpos - c_Pt.xpos)) +
					pow(2.0, (e_Pts[0].ypos - c_Pt.ypos)));
		return	(dist);
	}else
	if(count >= 2) {
		for(i = 0; i < count-1; i++)
		{
			line_seg	cur_seg( e_Pts[i], e_Pts[i+1] );
			cur_dist	= cur_seg.get_pt_seg_dist(c_Pt);
			if(i == 0) {
				dist	= cur_dist;
			}else {
				if(cur_dist < dist) {
					dist	= cur_dist;
				}
			}
		}
	}else {		// エラーの場合
		return	(-1.0);
	}
	return	(dist);
}

/*
	点列と点列の距離を求める
	@return	点列同士の距離
*/
double		poly_cls::
Get_Poly_Dist(	poly_cls	c_Poly	)	// 対象点列
{
	int	count			= e_Pts.size();
	double	a_dist		= 0.0;
	double	a_nearest	= 0.0;	// 最も近い距離
	int		i			= 0;

	for( i = 0; i < count; i++)
	{
		a_dist	= 0.0;	// 念のため毎回初期化
		a_dist	= get_pt_arc_dist(c_Poly.e_Pts[i]);
		if(i == 0) {	
			a_nearest	= a_dist;
			continue;
		}
		if(a_dist < a_nearest) {
			a_nearest	= a_dist;
		}
	}
	return	( a_nearest );
}

/*
	点列の回転（開始位置を変える）
*/
int		poly_cls::	
Rot_Pts(	int	c_Begin	)	// 開始位置番号[IN] 0origin
{
	std::vector<Dpos>	aTmpPts;	// テンポラリ

	int	count	= e_Pts.size();
	int	i		= 0;
	if(c_Begin < 0 || c_Begin >= count) {	// 不正な値は何もせず、-1を返す
		return	(-1);
	}
	// 開始位置から最後までを放り込む
	for(i = c_Begin; i < count; i++)
	{
		Dpos	cur	= e_Pts[i];
		aTmpPts.push_back( cur );
	}
	// 最初から開始位置前まで放り込む
	for(i = 0; i < c_Begin; i++)
	{
		Dpos	cur = e_Pts[i];
		aTmpPts.push_back( cur);
	}
	// 元のデータを上書き
	for(i = 0; i < count; i++)
	{
		e_Pts[i]	= aTmpPts[i];
	}
	return	(0);
}

/*
	点列の反転
	@return	0	成功
	@return -1	失敗
*/
int		poly_cls::
Reverse_Pts()
{
	std::vector<Dpos>	aTmpPts;	// テンポラリ

	int	count	= e_Pts.size();
	int	i		= 0;
	if(count == 0) {				// 点が無いなら何もせず、-1を返す
		return	(-1);
	}
	// 最後から順に取り出す
	for(i = count-1; i >= 0; i--)
	{
		Dpos	cur	= e_Pts[i];
		aTmpPts.push_back( cur );
	}
	// 元のデータを上書き
	for(i = 0; i < count; i++)
	{
		e_Pts[i]	= aTmpPts[i];
	}
	return	(0);
}

/*
	点列の分割
*/
int		poly_cls::
Div_Pts(	int			c_Begin,	// 分割始点番号
			int			c_Num,		// 始点からの補間点数
			poly_cls*	c_Poly )	// 分割点列格納
{
	if(c_Begin < 0) {
		return	(-1);
	}
	int	count	= e_Pts.size();		// 補間点数
	int	max		= c_Begin + c_Num;
	if( max > count) {
		return	(-1);
	}
	int	i = 0;
	for(i = c_Begin; i < max; i++)
	{
		Dpos	cur	= e_Pts[i];
		c_Poly->e_Pts.push_back( cur );
	}
	return	(0);
}

/*
	点列から、アーク列へ内部的に変換
*/
int		poly_cls::
Make_Arcs(	void	)
{
	int	count	= e_Pts.size();

	if(count < 2) {
		return	(-1);
	}

	e_Arcs.clear();		// 初期化する

	int	i		= 0;
	for(i = 0; i < count-1; i++)
	{
		line_seg	cur(e_Pts[i], e_Pts[i+1]);
		e_Arcs.push_back(cur);
	}
	return	(0);
}

/*
	アーク列から、点列へ内部的に変換
	line_seg の　e_Headとe_Tailが一致していることが
	前提となる
*/
void		poly_cls::
Make_Pts(	void	)
{
	e_Pts.clear();		// 初期化しておく

	std::vector<line_seg>::iterator		indx;
	for(indx = e_Arcs.begin(); indx != e_Arcs.end(); indx++)
	{
		line_seg	cur	= *indx;
		if(indx == e_Arcs.begin()) {	// 最初だけ始点と終点を入れる
			e_Pts.push_back( cur.e_Head );
			e_Pts.push_back( cur.e_Tail );
		}else {							// あとは終点だけを入れていく
			e_Pts.push_back( cur.e_Tail );
		}
	}
}

/*
	アークをアーク列に挿入する
	@return	0	成功
	@return -1	失敗
*/
int		poly_cls::
Arc_Insert(	line_seg	c_Arc,	///< 挿入アーク
			int			c_Num )	///< アーク挿入位置
{
	if(c_Num < 0) {
		return	(-1);
	}

	//vector<line_seg>	a_TmpArcs;	// アーク列

	int	count	= e_Arcs.size();		// アーク列数(補間点数-1）
	if( c_Num > (count-1)) {
		return	(-1);
	}

	e_Arcs.insert(e_Arcs.begin()+c_Num, c_Arc);

	/*
	// 挿入位置の前までをテンポラリにコピー
	int	i	= 0;
	for(i = 0; i < c_Num; i++)
	{
		line_seg	cur	= e_Arcs[i];
		a_TmpArcs.push_back( cur );
	}
	// 該当アークをコピー
	a_TmpArcs.push_back( c_Arc );
	
	// 残りをコピー
	for(i = c_Num; i < count; i++)
	{
		line_seg	cur	= e_Arcs[i];
		a_TmpArcs.push_back( cur );
	}

	// 元のデータを消去して上書き
	e_Arcs.clear();
	vector<line_seg>::iterator	indx;
	for(indx = a_TmpArcs.begin(); indx != a_TmpArcs.end(); indx++)
	{
		e_Arcs.push_back( *indx );
	}*/
	return	(0);
}

/*
	アーク列をアーク列に挿入する
	@return	0	成功
	@return -1	失敗
*/
int		poly_cls::
Arcs_Insert(	poly_cls&	c_Poly,		///< 挿入アーク列
				int			c_Num )		///< アーク挿入位置
{
	if(c_Num < 0) {
		return	(-1);
	}

	std::vector<line_seg>	a_TmpArcs;	// アーク列

	int	count	= e_Arcs.size();		// アーク列数(補間点数-1）
	if( c_Num > (count-1)) {
		return	(-1);
	}
	e_Arcs.insert(e_Arcs.begin()+c_Num, c_Poly.e_Arcs.begin(), c_Poly.e_Arcs.end());

	return	(0);
}

/*
	ランダムなアーク列を接合順に並べる（元の点列を書き換えてしまう）
	@return	0	成功
	@return -1	失敗
*/
int		poly_cls::
Arc_Connect(	int	c_Begin	)		///< 接合開始アーク番号
{
	if(c_Begin < 0) {
		return	(-1);
	}

	int	count	= (int)e_Arcs.size();
	if( c_Begin > (count-1)) {
		return	(-1);
	}
	// 2003.7.29 サーチ用のデータ
	line_seg	a_lBegin_Seg	= e_Arcs[c_Begin];
	
	// 2003.7.29 接合アークの高速化
	std::multimap < Dpos, line_seg, std::less <Dpos> >	a_SrcArcs;
	std::list<line_seg>									a_TmpArcs;
	std::vector<line_seg>::iterator	indx;
	
	// テンポラリにデータをコピー
	int	a_iNo_Add_Count	= 0;	// 開始線分と同じ線分は一つは削除しておく
	for(indx = e_Arcs.begin(); indx != e_Arcs.end(); indx++)
	{
		line_seg	a_lTmp	= *indx;
		Dpos		a_dKey	= a_lTmp.e_Head;	// 始点でソート
		if( a_iNo_Add_Count == 0 && a_lTmp == a_lBegin_Seg ) {
			a_iNo_Add_Count++;
			continue;
		}
		a_SrcArcs.insert(std::multimap < Dpos, line_seg, std::less <Dpos> >::value_type( a_dKey, a_lTmp ));
	}

	std::list<line_seg>::iterator	indx_tmp;

	line_seg	ls_tmp	= a_lBegin_Seg;
	a_TmpArcs.push_back	( ls_tmp );	// 開始位置のデータをテンポラリに格納

	std::multimap< Dpos, line_seg, std::less <Dpos> >::iterator	indx2;
	for(indx_tmp = a_TmpArcs.begin(); (int)a_TmpArcs.size() != count; indx_tmp++)	// データが全部はけたら終わる
	{
		line_seg	cur_arc		= (*indx_tmp);

		std::multimap< Dpos, line_seg, std::less <Dpos> >	a_ChkArcs;

		// 接続アークのチェックループ
		int		a_iSame_Count	= a_SrcArcs.count ( cur_arc.e_Tail );
		int i = 0;
	//	for( indx2 = a_SrcArcs.lower_bound ( cur_arc.e_Tail ); i < a_iSame_Count; i++ )	// 現在アークの終点と、サーチ中のアークの始点が一致		
		for( indx2 = a_SrcArcs.lower_bound ( cur_arc.e_Tail ); i < a_iSame_Count; i++ )	// 現在アークの終点と、サーチ中のアークの始点が一致
		{
			line_seg	srch_arc	= (*indx2).second;	// サーチ中のアーク
			// チェック用テンポラリに放り込む
			a_ChkArcs.insert ( std::multimap < Dpos, line_seg, std::less <Dpos> >::value_type( srch_arc.e_Head, srch_arc ));
			a_SrcArcs.erase (indx2++);	// データいったん消す
		}

		//接続アークについての処理
		if(a_ChkArcs.size() == 1) {	// 対象アークがひとつしかない場合は、a_TmpArcsへコピーする
			line_seg	tmp = (*a_ChkArcs.begin()).second;
			a_TmpArcs.push_back(tmp);
		}else
		if(a_ChkArcs.size() > 1) {	// 対象アークが複数ある場合
			int		i		= 0;
			double	degree	= 0.0;	// 角度

			std::multimap < Dpos, line_seg, std::less <Dpos> >::iterator	chk_indx;
			std::multimap < Dpos, line_seg, std::less <Dpos> >::iterator	del_indx;
			for(chk_indx = a_ChkArcs.begin(); chk_indx != a_ChkArcs.end(); chk_indx++, i++)
			{
				// ２つのベクトルのなす角をチェックする
				line_seg	chk_arc		= (*chk_indx).second;
				double		cur_deg	= 0.0;
				dvec		v1, v2;
				v1.get_vec	(cur_arc.e_Head, cur_arc.e_Tail);	// 今見ているアーク
				v2.get_vec	(chk_arc.e_Head, chk_arc.e_Tail);	// 接続するアーク
				cur_deg		= v1.get_deg( v2 );
				
				if(chk_indx == a_ChkArcs.begin()) {	// ループの最初は初期化する
					degree		= cur_deg;
					del_indx	= chk_indx;
				}else {
					if(	!(cur_arc.e_Head == chk_arc.e_Tail &&
						  cur_arc.e_Tail == chk_arc.e_Head)) {	// 完全逆方向のアークは、対象からはずす
						if(cur_deg > degree ) {	// より角度の大きい方をとる
							degree		= cur_deg;
							del_indx	= chk_indx;
						}
					}
				}
			}

			line_seg	a_lDel_Seg	= (*del_indx).second;
			a_TmpArcs.push_back	(a_lDel_Seg);	// a_TmpArcsへコピー
			a_ChkArcs.erase		(del_indx);	// a_TmpArcsへ格納したデータは消す
		//	a_SrcArcs.erase		(del_indx);		
			// 対象外だったデータを、a_SrcArcsに戻して、再びチェック

			for(chk_indx = a_ChkArcs.begin(); chk_indx != a_ChkArcs.end(); chk_indx++)
			{
				line_seg	a_lReload	= (*chk_indx).second;
				a_SrcArcs.insert ( std::multimap < Dpos, line_seg, std::less <Dpos> >::value_type( a_lReload.e_Head, a_lReload ));
			}

		}else{	// アークがみつからなかった場合は、抜ける
			return	(-1);
		}
	}

	// コピー処理
	e_Arcs.clear();
	for(indx_tmp = a_TmpArcs.begin(); indx_tmp != a_TmpArcs.end(); indx_tmp++)
	{
	//	line_seg	a_lCopy	= (*indx_tmp);
	//	e_Arcs
		e_Arcs.push_back( *indx_tmp );
	}
	return	(0);
}

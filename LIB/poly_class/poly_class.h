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

///< ポリゴンの交差等 2000.6.02
#ifndef	_POLY_CLASS_
#define	_POLY_CLASS_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <set>

// 更新履歴
// 2003.3.5	未参照の変数を削除

///<#include <crd_cnv.h>								///< 2000.5.30 座標変換
/**
	このクラスでは、点、線、ポリゴンに関する処理をまとめ、汎用的に使える
	ものにすることが目的であるかもしれない。
*/

/**
	座標点
*/
struct	Dpos
{
	double	xpos;
	double	ypos;
	int		e_iAttr;
	Dpos	() {
		xpos	= 0.0;
		ypos	= 0.0;
		e_iAttr	= 0;
	}
	Dpos	(	double	c_xpos,
				double	c_ypos	) {
		xpos	= c_xpos;
		ypos	= c_ypos;
	}

	~Dpos	() {
	}

	/*
		データのセット
		@return 無し
	*/
	void	Data_Set(	double	c_xpos,		///< 代入座標点X
						double	c_ypos)		///< 代入座標点Y
	{
		xpos	= c_xpos;
		ypos	= c_ypos;
	}

	/**
		座標点同士の距離を獲得
	*/
	double	Get_Dist(
				Dpos	c_Pt		///< 対象点
			);
	/*
		等値演算子
	*/
	bool	operator==( const Dpos c_Pos ) const {	///< 比較データ
			if(	xpos == c_Pos.xpos &&
				ypos == c_Pos.ypos )
			{
				return true;
			}
			return false;
	}

	/**
		比較演算子
	*/
	bool	operator<( const Dpos c_Pos ) const {
		bool	ret	= false;
		if( xpos != c_Pos.xpos ) {
			if( xpos < c_Pos.xpos ) {
				ret	= true;
			}
		}else
		if( ypos != c_Pos.ypos ) {
			if( ypos < c_Pos.ypos ) {
				ret	= true;
			}
		}
		return ( ret );
	}

	/*
		代入演算子
	*/
	/*	void	operator=( Dpos	c_Pos)	///< 代入座標点
			{
				xpos	= c_Pos.xpos;
				ypos	= c_Pos.ypos;
			};*/
};

struct	Dpos2	// 2003.07.29　仮想線発生の高速化用で追加
{
	double	xpos;
	double	ypos;
	int		e_indx;
	Dpos2	() {
		xpos	= 0.0;
		ypos	= 0.0;
		e_indx	= 0;
	}
	Dpos2	(	double	c_xpos,
				double	c_ypos,
				int		c_indx) {
		xpos	= c_xpos;
		ypos	= c_ypos;
		e_indx	= c_indx;
	}

	~Dpos2	() {
	}

	/*
		データのセット
		@return 無し
	*/
	void	Data_Set(	double	c_xpos,		///< 代入座標点X
						double	c_ypos,		///< 代入座標点Y
						int		c_indx	)
	{
		xpos	= c_xpos;
		ypos	= c_ypos;
		e_indx	= c_indx;
	}

	/**
		座標点同士の距離を獲得
	*/
	double	Get_Dist(
				Dpos2	c_Pt		///< 対象点
			);
	/*
		等値演算子
	*/
	bool	operator==( const Dpos2 c_Pos ) const {	///< 比較データ
			if(	xpos == c_Pos.xpos &&
				ypos == c_Pos.ypos )
			{
				return true;
			}
			return false;
	}

	/**
		比較演算子
	*/
	bool	operator<( const Dpos2 c_Pos ) const {
		bool	ret	= false;
		if( xpos != c_Pos.xpos ) {
			if( xpos < c_Pos.xpos ) {
				ret	= true;
			}
		}else
		if( ypos != c_Pos.ypos ) {
			if( ypos < c_Pos.ypos ) {
				ret	= true;
			}
		}else
		if( e_indx != c_Pos.e_indx ) {
			if( e_indx < c_Pos.e_indx ) {
				ret	= true;
			}
		}
		return ( ret );
	}
};

typedef	std::set<Dpos2, std::less <Dpos2> >				s_Dpos2;
typedef	std::set<Dpos2, std::less <Dpos2> >::iterator	is_Dpos2;

/*
	線分
*/
struct	line_seg
{
	Dpos	e_Head;		// 線分の始点
	Dpos	e_Tail;		// 線分の終点

	/*
		コンストラクタ（デフォルト）
	*/
	line_seg	( ) {}

	/*
		コンストラクタ
	*/
	line_seg	(	Dpos	c_Head,		///< 代入座標点（始点）
					Dpos	c_Tail)		///< 代入座標点（終点）
	{
		e_Head	= c_Head;
		e_Tail	= c_Tail;
	}

	/**
		等値演算子定義
	*/
	int		operator==( const line_seg c_Seg ) const {
		int	ret	= 0;
		if( e_Head == c_Seg.e_Head &&
			e_Tail == c_Seg.e_Tail ) {
			ret	= 1;
		}
		return ( ret );
	}

	/*
		データのセット
		@return 無し
	*/
	void	Data_Set(	Dpos	c_Head,		///< 代入座標点（始点）
						Dpos	c_Tail)		///< 代入座標点（終点）
	{
		e_Head	= c_Head;
		e_Tail	= c_Tail;
	}

	/*
		線分の交差判定を行なう
		algorithm:
			線分１、２各々について互いに他の線分のどちら側にあるかを調べ
			両方とも異なる側にあれば交差していることになる
		@retval	0		線分が交差しない
		@retval	1		線分が交差する				
	*/
	int		Cross_Seg(
					line_seg	c_Seg	///< 対象線分
				);
	/*
		２点 c_pt1,c_pt2で構成される線分と点c_src_ptの距離を求める
		@return	距離(正規化座標値)
	*/
	double	get_pt_seg_dist(
					Dpos	c_Pt		///< 距離を求める点
				);
	/*
		PointsDiet で使ってたものを改造
		２点 c_pt1,c_pt2を通る直線と点c_src_ptの距離を求める
		@return	距離(正規化座標値)
	*/
	double	get_pt_line_dist(
					Dpos	c_Pt		///< 対象点
				);
};

struct	Crect
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;
	
	/*
		コンストラクタ
	*/
	Crect	(	void	)
	{
		xmin	= 0.0;
		ymin	= 0.0;
		xmax	= 0.0;
		ymax	= 0.0;
	}
	
	/*
		矩形同士の交差チェック
		外接長方形で、高速にチェックする
		@return	1 = 矩形1と矩形2が交差
		@return 内部 = 1 外 = 0
	*/
	int		Rect_Cross_Chk	(
					Crect	c_Rect		///< 対象矩形
				);

	/*
		矩形内に矩形が内包されるかどうか 2000.5.18
		外接長方形で、高速にチェックする
		@return 矩形2が矩形1の内部 = 1 外 = 0
	*/
	int		Rect_Inner_Chk	(
					Crect	c_Rect		///< 対象矩形
				);
	/*
		外接長方形で、高速にチェックする
		@return 点が内部 = 1 点が外 = 0
	*/
	int		Point_Inner_Chk	(
					Dpos		c_Pt	///< ポイント
				);

};

struct	dvec
{
	double	x;
	double	y;
	dvec	()	{
		x	= 0;
		y	= 0;
	}
	~dvec	() {}
	/*
		ベクトルの外積の値を求める(符号だけ)
		@return ２つの２次元ベクトルの外積の値
	*/
	int		get_gaiseki(
					dvec		c_Vec		///< vector
				);
	/*
		与えられた２点を結ぶベクトルを作成する
		@return 与えられた始点と終点の座標から作成されたベクトル
	*/				
	void	get_vec(
					Dpos		pt0,		///< ベクトルの始点の座標
					Dpos		pt1			///< ベクトルの終点の座標
				);
	/*
		ベクトルの内積の値を求める(符号だけ)
		@return 0 = なす角が90度
		@return 1 = なす角が鋭角
		@return	-1 = なす角が鈍角
	*/
	double	get_naiseki(
					dvec		c_Vec		///< 対象ベクトル
				);
	/*
		２つのベクトルのなす角を求める
		@return	角度
	*/
	double	get_deg(
					dvec		c_Vec
				);
};

/*
	点列に対して何らかの処理を行うクラス
*/
struct	poly_cls
{
		std::vector<Dpos>		e_Pts;			///< 点列
		std::vector<line_seg>	e_Arcs;			///< アーク列

		/*
			内部点列のクリア
		*/
		void	Clear_Pts() {
				e_Pts.clear();
		}

		/*
			内部アーク列のクリア
		*/
		void	Clear_Arcs() {
				e_Arcs.clear();
		}
		
		/*
			点列の回転（開始位置を変える）
			@return	0	成功
			@return -1	失敗
		*/
		int		Rot_Pts(
						int			c_Begin		///< 開始位置番号
					);
		/*
			点列の反転
			@return	0	成功
			@return -1	失敗
		*/
		int		Reverse_Pts();

		/*
			点列の分割
			@return	0	成功
			@return -1	失敗
		*/
		int		Div_Pts(
						int			c_Begin,	///< 分割始点番号
						int			c_Num,		///< 始点からの補間点数
						poly_cls*	c_Poly		///< 分割点列格納
					);
		/*
			点列から、アーク列へ内部的に変換
			@return	0	成功
			@return -1	失敗
		*/
		int		Make_Arcs();

		/*
			アーク列から、点列へ内部的に変換
		*/
		void	Make_Pts();

		/*
			アーク列を接合順に並べる
			@return	0	成功
			@return -1	失敗
		*/
		int		Arc_Connect(
						int			c_Begin		///< 接合開始アーク番号
					);

		/*
			アークをアーク列に挿入する
			@return	0	成功
			@return -1	失敗
		*/
		int		Arc_Insert(
						line_seg	c_Arc,		///< 挿入アーク
						int			c_Num		///< アーク挿入位置
					);

		/*
			アーク列をアーク列に挿入する
			@return	0	成功
			@return -1	失敗
		*/
		int		Arcs_Insert(
						poly_cls&	c_Poly,		///< 挿入アーク列
						int			c_Num		///< アーク挿入位置
					);


		/*
			ポリゴン点列から、外接矩形の獲得
		*/
		int		getouterrect(
						Crect	*rect		///< 獲得矩形
					);

		/*
			２つの折れ線の交点の個数を求める
			algorithm:
				２つの折れ線を構成する線分すべての組み合わせにたいして CrossSegment
				により交差判定を行ない交差している場合の数を求める

			@return	２つの折れ線の交点の個数
		*/
		int		crossline(
						poly_cls	c_Poly	///< チェック対象点列
					);
		
		/*
			PointsDiet で使ってたものを改造
			点c_src_ptと、Dpos *c_ptsで表されるラインの距離を求める
			@return 距離(正規化座標値)
		*/
		double	get_pt_arc_dist(
						Dpos	c_src_pt	///< 距離を求める点
					);

		/*
			点列と点列の距離を求める
			@return	点列同士の距離
		*/
		double	Get_Poly_Dist(
						poly_cls	c_Poly	///< 対象点列
					);

		/*
			矩形の中心がポリゴン内かどうかのチェック
		*/
		int		iscenterin(
						Crect	*rect,		///< チェック対象矩形
						Dpos	*RprPt		///< 代表点
					);

		/*
			ポリゴン内包チェック
			ポリゴン１の内部にポリゴン２が内包されるかどうか
			@return	0  ポリゴンは内包されない
			@return 1  ポリゴンは内包される
		*/
		int		innerpolycheck(
						poly_cls	c_Poly	///< チェック対象ポリゴン点列
					);

		/*
			与えられた点が多角形に対してどこにあるかを調べる
			@return	0	点は多角形の外側にある
			@return	1	点は多角形の内側にある
			@return -1	点は多角形の境界上にある
		*/
		int		innerpolygon(
						Dpos	c_Pt		///< 調べる点の座標
					);
		/*
			ポリゴンの代表点を探す
			@retval	0	見つかった
			@retval	1	見つからない
		*/
		int		Find_Center(
						Dpos	*RprPt		///< 代表点
					);

		/*
			ポリゴンの面積
		*/
		double	calc_area(
						double	c_xratio,	///< x方向比率?
						double	c_yratio	///< y方向比率?
					);

		/*
			内部アーク列の座標値の表示（動作確認目的）
		*/
		void	Print_Arcs( void )	{
			
			int	i = 0;
			std::vector<line_seg>::iterator	indx;
			printf("[count] : (head(x,y) - tail(x,y)\n");
			for(indx = e_Arcs.begin(); indx != e_Arcs.end(); indx++, i++)
			{
				line_seg	cur	= *indx;
				printf	("[%d] : (%0.5f, %0.5f - %0.5f, %0.5f)\n",
							i,
							cur.e_Head.xpos,
							cur.e_Head.ypos,
							cur.e_Tail.xpos,
							cur.e_Tail.ypos );
			}
		}
};

#endif	///<_POLY_CLASS_

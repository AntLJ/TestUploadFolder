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

#ifndef	_EXP_POLY_CLASS_H_
#define	_EXP_POLY_CLASS_H_

#include "StdAfx.h"
#include <poly_class.h>
/**
	poly_classの拡張クラス
	IGeometryPtrとの受け渡しがメイン？
*/

struct	line_seg2 : public line_seg
{
	int		e_iIndx;
	
	/// コンストラクタ
	line_seg2() {
		line_seg::line_seg();
		e_iIndx	= 0;
	}
	/// デストラクタ
	~line_seg2() {
	}

	/*
		２点 c_pt1,c_pt2で構成される線分と点c_src_ptの距離を求める
		@return	距離(正規化座標値)
	*/
	double		get_pt_seg_dist	(	Dpos2		c_src_pt	);	// 距離を求める点

	/*
		データのセット(XYの小さい順にする）
		@return 無し
	*/
	void	Data_Set(	Dpos2	c_Head,		///< 代入座標点（始点）
						Dpos2	c_Tail)		///< 代入座標点（終点）
	{
		if( c_Head.xpos < c_Tail.xpos ) {
			e_Head.xpos	= c_Head.xpos;
			e_Tail.xpos	= c_Tail.xpos;
		}else {
			e_Head.xpos	= c_Tail.xpos;
			e_Tail.xpos	= c_Head.xpos;
		}
		if( c_Head.ypos < c_Tail.ypos ) {
			e_Head.ypos	= c_Head.ypos;
			e_Tail.ypos	= c_Tail.ypos;
		}else {
			e_Head.ypos	= c_Tail.ypos;
			e_Tail.ypos	= c_Head.ypos;
		}
	}

	/**
		比較演算子
	*/
	int		operator<( const line_seg2 c_Seg ) const {
		int	ret	= 0;
		if( line_seg::e_Head.xpos != c_Seg.e_Head.xpos ) {
			if( line_seg::e_Head.xpos < c_Seg.e_Head.xpos ) {
				ret	= 1;
			}
		}else
		if( line_seg::e_Head.ypos != c_Seg.e_Head.ypos ) {
			if( line_seg::e_Head.ypos < c_Seg.e_Head.ypos ) {
				ret	= 1;
			}
		}else
		if( line_seg::e_Tail.xpos != c_Seg.e_Tail.xpos ) {
			if( line_seg::e_Tail.xpos < c_Seg.e_Tail.xpos ) {
				ret	= 1;
			}
		}else
		if( line_seg::e_Tail.ypos != c_Seg.e_Tail.ypos ) {
			if( line_seg::e_Tail.ypos < c_Seg.e_Tail.ypos ) {
				ret	= 1;
			}
		}
		return ( ret );
	}

};

/*
	点列に対して何らかの処理を行うクラス
*/
struct	exp_poly_class : public poly_cls
{
	set<Dpos2>			e_sExpPts;		///< XYでソートした点列
//	multiset<Dpos2>		e_msErrPts;		///< エラーとして検出される点のマルチセット

	vector<line_seg2>	e_vExpArcs;		///< アーク列
//	multiset<line_seg2>	e_msExpArcs;	///< アーク列

private:
	/**
		ジオメトリの点列を確保する
		@return	成功	WKSPoint点列へのポインタ
		@return	失敗	NULLポインタ
	*/
	WKSPoint*	Get_GeoPts(
					IGeometryPtr	c_ipGeo,	///< ジオメトリ[IN]
					long&			c_lCount	///< 補間点数[OUT]
				);
public:

	/// コンストラクタ
	exp_poly_class	() {
	}
	/// デストラクタ
	~exp_poly_class	() {
	}
	
	/**
		ジオメトリをばらけた点列に落とし込む	
		@return	0	成功
		@return -1	失敗
	*/
	int		Set_GeotoPts(
				IGeometryPtr	c_ipGeo,	///< ジオメトリ[IN]
				int				c_iID 		///< ID[IN]
			);

	/**
		ジオメトリをばらけたアーク列（ベクタ）に落とし込む	
		@return	0	成功
		@return -1	失敗
	*/
	int		Set_GeotoArcs(
				IGeometryPtr	c_ipGeo,	///< ジオメトリ[IN]
				int				c_iID		///< ID[IN]
			);

	/**
		アークに最も近い点を取り出す
		@return	0	取り出し成功
		@return	1	該当データ無し
	*/
	int		Get_NearestPt(
				line_seg2			c_lSeg,			///< アークの線分[IN]
				double				c_lDeg,			///< 閾値（度）[IN}
				vector<Dpos2>&		c_msErrPts		///< エラーとなる点のセット[OUT]
			);

	/**
		点に最も近いアークを取り出す
		@return	0	取り出し成功
		@return	1	該当データ無し
	*/
	int		Get_NearestArcs(
				Dpos2				c_lPt,			///< 点IN]
				double				c_lDeg,			///< 閾値（度）[IN}
				vector<line_seg2>&	c_msErrPts		///< エラーとなるアークのセット[OUT]
			);
};

#endif

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

//------------------------------------------------------------
//
// 代表点を扱うクラス
//
//------------------------------------------------------------
#include <stdio.h>
#include <set>

#ifndef	_REPRE_HANDLE_H_
#define	_REPRE_HANDLE_H_

using namespace stlport;

/**
	更新情報
		2003.4.16	repre_ptの代償判定を座標値も使用するように変更（こっちのほうがよい？）
*/

/**
	代表点用クラス
	１個の11桁コードを持つ行政界に大しては、１個の代表点しかないということを前提
*/
class	repre_pt
{
public:
	char	e_cpCode[12];	///< 11桁コード
	char	e_cpName[128];	///< 名称
	string	e_sLayerName;	///< レイヤ名称
	int		e_iType;		///< データタイプ
	int		e_iOid;			///< オブジェクトID
	double	e_dX;			///< 代表点X座標（経度？）
	double	e_dY;			///< 代表点Y座標（緯度？）
	IGeometryPtr	e_ipPt;	///< ポイントジオメトリ 2003.04.21
public:
	/**
		コンストラクタ
	*/
	repre_pt() {
		memset	( e_cpCode, '\0', 12 );
		memset	( e_cpName, '\0', 128 );
		e_sLayerName	= "";
		e_iType	= 0;
		e_iOid	= 0;
		e_dX	= 0.0;
		e_dY	= 0.0;
		e_ipPt	= NULL;
	}
	/**
		デストラクタ
	*/
	~repre_pt() {
	}

	/**
		比較演算子定義
		@return	真 = 1
		@return 偽 = 0
	*/
	int	operator<( const repre_pt& c_rDat ) const {
		int	ret	= 0;
		int	cmpret	= 0;
		cmpret	= strcmp ( e_cpCode, c_rDat.e_cpCode );	// 11桁での比較
		if( cmpret != 0 ) {
			if( cmpret < 0 ) {
				ret	= 1;
			}
		}else
		if( e_iType != c_rDat.e_iType ) {		// データタイプでの比較
			if( e_iType < c_rDat.e_iType ) {
				ret	= 1;
			}
		}
		return	( ret );
	}

	/**
		等値演算子定義
		@return	真 = 1
		@return 偽 = 0
	*/
	int	operator==( const repre_pt& c_rDat ) const {
		int	ret	= 0;
		if( strcmp ( e_cpCode, c_rDat.e_cpCode ) == 0 ) {
			if( e_dX == c_rDat.e_dX && e_dY == c_rDat.e_dY ) {
				ret	= 1;
			}
		}
		return	( ret );
	}

};

typedef	multiset<repre_pt, less<repre_pt> >				ms_repre_pt;
typedef	multiset<repre_pt, less<repre_pt> >::iterator	msi_repre_pt;

#endif	//_REPRE_HANDLE_H_

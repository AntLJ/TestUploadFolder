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
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
//#include "feature_info.h"

#ifndef	_MSI_HANDLE_WS_H_
#define	_MSI_HANDLE_WS_H_

#define	RET_COUNT	0x01
#define	RET_FCURSOR	0x02

/**
	ワークスペース等のハンドル用のクラス
*/
class	msi_handle_ws
{
protected:
	IFeatureWorkspacePtr	e_ipFeWSpace;		///< ワークスペース
	IFeatureClassPtr		e_ipFeCls;			///< 比較元フィーチャクラス
	IFeatureCursorPtr		e_ipFeCsr;			///< 比較元フィーチャカーソル
	char*					e_cpLayer;			///< レイヤ名称
	long					e_lData_Count;		///< 現在サーチかけた分のデータ数
public:
	/**
		コンストラクタ
	*/
	msi_handle_ws	() {
		e_ipFeWSpace	= NULL;
		e_ipFeCls		= NULL;
		e_ipFeCsr		= NULL;
		e_cpLayer		= NULL;
		e_lData_Count	= 0;
	}

	/**
		デストラクタ
	*/
	~msi_handle_ws	() {
		if( e_cpLayer != NULL ) {
			delete [] e_cpLayer;
		}
	}
	
	/**
		初期化メンバ
	*/
	bool	Init	(
		IWorkspacePtr	c_ipWSpace,		///< オープンしたワークスペース
		char*			c_cpLayer		///< レイヤ名称
	);
	
	/**
		比較先データを、比較元データのオブジェクトIDでサーチする
	*/
	IFeatureCursorPtr	Search_ID	(
		double			c_dObj_ID	///< 指定オブジェクトID
	);
	
	/**
		比較先データを、比較元データのshapeでサーチする
	*/
	IFeatureCursorPtr	Search_shp	(
		IGeometryPtr	c_ipGeo		///< 指定ジオメトリ
	);

	/**
		指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータを
		とってくる
	*/
	IFeatureCursorPtr	Search_Feature	(
		IGeometryPtr		c_ipGeo,		///< 指定ジオメトリ
		esriSpatialRelEnum	c_ipSpatialRel,	///< 関係指定
		std::string			c_sWhere,		///< 指定WHERE句
		int					c_iRetType		///< 何を返すか
	);

	/**
		WHERE句を指定してデータを取ってくる
		@return	IFeatureCursorPtr
	*/
	IFeatureCursorPtr	Search_Where	(
		std::string		c_sWhere,	///< 指定WHERE句
		int				c_iOrder=0	///< ソートするかのしてい（１=ソート、その他=ソートしない）
	);

	/**
		レイヤ名称を得る
	*/
	char*	Get_Layer_Name	( void ) {
		return ( e_cpLayer );		
	}

	/**
		検索データ件数を獲得する

	*/
	long	Get_Data_Count	( void ) {
		return	( e_lData_Count );		
	}

	/**
		カウントをクリアする
	*/
	void	Clear_Count	( void ) {
		e_lData_Count	= 0;
	}

	/**
		SpacialReferenceを獲得する
	*/
	ISpatialReferencePtr	Get_SpRef	( void ) {
		ISpatialReferencePtr	a_ipSpRef;
		if ( FAILED ( ((IGeoDatasetPtr )e_ipFeCls)->get_SpatialReference( &a_ipSpRef ) ) ) {
			return	( NULL );
		}
		return ( a_ipSpRef );
	}

};

#endif


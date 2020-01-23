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
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
#include "feature_info.h"

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
		指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータを
		とってくる
		ジオメトリがNULLの場合はWhere句だけの検索を行う
	*/
	IFeatureCursorPtr	Search_Feature	(
		IGeometryPtr		c_ipGeo,			///< 指定ジオメトリ
		esriSpatialRelEnum	c_ipSpatialRel,		///< 関係指定
		std::string			c_sWhere,			///< Where句をつける
		int					c_iRetType			///< 何を返すか
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
		エラー出力
	*/
	void	Print_Err	(
				char*	c_cpAddMsg,		///< 基本エラー情報に追加したいメッセージ
				char*	c_cpFuncName 	///< 関数名
			);


};

#endif


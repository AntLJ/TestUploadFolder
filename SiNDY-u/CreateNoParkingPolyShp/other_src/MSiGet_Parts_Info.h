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

#ifndef	__MSIGET_PARTS_INFO_H__
#define	__MSIGET_PARTS_INFO_H__

using	namespace	std;	// 2007.08.23 修正

#include <stdio.h>
#include <vector>
#include <WKSHelper.h>	// 2003.08.21 QueryExteriorその他の問題によりこちらで対応することにした

/**
	2002.10.16	Get_Geo_from_Partを追加
	2002.10.16	Get_Part_Polygonをsimplifyを用いて修正
	2003.06.13	Get_Part_Polygonをsimplifypreservefromto()を用いて修正
	2003.10.25	Init()時に、ポリゴンの状態を見て正常な状態に構築し直すモードを追加
*/

#define	GPI_NORMAL_MODE		0x00	///< ノーマルモード
#define	GPI_RECONFIG_MODE	0x01	///< ポリゴン再構成モード 2003.10.25

/**
	ジオメトリのパート情報を獲得する
*/
class	msi_get_parts_info
{
	long					e_lPart_Num;	///< パート数
	long					e_lSubPart_Max;	///< サブパート数の最大数
	IGeometryPtr			e_ipGeo;		///< ジオメトリ
	IGeometryCollectionPtr	e_ipGeoCol;		///< ジオメトリコレクション
	esriGeometryType		e_eGeoType;		///< ジオメトリ自体のオブジェクトのタイプ
	IPolygon2Ptr			e_ipPolygon;	///< ポリゴンインタフェース
	// 改良用に追加 2003.08.21 ------------------------------
	vector<RING>			e_vOuter;		///< アウターリング情報
	vector<RING>			e_vChk_Rings;	///< 全リング情報（チェック用）
	list<RING>				e_lRings;		///< 2003.08.21
	WKSPoint*				e_wpPoints;		///< 2003.08.21
	// -----------------------------------------------------
	// エラー出力用
	long					e_lObj_ID;		///< オブジェクトID
	string					e_sLayer_Name;	///< レイヤ名称

	int						e_iMeshCode;	///< メッシュコード
	FILE*					e_fpLog;		///< ログファイル用

private:
	/**
		既にInitされているかをチェックし、あらたに指定されたオブジェクトで　\
		初期化するかを決めるために使用する
	*/
	void	Pre_Init_Chk	( void ) {
		// 2003.08.21 追加
		if( e_wpPoints != NULL ) {
			delete [] e_wpPoints;
			e_wpPoints = NULL;
		}
	
	}

	/**
		アウターリング用の領域の確保
		@return	bool
	*/
	bool	Get_Outer_Buf	( void ) {
		// ダミー状態
		return	( true );
	}

	/**
		インナーリング用の領域の確保
		@return bool
	*/
	bool	Get_Inner_Buf	( void ) {
		// ダミー状態
		return	( true );
	}

	/**
		アウターリング用の領域の削除
		@return	なし
	*/
	void	Del_Outer_Buf	( void ) {
		// ダミー状態
	}


	/**
		インナーリング用の領域の削除
		※インナーは、アウターリング領域より先に解放される必要がある
		@return	なし
	*/
	void	Del_Inner_Buf	( void ) {
		// ダミー状態
	}

	
	/**
		エラープリント
		@return	無し
	*/
	void			Error_Print	(
						char*	c_cpErr_Code,	///< エラーコード
						char*	c_cpErr_Str		///< 出力エラー文字列
					);

	/** 2003.10.02
		リングがない方する中抜きリング（サブパート）のチェック用
		※基本的にInit内だけで使用
		@return	中抜きリング数
	*/
	int				Inner_Ring_Chk(
						int		c_iIndx			///< ポリゴンのインデックス
					);

	/** 2003.10.03
		リングに対して内側リングが直の中抜きポリゴンかどうかをチェックする
		※Init後に使用
		@return	bool
	*/
	bool			Is_Inner_Ring(
						RING&	c_rOuter,	///< 外側ポリゴン
						RING&	c_rInner 	///< 内側ポリゴン
					);
	/**　2003.10.25
		ポリゴンの状態を見て再構成する（他パートが自分の１レベル内包ポリゴンの場合に行う）
		@return	TRUE	再構成処理があったばあい
		@return	FALSE	再構成処理が無かった場合
	*/
	bool			Reconfig_Polygon( void );

public:
	/**
		コンストラクタ
	*/
	msi_get_parts_info	() {
		e_lPart_Num		= 0;
		e_lSubPart_Max	= 0;
		e_lObj_ID		= 0;		///< オブジェクトID
		e_wpPoints		= NULL;		///< 2003.08.21 追加

		e_iMeshCode		= 0;		///< メッシュコード
		e_fpLog			= stderr;	///< ログファイル用
	}
	/**
		デストラクタ
	*/
	~msi_get_parts_info	() {
		// 結局はPre_Init_Chkと同じことをやりたいのでそのまま使用
		Pre_Init_Chk	( );
	
	}

	/**
		初期化(このときに情報を取り出す)
		※2002.10.9　Initが複数回呼ばれたときにも対応
		@return	初期化成功 true
		@return 初期化失敗 false　
	*/
	bool	Init	( 
				IGeometryPtr	c_ipGeo,		///< ジオメトリ
				long			c_lObj_ID,		///< オブジェクトID
				string			c_sLayer_Name,	///< レイヤ名称
				int				c_iMeshCode,	///< メッシュコード
				FILE*			c_fpLog=stderr,	///< ログファイル用
				int				c_iMode=GPI_NORMAL_MODE	///< 再構成モードの設定（デフォルトは再構成しない）
			);

	/**
		パート数の獲得
		@return	パート数
	*/
	long	Get_Part_Num	( void );

	/**
		サブパート数の獲得
		@return	サブパート数
	*/
	long	Get_SubPart_Num	(
				long	c_lPart_Num	///< 指定パート番号
			);
	
	/**
		指定パート、サブパートの点列の獲得
		@return IPointCollection点列
	*/
	IPointCollectionPtr	Get_Points	(
				long	c_lPart_Num,	///< パート番号
				long	c_lSubPart_Num	///< サブパート番号
			);

	/**
		シェイプの指定パート、サブパートから、シェイプを作成する
		@return	true　成功
		@return false 失敗
	*/
	IGeometryPtr	Get_Geo_from_Part_Subpart	(
				long	c_lPart_Num,	///< パート番号
				long	c_lSubPart_Num	///< サブパート番号
			);

	/**
		指定パートのジオメトリを取り出す
		
		@return 指定パートのジオメトリ
	*/
	IGeometryPtr		Get_Geo_from_Part	(
				long	c_lPart_Num		///< パート番号
			);

	/**
		指定パートのポリゴンを取り出す（ポリゴンの場合）
		※与えられたジオメトリがポリゴンである場合のみ。もしライン等で使用した場合
		　には、NULLを返す
		@return 指定パートのポリゴン
	*/
	IPolygonPtr		Get_Part_Polygon	(
				long	c_lPart_Num		///< パート番号
			);
	
};
#endif

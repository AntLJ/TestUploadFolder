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
#include <StdAfx.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <string>
#include <crd_cnv.h>
#include "feature_info.h"
#include "prog_init_cls.h"
#include <smp_prn_info.h>
#include "msi_handle_ws.h"

using	namespace	std;	// 2007.09.03 修正

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_MESH			0x01				///< メッシュコード指定モード 2002.9.24
#define		ON_MONO_MESH	0x02				///< 単メッシュ指定	2003.04.07
#define		ON_LOG_FILE		0x04				///< ログファイル出力
#define		ON_PGDB_MODE	0x08				///< PGDBモード 2007.05.21
#define		ON_INIT_FILE	0x10				///< 初期化設定ファイル 2007.10.25

#define	IS_SCCLS	1	///< 	スケール種別
#define	IS_SCFLG	2	///<	スケールフラグ

struct	Geo_Rect
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;

	Geo_Rect () {
		xmin	= 0.0;
		ymin	= 0.0;
		xmax	= 0.0;
		ymax	= 0.0;
	}
	~Geo_Rect() {
	}
	bool	operator<(const Geo_Rect c_cDat ) const {
		bool	ret	= 0;
		if( xmin != c_cDat.xmin ) {
			if( xmin < c_cDat.xmin ) {
				ret	= 1;
			}
		}else
		if( ymin != c_cDat.ymin ) {
			if( ymin < c_cDat.ymin ) {
				ret	= 1;
			}
		}else
		if( xmax != c_cDat.xmax ) {
			if( xmax < c_cDat.xmax ) {
				ret	= 1;
			}
		}else
		if( ymax != c_cDat.ymax ) {
			if( ymax < c_cDat.ymax ) {
				ret	= 1;
			}
		}
		return	( ret );
	}
	bool	operator!=(const Geo_Rect c_cDat ) const {
		bool	ret	= 1;
		if( xmin == c_cDat.xmin &&
			ymin == c_cDat.ymin &&
			xmax == c_cDat.xmax &&
			ymax == c_cDat.ymax )
		{
			ret	= 0;
		}
		return	( ret );
	}
	void	operator=( Geo_Rect c_cDat ) {
		xmin	= c_cDat.xmin;
		ymin	= c_cDat.xmin;
		xmax	= c_cDat.xmax;
		ymax	= c_cDat.ymax;
	}
	//--------------------------------------------------------------------
	//	矩形同士の交差チェック
	//	外接長方形で、高速にチェックする
	//	返り値
	//		矩形1と矩形2が交差 = 1
	//		内部 = 1 外 = 0
	//--------------------------------------------------------------------
	bool	Rect_Cross_Chk	(	Geo_Rect c_cDat	)	//	矩形2
	{
		// 外れる条件でチェックする
		if((xmax < c_cDat.xmin ||
			xmin > c_cDat.xmax ||
			ymax < c_cDat.ymin ||
			ymin > c_cDat.ymax))
		{
			return(false);
		}
		return(true);
	}
};

/**
	IGeometryPtr自体のvectorとかの使用がうまくいかないので、要素にIGeometryPtrを
	持つ構造体を作成する	2002/05/23
*/
struct	geometry_inc
{
	int				e_iOid;			// オブジェクトID
	IGeometryPtr	e_ipGeometry;	// ジオメトリ
	Geo_Rect		e_igRect;		// 矩形

	geometry_inc	(){
		e_iOid	= 0;
	};
	geometry_inc	(IGeometryPtr	c_ipGeometry)
	{
		e_iOid			= 0;
		e_ipGeometry	= c_ipGeometry;
	}
	~geometry_inc	(){};

	bool	operator<(const geometry_inc c_cDat ) const {
		bool	ret	= 0;
		if( e_igRect != c_cDat.e_igRect ) {
			if( e_igRect < c_cDat.e_igRect ) {
				ret	= 1;
			}
		}
		return	( ret );
	}

};

typedef		vector<geometry_inc>				v_geometry_inc;
typedef		vector<geometry_inc>::iterator		iv_geometry_inc;

typedef		multiset<geometry_inc,less<geometry_inc> >	ms_geometry_inc;
typedef		multiset<geometry_inc,less<geometry_inc> >::iterator	ims_geometry_inc;

/**
	チェック済みを確認するよう
*/
struct	checked_id
{
	int		e_iOid_1;
	int		e_iOid_2;
	checked_id	() {
		e_iOid_1	= 0;
		e_iOid_2	= 0;
	}
	~checked_id() {}

	bool	operator<(const checked_id c_cDat ) const {
		bool	ret	= 0;
		if( e_iOid_1 != c_cDat.e_iOid_1 ) {
			if( e_iOid_1 < c_cDat.e_iOid_1 ) {
				ret	= 1;
			}
		}else
		if( e_iOid_2 != c_cDat.e_iOid_2 ) {
			if( e_iOid_2 < c_cDat.e_iOid_2 ) {
				ret	= 1;
			}
		}
		return	( ret );
	}
};

class	main_rootin
{
	public:
		char*					e_cpMeshCode_File;		///< メッシュコードリストファイル 2002.9.24
		char*					e_cpLog_File;			///< ログデータ出力パス
		char*					e_cpInit_File;			///< 初期化ファイル
		char*					e_sCheck_Layer;			///< チェックレイヤ名
		char*					e_cpPGDB_File;			///< PGDBファイル名 2007.05.21
		int						eMake_Type;				///< データ作成タイプ 2002.06.26
		int						eExe_Type;				///< 実行タイプ
		int						eDigit;					///< メッシュコード桁数
		int						eExe_Flag;
		int						e_iMeshCode;			///< 指定メッシュコード 2003.4.7
		double					e_iRange_Dist;			///< 設定範囲（m単位）
//		IWorkspacePtr			e_ipWorkSpace;
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<　エラーログファイル用ファイルポインタ
		set<int, less<int> >	e_sExi_Mesh_List;		///<　存在メッシュのリスト 2002.12.06
		smp_sindy_err_info		e_sErr_Log;				///<  エラーログ用 2007.05.21
		prog_init				e_pTool_Init;			// DBの初期化用設定

	public:

		main_rootin	()
		{
			e_cpMeshCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpInit_File			= NULL;
			e_sCheck_Layer			= NULL;
			e_cpPGDB_File			= NULL;
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_iRange_Dist			= 0.0;
			e_fErr_Fp				= stderr;	// デフォルトは標準エラー出力へ
			e_sExi_Mesh_List.clear();
		}
		~main_rootin	()
		{
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}

		/**
			引数チェック
			@return	成功	true
			@return	成功	false
		*/
		bool			arg_check	(
							int		argc,			///< 引数個数
							char	**argv			///< 引数列
						);

		/**
			ツールの使用法の表示
		*/
		void			print_usage	(
							char	**argv			///< 引数列
						);

		/**
			バージョン情報の表示
			@return		無し
		*/
		void			print_version	( void );


		/**
			メッシュリストファイル名を返す
			@return		ファイル名へのポインタ
		*/
		char*			Ret_Mesh_File	( void ) {
							return	( e_cpMeshCode_File );
						}

		/**
			ログデータ出力ファイル名を返す
			@retur		パス名へのポインタ
		*/
		char*			Ret_Log_File	( void ) {
							return	( e_cpLog_File );
						}

		/**
			初期化ファイル名を返す
			@return		ファイル名へのポインタ
		*/
		char*			Ret_Init_File	( void ) {
							return	( e_cpInit_File );
						}

		/**
			チェックするレイヤの名称
			@return		ファイル名へのポインタ
		*/
		char*			Ret_Check_Layer	( void ) {
							return	( e_sCheck_Layer );
						}

		/**
			メッシュ矩形をDBから獲得するメソッドの初期化 \
			※get_mesh_rect をやる前に必ず呼ぶ
		*/
		bool			get_mesh_init	(
							string			c_sMeshLayer,		///< メッシュレイヤ名
							IWorkspacePtr	c_ipWorkSpace		///< 指定するワークスペース
						);
		/**
			メッシュ矩形をGeoDBから取得する \
			※GeoDBへの接続が確立されていることが前提
		*/
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< メッシュコード
						);

		/**
			メッシュリストをGeoDB上から取得する
			@return	bool
		*/
		bool			get_mesh_list	(
							long	c_lDigit					///< メッシュコードの桁数
						);

		/**
			指定メッシュが存在するかのチェック
			@return	bool
		*/
		bool			chk_mesh_exist	(
							int				c_iMeshCode			///< メッシュコード
						);
	
		/**
			エラーログファイルをオープンする
		*/
		bool			Open_Err_Log	( void );

		/**
			エラーログを送るファイルポインタを返す
		*/
		FILE*			Log_Fp			( void ) {
							return	( e_fErr_Fp );
						}

		// 2006.05.16
		/**
			スケール種別の判定
			@return	0		スケール種別でない
			@return	IS_SCCLS(1)	スケール種別
			@return	IS_SCFLG(2)	スケールフラグ
		*/
		int				Scale_Attr_Chk	(
							string&		c_sFieldName,	///< フィールド名
							int*		c_ipScale		///< 該当スケール
						);

		/**
			スケール種別の整合性の判別
		*/
		int				Chk_Scale_Class_Adjust (
							int*		c_ipScaleAttr
						);

		/**
			スケールフラグの整合性の判別
		*/
		int				Chk_Scale_Flag_Adjust (
							int*		c_ipScaleAttr
						);

		/**
			DBへの接続オープン
			@return	true	成功
			@return	false	失敗
		*/
		bool			DB_Open	(
							IWorkspaceFactoryPtr&	c_ipWorkspaceFactory,
							IPropertySetPtr&		c_ipPropertySet,
							IWorkspacePtr&			c_ipWorkspace
						);

		/**
			チェック実行部
			※内部でエラーファイルの処理等も行う
			@return	成功	true
			@return 失敗	false
		*/
		bool			check_exe	( void );

		/**
			レイヤ名称から必要なワークスペースを判定する
		*/
		IWorkspacePtr	Get_Ref_Workspace	(
							IWorkspacePtr	c_ipWSpace_Map,		///< ワークスペース
							IWorkspacePtr	c_ipWSpace_Base,	///< ワークスペース
							IWorkspacePtr	c_ipWSpace_Addr,	///< ワークスペース
							char*			c_cpLayerName
						);

};

#endif	//_MAIN_ROOTIN_H_

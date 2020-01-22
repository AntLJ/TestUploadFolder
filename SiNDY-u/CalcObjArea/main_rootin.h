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

#include "sindy/workspace/name.h"

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_MESH			0x01		///< メッシュコード指定モード 2002.9.24
#define		ON_BASEMODE		0x02		///< 中縮尺モード 2002.10.25
#define		ON_MONO_MESH	0x04
#define		ON_WHERE_STR	0x08
#define		ON_PGDB_MODE	0x10
#define		ON_LOGFILE		0x20		///< ログファイル
#define		ON_OUTFILE		0x40		///< 出力ファイル
#define		ON_ADDRDB		0x80		///< 住所DB
#define		ON_FIELDS		0x100		///< 情報取得フィールド
#define		ON_DB_CNCT		0x200		///< DB初期化
#define		ON_CHKLAYER		0x400		///< チェックレイヤ
#define		ON_MESHLAYER	0x800		///< メッシュレイヤ
#define		ON_KU_MODE		0x1000		///< 特別区はまとめて市にする
#define		ON_GUN_MODE		0x2000		///< xx郡をつける

struct	data_info
{
	string		e_sAddrCode;	// 住所コード
	string		e_sChimei;		// 地名
	vector<CComVariant>	e_vcAttrs;	// 取得属性値

//	int			e_iMeshCode;	// メッシュコード
//	unsigned	e_uDataType;	// データタイプ 0x30, 0x50
//	unsigned	e_uLayerID;		// レイヤID
//	unsigned	e_uAttrCode;	// 種別コード

	double		e_dSum;			// 合計値

	/**
		コンストラクタ
	*/
	data_info	( void ) {
//		e_iMeshCode	= 0;
//		e_uDataType	= 0;
//		e_uLayerID	= 0;
//		e_uAttrCode	= 0;
		e_dSum	= 0.0;
	}
	/**
		デストラクタ
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_vcAttrs != c_dDat.e_vcAttrs ) {
			if( e_vcAttrs < c_dDat.e_vcAttrs ) {
				ret	= true;
			}
		}else
		if( e_sAddrCode != c_dDat.e_sAddrCode ) {
			if( e_sAddrCode < c_dDat.e_sAddrCode ) {
				ret	= true;
			}
		}else
		if( e_sChimei != c_dDat.e_sChimei ) {
			if( e_sChimei < c_dDat.e_sChimei ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;

struct	Norm_Pt
{
	int	e_iX;
	int	e_iY;
	Norm_Pt() {
		e_iX	= 0;
		e_iY	= 0;
	}
};

/**
	2005.09.28	Get_Obj_Len_Areaを元に作成
*/

class	main_rootin
{
	public:
		string					e_sMeshCode_File;		///< メッシュコードリストファイル 2002.9.24
		string					e_sLog_File;			///< ログデータ出力パス
		string					e_sConnect_Info;		///< 接続情報文字列
		string					e_sCheck_Layer;			///< チェックレイヤ名
		string					e_sMesh_Layer;			///< メッシュレイヤ名
		string					e_sOut_File;			///< 出力ファイル
		int						eMake_Type;				///< データ作成タイプ 2002.06.26
		int						eExe_Type;				///< 実行タイプ
		int						eDigit;					///< メッシュコード桁数
		int						eExe_Flag;
		int						e_iMeshCode;			///< メッシュコード
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fpErr_Fp;				///< エラーログファイル用ファイルポインタ
		FILE*					e_fpOut_Fp;				///< 出力ファイル用ポインタ
		set<int, less<int> >	e_sExi_Mesh_List;		///< 存在メッシュのリスト 2002.12.06
	private:
		prog_init				e_pTool_Init;		// DBの初期化用設定
		string					e_sWhere_Str;		///< Where句用
		common_use				e_cCU;				///< 2007.12.10
		string					e_sVersion;			///< ツールのバージョン
		string					e_sConnectAddrDB;	///< 住所DBの接続設定
		string					e_sFields;			///< 取得フィールド指定
		vector<string>			e_vsGetField;		///< 情報を取得するフィールド

		addr_db_handle			e_aAddr_DB_Hndl;	///< 住所DBのハンドリング

	public:

		main_rootin	()
		{
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_fpErr_Fp				= stderr;	// デフォルトは標準エラー出力へ
			e_fpOut_Fp				= stdout;	// デフォルトは標準出力へ
			e_sExi_Mesh_List.clear();
			e_sWhere_Str			= "";
		}
		~main_rootin	()
		{
			if( e_fpErr_Fp != NULL && e_fpErr_Fp != stderr && e_fpErr_Fp != stdout ) {
				fclose ( e_fpErr_Fp );
			}
			if( e_fpOut_Fp != NULL && e_fpOut_Fp != stderr && e_fpOut_Fp != stdout ) {
				fclose ( e_fpOut_Fp );
			}
		}

		//------------------------------------------------------
		// エラーメッセージ
		//------------------------------------------------------
		void			PrnMsg	( FILE* c_cpFp,	const char* c_cpFormat, ... ) {
							
							va_list	a_vArgList = NULL;

							va_start(a_vArgList, c_cpFormat );	// 初期化

							vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
							fflush	( c_cpFp );
						}

		/**
			DBへの接続オープン
			@return	true	成功
			@return	false	失敗
		*/
		bool			DB_Open	(
							IWorkspacePtr&			c_ipWorkspace
						);

		/**
			住所DB関係のオープン
			@return 成功	true
			@return	失敗	false
		*/
		bool			Connect_to_Adm_DB	( void );	///< 接続文字列[IN]


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
			エラーログのヘッダを出力する
			@return	無し
		*/
		void			Output_Log_Header	( void );

		/**
			メッシュ矩形をDBから獲得するメソッドの初期化 \
			※get_mesh_rect をやる前に必ず呼ぶ
		*/
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			// 入力リストクラス
						//	string			c_sMeshLayer,		///< メッシュレイヤ名
							IWorkspacePtr	c_ipWorkSpace		///< 指定するワークスペース
						);
		/**
			メッシュ矩形をGeoDBから取得する \
			※GeoDBへの接続が確立されていることが前提
		*/
		IPolygonPtr		get_mesh_rect	(
							int				c_iMeshCode			///< メッシュコード
						);
	
		/**
			エラーログファイルをオープンする
		*/
		bool			Open_Err_Log	( void );

		/**
			レイヤ名から比較用の属性文字列を作成する
		*/
		//string	Get_Attr_Str	(	string	c_sLayer,	///< レイヤ名

		/**
			フィーチャから属性を取り出す
		*/
		void			GetFAttrs	(
							Feature_All_Attrs&	c_fAttrs,	///< 属性値[IN]
							data_info&			c_dInfo		///< 取り出した情報[OUT]
						);

		/**
			チェック実行部
			※内部でエラーファイルの処理等も行う
			@return	成功	true
			@return 失敗	false
		*/
		bool			check_exe	(
							IWorkspacePtr	c_ipWSpace		///< ワークスペース
						);

		/**
			矩形のラインを
			@return	IPolylinePtr
		*/
		IPolylinePtr	Get_Rect_Line	(
							IEnvelopePtr	c_ipEnv			///< メッシュ矩形
						);
		/**
			矩形のポリゴンを
			@return	IPolygonPtr
		*/
		IPolygonPtr		Get_Rect_Polygon	(
							IEnvelopePtr	c_ipEnv			///< メッシュ矩形
						);

		/**
			クリップ図形を返す
		*/
		IGeometryPtr	Get_Clip_Obj	(
							IGeometryPtr	c_ipGeo,		///< ジオメトリ
							IGeometryPtr	c_ipClip		///< これでクリップ
						);

		/**
			オブジェクトの面積及びアーク長を返す
		*/
		double			Get_Obj_LA	(
							IGeometryPtr	c_ipGeo,		///< ジオメトリ[IN]
							int				c_iMeshCode,	///< メッシュコード[IN]
							int*			c_iType			///< データタイプ 1=line, 2=polygon
						);

		/**
			面積値を求める
		*/
		double			CalcArea	(
							IGeometryPtr	c_ipSrc_Geo,	///< クリップ前のジオメトリ
							IGeometryPtr	c_ipCliped_Geo	///< クリップ後のジオメトリ
						);

		/**
			長さの計算
			@return	正規化座標値での距離	
		*/
		double			Get_Length	(
							int				c_iPtCount,	// ポイント数
							Norm_Pt*		c_ipPts		// IN点列
						);

		/**
			ポリゴンの面積
		*/
		double			Get_Real_Area	(
							int				c_count,	// 補間点数
							Norm_Pt*		c_ipPts,	// IN点列
							int				c_meshcode	// メッシュコード
						);
		/**
			ポリゴンの面積
		*/
		double			calc_area	(
							int				c_count,	// 補間点数
							Norm_Pt*		c_ipPts,	// IN点列
							double			c_xratio,	// x方向比率?
							double			c_yratio	// y方向比率?
						);

		/**
			情報の追加
			@return	無
		*/
		void			Put_Info	(
							ms_data_info&	c_mInfos,
							data_info		c_dInfo
						);

		/**
			情報の出力(集計)
			@return	無し
		*/
		void			Print_Info	(
							ms_data_info&	c_mInfos,		///< 出力情報
							FILE*			c_fpOutFp		///< 出力ファイルポインタ
						);

		/**
			情報の出力(単発)
			@return	無し
		*/
		void			Print_Info	(
							int				c_iMeshCode,	///< メッシュコード[IN]
							data_info&		c_mInfos,		///< 出力情報[IN]
							FILE*			c_fpOutFp		///< 出力ファイルポインタ[IN]
						);


		/**
			点列にする
		*/	
		bool			Get_Norm_Ps	(
							IGeometryPtr		c_ipGeo,	///< ジオメトリ[IN]
							int					c_iMeshCode,///< メッシュコード[IN]
							int					c_iMode,	///< 0=line,1=Polygon
							vector<Norm_Pt>&	c_vPts		///< 点列[OUT]
						);


};

//------------------------------------------------------<ここまで>



#endif	//_MAIN_ROOTIN_H_

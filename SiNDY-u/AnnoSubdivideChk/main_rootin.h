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

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#include <sindy/schema.h>
#include "sindy/workspace/name.h"

#define		ON_MESH			0x02				///< メッシュコード指定モード 2002.9.24
#define		ON_INNER_CHK	0x04				///< 中縮尺行政界内包チェックモード　2002.11.13
#define		ON_MONO_MESH	0x08				///< 単メッシュ指定	2002.04.01
#define		ON_LOGFILE		0x10				///< ログファイル指定
#define		ON_INIT_FILE	0x20				///< 初期化ファイル読み込み用
// 2004.02.03
#define		ON_ROADNUM_ONLY	0x40				///< 道路番号のみチェックモード
#define		ON_FELLY_ONLY	0x80				///< フェリー関係のみチェックモード
#define		ON_PGDB_MODE	0x100				///< PGDBファイルモード（SiNDY-Cとは別）
#define		ON_OUT_FUYOU_MESSAGE		0x200	///< 細分化不要のものについてもログを出力
#define		ON_DEL_DISP_LINE_MESSAGE	0x400	///< 表示ラインについてのメッセージを出力しない
#define		ON_NEAREST_CHK				0x800	///< 一番近い路線とのみチェックを行う

struct	disp_pt_info
{
	int			e_iStat;	// 1=真位置、0=表示文字列
	int			e_iOid;
	int			e_iScale;
	IPointPtr	e_ipPt;
	disp_pt_info(){
		e_iStat	= 0;
		e_iOid	= 0;
		e_iScale= 0;
	}
};

struct	data_info
{
	int					m_iOID;			///< オブジェクトID
//	int					m_iAttr;		///< 種別
//	int					m_iUnder_Flag;	///< 地下フラグ
	Feature_All_Attrs	m_faaAttr;		///< オブジェクトの持つ全属性値
	IGeometryPtr		m_ipGeo;		///< ジオメトリ
	/**
		コンストラクタ
	*/
	data_info	( void ) {
		m_iOID	= 0;
	}
	/**
		デストラクタ
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( m_iOID != c_dDat.m_iOID ) {
			if( m_iOID < c_dDat.m_iOID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
/*	bool	operator!=(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ||
			e_iPrim_ID != c_dDat.e_iPrim_ID )
		{
			ret	= true;
		}
		return	( ret );
	}*/
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;
typedef	set<data_info, less <data_info> >					s_data_info;
typedef	set<data_info, less <data_info> >::iterator			is_data_info;

class	main_rootin
{
public:
	///ログに出力するエラーメッセージコードの定義
	enum ERROR_MESSAGE_CODE {
		NONE_SUBDIVIDE,      //!< 細分化の必要無し
		ABLE_SUBDIVIDE,      //!< 細分化可能
		RAIL_INCONSISTENT,   //!< 路線と不整合
		GROUND_INCONSISTENT, //!< 地上地下が反対
		NON_SUBJECT,         //!< 対象外種別
	};

	char*				e_cpAdminCode_File;		///< 行政コードリストファイル
	char*				e_cpMeshCode_File;		///< メッシュコードリストファイル 2002.9.24
	char*				e_cpAddrTxt_Path;		///< 住所DBテキストパス
	char*				e_cpLog_File;			///< ログデータ出力パス
	char*				e_cpInit_File;			///< 初期化ファイル
	char*				e_cpErrList;			///< エラーリストファイル
	int					eMake_Type;		///< データ作成タイプ 2002.06.26
	int					eExe_Type;		///< 実行タイプ
	int					m_iDigit;		///< メッシュコード桁数
	int					eExe_Flag;
	double				e_Warn_Dist;	///< ワーニング距離
	double				e_Err_Dist;		///< エラー距離
	double				e_dDev_Dist;	///< デバイス座標ポイントでの距離（デフォルト16pt）
	int					e_MeshCode;		///< 単メッシュ
//		IWorkspacePtr		e_ipWorkSpace;

	msi_handle_ws		m_mAny_Railway_WS;		// 都市地図、中縮尺系の鉄道関連データ
	msi_handle_ws		m_mAny_Note_WS;			// 都市地図、中縮尺系の注記関連データ
	msi_handle_ws		m_mAny_Note_Line_WS[4]; // 注記ライン（4レイヤ分）
	msi_handle_ws		m_mAny_City_Line_WS;	// 都市地図背景ライン（フェリー航路関係のため）

	string				m_sAny_Railway_Layer;	// 鉄道レイヤ名
	string				m_sAny_Note_Layer;		// 注記レイヤ名
	string				m_sAny_Note_Line_Layer[4];
//	char*				m_cpAny_City_Line_Layer;

	string				e_sVersion;				///< ツールバージョン

	IWorkspacePtr       m_ipWSpace_Map;         ///< 都市地図系ワークスペース
	IWorkspacePtr       m_ipWSpace_Base;        ///< 中縮尺系ワークスペース
	
	mk_input_list		m_mChk_List;

	IFeatureClassPtr	e_ipF_Class;
	FILE*				e_fErr_Fp;		///<	エラーログファイル用ファイルポインタ
	prog_init			e_pTool_Init;	// DBの初期化用設定
	smp_sindy_err_info	e_sPrnInfo;		// エラー等出力用

	char*				e_cpPGDB_File;	// PGDBファイル

	map<long, string, less<long> >	m_mAttrStrTbl;	// 種別と属性の対応テーブル（注記と鉄道混在）

//	char*				e_cpChk_Note_Layer;
private:
	/** ---------------------------------------------------------------------------
	//	一度デバイス座標に変換し、その後緯度経度に戻す
	//	@return	IPointPtr
	-----------------------------------------------------------------------------*/
	IPointPtr	get_Device_Pt	(
					int				c_iMeshCode,
					IPointPtr		c_ipPoint,
					int				c_iScale 	///< スケール
				);

	/** ---------------------------------------------------------------------------
	//	SJISへの変換
	//	@return	変換後文字列
	-----------------------------------------------------------------------------*/
	string		Str_to_SJIS		(
					CComBSTR*		c_CStr		// 文字列(Unicode)
				);

	/** ---------------------------------------------------------------------------
	//	指定の長さから何度に当たるかを返す
	//	@return	度単位での距離
	-----------------------------------------------------------------------------*/
	double		Get_Degree_Range(
					int				c_iMeshCode,	///< メッシュコード 
					double			c_dMeter_Range 	///< メートル単位の範囲
				);

	/** ---------------------------------------------------------------------------
	//	指定の長さから何度に当たるかを返す
	//	@return	度単位での距離
	-----------------------------------------------------------------------------*/
	double		Get_Degree_Range_Dev (
					int				c_iMeshCode,		///< メッシュコード 
					int				c_iScale,			///< スケール
					int				c_iDev_Dist 		///< デバイス座標点値での距離
				);


	/** ---------------------------------------------------------------------------
	//	指定の長さから何度に当たるかを返す
	-----------------------------------------------------------------------------*/
	void		Get_Degree_Range_Dev (
					int				c_iMeshCode,		///< メッシュコード[IN]
					int				c_iScale,			///< スケール[IN]
					int				c_iDev_Dist, 		///< デバイス座標点値での距離[IN]
					double*			c_cpRetXDeg,		///< X方向の距離[OUT]
					double*			c_cpRetYDeg 		///< Y方向の距離[OUT]
				);

	/** ---------------------------------------------------------------------------
	//	真位置も含めた表示位置を全て取り出す
	-----------------------------------------------------------------------------*/
	bool		Get_Disp_Pts	(
					int						c_iMeshCode,		// メッシュコード
				//	int						c_iAttr_Code,		// 属性コード
					Feature_All_Attrs&		c_fFAtt,			// 注記真位置の情報
					msi_handle_ws*			c_mAny_Note_Line_WS,// 該当ラインの検索用
					vector<disp_pt_info>&	c_vPoints
				);
										
	/** ---------------------------------------------------------------------------
	//	ラインから記号中心位置にあたる点を取り出す
	-----------------------------------------------------------------------------*/
	IPointPtr	Get_Center_Pt	(
					IGeometryPtr	c_ipGeo		// ジオメトリ
				);

	/** ---------------------------------------------------------------------------
	//	ラインから記号中心位置にあたる点を取り出す
	-----------------------------------------------------------------------------*/
	IPointPtr	Get_Center_Pt2	(
					IGeometryPtr	c_ipGeo,		///< ジオメトリ
					int				c_iMeshCode,	///< メッシュコード
					int				c_iScale		///< スケール
				//	int				c_iFontSize 	///< フォントサイズ
				);

	/** ---------------------------------------------------------------------------
		ポイントとアークの距離を求める
		@return	double	距離
	-----------------------------------------------------------------------------*/
	double		Get_Dist	(
					IPointPtr&		c_ipPt,			///< ポイント[IN]
					const IGeometryPtr&	c_ipLine		///< ライン[IN]
				);

public:
	//	enum make_type {
	//		back			= 1,
	//		pol				= 2,
	//		note			= 3
	//	};
		/// コンストラクタ
		main_rootin	()
		{
			namespace anno_code = sindy::schema::annotation::annotation_code;

			e_cpAdminCode_File		= NULL;
			e_cpMeshCode_File		= NULL;
			e_cpAddrTxt_Path		= NULL;
			e_cpLog_File			= NULL;
			e_cpInit_File			= NULL;
			e_cpErrList				= NULL;
		//	m_cpAny_City_Line_Layer	= NULL;

			e_cpPGDB_File			= NULL;

			eMake_Type				= 0;
			eExe_Type				= 0;
			m_iDigit				= 0;
			eExe_Flag				= 0;
			e_dDev_Dist				= 16.0;
			e_Warn_Dist				= 1.0;	// 1.0km以上、e_Err_Dist未満はワーニング
			e_Err_Dist				= 3.0;	// 3.0km以上はエラー
			e_MeshCode				= 0;
			e_fErr_Fp				= NULL;
			// 種別名称の表
			m_mAttrStrTbl[anno_code::kShinkansenLineName]  = "新幹線路線名";
			m_mAttrStrTbl[anno_code::kJRLineName]          = "ＪＲ路線名";
			m_mAttrStrTbl[anno_code::kQuasiPublicLineName] = "第３セクター路線名";
			m_mAttrStrTbl[anno_code::kPrivateLineName]     = "私鉄路線名";
			m_mAttrStrTbl[anno_code::kSubwayLineName]      = "地下鉄路線名";
			m_mAttrStrTbl[anno_code::kMonorailLineName]    = "モノレール路線名";
			m_mAttrStrTbl[anno_code::kShinkotsuLineName]   = "新交通路線名";

			m_mAttrStrTbl[anno_code::kShinkansenSubLineName]  = "新幹線路線名（地下）";
			m_mAttrStrTbl[anno_code::kJRSubLineName]          = "ＪＲ路線名（地下）";
			m_mAttrStrTbl[anno_code::kQuasiPublicSubLineName] = "第３セクター路線名（地下）";
			m_mAttrStrTbl[anno_code::kPrivateSubLineName]     = "私鉄路線名（地下）";
			m_mAttrStrTbl[anno_code::kSubwaySubLineName]      = "地下鉄路線名（地下）";
			m_mAttrStrTbl[anno_code::kMonorailSubLineName]    = "モノレール路線名（地下）";
			m_mAttrStrTbl[anno_code::kShinkotsuSubLineName]   = "新交通路線名（地下）";

			m_mAttrStrTbl[1] = "新幹線";
			m_mAttrStrTbl[2] = "ＪＲ";
			m_mAttrStrTbl[3] = "第３セクター";
			m_mAttrStrTbl[4] = "私鉄";
			m_mAttrStrTbl[6] = "地下鉄";
			m_mAttrStrTbl[7] = "モノレール";
			m_mAttrStrTbl[8] = "新交通";
			// 都市地図用(属性値*10+地下フラグで検索する)
			//[GS]1.地下鉄以外が1.普通鉄道 5.新交通システム 6.モノレール 7.ケーブルカーに分離されたため
			//50,51,60,61,70,71の要素を追加 2014/3/19 第二技術部第四技術G 加藤
			m_mAttrStrTbl[10] = "普通鉄道（地上）";
			m_mAttrStrTbl[50] = "新交通システム（地上）";
			m_mAttrStrTbl[60] = "モノレール（地上）";
			m_mAttrStrTbl[70] = "ケーブルカー（地上）";
			m_mAttrStrTbl[11] = "普通鉄道（地下）";
			m_mAttrStrTbl[51] = "新交通システム（地下）";
			m_mAttrStrTbl[61] = "モノレール（地下）";
			m_mAttrStrTbl[71] = "ケーブルカー（地下）";
			m_mAttrStrTbl[20] = "地下鉄（地上）";
			m_mAttrStrTbl[21] = "地下鉄（地下）";

		//	e_cpChk_Note_Layer		= NULL;
		}
		~main_rootin	()
		{
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}

		/** ---------------------------------------------------------------------------
		//	引数チェック
		//	@return	成功	true
		//	@return	成功	false
		-----------------------------------------------------------------------------*/
		bool			arg_check	(
							int		argc,			///< 引数個数
							char	**argv			///< 引数列
						);

		/** ---------------------------------------------------------------------------
		//	ツールの使用法の表示
		-----------------------------------------------------------------------------*/
		void			print_usage	(
							char	**argv			///< 引数列
						);

		/** ---------------------------------------------------------------------------
		//	バージョン情報の表示
		//	@return		無し
		-----------------------------------------------------------------------------*/
		void			print_version	( void );


		/** ---------------------------------------------------------------------------
		//	行政コードリストファイル名を返す
		//	@return		ファイル名へのポインタ
		-----------------------------------------------------------------------------*/
		char*			Ret_Adm_Code_File	( void ) {
							return	( e_cpAdminCode_File );
						}

		/** ---------------------------------------------------------------------------
		//	メッシュリストファイル名を返す
		//	@return		ファイル名へのポインタ
		-----------------------------------------------------------------------------*/
		char*			Ret_Mesh_File	( void ) {
							return	( e_cpMeshCode_File );
						}

		/** ---------------------------------------------------------------------------
		//	ログデータ出力ファイル名を返す
		//	@retur		パス名へのポインタ
		-----------------------------------------------------------------------------*/
		char*			Ret_Log_File	( void ) {
							return	( e_cpLog_File );
						}

		/** ---------------------------------------------------------------------------
		//	初期化ファイル名を返す
		//	@return		ファイル名へのポインタ
		-----------------------------------------------------------------------------*/
		char*			Ret_Init_File	( void ) {
							return	( e_cpInit_File );
						}

		/** ---------------------------------------------------------------------------
		//	エラーリストファイル
		//	@return		ファイル名へのポインタ
		-----------------------------------------------------------------------------*/
		char*			Ret_Err_List	( void ) {
							return	( e_cpErrList );
						}

		/** ---------------------------------------------------------------------------
		//	メッシュ矩形をDBから獲得するメソッドの初期化 \
		//	※get_mesh_rect をやる前に必ず呼ぶ
		-----------------------------------------------------------------------------*/
		bool			get_mesh_init	(
							string			c_sMeshLayer,		///< メッシュレイヤ名
							IWorkspacePtr	c_ipWorkSpace		///< 指定するワークスペース
						);

		/** ---------------------------------------------------------------------------
		//	メッシュ矩形をGeoDBから取得する \
		//	※GeoDBへの接続が確立されていることが前提
		-----------------------------------------------------------------------------*/
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< メッシュコード
						);

		/** ---------------------------------------------------------------------------
		//	指定ポイントを中心に、指定したX,Y方向の幅を持った矩形を作成
		-----------------------------------------------------------------------------*/
		IEnvelopePtr	make_rect	(
							IPointPtr&		c_ipPt,				///< 中心ポイント
							double			c_dDegX,			///< X方向
							double			c_dDegY				///< 矩形
						);

		/** ---------------------------------------------------------------------------
		//	ワーニング距離を返す
		//	@return	エラー判定距離
		-----------------------------------------------------------------------------*/
		double			Get_Warn_Dist	( void ) {
							return	( e_Warn_Dist );
						}

		/** ---------------------------------------------------------------------------
		//	エラー距離を返す
		//	@return	エラー判定距離
		-----------------------------------------------------------------------------*/
		double			Get_Err_Dist	( void ) {
							return	( e_Err_Dist );
						}

		/** ---------------------------------------------------------------------------
		//	デバイス座標ポイントを返す
		//	@return	デバイス座標値ポイント
		-----------------------------------------------------------------------------*/
		double			Get_Dev_Dist	( void ) {
							return	( e_dDev_Dist );
						}

		/** ---------------------------------------------------------------------------
		//	エラーログファイルをオープンする
		//	@return	成功	true
		//	@return	失敗	false
		-----------------------------------------------------------------------------*/
		bool			Open_Err_Log	( void );

		/** ---------------------------------------------------------------------------
		//	エラーログのヘッダを出力する
		//	@return	無し
		-----------------------------------------------------------------------------*/
		void			Output_Log_Header	(
							long					c_lDigit	///< メッシュ桁数[IN]
						);

		/** ---------------------------------------------------------------------------
		//	DBへの接続オープン
		//	@return	true	成功
		//	@return	false	失敗
		-----------------------------------------------------------------------------*/
		bool			DB_Open	(							
							IWorkspacePtr&			c_ipWorkspace,							
							IWorkspacePtr&			c_ipWorkspace_Base
						);

		/** ---------------------------------------------------------------------------
		//	チェックの初期化処理
		-----------------------------------------------------------------------------*/
		bool			check_init	( void );

		/** ---------------------------------------------------------------------------
		//	読み込んだデータをデータセットに落とす
		//	@return	データ数
		-----------------------------------------------------------------------------*/
		int				Get_Data_info (
							IFeatureCursorPtr&		c_ipCursor,		///< チェック元カーソル[IN]
							ms_data_info&			c_ipGetData		///< チェック元データのセット[OUT]
						);

		/** ---------------------------------------------------------------------------
		//	注記と鉄道の属性を見てチェック
		//	@return	データ数
		-----------------------------------------------------------------------------*/
		void			Anno_Railway_Chk (
							data_info&				c_dAnnoData,	///< 注記データ[IN]
							disp_pt_info&			c_dCurDat,		///< 表示位置データ[IN]
							ms_data_info&			c_mRailway_Data	///< 検索で得られた鉄道データ[IN]
						);

		/** ---------------------------------------------------------------------------
		//	チェック実行部
		//	※内部でエラーファイルの処理等も行う
		//	@return	成功	true
		//	@return 失敗	false
		-----------------------------------------------------------------------------*/
		bool			check_exe		( void );
};

#endif	//_MAIN_ROOTIN_H_

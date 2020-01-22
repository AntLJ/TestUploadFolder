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
#include <vector>
#include <set>
#include <stdio.h>
#include <string>
#include <libsmpprninfo.h>
#include <smp_prn_info.h>
#include "msi_handle_ws.h"
#include <feature_info.h>
#include "prog_init_cls.h"
#include "exec_time_handle.h"
#include "exp_poly_class.h"
#include "coord_converter.h"
#include "AKAttrHandler.h"

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_WHERE		0x01	///< Where句指定モード
#define		ON_LOGFILE		0x02	///< ログファイル指定
#define		ON_INIT_FILE	0x04	///< 初期化ファイル読み込み用
#define		ON_MESH			0x08	///< 住所コードコードリスト指定モード 2002.9.24
#define		ON_MONO_MESH	0x10	///< 単住所コード指定モード
#define		ON_REVICE_LIST	0x20	///< 建物修正リスト
#define		ON_MATCH_FILE	0x40	///< 出力対応テーブルファイル用
#define		ON_LOWACC_MODE	0x80	///< 既存対応と低精度の切り替え用 2006.06.12

#define		STAT_EQUAL		0x01	///< 等しい
#define		STAT_MODIFY		0x02	///< 形状変更
#define		STAT_RINOBJ		0x04	///< 隣接
#define		STAT_DUPLI		0x08	///< 形状重複
#define		STAT_MIDEQUAL	0x10	///< メッシュと検索用図形IDが一致した

// 2005.12.14 削除・追加オブジェクト関係の対応で追加
#define		STAT_LOWA_EQUAL		0x20	///< 形状一致（低精度）
#define		STAT_LOWA_MODIFY	0x40	///< 形状変更（低精度）
#define		STAT_MESHOUTER		0x1000	///< メッシュ外

/**
* @brief メッセージの出力
*/
void PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... );

struct	data_info
{
	int				e_iOID;				///< オブジェクトID
	int				e_iMeshCode;		///< メッシュコード
	int				e_iStat;			///< 対応の状態（0=対応がない,1=対応が見つかった）
	int				e_iAttr;			///< 背景種別
	IGeometryPtr	e_ipGeo;			///< ジオメトリ
	/** ------------------------------------------------------------------------------
	* @brief コンストラクタ
	------------------------------------------------------------------------------- */
	data_info	( void ) {
		e_iOID			= 0;
		e_iMeshCode		= 0;
		e_iStat			= 0;
		e_iAttr			= 0;
	}
	/** ------------------------------------------------------------------------------
	* @brief デストラクタ
	------------------------------------------------------------------------------- */
	~data_info	( void ) { }

	/**
	* @brief 比較演算子
	* @return src<tgt の場合にtrueを返却
	*/
	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iOID != c_dDat.e_iOID ) {
			if( e_iOID < c_dDat.e_iOID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief 非等値演算子
	* @return src!=tgt の場合にtrueを返却
	*/
	bool	operator!=(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iOID != c_dDat.e_iOID ) {
			ret	= true;
		}
		return	( ret );
	}
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;
typedef	set<data_info, less <data_info> >					s_data_info;
typedef	set<data_info, less <data_info> >::iterator			is_data_info;

typedef	set<string, less<string> >							s_string;
typedef	set<string, less<string> >::iterator				is_string;

/** ------------------------------------------------------------------------------
* @brief  一致したデータを管理するクラス
------------------------------------------------------------------------------- */
struct	match_record
{
	data_info	e_dData1; ///< データ1情報
	data_info	e_dData2; ///< データ1と一致とみなすデータの情報
	
	/**
	* @brief コンストラクタ
	*/
	match_record() {};

	/**
	* @brief デストラクタ
	*/
	~match_record() {};

	/**
	* @brief 比較演算子
	* @return src<tgt の場合にtrueを返却
	*/
	bool	operator<(const match_record c_dDat ) const {
		bool	ret	= false;
		if( e_dData1 != c_dDat.e_dData1 ) {
			if( e_dData1 < c_dDat.e_dData1 ) {
				ret	= true;
			}
		}else if( e_dData2 != c_dDat.e_dData2 ) {
			if( e_dData2 < c_dDat.e_dData2 ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef	multiset<match_record, less <match_record> >			ms_match_record;
typedef	multiset<match_record, less <match_record> >::iterator	ims_match_record;

/**
* @brief 実数値のポイント管理用クラス
*/
struct	Real_Pt
{
	double	e_iX; ///< X座標値
	double	e_iY; ///< Y座標値

	/**
	* @brief コンストラクタ
	*/	
	Real_Pt() {
		e_iX			= 0.0;
		e_iY			= 0.0;
	}
	/**
	* @brief デストラクタ
	*/	
	~Real_Pt() {}

	/**
	* @brief 比較演算子
	* @return src<tgt の場合にtrueを返却
	*/
	bool	operator<(const Real_Pt& c_dDat ) const {
		bool	ret	= false;
		if( e_iX != c_dDat.e_iX ) {
			if( e_iX < c_dDat.e_iX ) {
				ret	= true;
			}
		}else if( e_iY != c_dDat.e_iY ) {
			if( e_iY < c_dDat.e_iY ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief 代入演算子
	*/
	void	operator=(const WKSPoint& c_dDat ) {
		e_iX	= c_dDat.X;
		e_iY	= c_dDat.Y;
	}
};

/** ------------------------------------------------------------------------------
* @brief Real_Ptは既存機能をそのままにしておきたいので、継承してクラスを作成
------------------------------------------------------------------------------- */
struct	Real_Pt2 : public Real_Pt
{

	int		e_iInteriorFlag;	///< インテリアかのフラグ

	/**
	* @brief コンストラクタ
	*/
	Real_Pt2() {
		e_iInteriorFlag	= 0;	
	}
	/**
	* @brief デストラクタ
	*/	
	~Real_Pt2() {}

	/**
	* @brief 比較演算子
	* @return src<tgt の場合にtrueを返却
	*/
	bool	operator<(const Real_Pt2& c_dDat ) const {
		bool	ret	= false;
		if( e_iX != c_dDat.e_iX ) {
			if( e_iX < c_dDat.e_iX ) {
				ret	= true;
			}
		}else if( e_iY != c_dDat.e_iY ) {
			if( e_iY < c_dDat.e_iY ) {
				ret	= true;
			}
		}else if( e_iInteriorFlag != c_dDat.e_iInteriorFlag ) {
			if( e_iInteriorFlag < c_dDat.e_iInteriorFlag ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief 代入演算子
	*/
	void	operator=(const WKSPoint& c_dDat ) {
		e_iX	= c_dDat.X;
		e_iY	= c_dDat.Y;
	}

};

//typedef	map<_Rb_tree_base_iterator::_Base_ptr, ims_data_info, less<_Rb_tree_base_iterator::_Base_ptr> >				m_ims_data_info;
//typedef	map<_Rb_tree_base_iterator::_Base_ptr, ims_data_info, less<_Rb_tree_base_iterator::_Base_ptr> >::iterator	im_ims_data_info;
typedef	map<long, ims_data_info, less<long> >			m_ims_data_info;
typedef	map<long, ims_data_info, less<long> >::iterator	im_ims_data_info;

/**
* @brief ツールのメイン部
*/
class	main_rootin
{
	public:
		char*					e_cpMeshCode_File;		///< メッシュコードリストファイル
		char*					e_cpLog_File;			///< ログデータ出力パス
		char*					e_cpInit_File;			///< 初期化ファイル
		char*					e_cpInit_File_Tgt;		///< 初期化ファイル
		char*					e_cpCheck_Src_Layer;	///< チェック元レイヤ名
		char*					e_cpCheck_Tgt_Layer;	///< チェック先レイヤ名
		char*					e_cpMatch_Tbl_File;		///< 出力対応テーブルファイル
		int						eDigit;					///< メッシュコード桁数
		int						eExe_Flag;              ///< 実行フラグ
		int						e_iMeshCode;			///< メッシュコード
		IFeatureClassPtr		e_ipF_Class;            ///< メッシュフィーチャクラス
		FILE*					e_fErr_Fp;				///<　エラーログファイル用ファイルポインタ
	private:
		prog_init				e_pTool_Init;			///< DBの初期化用設定
		prog_init				e_pTool_Init_Tgt;		///< DB(Tgt)の初期化用設定
		smp_sindy_err_info		e_sPrnInfo;				///< エラー等出力用
		exec_time_handle		e_eExeTime;				///< 実行時間管理
		msi_handle_ws			e_mChk_Src_WS;			///< チェック元
		msi_handle_ws			e_mChk_Tgt_WS;			///< チェック先データ
		IGeometryPtr			e_ipMeshRect;			///< メッシュ矩形
		IGeometryPtr			e_ipMeshRect_Line;		///< メッシュ矩形の境界線
	private:
		ms_data_info			e_msdChk_Src_Datas;		///< チェック元データ
		ms_data_info			e_msdChk_Tgt_Datas;		///< チェック対象データ
	private:
		ms_match_record			e_msMatch_Tbl;			///< 家形一致レコード
		AKAttrHandler           mAKAttrHandler;         ///< AK対応属性取扱いクラス
	private:
		double		m_CompMaxX; ///< マージ判定用最大X
		double		m_CompMaxY; ///< マージ判定用最大Y
		double		m_CompMinX; ///< マージ判定用最小X
		double		m_CompMinY; ///< マージ判定用最小Y
	private:
		FILE*					e_fpMatch_Fp;			///< 対応テーブルファイル
		string					m_sVersion;				///< バージョン名2009.10.23 追加
	public:

		/**
		* @brief コンストラクタ
		*/
		main_rootin	() {
			e_cpMeshCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpInit_File			= NULL;
			e_cpInit_File_Tgt		= NULL;
			e_cpCheck_Src_Layer		= NULL;
			e_cpCheck_Tgt_Layer		= NULL;
			e_cpMatch_Tbl_File		= NULL;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_fErr_Fp				= stderr;	// デフォルトは標準エラー出力へ
			e_msMatch_Tbl.clear();

			m_CompMaxX = 0.0;
			m_CompMaxY = 0.0;
			m_CompMinX = 0.0;
			m_CompMinY = 0.0;

			e_fpMatch_Fp			= NULL;
		}
		/**
		* @デストラクタ
		*/
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
			if( e_fpMatch_Fp != NULL ) {
				fclose ( e_fpMatch_Fp );
			}
		}

		/** ------------------------------------------------------------------------------
		* @brief SiNDYDBへの接続オープン
		* @return	true	成功
		* @return	false	失敗
		------------------------------------------------------------------------------- */
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Base,
							IWorkspacePtr&			c_ipWorkspace_Addr
						);

		/** ------------------------------------------------------------------------------
		* @brief 引数チェック
		* @return	成功	true
		* @return	成功	false
		------------------------------------------------------------------------------- */
		bool			arg_check	(
							int		argc,			///< [in] 引数個数
							char	**argv			///< [in] 引数列
						);

		/** ------------------------------------------------------------------------------
		* @brief ツールの使用法の表示
		------------------------------------------------------------------------------- */
		void			print_usage	(
							char	**argv			///< [in] 引数列
						);

		/** ------------------------------------------------------------------------------
		* @brief バージョン情報の表示
		------------------------------------------------------------------------------- */
		void			print_version	( void );

		/** ------------------------------------------------------------------------------
		* @brief メッシュ矩形をDBから獲得するメソッドの初期化 \
		* @note ※get_mesh_rect をやる前に必ず呼ぶ
		* @return 成功=true 失敗=false
		------------------------------------------------------------------------------- */
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			///< [in] 入力リストクラス
							IWorkspacePtr&	c_ipWorkSpace		///< [in] 指定するワークスペース
						);
		/** ------------------------------------------------------------------------------
		* @brief	メッシュ矩形をGeoDBから取得する \
		* @note ※GeoDBへの接続が確立されていることが前提
		* @return メッシュ矩形のエンベロープ
		------------------------------------------------------------------------------- */
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< [in] メッシュコード
						);
		/** ------------------------------------------------------------------------------
		* @brief エラーログファイルをオープンする
		* @return 成功=true 失敗=false
		------------------------------------------------------------------------------- */
		bool			Open_Err_Log	( void );

		/** ------------------------------------------------------------------------------
		* @brief 出力対応テーブルファイルをオープンする
		* @return 成功=true 失敗=false
		------------------------------------------------------------------------------- */
		bool			Open_Match_Tbl	( void );

		/** ------------------------------------------------------------------------------
		* @brief チェック実行部
		* @note ※内部でエラーファイルの処理等も行う
		* @return 成功	true
		* @return 失敗	false
		------------------------------------------------------------------------------- */
		bool			check_exe	( void );

		/** ------------------------------------------------------------------------------
		* @brief 矩形のポリゴンを
		* @return	IPolygonPtr
		------------------------------------------------------------------------------- */
		IPolygonPtr		Get_Rect_Polygon	(
							IEnvelopePtr&	c_ipEnv			///< [in] メッシュ矩形
						);

		/** ------------------------------------------------------------------------------
		* @brief チェック用行政界リスト、メッシュコードリストの読み込み
		* @return 成功=true 失敗=false
		------------------------------------------------------------------------------- */
		bool			Read_Chk_List	(
							IWorkspacePtr&	c_ipWSpace,			///< [in] ワークスペース
							mk_input_list&	c_aChk_List_Cls		///< [in] リスト管理クラスの参照
						);

		/** ------------------------------------------------------------------------------
		* @brief レイヤオープン
		* @return 成功=true 失敗=false
		------------------------------------------------------------------------------- */
		bool			Open_Layers		(
							IWorkspacePtr&	ipWorkspace_Src,	///< [in] チェック元ワークスペース
							IWorkspacePtr&	ipWorkspace_Tgt 	///< [in] チェック先ワークスペース
						);

		/** ------------------------------------------------------------------------------
		* @brief 差分チェックのループ
		------------------------------------------------------------------------------- */
		void			Diff_Check_Roops (
							IFeatureCursorPtr&	c_ipChk_Src_Cursor,	///< [in] チェック元となるデータのカーソル
							IFeatureCursorPtr&	c_ipChk_Tgt_Cursor 	///< [in] チェック先となるデータのカーソル
						);

		/**
		* @brief 対応テーブルの出力
		*/
		void			Print_Table		(
							ms_data_info&		c_ipChk_Src_Data,	///< [in] チェック元データのセット
							ms_data_info&		c_ipChk_Tgt_Data	///< [in] チェック元データのセット
						);

		/** ------------------------------------------------------------------------------
		* @brief 注記チェック全般をまとめて行う
		------------------------------------------------------------------------------- */
		void			Note_Check	(
							char*				c_cpAddrCode,		///< [in] 住所コード
							IFeatureCursorPtr&	c_ipNoteCursor,		///< [in] 注記フィーチャ
							int&				c_ipCityName_Count,	///< [in] 市区町村名カウント
							int&				c_ipCityOffice_Count///< [in] 市区町村の役所数
						);

		/** ------------------------------------------------------------------------------
		* @brief チェックデータをセットに落とす
		* @return データ数
		------------------------------------------------------------------------------- */
		int				Set_Data_Infos (
							IFeatureCursorPtr&	c_ipChk_Cursor, ///< [in] チェックカーソル
							ms_data_info&		c_ipChk_Data	///< [out] チェックデータのセット
						);

		/** ------------------------------------------------------------------------------
		* @brief ソースとターゲットのチェックを行う
		------------------------------------------------------------------------------- */
		void			Chk_Src_Tgt_Data ( 
							ms_data_info&		c_ipChk_Src_Data,	///< [in] チェック元データのセット
							ms_data_info&		c_ipChk_Tgt_Data, 	///< [in] チェック元データのセット
							msi_handle_ws&		c_mChk_Src_WS,		///< [in] ターゲットのワークスペースハンドラ
							msi_handle_ws&		c_mChk_Tgt_WS,		///< [in] ターゲットのワークスペースハンドラ
							char*				c_cpCheck_Src_Layer	///< [in] チェックレイヤ名
						);

		/** ------------------------------------------------------------------------------
		* @brief 削除・追加と判定されたものに対して精度を若干落とした形で再チェックを行う
		------------------------------------------------------------------------------- */
		void			Chk_Retry ( 
							ms_data_info&		c_ipChk_Src_Data,	///< [in] チェック元データのセット
							ms_data_info&		c_ipChk_Tgt_Data, 	///< [in] チェック元データのセット
							msi_handle_ws&		c_mChk_Src_WS,		///< [in] ターゲットのワークスペースハンドラ
							msi_handle_ws&		c_mChk_Tgt_WS,		///< [in] ターゲットのワークスペースハンドラ
							char*				c_cpCheck_Src_Layer	///< [in] チェックレイヤ名
						);


		/** ------------------------------------------------------------------------------
		* @brief 合致したデータをマッチテーブルに登録する
		------------------------------------------------------------------------------- */
		void			Entry_to_Match_Tbl(
							data_info&	c_dData1, ///< [in] データ1
							data_info&	c_dData2  ///< [in] データ1と一致とみなされたデータ
						);

		/** ------------------------------------------------------------------------------
		* @brief メッシュ境界に存在するデータを取得し、マージした図形を作成する
		* @note	※マージ後のジオメトリとマージした中で最も若いメッシュ番号とPrimIDを代表として返す\
		*       ※メッシュ内部のデータについても内部でメッシュ境界との判定をし、メッシュ境界でなければ\
		*         falseの値を返すものとする
		* @return true	メッシュ境界で隣接するデータがあった場合
		* @return false	隣接データが無い場合
		------------------------------------------------------------------------------- */
		bool			Get_Neibour_Obj ( 
							data_info&			c_ipChk_Src_Data,	///< [in] チェック元データのセット
							bool				c_iMFlag,			///< [in] 一度マージしたデータかどうかのフラグ
							msi_handle_ws&		c_mChk_Src_WS,		///< [in] 自分のワークスペースハンドラ
							char*				c_cpCheck_Src_Layer,///< [in] チェックレイヤ名
							data_info&			c_ipMerged_Data		///< [out] 隣接形状をマージしたもの
						);

		/** ------------------------------------------------------------------------------
		* @brief 低精度オブジェクトの作成（ポリゴン）
		* @return ジオメトリ
		------------------------------------------------------------------------------- */
		IGeometryPtr	Get_LowAccuracy_Obj (
							IGeometryPtr&		c_ipSrcObj,		///< [in] オブジェクト
							int					c_iValue		///< [in] 小数点以下何分の１まで見るか
						);

		/**
		* @brief 拡大した矩形を作成
		* @return 拡大矩形のエンベロープ
		*/
		IEnvelopePtr	Get_Expand_Rect	(
							IEnvelopePtr&		c_iRect,	///< [in] 入力矩形
							int					c_iMode,	///< [in] 作成モード（0=元矩形と左下原点を一緒に作成、1=元矩形を中心に作成)
							double				c_dX,		///< [in] X方向の倍率
							double				c_dY		///< [in] Y方向の倍率
						);

		/** ------------------------------------------------------------------------------
		* @brief ジオメトリから点列の情報を取得する
		------------------------------------------------------------------------------- */
		void			Get_PtsWithOrientation	(
							IGeometryPtr&			c_ipGeo,	///< [in] ジオメトリ
							set<Real_Pt2>&			c_srPts		///< [out] 点列
						);

		/** ------------------------------------------------------------------------------
		* @brief 形状のマッチング情報を取得する
		* @return	STAT_EQUAL	一致
		* @return	STAT_MODIFY	形状変更
		* @return	STAT_LOWA_EQUAL	形状一致（低精度）
		* @return	STAT_LOWA_MODIFY	形状変更（低精度）
		* @return	0	不一致
		--------------------------------------------------------------------------------*/
		int				Get_Match_Info (
							data_info&		c_dSrcDat,			///< [in] チェック元データ
							data_info&		c_dTgtDat,			///< [in] チェック先データ
							ms_data_info&	c_ipChk_Tgt_Data	///< [in] チェック元データのセット
						);

		/** ------------------------------------------------------------------------------
		* @brief 形状一致のチェックを行い、状態を返す
		* @return	STAT_EQUAL	一致
		* @return	STAT_MODIFY	形状変更
		* @return	STAT_LOWA_EQUAL	形状一致（低精度）
		* @return	STAT_LOWA_MODIFY	形状変更（低精度）
		* @return	0	不一致
		------------------------------------------------------------------------------- */
		int				Shape_Chk_and_Ret_Stat ( 
							data_info&		c_dSrcDat,		///< [in] チェック元データ
							data_info&		c_dTgtDat		///< [in] チェック先データ
						);

		/** ------------------------------------------------------------------------------
		* @brief 形状一致のチェック
		* @return	true	一致
		* @return	false	不一致
		------------------------------------------------------------------------------- */
		bool			Shape_Chk	(
							IGeometryPtr&	c_dSrcGeo,		///< [in] チェック元データ
							IGeometryPtr&	c_dTgtGeo		///< [in] チェック先データ
						);

		/** ------------------------------------------------------------------------------
		* @brief 形状一致のチェック（低精度版） 2006.09.25
		* @note  ※オブジェクトの重複部の面積を算出し、重複面積がチェック元データの1/10以下なら\
		*          不一致として判定するようにした。\
		*          中抜き部分は、そのポイントが外側か内側のものかの情報をみて判定するようにした。
		* @return	STAT_LOWA_EQUAL		形状一致（低精度）
		* @return	STAT_LOWA_MODIFY	形状変更（低精度）
		* @return	0	不一致
		------------------------------------------------------------------------------- */
		int				Shape_Chk_LowAccuracy (
							IGeometryPtr&	c_ipSrcGeo,		///< [in] チェック元データ
							IGeometryPtr&	c_ipTgtGeo		///< [in] チェック先データ
						);

#ifdef _DEBUG
		void			Print_Geo	(
							data_info&	c_dSrc,
							data_info&	c_dTgt
						);
#endif
		/**
		* @brief マージの必要性チェック
		* @return 必要=true 不要=false
		*/
		bool			fnIsNeedMergePolygon2(
		                    IGeometryPtr& ipSrcGeom, ///< [in] 元ジオメトリ
		                    IGeometryPtr& ipTgtGeom  ///< [in] ターゲットのジオメトリ
		                );


//------------------------------------------------------<ここまで>

};

#endif	//_MAIN_ROOTIN_H_

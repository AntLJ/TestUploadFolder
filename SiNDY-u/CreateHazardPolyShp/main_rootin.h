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

#include <libsmpprninfo.h>
#include <smp_prn_info.h>
#include <feature_info.h>
#include "msi_handle_ws.h"
#include "prog_init_cls.h"
#include "util_class.h"

#include "exec_time_handle.h"
#include "MSiGet_Parts_Info.h"


#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_LOGFILE		0x01				///< ログファイル指定
#define		ON_MESH			0x02				///< メッシュリスト指定モード
#define		ON_MONO_MESH	0x04				///< 単メッシュコード指定モード
#define		ON_PGDB_MODE	0x08				///< PGDBモード
#define		ON_OUTPATH		0x10				///< データ出力パス
// 2007.08.21
#define		ON_INIT_HZ_AREA	0x20				///< 災害エリア初期化情報
#define		ON_FC_HZ_AREA	0x80				///< 災害エリアフィーチャクラス
#define		ON_INIT_MESH	0x100				///< メッシュ初期化情報
#define		ON_FC_MESH		0x200				///< メッシュフィーチャクラス

#define		CITY_NOTE		0x01
#define		BASE_NOTE		0x02
#define		MID_NOTE		0x04
#define		STR_FIELD_LENG	128

struct	data_info
{
	int			e_iOID;			///< オブジェクトID
	int			e_iAttrCode;	///< 属性分類コード (914@(最重点)=1 915@(重点)=2)
	int			e_iGroupCode;	///< 識別番号
//	int			e_iAnno_Cls;	///< 注記属性
	IGeometryPtr	e_ipGeo;	///< 注記ジオメトリ

	/**
		コンストラクタ
	*/
	data_info	( void ) {
		e_iOID		= 0;
	//	e_dX		= 0.0;
	//	e_dY		= 0.0;
	}
	/**
		デストラクタ
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iAttrCode != c_dDat.e_iAttrCode ) {
			if( e_iAttrCode > c_dDat.e_iAttrCode ) {
				ret	= true;
			}
		}else
		if( e_iGroupCode != c_dDat.e_iGroupCode ) {
			if( e_iGroupCode < c_dDat.e_iGroupCode ) {
				ret	= true;
			}
		}else
		if( e_iOID != c_dDat.e_iOID ) {
			if( e_iOID < c_dDat.e_iOID ) {
				ret	= true;				
			}
		}
		return	( ret );
	}
};

typedef	std::multiset<data_info, std::less <data_info> >			ms_data_info;
typedef	std::multiset<data_info, std::less <data_info> >::iterator	ims_data_info;

struct	Norm_Pt
{
	int	e_iX;
	int	e_iY;
	Norm_Pt() {
		e_iX	= 0;
		e_iY	= 0;
	}
};

class	main_rootin
{
	public:
		string					m_sMeshCode_File;		///< メッシュコードリストファイル 2002.9.24
		string					m_sAddrCode_File;		///< チェック用住所コードファイル
		string					m_sLog_File;			///< ログデータ出力パス
		string					m_sInit_Str_HZ_AREA;	///< 初期化文字列（災害エリア）
		string					m_sInit_Str_MESH;		///< 初期化文字列（メッシュ）
		string					m_sHZ_AREA_FC;			///< 災害エリアレイヤ 2007.08.21
		string					m_sMesh_FC;				///< メッシュレイヤ		
		string					m_sOutPath;				///< データ出力パス名
		string					m_sPGDB_File;			///< PGDBファイル
		int						m_iDigit;				///< メッシュコード桁数
		int						m_iExe_Flag;
		int						m_iMeshCode;			///< メッシュコード
		int						m_iCurMeshCode;			///< 現在実行中のメッシュ
		IFeatureClassPtr		m_ipF_Class;
		FILE*					m_fErr_Fp;				///<　エラーログファイル用ファイルポインタ
	private:
		prog_init				m_pTool_Init_NP;		// DBの初期化用設定
		prog_init				m_pTool_Init_Mesh;		// メッシュ用

		smp_sindy_err_info		m_sPrnInfo;				// エラー等出力用
		mk_input_list			m_sInput_List;			// 入力リスト用
		exec_time_handle		m_eExeTime;				// 実行時間管理
		msi_handle_ws			m_mHZ_AREA_WS;			// 災害エリアデータ

		crd_cnv					m_cCrd;
		util_class				m_cUC;
	private:
		ms_data_info			m_mAll_Data;

		IWorkspacePtr			m_ipWorkspace_HZ_AREA;
		IWorkspacePtr			m_ipWorkspace_Mesh;

		string					m_sVersion;				//< 2009.09.30

	public:

		main_rootin	() {
			m_iDigit				= 0;
			m_iMeshCode				= 0;
			m_iCurMeshCode			= 0;
			m_fErr_Fp				= stderr;	// デフォルトは標準エラー出力へ
		}
		~main_rootin	() {
			if( m_fErr_Fp != NULL ) {
				fclose ( m_fErr_Fp );
			}
		}
		/**
			SiNDYDBへの接続オープン
			@return	true	成功
			@return	false	失敗
		*/
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Note
						);
		/**
			DBへの接続オープン
			@return	true	成功
			@return	false	失敗
		*/
		bool			DB_Open	(
							IWorkspacePtr&			c_ipWorkspace,
							char*					c_cpUserName,		///< ユーザ名
							char*					c_cpPassword,		///< パスワード
							char*					c_cpVersion,		///< バージョン
							char*					c_cpInstance,		///< インスタンス
							char*					c_cpServer			///< サーバ名
						);

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
			メッシュ矩形をDBから獲得するメソッドの初期化 \
			※get_mesh_rect をやる前に必ず呼ぶ
		*/
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			// 入力リストクラス
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
			エラーログファイルをオープンする
		*/
		bool			Open_Err_Log	( void );

		/**
			変換実行部
			※内部でエラーファイルの処理等も行う
			@return	成功	true
			@return 失敗	false
		*/
		bool			conv_exe	( void );

		/**
			矩形のポリゴンを
			@return	IPolygonPtr
		*/
		IPolygonPtr		Get_Rect_Polygon	(
							IEnvelopePtr	c_ipEnv			///< メッシュ矩形
						);

		/**
			チェック用行政界リスト、メッシュコードリストの読み込み
		*/
		bool			Read_Mesh_List	( void );

		/** ------------------------------------------------------------------------------
			レイヤオープン
			@return	成功	true
			@return 失敗	false
		--------------------------------------------------------------------------------*/
		bool			Open_Layers		( void );

		/** ------------------------------------------------------------------------------
			メッシュ単位ファイル名の作成
		--------------------------------------------------------------------------------*/
		std::string		make_mesh_fname (
							int		c_iMeshCode,	///< メッシュコード
							int		e_iSubPathNum,	///< サブパスの階層
							char*	c_cpSrcShpPath,	///< ソースシェイプパス
							char*	c_cpAddStr,		///< メッシュへの追加文字列
							string&	c_sMkSubPath	///< 生成されたサブディレクトリ
						);

		/** ------------------------------------------------------------------------------
			ファイルのフルパス名から、シェイプファイル名を抽出する関数
			Function to extract the shape file name from the fullpath of the file
			@return	なし
		--------------------------------------------------------------------------------*/
		bool			GetFeatureClass	(
							const std::string&	sourceFile,		///< ソースファイル名
							std::string&		className		///< クラス名
						);

		/** ------------------------------------------------------------------------------
			フルパス名から、ファイル名を除いて、ディレクトリ名を返す関数
			// Function to remove the file name from the fullpath and return the directory
			@return	なし
		--------------------------------------------------------------------------------*/
		bool			GetFilePath	(
							const std::string&	sourceFile,		///< ソースファイル名
							std::string&		filePath		///< パス名
						);

		/** ------------------------------------------------------------------------------
			データを読み込み、Shapeファイルに変換
			@return	成功	true
			@return 失敗	false
		--------------------------------------------------------------------------------*/
		bool			Read_HZ_AREA_to_Shape	( void );

		/** ------------------------------------------------------------------------------
			形状クリップ
		--------------------------------------------------------------------------------*/
		IGeometryPtr	Mk_Clip_Data (
							IGeometryPtr&		c_ipClipPoly,	///< クリップ用ポリゴン
							IGeometryPtr&		c_ipTgtGeo, 	///< クリップされる図形
							int					c_iClipType,	///< クリップ条件 0=Intersect 1=Difference
							int*				c_ipEmpty_F		///< 空フラグ
						);
		
		/** ------------------------------------------------------------------------------
			災害エリアデータをセットに落とす
			@return	データ数
		--------------------------------------------------------------------------------*/
		int				Set_HZ_AREA_Data (
							IGeometryPtr&		c_ipMeshRect,		///< メッシュ矩形
							IFeatureCursorPtr&	c_ipNoteCursor, 	///< 注記カーソル
							ms_data_info&		c_mHZ_Area_Data,	///< 災害ポリゴンデータ
							int					c_iData_Type		///< データタイプ
						);

		/** ------------------------------------------------------------------------------
			災害エリアデータをシェープファイルに変換
			@return	bool
		--------------------------------------------------------------------------------*/
		bool			HZ_Area_to_Shape (
							ISpatialReferencePtr&	c_ipSpRef,			///< 空間参照情報
							ms_data_info&			c_mHZ_Area_Data,	///< 災害エリアデータ
							char*					c_cpOutFName		///< 出力シェイプファイル名
						);

		/** ------------------------------------------------------------------------------
			実行情報を出力する 2006.05.17
		--------------------------------------------------------------------------------*/
		void			Print_Check_Info	( void );

		//------------------------------------------------------
		// 2009.09.30
		// エラーメッセージ
		//------------------------------------------------------
		void			PrnMsg	( FILE* c_cpFp,	const char* c_cpFormat, ... ) {
							
							va_list	a_vArgList = NULL;

							va_start(a_vArgList, c_cpFormat );	// 初期化

							vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
							fflush	( c_cpFp );
						}
};
//------------------------------------------------------<ここまで>



#endif	//_MAIN_ROOTIN_H_

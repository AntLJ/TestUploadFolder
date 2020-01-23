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
#include <crd_cnv.h>
#include <smp_prn_info.h>
#include "msi_handle_ws.h"
#include <feature_info.h>
#include "prog_init_cls.h"
#include "util_class.h"

//include "WKSHelper.h"
//#include "SiINClassTable.h"
//#include "addr_db_handle.h"
//#include "poi_text_handle.h"
#include "exec_time_handle.h"
//#include "stringcomp.h"
//#include "addr_str_work.h"
#include "MSiGet_Parts_Info.h"

//using	namespace	stlport;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_LOGFILE		0x01				///< ログファイル指定
#define		ON_MESH			0x02				///< メッシュリスト指定モード
#define		ON_MONO_MESH	0x04				///< 単メッシュコード指定モード
#define		ON_PGDB_MODE	0x08				///< PGDBモード
#define		ON_OUTPATH		0x10				///< データ出力パス
// 2007.08.21
#define		ON_INIT_NP_AREA	0x20				///< 駐車違反取締エリア初期化情報
#define		ON_FC_NP_AREA	0x80				///< 駐車違反取締エリアフィーチャクラス
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
//		if( e_iOID != c_dDat.e_iOID ) {
//			if( e_iOID < c_dDat.e_iOID ) {
//				ret	= true;
//			}
//		}
/*		if( e_dX != c_dDat.e_dX ) {
			if( e_dX < c_dDat.e_dX ) {
				ret	= true;
			}
		}else
		if( e_dY != c_dDat.e_dY ) {
			if( e_dY < c_dDat.e_dY ) {
				ret	= true;
			}
		}*/
	/*	if( e_sNoteStr1 != c_dDat.e_sNoteStr1 ) {
			if( e_sNoteStr1 < c_dDat.e_sNoteStr1 ) {
				ret	= true;
			}
		}
	*/
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
		char*					e_cpMeshCode_File;		///< メッシュコードリストファイル 2002.9.24
		char*					e_cpAddrCode_File;		///< チェック用住所コードファイル
		char*					e_cpGentyou_File;		///< 現調交差点リストファイル
		char*					e_cpLog_File;			///< ログデータ出力パス
		char*					e_cpInit_Str_NP_AREA;	///< 初期化文字列（駐車違反取締エリア）
		char*					e_cpInit_Str_MESH;		///< 初期化文字列（メッシュ）
		char*					e_cpNP_AREA_FC;			///< 駐車違反取締エリアレイヤ 2007.08.21
		char*					e_cpMesh_FC;			///< メッシュレイヤ		
		char*					e_cpOutPath;			///< データ出力パス名
		char*					e_cpPGDB_File;			///< PGDBファイル
		std::string				e_sWhereString;			///< 検索に使用するWhere句
		int						eMake_Type;				///< データ作成タイプ
		int						eExe_Type;				///< 実行タイプ
		int						eDigit;					///< メッシュコード桁数
		char*					e_cpAddrCode;			///< 住所コード
		int						eExe_Flag;
		int						e_iMeshCode;			///< メッシュコード
		int						e_iCurMeshCode;			///< 現在実行中のメッシュ
//		IWorkspacePtr			e_ipWorkSpace;
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<　エラーログファイル用ファイルポインタ
		std::set<int, std::less<int> >	e_sExi_Mesh_List;		///<　存在メッシュのリスト 2002.12.06
		std::set<int>					e_sOID_Set;				///<　オブジェクトIDのセット
	private:

//		SiINClassTable			e_sAttrTable;
		smp_sindy_err_info		e_sPrnInfo;				// エラー等出力用
		mk_input_list			e_sInput_List;			// 入力リスト用
		exec_time_handle		e_eExeTime;				// 実行時間管理
		msi_handle_ws			e_mNP_AREA_WS;			// 駐車違反取締エリアデータ

		crd_cnv					e_cCrd;
		util_class				e_cUC;
	private:
		ms_data_info			e_mNote_Data;			///< 駐車違反取締エリアデータ
		ms_data_info			e_mAll_Data;
		int						e_iFontChkFlag;			///< フォントチェックするかどうか（ミドルのみ）

		IWorkspacePtr			e_ipWorkspace_NP_AREA;
		IWorkspacePtr			e_ipWorkspace_Mesh;

		string					m_sVersion;				//< 2009.09.30

	public:

		main_rootin	() {
			e_cpMeshCode_File		= NULL;
			e_cpAddrCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpGentyou_File		= NULL;
			e_cpInit_Str_NP_AREA	= NULL;
			e_cpInit_Str_MESH		= NULL;
			e_cpNP_AREA_FC			= NULL;
			e_cpMesh_FC				= NULL;
			e_cpOutPath				= NULL;
			e_cpPGDB_File			= NULL;
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_iCurMeshCode			= 0;
			e_fErr_Fp				= stderr;	// デフォルトは標準エラー出力へ
			e_sExi_Mesh_List.clear();
			e_iFontChkFlag			= 0;
		}
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}

		/**
			DBへの接続オープン
			@return	ワークスペースのポインタ
		*/
		IWorkspacePtr	DB_Open	(
							char*	c_cpTargetDB	///< 対象DB名
						);

		/**
			文字列チェック
			@return	文字列
		*/
		std::string		Str_to_SJIS	(
							CComBSTR*	c_CStr		///< 文字列(Unicode)
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
			メッシュレイヤオープン
		*/
		bool			Open_Mesh_Layers	( void );

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
			エラーログを送るファイルポインタを返す
		*/
		FILE*			Log_Fp			( void ) {
							return	( e_fErr_Fp );
						}
		/**
			変換実行部
			※内部でエラーファイルの処理等も行う
			@return	成功	true
			@return 失敗	false
		*/
		bool			conv_exe	( void );

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
		bool			Read_NP_AREA_to_Shape	( void );

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
			駐車違反取締エリアデータをセットに落とす
			@return	成功	true
			@return 失敗	false
		--------------------------------------------------------------------------------*/
		bool			Set_NP_AREA_Data (
							IGeometryPtr&		c_ipMeshRect,		///< メッシュ矩形
							IFeatureCursorPtr&	c_ipNoteCursor, 	///< 注記カーソル
							ms_data_info&		c_mNP_Area_Data,	///< 駐車違反ポリゴンデータ
							int					c_iData_Type		///< データタイプ
						);

		/** ------------------------------------------------------------------------------
			駐車違反取締エリアデータをシェープファイルに変換
			@return	bool
		--------------------------------------------------------------------------------*/
		bool			NP_Area_to_Shape (
							ISpatialReferencePtr&	c_ipSpRef,			///< 空間参照情報
							ms_data_info&			c_mNP_Area_Data,	///< 駐車違反取締エリアデータ
							char*					c_cpOutFName		///< 出力シェイプファイル名
						);

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

	private:

};
//------------------------------------------------------<ここまで>



#endif	//_MAIN_ROOTIN_H_

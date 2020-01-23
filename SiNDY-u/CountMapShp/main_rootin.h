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
#include "feature_info.h"

#include "util_class.h"
#include "chk_attr_value_tbl.h"
#include "note_relchk.h"
#include "prog_init_cls.h"
#include "SiINClassTable.h"
#include <coord_converter.h>
#include <arctl/coinitializer.h>

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		FLENMAX		32

#define		ON_OUTFILE		0x01
#define		ON_LOGFILE		0x02
#define		ON_MESHLIST		0x04
#define		ON_MONO_MESH	0x08
#define		ON_SUBDIR		0x10
#define		ON_ADDSTR		0x20
#define		ON_NOTEDISP		0x40	// 注記の関係チェック用
#define		ON_MARKDISP		0x80	// 注記の関係チェック用
#define		ON_NOTECOUNT	0x1000	// 注記カウントを示す
#define		ON_MESHCOUNT	0x2000	// メッシュ別カウント
#define		ON_BGCLASS_TBL		0x4000
#define		ON_ANNOCLASS_TBL	0x8000
#define		ON_INPUT_PATH	0x10000
#define		ON_ALLATTR		0x20000	// 2010.08.19 追加
#define		ON_DATATYPEOFF	0x40000

#define	IS_SCCLS	1	///< 	スケール種別
#define	IS_SCFLG	2	///<	スケールフラグ

/**
	shapeのオブジェクトの情報
*/
struct	data_info
{
	int			e_iMeshCode;	// メッシュコード
//	unsigned	e_uDataType;	// データタイプ 0x30, 0x50
//	unsigned	e_uLayerID;		// レイヤID
	string		e_sFIELD;		// フィールド名
	int			e_iSindyAttr;	// SiNDY上の属性
	int			e_iUnderF;		// 地下フラグ
	unsigned	e_uAttrCode;	// 種別コード
	int			e_iSCount[4];	// スケールカウント
	int			e_iSCount_M[4];	// スケールカウント（記号）
	int			e_iTrueCount;	// 真位置カウント 2010.08.17
	double		e_dLength;		// 長さ

	/**
		コンストラクタ
	*/
	data_info	( void ) {
		e_iMeshCode	= 0;
//		e_uDataType	= 0;
//		e_uLayerID	= 0;
		e_sFIELD	= "";
		e_iSindyAttr= 0;
		e_iUnderF	= 0;
		e_uAttrCode	= 0;
		for(int i=0;i<4;i++)
		{
			e_iSCount[i]	= 0;
			e_iSCount_M[i]	= 0;
		}
		e_iTrueCount= 0;
		e_dLength	= 0.0;
	}
	/**
		デストラクタ
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ) {
			if( e_iMeshCode < c_dDat.e_iMeshCode ) {
				ret	= true;
			}
		}else
//		if( e_uDataType != c_dDat.e_uDataType ) {
//			if( e_uDataType < c_dDat.e_uDataType ) {
//				ret	= true;
//			}
//		}else
//		if( e_uLayerID != c_dDat.e_uLayerID ) {
//			if( e_uLayerID < c_dDat.e_uLayerID ) {
//				ret	= true;
//			}
//		}else
		if( e_iSindyAttr != c_dDat.e_iSindyAttr ) {
			if( e_iSindyAttr < c_dDat.e_iSindyAttr ) {
				ret	= true;
			}
		}else
		if( e_iUnderF != c_dDat.e_iUnderF ) {
			if( e_iUnderF < c_dDat.e_iUnderF ) {
				ret	= true;
			}
		}else
		if( e_uAttrCode != c_dDat.e_uAttrCode ) {
			if( e_uAttrCode < c_dDat.e_uAttrCode ) {
				ret	= true;
			}
		}

		return	( ret );
	}
};

typedef	multiset<data_info, less <data_info> >				ms_data_info;
typedef	multiset<data_info, less <data_info> >::iterator	ims_data_info;

class	main_rootin
{
	public:
		string		m_sSrcShpFile;			///< 参照元シェイプファイル
		string		m_sSrcShpPath;			///< 参照元シェイプファイルまでのパス
		string		m_sSrcFClass;			///< フィーチャクラス名（シェイプファイル名）
		string		m_sOutInfoFile;			///< 出力情報ファイル
		string		m_sOutLogFile;			///< 出力ログファイル
		int			m_iExe_Flag;
		FILE*		m_fpOutInfoFp;			///< 出力情報用
		FILE*		m_fpOutLogFp;			///< 出力ログファイル
	//	string		m_sInit_File;			///< 初期化ファイル 2007.01.09

	//	prog_init	m_pTool_Init;			// DBの初期化用設定 2007.01.09
		SiINClassTable	m_sAttrTable;		// 属性テーブル 2007.01.09
		int			m_iInput_Lv;			// データのレベル 2007.01.10
		ms_data_info	m_mData_Info;		// 背景・注記・行政界 2007.01.10

		int			m_iSubPathNum;			//　サブ階層の数
		string		m_sMeshList;
		string		m_sAddStr;
		set<int>	m_sMesh;
		long		m_lMeshCode;			///< 単メッシュ指定時のメッシュコード

		string		m_DB_Init;              //!< DB接続

		int			m_iDigit;

		// 注記チェック用
		string		m_sNoteDispFile;		///< 注記表示情報ファイル
		string		m_sMarkDispFile;		///< 記号表示情報ファイル
		string		m_sDispRuleFile;		///< 表示ルールを記述したファイル（DBからエクスポートして作成。あとでDB直にも対応させたい）
		string		m_sAttrTableName;		///< 属性テーブル名

		string		m_sVersion;

		util_class	m_uUC;
		crd_cnv		m_cCrd;

	//	chk_attr_value_tbl		m_cAttValTbl;		///< 属性値テーブルクラス
		string					m_sAttValTblFile;	///< 属性値テーブルファイル
		esriGeometryType		m_eGeometryType;	///< ジオメトリタイプ

		enum {
			BGP	= 0,
			BGL,
			BLD,
			RAL,
			USP,
			STA,
			CNT,
			ADM,
			DCL
		};

	public:
		main_rootin	()
		{
			m_iExe_Flag		= 0;
			m_fpOutInfoFp	= stdout;
			m_fpOutLogFp	= stderr;
			m_iInput_Lv		= 0;
			m_iSubPathNum	= 0;
			m_lMeshCode		= 0;
			m_iDigit		= 0;
			m_eGeometryType	= esriGeometryPoint;
		}
		~main_rootin ()
		{
		/*	if( e_cpSrcShpPath != NULL ) {
				delete [] e_cpSrcShpPath;
				e_cpSrcShpPath	= NULL;
			}
			if( e_cpSrcFClass != NULL ) {
				delete [] e_cpSrcFClass;
				e_cpSrcFClass	= NULL;
			}*/
			if (m_fpOutInfoFp != NULL) {
				fflush(m_fpOutInfoFp);
				if (m_fpOutInfoFp != stdout)
					fclose(m_fpOutInfoFp);
			}
			m_fpOutInfoFp = stdout;

			if (m_fpOutLogFp != NULL) {
				fflush(m_fpOutLogFp);
				if (m_fpOutLogFp != stderr)
					fclose(m_fpOutLogFp);
			}
			m_fpOutLogFp = stderr;
		}

		/**
			引数チェック
			@return	成功	true
			@return	成功	false
		*/
		bool	arg_check	(
			int		argc,			///< 引数個数
			char	**argv			///< 引数列
		);

		/**
			ツールの使用法の表示
		*/
		void	print_usage	(
			char	**argv			///< 引数列
		);

		/**
			バージョン情報の表示
		*/
		void	print_version ( void );

		/**
			ファイルのフルパス名から、シェイプファイル名を抽出する関数
			Function to extract the shape file name from the fullpath of the file
			@return	なし
		*/
		bool	GetFeatureClass	(
			const char*	sourceFile,		///< ソースファイル名（パス名含む）
			string&		className		///< フィーチャクラス名
		);

		/**
			フルパス名から、ファイル名を除いて、ディレクトリ名を返す関数
			// Function to remove the file name from the fullpath and return the directory
			@return	なし
		*/
		bool	GetFilePath	(
			const char*		sourceFile,		///< ソースファイル名
			string&			filePath		///< パス名
		);

		/**
			実実行部
		*/
		bool 	exec_command(
			IWorkspacePtr	c_ipWSpace		///< ワークスペースvoid
		);

		/**
			注記以外のデータのカウントを行う
		*/
		bool	count_info (
			int		c_iMeshCode,
			char*	c_cpSrcShpPath,
			char*	c_cpSrcFClass
		);

		/**
			注記の属性カウントを行う
		*/
		bool	count_note_info (
			int						c_iMeshCode,
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			v_Feature_All_Attrs&	c_vpFNDF_Data,
			v_Feature_All_Attrs&	c_vpFMDF_Data
		);

		/**
			情報を読み込む
		*/
		bool	read_info (
			int						c_iMeshCode,
			char*					c_cpSrcShpPath,
			char*					c_cpSrcFClass,
			v_Feature_All_Attrs*	c_vpFAttr_List
		);

		/**
			データをチェック用の構造にする
			@return	bool
		*/
		bool	mk_data_for_chk	(
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			mm_note_layer*			c_mpNote_Data
		);
		/**
			データをチェック用の構造にする
			@return	bool
		*/
		bool	mk_data_for_chk (
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			mm_note_disp_info*		c_mpNote_Disp_Data
		);
		/**
			データをチェック用の構造にする
			@return	bool
		*/
		bool	mk_data_for_chk (
			v_Feature_All_Attrs&	c_vpFSrc_Data,
			mm_mark_disp_info*		c_mpMark_Disp_Data
		);

		/**
			スケール種別の判定
			@return	0	スケール種別でない
			@return	1	スケール種別
			@return	2	スケールフラグ
		*/
		int		Scale_Attr_Chk	(
			string&		a_sFieldName,	///< フィールド名
			int*		c_ipScale		///< 該当スケール
		);

		/**
			表示ルールの妥当性を確認
		*/
		bool	Chk_Disp_Rule	(
			int		c_iDispState,	///< 表示状態
			int		c_iRuleType 	///< 表示ルールの値
		);

		/**
			注記表示のスケール間整合チェック
		*/
		bool	Chk_Scale_Disp_Rule (
			int				c_iDigit,			///< 該当レベルを表すメッシュ桁数
			int*			c_ipNote_State,		///< 注記の表示状態
			note_disp_rule&	c_nFoundRule		///< 注記のルール
		);

		/**
			メッシュ単位ファイル名の作成
		*/
		string	make_mesh_fname (
			int		c_iMeshCode,	///< メッシュコード
			char*	c_cpSrcShpPath,	///< ソースシェイプパス
			char*	c_cpAddStr		///< メッシュへの追加文字列
		);

		/** 2007.01.09
			DBへの接続オープン
			@return	true	成功
			@return	false	失敗
		*/
		bool	DB_Open	(
			IWorkspaceFactoryPtr&	c_ipWorkspaceFactory,
			IPropertySetPtr&		c_ipPropertySet,
			IWorkspacePtr&			c_ipWorkspace
		);

		/** 2007.01.10
			情報の追加
			@return	無
		*/
		void	Put_Info	(
			data_info		c_dInfo		///< 書き込みデータ[IN]
		);

		/** 2007.01.10
			情報の出力
			@return	無し
		*/
		void	Print_Info	(
			ms_data_info*	c_mpmspCur_DSets,	///< 現在着目中のデータ
			int				c_iData_Level,		///< データレベル
			FILE*			c_fpOutFp			///< 出力ファイルポインタ
		);

		/** 2007.01.10
			SiNDY上の属性フィールド名を取得
		*/
		bool	GetDBAttrFld(
			string*			c_sSiNDYFld,	///< SiNDY上のフィールド名[OUT]
			int*			c_ipDataType,	///< データタイプ[OUT]
			int*			c_ipShpType		///< シェイプデータのタイプ[OUT]
		);
};

#endif

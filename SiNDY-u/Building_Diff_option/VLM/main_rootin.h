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
#include "msi_handle_ws.h"
#include <feature_info.h>
#include "prog_init_cls.h"
#include "exec_time_handle.h"
#include "exp_poly_class.h"
#include "coord_converter.h"
#include <atlstr.h>
#include "..\..\Building_Diff\AKAttrHandler.h" // [Bug 10159] 対応

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_LOGFILE		0x02	///< ログファイル指定
#define		ON_INIT_FILE	0x04	///< 初期化ファイル読み込み用
#define		ON_MESH			0x08	///< 住所コードコードリスト指定モード 2002.9.24
#define		ON_MONO_MESH	0x10	///< 単住所コード指定モード
#define		ON_ATTR_FILE	0x40	///< 属性テーブルファイル用

/**
* @brief メッセージの出力
*/
void PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... );

struct	data_info
{
	int				e_iOID;				///< オブジェクトID
	int				e_iMeshCode;		///< メッシュコード
	int				e_iPrim_ID;			///< プリミィティブID
	int				e_iStat;			///< 対応の状態（0=対応がない,1=対応が見つかった）
	int				e_iAttr;			///< 背景種別
//	char			e_cRP_F;			///< 渡り廊下フラグ（a roofed passage）
	IGeometryPtr	e_ipGeo;			///< ジオメトリ
	/**
	* @brief コンストラクタ
	*/
	data_info	( void ) {
		e_iOID			= 0;
		e_iMeshCode		= 0;
		e_iPrim_ID		= 0;
		e_iStat			= 0;
		e_iAttr			= 0;
//		e_cRP_F			= 0;
	}
	/**
	* @brief デストラクタ
	*/
	~data_info	( void ) { }

	/**
	* @brief 比較演算子
	* @return src<tgtの場合true それ以外false
	*/
	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ) {
			if( e_iMeshCode < c_dDat.e_iMeshCode ) {
				ret	= true;
			}
		}else
		if( e_iPrim_ID != c_dDat.e_iPrim_ID ) {
			if( e_iPrim_ID < c_dDat.e_iPrim_ID ) {
				ret	= true;
			}
		}
		return	( ret );
	}
	/**
	* @brief 非等値演算子
	* @return src!=tgtの場合true それ以外false
	*/
	bool	operator!=(const data_info c_dDat ) const {
		bool	ret	= false;
		if( e_iMeshCode != c_dDat.e_iMeshCode ||
			e_iPrim_ID != c_dDat.e_iPrim_ID )
		{
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

class	main_rootin
{
	public:
		string					e_cpMeshCode_File;		///< メッシュコードリストファイル
		string					e_cpLog_File;			///< ログデータ出力パス
		string					e_cpInit_File;			///< 初期化ファイル
		string					e_cpCheck_Src_Layer;	///< チェック元レイヤ名
		string					e_cpAttr_Tbl_File;		///< 属性対応テーブルファイル
		int						eDigit;					///< メッシュコード桁数
		int						eExe_Flag;              ///< 実行フラグ
		int						e_iMeshCode;			///< メッシュコード
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<　エラーログファイル用ファイルポインタ
		set<int, less<int> >	e_sExi_Mesh_List;		///<　存在メッシュのリスト 2002.12.06
		set<int>				e_sOID_Set;				///<　オブジェクトIDのセット
	private:
		prog_init				e_pTool_Init;			///< DBの初期化用設定
		exec_time_handle		e_eExeTime;				///< 実行時間管理
		msi_handle_ws			e_mChk_Src_WS;			///< チェック元
		IGeometryPtr			e_ipMeshRect;			///< メッシュ矩形
	private:
		AKAttrHandler           mAKAttrHandler;         ///< AK対応属性取扱いクラス [Bug 10159]対応
	private:
		FILE*					e_fpAttr_Fp;			///< 属性対応テーブルファイル
		CString                 m_sConnectStr;          ///< DB接続文字列
	public:
		/**
		* @brief コンストラクタ
		*/
		main_rootin	() {
			eDigit            = 0;
			eExe_Flag         = 0;
			e_iMeshCode       = 0;
			e_fErr_Fp         = stderr;	// デフォルトは標準エラー出力へ
			e_sExi_Mesh_List.clear();
			e_fpAttr_Fp       = NULL;
		}
		/**
		* @brief デストラクタ
		*/
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
			if( e_fpAttr_Fp != NULL ) {
				fclose ( e_fpAttr_Fp );
			}
		}
		/**
		* @brief SiNDYDBへの接続オープン
		* @return	true	成功
		* @return	false	失敗
		*/
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Base
						);

		/**
		* @brief 引数チェック
		* @return	成功	true
		* @return	成功	false
		*/
		bool			arg_check	(
							int		argc,			///< [in] 引数個数
							char	**argv			///< [in] 引数列
						);

		/**
		* @brief ツールの使用法の表示
		*/
		void			print_usage	(
							char	**argv			///< [in] 引数列
						);

		/**
		* @brief バージョン情報の表示
		* @return		無し
		*/
		void			print_version	( void );

		/**
		* @brief メッシュ矩形をDBから獲得するメソッドの初期化 \
		* @note  ※get_mesh_rect をやる前に必ず呼ぶ
		*/
		bool			get_mesh_init	(
							mk_input_list&	c_mMList,			///< [in] 入力リストクラス
							IWorkspacePtr&	c_ipWorkSpace		///< [in] 指定するワークスペース
						);
		/**
		* @brief メッシュ矩形をGeoDBから取得する \
		* @note  ※GeoDBへの接続が確立されていることが前提
		*/
		IEnvelopePtr	get_mesh_rect	(
							int				c_iMeshCode			///< [in] メッシュコード
						);
		/**
		* @brief エラーログファイルをオープンする
		* @return 成功=true, 失敗=false
		*/
		bool			Open_Err_Log	( void );

		/**
		* @brief 属性テーブルファイルをオープンする
		* @return 成功=true, 失敗=false
		*/
		bool			Open_Attr_Tbl	( void );

		/**
		* @brief チェック実行部
		* @note  ※内部でエラーファイルの処理等も行う
		* @return 成功 true
		* @return 失敗 false
		*/
		bool			check_exe	( void );

		/**
		* @brief チェック用行政界リスト、メッシュコードリストの読み込み
		*/
		bool			Read_Chk_List	(
							IWorkspacePtr&	c_ipWSpace,			///< [in] ワークスペース
							mk_input_list&	c_aChk_List_Cls		///< [in] リスト管理クラスの参照
						);

		/**
			レイヤオープン
		* @return 成功=true, 失敗=false
		*/
		bool			Open_Layers		(
							IWorkspacePtr&	ipWorkspace_Src		///< [in] チェック元ワークスペース
						);

		/**
		* @brief 処理ループ
		*/
		void			Exec_Roops (
							IFeatureCursorPtr&	c_ipChk_Src_Cursor	///< [in] チェック元となるデータのカーソル
						);

		/**
		* @brief 対応テーブルの出力
		*/
		void			Print_Table		(
							ms_data_info&		c_ipChk_Src_Data ///< [in] チェック元データのセット
						);

		/**
		* @brief チェックデータをセットに落とす
		* @return	データ数
		*/
		int				Set_Data_Info (
							IFeatureCursorPtr&	c_ipChk_Cursor, ///< [in] チェックカーソル
							ms_data_info&		c_ipChk_Data	///< [out] チェックデータのセット
						);

//------------------------------------------------------<ここまで>

};

#endif	//_MAIN_ROOTIN_H_

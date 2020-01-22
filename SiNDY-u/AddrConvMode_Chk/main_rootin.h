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
#include <libsmpprninfo.h>
#include "prog_init_cls.h"
//include "WKSHelper.h"
//#include "SiINClassTable.h"
#include "exec_time_handle.h"
#include <arctl/coinitializer.h>
#include "sindy/workspace/name.h"

using	namespace	std;

#ifndef	_MAIN_ROOTIN_H_
#define	_MAIN_ROOTIN_H_

#define		ON_ADCODE			0x01				///< 住所コードコードリスト指定モード 2002.9.24
#define		ON_MONO_ADCODE		0x02				///< 単住所コード指定モード
#define		ON_WHERE			0x04				///< Where句指定モード
#define		ON_LOGFILE			0x08				///< ログファイル指定
#define		ON_INIT_FILE		0x10				///< 初期化ファイル読み込み用
#define		ON_MESH				0x20				///< 住所コードコードリスト指定モード 2002.9.24
#define		ON_MONO_MESH		0x40				///< 単住所コード指定モード
#define		ON_CODE_LIST_TXT	0x80				///< コードリストテキストファイル
#define		ON_PGDB_MODE		0x100
#define		ON_CITY_ADMIN		0x200
#define		ON_GOU_POINT		0x400

struct	data_info
{
	int			e_iOID;				///< オブジェクトID
//	char		e_cpCityCode[6];	///< 5桁コード
//	char		e_cpAddrCode[7];	///< 6桁コード
	string		e_sCityCode;		///< 5桁コード
	string		e_sAddrCode;		///< 6桁コード
	string		e_sAddrCode2;		///< 6桁コード
//	int			e_iConvMode;		///< 変換モード
	/**
		コンストラクタ
	*/
	data_info	( void ) {
		e_iOID		= 0;
	}
	/**
		デストラクタ
	*/
	~data_info	( void ) { }

	bool	operator<(const data_info c_dDat ) const {
		bool	ret	= false;
/*		if( strcmp( e_cpCityCode, c_dDat.e_cpCityCode ) != 0 ) {
			if( strcmp ( e_cpCityCode, c_dDat.e_cpCityCode ) < 0 ) {
				ret	= true;
			}
		}else
		if( 
*/		if( e_sCityCode != c_dDat.e_sCityCode ) {
			if( e_sCityCode < c_dDat.e_sCityCode ) {
				ret	= true;
			}
		}else
		if( e_sAddrCode != c_dDat.e_sAddrCode ) {
			if( e_sAddrCode < c_dDat.e_sAddrCode ) {
				ret	= true;
			}
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

/**
	号ポイントの情報
*/
struct	gou_info
{
	string	e_sChiban;	///< 地番文字列（号番号含む）
	string	e_sCode11_1;///< 内包される行政界の11桁コード(CITYCODE+ADDRCODE) [bug 8338]②対応
	string	e_sCode11_2;///< 内包される行政界の11桁コード(CITYCODE+ADDRCODE2)[bug 8338]②対応
	int		e_iGouType;	///< 号タイプ 2008.12.03 [Bug 6543]対応
	long	e_lGaiku;	///< 街区符号 2008.12.16 [Bug 6543]対応
	string	e_sExtGaiku;///< 拡張街区符号 2008.12.16 [Bug 6543]対応
	int		e_iOID;		///< オブジェクトID
	/**
		コンストラクタ
	*/
	gou_info	( void ) {
		e_iOID		= 0;
		e_lGaiku	= 0;
	}
	/**
		デストラクタ
	*/
	~gou_info	( void ) { }

	bool	operator<(const gou_info c_dDat ) const {
		bool	ret	= false;
		if( e_sChiban != c_dDat.e_sChiban ) {
			if( e_sChiban < c_dDat.e_sChiban ) {
				ret	= true;
			}
		}else	// [bug 8338]②対応で修正
		if( e_sCode11_1 != c_dDat.e_sCode11_1 ) {
			if( e_sCode11_1 < c_dDat.e_sCode11_1 ) {
				ret	= true;
			}
		}else
		if( e_sCode11_2 != c_dDat.e_sCode11_2 ) {
			if( e_sCode11_2 < c_dDat.e_sCode11_2 ) {
				ret	= true;
			}
		}

		return	( ret );
	}
};

typedef	multiset<gou_info, less<gou_info> >					ms_gou_info;
typedef	multiset<gou_info, less<gou_info> >::iterator		ims_gou_info;

struct	admin_info
{
	int				e_iOID;		///< オブジェクトID
	string			e_sCode1;	///< 11桁コード(CITYCODE+ADDRCODE)[bug 8338]②対応
	string			e_sCode2;	///< 11桁コード(CiTYCODE+ADDRCODE2)[bug 8338]②対応
	long			e_lGaiku;	///< 街区符号 2008.12.16 [Bug 6543]対応
	string			e_sExtGaiku;///< 拡張街区符号 2008.12.16 [Bug 6543]対応
	IGeometryPtr	e_ipGeom;	///< ジオメトリ
	admin_info	() {
		e_iOID	= 0;
		e_lGaiku= 0;
	}
	~admin_info	() {}

	bool	operator<(const admin_info c_dDat ) const {
		bool	ret	= false;
		// [bug 8338]②対応で修正
		if( e_sCode1 != c_dDat.e_sCode1 ) {
			if( e_sCode1 < c_dDat.e_sCode1 ) {
				ret	= true;
			}
		}else
		if( e_sCode2 != c_dDat.e_sCode2 ) {
			if( e_sCode2 < c_dDat.e_sCode2 ) {
				ret	= true;
			}
		}
		return	( ret );
	}
};

typedef	vector<admin_info>				v_admin_info;
typedef	vector<admin_info>::iterator	iv_admin_info;

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
		char*					e_cpLog_File;			///< ログデータ出力パス
		char*					e_cpCode_List_Txt;		///< 11桁コードリストファイル
		char*					e_cpInit_File;			///< 初期化ファイル
		char*					e_sCheck_Layer;			///< チェックレイヤ名
		char*					e_sGou_Layer;			///< 号ポイントレイヤ名
		string					e_sWhereString;			///< 検索に使用するWhere句
		int						eMake_Type;				///< データ作成タイプ
		int						eExe_Type;				///< 実行タイプ
		int						eDigit;					///< メッシュコード桁数
		char*					e_cpAddrCode;			///< 住所コード
		int						eExe_Flag;
		int						e_iMeshCode;			///< メッシュコード
		double					e_Err_Dist;				///< エラー距離
		IFeatureClassPtr		e_ipF_Class;
		FILE*					e_fErr_Fp;				///<　エラーログファイル用ファイルポインタ
		set<int, less<int> >	e_sExi_Mesh_List;		///<　存在メッシュのリスト 2002.12.06
		set<int>				e_sOID_Set;				///<　オブジェクトIDのセット
		string					e_sVersion;				///< ツールバージョン 2008.11.12
	private:
		prog_init				e_pTool_Init;			// DBの初期化用設定
		smp_sindy_err_info		e_sPrnInfo;				// エラー等出力用
		mk_input_list			e_sInput_List;			// 入力リスト用
		exec_time_handle		e_eExeTime;				// 実行時間管理
		msi_handle_ws			e_mCity_Admin_WS;		// 行政界データ
		msi_handle_ws			e_mGou_Point_WS;		// 行政界データ
	private:
		ms_data_info			e_mAdmin_Datas;			///< 行政情報データ（変換モード確認用）
		s_data_info				e_mAdmin_Datas_Uniq;	///< 行政情報データ（11桁ユニーク）
		s_string				e_sCode_8;				///< 8桁コードリスト
		char*					e_cpPGDB_File;			///< PGDBファイル
		char*					WCARD1;
	public:

		main_rootin	() {
			e_cpMeshCode_File		= NULL;
			e_cpAddrCode_File		= NULL;
			e_cpLog_File			= NULL;
			e_cpCode_List_Txt		= NULL;
			e_cpInit_File			= NULL;
			e_sCheck_Layer			= NULL;
			e_sGou_Layer			= NULL;
			eMake_Type				= 0;
			eExe_Type				= 0;
			eDigit					= 0;
			eExe_Flag				= 0;
			e_iMeshCode				= 0;
			e_Err_Dist				= 100.0;	// 5.0km以上はエラー
			e_fErr_Fp				= stderr;	// デフォルトは標準エラー出力へ
			e_sExi_Mesh_List.clear();
			e_cpPGDB_File			= NULL;
			WCARD1					= "_";
		}
		~main_rootin	() {
			if( e_fErr_Fp != NULL ) {
				fclose ( e_fErr_Fp );
			}
		}
		/**
			SiNDYDBへの接続オープン
			@return	true	成功
			@return	false	失敗
		*/
		bool			SiNDY_DB_Open	(
							IWorkspacePtr&			c_ipWorkspace_Addr
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
							char*					c_cpInstance,		///< インスタンス
							char*					c_cpServer,			///< サーバ名
							char*					c_cpVersion			///< バージョン
						);

		/**
			文字列チェック
			@return	文字列
		*/
		string			Str_to_SJIS	(
							CComBSTR&	c_CStr		///< 文字列(Unicode)
						);

		/** ---------------------------------------------------------------------------
		//	エラーログのヘッダを出力する
		//	@return	無し
		-----------------------------------------------------------------------------*/
		void			Output_Log_Header	( void );

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
			Where()句を取得する
		*/
		string			Get_Where_String(
							char*			c_cpLayerName		// レイヤ名
						);
		/**
			チェック実行部
			※内部でエラーファイルの処理等も行う
			@return	成功	true
			@return 失敗	false
		*/
		bool			check_exe	( void );

		/**
			レイヤオープン
		*/
		bool			Open_Layers		(
							IWorkspacePtr	c_ipWorkspace_Addr	// 住所用ワークスペース
						);


		/**
			行政界のループ
		*/
		void			Admin_Check_Roops (
							IFeatureCursorPtr&	c_ipAdminCursor	///< 行政界カーソル
						);

		/**
			行政コードリストををセットに落とす
			@return	データ数
		*/
		int				Set_AdminCode_Data (
							IFeatureCursorPtr&	c_ipAdminCursor 	///< 注記カーソル
						);

		/**
			11桁コードリストの出力
		*/
		void			Print_Code_List	( void );
		
		/**
			変換モードコードエラーのチェック
		*/
		void			Chk_ConvMode	( void );

		/**
			11桁が一致する行政界の中で、変換モードが違うものをチェックする
		*/
		void			Chk_SameCode11Admin ( void );

		/**
			8桁が一致する行政界の中で、別ポリゴンに同じ号ポイントがあるかのチェック
		*/
		void			Chk_SameCode8Admin ( void );

		/**
			8桁が一致する行政界の中で、変換モードが違うものをチェックする
		*/
		void			Chk_SameCode8Admin_Mode ( void );

		/**
			号ポイントのリストを作成する
		*/
		int				Set_Gou_Datas	(
							v_admin_info&		c_vAdmLists, 		///< 行政界
							ms_gou_info&		c_mGou_Infos		///< 取り出した号データ
						);						
		/** 
			セットを見て号の重複をチェックする
		*/
		void			Chk_Gou_Dupli	( 
							ms_gou_info&		c_mGou_Infos		///< 号ポイント情報
						);

		/**
			行政界のリストを取得する（ジオメトリ含む）
		*/
		void			Set_Admin_List	(
							IFeatureCursorPtr&	c_ipAdminCursor,
							v_admin_info&		c_vAdmLists
						);
	
//------------------------------------------------------<ここまで>

};

#endif	//_MAIN_ROOTIN_H_

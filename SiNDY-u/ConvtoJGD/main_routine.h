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
#include <string>
#include <coord_converter.h>
#include "exec_time_handle.h"
#include <atlstr.h>
#include <SiNDYLib/Workspace.h>

#ifndef	_MAIN_ROUTINE_H_
#define	_MAIN_ROUTINE_H_

#define     ON_UPDATEMODE  0x01 //!< 更新モード指定（デフォルトはチェックモード）
#define     ON_RPARAM      0x02 //!< 地域パラメータ変換

#define	RET_COUNT   0x01
#define	RET_CURSOR  0x02

#define  ERR_STAT  _T("ERROR\t")

/**
 * @brief メイン処理クラス
 */
class	main_routine
{
	/**
	 * @brief 関係定義用
	 */
	enum {
		contains = 1,
		overlaps,
		within,
		touches,
		equals,
		crosses
	};

	public:
		//------------------------------------------------------------------------
		std::string   m_sDB_Connect1;   //!< DB接続情報１
		std::string   m_sFeatureClass1; //!< フィーチャクラス１名称
		std::string	  m_sLog_File;      //!< ログデータ出力パス
		std::string   m_sExeLog_File;   //!< 実行ログ出力パス
		std::string   m_sParam_File;    //!< 地域パラメータファイル
		std::string	  m_sRel_Str;       //!< 関係指定文字列
		long          m_lRel_Type;      //!< 指定したオブジェクト同士の関係の形式

		std::set<long>    m_sOBJECTID_List;   //!< OBJECTIDリスト

		IFeatureClassPtr  m_ipFeatureClass_1; //!< フィーチャクラス１

		IWorkspacePtr     ipWorkspace_1;      //!< ワークスペース

		IWorkspaceEditPtr m_ipWorkspaceEdit_1;//!< エディット用

		esriGeometryType  m_ipGeom_Type; //!< ジオメトリタイプ

		//------------------------------------------------------------------------
		long					m_lExe_Flag;
		FILE*					m_fErr_Fp;  //!< エラーログファイル用ファイルポインタ
		FILE*					m_fExe_Fp;  //!< エラーログファイル用ファイルポインタ
	private:
		exec_time_handle		m_eExeTime; //!< 実行時間管理

		crd_cnv					m_cCrd;
	private:
		CString       m_sVersion;  //!< ツールバージョン

	public:
		//!< コンストラクタ
		main_routine () {
			m_lExe_Flag		= 0;
			m_sRel_Str      = "contains";
			m_lRel_Type     = contains;
			m_fErr_Fp		= stderr; // デフォルトは標準エラー出力へ
			m_fExe_Fp		= stderr; // デフォルトは標準エラー出力へ
		}
		//!< デストラクタ
		~main_routine () {
			if( m_fErr_Fp != NULL ) {
				fclose ( m_fErr_Fp );
			}
			if( m_fExe_Fp != NULL ) {
				fclose ( m_fExe_Fp );
			}
		}

		/**
		 * @brief DBへの接続オープン
		 * @return  true   成功
		 * @return  false  失敗
		 */
		bool	DB_Open	(
		            const CString& c_csConnectStr, //!< [in] 接続文字列
		            IWorkspacePtr& c_ipWorkspace   //!< [in,out] Workspaceの参照
		        );

		/**
		 * @brief 実行ログファイルをオープンする
		 * @return true 成功
		 * @return  false  失敗
		 */
		bool    Open_Exe_Log ( void );

		/**
		 * @brief エラーログファイルをオープンする
		 * @return true 成功
		 * @return  false  失敗
		 */
		bool    Open_Err_Log ( void );

		/**
		 * @brief ツールの使用法の表示
		 */
		void    print_usage (
		            char** argv //!< [in] 引数列
		        );

		/**
		 * @brief 引数チェック
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    arg_check (
		            int    argc, //!< [in] 引数個数
		            char** argv  //!< [in] 引数列
		        );

		/**
		 * @brief リストの読み込み
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    Read_List (
		            const std::string& c_sFileName, //!< [in] ファイル名
		            std::set<long>&    c_sList      //!< [in] リスト（ID等に使用）
		        );

		/**
		 * @brief 指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータをとってくる
		 * @return IFeatureCursorPtr
		 */
		IFeatureCursorPtr   Search_Feature (
		                        const IFeatureClassPtr& c_ipFeCls,     //!< [in] フィーチャクラス
		                        const IGeometryPtr&     c_ipGeo,       //!< [in] 検索用ジオメトリ
		                        esriSpatialRelEnum      c_ipSpatialRel,//!< [in] 関係指定
		                        std::string             c_sWhere,      //!< [in] Where句指定
		                        long&                   c_iCount       //!< [out] 検索数
		                    );

		/**
		 * @brief オブジェクトIDリストの作成
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    MakeOBJECTIDList (
		            const IFeatureClassPtr& c_ipFeCls,        //!< [in] フィーチャクラス
		            std::set<long>&         c_sOBJECTID_List  //!< [out] OBJECTIDリスト
		        );

		/**
		 * @brief エディットの開始
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    StartEdit ( void );

		/**
		 * @brief エディットの終了
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool    StopEdit ( void );

		/**
		 * @brief 元のオブジェクトに対するターゲットの関係チェック
		 * @return 指定関係が成立   true
		 * @return 指定関係が不成立 false
		 */
		bool    RelCheck ( 
		            const IGeometryPtr& c_ipSrcGeom, //!< [in] 比較元ジオメトリ
		            const IGeometryPtr& c_ipTgtGeom, //!< [in] 比較先ジオメトリ
		            long                c_lRel_Type  //!< [in] 関係のタイプ
				);

		/**
		 * @brief 代表点を取得
		 * @return 
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool   GetPoint (
		           const IGeometryPtr& c_ipGeom, //!< [in] ジオメトリ
		           double&             c_dX,     //!< [out] 経度
		           double&             c_dY      //!< [out] 経度
		       );


		/**
		 * @brief 検索句を指定し、アップデート用のカーソルを取って来る
		 * @return	成功	true
		 * @return	失敗	false
		 */
		bool    Update_Where (
		            const IFeatureClassPtr&  c_ipTable,  ///< [in] 検索元テーブル
			        CString&                 c_sWhere,   ///< [in] 指定WHERE句
		            int                      c_iRetType, ///< [in] 何を返すか
		            long&                    c_lCount,   ///< [out] カウント
		            IFeatureCursorPtr&       c_ipCursor  ///< [out] カーソル
		        );

		/**
		 * @brief JGD移行したジオメトリを作成する
		 * @return IGeometryPtr
		 */
		IGeometryPtr Make_JGD_Geometry (
		                 IGeometryPtr& c_ipGeom //!< [in] ジオメトリ
		             );

		/**
		 * @brief 変換実行部
		 * @note ※内部でエラーファイルの処理等も行う
		 * @return	成功	true
		 * @return	成功	false
		 */
		bool update_exec ( void );
		bool update_exec_nolist ( void ); // 内部でリスト作成しない
		
		//--------------------------------------------------------------------------------
		/**
		* @brief データのコンテナを作成する
		* @return	データ数
		*/
		//--------------------------------------------------------------------------------
		int				Make_Data_Container (
							const IFeatureCursorPtr&							    c_ipFCursor,	//!< [IN] 注記カーソル
							std::map<long, CAdapt<IFeaturePtr>, std::less<long> >&	c_mWork_Data	//!< [IN] 作業用データ
						);
		
		//--------------------------------------------------------------------------------
		/**
		* @brief エラーメッセージ
		*/
		//--------------------------------------------------------------------------------
		void	PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... ) {
							
					va_list	a_vArgList = NULL;

					va_start(a_vArgList, c_cpFormat );	// 初期化

					vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
					fflush	( c_cpFp );
					if( c_cpFp2 != NULL && c_cpFp != c_cpFp2 ) {
						vfprintf_s ( c_cpFp2, c_cpFormat, a_vArgList );
						fflush	( c_cpFp2 );					
					}
				}
};
//------------------------------------------------------<ここまで>

#endif	//_MAIN_ROOTIN_H_

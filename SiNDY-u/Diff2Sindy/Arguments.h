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

#pragma once
#include <RowBase.h>

/// モードタイプ
enum S2SMode : int
{
	s2sModeConvert, //!< コンバートモード
	s2sModeMerge, //!< マージモード
	s2sModeCrdcnv, //!< 測地系変換モード
	s2sModeClip, //!< クリッピングモード
};

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	// システム変数(ツール内で使用する特別な変数)の識別子を定義
	enum SYSTEM_VALUE
	{
		MESHCODE,
	};

	/**
	 * @brief コンストラクタ
	 */
	Arguments()
	{
		m_thread = 1;
		m_meshLayerName = _T("CITYMESH");
		m_dissolveMulti = true;
		m_output_ondemand_search = false;
	}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  cOptName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam( const char* const cOptName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  入力データがPGDB又はFGDBか?
	 * @note   DB接続情報がファイルまたはフォルダであるかで判断
	 * @param  str  [in]  DB接続情報
	 * @retval true/DB接続情報がPGDBかFGDB
	 */
	bool isPGDBorFGDB(const uh::tstring& str);

	/**
	 * @brief  ディレクトリパスからPG(FG)DBのファイルパスを見つけ一覧で返却
	 * @param  directoryPath  [in]  ディレクトリパス
	 * @retval PG(FG)DBのファイルパス
	 */
	std::vector<uh::tstring> getPGFGDBPath(const uh::tstring& directoryPath);

	/**
	 * @brief  PG(FG)DBのファイルパスからメッシュコードを見つけ一覧で返却
	 * @param  vPaths  [in]  PG(FG)DBのファイルパス
	 * @retval メッシュコード一覧
	 */
	std::vector<uh::tstring> getMeshCodesFromPaths(const std::vector<uh::tstring>& vPaths);

	/**
	* @brief インプット/アウトプット側のオーナー名補完テーブル名を返す
	* @param name [in] 補完したいテーブル名
	* @return 「オーナー名.テーブル名」を返す\nオーナー名指定が無ければ「テーブル名」のみ返す
	*/
	uh::tstring getInputTableName( const uh::tstring& name ) const;
	uh::tstring getOutputTableName( const uh::tstring& name ) const;

	/**
	 * @brief オプションで指定された値を取得する
	 */
	CComVariant GetValue( LPCTSTR name ) const;

	/**
	 * @brief 識別子に対応するシステム変数を取得
	 * @param[in] identifier  識別子
	 * @param[in] nullValue  変数が定義されていない場合に返す値(デフォルト：空文字列)
	 * @retval nullValue以外 識別子に対応するシステム変数
	 * @retval nullValue     識別子に対応するシステム変数は定義されていない
	 */
	uh::tstring GetSystemValue(
		SYSTEM_VALUE identifier, const uh::tstring& nullValue = uh::tstring() ) const;

public:
	uh::tstring  m_input;					//!< [必須]入力情報
	uh::tstring  m_output;					//!< [必須]出力情報
	uh::tstring  m_input_owner;				//!< [任意]入力側オーナー名
	uh::tstring  m_output_owner;			//!< [任意]出力側オーナー名
	uh::tstring  m_mesh_list_str;			//!< [任意]メッシュ/リスト/ファイル
	uh::tstring  m_meshDB;					//!< [必須]メッシュDB
	uh::tstring  m_meshTableOwner;			//!< [任意]メッシュDBオーナー
	uh::tstring  m_meshLayerName;			//!< [必須]メッシュ処理の単位
	std::map<S2SMode,uh::tstring>  m_mapDefFile; //!< [必須]定義ファイルパス
	uh::tstring  m_runLog;					//!< [必須]実行ログファイルパス
	uh::tstring  m_errLog;					//!< [必須]エラーログファイルパス
	long m_thread;                          //!< [任意]スレッド数
	std::vector<uh::tstring> m_values;      //!< [任意]定義中で使用する値の指定(複数可)
	std::vector<uh::tstring> m_meshes;		//!< メッシュコードリスト
	std::vector<uh::tstring> m_paths;		//!< PG(FG)DBのファイルパス群
	bool m_dissolveMulti;                   //!< [任意]マルチパートを解消するかどうか（デフォルト：true）
	std::map<uh::tstring,CComVariant> m_mapValue; //!< GetValueで返す値
	std::map<SYSTEM_VALUE,uh::tstring> m_systemValue; //!< ツール内で特別な意味をもつ変数
	bool m_output_ondemand_search;          //!< [任意]アウトプット側の更新対象フィーチャは都度検索する（デフォルト：false）
};

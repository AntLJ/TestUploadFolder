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


/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:

	/**
	 * @brief パラメータの追加チェックモード
	 */
	enum ChkMode
	{
		kNone,		//!< なし
		kIsFile,	//!< ファイルパスチェック
		kIsFolder	//!< フォルダパスチェック
	};

	/**
	 * @brief コンストラクタ
	 */
	Arguments(){}

	/**
	 * @brief コマンドライン引数の解析
	 * @param[in]	argc  コマンドライン引数の数
	 * @param[in]	argv  コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);


private:

	/**
	 * @brief  コマンドライン引数の汎用チェック
	 * @param[in]  optName   オプション名
	 * @param[in]  optValue  オプション値
	 * @param[in]  bChkMode  オプションの追加チェックモード指定 
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, ChkMode eChkMode = kNone ) const;

	/**
	 * @brief  latlonオプションのチェック
	 * @param[in]  strOptValue   オプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckLatLonParam( const uh::tstring& strOptValue ) const;

public:
	uh::tstring  m_input_db;		//!< DB接続先
	uh::tstring  m_name_rule_ini;	//!< イラスト名生成ルール情報ファイル
	uh::tstring  m_output_ini;		//!< イラスト出力情報設定ファイル 
	uh::tstring  m_output_dir;		//!< 出力フォルダ 
	uh::tstring  m_runlog;			//!< 出力ログ 
	uh::tstring  m_errlog;			//!< エラーログ 
	uh::tstring  m_latlon;			//!< 緯度経度出力設定 

	uh::tstring  m_road_db;			//!< DB接続先(道路ノード)

};

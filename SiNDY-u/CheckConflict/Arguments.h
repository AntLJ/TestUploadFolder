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
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );
public:
	uh::tstring  m_tgtdb;    //!< チェック対象接続先
	uh::tstring  m_diffdb;   //!< 差分DB接続先
	uh::tstring  m_config;   //!< 設定ファイルのパス
	uh::tstring  m_runlog;   //!< 実行ファイルのパス
	uh::tstring  m_errlog;   //!< エラーログファイルのパス
};

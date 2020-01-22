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
	 * @brief コンストラクタ
	 */
	Arguments();

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

protected:
	/**
	 * @brief  必須となるコマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true    引数OK
	 * @retval false   引数NG
	 */
	bool checkParam( const char* const optName, const uh::tstring& optValue, bool bFile = false ) const;

	/**
	 * @brief 引数がファイルであることを確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true  ファイルである
	 * @retval false ファイルではない
	 */
	bool checkFile( const char* const optName, const uh::tstring& optValue ) const;

public:
	uh::tstring m_db;     //!< 対象DBの接続文字列
	uh::tstring m_config; //!< 定義ファイルパス
	uh::tstring m_runLog; //!< 実行ログファイルパス
	uh::tstring m_errLog; //!< エラーログファイルパス

};

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
	Arguments() : m_executeMode(ExecuteMode::none)
	{
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
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam(const char* const optName, const uh::tstring& optValue);

	/**
	 * @brief コマンドラインオプション一覧の取得
	 * retval CString 全オプションと指定した値
	 */
	CString GetAllOptionInfo();

public:
	uh::tstring m_cityDb; //!< 都市地図行政界ポリゴン接続先
	uh::tstring m_jusyoUser; //!< 住所ユーザ名(パスワードも同じ文字列とする)
	uh::tstring m_jusyoDb; //!< 住所DB名(delltcp、proteus)
	uh::tstring m_outputDir; //!< shpファイル出力先
	uh::tstring m_shapeName; //!< 郵便番号ポリゴンファイル名
	uh::tstring m_tableName; //!< 郵便番号テーブルファイル名
	uh::tstring m_runLog; //!< 実行ログ出力先パス
	uh::tstring m_errLog; //!< エラーログ出力先パス

	uh::tstring m_outputDb; //!< 出力先DB
	ExecuteMode m_executeMode; //!< 実行モード
};

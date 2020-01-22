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
	Arguments(){}

	/**
	 * @brief デストラクタ
	 */
	virtual ~Arguments(){}

	/**
	 * @brief コマンドライン引数の解析
	 * @param[in]	argc 	コマンドライン引数の数
	 * @param[in]	argv 	コマンドライン引数の内容
	 * @retval[in]	true	成功
	 * @retval[in]	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true    引数OK
	 * @retval false   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue);

public:
	uh::tstring  m_roadDB;    //!< 道路DB接続情報
	uh::tstring  m_adamDB;    //!< ADAM DB接続情報

	uh::tstring  m_meshListStr;     //!< メッシュコード、またはメッシュリストファイルパス
	std::vector<long> m_meshList;   //!< メッシュリスト

	uh::tstring  m_logDir;    //!< ログ出力フォルダパス
};

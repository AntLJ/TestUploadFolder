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
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  opt_name  [in]  オプション名
	 * @param  opt_value [in]  オプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam( const char* const opt_name, const uh::tstring& opt_value );

public:
	uh::tstring  m_RoadDB;    //!< 道路DB接続情報
	uh::tstring  m_AdamDB;    //!< ADAM DB接続情報
	uh::tstring  m_DiffDB;    //!< 道路リンクまたは車線リンクの形状差分FGDB

	uh::tstring  m_MeshListStr;     //!< メッシュコード、またはメッシュリストファイルパス
	std::vector<long> m_MeshList;   //!< メッシュリスト

	uh::tstring  m_TargetRoadWhere;	//!< 道路リンク検索条件

	uh::tstring  m_LogDir;    //!< ログ出力フォルダパス
};

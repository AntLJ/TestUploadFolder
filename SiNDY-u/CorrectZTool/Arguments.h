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
	Arguments(void):m_slope_max( -1.0 ){}

	/**
	 * @brief デストラクタ
	 */
	virtual ~Arguments(void){}
	
	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse( int argc, _TCHAR* argv[] );

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool checkParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool checkParam( const char* const optName, double optValue );

public:
	uh::tstring  m_db;				//!< 処理対象DB
	uh::tstring  m_mesh_db;			//!< メッシュ参照先DB
	uh::tstring  m_mesh_list;		//!< メッシュリスト
	uh::tstring  m_run_log;			//!< 実行ログ
	uh::tstring  m_err_log;			//!< エラーログ
	double       m_slope_max;		//!< 勾配の最大許容値（%）
};

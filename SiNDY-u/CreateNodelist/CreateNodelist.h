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

#include "Arguments.h"
#include <fstream>

/**
 * @brief   メインクラス
 */
class CCreateNodelist
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCreateNodelist( const Arguments& args ){ m_args = args; }

	/**
	 * @brief   デストラクタ
	 */
	CCreateNodelist(){ mRunLog.close(); mErrLog.close(); }

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief   エラーを出力する
	 * @brief strErrMsg  エラーメッセージ
	 */
	void outputErrMsg( const CString& strErrMsg );


private:

	Arguments m_args;			//!< 引数

	std::ofstream mRunLog;		//!< 処理ログのファイルストリーム
	std::ofstream mErrLog;		//!< エラーログのファイルストリーム
};


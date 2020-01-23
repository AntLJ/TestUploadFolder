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
#include "CheckHandler.h"

/**
 * @brief   メインクラス
 */
class CheckStationPolygonApp
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CheckStationPolygonApp( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	~CheckStationPolygonApp(){}

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();


private:

	/**
	 * @brief   ログファイル初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool initLog();

	/**
	 * @brief   チェッカー登録
	 */
	void resistChecker();

	/**
	 * @brief   チェッカー初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool initChecker();

	/**
	 * @brief   チェック実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool runCheck();

	/**
	 * @brief   後処理
	 * @param [in] bSuccess 正常終了したか否か
	 */
	void finish( bool bSuccess );

private:

	Arguments m_args;      //!< 引数

	std::vector<SPCheckHandler> m_CheckerContainer;		//!< チェッカーコンテナ
};

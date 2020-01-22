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
#include <TDC/sindylib_base/TableFinder.h>

/**
 * @brief   メインクラス
 */
class BldNameAddrAnalyzer
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	BldNameAddrAnalyzer( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~BldNameAddrAnalyzer(){};

	/**
	 * @brief   実行
	 * @note    致命的なエラーが発生した場合は例外を投げる
	 */
	void execute();

private:

	/**
	 * @brief   初期化
	 * @note    失敗した場合は例外を投げる
	 */
	void init();


private:

	Arguments m_args;             //!< 引数
	sindy::CTableFinder m_finder; //!< ファインダ
};


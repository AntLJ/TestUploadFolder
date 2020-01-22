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

#include "CheckControl.h"
#include "Argument.h"

/**
 * @class	CheckAddress
 * @brief	メインルーチンクラス
 */
class CheckAddress
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	CheckAddress(void){}

	 /**
	 * @brief  デストラクタ
	 */
	virtual ~CheckAddress(void){}

	/**
	* @brief   初期化
	* @param   [in] argc 	コマンドライン引数の数
	* @param   [in] argv 	コマンドライン引数の内容
	* @retval  true:成功
	* @retval  false:失敗
	*/
	bool init( int argc, TCHAR** argv );

	/**
	* @brief   実行
	* @retval  true:成功  false:失敗
	*/
	bool run();

private:	

	Argument                       m_argument;      //!< 引数
	CheckControl                   m_checkControl;  //!< 制御クラス

};


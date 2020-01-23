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

#include "Parameter.h"

/**
 * @enum	emCreateMode
 * @brief	作成モード
 */
enum emCreateMode
{
	kGPMode,		//!< 号ポイント
	kKPMode,		//!< 家形リンクポイント
	kCSMode,		//!< CSポイント
	kUnknownMode	//!< 不明
};

/**
 * @class	CApp
 * @brief	アプリケーションクラス
 */
class CApp
{
public:
	CApp(void) : m_emMode(kUnknownMode){}
	~CApp(void){}

	/**
	 * @brief	使用法
	 */
	static void Usage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool Run();

private:

	emCreateMode	m_emMode;	//!< 作成モード

	CParameter		m_cParam;	//!< 実行情報

	std::ofstream	m_ofs;		//!< ログ出力ストリーム
};

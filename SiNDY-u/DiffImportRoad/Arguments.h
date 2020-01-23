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

#include "InputInfo.h"
#include <TDC/useful_headers/boost_program_options.h>

namespace po = boost::program_options;

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments()
	{
	}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @param   cInputData[out] オプション情報確保用クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse( int argc, TCHAR* argv[], CInputInfo& cInputInfo );

private:
	/** 
	 * @brief 使用法表示 
	 * @param[in]	App	実行時コマンドラインの一つ目 
	 */ 
	void PrintUsage(LPCTSTR App, po::options_description& opt);

	/** 
	 * @brief 使用法表示 （ちょっとだけ詳しく）
	 * @param[in]	App	実行時コマンドラインの一つ目 
	 */ 
	void PrintHelp(LPCTSTR App, po::options_description& opt);

public:
	uh::tstring	input;	///< 入力パラメータ
};

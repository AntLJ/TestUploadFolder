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

#include <functional>

namespace uh
{
	/**
	* @brief flush用スコープガードクラス
	* @note Arc10.3.1で標準（エラー）出力をリダイレクトすると欠ける場合がある
	*       上記への対応としてスコープを抜ける際に必ずstd::flushを実行させる
	*/
	class flush_scope_guard
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		flush_scope_guard(){}
		/**
		* @brief デストラクタ
		*/
		~flush_scope_guard(){
			std::cout<<std::flush;
			std::cerr<<std::flush;
		}

	private:
	};
}

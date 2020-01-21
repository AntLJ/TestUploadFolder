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

namespace uh{
	/**
	* @brief スコープガードクラス
	* @note スコープを抜ける際に必ず処理を実行させることができます
	*/
	class scope_guard
	{
		/**
		* @brief コンストラクタ
		* @note 使用しない
		*/
		scope_guard();
	public:
		/**
		* @brief 関数
		* @param f [in] 実行する関数（関数ポインタ、ラムダ式等）
		*/
		explicit scope_guard( std::function<void()> f ): func(f){}
		/**
		* @brief デストラクタ
		*/
		~scope_guard(){ func(); }

	private:
		std::function<void()> func; //!< 実行する関数
	};
}

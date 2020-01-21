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

#include <sstream>
#include <stdexcept>

namespace uh
{
	/**
	 * @brief ロジック例外のメッセージを作成する
	 * @param file [in] ファイル名
	 * @param line [in] 行番号
	 * @param func [in] 関数名
	 */
	inline std::string makeLogicErrorMessage(const char* file, int line, const char* func)
	{
		std::ostringstream oss;
		oss << "THROW_LOGIC_ERROR @ " << func << " @ " << file << "(" << line << ")";
		return oss.str();
	}
}

/**
 * @brief 強制的にロジック例外を送出する
 * @note 到達し得ない個所を示すために開発した
 */
#define THROW_LOGIC_ERROR() throw std::runtime_error(uh::makeLogicErrorMessage(__FILE__, __LINE__, __FUNCTION__))

/**
 * @brief 式がfalseのときにロジック例外を送出する
 * @note Releaseビルドでも機能するassertとして開発した
 * @param expression [in] 評価する式
 */
#define ASSERT_LOGIC(expression) (void)( (!!(expression)) || ((THROW_LOGIC_ERROR()), 0) )

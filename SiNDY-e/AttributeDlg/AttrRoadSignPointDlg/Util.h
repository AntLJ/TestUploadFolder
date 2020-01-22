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

#include <regex>
#include <boost/format.hpp>

namespace util
{
	/**
	 *  @brief 文字列同士が部分一致しているかを返す
	 *         指定される文字はワイド文字であることを前提とする
	 *  @param[in] target_str   検索される側の文字列
	 *  @param[in] search_word  検索する文字列
	 *  @retval true   部分一致している
	 *  @retval false  部分一致していない
	 */
	bool partialMatch(const CString& target_str, const CString& search_word);
}


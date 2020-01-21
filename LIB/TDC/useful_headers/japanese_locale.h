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
#include <locale>

namespace uh
{
	/**
	 * @brief 文字種の扱い(と文字の並び順の設定)だけを日本語ロケールにする
	 * @note	std::ifstreamで日本語を含むマルチバイト文字列のファイル名を使いたいけど、
	 *			数値が3桁毎にカンマで区切られては困るという場合に使用。
	 */
	inline void setJapaneseCharacterTypeFacet()
	{
		std::locale::global(std::locale("Japanese", std::locale::ctype));
	}
}

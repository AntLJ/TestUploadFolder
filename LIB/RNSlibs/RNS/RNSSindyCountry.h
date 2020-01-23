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

/**
 * @file
 * @brief グローバルPJで追加。国の情報を扱う。
 */

#pragma once
#include <map>
#include <set>

namespace country
{
	/**
	 * @brief 国の種類
	 * @note 「MPA＝タイ」として良いかは今後の動向次第だが、とりあえず初期段階ではそうなのでそうしておく。
	 */
	enum class country_type
	{
		japan,     //!< 日本
		aseanL,    //!< 左側通行のASEAN諸国
		aseanR,    //!< 右側通行のASEAN諸国
	};

	typedef std::set< country_type > CountryTypes;

	/**
	 * @brief country_type全種類の集合を返す
	 * @return country_type集合
	 */
	CountryTypes allCountryTypes();

} // namespace country

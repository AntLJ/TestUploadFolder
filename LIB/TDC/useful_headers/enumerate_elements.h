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
#include <vector>

namespace uh
{
	typedef std::vector< std::string > ElementsStrings;

	// TODO: 戻り値型もテンプレート化してもいいかも。

	/**
	 * @brief 与えられた集合の要素を列挙した文字列を作成して返す関数テンプレート
	 * @note	「IDや文字列を1000個ずつ列挙したWhere句群」を作るのに使われるのを想定。
	 * @note	ライブラリ化の経緯は下記を参照のこと。
	 *			http://orias.mr.ipc.pioneer.co.jp/reviewboard/r/1957/#comment4609
	 * @param  elements   [in] 対象集合（メンバ関数としてbegin(), end()を持ち、それらによりiteratorを取得できる型である事が条件）
	 * @param  max        [in] 列挙する数の最大値（デフォルトは1000）
	 * @param  quote      [in] 引用符（デフォルトは空文字列）\n
	 *                        （列挙された各要素を囲む文字列。IN句の中では文字列要素はシングルクォートで囲む必要があるので、その指定に使われる事を想定。）
	 * @param  delimiter  [in] デリミタ（デフォルトは","）
	 */
	template <typename T>
	ElementsStrings enumerate_elements(
		const T& elements,
		unsigned max = 1000,
		const std::string& quote = std::string(),
		const std::string& delimiter = ","
	)
	{
		// 数値を文字列化した時に３桁毎に','が入ってしまうのを防止
		std::stringstream ss;
		ss.imbue( std::locale("Japanese", std::locale::ctype) );

		ElementsStrings strs;

		long count = 1;			///< 列挙数
		for ( T::const_iterator it = elements.begin(); it != elements.end(); ++it )
		{
			const auto& aID = *it;

			ss << quote << aID << quote;

			// 列挙数が最大値に達したら、そこまでの文字列を自身のコンテナに格納して仕切り直し
			if (count == max)
			{
				strs.push_back( ss.str() );
				ss.str("");
				ss.clear( std::stringstream::goodbit );
				count = 1;
			}
			// 最後のIDでなければデリミタを書き込んで次へ
			else if (it != boost::prior( elements.end() ))
			{
				ss << delimiter;
				++count;
			}
		}

		// 列挙数が最大値に達しない分を追加
		if (! ss.str().empty() )
			strs.push_back( ss.str() );

		return strs;		// 自動moveによりコピーコストは発生しない（はず）
	}

} // namespace uh

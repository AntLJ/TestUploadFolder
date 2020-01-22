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

#ifndef RNS_UTILITY_H_
#define RNS_UTILITY_H_

#include <utility>

namespace rns {

/**
 * @brief std::pair 構築補助関数クラス。
 *
 * std::make_pair() をクラスにしたようなもの。
 *
 * @attention
 * std::pair の各要素に対する explicit 宣言は無視される。
 *
 * @par テンプレート引数:
 * - TPairType : 二項コンストラクタが存在し、 first_type と second_type が定義された型。
 * - TFirstType : デフォルトは TPairType::first_type 。TPairType::first_type を戻り値とする単項関数（関数オブジェクト不可）や、TPairType::first_type への暗黙の型変換が可能な型でも可。
 * - TSecondType : デフォルトは TPairType::second_type 。TPairType::second_type を戻り値とする単項関数（関数オブジェクト不可）や、TPairType::second_type への暗黙の型変換が可能な型でも可。
 */
template <typename TPairType, typename TFirstType = TPairType::first_type, typename TSecondType = TPairType::second_type>
class pair_maker : public std::binary_function<TFirstType, TSecondType, TPairType>
{
public:
	template <typename T>
	TPairType operator()(T const& rT) const
	{
		return TPairType(TFirstType(rT), TSecondType(rT));
	}

	template <typename T1, typename T2>
	TPairType operator()(T1 const& first, T2 const& second) const
	{
		return TPairType(TFirstType(first), TSecondType(second));
	}
};

} // namespace rns

#endif // RNS_UTILITY_H_

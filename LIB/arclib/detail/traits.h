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

#ifndef __SINDY_DETAIL_TRAITS_H__
#define __SINDY_DETAIL_TRAITS_H__

#include "ref.h"

namespace sindy {

/**
 * @defgroup traits
 */
/*@{*/

/**
 * @brief 特性定義クラステンプレート
 *
 * SiNDY系のクラス・インタフェースの特性を定義するクラスの雛型。<br>
 * 対象となるクラス・インタフェースのポインタで特殊化する。<br>
 * ポインタで特殊化するのは、VCではインタフェースポインタ型のテンプレート引数（ _IRow* など）からポインタを外すのに手間が掛かるから。
 */
template <typename T>
struct pointer_traits {};

namespace detail {

template <int T> struct traits_selector { template <typename T> struct traits_of { typedef pointer_traits<T*> type; };  };
template <> struct traits_selector<1> { template <typename T> struct traits_of { typedef pointer_traits<T> type; }; };
template <> struct traits_selector<2> { template <typename T> struct traits_of { typedef pointer_traits<T> type; }; };
template <> struct traits_selector<3> { template <typename T> struct traits_of { typedef pointer_traits<typename T::Interface*> type; }; };
template <> struct traits_selector<4> { template <typename T> struct traits_of { typedef pointer_traits<typename T::type*> type; }; };

} // namespace detail

/**
 * @brief 特性取り出しテンプレート
 *
 * 様々な型種から、然るべき pointer_traits を導き出すためのテンプレートクラス。<br>
 * @a T の型種によって、以下のように化ける。
 * - 非ポインタ : pointer_traits<T*>
 * - ポインタ : pointer_traits<T>
 * - _com_ptr_t : pointer_traits<T::Interface*>
 * - reference_wrapper : pointer_traits<T::type*>
 */
template <typename T> struct traits : public detail::traits_selector<type_of<T>::value>::traits_of<T>::type {};

/*@}*/

/**
 * @defgroup container_traits
 */
/*@{*/

namespace detail {

template <int T> struct container_traits_selector { template <typename T> struct traits_of { struct type {}; }; };
template <> struct container_traits_selector<0>
{
	template <typename T>
	struct traits_of
	{
		struct type
		{
			typedef typename T::ElementTraits ElementTraits;
			typedef typename ElementTraits::Element Element;
			typedef typename ElementTraits::ElementHolder ElementHolder;
			typedef typename T::iterator iterator;

			/// @note テンプレート化しないとC1001が発生。
			template <typename T2> static typename T::iterator make_first(T2& rT2) { return T::make_first(rT2); }
		};
	};
};
template <> struct container_traits_selector<2> { template <typename T> struct traits_of { typedef traits<T> type; }; };
template <> struct container_traits_selector<3> { template <typename T> struct traits_of { typedef traits<T> type; }; };
template <> struct container_traits_selector<4> { template <typename T> struct traits_of { typedef typename container_traits_selector<0>::traits_of<typename T::type>::type type; }; };

} // namespace detail

/**
 * @brief コンテナ特性取り出しテンプレート
 *
 * @a T が有効なコンテナであるならば、最低限以下の型と関数が定義される。
 * - 型
 *   - ElementTraits : 要素特性。
 *   - ElementHolder : 要素保持クラス。コンストラクタ要素を格納し、デストラクタで解放する。
 *   - iterator : 反復子クラス。
 * - 関数
 *   - static iterator make_first(T&) : @a T の始点反復子を取得する関数。
 */
template <typename T>
struct container_traits : public detail::container_traits_selector<type_of<T>::value>::traits_of<T>::type {};

/*@}*/

} // namespace sindy

#endif // __SINDY_DETAIL_TRAITS_H__

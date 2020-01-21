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
 * @file iterator_base.h
 */
#ifndef ATL2_ITERATOR_BASE_H_
#define ATL2_ITERATOR_BASE_H_

#include <atl2/exception.h>
#include <atl2/type_traits.h>

namespace atl2 {

/**
 * @defgroup container_traits_group コンテナ特性定義
 *
 * @ref atl2::container_pointer_traits を特殊化し、コンテナの特性を定義したクラス。<br>
 * @ref iterator_helper_functionが反復子を生成するための手がかりとする。
 */
/*@{*/

/**
 * @brief コンテナインタフェースポインタの特性テンプレートクラス。
 *
 * @ref container_traits_group は、このクラスを特殊化して行う。
 * 
 * @par 型:
 * - <b>iterator</b> : コンテナを操作する反復子。
 *
 * @par 関数:
 * - <b>static iterator begin(T)</b> : 始点反復子を返す関数。 atl2::begin(TContainer const&) の実装に必要。
 * - <b>static iterator end(T)</b> : 終点反復子を返す関数。 atl2::end(TContainer const&) の実装に必要。
 * - <b>static iterator static_end()</b> : 絶対終点反復子を返す関数。 arctl::end() の実装に必要。
 */
template <typename T>
struct container_pointer_traits {};

/*@}*/

/**
 * @brief コンテナインタフェースの特性クラス。
 *
 * このクラスは arctl::container_pointer_traits への橋渡しを行う。
 */
template <typename T> struct container_traits : public container_pointer_traits<typename atl2::interface_pointer_of<T>::type> {};

/// 反復子を同定するクラス。
template <typename T> struct iterator_of { typedef typename container_traits<T>::iterator type; };

/// 逆行反復子を同定するクラス。
template <typename T> struct reverse_iterator_of { typedef typename container_traits<T>::reverse_iterator type; };

} // namespace atl2

#if _MSC_VER >= 1400
/**
 * @brief ADL 回避用名前空間
 *
 * @note
 * Boost.Range の adl_begin() や adl_end() から見つからないようにする。
 *
 * @note
 * VC6 の ADL は演算子以外には働かないので、回避策をとる必要はない。<br>
 * というか、とるとコンパイルできなくなったりする。
 */
namespace atl2_iterator_unspecified {
#else
namespace atl2 {
#endif // _MSC_VER >= 1400

/**
 * @defgroup iterator_helper_function 反復子生成関数
 *
 * @ref atl2::container_traits "container_traits" を用いて、渡されたコンテナを走査する反復子を生成する関数群。
 *
 * @note
 * begin(TContainer const&) や end(TContainer const&) が引数をconst参照で受け取っているのは、
 * スマートポインタの参照カウント操作を抑制することよりも、
 * 一時オブジェクトの寿命を引き伸ばす意味合いが大きい。<br>
 * @sa <a href="http://docs.sun.com/source/806-6491/lionel.html#669">一時オブジェクトの寿命</a>
 *
 * @par 用例:
 * @code
 * void foo(IRow* ipRow);
 *
 * void bar(ITable* ipTable)
 * {
 *   std::for_each(arctl::begin(ipTable->_Search(0, VARIANT_FALSE)), arctl::end<ICursor>(), foo);
 * @endcode
 */
/*@{*/

/**
 * @brief 始端反復子を生成する。
 *
 * @param rContainer [in,ref] コンテナ。
 * @return 反復子。
 */
template <typename TContainer>
typename atl2::iterator_of<TContainer>::type begin(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::begin(rContainer);
}

/**
 * @brief 終端反復子を生成する。
 *
 * @param rContainer [in,ref] コンテナ。
 * @return 反復子。
 */
template <typename TContainer>
typename atl2::iterator_of<TContainer>::type end(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::end(rContainer);
}

/**
 * @brief 終端を指し示す反復子を生成する。
 *
 * @return 反復子。
 */
template <typename TContainer>
typename atl2::iterator_of<TContainer>::type end()
{
	return atl2::container_traits<TContainer>::static_end();
}

/**
 * @brief 逆行始端反復子を生成する。
 *
 * @param rContainer [in,ref] コンテナ。
 * @return 逆行反復子。
 */
template <typename TContainer>
typename atl2::reverse_iterator_of<TContainer>::type rbegin(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::rbegin(rContainer);
}

/**
 * @brief 逆行終端反復子を生成する。
 *
 * @param rContainer [in,ref] コンテナ。
 * @return 逆行反復子。
 */
template <typename TContainer>
typename atl2::reverse_iterator_of<TContainer>::type rend(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::rend(rContainer);
}

/*@}*/

#if _MSC_VER >= 1400
} // namespace atl2_iterator_unspecified 

namespace atl2 {
	using namespace atl2_iterator_unspecified;
} // namespace atl2
#else
} // namespace atl2
#endif // _MSC_VER >= 1400

#endif // ATL2_ITERATOR_BASE_H_

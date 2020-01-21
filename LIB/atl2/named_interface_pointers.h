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

#ifndef ATL2_NAMED_INTERFACE_POINTERS_H_
#define ATL2_NAMED_INTERFACE_POINTERS_H_

#include <boost/mpl/list.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/static_assert.hpp>
#include <rns/mpl/name.h>
#include <rns/mpl/linearly_inheritance.h>
#include <atl2/type_traits/is_com_ptr_t.h>

namespace atl2 {

/**
 * @brief 名前付きインタフェースポインタクラス
 *
 * 静的名称文字列とインタフェースポインタ型を組み合わせて一つの型とする。
 *
 * @par テンプレート引数:
 * - @a Name - 名称文字列
 * - @a InterfacePtr - _com_ptr_t 型
 */
template <LPCTSTR Name, typename InterfacePtr>
class named_interface_pointer
{
public:
	BOOST_STATIC_ASSERT(atl2::is_com_ptr_t<InterfacePtr>::value);

	typedef typename InterfacePtr::Interface interface_type;

	LPCTSTR name() const { return Name; }

/// @name スマートポインタとしての機能
//@{
	InterfacePtr& get() { return val; }
	const InterfacePtr& get() const { return val; }

	interface_type& operator*() const { return *val; }
	interface_type* operator->() const { return val; }
//@}

/// @name 各種アルゴリズム用自己識別関数
//@{
	template <LPCTSTR T>
	InterfacePtr& get(
		typename boost::enable_if<
			boost::is_same<
				rns::mpl::name<T>,
				rns::mpl::name<Name>
			>
		>::type* = 0
	)
	{
		return val;
	}

	template <LPCTSTR T>
	const InterfacePtr& get(
		typename boost::enable_if<
			boost::is_same<
				rns::mpl::name<T>,
				rns::mpl::name<Name>
			>
		>::type* = 0
	) const
	{
		return val;
	}
//@}

private:
	InterfacePtr val;
};

namespace named_interface_pointers_detail { 

/**
 * @brief 2つの named_interface_pointer 継承クラスを継承するクラスを返すメタ関数
 *
 * @par 引数:
 * - @a Base - どちらかというとベースとなる named_interface_pointer 継承クラス
 * - @a T - どちらかというと追加する named_interface_pointer 継承クラス
 */
template <typename Base, typename T>
struct inherit
{
	struct type : Base, T
	{
		using Base::get;
		using T::get;
	};
};

/**
 * @brief 第1引数の値が boost::mpl::empty_base である場合の特殊化版 
 *
 * @par 引数:
 * - @a T - メタ関数戻り値
 */
template <typename T>
struct inherit<boost::mpl::empty_base, T>
{
	typedef T type;
};

} // namespace named_interface_pointers_detail

/**
 * @brief named_interface_pointer 多重継承クラス
 * 
 * @par 引数:
 * - @a List - named_interface_pointer シーケンス
 */
template <typename Seq>
class named_interface_pointers :
	public rns::mpl::linearly_inheritance<
		Seq,
		named_interface_pointers_detail::inherit<
			boost::mpl::_,
			boost::mpl::_
		>
	>
{
};

} // namespace atl2

#endif // ATL2_NAMED_INTERFACE_POINTERS_H_

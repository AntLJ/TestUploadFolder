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

#ifndef RNS_MPL_LINEARLY_INHERITANCE_H_
#define RNS_MPL_LINEARLY_INHERITANCE_H_

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace rns {

namespace mpl {

/**
 * @brief 拡張型線状継承クラス
 *
 * メタ関数 inherit_linearly の解に、アルゴリズム用の属性を追加したクラス。
 */
template <typename Seq, typename Node, typename Root = boost::mpl::empty_base>
struct linearly_inheritance : public boost::mpl::inherit_linearly<Seq, Node, Root>::type
{
	typedef Seq inheritance_type;
};

namespace detail {

template <typename List, typename Function>
class apply_each;

/**
 * @brief rns::mpl::linearly_inheritance で継承した個々の型の実体に対して個別に関数 f() を適用する。
 *
 * @param[in] l rns::mpl::linearly_inheritance オブジェクト。
 * @param[in] f 単項関数。 rns::mpl::for_each() の実体を参照。
 */
template <typename List, typename Function>
inline void for_each_impl(List& l, Function& f)
{
	using namespace boost::mpl;

	// 重いコンストラクタ／デストラクタを持つ型を継承している可能性がある以上、
	// add_pointer でポインタ型に変換した方が無難。
	for_each<typename List::inheritance_type, boost::add_pointer<_>	>(
		apply_each<List, Function>(l, f)
	);
}

/**
 * @brief boost::mpl::for_each() 用関数適用関数クラス
 */
template <typename List, typename Function>
class apply_each
{
public:
	/**
	 * @param[in] list 関数適用対象。
	 * @param[in] func 単項関数。
	 */
	apply_each(List& list, Function& func) :
	m_list(list),
	m_func(func)
	{
	}

	/**
	 * @brief 型 @e Item が rns::mpl::linearly_inheritance であった場合、アップキャストしたのち rns::mpl::for_each() を開始。
	 */
	template <typename Item>
	void operator()(Item*, typename Item::inheritance_type* = 0) const
	{
		for_each_impl(get<Item>(), m_func);
	}

	/**
	 * @brief 型 @e Item にアップキャストして関数を適用する。
	 */
	template <typename Item>
	void operator()(Item*, ...) const
	{
		m_func(get<Item>());
	}

private:
	/**
	 * @brief @e List が const なら const @e Item に、 const でなければ @e Item にアップキャストする。
	 */
	template <typename Item>
	typename boost::mpl::if_<boost::is_const<List>, boost::add_const<Item>, Item>::type& get() const
	{
		return m_list;
	};
		
	List& m_list;
	Function& m_func;
};

} // namespace detail

/**
 * @brief rns::mpl::linearly_inheritance で継承した個々の型の実体に対して個別に関数 f() を適用する。
 *
 * @param[in] l rns::mpl::linearly_inheritance オブジェクト。
 * @param[in] f 単項関数。
 * @return f
 *
 * 第1引数は inheritance_type さえ定義されていれば rns::mpl::linearly_inheritance である必要はない。<br>
 * 必要があれば「継承している任意の型それぞれに関数 f() を適用する」実装にすることも可能。
 *
 * rns::mpl::linearly_inheritance が rns::mpl::linearly_inheritance を継承している場合、後者が継承する個々の型の実体に対して関数 f() が適用される。<br>
 * rns::mpl::linearly_inheritance 自体に関数 f() を適用することはできない。
 *
 * @note
 * rns と rns::mpl どちらの名前空間に置くのが妥当だろう。<br>
 * この関数は std::for_each と boost::mpl::for_each のどちらに近い？
 *
 * @note
 * 一番近いのは、Boost 1.35で追加される予定の boost::fusion::for_each 。<br>
 * 強制再帰の有無を除けば、やろうとしていることは全く一緒。
 */
template <typename List, typename Function>
inline Function for_each(List& l, Function f, typename List::inheritance_type* = 0)
{
	mpl::detail::for_each_impl(l, f);
	return f;
}

} // namespace mpl

} // namespace rns

#endif // RNS_MPL_LINEARLY_INHERITANCE_H_

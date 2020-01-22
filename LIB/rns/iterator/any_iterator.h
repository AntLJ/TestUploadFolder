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

#ifndef RNS_ANY_ITERATOR_H_
#define RNS_ANY_ITERATOR_H_

#include <boost/scoped_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/static_assert.hpp>
#include <boost/cast.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace rns {

/**
 * @brief any_iterator 用 private 名前空間
 */
namespace any_iterator_detail {

/**
 * @brief 純粋な走査仕様を抜き出すメタ関数
 *
 * @par 引数:
 * - @e Traversal - 走査仕様。
 * 
 * @par 戻り値:
 * - @e boost::random_access_traversal_tag - ランダムアクセス走査。
 * - @e boost::bidirectional_traversal_tag - 双方向走査。
 * - @e boost::single_pass_traversal_tag - 入力走査。
 * - @e boost::incrementable_traversal_tag - インクリメント可能走査。
 * - @e void - 不明。
 *
 * @note
 * iterator_categories.hpp にあるメタ関数 boost::detail::pure_traversal_tag のパクリ。
 */
template <class Traversal>
struct pure_traversal_tag :
	boost::mpl::eval_if<
		boost::is_convertible<Traversal, boost::random_access_traversal_tag>,
		boost::mpl::identity<boost::random_access_traversal_tag>,
		boost::mpl::eval_if<
			boost::is_convertible<Traversal, boost::bidirectional_traversal_tag>,
			boost::mpl::identity<boost::bidirectional_traversal_tag>,
			boost::mpl::eval_if<
				boost::is_convertible<Traversal, boost::single_pass_traversal_tag>,
				boost::mpl::identity<boost::single_pass_traversal_tag>,
				boost::mpl::if_<
					boost::is_convertible<Traversal, boost::incrementable_traversal_tag>,
					boost::incrementable_traversal_tag,
					void
				>
			>
		>
	>
{
};

/**
 * @brief 仮想反復子に格納する反復子を保持するクラスのインタフェース、及びその実装を提供するメタ関数
 *
 * @par 引数:
 * - @e Reference - 反復子の参照はがしによって返される型。
 * - @e Difference - 反復子間の距離を表現する型。
 *
 * @par 戻り値:
 * - interface_t
 * - implement_t
 */
template <typename Reference, typename Difference>
struct iterator_holder
{
	/**
	 * @brief 仮想反復子に格納する反復子を保持するクラスのインタフェース
	 *
	 * @par テンプレート引数:
	 * - メタ関数 pure_traversal_tag の戻り値。
	 */
	template <typename>
	struct interface_t
	{
		typedef Reference reference;
		typedef Difference difference_type;
		typedef interface_t interface_type;
		
		virtual ~interface_t() {}

		virtual reference dereference() const = 0;
		virtual void increment() = 0;

		virtual interface_t* clone() const = 0;
	};

	template <>
	struct interface_t<boost::single_pass_traversal_tag> : public interface_t<boost::incrementable_traversal_tag>
	{
		virtual bool equal(const interface_type& rhs) const = 0;

		virtual interface_t* clone() const = 0;
	};

	template <>
	struct interface_t<boost::bidirectional_traversal_tag> : public interface_t<boost::single_pass_traversal_tag>
	{
		virtual void decrement() = 0;

		virtual interface_t* clone() const = 0;
	};

	template <>
	struct interface_t<boost::random_access_traversal_tag> : public interface_t<boost::bidirectional_traversal_tag>
	{
		virtual void advance(const difference_type& n) = 0;
		virtual difference_type distance_to(const interface_type& rhs) const = 0;

		virtual interface_t* clone() const = 0;
	};

	/**
	 * @brief 仮想反復子に格納する反復子を保持するクラス
	 *
	 * @par テンプレート引数:
	 * - @e Iterator - 格納する反復子。
	 */
	template <typename Iterator>
	class implement_t :
		public interface_t<
			typename pure_traversal_tag<
				typename boost::iterator_category_to_traversal<
					typename std::iterator_traits<Iterator>::iterator_category
				>::type
			>::type
		>
	{
	public:
		implement_t(const Iterator& it) :
		m_it(it)
		{
		}

	/// @name インタフェースの実装（必要なもののみ）
	//@{
		reference dereference() const
		{
			return *m_it;
		}

		bool equal(const interface_type& rhs) const
		{
			return m_it == dynamic_cast<const implement_t&>(rhs).m_it;
		}

		void increment()
		{
			++m_it;
		}

		void decrement()
		{
			--m_it;
		}

		void advance(const difference_type& n)
		{
			std::advance(m_it, n);
		}

		difference_type distance_to(const interface_type& rhs) const
		{
			return std::distance(m_it, dynamic_cast<const implement_t&>(rhs).m_it);
		}

		virtual implement_t* clone() const
		{
			return new implement_t(m_it);
		}
	//@}

		const Iterator& base() const
		{
			return m_it;
		}

	private:
		Iterator m_it;
	};
};

} // namespace any_iterator_detail

/**
 * @brief 仮想入力反復子クラス
 *
 * 使用する反復子を実行時に決められる入力反復子。<br>
 * Boost.Function の入力反復子版。のようなもの。
 *
 * @par テンプレート引数:
 * - @e Value - 要素型。
 * - @e CategoryOrTraversal - 反復子の種類。
 * - @e Reference - 参照はがしによって返される要素の型。デフォルトは要素への参照型。
 * - @e Difference - 反復子間の距離を表すのに用いる型。デフォルトは std::ptrdiff_t 。
 *
 * @attention
 * このクラスはdynamic_castを用いるので、使用にはRTTIが必要。
 */
template <
	typename Value,
	typename CategoryOrTraversal,
	typename Reference = Value&,
	typename Difference = std::ptrdiff_t
>
class any_iterator :
	public boost::iterator_facade<
		any_iterator<Value, CategoryOrTraversal, Reference, Difference>,
		Value,
		CategoryOrTraversal,
		Reference,
		Difference
	>
{
	friend class boost::iterator_core_access;

	typedef typename boost::iterator_category_to_traversal<iterator_category>::type iterator_traversal;
	typedef typename any_iterator_detail::iterator_holder<Reference, Difference> iterator_holder;
	typedef typename iterator_holder::template interface_t<any_iterator_detail::pure_traversal_tag<iterator_traversal>::type> iterator_holder_interface;
public:
	/**
	 * @brief あらゆる種類の反復子を受け取り、仮想化を試みる。
	 */
	template <typename Iterator>
	any_iterator(const Iterator& it) :
	m_pIt(new iterator_holder::implement_t<Iterator>(it))
	{
	}

	/**
	 * @brief @e Value か @e CategoryOrTraversal が異なる仮想反復子の、効率的な複製を試みる。
	 */
	template <typename V, typename CoT>
	any_iterator(const any_iterator<V, CoT, Reference, Difference>& rhs) :
	m_pIt(rhs.clone_iterator_holder())
	{
		BOOST_STATIC_ASSERT((boost::is_convertible<CoT, CategoryOrTraversal>::value));
	}

	/**
	 * @brief 純粋なコピーコンストラクタ。
	 */
	any_iterator(const any_iterator& rhs) :
	m_pIt(rhs.m_pIt->clone())
	{
	}

	/**
	 * @brief @e Value か @e CategoryOrTraversal が異なる仮想反復子の代入を試みる。
	 */
	template <typename V, typename CoT>
	any_iterator& operator=(const any_iterator<V, CoT, Reference, Difference>& rhs)
	{
		BOOST_STATIC_ASSERT((boost::is_convertible<CoT, CategoryOrTraversal>::value));
		m_pIt.reset(rhs.clone_iterator_holder());
		return *this;
	}

	/**
	 * @brief 純粋な代入演算。
	 */
	any_iterator& operator=(const any_iterator& rhs)
	{
		m_pIt.reset(rhs.m_pIt->clone());
		return *this;
	}

	/**
	 * @brief 格納している反復子を取り出す。
	 *
	 * @par テンプレート引数：
	 * - @e Iterator - 格納している反復子の型。
	 *
	 * @exception std::bad_cast 実際に格納されている反復子と異なる型を指定した。
	 */
	template <typename Iterator>
	const Iterator& base() const
	{
		return boost::polymorphic_cast<iterator_holder::implement_t<Iterator>*>(m_pIt.get())->base();
	}

	/**
	 * @brief テンプレート引数が部分的に異なる any_iterator 用の反復子保持オブジェクト複製関数。
	 *
	 * @note
	 * private化したいのだが、良い方法が思い浮かばない。
	 */
	iterator_holder_interface* clone_iterator_holder() const
	{
		return m_pIt->clone();
	}

private:
	reference dereference() const
	{
		return m_pIt->dereference();
	}

	bool equal(const any_iterator& rhs) const
	{
		return m_pIt->equal(*rhs.m_pIt);
	}

	void increment()
	{
		m_pIt->increment();
	}

	void decrement()
	{
		m_pIt->decrement();
	}

	void advance(difference_type n)
	{
		m_pIt->advance(n);
	}

	difference_type distance_to(const any_iterator& rhs) const
	{
		return m_pIt->distance_to(*rhs.m_pIt);
	}

	boost::scoped_ptr<iterator_holder_interface> m_pIt;
};

} // namespace rns

#endif // RNS_ANY_ITERATOR_H_

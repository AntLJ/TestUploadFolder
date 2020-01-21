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
 * @file zero_based_random_access_iterator.h
 */
#ifndef ATL2_ZERO_BASED_RANDOM_ACCESS_ITERATOR_H_
#define ATL2_ZERO_BASED_RANDOM_ACCESS_ITERATOR_H_

#include <algorithm>
#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/iterator/iterator_facade.hpp>
#pragma warning( pop )
#include <boost/iterator/reverse_iterator.hpp>
#include <atl2/exception.h>
#include <atl2/iterator/element_wrapper.h>

namespace atl2 {

/**
 * @brief 0起点のランダムアクセス反復子。
 *
 * ArcObjectsの @b IFields など、0を起点としたlong型をキーにしたランダムアクセス反復子。<br>
 *
 * @attention
 * dereference() はメンバ変数の、 distance_to() は渡された反復子のメンバ変数をconst_castする。
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*),
	long TBase = 0
>
class zero_based_random_access_iterator :
	public boost::iterator_facade<
		zero_based_random_access_iterator<TIContainer, TElement, TGetMethod, TCountMethod>, // CRTP
		TElement,                                                                           // 要素型
		boost::random_access_traversal_tag,                                                 // カテゴリ
		typename detail::element_wrapper_of<TElement>::type                                 // 参照型
	>
{
	friend class boost::iterator_core_access;
	typedef reference element_wrapper;
public:
// 定義
	typedef TIContainer container_type;

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)
	typedef std::_Undefined_inner_type_tag _Inner_type;
#endif //  (1400 <= _MSC_VER) && (_MSC_VER < 1700)

//@{ @name 構築/消滅
	/**
	 * @brief 遅延評価反復子を構築する。
	 *
	 * 遅延評価反復子は、通常の反復子との比較時に、インデックスが一意に定まる。
	 *
	 * @param bBegin [in] 始端反復子か。
	 */
	zero_based_random_access_iterator(bool bBegin = false) :
	m_ipContainer(0),
	m_nIndex(bBegin ? TBase : -1)
	{
	}

	/**
	 * @brief 反復子を構築する。
	 *
	 * @param ipContainer [in] コンテナ。
	 * @param nIndex [in] 要素のインデックス。
	 */
	zero_based_random_access_iterator(TIContainer* ipContainer, long nIndex) :
	m_ipContainer(ipContainer),
	m_nIndex(nIndex)
	{
	}
//@}

//@{ @name 取得
	TIContainer* container() const
	{
		return m_ipContainer; 
	}
//@}

private:
//@{ @name 反復子の定義
	element_wrapper dereference() const
	{
		_ASSERT(m_ipContainer != 0);
		element_wrapper aElement;
		atl2::valid((m_ipContainer->*TGetMethod)(m_nIndex, &aElement));
		return aElement;
	}

	void increment()
	{
		_ASSERT(m_ipContainer != 0);
		m_nIndex++;
	}

	void decrement()
	{
		_ASSERT(m_ipContainer != 0);
		m_nIndex--;
	}

	void advance(difference_type n)
	{
		_ASSERT(m_ipContainer != 0);
		m_nIndex += n;
	}

	bool equal(const zero_based_random_access_iterator& rhs) const
	{
		return distance_to(rhs) == 0;
	}

	difference_type distance_to(const zero_based_random_access_iterator& rhs) const
	{
		if(m_ipContainer == 0) {
			// 遅延評価反復子同士の比較。
			if(rhs.m_ipContainer == 0) {
				// 異なる遅延評価反復子間の距離を求めることはできない。
				_ASSERT(rhs.m_nIndex == m_nIndex);
			}
			else 
				return rhs.distance_to(*this);
		}
		else {
			if(rhs.m_ipContainer == 0) {
				// 遅延評価反復子の値を設定。
				zero_based_random_access_iterator& r = const_cast<zero_based_random_access_iterator&>(rhs);

				r.m_ipContainer = m_ipContainer;
				if(r.m_nIndex == -1) {
					atl2::valid((m_ipContainer->*TCountMethod)(&r.m_nIndex));
					r.m_nIndex += TBase;
				}
			}
		}

		return rhs.m_nIndex - m_nIndex;
	}
//@}

// 変数
	_com_ptr_t<_com_IIID<TIContainer, &__uuidof(TIContainer)> > m_ipContainer; ///< コンテナ。遅延評価反復子の場合はnull。
	long m_nIndex; ///< インデックス。遅延評価反復子の場合、-1ならば終端を指す。
};

/**
 * @brief ランダムアクセス可能なコンテナの特性。
 *
 * @par テンプレート引数:
 * - TIContainer : インタフェース。
 * - TElement : コンテナに含まれる要素。
 * - TGetNextMethod : 次の要素を取り出す関数。
 * - TCountMethod : 要素数を数える関数。
 * - TBase : 起点。
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*),
	long TBase = 0
>
struct zero_based_random_access_iterator_traits
{
// 定義
	typedef zero_based_random_access_iterator<TIContainer, TElement, TGetMethod, TCountMethod, TBase> iterator;
	typedef boost::reverse_iterator<iterator> reverse_iterator;

	typedef iterator const_iterator;
	typedef reverse_iterator const_reverse_iterator;

///@name 反復子の生成
//@{
	/**
	 * @brief 始端反復子を得る。
	 *
	 * @param ipContainer [in] コンテナ。nullの場合、終端遅延評価反復子が返る。
	 */
	static iterator begin(TIContainer* ipContainer)
	{
		if(ipContainer) {
			return iterator(ipContainer, TBase);
		}
		else
			return iterator(false);
	}

	/**
	 * @brief 終端反復子を得る。
	 *
	 * @param ipContainer [in] コンテナ。nullの場合、終端遅延評価反復子が返る。
	 */
	static iterator end(TIContainer* ipContainer)
	{
		if(ipContainer) {
			long nIndex;
			atl2::valid((ipContainer->*TCountMethod)(&nIndex));
			return iterator(ipContainer, nIndex + TBase);
		}
		else
			return iterator(false);
	}
//@}

///@name 逆行反復子の生成
//@{
	static reverse_iterator rbegin(TIContainer* ipContainer) { return reverse_iterator(end(ipContainer)); }
	static reverse_iterator rend(TIContainer* ipContainer) { return reverse_iterator(begin(ipContainer)); }
//@}

///@name 遅延評価反復子の生成
//@{
	static iterator static_begin() { return iterator(true); }
	static iterator static_end() { return iterator(false); }

	static reverse_iterator static_rbegin() { return reverse_iterator(static_end()); }
	static reverse_iterator static_rend() { return reverse_iterator(static_begin()); }
//@}
};

} // namespace atl2


#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)

namespace std {

template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*),
	long TBase
>
class _Checked_iterator_category<atl2::zero_based_random_access_iterator<TIContainer, TElement, TGetMethod, TCountMethod, TBase> >
{
public:
	typedef _Range_checked_iterator_tag _Checked_cat;
};

/**
 * @defgropu atl2_zero_based_random_access_iterator_algorithm zero_base_random_access_iterator特殊化版アルゴリズム
 *
 * 現在（VC8）の std::distance() や std::advance() は boost::random_access_traversal_tag を理解できない。<br>
 * そのため、より最適なアルゴリズムを使ってもらうためにはこちらで独自に定義しなければならない。
 *
 * @note
 * 新しい反復子コンセプトはTechnical Reportとして受け入れられている。<br>
 * いずれはこの特殊化も要らなくなるだろう。
 */
//@{
template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base
>
inline std::size_t distance(
	atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> first,
	atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> last
)
{
	return last - first;
}

template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base
>
inline std::size_t distance(
	boost::reverse_iterator<atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> > first,
	boost::reverse_iterator<atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> > last
)
{
	return last - first;
}

template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base,
	typename Diff
>
inline void advance(
	atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base>& it,
	Diff off
)
{
	it += off;
}

template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base,
	typename Diff
>
inline void advance(
	boost::reverse_iterator<atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> >& it,
	Diff off
)
{
	it += off;
}
//@}

} // namespace std

#endif // (1400 <= _MSC_VER) && (_MSC_VER < 1700)

#endif // ATL2_ZERO_BASED_RANDOM_ACCESS_ITERATOR_H_

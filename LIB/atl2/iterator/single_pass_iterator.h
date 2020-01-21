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
 * @file single_pass_iterator.h
 */
#ifndef ATL2_SIGLE_PASS_ITERATOR_H_
#define ATL2_SIGLE_PASS_ITERATOR_H_

#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/iterator/iterator_facade.hpp>
#pragma warning( pop )
#include <atl2/exception.h>
#include <atl2/iterator/element_wrapper.h>

namespace atl2 {

/**
 * @brief 単純走査反復子。
 *
 * <b>IEnum*</b>系のように、始まりから終わりまで単純に走査することしかできないコンテナの反復子。<br>
 * 反復子同士の比較は、両者が終端反復子であるか否かの判定しかできない。
 *
 * @note
 * この反復子は single pass iterator でなので、複製しても「位置」は保存されないが、反復子が指す「要素」は保存される。
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)
>
class single_pass_iterator :
	public boost::iterator_facade<
		single_pass_iterator<TIContainer, TElement, TGetNextMethod>, // CRTP
		TElement,                                                    // 要素型
		boost::single_pass_traversal_tag,                            // カテゴリ
		typename detail::element_wrapper_of<TElement>::type          // 参照型
	>
{
	friend class boost::iterator_core_access;
	typedef reference element_wrapper;
public:
// 定義
	typedef TIContainer container_type;

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)
	typedef std::_Undefined_inner_type_tag _Inner_type;
#endif // (1400 <= _MSC_VER) && (_MSC_VER < 1700)

//@{ @name コンストラクタとデストラクタ
	/**
	 * @brief 終端反復子を構築する。
	 */
	single_pass_iterator() :
	m_ipContainer(0)
	{
	}

	/**
	 * @brief 始端反復子を構築する。
	 *
	 * @param ipContainer [in] コンテナ。
	 */
	explicit single_pass_iterator(TIContainer* ipContainer) :
	m_ipContainer(ipContainer)
	{
		if(m_ipContainer != 0) {
			increment();
		}
	}

	~single_pass_iterator()
	{
	}
//@}

//@{ @name 取得
	TIContainer* container() const { return m_ipContainer; }
//@}

private:
//@{ @name 反復子の定義
	element_wrapper dereference() const
	{
		return m_Element;
	}

	void increment()
	{
		_ASSERT(m_ipContainer != 0);
		atl2::valid((m_ipContainer->*TGetNextMethod)(&m_Element));
	}

	bool equal(single_pass_iterator const& rIterator) const
	{
		return m_Element == 0 && rIterator.m_Element == 0;
	}
//@}

// 変数
	_com_ptr_t<_com_IIID<TIContainer, &__uuidof(TIContainer)> > m_ipContainer; ///< コンテナ。
	element_wrapper m_Element; ///< 現在指し示している要素。
};

/**
 * @brief 単純走査のみが可能なコンテナの特性。
 *
 * @par テンプレート引数:
 * - TIContainer : インタフェース。
 * - TElement : コンテナに含まれる要素。
 * - TGetNextMethod : 次の要素を取り出す関数。
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)
>
struct single_pass_container_traits
{
	typedef single_pass_iterator<TIContainer, TElement, TGetNextMethod> iterator;

	static iterator begin(TIContainer* ipContainer) { return iterator(ipContainer); }
	static iterator end(TIContainer*) { return iterator(); }

	static iterator static_end() { return iterator(); }
};

/**
 * @brief 最初の要素を取得するために Reset() する single_pass_container_traits 。
 *
 * @par テンプレート引数:
 * - TIContainer : インタフェース。
 * - TElement : コンテナに含まれる要素。
 * - TGetNextMethod : 次の要素を取り出す関数。
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*) = &TIContainer::Next
>
struct enumeration_container_traits : public single_pass_container_traits<TIContainer, TElement, TGetNextMethod>
{
	static iterator begin(TIContainer* ipContainer)
	{
		if(ipContainer) {
			ipContainer->Reset();
		}
		return iterator(ipContainer);
	}
};

} // namespace atl2

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)

namespace std {

template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)
>
class _Checked_iterator_category<atl2::single_pass_iterator<TIContainer, TElement, TGetNextMethod> >
{
public:
	typedef _Range_checked_iterator_tag _Checked_cat;
};

} // namespace std

#endif // (1400 <= _MSC_VER) && (_MSC_VER < 1700)

#endif // ATL2_SIGLE_PASS_ITERATOR_H_

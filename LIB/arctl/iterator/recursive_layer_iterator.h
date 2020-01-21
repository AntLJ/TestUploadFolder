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

#ifndef ARCTL_RECURSIVE_LAYER_ITERATOR_H_
#define ARCTL_RECURSIVE_LAYER_ITERATOR_H_

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <arctl/iterator/zero_based_random_access_iterators.h>

namespace arctl {

/**
 * @brief レイヤ再帰巡回反復子
 *
 * 複合レイヤの中を再帰的に巡る。
 *
 * @par テンプレート引数:
 * - @a BaseIterator - ILayerPtr を返す反復子。
 */
template <typename BaseIterator>
class recursive_layer_iterator :
	public boost::iterator_facade<
		recursive_layer_iterator<BaseIterator>,
		ILayerPtr,
		boost::single_pass_traversal_tag,
		ILayerPtr
	>
{
	friend class boost::iterator_core_access;
public:
// コンストラクタ
	/**
	 * @brief レイヤ再帰巡回反復子始点を構築する。
	 *
	 * @param it [in] レイヤ巡回反復子始点。
	 * @param last [in,optional] レイヤ巡回反復子終点。
	 *
	 * @warning
	 * @a last を省略できるのは、 BaseIterator() が終点を指す場合のみ。
	 */
	recursive_layer_iterator(BaseIterator it, BaseIterator last = BaseIterator()) :
	m_it(it),
	m_itLast(last)
	{
		expand_composite_layer();
	}

	recursive_layer_iterator(const recursive_layer_iterator& rhs) :
	m_it(rhs.m_it),
	m_itLast(rhs.m_itLast),
	m_pChildRange(rhs.m_pChildRange ? new child_range(*rhs.m_pChildRange) : 0)
	{
	}

// 取得
	const BaseIterator& base() const
	{
		return m_it;
	}

	const BaseIterator& end() const
	{
		return m_itLast;
	}

// 演算
	recursive_layer_iterator& operator=(const recursive_layer_iterator& rhs)
	{
		iterator_adaptor_::operator=(rhs);
		m_itLast = rhs.m_itLast;
		m_pChildRange.reset(rhs.m_pChildRange ? new child_range(*rhs.m_pChildRange) : 0);

		return *this;
	}

private:
// 型定義
	typedef recursive_layer_iterator<atl2::iterator_of<ICompositeLayer>::type> child_iterator; ///< 複合レイヤの１つ下の階層を巡る反復子
	typedef std::pair<child_iterator, child_iterator> child_range; ///< 複合レイヤの１つ下の階層を巡る反復子の対。

/// @name iterator_facade用関数
//@{
	/**
	 * @brief 指しているレイヤを返す。
	 *
	 * @pre *this != end()
	 *
	 * @return 反復子が指しているレイヤ。
	 */
	ILayerPtr dereference() const
	{
		return m_pChildRange ? ILayerPtr(*m_pChildRange->first) : *m_it;
	}

	/**
	 * @brief 同じ要素を指しているか。
	 *
	 * @param rhs [in] 比較対象反復子。
	 * @retval true 同じ要素を指している。
	 * @retval false 異なる要素を指している。
	 */
	bool equal(const recursive_layer_iterator& rhs) const
	{
		if(m_it != rhs.m_it)
			return false;

		return
			m_pChildRange
			? (rhs.m_pChildRange ? m_pChildRange->first == rhs.m_pChildRange->first : false)
			: (! rhs.m_pChildRange);
	}

	/**
	 * @brief 次の要素を指す。
	 *
	 * @pre *this != end()
	 *
	 * 複合レイヤの中を巡っている間は、複合レイヤの中の次の要素を指す。<br>
	 * 複合レイヤをめぐり終わったら次の要素へ移動し、 expand_composite_layer() を行う。
	 */
	void increment()
	{
		if(m_pChildRange) {
			if(++(m_pChildRange->first) != m_pChildRange->second)
				return;

			m_pChildRange.reset();
		}

		++m_it;

		expand_composite_layer();
	}
//@}

	/**
	 * @brief 複合レイヤの中を巡り始める。
	 *
	 * 現在指している要素が複合レイヤではない場合は何もしない。
	 *
	 * @pre ! m_pChildRange
	 */
	void expand_composite_layer()
	{
		// 複合レイヤを展開中に、この
		_ASSERT(! m_pChildRange);

		for(; m_it != m_itLast; ++m_it) {
			// 複合レイヤか。
			ICompositeLayerPtr ipCompositeLayer(*m_it);
			if(ipCompositeLayer == 0)
				break;

			// 複合レイヤを展開。
			std::auto_ptr<child_range>
				pChildRange(new child_range(atl2::begin(ipCompositeLayer), atl2::end(ipCompositeLayer)));

			// 複合レイヤの中身が空じゃないことを確認。
			if(pChildRange->first != pChildRange->second) {
				m_pChildRange.reset(pChildRange.release());
				break;
			}
		}
	}

// 変数
	BaseIterator m_it, m_itLast; ///< 終点反復子。
	boost::scoped_ptr<child_range> m_pChildRange; ///< 複合レイヤの始終点反復子。
};

/**
 * @brief レイヤ再帰巡回反復子を作成する。
 *
 * @param first [in] レイヤ巡回反復子始点。
 * @param last [in,optional] レイヤ巡回反復子終点。
 * @return レイヤ再帰巡回反復子。
 */
template <typename BaseIterator>
inline recursive_layer_iterator<BaseIterator>
make_recursive_layer_iterator(BaseIterator first, BaseIterator last = BaseIterator())
{
	return recursive_layer_iterator<BaseIterator>(first, last);
}

} // namespace arctl

#endif // ARCTL_RECURSIVE_LAYER_ITERATOR_H_

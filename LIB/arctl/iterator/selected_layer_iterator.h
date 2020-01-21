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

#ifndef ARCTL_SELECTED_LAYER_ITERATOR_H_
#define ARCTL_SELECTED_LAYER_ITERATOR_H_

#include <boost/iterator/iterator_facade.hpp>
#include <atl2/exception.h>

namespace arctl {

/**
 * @brief コンテンツビューで選択されているレイヤを巡回する反復子
 */
class selected_layer_iterator :
	public boost::iterator_facade<
		selected_layer_iterator,
		const ILayerPtr,
		boost::single_pass_traversal_tag
	>
{
	friend class boost::iterator_core_access;
public:
// コンストラクタ
	/**
	 * @brief 終点反復子を構築する。
	 */
	selected_layer_iterator()
	{
	}

	/**
	 * @brief 始点反復子を構築する。
	 *
	 * レイヤを選択していない場合は終点反復子となる。
	 *
	 * @param ipCV [in] コンテンツビュー。
	 */
	selected_layer_iterator(IContentsView* ipCV)
	{
		_ASSERT(ipCV);

//		_variant_t vSelectedItem(ipCV->SelectedItem);
		_variant_t vSelectedItem;
		ipCV->get_SelectedItem(&vSelectedItem);

		// 何も選択されていない場合は VT_NULL
		switch(V_VT(&vSelectedItem)) {
		case VT_UNKNOWN:
		case VT_DISPATCH:
			break;
		default:
			return;
		}

		m_ipSet = vSelectedItem;
		if(m_ipSet != 0) {
			// 複数のレイヤが選択されている。
			atl2::valid(m_ipSet->Reset());
			increment();
		}
		else {
			// １つのレイヤが選択されている。
			m_ipLayer = vSelectedItem;
		}
	}
	
private:
/// @name iterator_facade用関数
//@{
	/**
	 * @brief 指しているレイヤを返す。
	 *
	 * @pre *this != selected_layer_iterator()
	 *
	 * @return 反復子が指しているレイヤ。
	 */
	const ILayerPtr& dereference() const
	{
		return m_ipLayer;
	}

	/**
	 * @brief 終点に達したかを判定する。
	 *
	 * @param rhs [in] 比較対象反復子。
	 * @retval true どちらの反復子も終点を指している。
	 * @retval false どちらかの反復子が終端を指していない。
	 */
	bool equal(const selected_layer_iterator& rhs) const
	{
		return m_ipLayer == 0 && rhs.m_ipLayer == 0;
	}

	/**
	 * @brief 複数のレイヤが選択されている場合に限り、次のレイヤを指す。
	 *
	 * @pre *this != selected_layer_iterator()
	 */
	void increment()
	{
		m_ipLayer = 0;

		if(m_ipSet != 0) {
			for(IUnknownPtr ipUnk; atl2::valid(m_ipSet->Next(&ipUnk)) == S_OK; ) {
				m_ipLayer = ipUnk;
				if(m_ipLayer != 0)
					break;
			}
		}
	}
//@}

// 変数
	_ISetPtr m_ipSet; ///< 選択されているレイヤの一覧。
	ILayerPtr m_ipLayer; ///< 反復子が今指しているレイヤ。
};

} // namespace arctl

#endif // ARCTL_SELECTED_LAYER_ITERATOR_H_

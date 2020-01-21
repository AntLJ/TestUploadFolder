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
 * @file cursor.h
 * @brief _ICursor 系テンプレートライブラリ。
 */
#ifndef ARCTL_CURSOR_H_
#define ARCTL_CURSOR_H_

#include <atl2/exception.h>
#include <arctl/iterator/single_pass_iterators.h>

namespace arctl {

namespace detail {

template <bool TIsCursorIterator>
struct cursor_iterator_operator
{
	template <typename TIterator> static void update(TIterator it) { atl2::valid((*it)->Store()); }
	template <typename TIterator> static void del(TIterator it) { atl2::valid((*it)->Delete()); }
};

template <>
struct cursor_iterator_operator<true>
{
	static void update(_ICursor* ipCursor, _IRow* ipRow)
	{
		if(FAILED(ipCursor->UpdateRow(ipRow))) {
			atl2::valid(ipRow->Store());
		}
	}
	static void update(IFeatureCursor* ipFeatureCursor, IFeature* ipFeature)
	{
		if(FAILED(ipFeatureCursor->UpdateFeature(ipFeature))) {
			atl2::valid(ipFeature->Store());
		}
	}

	static void del(_ICursor* ipCursor, _IRow* ipRow)
	{
		if(FAILED(ipCursor->DeleteRow())) {
			atl2::valid(ipRow->Delete());
		}
	}
	static void del(IFeatureCursor* ipFeatureCursor, IFeature* ipFeature)
	{
		if(FAILED(ipFeatureCursor->DeleteFeature())) {
			atl2::valid(ipFeature->Delete());
		}
	}

	template <typename TIterator> static void update(TIterator it)
	{
		update(it.container(), *it);
	}
	template <typename TIterator> static void del(TIterator it)
	{
		del(it.container(), *it);
	}
};

template <typename T> struct cursor_pointer_traits {};

template <>
struct cursor_pointer_traits<_ICursor*>
{
	static _variant_t insert(_ICursor* ipCursor, IRowBuffer* ipRowBuffer)
	{
		_variant_t va;
		atl2::valid(ipCursor->InsertRow(ipRowBuffer, &va));
		return va;
	}
};

template <>
struct cursor_pointer_traits<IFeatureCursor*>
{
	static _variant_t insert(IFeatureCursor* ipCursor, IFeatureBuffer* ipFeatureBuffer)
	{
		_variant_t va;
		atl2::valid(ipCursor->InsertFeature(ipFeatureBuffer, &va));
		return va;
	}
};

} // namespace detail

/// _ICursor系反復子か否かを判定するクラス。
template <typename T> struct is_cursor_iterator { enum { value = false }; };
template <> struct is_cursor_iterator<atl2::container_traits<_ICursor>::iterator> { enum { value = true }; };
template <> struct is_cursor_iterator<atl2::container_traits<IFeatureCursor>::iterator> { enum { value = true }; };

/**
 * @brief 全ての要素に op(*it) を施行し、真値を返したものを _IRow::Store() するアルゴリズム。
 *
 * TInputIterator が _ICursor や IFeatureCursor の反復子である場合、 _IRow::Store() の前に _ICursor::UpdateRow() を試み、
 * それがエラーを返した時に改めて _IRow::Store() を施工する。<br>
 * これは反復子を構成するカーソルが「Searchカーソル」「Updateカーソル」のどちらであっても対応できるようにするための仕組みである。
 *
 * @note
 * 本当は FDO_E_CURSOR_WRONG_TYPE との比較が良さげなのが、PGDBが相手だと E_FAIL が返ってくるようなので、
 * エラーが返ってきたらすべて _IRow::Store() へ回すようにしている。
 *
 * @par テンプレート引数:
 * - @a TInputIterator : value_type が _IRow* か IFeature* である反復子。
 * - @a TUnaryOperation : std::unary_function<_IRow*, bool> 。
 *
 * @param first [in] 始点反復子。
 * @param last [in] 終点反復子。
 * @param op [in] 更新関数。
 * @return 更新関数。
 *
 * @test @ref unit_test UpdateAlgorithmTest
 */
template <typename TInputIterator, typename TUnaryOperation>
inline TUnaryOperation update(TInputIterator first, TInputIterator last, TUnaryOperation op)
{
	for(; first != last; ++first) {
		if(op(*first)) {
			arctl::detail::cursor_iterator_operator<is_cursor_iterator<TInputIterator>::value>::update(first);
		}
	}

	return op;
}

/**
 * @brief 全ての要素を _IRow::Delete() するアルゴリズム。
 *
 * @par テンプレート引数:
 * - TInputIterator : value_type が _IRow* か IFeature* である反復子。
 *
 * @param first [in] 始点反復子。
 * @param last [in] 終点反復子。
 *
 * @note
 * TInputIterator が _ICursor や IFeatureCursor の反復子である場合、 _IRow::Delete() の代わりに _ICursor::DeleteRow() を用いる。
 *
 * @test @ref unit_test UpdateAlgorithmTest
 */
template <typename TInputIterator>
inline void delete_all(TInputIterator first, TInputIterator last)
{
	for(; first != last; ++first) {
		arctl::detail::cursor_iterator_operator<is_cursor_iterator<TInputIterator>::value>::del(first);
	}
}

template <typename T>
class cursor_traits : public detail::cursor_pointer_traits<typename atl2::interface_pointer_of<T>::type>
{
	typedef detail::cursor_pointer_traits<typename atl2::interface_pointer_of<T>::type> super;
};

} // namespace arctl

#endif // ARCTL_CURSOR_H_

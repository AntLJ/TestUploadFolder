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

#ifndef SINDY_LQ_LIST_H_
#define SINDY_LQ_LIST_H_

#include <vector>
#include <algorithm>
#include <functional>
#include <rns/sstream.h>
#include <arctl/field.h>
#include <arctl/iterator.h>
#include <../../LIB/sindy/schema.h>
#include <../../LIB/arclib/sindy/link_queue/lq.h>

namespace sindy {

/**
 * @brief リンク列クラス
 *
 * リンク列を @a Lq の0起点配列で表したもの。<br>
 * Sequence属性値は1起点なので、配列番号=シーケンス-1となる。
 *
 * @par テンプレート引数:
 * Lq : リンク列を構成するリンク１つを表す型。
 */
template <typename Lq>
class lq_list_base : public std::vector<Lq>
{
public:
// 定義
	/// Lq属性値一覧。
	ARCTL_FIELD_VALUE_CLASS(lq_field_value_type, ((Lq)) (2(link_queue::kSequence, long)));
	/// 挿入反復子。
	class insert_iterator
	{
	public:
		insert_iterator(lq_list_base& rList, const lq_field_value_type::index_type* pIndex = 0) :
		m_rLqList(rList),
		m_pIndex(pIndex)
		{
		}

		insert_iterator(lq_list_base& rList, const lq_field_value_type::index_type& rIndex) :
		m_rLqList(rList),
		m_pIndex(&rIndex)
		{
		}

		insert_iterator& operator=(IRowBuffer* ipRow)
		{
			if(m_pIndex) {
				m_rLqList.insert_lq(lq_field_value_type(ipRow, *m_pIndex));
			}
			else {
				m_rLqList.insert_lq(lq_field_value_type(ipRow));
			}

			return *this;
		}

		insert_iterator& operator*() { return *this; }
		insert_iterator& operator++() { return *this; }

	private:
		lq_list_base& m_rLqList;
		const lq_field_value_type::index_type* m_pIndex;
	};

// 設定
	/**
	 * @brief LQ行を追加する
	 *
	 * @param row [in] LQ行。
	 * @retval true 追加成功。
	 * @retval false 追加失敗。
	 */
	template<typename Row>
	bool insert_lq(Row const& row)
	{
		return insert_lq(arctl::field::value_t<link_queue::kSequence, long>(row).value(), row);
	}

	/**
	 * @brief LQ行を追加する
	 *
	 * @param sequence [in] シーケンス番号。
	 * @param row [in] LQ行。
	 * @retval true 追加成功。
	 * @retval false 追加失敗。
	 */
	template<typename Row>
	bool insert_lq(long sequence, Row const& row)
	{
		// シーケンス番号は1以上である。
		if(sequence <= 0)
			return false;

		// 必要に応じて配列を拡張する。
		if(size() < static_cast<unsigned>(sequence)) {
			resize(sequence);
		}

		// LQ列を登録する。
		(*this)[sequence - 1] = row;

		return true;
	}

	/**
	 * @brief LQテーブルとリンク列ID指定で配列を構築（インデックス必須版）。
	 *
	 * @param ipLqTable [in] LQテーブル。
	 * @param inf_id [in] リンク列ID。
	 * @param index [in] 属性インデックス。
	 */
	void build(ITable* ipLqTable, long inf_id, const lq_field_value_type::index_type& index)
	{
		build(ipLqTable, inf_id, &index);
	}

	/**
	 * @brief LQテーブルとリンク列ID指定で配列を構築（インデックス任意版）。
	 *
	 * @param ipLqTable [in] LQテーブル。
	 * @param inf_id [in] リンク列ID。
	 * @param index [in,optioanl] 属性インデックス。
	 */
	void build(ITable* ipLqTable, long inf_id, const lq_field_value_type::index_type* index = 0)
	{
		using namespace arctl;

		// フィルタ用意。
		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		ipQueryFilter->put_SubFields(CComBSTR(link_queue::kLinkId));
		Lq::index_type::copy_name(adder(ipQueryFilter, &IQueryFilter::_AddField));

		// Where句用意。
		rns::tostringstream oss;
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		oss.imbue(loc);
		oss << link_queue::kInfId << _T("=") << inf_id;
		ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str()));

		// 導出されたLQ群で配列を構築。
		std::copy(
			atl2::begin(ipLqTable->_Search(ipQueryFilter, VARIANT_FALSE)),
			atl2::end<ICursor>(),
			insert_iterator(*this, index)
		);
	}

// 取得
	insert_iterator make_insert_iterator() { return insert_iterator(*this); }
	insert_iterator make_insert_iterator(const lq_field_value_type::index_type& index) { return insert_iterator(*this, index); }

// 判定
	/**
	 * @brief 指定したリンクIDの並びに、このリンク列は含まれるか
	 *
	 * @par テンプレート引数:
	 * - InputIterator 反復子の型。
	 * 反復子の要素は arctl::field::value_t<link_queue::kLinkId, long> へ暗黙の型変換が行えなければならない。
	 *
	 * @param first [in] 始点反復子。
	 * @param last [in] 終点反復子。
	 * @retval true 含まれる。
	 * @retval false 含まれない。
	 */
	template <typename InputIterator>
	bool within(InputIterator first, InputIterator last) const
	{
		// 要素数の差を得る。
		int gap = std::distance(first, last) - size();

		// 含まれるか否か判定する。
		for(int i = 0; i <= gap; i++, ++first) {
			if(std::equal(begin(), end(), first, std::equal_to<arctl::field::value_t<link_queue::kLinkId, long> >()))
				return true;
		}

		return false;
	}
};

/**
 * @brief LQ列型リンク列クラス定義。
 *
 * リンク列を「リンクID」と「方向」の配列で表したもの。
 */
typedef lq_list_base<lq_t> lq_list;

/**
 * @brief リンクID列型リンク列クラス定義。
 *
 * リンク列を「リンクID」の配列で表したもの。
 */
typedef lq_list_base<arctl::field::value_t<link_queue::kLinkId, long> > link_id_list;

/**
 * @brief sindy::lq3 による、リンクの並びクラス定義。 
 *
 * リンク列を「リンクID」と「方向」と「メッシュコード」の配列で表したもの。
 */
typedef lq_list_base<lq3_t> lq3_list;

} // namespace sindy

#endif // SINDY_LQ_LIST_H_

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
#include <iterator>
#include <algorithm>
#include <functional>
#include <arctl/field.h>
#include <arctl/iterator.h>
#include <CodeanAlysis/SourceAnnotations.h>

namespace sindy {

/**
 * @brief リンク列クラステンプレート
 *
 * リンク列を構成する @b Lq オブジェクトを要素とするvector配列。<br>
 * LQテーブルのシーケンス番号属性 @b SEQUENCE は1起点なので、 <b>配列のインデックス=SEQUENCE-1</b> となる。
 *
 * @par テンプレート引数:
 * Lq : リンク列を構成するリンク1つを表す型。
 */
template <typename Lq>
class lq_list : public std::vector<Lq>
{
public:
	/**
	 * @class lq_field_value_type
	 * @brief LQ属性値一覧
	 */
	ARCTL_FIELD_VALUE_CLASS(
		lq_field_value_type,
		((Lq))
		(2(schema::link_queue::kSequence, long))
	);

	/**
	 * @brief LQ挿入反復子
	 *
	 * 書き込まれた lq_field_value_type （あるいはそれに暗黙の型変換が可能な）オブジェクトを lq_list::insert_lq() に渡す反復子。
	 */
	class lq_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
	{
		typedef lq_insert_iterator self;
	public:
		lq_insert_iterator(lq_list& rList) :
		m_rLqList(rList)
		{
		}

		template<typename Row>
		self& operator=(Row const& rRow)
		{
			m_rLqList.insert_lq(rRow);
			return *this;
		}

		self& operator*() { return *this; }
		self& operator++() { return *this; }

	private:
		lq_list& m_rLqList;
	};

	/**
	 * @brief LQを挿入する。
	 *
	 * @param row [in] lq_field_value_type か、それに暗黙の型変換が可能なオブジェクト。
	 */
	template<typename Row>
	void insert_lq(Row const& row)
	{
		using namespace arctl::field;
		using namespace schema::link_queue;

		insert_lq(value_t<kSequence, long>(row).value(), row);
	}

	/**
	 * @brief LQを挿入する。
	 *
	 * @param sequence [in] シーケンス番号。
	 * @param row [in] @b Lq か、それに暗黙の型変換が可能なオブジェクト。
	 */
	template<typename Row>
	void insert_lq(long sequence, Row const& row)
	{
		// シーケンス番号は1以上である。
		if(sequence <= 0)
			throw std::underflow_error("sequence <= 0.");

		// 必要に応じて配列を拡張する。
		if(size() < static_cast<unsigned>(sequence)) {
			resize(sequence);
		}

		// LQ列を挿入する。
		(*this)[sequence - 1] = row;
	}

	/**
	 * @brief LQテーブルとリンク列ID指定で配列の要素を割り当てる。
	 *
	 * @param ipLqTable [in] LQテーブル。
	 * @param inf_id [in] リンク列ID。
	 */
	void assign_lq([SA_Pre(Null=SA_No)] ITable* ipLqTable, long inf_id)
	{
		assign_lq(ipLqTable, inf_id, lq_field_value_type::index_type(ipLqTable));
	}

	/**
	 * @brief LQテーブルとリンク列ID指定で配列の要素を割り当てる。
	 *
	 * @param ipLqTable [in] LQテーブル。
	 * @param inf_id [in] リンク列ID。
	 * @param index [in] 属性フィールドインデックス。
	 */
	void assign_lq([SA_Pre(Null=SA_No)] ITable* ipLqTable, long inf_id, const typename lq_field_value_type::index_type& index)
	{
		using namespace atl2;
		using namespace arctl;
		using namespace schema::link_queue;

		clear();

		const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		valid(ipQueryFilter->put_SubFields(CComBSTR(_T(""))));
		lq_field_value_type::index_type::copy_name(ipLqTable, adder(ipQueryFilter, &IQueryFilter::_AddField));

		std::basic_ostringstream<TCHAR> oss;

		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		oss.imbue(loc);

		oss << kInfID << _T("=") << inf_id;
		valid(ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str())));

		_ICursorPtr ipCursor;
		valid(ipLqTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor));

		std::transform(
			atl2::begin(ipCursor),
			atl2::end(ipCursor),
			lq_inserter(),
			field::interpreter<lq_field_value_type>(&index)
		);
	}

	/**
	 * @brief LQ挿入反復子を生成する。
	 */
	lq_insert_iterator lq_inserter()
	{
		return lq_insert_iterator(*this);
	}
};

/**
 * 
 */
template <typename LqListSpecifier>
class indirect_lq_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef indirect_lq_insert_iterator self;
public:
	indirect_lq_insert_iterator(LqListSpecifier cSpecifier) :
	m_Specifier(cSpecifier)
	{
	}

	template <typename Lq>
	self& operator=(const Lq& rLq)
	{
		insert_lq(m_Specifier(rLq), rLq);
		return *this;
	}
	
	self& operator*() { return *this; }
	self& operator++() { return *this; }

private:
	template <typename LqList, typename Lq>
	void insert_lq(LqList& rLqList, const Lq& rLq)
	{
		rLqList.insert_lq(rLq);
	}

	template <typename LqList, typename Lq>
	void insert_lq(LqList* pLqList, const Lq& rLq)
	{
		if(pLqList) {
			insert_lq(*pLqList, rLq);
		}
	}

	LqListSpecifier m_Specifier;
};

template <typename LqListSpecifier>
indirect_lq_insert_iterator<LqListSpecifier> make_indirect_lq_insert_iterator(LqListSpecifier& rLqListSpecifier)
{
	return indirect_lq_insert_iterator<LqListSpecifier>(rLqListSpecifier);
}

} // namespace sindy

#endif // SINDY_LQ_LIST_H_

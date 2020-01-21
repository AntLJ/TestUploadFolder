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

#ifndef SINDY_LQ_RELATION_SYNCER_H_
#define SINDY_LQ_RELATION_SYNCER_H_

#include <functional>
#include <boost/shared_ptr.hpp>
#include <rns/string.h>
#include <rns/sstream.h>
#include <sqltl/inclause_portal.h>
#include <arctl/field/oid.h>
#include <sindy/lq_relation/edit_watcher.h>
#include <sindy/stamper/modify_stamper.h>

namespace sindy {

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect 定義
//////////////////////////////////////////////////////////////////////

/**
 * @brief lq_relation_edit_watcher の情報を基に、LqRfの更新を行う関数クラス。
 *
 * @attention 更新を確実に反映させるには、オブジェクトを破棄する前に手動で flush() しなければならない。
 */
class lq_relation_reflect : public std::unary_function<const lq_relation_edit_watcher::value_type, void>
{
	class impl;
public:
// 定義
	typedef boost::shared_ptr<const modify_stamper> modify_stamper_ref;

// コンストラクタとデストラクタ
	lq_relation_reflect(
		IFeatureClass* ipRoadLinkClass,
		LPCTSTR lpszLqRfName,
		long nLqRfIndex,
		const modify_stamper_ref& pModStamper,
		ITable* ipLqTable);
	~lq_relation_reflect();

// 処理
	void flush() const;

// 演算
	void operator()(const lq_relation_edit_watcher::value_type& value) const;

private:
// 変数
	boost::shared_ptr<impl> pimpl;
};

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect::impl
//////////////////////////////////////////////////////////////////////

class lq_relation_reflect::impl
{
public:
// コンストラクタ
	impl(
		IFeatureClass* ipRoadLinkClass,
		LPCTSTR lpszLqRfName,
		long nLqRfIndex,
		const modify_stamper_ref& pModStamper,
		ITable* ipLqTable);

// 処理
	void flush();

	void flagup(long id);
	void flagdown(long id);
	void flagcheck(long id);

private:
// 定義
	struct data_t
	{
		IFeatureClassPtr m_ipRoadLinkClass; ///< 道路リンクフィーチャクラス。
		rns::tstring m_strLqRfName; ///< LqRf属性フィールド名。
		long m_nLqRfIndex; ///< LqRf属性フィールドインデックス。
		ITablePtr m_ipLqTable; ///< Lqテーブル。
	};

	/// フラグの付け替えを行う関数クラス。
	class flag : public std::unary_function<LPCTSTR, void>
	{
	public:
		flag(const data_t& d, long nValue, const modify_stamper_ref& pModStamper);

		void operator()(LPCTSTR where_clause) const;

	private:
		const data_t& m_Data;
		_variant_t m_vValue;
		const modify_stamper_ref m_pModStamper;
	};

	typedef sqltl::inclause_portal<flag> flag_portal;

// 変数
	data_t m_Data;
	flag_portal m_FlagUp;
	flag_portal m_FlagDown;
};

inline lq_relation_reflect::impl::impl(
	IFeatureClass* ipRoadLinkClass,
	LPCTSTR lpszLqRfName,
	long nLqRfIndex,
	const modify_stamper_ref& pModStamper,
	ITable* ipLqTable) :
m_FlagUp(flag(m_Data, 1, pModStamper), arctl::field::oid_index_t::bname(ipRoadLinkClass), 998),
m_FlagDown(flag(m_Data, 0, pModStamper), arctl::field::oid_index_t::bname(ipRoadLinkClass), 998)
{
	m_Data.m_ipRoadLinkClass = ipRoadLinkClass;
	m_Data.m_strLqRfName = lpszLqRfName;
	m_Data.m_nLqRfIndex = nLqRfIndex;
	m_Data.m_ipLqTable = ipLqTable;
};

/**
 * @brief 関係フラグを立てる。
 *
 * @param id [in] リンクID。
 */
inline void lq_relation_reflect::impl::flagup(long id)
{
	m_FlagUp(id);
}

/**
 * @brief 関係フラグを降ろす。
 *
 * @param id [in] リンクID。
 */
inline void lq_relation_reflect::impl::flagdown(long id)
{
	m_FlagDown(id);
}

/**
 * @brief 関係フラグを更新する。
 *
 * @param id [in] リンクID。
 */
inline void lq_relation_reflect::impl::flagcheck(long id)
{
	if(m_Data.m_ipLqTable == 0)
		throw std::runtime_error("Lqテーブルが存在しないため indeterminate を解決できない。");

	// 検索フィルタ用意。
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);

	// where句設定。
	rns::tostringstream oss;
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	oss.imbue(loc);
	oss << sindy::schema::link_queue::kLinkID << _T("=") << id;
	ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str()));

	// 関係フラグ更新。
	long nCount;
	atl2::valid(m_Data.m_ipLqTable->RowCount(ipQueryFilter, &nCount));
	if(nCount > 0) {
		// Lqが１つでも存在していたらフラグを立てる。
		flagup(id);
	}
	else {
		// Lqが１つも存在していなかったらフラグを降ろす。
		flagdown(id);
	}
}

/**
 * @brief 関係フラグの更新のデータベースへの反映を完全なものとする。
 */
inline void lq_relation_reflect::impl::flush()
{
	m_FlagUp.flush();
	m_FlagDown.flush();
}

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect::impl::flag
//////////////////////////////////////////////////////////////////////

inline lq_relation_reflect::impl::flag::flag(const data_t& data, long nValue, const modify_stamper_ref& pModStamper) :
m_Data(data),
m_vValue(nValue),
m_pModStamper(pModStamper)
{
}

/**
 * @brief フラグの付け替えを行う。
 *
 * @param where_clause [in] フラグの付け替え対象となるオブジェクトを求めるwhere句。
 */
inline void lq_relation_reflect::impl::flag::operator()(LPCTSTR where_clause) const
{
	// 検索フィルタ用意。
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);

	// フィールド限定。
	ipQueryFilter->put_SubFields(CComBSTR(m_Data.m_strLqRfName.c_str()));

	// where句設定。
	rns::tostringstream oss;
	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	oss.imbue(loc);
	oss << where_clause << _T(" AND ") << m_Data.m_strLqRfName << _T("<>") << V_I4(&m_vValue);
	ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str()));

	// 更新カーソル取得。
	IFeatureCursorPtr ipFeatureCursor;
	if(m_Data.m_ipRoadLinkClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor) != S_OK)
		return;

	// 更新。
	IFeaturePtr ipFeature;
	while(ipFeatureCursor->NextFeature(&ipFeature) == S_OK) {
		// 値が現状と違う場合のみ更新する。
		_variant_t vOldValue;
		atl2::valid(ipFeature->get_Value(m_Data.m_nLqRfIndex, &vOldValue));
		if(vOldValue != m_vValue) {
			atl2::valid(ipFeature->put_Value(m_Data.m_nLqRfIndex, _variant_t(m_vValue)));

			// 更新履歴を付与する。
			if(m_pModStamper) {
				(*m_pModStamper)(ipFeature, sindy::schema::ipc_table::update_type::kAttrUpdated);
			}

			ipFeature->Store();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// lq_relation_reflect 実装
//////////////////////////////////////////////////////////////////////

/**
 * @param ipRoadLinkClass [in] 道路リンクフィーチャクラス。
 * @param lpszLqRfName [in] LqRf属性フィールド名。
 * @param nLqRfIndex [in] LqRf属性フィールドインデックス。
 * @param pModStamper [in] 更新情報付与オブジェクト。
 * @param ipLqTable [in] Lqテーブル。indeterminateの解決時に必要となる。
 */
inline lq_relation_reflect::lq_relation_reflect(
	IFeatureClass* ipRoadLinkClass,
	LPCTSTR lpszLqRfName,
	long nLqRfIndex,
	const modify_stamper_ref& pModStamper,
	ITable* ipLqTable) :
pimpl(new impl(ipRoadLinkClass, lpszLqRfName, nLqRfIndex, pModStamper, ipLqTable))
{
}

inline lq_relation_reflect::~lq_relation_reflect()
{
}

/**
 * @brief 関係フラグの更新のデータベースへの反映を完全なものとする。
 */
inline void lq_relation_reflect::flush() const
{
	pimpl->flush();
}

/**
 * @brief 関係フラグの更新を試みる。
 *
 * @param val [in] リンクIDとフラグの対。
 */
inline void lq_relation_reflect::operator()(const lq_relation_edit_watcher::value_type& val) const
{
	if(val.second) {
		// true
		pimpl->flagup(val.first);
	}
	else if(! val.second) {
		// false
		pimpl->flagdown(val.first);
	}
	else {
		// indeterminate
		pimpl->flagcheck(val.first);
	}
}

} // namespace sindy

#endif // SINDY_LQ_RELATION_SYNCER_H_

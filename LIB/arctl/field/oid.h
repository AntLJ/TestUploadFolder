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
 * @file oid.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_OID_H_
#define ARCTL_FIELD_OID_H_

#include <boost/operators.hpp>
#include <atl2/exception.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {
namespace field {

/**
 * @ingroup field_index_group 属性値クラス
 * @brief OID属性インデックスクラス。
 */
class oid_index_t : public arctl::detail::multi_inherit::base_t
{
public:
//@{ @name 構築/消滅
	oid_index_t() : m_n(-1) {}
	oid_index_t(const oid_index_t& rIndex) : m_n(rIndex.m_n) {}
	oid_index_t(IClass* ipClass) { operator=(ipClass); }
//@}

//@{ @name 取得
	long index() const { return m_n; }
//@}

//@{ @name 演算
	operator long() const { return index(); }
	bool operator!() const { return m_n == -1; }

	oid_index_t& operator=(IClass* ipClass)
	{
		atl2::valid(ipClass->FindField(bname(ipClass), &m_n));
		return *this;
	}
//@}
	
//@{ @name 静的関数
	/// 属性フィールド名をBSTR型で取得する。
	static CComBSTR bname(IClass* ipClass)
	{
		CComBSTR bstrName;
		atl2::valid(ipClass->get_OIDFieldName(&bstrName));
		return bstrName;
	}

	/// 属性フィールド名を出力する。
	template <typename TOutputIterator>
	static void copy_name(IClass* ipClass, TOutputIterator result)
	{
		*result = COLE2CT(bname(ipClass)).m_psz;
		++result;
	}
//@}

private:
// 変数
	long m_n; ///< 属性フィールドインデックス。
};

/**
 * @ingroup field_value_group
 * @brief オブジェクトIDクラス。
 *
 * オブジェクトIDを取得・保持するクラス。
 *
 * @par 属性値取得可能オブジェクト:
 * _IRow
 * @par 属性値の取得:
 * _IRow::get_OID()
 * @par 属性値の書き出し:
 * 不可。 write() は false を返す。
 *
 * @test @ref unit_test FieldOIDTest
 */
class oid_t : public arctl::detail::multi_inherit::base_t, boost::totally_ordered<oid_t>
{
public:
// 定義
	typedef oid_index_t index_type;
	typedef long value_type;

///@name 構築/消滅
//@{
	oid_t() : m_nOID(0) {}
	oid_t(long nOID) : m_nOID(nOID) {}
	oid_t(_IRow* ipRow) { atl2::valid((ipRow->get_OID)(&m_nOID)); }
	oid_t(_IRow* ipRow, const index_type&) { atl2::valid((ipRow->get_OID)(&m_nOID)); }
//@}

///@name 取得
//@{
	const long& oid() const { return m_nOID; }
//@}

///@name 属性値クラス互換
//@{
	long& value() { return m_nOID; }
	const long& value() const { return oid(); }
	bool write(IRowBuffer*, const index_type&) const { return false; }
//@}

///@name 演算子
//@{
	bool operator==(const oid_t& rhs) const { return m_nOID == rhs.m_nOID; }
	bool operator<(const oid_t& rhs) const { return m_nOID < rhs.m_nOID; }

	oid_t& operator=(long nOID)
	{
		m_nOID = nOID;
	}

	oid_t& operator=(_IRow* ipRow)
	{
		atl2::valid((ipRow->get_OID)(&m_nOID));
		return *this;
	}
//@}

private:
// 変数
	long m_nOID;
};

} // namespace field
} // namespace arctl

#endif // ARCTL_FIELD_OID_H_

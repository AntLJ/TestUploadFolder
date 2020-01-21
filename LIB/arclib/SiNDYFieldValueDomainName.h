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
 * @file SiNDYFieldValueDomainName.h
 * @brief 属性値ドメイン名取得関数・保持クラス定義
 * @author 地図DB制作部道路制作グループ 菊地福太郎
 * $Id$
 */
#ifndef _SINDY_FIELDVALUE_DOMAINNAME_H_
#define _SINDY_FIELDVALUE_DOMAINNAME_H_

#include "SiNDYField.h"
#include "SiNDYFieldValue.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////////////////////////
//
// getDomainName
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 属性値ドメイン名をBSTR型で取得する
 *
 * @param ipCodedValueDomain [in] ICodedValueDomain。
 * @param rValue [in] 値。
 * @param lpDomainName [out] ドメイン名（BSTR）。
 * @retval S_OK 取得成功。
 * @retval <>S_OK 取得失敗。
 */
inline HRESULT getDomainName(ICodedValueDomain* ipCodedValueDomain, const VARIANT& rValue, BSTR* lpDomainName)
{
	if(! (ipCodedValueDomain && lpDomainName))
		return E_POINTER;

	long nCodeCount = 0;
	ipCodedValueDomain->get_CodeCount(&nCodeCount);
	for(long i = 0; i < nCodeCount; i++) {
		_variant_t va;
		ipCodedValueDomain->get_Value(i, &va);
		if(va == rValue) {
			ipCodedValueDomain->get_Name(i, lpDomainName);
			return S_OK;
		}
	}

	return S_FALSE;
}

/**
 * @brief 属性値ドメイン名をCString型で取得する
 *
 * @param ipCodedValueDomain [in] ICodedValueDomain。
 * @param rValue [in] 値。
 * @param rDomainName [out] ドメイン名（CString）。
 * @retval S_OK 取得成功。
 * @retval <>S_OK 取得失敗。
 */
inline HRESULT getDomainName(ICodedValueDomain* ipCodedValueDomain, const VARIANT& rValue, CString& rDomainName)
{
	CComBSTR bstrDomainName;
	HRESULT hr = getDomainName(ipCodedValueDomain, rValue, &bstrDomainName);
	rDomainName = bstrDomainName;
	return hr;
}

/**
 * @brief 属性値ドメイン名を取得するCString型で取得する
 *
 * @param ipField [in] コード値ドメインを持つフィールド。
 * @param rValue [in] 値。
 * @param rDomainName [out] ドメイン名（CString）。
 * @retval S_OK 取得成功。
 * @retval <>S_OK 取得失敗。
 */
inline HRESULT getDomainName(IField* ipField, const VARIANT& rValue, CString& rDomainName)
{
	IDomainPtr ipDomain;
	ipField->get_Domain(&ipDomain);
	return getDomainName(ICodedValueDomainPtr(ipDomain), rValue, rDomainName);
}

/**
 * @brief 属性値ドメイン名をCString型で返す
 *
 * @param ipCodedValueDomain [in] ICodedValueDomain。
 * @param rValue [in] 値。
 * @return コード値ドメイン名。
 */
inline CString getDomainName(ICodedValueDomain* ipCodedValueDomain, const VARIANT& rValue)
{
	CComBSTR bstrDomainName;
	getDomainName(ipCodedValueDomain, rValue, &bstrDomainName);
	return bstrDomainName;
}

/**
 * @brief 属性値ドメイン名をCString型で返す
 *
 * @param ipField [in] コード値ドメインを持つフィールド。
 * @param rValue [in] 値。
 * @return コード値ドメイン名。
 */
inline CString getDomainName(IField* ipField, const VARIANT& rValue)
{
	IDomainPtr ipDomain;
	ipField->get_Domain(&ipDomain);
	return getDomainName(ICodedValueDomainPtr(ipDomain), rValue);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// FieldValueDomainName
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 属性値ドメイン名を取得・格納するクラス
 */
template <LPCTSTR _FieldName>
class FieldValueDomainName
{
public:
	/**
	 * @brief _FieldNameに関するICodedValueDomain、および値を持つオブジェクトからドメイン名を取得し、インスタンスを構築
	 *
	 * @param rRow [in] FieldとFieldValueを取り出すことのできるオブジェクト。
	 */
	template<typename _Row>
	FieldValueDomainName(_Row const& rRow) :
	m_strDomainName(getDomainName(rRow, rRow))
	{
	}

// 取得演算
	operator const CString& () const
	{
		return m_strDomainName;
	}

	operator const LPCTSTR () const
	{
		return static_cast<LPCTSTR>(m_strDomainName);
	}

private:
// 静的取得
	/**
	 * @brief _RowからFieldとFieldValueを一意に取り出せた場合の処理
	 *
	 * @param rField [in] フィールド保持者。
	 * @param rValue [in] 値保持者。
	 * @return ドメイン名。
	 */
	static CString getDomainName(const sindy::Field<_FieldName>& rField, const sindy::FieldValue<_FieldName>& rValue)
	{
		return sindy::getDomainName(static_cast<IField*>(rField), static_cast<const _variant_t&>(rValue));
	}

// 変数
	CString m_strDomainName; ///< ドメイン名
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// domainname_cast
//
//////////////////////////////////////////////////////////////////////////////////////////

template <LPCTSTR _FieldName>
const FieldValueDomainName<_FieldName>& domainname_cast(const FieldValueDomainName<_FieldName>& rDomainName)
{
	return rDomainName;
}

} // namespace sindy;

#endif // _SINDY_FIELDVALUE_DOMAINNAME_H_

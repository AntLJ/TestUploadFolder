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
 * @file domain.h
 * @brief ドメイン関係。
 *
 * @warning
 * このファイルはdraft版であり、今後仕様変更／廃止される可能性があります。
 */
#ifndef ARCTL_DOMAIN_H_
#define ARCTL_DOMAIN_H_

#include <atl2/exception.h>

namespace arctl {

namespace domain {

/**
 * @brief ドメインにおける値の別称を求める。
 *
 * @p ipCodedValueDomain がnullであるか、対応する値がコード化されていない場合、 @p rValue をそのまま文字列化する。
 *
 * @param rValue [in] 値。
 * @param ipCodedValueDomain [in,nullable] コード値ドメイン。
 * @return 値の別称。 
 */
inline _bstr_t get_code_name(const VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain)
{
	if(ipCodedValueDomain) {
		long nCodeCount;
		ipCodedValueDomain->get_CodeCount(&nCodeCount);

		for(long i = 0; i < nCodeCount; i++) {
			_variant_t aValue;
			CComBSTR bstrName;

			ipCodedValueDomain->get_Value(i, &aValue);
			ipCodedValueDomain->get_Name(i, &bstrName);

			if(aValue == rValue)
				return _bstr_t(bstrName.Detach(), false);
		}
	}

	// F.Abe 2007/12/03追加
	if (rValue.vt == VT_NULL)
		return _bstr_t(CComBSTR(_T("")).Detach(), false);

	VARIANT vNew;
	::VariantInit(&vNew);
	atl2::valid(::VariantChangeType(&vNew, const_cast<VARIANT*>(&rValue), 0, VT_BSTR));
	return _bstr_t(vNew.bstrVal, false);
}

} // namespace domain

} // namespace arctl

#endif // ARCTL_DOMAIN_H_

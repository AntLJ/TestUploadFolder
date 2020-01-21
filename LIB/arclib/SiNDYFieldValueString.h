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

#ifndef __SINDY_FIELDVALUE_STRING_H__
#define __SINDY_FIELDVALUE_STRING_H__

#include "SiNDYImplicitCast.h"

namespace sindy {

// CFieldValueBSTR 定義
/////////////////////////////////////////////////////////////////////////////

class CFieldValueBSTR : public CComBSTR
{
public:
	CFieldValueBSTR(VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain = 0);

private:
	void setFieldValueString(VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain);
	void setFieldValueString(VARIANT& rValue);
};

// CFieldValueString 定義
/////////////////////////////////////////////////////////////////////////////

class CFieldValueString : public CString
{
public:
	CFieldValueString(VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain = 0);
	CFieldValueString(VARIANT& rValue, ICodedValueDomainPtr& ipCodedValueDomain);
	CFieldValueString(VARIANT& rValue, const CodedValueDomainPtr& ipCodedValueDomain);
	CFieldValueString(const CFieldValueBSTR& rBSTR);
};

// CFieldValueBSTR 実装
/////////////////////////////////////////////////////////////////////////////

inline CFieldValueBSTR::CFieldValueBSTR(VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain)
{
	setFieldValueString(rValue, ipCodedValueDomain);
}

/**
 * @note このメソッドを使用するには、 SiNDYAlgorithm.h をインクルードする必要があります。
 */
inline void CFieldValueBSTR::setFieldValueString(VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain)
{
	// ドメイン名取得
	if(ipCodedValueDomain) {
		long nIndex = find(ipCodedValueDomain, rValue);
		if(nIndex >= 0) {
			ipCodedValueDomain->get_Name(nIndex, operator&());
			return;
		}
	}

	setFieldValueString(rValue);
}

inline void CFieldValueBSTR::setFieldValueString(VARIANT& rValue)
{
	USES_CONVERSION;

	// 値の文字列化
	switch(rValue.vt) {
	case VT_EMPTY:
		Attach(T2BSTR(_T("<empty>")));
		break;
	case VT_NULL:
		Attach(T2BSTR(_T("<null>")));
		break;
	case VT_ERROR:
		Attach(T2BSTR(_T("<error>")));
		break;
	case VT_UNKNOWN:
		Attach(T2BSTR(_T("<unknown>")));
		break;
	case VT_BSTR:
		CComBSTR::operator=(rValue.bstrVal);
		break;
	default:
		{
			CComVariant va;
			va.ChangeType(VT_BSTR, &rValue);
			operator=(va);
		}
	}
}

// CFieldValueString 実装
/////////////////////////////////////////////////////////////////////////////

inline CFieldValueString::CFieldValueString(VARIANT& rValue, ICodedValueDomain* ipCodedValueDomain) :
CString(CFieldValueBSTR(rValue, ipCodedValueDomain))
{
}

inline CFieldValueString::CFieldValueString(VARIANT& rValue, ICodedValueDomainPtr& ipCodedValueDomain) :
CString(CFieldValueBSTR(rValue, ipCodedValueDomain))
{
}

inline CFieldValueString::CFieldValueString(VARIANT& rValue, const CodedValueDomainPtr& ipCodedValueDomain) :
CString(CFieldValueBSTR(rValue, ipCodedValueDomain))
{
}

inline CFieldValueString::CFieldValueString(const CFieldValueBSTR& rBSTR) :
CString(rBSTR)
{
}

} // namespace sindy

#endif // __SINDY_FIELDVALUE_STRING_H__

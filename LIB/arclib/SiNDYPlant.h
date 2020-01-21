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

// SiNDYPlant.h: SiNDYPlant クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYPLANT_H__7AA94568_52CE_44FA_8730_8C657A9DCCF8__INCLUDED_)
#define AFX_SINDYPLANT_H__7AA94568_52CE_44FA_8730_8C657A9DCCF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

namespace sindy
{
	/// VARIANTの単純な大小比較クラス
	class CComVariantLess : std::binary_function<CComVariant, CComVariant, bool>
	{
	public:
		bool operator()(const CComVariant& lhs, const CComVariant& rhs) const;
	};

	// フィールド取得
	IFieldPtr getField(_IRow* ipRow, BSTR bstrFieldName);

	// 文字列取得系
	void getValuesOrDomainNamesString(const std::map<long, _IRowPtr>& rRowMap, BSTR bstrFieldName, CString& rString);
	CComVariant getValue(_IRow* ipRow, BSTR bstrFieldName);
	CString getValueOrDomainNameString(_IRow* ipRow, BSTR bstrFieldName);
	CString getValueOrDomainNameString(IField* ipField, const CComVariant& vaValue);
	CString getUnknownValueString(IUnknown* ipUnknown);
	CString getPointCollectionString(IPointCollection* ipPointCol);
	CString getPointString(IPoint* ipPoint);
};

#endif // !defined(AFX_SINDYPLANT_H__7AA94568_52CE_44FA_8730_8C657A9DCCF8__INCLUDED_)

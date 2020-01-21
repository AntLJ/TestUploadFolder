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

// SiNDYPlant.cpp: SiNDYPlant クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiNDYPlant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy
{

bool CComVariantLess::operator()(const CComVariant& lhs, const CComVariant& rhs) const
{
	if(lhs.vt != rhs.vt) 
		return false;

	switch(lhs.vt) {
	case VT_I2:
		return lhs.iVal < rhs.iVal;
	case VT_I4:
		return lhs.lVal < rhs.lVal;
	case VT_R4:
		return lhs.fltVal < rhs.fltVal;
	case VT_R8:
		return lhs.dblVal < rhs.dblVal;
	case VT_BSTR:
		return _bstr_t(lhs) < _bstr_t(rhs);
	default:
		{
			CComVariant va1(lhs);
			CComVariant va2(rhs);
			if(FAILED(va1.ChangeType(VT_I4)))
				return false;
			if(FAILED(va2.ChangeType(VT_I4)))
				return false;
			return (*this)(va1, va2);
		}
	}
}

IFieldPtr getField(_IRow* ipRow, BSTR bstrFieldName)
{
	if(! ipRow)
		return 0;

	IFieldsPtr ipFields;
	if(ipRow->get_Fields(&ipFields) != S_OK)
		return 0;

	long nIndex;
	if(ipFields->FindField(bstrFieldName, &nIndex) != S_OK || nIndex < 0)
		return 0;

	IFieldPtr ipField;
	if(ipFields->get_Field(nIndex, &ipField) != S_OK)
		return 0;

	return ipField;
}

void getValuesOrDomainNamesString(const std::map<long, _IRowPtr>& rRowMap, BSTR bstrFieldName, CString& rString)
{
	for(std::map<long, _IRowPtr>::const_iterator it = rRowMap.begin(); it != rRowMap.end(); ++it) {
		CString strValue = getValueOrDomainNameString(it->second, bstrFieldName);
		if(strValue.IsEmpty())
			continue;

		if(it != rRowMap.begin()) {
			rString += _T(";");
		}

		rString += strValue;
	}
}

CComVariant getValue(_IRow* ipRow, BSTR bstrFieldName)
{
	CComVariant va;

	if(ipRow == 0)
		return va;

	// 該当するフィールド取得
	IFieldsPtr ipFields;
	if(ipRow->get_Fields(&ipFields) != S_OK)
		return va;

	long nFieldIndex;
	if(ipFields->FindField(bstrFieldName, &nFieldIndex) != S_OK)
		return va;

	// 値取得
	ipRow->get_Value(nFieldIndex, &va);

	return va;
}

/**
 * @brief 指定したオブジェクトの属性値を文字列として、コード値ドメイン優先で取得する
 *
 * @param ipRow [in] 対象オブジェクト
 * @param bstrFieldName [in] 属性フィールド名
 * @return 属性値文字列。取得できなかった場合\"\<error\>\"。
 */
CString getValueOrDomainNameString(_IRow* ipRow, BSTR bstrFieldName)
{
	CString strResult;

	if(ipRow == 0)
		return _T("<error>");

	IFieldsPtr ipFields;
	if(ipRow->get_Fields(&ipFields) != S_OK)
		return _T("<error>");

	// 該当するフィールド取得
	long nFieldIndex;
	if(ipFields->FindField(bstrFieldName, &nFieldIndex) != S_OK)
		return _T("<error>");

	IFieldPtr ipField;
	if(ipFields->get_Field(nFieldIndex, &ipField) != S_OK)
		return _T("<error>");

	// 属性値取得
	CComVariant va;
	ipRow->get_Value(nFieldIndex, &va);

	// 文字列化
	return getValueOrDomainNameString(ipField, va);
}

CString getValueOrDomainNameString(IField* ipField, const CComVariant& vaValue)
{
	// 該当するフィールドのドメイン取得
	IDomainPtr ipDomain;
	if(ipField != 0) {
		ipField->get_Domain(&ipDomain);
	}
	ICodedValueDomainPtr ipCodedValueDomain = ipDomain;

	// 属性の文字列化
	switch(vaValue.vt) {
	case VT_NULL:
		return _T("<null>");
	case VT_EMPTY:
		return _T("<empty>");
	case VT_ERROR:
		return _T("<error>");
	case VT_UNKNOWN:
		return getUnknownValueString(vaValue.punkVal);
	default:
		if(ipCodedValueDomain != 0) {
			long nCodeCount = 0;
			ipCodedValueDomain->get_CodeCount(&nCodeCount);
			for(int i = 0; i < nCodeCount; i++) {
				CComVariant va;
				ipCodedValueDomain->get_Value(i, &va);
				if(va == vaValue) {
					CComBSTR bstrValue;
					ipCodedValueDomain->get_Name(i, &bstrValue);
					return CString(bstrValue);
				}
			}
		}
		{
			CComVariant va = vaValue;
			return va.ChangeType(VT_BSTR) == S_OK ? CString(va.bstrVal) : _T("<error>");
		}
	}

	return _T("<error>");
}

CString getUnknownValueString(IUnknown* ipUnknown)
{
	if(ipUnknown == 0)
		return _T("<null unknown>");

	IPointPtr ipPoint = ipUnknown;
	if(ipPoint != 0)
		return getPointString(ipPoint);

	IPointCollectionPtr ipPointCol = ipUnknown;
	if(ipPointCol != 0)
		return getPointCollectionString(ipPointCol);


	return _T("<error unknown>");
}

CString getPointCollectionString(IPointCollection* ipPointCol)
{
	if(ipPointCol == 0)
		return _T("<null pointcollection>");

	CString strResult;

	long nPointCount;
	ipPointCol->get_PointCount(&nPointCount);
	for(int i = 0; i < nPointCount; i++) {
		IPointPtr ipPoint;
		ipPointCol->get_Point(i, &ipPoint);
		strResult += getPointString(ipPoint);
	}

	return strResult;
}

CString getPointString(IPoint* ipPoint)
{
	if(ipPoint == 0)
		return _T("(?,?)");

	double dX, dY;
	ipPoint->get_X(&dX);
	ipPoint->get_Y(&dY);
	
	CString strResult;
	strResult.Format(_T("(%f,%f)"),dX, dY);

	return strResult;
}

}; // SiNDYPlant

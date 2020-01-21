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
 *	GDBDomain.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		V‹Kì¬
 */

#include "GDB.h"

HRESULT GDBGetDomainName(ICodedValueDomainPtr ipCodedValueDomain, VARIANT vaVal, std::string *cName)
{
	HRESULT hr;
	const char *func = "GDBGetDomainName";

	*cName = "";

	long aCodeCount = 0;
	if (FAILED(hr = ipCodedValueDomain->get_CodeCount(&aCodeCount)))
		return GDBERROR(hr, func);

	for (long i = 0; i < aCodeCount; i++)
	{
		CComVariant vaIndex;
		if (FAILED(hr = ipCodedValueDomain->get_Value(i, &vaIndex)))
			return GDBERROR(hr, func);
		if (vaIndex == vaVal)
		{
			CComBSTR bstrName;
			if (FAILED(hr = ipCodedValueDomain->get_Name(i, &bstrName)))
				return GDBERROR(hr, func);
			USES_CONVERSION;
/*
			char buff[BUFSIZ];
			long s = wcstombs(buff, OLE2W(bstrName), BUFSIZ);
			buff[s] = '\0';
			*cName = buff;
*/
			*cName = OLE2T(bstrName);
			break;
		}
	}

	return S_OK;
}

HRESULT GDBGetDomainName(_IRowPtr ipRow, const std::string &cColumn, std::string *cName)
{
	HRESULT hr;
	const char *func = "GDBGetDomainName";
/*
	CComVariant vaVal;
	if (FAILED(hr = GDBGetValue(ipRow, cColumn, &vaVal)))
		return hr;
	IFieldsPtr ipFields = NULL;
	if (FAILED(hr = ipRow->get_Fields(&ipFields)))
		return GDBERROR(hr, func);
	long aIndex = 0;
	if (FAILED(hr = ipFields->FindField(CComBSTR(_T(cColumn.c_str())), &aIndex)))
		return GDBERROR(hr, func);
	IFieldPtr ipField = NULL;
	if (FAILED(hr = ipFields->get_Field(aIndex, &ipField)))
		return GDBERROR(hr, func);
	IDomainPtr ipDomain = NULL;
	if (FAILED(hr = ipField->get_Domain(&ipDomain)))
		return GDBERROR(hr, func);
	if (ipDomain == NULL)
	{
		vaVal.ChangeType(VT_BSTR);
		USES_CONVERSION;
		*cName = OLE2T(vaVal.bstrVal);
	}
	else
	{
		ICodedValueDomainPtr ipCodedValueDomain(ipDomain);
		ATLASSERT(ipCodedValueDomain != NULL);
		if (FAILED(hr = GDBGetDomainName(ipCodedValueDomain, vaVal, cName)))
			return hr;
	}
*/
	ICodedValueDomainPtr ipCodedValueDomain = NULL;
	if (FAILED(hr = GDBGetCodedValueDomain(ipRow, cColumn, &ipCodedValueDomain)))
		return hr;
	CComVariant vaVal;
	if (FAILED(hr = GDBGetValue(ipRow, cColumn, &vaVal)))
		return hr;

	if (ipCodedValueDomain == NULL)
	{
		vaVal.ChangeType(VT_BSTR);
		USES_CONVERSION;
		*cName = OLE2T(vaVal.bstrVal);
	}
	else
	{
		if (FAILED(hr = GDBGetDomainName(ipCodedValueDomain, vaVal, cName)))
			return hr;
	}

	return S_OK;
}

HRESULT GDBGetDomainName(IFeaturePtr ipFeature, const std::string &cColumn, std::string *cName)
{
	return GDBGetDomainName((_IRowPtr)ipFeature, cColumn, cName);
}

HRESULT GDBGetCodedValueDomain(_IRowPtr ipRow, const std::string &cColumn, ICodedValueDomain **ipCodedValueDomain)
{
	HRESULT hr;
	const char *func = "GDBGetDomainName";

	*ipCodedValueDomain = NULL;

	CComVariant vaVal;
	if (FAILED(hr = GDBGetValue(ipRow, cColumn, &vaVal)))
		return hr;
	IFieldsPtr ipFields = NULL;
	if (FAILED(hr = ipRow->get_Fields(&ipFields)))
		return GDBERROR(hr, func);
	long aIndex = 0;
	if (FAILED(hr = ipFields->FindField(CComBSTR(_T(cColumn.c_str())), &aIndex)))
		return GDBERROR(hr, func);
	IFieldPtr ipField = NULL;
	if (FAILED(hr = ipFields->get_Field(aIndex, &ipField)))
		return GDBERROR(hr, func);
	IDomainPtr ipDomain = NULL;
	if (FAILED(hr = ipField->get_Domain(&ipDomain)))
		return GDBERROR(hr, func);
	if (ipDomain == NULL) return S_OK;

	*ipCodedValueDomain = (ICodedValueDomainPtr)ipDomain;
	(*ipCodedValueDomain)->AddRef();

	return S_OK;
}

HRESULT GDBGetCodedValueDomain(IFeaturePtr ipFeature, const std::string &cColumn, ICodedValueDomain **ipCodedValueDomain)
{
	return GDBGetCodedValueDomain((_IRowPtr)ipFeature, cColumn, ipCodedValueDomain);
}

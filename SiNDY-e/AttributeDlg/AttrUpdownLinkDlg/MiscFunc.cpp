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

#include "stdafx.h"
#include "MiscFunc.h"
#include <ArcHelperEx/ArcHelperEx.h>

using namespace sindy::schema;

bool miscfunc::GetCodedValueDomainMap(ITablePtr ipTable, const CString& fieldName, std::map<long, CString>& valueDomainMap)
{
	// フィールド取得できなければ空文字返す
	IFieldPtr ipField;
	AheGetFieldByFieldName(ipTable, fieldName, &ipField);
	if(!ipField) return false;

	// フィールドがコード値ドメインでなければ空文字返す
	IDomainPtr ipDomain;
	ipField->get_Domain(&ipDomain);
	ICodedValueDomainPtr ipCodedValueDomain(ipDomain);
	if(!ipCodedValueDomain)	return false;

	// ドメイン名称を巡回する
	long domainNameCount = 0;
	ipCodedValueDomain->get_CodeCount(&domainNameCount);
	for(long i=0; i<domainNameCount; ++i){
		CComVariant domainValue;
		CComBSTR domainName;
		ipCodedValueDomain->get_Value(i, &domainValue);
		ipCodedValueDomain->get_Name(i, &domainName);
		valueDomainMap[domainValue.lVal] = CString(domainName);
	}

	return true;
}

bool miscfunc::GetFieldType(const IFieldsPtr & fields, const CString & field_name, esriFieldType& field_type)
{
	// フィールドのタイプを取得
	long idx = AheFindField(fields, field_name);
	if (idx == -1) return false;

	IFieldPtr field;
	fields->get_Field(idx, &field);
	if (!field)	return false;

	field->get_Type(&field_type);

	return true;
}

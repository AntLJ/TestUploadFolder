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

// AnnoConv.cpp: AnnoConv クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AnnoConv.h"

#define SINDY_ATTR "CLASS"
#define ATTR_NAME "CLASS_NAME"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AnnoConv::AnnoConv()
{

}

AnnoConv::~AnnoConv()
{
	m_Table.clear();
}

bool AnnoConv::init(IFeatureClassPtr  ipAnno)
{
#if 1
	//ドメインから取得するように変更
	IFieldsPtr ipFields;
	ipAnno->get_Fields(&ipFields);
	long aIndex = -1;
	ipFields->FindField(CComBSTR("ANNOCLASS_C"), &aIndex);
	IFieldPtr ipField;
	ipFields->get_Field(aIndex, &ipField);
	IDomainPtr ipDomain;
	ipField->get_Domain(&ipDomain);
	ICodedValueDomainPtr ipAnnoDomain = ipDomain;
	long aCount = 0;
	ipAnnoDomain->get_CodeCount(&aCount);
	for(int i = 0; i < aCount; ++i){
		CComVariant aVal;
		CComBSTR aName;
		ipAnnoDomain->get_Value(i, &aVal);
		ipAnnoDomain->get_Name(i, &aName);
		m_Table.insert(ANNO_TABLE::value_type(aVal.lVal, CString(aName)));
	}
	return true;
#else
	long aSindyIndex, aNameIndex;

	if(FAILED(ipTable->FindField(CComBSTR(_T(SINDY_ATTR)),&aSindyIndex))){
		fprintf(stderr, "SiNDY属性フィールド取得失敗。\n");
		return false;
	}
	if(aSindyIndex < 0){
		fprintf(stderr, "SiNDY属性フィールド取得失敗。\n");
		return false;
	}
	if(FAILED(ipTable->FindField(CComBSTR(_T(ATTR_NAME)),&aNameIndex))){
		fprintf(stderr, "属性名称フィールド取得失敗。\n");
		return false;
	}
	if(aNameIndex < 0){
		fprintf(stderr, "属性名称フィールド取得失敗。\n");
		return false;
	}

	/// 検索
	ICursorPtr ipCursor;
	IRowPtr ipRow;
	long aSindyAttr;
	CString aAttrName;
	int aCount = 0;

	if(SUCCEEDED(ipTable->Search( NULL, VARIANT_FALSE, &ipCursor ))){
		while(ipCursor->NextRow( &ipRow ) == S_OK){
			CComVariant vaValue;
			ipRow->get_Value(aSindyIndex, &vaValue);
			aSindyAttr = vaValue.lVal;
			CComVariant vaValue2;
			ipRow->get_Value(aNameIndex, &vaValue2);
			aAttrName = vaValue2.bstrVal;
			m_Table.insert(ANNO_TABLE::value_type(aSindyAttr, aAttrName));
			aCount++;
		}
	}else{
		fprintf(stderr, "注記属性テーブル検索失敗。\n");
		return false;
	}
	if(aCount > 0){
		return true;
	}else{
		fprintf(stderr, "注記対応テーブルの該当データがありません。\n");
		return false;
	}
#endif
}

LPCTSTR AnnoConv::getAttrName(LONG cSindyAttr)
{
	ANNO_TABLE::iterator it;
	LPCTSTR aAttrName = "";
	it = m_Table.find(cSindyAttr);
	if(it != m_Table.end()){
		aAttrName = (*it).second;
	}else{
		fprintf(stderr, "対応する属性が見つからない。%d\n", cSindyAttr);
	}
	return aAttrName;
}

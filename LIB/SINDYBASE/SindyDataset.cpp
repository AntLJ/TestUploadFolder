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

// SindyDataset.cpp: SindyDataset クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyDataset.h"


SindyDataset::SindyDataset(SindyDBConnection *cDBConnection) :
GDBOBJLIST(),
mpDBConnection(cDBConnection),
ipDataset(NULL)
{

}

SindyDataset::~SindyDataset()
{

}

HRESULT SindyDataset::GetName(std::string *cName)
{
	HRESULT hr;
	const char *func = "SindyDataset::GetName";

	*cName = "";

	CComBSTR bstrName;
	IObjectClassPtr ipObjectClass(ipDataset);
	ATLASSERT(ipObjectClass != NULL);
	if (FAILED(hr = ipObjectClass->get_AliasName(&bstrName))) return hr;
	USES_CONVERSION;
	*cName = OLE2T(bstrName);

	return S_OK;
}

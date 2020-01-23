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

// SindyMeshFeature.cpp: SindyMeshFeature クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyMeshFeature.h"

SindyMeshFeature::SindyMeshFeature(SindyDBConnection *cDBConnection, const std::string &cName, const std::string &cMeshFieldName) :
SindyFeatureClass(cDBConnection, cName),
mMeshFieldName(cMeshFieldName)
{

}

SindyMeshFeature::~SindyMeshFeature()
{

}

HRESULT SindyMeshFeature::Load()
{
	HRESULT hr;
	const char *func = "SindyMeshFeature::Load";

	if (! is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	IFeatureCursorPtr ipFeatureCursor = NULL;
	if (FAILED(hr = GDBSearchByWhereClause((IFeatureClassPtr)ipDataset, std::string(""), &ipFeatureCursor)))
		return hr;

	IFeaturePtr ipFeature = NULL;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK && ipFeature != NULL)
	{
		long aOID;
		if (FAILED(hr = ipFeature->get_OID(&aOID)))
			return GDBERROR(hr, func);
		insert(GDBOBJSET(aOID, ipFeature));

		CComVariant vaMesh;
		if (FAILED(hr = GDBGetValue(ipFeature, mMeshFieldName, &vaMesh)))
			return hr;
		vaMesh.ChangeType(VT_I4);
		mMeshList.insert(GDBOBJSET(vaMesh.lVal, ipFeature));
	}

	return S_OK;
}

HRESULT SindyMeshFeature::SearchByMeshCode(long cMeshCode, IFeature **ipFeature)
{
	*ipFeature = NULL;

	GDBOBJITER aIter = mMeshList.find(cMeshCode);
	if (aIter != mMeshList.end())
	{
		(*ipFeature) = (IFeaturePtr)aIter->second;
		(*ipFeature)->AddRef();
	}

	return S_OK;
}


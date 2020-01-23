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

// SindyFeatureClass.cpp: SindyFeatureClass クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyFeatureClass.h"


SindyFeatureClass::SindyFeatureClass(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyDataset(cDBConnection)
{
	mpDBConnection->OpenFeatureClass(cName, (IFeatureClass **)&ipDataset);
}

SindyFeatureClass::~SindyFeatureClass()
{

}

HRESULT SindyFeatureClass::SearchByObjectID(long aID, IFeature **ipFeature)
{
	GDBOBJITER aIter = find(aID);
	if (aIter == end())
	{
		*ipFeature = NULL;
	}
	else
	{
		(*ipFeature) = (IFeaturePtr)aIter->second;
		(*ipFeature)->AddRef();
	}
	return S_OK;
}

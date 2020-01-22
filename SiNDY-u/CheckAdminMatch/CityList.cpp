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

// CityList.cpp: CityList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CityList.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CityList::CityList()
{
}

CityList::~CityList()
{
}

bool CityList::
Open(const char *cFileName)
{
	FILE	*mCityListFP;		///< 市区町村コードリストのファイルポインタ

	char aBuffer[1000], aCityCodeTmp[12];

	mCityListFP = fopen(cFileName, "r");
	if(mCityListFP == NULL) {
		fprintf(stderr, "[AddrList]オープンできません: %s\n", cFileName);
		return false;
	}

	while( fgets(aBuffer, 1000, mCityListFP) != NULL )
	{
		strncpy(aCityCodeTmp, aBuffer, 11);
		CityListCont	aCLCont(aCityCodeTmp);
		insert(aCLCont);
	}

	if(!feof(mCityListFP)) 
	{
		fprintf(stderr, "[AddrList]読み込みエラー\n");
		fclose(mCityListFP);
		return false;
	}

	if(mCityListFP != NULL)	fclose(mCityListFP);

	return true;

}

bool CityList::
Add(const char *cCityCode)
{

	if(strlen(cCityCode) != 5 && strlen(cCityCode) != 11) return false;
	if(strspn(cCityCode, "0123456789") != strlen(cCityCode)) return false;

	CityListCont	aCLCont(cCityCode);
	insert(aCLCont);

	return true;

}

bool CityList::
Add(const LONG cCityCode)
{

	if(cCityCode < 0 || 99999999999 < cCityCode) return false;

	char	aCityCodeStr[11];
	sprintf(aCityCodeStr, "%11d", cCityCode);

	CityListCont	aCLCont(aCityCodeStr);
	insert(aCLCont);

	return true;

}

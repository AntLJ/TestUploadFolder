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

#include "CityList.h"
#include "CountGouPoint.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CityList::CityList()
{

	mCityListFP	= NULL;

}

CityList::~CityList()
{

	if(mCityListFP != NULL)	fclose(mCityListFP);

}

bool CityList::
Open(char *cFileName)
{

	char aBuffer[1000], aCityCodeTmp[12];

	mCityListFP = fopen(cFileName, "r");
	if(mCityListFP == NULL) {
		fprintf(stderr, "[AddrList]オープンできません: %s\n", cFileName);
		return false;
	}

	while(1) {

		if(fgets(aBuffer, 1000, mCityListFP) == NULL) {
			if(feof(mCityListFP)) break;
			else {
				fprintf(stderr, "[AddrList]読み込みエラー\n");
				return false;
			}
		}

		strncpy(aCityCodeTmp, aBuffer, 11);
		CityListCont	aCLCont(aCityCodeTmp);
		insert(aCLCont);

	}

	return true;

}

bool CityList::
Add(char *cCityCode)
{
	
	if(strlen(cCityCode) < 5 ) return false;
	if(strspn(cCityCode, "0123456789") != strlen(cCityCode)) return false;

	CityListCont	aCLCont(cCityCode);
	insert(aCLCont);

	return true;

}

bool CityList::
Add(LONG cCityCode)
{

	if(cCityCode < 0 || 99999999999 < cCityCode) return false;

	char	aCityCodeStr[11];
	sprintf(aCityCodeStr, "%11d", cCityCode);

	CityListCont	aCLCont(aCityCodeStr);
	insert(aCLCont);

	return true;

}

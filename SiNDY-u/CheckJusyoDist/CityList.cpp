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
#include "StdAfx.h"
#include "CityList.h"
#include <TDC/useful_headers/text_util.h>
#include <TDC/useful_headers/directory_util.h>

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
Open(const char *cFileName)
{
	if(!uh::isFile(cFileName))
	{
		fprintf(stderr, "[AddrList]ファイルが存在しないか、ファイルが指定されていません: %s\n", cFileName);
		return false;
	}

	auto& list = uh::text_util::ToTStrList( cFileName );

	if(list.size() == 0)
	{
		fprintf(stderr, "[AddrList]読み込みエラー: %s\n", cFileName);
		return false;
	}

	for (const auto& line : list)
	{
		CityListCont aCLCont(line.c_str());
		insert(aCLCont);
	}

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

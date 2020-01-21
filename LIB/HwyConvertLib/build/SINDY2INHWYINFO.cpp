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

// SINDY2INHWYINFO.cpp: SINDY2INHWYINFO クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2INHWYINFO.h"


SINDY2INHWYINFO::SINDY2INHWYINFO(SindyDBConnection *cDBConnection) :
SindyHighwayInfo(cDBConnection)
{

}

SINDY2INHWYINFO::~SINDY2INHWYINFO()
{

}

HRESULT SINDY2INHWYINFO::Load(bool cRoadInfoLoad, const std::string &cHwyNodeWhereCause)
{
	return SindyHighwayInfo::Load(cRoadInfoLoad, cHwyNodeWhereCause);
}

void PrintError(_IRowPtr ipRow, const char *cMessage)
{
	SindyErrorInfo aErrorInfo(ipRow, cMessage);

	IFieldsPtr ipFields = NULL;
	ipRow->get_Fields(&ipFields);
	long aIndex;
	ipFields->FindField(CComBSTR(_T(sindy::hwy_mode::HWYMODE_F.c_str())), &aIndex);
	if (aIndex < 0) aErrorInfo.AddComment(std::string("HWY"));
	else
	{
		CComVariant vaHwyMode;
		GDBGetValue(ipRow, sindy::hwy_mode::HWYMODE_F, &vaHwyMode);
		vaHwyMode.ChangeType(VT_I4);
		if (vaHwyMode.lVal) aErrorInfo.AddComment(std::string("HWY"));
		else aErrorInfo.AddComment(std::string("非HWY"));
	}

	std::cout << aErrorInfo;
}

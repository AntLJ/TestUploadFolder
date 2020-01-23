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

// SindyTable.cpp: SindyTable クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyTable.h"


SindyTable::SindyTable(SindyDBConnection *cDBConnection, const std::string &cName) :
SindyDataset(cDBConnection)
{
	mpDBConnection->OpenTable(cName, (ITable **)&ipDataset);
}

SindyTable::~SindyTable()
{

}

HRESULT SindyTable::SearchByObjectID(long aID, _IRow **ipRow)
{
	GDBOBJITER aIter = find(aID);
	if (aIter == end())
	{
		*ipRow = NULL;
	}
	else
	{
		(*ipRow) = aIter->second;
		(*ipRow)->AddRef();
	}
	return S_OK;
}

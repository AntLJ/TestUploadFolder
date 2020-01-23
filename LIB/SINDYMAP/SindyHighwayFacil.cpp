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

// SindyHighwayFacil.cpp: SindyHighwayFacil クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyHighwayFacil.h"


SindyHighwayFacil::SindyHighwayFacil() :
eRoadCode(0),
eRoadSeq(0)
{

}

SindyHighwayFacil::SindyHighwayFacil(long cRoadCode, long cRoadSeq) :
eRoadCode(cRoadCode),
eRoadSeq(cRoadSeq)
{

}

SindyHighwayFacil::~SindyHighwayFacil()
{

}

bool SindyHighwayFacil::operator ==(const SindyHighwayFacil &cFacil)
{
	return cFacil.eRoadCode == eRoadCode && cFacil.eRoadSeq == eRoadSeq;
}

HRESULT SindyHighwayFacil::SetFacil(_IRowPtr ipRow)
{
	HRESULT hr;
	const char *func = "SindyHighwayFacil::SetFacil";

	CComVariant vaRoadCode, vaRoadSeq;
	if (FAILED(hr = GDBGetValue(ipRow, sindy::hwy_facil_code::ROAD_CODE, &vaRoadCode)))
		return hr;
	if (FAILED(hr = GDBGetValue(ipRow, sindy::hwy_facil_code::ROAD_SEQ, &vaRoadSeq)))
		return hr;
	vaRoadCode.ChangeType(VT_I4); vaRoadSeq.ChangeType(VT_I4);

	eRoadCode = vaRoadCode.lVal;
	eRoadSeq  = vaRoadSeq.lVal;

	return S_OK;
}

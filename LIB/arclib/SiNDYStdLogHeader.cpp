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

#include "stdafx.h"
#include "SiNDYStdLogHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// StdLogHeader
//////////////////////////////////////////////////////////////////////

StdLogHeader::StdLogHeader()
{
	Builder()(*this);
}

//////////////////////////////////////////////////////////////////////
// StdLogHeader::Builder
//////////////////////////////////////////////////////////////////////

const LPCTSTR StdLogHeader::Builder::m_rgszColumn[] = {
	_T("FLAG{対応状況}"),
	_T("LAYER{レイヤ名}"),
	_T("OBJECTID{オブジェクトID}"),
	_T("LONGITUDE{X(経度)}"),
	_T("LATITUDE{Y(緯度)}"),
	_T("エラーレベル"),
	_T("エラーコード"),
	_T("エラーメッセージ"),
};

void StdLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader)
{
	for(int i = 0; i < sizeof(m_rgszColumn) / sizeof(LPCTSTR); i++) {
		rLogHeader.setColumnHeader(m_rgszColumn[i], i);
	}
}

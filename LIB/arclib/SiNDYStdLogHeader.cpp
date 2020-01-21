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
	_T("FLAG{�Ή���}"),
	_T("LAYER{���C����}"),
	_T("OBJECTID{�I�u�W�F�N�gID}"),
	_T("LONGITUDE{X(�o�x)}"),
	_T("LATITUDE{Y(�ܓx)}"),
	_T("�G���[���x��"),
	_T("�G���[�R�[�h"),
	_T("�G���[���b�Z�[�W"),
};

void StdLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader)
{
	for(int i = 0; i < sizeof(m_rgszColumn) / sizeof(LPCTSTR); i++) {
		rLogHeader.setColumnHeader(m_rgszColumn[i], i);
	}
}

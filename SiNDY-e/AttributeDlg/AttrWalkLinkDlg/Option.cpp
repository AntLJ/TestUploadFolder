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
#include "Option.h"
#include <TDC/useful_headers/registry_util.h>

using namespace sindy::schema;

namespace reg_key{
	const CString path = REGPATH_SINDY_E_CHILDDLG _T("WalkLink");
	const CString BuddySearchRange = _T("BuddySearchRange");
}


COption::COption(void) :
	m_SearchRange(0)
{
}


COption::~COption(void)
{
}

void COption::Init()
{
	using namespace reg_key;

	// デフォルト：5
	m_SearchRange = uh::reg_util::read( BuddySearchRange, path, 5 );
}

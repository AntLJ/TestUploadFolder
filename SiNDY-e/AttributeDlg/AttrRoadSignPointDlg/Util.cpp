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
#include "Util.h"

bool util::partialMatch(const CString & target_str, const CString & search_word)
{
	std::wstring searchStr = CT2CW(search_word);
	std::wstring codeDiscription = CT2CW(target_str);
	std::wstring pattern = (boost::wformat(L".*%s.*") % searchStr).str();
	std::wregex regexPattern(pattern);
	if (std::regex_match(codeDiscription, regexPattern)) {
		return true;
	}
	return false;
}

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

#pragma once
#include <map>
/**
 * @brief ACCコード定義クラス
 */
class AccTable
{
public:
	AccTable(void){};
	~AccTable(void){};

	static StringMasterTbl initCodes()
	{
		std::map< long , std::pair<CString, CString> > codeMap;
		codeMap[1] = std::pair<CString, CString>(_T("1S"), _T("都市地図レベル　家形"));
		codeMap[2] = std::pair<CString, CString>(_T("3A"), _T("都市地図レベル　敷地"));
		codeMap[3] = std::pair<CString, CString>(_T("2A"), _T("都市地図レベル"));
		codeMap[4] = std::pair<CString, CString>(_T("1A"), _T("中縮レベル　家形・敷地・注記"));
		codeMap[5] = std::pair<CString, CString>(_T("1B"), _T("中縮レベル　確認済"));
		codeMap[6] = std::pair<CString, CString>(_T("1C"), _T("中縮レベル　確認不能"));
		codeMap[7] = std::pair<CString, CString>(_T("UK"), _T("他社基準ＸＹ"));
		
		return codeMap;
	}

	static StringMasterTbl codes;
};

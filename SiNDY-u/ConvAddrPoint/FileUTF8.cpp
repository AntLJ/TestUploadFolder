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
#include <codecvt>
#include "LogSys.h"
#include "FileUTF8.h"


CFileUTF8::CFileUTF8(void)
{
}


CFileUTF8::~CFileUTF8(void)
{
	close();
}

bool CFileUTF8::open(const CString& strDir, const CString& strFile)
{
	CString filename = strDir + _T("\\") + strFile;
	m_ofsFile.open(filename, std::ios::trunc | std::ios::binary);
	if (!m_ofsFile.is_open()) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errOpenFile, _T("ファイルオープン失敗 [%s]"), filename);
		return false;
	}
	m_ofsFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
	return true;
}

void CFileUTF8::close(void)
{
	if (m_ofsFile.is_open())
		m_ofsFile.close();
}

void CFileUTF8::write(const std::wstring& strMsg)
{
	std::wstring strUTF8 = UNICODEToUTF8(strMsg.c_str());
	m_ofsFile << strUTF8 << "\r\n";
}

std::wstring CFileUTF8::UNICODEToUTF8(const CString& src) const
{
	CT2W strUtf8(src, CP_UTF8);
	return std::wstring(strUtf8);
}
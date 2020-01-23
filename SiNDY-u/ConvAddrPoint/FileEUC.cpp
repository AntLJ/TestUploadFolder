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
#include <WKF_LIB/libwkf.h>
#include "LogSys.h"
#include "FileEUC.h"

CFileEUC::CFileEUC(void)
{
}

CFileEUC::~CFileEUC(void)
{
	close();
}

bool CFileEUC::open(const CString& strDir, const CString& strFile)
{
	CString filename = strDir + _T("\\") + strFile;
	m_ofsFile.open(filename, std::ios::trunc | std::ios::binary);
	if (!m_ofsFile.is_open()) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errOpenFile, _T("ファイルオープン失敗 [%s]"), filename);
		return false;
	}
	return true;
}

void CFileEUC::close(void)
{
	if (m_ofsFile.is_open())
		m_ofsFile.close();
}

void CFileEUC::write(const std::string& strMsg)
{
	std::string strEUC = ConvSJIS2EUC(strMsg);
	m_ofsFile << strEUC << "\n";
}

std::string CFileEUC::ConvSJIS2EUC(const std::string& str) const
{
	char szRet[4096] = { 0 };
	wkfConvertKanjiCodeOfString(KC_SJIS, const_cast<char *>(str.c_str()), KC_EUC, szRet, sizeof(szRet));
	return szRet;
}

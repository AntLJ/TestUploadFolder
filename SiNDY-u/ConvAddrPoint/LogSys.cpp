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
#include "LogSys.h"

CLogSys::CLogSys(void)
{
}


CLogSys::~CLogSys(void)
{
}


bool CLogSys::SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile)
{
	ofsLogFile->open(cFilePath, std::ios::out);
	if (!ofsLogFile->is_open()) {
		std::cerr << "ERROR\tログファイルのオープンに失敗しました : " << CT2CA(cFilePath) << std::endl;
		return false;
	}
	return true;
}

bool CLogSys::SetRunLog(const _TCHAR *cFilePath)
{
	return SetLogFile(cFilePath, &m_ofsRunLog);
}

bool CLogSys::SetErrLog(const _TCHAR *cFilePath)
{
	bool bRet = SetLogFile(cFilePath, &m_ofsErrLog);
	if (!bRet)
		return bRet;

	CLogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
	CLogSys::GetInstance().WriteErrLog(false, false, _T("ERRORLEVEL\tTABLE\tOBJECTID\tERRCODE\tERRMSG\n"));

	return bRet;
}

std::string CLogSys::GetTimeText(bool bSplit, bool bUtc) const
{
	SYSTEMTIME st;
	if (bUtc) GetSystemTime(&st);
	else GetLocalTime(&st);
	std::string ret;
	if (bSplit) {
		ret = boost::str(boost::format("%04d/%02d/%02d %02d:%02d:%02d.%03d")
			% st.wYear % st.wMonth % st.wDay % st.wHour % st.wMinute % st.wSecond % st.wMilliseconds);
	}
	else {
		ret = boost::str(boost::format("%4d-%02d-%02d %02d:%02d:%02d.%03d")
			% st.wYear % st.wMonth % st.wDay % st.wHour % st.wMinute % st.wSecond % st.wMilliseconds);
	}
	return ret;
}

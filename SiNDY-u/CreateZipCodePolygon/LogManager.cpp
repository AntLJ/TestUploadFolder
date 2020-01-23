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
#include "LogManager.h"
#include <boost/assign.hpp>
#include <TDC/useful_headers/str_util.h>

LogManager::LogManager(void)
{
}

LogManager::~LogManager(void)
{
}

bool LogManager::OpenRunLog(const CString& path)
{
	return OpenLog(path, m_ofsRunLog);
}

bool LogManager::OpenErrLog(const CString& path)
{
	return OpenLog(path, m_ofsErrLog);
}

void LogManager::WriteRunLog(const CString& message)
{
	WriteLog(message, m_ofsRunLog);
}

void LogManager::WriteErrComment(const CString& comment)
{
	WriteLog(_T("#")+comment, m_ofsErrLog);
}

void LogManager::WriteErrLog(
	const CString& layer, long objectId,
	ErrorLevel level, const CString& message,
	const CString& info1, const CString& info2)
{
	CString errLevel;
	switch(level)
	{
	case Info: errLevel=_T("INFO"); break;
	case Warning: errLevel=_T("WARNING"); break;
	case Error: errLevel=_T("ERROR"); break;
	case Fatal: errLevel=_T("FATAL"); break;
	default: break;
	}

	std::vector<CString> errLog = boost::assign::list_of
		(_T(""))
		(layer)
		(uh::str_util::format(_T("%ld"), objectId))
		(_T(""))
		(_T(""))
		(errLevel)
		(_T(""))
		(message)
		(info1)
		(info2);
	CString joinLog = uh::str_util::join(errLog, _T("\t") );
	WriteLog(joinLog, m_ofsErrLog);
}

bool LogManager::OpenLog(const CString& path, std::ofstream& ofs)
{
	ofs.open(path);
	return ofs.is_open();
}

void LogManager::WriteLog(const CString& message, std::ofstream& ofs)
{
	ofs << (CStringA)message << std::endl;
}

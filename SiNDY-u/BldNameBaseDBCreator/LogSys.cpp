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

#include "LogSys.h"


LogSys::LogSys(void)
{
}


LogSys::~LogSys(void)
{
}

bool LogSys::SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile)
{
	ofsLogFile->open(cFilePath, std::ios::app);
	if (! ofsLogFile->is_open()){
		std::cerr << cFilePath << "‚ÌƒI[ƒvƒ“‚É¸”s" << std::endl;
		return false;
	}
	return true;
}

bool LogSys::SetProcLog(const _TCHAR *cFilePath)
{
	return SetLogFile(cFilePath, &m_ofsProcLog);
}

bool LogSys::SetErrLog(const _TCHAR *cFilePath)
{
	return SetLogFile(cFilePath, &m_ofsErrLog);
}

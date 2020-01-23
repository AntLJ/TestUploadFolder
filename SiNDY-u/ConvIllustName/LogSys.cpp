
#include "stdafx.h"
#include "LogSys.h"


LogSys::LogSys(void)
{
}


LogSys::~LogSys(void)
{
}

bool LogSys::SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile)
{
	ofsLogFile->open(cFilePath, std::ios::trunc);
	if (! ofsLogFile->is_open()){
		std::cerr << CT2CA(cFilePath) << "‚ÌƒI[ƒvƒ“‚É¸”s" << std::endl;
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

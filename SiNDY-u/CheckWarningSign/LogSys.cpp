#include "LogSys.h"


LogSys::LogSys(void)
{
}


LogSys::~LogSys(void)
{
}

bool LogSys::setLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile)
{
	ofsLogFile->open(cFilePath, std::ios::app);
	if (! ofsLogFile->is_open()){
		std::cerr << CT2CA(cFilePath) << "‚ÌƒI[ƒvƒ“‚É¸”s" << std::endl;
		return false;
	}
	return true;
}

bool LogSys::setProcLog(const _TCHAR *cFilePath)
{
	return setLogFile(cFilePath, &m_ofsProcLog);
}

bool LogSys::setErrLog(const _TCHAR *cFilePath)
{
	return setLogFile(cFilePath, &m_ofsErrLog);
}

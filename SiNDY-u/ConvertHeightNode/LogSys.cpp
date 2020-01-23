#include "stdafx.h"
#include <atltime.h>
#include "LogSys.h"

namespace log_utility
{
	// “úŽž‚Ìî•ñ‚ð•¶Žš—ñ‚Å•Ô‚·
	CString GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return strTime;
	}
}


LogSys::LogSys(void)
{
}


LogSys::~LogSys(void)
{
}

bool LogSys::SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile)
{
	ofsLogFile->open(cFilePath);
	if (! ofsLogFile->is_open()){
		std::cerr << CT2CA(cFilePath) << "‚ÌƒI[ƒvƒ“‚ÉŽ¸”s" << std::endl;
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


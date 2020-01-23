#pragma once

namespace err_level {
	enum ELevel {
		warning,
		err,
		fatal,
	};
}


namespace log_utility
{
	/**
	 * @brief  “ú‚Ìî•ñ‚ğ•¶š—ñ‚Å•Ô‚·
	 * @return “úî•ñ ( [YYYY-MM-DD HH:MM::SS] )
	 */
	 CString GetDateTimeInfo();
}

class LogSys
{
private:
	LogSys(void);
	~LogSys(void);

	inline void WriteLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		CString curr = log_utility::GetDateTimeInfo();
		if (bCout) {
			if (bTime)
				std::cout << CT2CA(curr) << "\t" << CT2CA(lpszMsg) << std::flush;
			else
				std::cout << CT2CA(lpszMsg) << std::flush;
		}
		if (bCerr) {
			if (bTime)
				std::cerr << CT2CA(curr) << "\t" << CT2CA(lpszMsg) << std::flush;
			else
				std::cerr << CT2CA(lpszMsg) << std::flush;
		}
		if (ofsLog->is_open()){
			if (bTime)
				*ofsLog << CT2CA(curr) << "\t" << CT2CA(lpszMsg) << std::flush;
			else 
				*ofsLog << CT2CA(lpszMsg) << std::flush;
		}
	}

public:

	static LogSys& GetInstance(void){
		static LogSys kInstance;
		return kInstance;
	}


	bool SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile);
	bool SetProcLog(const _TCHAR *cFilePath);
	bool SetErrLog(const _TCHAR *cFilePath);

	inline void WriteProcLog(bool bCout, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsProcLog, bCout, false, bTime, lpszMsg);
	}

	inline void WriteErrLog(bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsErrLog, false, bCerr, bTime, lpszMsg);
	}

	inline void WriteFreeStyleLog(bool bCerr, err_level::ELevel eErrLevel, LPCTSTR lpszErrMsg, LPCTSTR lpszLayer = _T("-"), long lObjectId = -1)
	{
		CString strErrLevel;
		switch(eErrLevel){
		case err_level::warning:
			strErrLevel = _T("WARNING");
			break;
		case err_level::err:
			strErrLevel = _T("ERR");
			break;
		case err_level::fatal:
			strErrLevel = _T("FATAL");
			break;
		default:
			strErrLevel = _T("unknown");
		}
		CString strMsg;
		strMsg.Format(_T("0\t%s\t%ld\t%s\t%s\n"), lpszLayer, lObjectId, strErrLevel, lpszErrMsg);

		WriteErrLog(bCerr, false, strMsg);
	}

private:
	std::ofstream m_ofsProcLog;
	std::ofstream m_ofsErrLog;

};

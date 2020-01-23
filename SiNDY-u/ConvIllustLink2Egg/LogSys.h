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

#include "ctime.h"

namespace err_level {
	enum ELevel {
		warning,
		err,
		fatal,
	};
}

class LogSys
{
private:
	LogSys(void);
	~LogSys(void);

	inline void WriteLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		std::string curr = GetTimeText();
		if (bCout) {
			if (bTime)
				std::cout << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cout << CT2CA(lpszMsg);
		}
		if (bCerr) {
			if (bTime)
				std::cerr << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cerr << CT2CA(lpszMsg);
		}
		if (ofsLog->is_open()){
			if (bTime)
				*ofsLog << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else 
				*ofsLog << CT2CA(lpszMsg);
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


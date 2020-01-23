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

/**
 * @file	LogSys.h
 */

 // 本クラスを使用するには以下のincludeが必要
 // 可能な限り.hでのincludeは避けるべきだが、ほぼ全ての.cppに影響があるため本ヘッダでincludeしてしまう
#include <stdarg.h>

// OBJECTIDをNULLで出力する場合
const long D_LONG_NULL = 0xFFFFFFFF;

namespace err_level {
	enum ELevel {
		fatal,
		err,
		warning,
		info
	};
}

// エラーコード
namespace err_code {
	enum ECode {
		// ArcObjects関連
		errConnectDB   = 1001,
		errOpenTable   = 1002,
		errGetIndex    = 1003,
		errSelectTable = 1004,
		errSelectCount = 1005,
		errGetDomain   = 1006,

		// レコード関連
		errGetValue    = 2001,
		errGetShape    = 2002,
		errGetXY       = 2003,
		errLLtoMesh    = 2004,

		// 出力ファイル関連
		errOpenFile    = 3001
	};
}; // err_code

/**
 * @class	CLogSys
 * @brief	ログクラス
 */
class CLogSys
{
private:
	CLogSys(void);
	~CLogSys(void);

	inline void WriteLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		std::string curr = GetTimeText(true);
		if (bCout) {
			if (bTime)
				std::cout << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg) << std::flush;
			else
				std::cout << CT2CA(lpszMsg) << std::flush;
		}
		if (bCerr) {
			if (bTime)
				std::cerr << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg) << std::flush;
			else
				std::cerr << CT2CA(lpszMsg) << std::flush;
		}
		if (ofsLog->is_open()) {
			if (bTime)
				*ofsLog << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				*ofsLog << CT2CA(lpszMsg);
		}
	}

	bool SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile);
	std::string GetTimeText(bool bSplit, bool bUtc = false) const;

public:

	static CLogSys& GetInstance(void) {
		static CLogSys kInstance;
		return kInstance;
	}

	bool SetRunLog(const _TCHAR *cFilePath);
	bool SetErrLog(const _TCHAR *cFilePath);

	inline void WriteRunLog(bool bCout, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsRunLog, bCout, false, bTime, lpszMsg);
	}

	inline void WriteErrLog(bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsErrLog, false, bCerr, bTime, lpszMsg);
	}

	inline void PrintFreeStyleLog(err_level::ELevel eErrLevel, LPCTSTR lpszTableName, long lObjectID, err_code::ECode eErrCode, LPCTSTR lpszFormat, ...)
	{
		CString strErrLevel;
		switch (eErrLevel) {
		case err_level::fatal:
			strErrLevel = _T("FATAL");
			break;
		case err_level::err:
			strErrLevel = _T("ERROR");
			break;
		case err_level::warning:
			strErrLevel = _T("WARNING");
			break;
		case err_level::info:
			strErrLevel = _T("INFO");
			break;
		default:
			strErrLevel = _T("unknown");
		}
		CString strObjectID;
		if (lObjectID != D_LONG_NULL)
			strObjectID.Format(_T("%ld"), lObjectID);

		CString strErrMsg;
		va_list ap;
		va_start(ap, lpszFormat);
		strErrMsg.FormatV(lpszFormat, ap);
		va_end(ap);

		CString strMsg;
		strMsg.Format(_T("%s\t%s\t%s\t%ld\t%s\n"), strErrLevel, lpszTableName, strObjectID, eErrCode, strErrMsg);

		WriteErrLog(false, false, strMsg);
	}

private:
	std::ofstream m_ofsRunLog;
	std::ofstream m_ofsErrLog;
};

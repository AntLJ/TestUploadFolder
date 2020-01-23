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

#include <WinLib/VersionInfo.h>

/**
 * @brief ツールのバージョンと実行時刻・時間を出力するクラス
 */
class CTimeLog
{
public:
	/**
	 * @brief コンストラクタ。
	 */
	CTimeLog()
	{
		time(&m_st_time);
		// ツールバージョン
		CVersion cVer;
		_ftprintf(stderr, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
		SYSTEMTIME  aTime;
		GetLocalTime(&aTime);
		_ftprintf(stderr, _T("#開始時刻: %04d/%02d/%02d %02d:%02d:%02d\n"),
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);
	};

	/**
	 * @brief デストラクタ。
	 */
	~CTimeLog()
	{
		time(&m_ed_time);
		SYSTEMTIME  aTime;
		GetLocalTime(&aTime);
		_ftprintf(stderr, _T("#終了時刻: %04d/%02d/%02d %02d:%02d:%02d\n"),
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);
		int h,m,s;
		double t = difftime( m_ed_time, m_st_time);
		h = int(t/3600);
		m = (int(t)%3600)/60;
		s = int(t)%60;
		_tprintf(_T("%d時間%d分%d秒\n"), h, m, s);
		_ftprintf(stderr, _T("#トータル時間 %d時間%d分%d秒\n"), h, m, s);
		_tprintf(_T("終了します\n"));
	};
private:
	time_t m_st_time;	///< 開始時刻
	time_t m_ed_time;	///< 終了時刻
};

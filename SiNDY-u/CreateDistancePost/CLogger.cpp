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
#include "CLogger.h"
#include <TDC/useful_headers/str_util.h>
#pragma comment(lib, "version.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy
{
	const TCHAR* log_format = _T("DISTANCE_POST_POINT\t%d\t%s\t%.1f\n");   ///< ログフォーマット

	// ログ初期設定
	void CLogger::initLog(LPCTSTR lpszLogFile, boost::tuple<long,long,CString> pair, LPCTSTR lpcszOpParam)
	{
		if ( _tcslen(lpszLogFile) <= 0 )
			return;

		m_strLogFile.Format( _T("%s\\%06d_%01d_%s.log"), lpszLogFile, pair.get<0>(), pair.get<1>(), pair.get<2>());

		m_stream = _fsopen( CT2CA(m_strLogFile), "w", _SH_DENYWR );

		if ( !m_stream )
			return;

		_ftprintf( m_stream, CreateLogHeader(lpcszOpParam) );
		fflush( m_stream );
	}

	// ログ書き込み
	void CLogger::Log(long nOID, LPCTSTR lpcszMark, double dbVal)
	{
		if ( ! m_stream )
			return;

		CString strLog;

		strLog.Format( log_format, nOID, lpcszMark, dbVal );

		_ftprintf( m_stream, strLog);
		fflush( m_stream );
	}

	// ログ終了処理
	void CLogger::EndLog()
	{
		if ( ! m_stream )
			return;

		fflush( m_stream );
		fclose( m_stream );
		m_stream = nullptr;
	}
}

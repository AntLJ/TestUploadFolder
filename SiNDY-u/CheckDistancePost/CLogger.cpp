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
#include "RowBase.h"
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
	const TCHAR* log_format = _T("0\tDISTANCE_POST_POINT\t%d\t%s\t%.1f\t%d\t%s\n");   ///< ログフォーマット

	// ログ初期設定
	void CLogger::initLog(LPCTSTR lpszLogFile, long nRoadCode, long nDirCode, LPCTSTR lpcszMark, LPCTSTR lpcszOpParam, bool bSetHeader)
	{
		if ( _tcslen(lpszLogFile) > 0 )
		{
			m_strLogFile.Format( _T("%s\\%06d_%01d_%s.log"), lpszLogFile, nRoadCode, nDirCode, lpcszMark);

			m_stream = _fsopen( CT2CA(m_strLogFile), bSetHeader ? "w" : "a", _SH_DENYWR );

			if ( m_stream && bSetHeader )
			{
				CString strHeader( CreateLogHeader(lpcszOpParam) ), strTmp;

				strTmp.Format(_T("#start %s\n"), GetNow());
				strHeader += strTmp;

				strHeader += _T("FLAG\t");
				strHeader += _T("LAYER\t");
				strHeader += _T("OBJECTID\t");
				strHeader += _T("距離標記号\t");
				strHeader += _T("距離標値\t");
				strHeader += _T("エラーコード\t");
				strHeader += _T("エラーメッセージ\n");

				_ftprintf( m_stream, strHeader );
				fflush( m_stream );
			}
		}
	}

	// ログ書き込み
	void CLogger::AddLog(sindy::CSPRowBase pRow, long nCode, LPCTSTR lpcszMsg)
	{
		CString strLog;

		CSPFieldMap pSpFieldMap = pRow->GetFieldMap();
		long nIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistance );
		long nMIdx = pSpFieldMap->FindField( sindy::schema::distance_post_point::kDistanceMark );

		CComVariant vaVal( pRow->GetValue(nIdx) );
		double dbVal = vaVal.fltVal;

		CComVariant vaValM( pRow->GetValue(nMIdx) );

		AddLog(pRow->GetOID(), dbVal, CString(vaValM.bstrVal), nCode, lpcszMsg );
	}

	// ログ書き込み
	void CLogger::AddLog(long nOID, double dbVal, LPCTSTR lpcszMark, long nCode, LPCTSTR lpcszMsg)
	{
		if ( ! m_stream )
			return;

		CString strLog;

		strLog.Format( log_format, nOID, lpcszMark, dbVal, nCode, lpcszMsg );

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

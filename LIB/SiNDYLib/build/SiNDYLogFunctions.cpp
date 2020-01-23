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

/**
 * @file SiNDYLogFunctions.cpp
 * @brief <b>SiNDYでハンドリング可能なログのユーティリティ関数群実装ファイル</b>\n
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "SiNDYLogFunctions.h"
#include "meshutil.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR FREE_STYLE_LOG = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tMESHCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{エラーレベル}\tCOMMENT{エラーコード}\tCOMMENT{エラーメッセージ}\tCOMMENT{備考}\tCOMMENT{フリーコメント}\n");
#else
	LPCTSTR FREE_STYLE_LOG = _T("# FREESTYLELOG\nFLAG\tLAYER\tOBJECTID\tmeshCODE\tX\tY\tLATITUDE\tLONGITUDE\tSCALE\tCOMMENT{error level}\tCOMMENT{error code}\tCOMMENT{error message}\tCOMMENT{note}\tCOMMENT{comment}\n");
#endif // SINDY_FOR_ENGLISH
}

void SINDYLOGHEADER( FILE* out, sindyLogFormat emFormat )
{
	CString strHeader;
	switch( emFormat )
	{
		case sindyLogFormat_Freestyle:
			strHeader= FREE_STYLE_LOG;
			break;
		case sindyLogFormat_Common:
		default:
			strHeader = _T("# SINDYSTDLOG\n");
			break;
	}
	// ファイルに出力
	if( out )
	{
		_ftprintf( out, strHeader );
		fflush( out );
	}
}

// ログハンドルツール標準フォーマット形式でログを出力する
void SINDYCOMMONLOG( FILE* out, sindyLogFormat emFormat, LPCTSTR lpcszTableName, long lOID, const double& dX, const double& dY, long scale, sindyErrLevel emErrLevel, sindyErrCode emErr, LPCTSTR lpcszFormat/* = NULL*/, ... )
{
	// エラーレベル文字列作成
	CString strLevel;
	switch( emErrLevel )
	{
		case sindyErrLevel_INFO: strLevel = _T("INFO"); break;
		case sindyErrLevel_WARN: strLevel = _T("WARN"); break;
		default:                 strLevel = _T("ERR"); break;
	}
	// 出力文字列を作成する
	CString strLog;
	switch( emFormat )
	{
		case sindyLogFormat_Freestyle:
		{
			int x = 0, y = 0;
			unsigned int nMeshCode = LL2MESHcity( dX, dY, x, y );
			strLog.Format(_T("0\t%s\t%d\t%u\t%d\t%d\t%.20lf\t%.20lf\t%d\t%s\t%d\t%s\t"), lpcszTableName, lOID, nMeshCode, x, y, dY, dX, scale, strLevel, emErr, SINDYERRCODE2STRING(emErr) );
			break;
		}
		case sindyLogFormat_Common:
		default:
			strLog.Format(_T("0,%s,%d,%.20lf,%.20lf,%s,%d,%s,"), lpcszTableName, lOID, dX, dY, strLevel, emErr, SINDYERRCODE2STRING(emErr) );
			break;
	}

	if( NULL != lpcszFormat )
	{
		va_list args;
		va_start(args, lpcszFormat);

		int nBuf;
#ifdef _UNICODE
		WCHAR szBuffer[1024];
#else
		char szBuffer[1024];
#endif

		nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpcszFormat, args);
		SASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

		va_end(args);

		strLog += szBuffer;
	}
	strLog += _T("\n");

	// ファイルに出力
	if( out )
	{
		_ftprintf( out, strLog );
		fflush( out );
	}

	// 標準出力に出力
	_tprintf( strLog );
	fflush(stdout);

	// アウトプットウィンドウに出力
#ifdef DEBUG
#ifdef _UNICODE
	OutputDebugStringW(strLog);
#else
	OutputDebugStringA(strLog);
#endif // _UNICODE
#endif // DEBUG
}

} // sindy

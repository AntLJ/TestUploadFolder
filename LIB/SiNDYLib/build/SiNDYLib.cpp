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

// SiNDYLib.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//
#include "stdafx.h"
#include "exportimport.h"
#include "GlobalFiles.h"
#include "SiNDYeSettings.h"
#include "util.h"
#include <stdio.h>
#include <locale.h>
#include "../../LIB/WinLib/crashdump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _LIB

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR LOGFILE_NAMES  = _T("error log file name:%s trace log file name:%s\n");
#else
	LPCTSTR LOGFILE_NAMES  = _T("エラーログファイル名：%s トレースログファイル名：%s\n");
#endif	// SINDY_FOR_ENGLISH
} // namespace

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID/* lpReserved*/
					 )
{
#ifndef _AFXDLL
	_CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif // ifndef _AFXDLL

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			_tsetlocale ( LC_ALL, _T("") ); // VS2005 SP1 でも直ってないなんて…

			// エラーログファイル・トレースログファイルをレジストリから読み込む
			CSiNDYeSettings set;
			set.Load();

			// エラーログファイル
			if( lstrlen( set.GetErrorLogFileName() ) > 0 )
				lstrcpyn( sindy::g_strErrlog, set.GetErrorLogFileName(), _MAX_PATH );
			// トレースログファイル
			if( lstrlen( set.GetTraceLogFileName() ) > 0 )
				lstrcpyn( sindy::g_strTracelog, set.GetTraceLogFileName(), _MAX_PATH );

			// モジュール名を取得
			TCHAR lpszBuffer[_MAX_PATH];
			DWORD dwCount = _MAX_PATH;
			GetModuleFileName( (HMODULE)hModule, lpszBuffer, dwCount );
			TRACEMESSAGE(_T("%s\n"), _T("--------------------------------------------------------------------------------") );
			TRACEMESSAGE( _T("%s : loaded.\n"), lpszBuffer );
			TRACEMESSAGE( LOGFILE_NAMES, sindy::g_strErrlog, sindy::g_strTracelog );

			break;
		}
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
		{
			// モジュール名を取得
			TCHAR lpszBuffer[_MAX_PATH];
			DWORD dwCount = _MAX_PATH;
			GetModuleFileName( (HMODULE)hModule, lpszBuffer, dwCount );
			TRACEMESSAGE( _T("%s : unloading...\n"), lpszBuffer );
			TRACEMESSAGE(_T("%s\n"), _T("--------------------------------------------------------------------------------") );			
			// エラーログファイル、トレースログファイルクローズ
			if( sindy::g_Errlog )
			{
				fflush( sindy::g_Errlog );
				fclose( sindy::g_Errlog );
				sindy::g_Errlog = NULL;
			}
			if( sindy::g_Tracelog )
			{
				fflush( sindy::g_Tracelog );
				fclose( sindy::g_Tracelog );
				sindy::g_Tracelog = NULL;
			}

			break;
		}
		default:
			break;
    }
    return TRUE;
}

#endif // _LIB

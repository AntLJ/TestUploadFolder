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
#include "crashdump.h"

/*-------------------------------------------*/
/*
	クラッシュダンプ作成用
*/
typedef enum _MINIDUMP_TYPE{
	MiniDumpNormal			= 0x0000,
	MiniDumpWithDataSegs	= 0x0001,
	MiniDumpWithFullMemory	= 0x0002,
	MiniDumpWithHandleData	= 0x0004,
	MiniDumpFilterMemory	= 0x0008,
	MiniDumpScanMemory		= 0x0010
}MINIDUMP_TYPE;

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
	DWORD ThreadId;
	PEXCEPTION_POINTERS ExceptionPointers;
	BOOL ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

BOOL (WINAPI *_MiniDumpWriteDump)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, LPVOID, LPVOID) = NULL;

bool miniDump(EXCEPTION_POINTERS *e){
	if(!_MiniDumpWriteDump){
		HMODULE h=GetModuleHandle(_T("dbghelp.dll"));
		if(!h)	h=LoadLibrary(_T("dbghelp.dll"));
		if(!h)	return false;
		_MiniDumpWriteDump=(BOOL (WINAPI *)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION, LPVOID, LPVOID))GetProcAddress(h,"MiniDumpWriteDump");
		if(!_MiniDumpWriteDump)	return false;
	}
	
	MINIDUMP_EXCEPTION_INFORMATION einfo;
	if(e){
		einfo.ThreadId = GetCurrentThreadId();
		einfo.ExceptionPointers = e;
		einfo.ClientPointers = FALSE;
	}
	
	// ダンプファイル名
	CString strDmpFileName;

	TCHAR strUser[64] = _T("");
	DWORD dw = 63;
	GetUserName( strUser, &dw );

	CString strTime;
	SYSTEMTIME stTime = {0};
	GetLocalTime( &stTime );
	strTime.Format(_T("%04d%02d%02d%02d%02d%02d"), stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	strDmpFileName.Format(_T("\\\\win\\tdc\\SiNDY\\SiNDY-e\\dump\\%s_%s.dmp"), strUser, strTime );

	HANDLE hf = 0;
	try {
	hf = CreateFile(strDmpFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	catch(...)
	{
	}
	if( INVALID_HANDLE_VALUE == hf )
	{
		// 作れなかった場合はC:直下
		strDmpFileName.Format(_T("C:\\%s_%s.dmp"), strUser, strTime );
		hf = CreateFile( strDmpFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if( INVALID_HANDLE_VALUE == hf )
			return false;
	}
	
	if(!_MiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hf,
		MiniDumpNormal,
		((e)? &einfo : NULL),
		NULL,
		NULL
	)){
		CloseHandle(hf);
		MessageBox(NULL,_T("ダンプファイルの出力に失敗しました。"), _T("アプリケーション例外"), MB_OK|MB_ICONSTOP );
		return false;
	}

	CloseHandle(hf);

	CString strMsg;
	strMsg.Format(_T("ハンドリングされていない例外をキャッチしました。\n以下フォルダにダンプファイルを出力しました。\n\n%s\n\n申し訳ありませんが、担当者への報告をお願いします。"), strDmpFileName );
	MessageBox( NULL, strMsg, _T("アプリケーション例外"), MB_OK|MB_ICONSTOP );

	return true;
}
/*-------------------------------------------*/

LONG CALLBACK myExceptionHandler(EXCEPTION_POINTERS *e){
	miniDump(e);
	return EXCEPTION_CONTINUE_SEARCH;	/* デフォルトのハンドラにDispatch */
}

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

// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef STRICT
#define STRICT
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600	// これを IE. の他のバージョン向けに適切な値に変更してください。
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_USE_CSTRING_FLOAT
#define _ATL_NO_COM

#define _WTL_NO_THEME_DELAYLOAD
#define _WTL_NO_WTYPES
#define _WTL_USE_CSTRING

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#pragma warning(disable:4005)
#pragma warning(disable:4018)
#pragma warning(disable:4049)
#pragma warning(disable:4091)
#pragma warning(disable:4100)
#pragma warning(disable:4146)
#pragma warning(disable:4192)
#pragma warning(disable:4231)
#pragma warning(disable:4244)
#pragma warning(disable:4245)
#pragma warning(disable:4278)
#pragma warning(disable:4310)
#pragma warning(disable:4336)
#pragma warning(disable:4503)
#pragma warning(disable:4509)
#pragma warning(disable:4663)
#pragma warning(disable:4786)
#pragma warning(disable:4800)

#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

#include <locale.h>

#ifdef _USE_HASH_MAP
	#include <hash_map>
	#if _MSC_VER >= 1400
		using namespace stdext;
	#endif
#endif
#include <map>
#include <vector>
using namespace std;

#include <oci.h>

enum enumociDataType {
	DT_UNKNOWN = 0,
	DT_TEXT,
	DT_LONG,
	DT_DOUBLE,
	DT_DATE,
	DT_RECORDSET,
};

#define	FETCH_SIZE				8192
#define	CHAR_SIZE				8192
#define	FIRST_COLUMN			1
#define	FIRST_PARAMETER			1
#define NUMBER_FORMAT1			_T("FM9999999999999999999999990.9999999999999999999999999999999999999")
#define NUMBER_FORMAT2			_T("FM99999999999999999999999999999999999990.999999999999999999999999")
#define NUMBER_FORMAT2_DECIMAL	(sizeof(_T("999999999999999999999999")) - 1)
#define NUMBER_FORMAT_INT		_T("FM99999999999999999999999999999999999990")
#define NLS_PARAM				_T("NLS_NUMERIC_CHARACTERS='.,' NLS_ISO_CURRENCY='JAPAN'")

#define NEW(p,s,t)				p = new t[s+sizeof(t)]; \
								ZeroMemory( p, s * sizeof(t) );

#ifdef _UNICODE
	#define MtoW(ptsz,s,size)	size = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)s, -1, NULL, 0 ); \
								ZeroMemory( ptsz, size * sizeof(TCHAR) ); \
								MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)s, -1, ptsz, size );

	#define WtoM(ptsz,s,size)	size = WideCharToMultiByte( CP_ACP, 0, s, -1, NULL, NULL, NULL, NULL ); \
								ZeroMemory( ptsz, size * sizeof(char) ); \
								WideCharToMultiByte( CP_ACP, 0, s, -1, ptsz, size, NULL, NULL );

	#define MtoW2(ptsz,s,size)	size = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)s, -1, NULL, 0 ); \
								NEW( ptsz, size, TCHAR ); \
								MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)s, -1, ptsz, size );

	#define WtoM2(ptsz,s,size)	size = WideCharToMultiByte( CP_ACP, 0, s, -1, NULL, NULL, NULL, NULL ); \
								NEW( ptsz, size, char ); \
								WideCharToMultiByte( CP_ACP, 0, s, -1, ptsz, size, NULL, NULL );
#else
	#define MtoW(ptsz,s,size)	if ( s != NULL ) { \
									size = strlen(s); \
									ZeroMemory( ptsz, size * sizeof(TCHAR) ); \
									strcpy( ptsz, (LPCSTR)s ); \
								}

	#define WtoM(ptsz,s,size)	if ( s != NULL ) { \
									size = _tcslen(s); \
									ZeroMemory( ptsz, size * sizeof(char) ); \
									_tcscpy( ptsz, s ); \
								}

	#define MtoW2(ptsz,s,size)	if ( s != NULL ) { \
									size = strlen(s); \
									NEW( ptsz, size, TCHAR ); \
									strcpy( ptsz, (LPCSTR)s ); \
								}

	#define WtoM2(ptsz,s,size)	if ( s != NULL ) { \
									size = _tcslen(s); \
									NEW( ptsz, size, char ); \
									_tcscpy( ptsz, s ); \
								}
#endif


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
#include "time_convert.h"
#include <time.h>

// time_t -> FILETIME
void UnixTimeToFileTime(time_t t, LPFILETIME pft)
{
	// [MSDN] Win32 FILETIME ‚Ü‚½‚Í SYSTEMTIME ‚É UNIX time_t ‚ð•ÏŠ·‚·‚é•û–@
	// Note that LONGLONG is a 64-bit value
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = (DWORD)(ll >> 32);
}

// time_t -> SYSTEMTIME
void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst)
{
	FILETIME ft;

	// [MSDN] Win32 FILETIME ‚Ü‚½‚Í SYSTEMTIME ‚É UNIX time_t ‚ð•ÏŠ·‚·‚é•û–@
	UnixTimeToFileTime(t, &ft);
	FileTimeToSystemTime(&ft, pst);
}

// FILETIME -> time_t •ÏŠ·
time_t FileTimeToUnixTime(const FILETIME& ft)
{
    LONGLONG ll;
    ll = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
    return (time_t)((ll - 116444736000000000) / 10000000);
}

// SYSTEMTIME -> time_t •ÏŠ·
time_t SystemTimeToUnixTime(const SYSTEMTIME& st){
    FILETIME ft;
    ::SystemTimeToFileTime(&st, &ft);
    return FileTimeToUnixTime(ft);
}

// tm -> DATE
DATE tmToVariantTime( tm& stTime )
{
	return UnixTimeToVariantTime( _mkgmtime(&stTime) );
}

// DATE -> tm
void VariantTimeTotm( const DATE& dDate, tm& stTime )
{
	time_t tt = VariantTimeToUnixTime( dDate );
	gmtime_s( &stTime, &tt );
}

// time_t -> DATE
DATE UnixTimeToVariantTime( time_t tm )
{
	DATE dRet;

	// tm -> SYSTEMTIME
	SYSTEMTIME sys;
	UnixTimeToSystemTime( tm, &sys );
	SystemTimeToVariantTime( &sys, &dRet );
	return dRet;
}

// DATE -> time_t
time_t VariantTimeToUnixTime( const DATE& dDate )
{
	// DATE -> SYSTEMTIME
	SYSTEMTIME sys;
	VariantTimeToSystemTime( dDate, &sys );
	// SYSTEMTIME -> st
	return SystemTimeToUnixTime( sys );
}


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

/// time_t -> FILETIME
void UnixTimeToFileTime(time_t t, LPFILETIME pft);

// time_t -> SYSTEMTIME
void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst);

/// SYSTEMTIME -> time_t
time_t SystemTimeToUnixTime(const SYSTEMTIME& st);

/// tm -> DATE
DATE tmToVariantTime( tm& stTime );

/// DATE -> tm
void VariantTimeTotm( const DATE& dDate, tm& stTime );

// FILETIME -> time_t •ÏŠ·
time_t FileTimeToUnixTime(const FILETIME& ft);

/// time_t -> DATE
DATE UnixTimeToVariantTime( time_t tm );

// DATE -> time_t
time_t VariantTimeToUnixTime( const DATE& dDate );


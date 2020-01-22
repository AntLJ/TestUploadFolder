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
#include "stdafx.h"

/**
 * @brief	現在時刻取得用
 * @retval	CString  「YYYY/MM/DD hh:mm:ss」 
 */
inline CString GetCurTime()
{
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strTime;
	strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
};

/**
 * @brief	現在日付取得用
 * @retval	CString  「YYYYMMDD」
 */
inline CString GetCurDay()
{
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strDay;
	strDay.Format( _T("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay );
	return strDay;
};

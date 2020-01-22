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
 * @brief	���ݎ����擾�p
 * @retval	CString  �uYYYY/MM/DD hh:mm:ss�v 
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
 * @brief	���ݓ��t�擾�p
 * @retval	CString  �uYYYYMMDD�v
 */
inline CString GetCurDay()
{
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strDay;
	strDay.Format( _T("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay );
	return strDay;
};

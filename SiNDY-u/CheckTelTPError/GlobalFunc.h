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

/**
 * @file	GlobalFunc.h
 * @brief	グローバル関数定義
 */

namespace gf
{
	/**
	 * @brief	現在時刻を文字列で取得
	 * @return	文字列(yyyy/mm/dd hh/mm/ss)
	 */
	inline CString	GetCurDateStr()
	{
		SYSTEMTIME	st;
		::GetLocalTime( &st );

		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

		return strTime;
	}

	/**
	 * @brief	実行アカウント名取得
	 * @return	アカウント名
	 */
	inline CString GetCurAccount()
	{
		_TCHAR szUser[20] = _T("");
		DWORD dwSize = 19;
		::GetUserName( szUser, &dwSize );
		return CString(szUser);
	}

	/**
	 * @brief	IPoint → メッシュＸＹ
	 * @param	ipPoint		[in]	ポイント
	 * @param	pMXY		[out]	メッシュXY
	 * @return	bool
	 */
	bool ConvPoint2MXY( IPointPtr ipPoint, CMeshXY* pMXY );

	/**
	 * @brief	メッシュXY → IPoint
	 * @param	cMXY	[in]	メッシュXY
	 * @return	IPoint
	 */
	IPointPtr CreateMXYPoint( const CMeshXY& cMXY );
}

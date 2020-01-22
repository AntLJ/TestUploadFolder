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
 * @brief	�O���[�o���֐���`
 */

namespace gf
{
	/**
	 * @brief	���ݎ����𕶎���Ŏ擾
	 * @return	������(yyyy/mm/dd hh/mm/ss)
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
	 * @brief	���s�A�J�E���g���擾
	 * @return	�A�J�E���g��
	 */
	inline CString GetCurAccount()
	{
		_TCHAR szUser[20] = _T("");
		DWORD dwSize = 19;
		::GetUserName( szUser, &dwSize );
		return CString(szUser);
	}

	/**
	 * @brief	IPoint �� ���b�V���w�x
	 * @param	ipPoint		[in]	�|�C���g
	 * @param	pMXY		[out]	���b�V��XY
	 * @return	bool
	 */
	bool ConvPoint2MXY( IPointPtr ipPoint, CMeshXY* pMXY );

	/**
	 * @brief	���b�V��XY �� IPoint
	 * @param	cMXY	[in]	���b�V��XY
	 * @return	IPoint
	 */
	IPointPtr CreateMXYPoint( const CMeshXY& cMXY );
}

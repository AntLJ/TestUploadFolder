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

#ifndef __ADDRGLOBALS_H__
#define __ADDRGLOBALS_H__

#include "CityAdmin.h"
#include "GouPoint.h"

namespace addr
{
	/**
	 * @brief	PGDB���[�h?
	 * @note	���ϐ� PGDB_MODE �ɉ������ݒ肳��Ă�����n�j
	 * @return	bool
	 */
	inline bool IsPGDBMode(){ return CString().GetEnvironmentVariable(_T("PGDB_MODE")) != FALSE; }

	/**
	 * @brief	�����݂̂ō\�����ꂽ������H
	 * @param	lpcszData	[in]	�`�F�b�N�Ώە�����
	 * @return	bool
	 */
	bool IsNumString( const _TCHAR* lpcszData );

	/**
	 * @brief	�ʏ�n��?
	 * @note	���l�݂̂ō\������Ă��� 0 �` 99999 �͈͓̔������`�F�b�N
	 * @param	lpcszData	[in]	�n�ԃf�[�^
	 * @return	bool
	 */
	inline bool IsNormalPnt( const _TCHAR* lpcszData){ return IsNumString(lpcszData) && (0 <= _tstol(lpcszData) && _tstol(lpcszData) <= 99999); }

	/**
	 * @brief	�ʏ퍆?
	 * @note	���l�݂̂ō\������Ă��� 0 �` 9999 �͈͓̔������`�F�b�N
 	 * @param	lpcszData	[in]	���f�[�^
	 * @return	bool
	 */
	inline bool IsNormalGou( const _TCHAR* lpcszData){ return IsNumString(lpcszData) && (0 <= _tstol(lpcszData) && _tstol(lpcszData) <= 9999); }

	/**
	 * @brief	���p�p������S�p��
	 * @param	lpcszData	[in]	�f�[�^
	 * @return	�ϊ��f�[�^(���s������A���̂܂ܕԂ�)
	 */
	inline const _TCHAR* GetZenkakuData( const _TCHAR* lpcszData )
	{
		static TCHAR szConvData[100] = _T("");
		ZeroMemory( szConvData, sizeof(szConvData) );
		int nRet = ::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszData, _tcslen(lpcszData), szConvData, sizeof(szConvData) );
		return nRet? szConvData : lpcszData;
	}

}

#endif // __ADDRGLOBALS_H__

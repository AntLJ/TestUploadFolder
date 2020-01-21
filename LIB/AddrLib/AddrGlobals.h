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
	 * @brief	PGDBモード?
	 * @note	環境変数 PGDB_MODE に何かが設定されていたらＯＫ
	 * @return	bool
	 */
	inline bool IsPGDBMode(){ return CString().GetEnvironmentVariable(_T("PGDB_MODE")) != FALSE; }

	/**
	 * @brief	数字のみで構成された文字列？
	 * @param	lpcszData	[in]	チェック対象文字列
	 * @return	bool
	 */
	bool IsNumString( const _TCHAR* lpcszData );

	/**
	 * @brief	通常地番?
	 * @note	数値のみで構成されていて 0 ～ 99999 の範囲内かをチェック
	 * @param	lpcszData	[in]	地番データ
	 * @return	bool
	 */
	inline bool IsNormalPnt( const _TCHAR* lpcszData){ return IsNumString(lpcszData) && (0 <= _tstol(lpcszData) && _tstol(lpcszData) <= 99999); }

	/**
	 * @brief	通常号?
	 * @note	数値のみで構成されていて 0 ～ 9999 の範囲内かをチェック
 	 * @param	lpcszData	[in]	号データ
	 * @return	bool
	 */
	inline bool IsNormalGou( const _TCHAR* lpcszData){ return IsNumString(lpcszData) && (0 <= _tstol(lpcszData) && _tstol(lpcszData) <= 9999); }

	/**
	 * @brief	半角英数字を全角化
	 * @param	lpcszData	[in]	データ
	 * @return	変換データ(失敗したら、そのまま返す)
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

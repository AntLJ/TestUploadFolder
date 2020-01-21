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

#include "enum_def.h"
#include "JusyoHelper.h"

namespace addr
{

/**
 * @class	CAreaCodeHelper
 * @brief	Areacode_{Master|Kana}へのアクセスヘルパークラス
 */
class CAreaCodeHelper : public CJusyoHelper
{
public:
	CAreaCodeHelper(void){}
	~CAreaCodeHelper(void){}

	//@{ @name 住所コード関連
	/**
	 * @brief	<b>最新住所コード取得</b>
	 * @note	ストアドファンクションGetNewestCodeをラップしたもの
 	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @return	最新住所コード(取得失敗時は、NULL)
	 */
	CString GetNewestCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	<b>旧住所コード取得(合併前等の)</b>
	 * @note	ストアドファンクションGetPrefCodeをラップしたもの
	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @return	旧住所コード(取得失敗時は、NULL)
	 */
	CString GetPrevCode( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	<b>住所コードステータス返す(施行、廃止、無効)</b>
	 * @param	lpcszAddrCode	[in]	11桁住所コード
	 * @return	コードステータス
	 */
	addrCodeStatus GetCodeStatus( const _TCHAR* lpcszAddrCode );
	//@}

	//@{ @name 住所名称関連
	/**
	 * @brief	住所名取得
	 * @noet	住所コード桁数に応じて名称を取得できます
	 * @param	lpcszAddrCode	[in]	住所コード(2, 5, 8, 11桁指定)
	 * @return	住所名称(取得失敗時は、NULL)
	 */
	CString GetAddrKanji( const _TCHAR* lpcszAddrCode );

	/**
	 * @brief	住所カナ取得
	 * @noet	住所コード桁数に応じてカナを取得できます
	 * @param	lpcszAddrCode	[in]	住所コード(2, 5, 8, 11桁指定)
	 * @return	住所カナ(取得失敗時は、NULL)
	 */
	CString GetAddrKana( const _TCHAR* lpcszAddrCode );
	//@}

	//@{ @name その他情報取得関連
	/**
	 * @brief	郵便番号取得
	 * @param	lpcszAddrCode	[in]	住所コード
	 * @return	郵便番号コード(取得失敗時は、NULL)
	 */
	CString GetZipCode( const _TCHAR* lpcszAddrCode );
	//@}
};

extern CAreaCodeHelper	g_cAreaCodeHelper;

}	// namespace addr

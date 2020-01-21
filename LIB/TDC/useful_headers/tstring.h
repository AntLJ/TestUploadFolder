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

#include <string>

namespace uh
{
	/**
	 * @brief TCHAR対応std::string
	 */
	typedef std::basic_string<TCHAR> tstring;

	/**
	 * @brief ワイド文字列をstd::stringに変換する
	 * @note	TCHAR*におけるCT2A()のように、
	 *			uh::tstringに対して常にマルチバイト文字列を返すことを目的とする関数である。
	 * @param	str [in]	ワイド文字列
	 * @return	std::string文字列
	 */
	inline std::string toStr(const std::wstring& wstr)
	{
		return std::string(CW2A(wstr.c_str()));
	}

	/**
	 * @brief マルチバイト文字列をstd::stringにする
	 * @note	TCHAR*におけるCT2A()のように、
	 *			uh::tstringに対して常にマルチバイト文字列を返すことを目的とする関数である。
	 * @param	str [in]	マルチバイト文字列
	 * @return	std::string文字列
	 */
	inline std::string toStr(const std::string& str)
	{
		return str;
	}

	/**
	 * @brief ワイド文字列をstd::wstringにする
	 * @note	TCHAR*におけるCT2W()のように、
	 *			uh::tstringに対して常にワイド文字列を返すことを目的とする関数である。
	 * @param	wstr [in]	ワイド文字列
	 * @return	std::wstring文字列
	 */
	inline std::wstring toWStr(const std::wstring& wstr)
	{
		return wstr;
	}

	/**
	 * @brief マルチバイト文字列をstd::wstringに変換する
	 * @note	TCHAR*におけるCT2W()のように、
	 *			uh::tstringに対して常にワイド文字列を返すことを目的とする関数である。
	 * @param	str [in]	マルチバイト文字列
	 * @return	std::wstring文字列
	 */
	inline std::wstring toWStr(const std::string& str)
	{
		return std::wstring(CA2W(str.c_str()));
	}

	/**
	 * @brief ワイド文字列をtstringに変換する
	 * @note	TCHAR*におけるCW2T()のように、
	 *			常にtstring文字列を返すことを目的とする関数である。
	 * @param	wstr [in]	ワイド文字列
	 * @return	tstring文字列
	 */
	inline tstring toTStr(const std::wstring& wstr)
	{
		return tstring(CW2T(wstr.c_str()));
	}

	/**
	 * @brief マルチバイト文字列をtstringに変換する
	 * @note	TCHAR*におけるCA2T()のように、
	 *			常にtstring文字列を返すことを目的とする関数である。
	 * @param	str [in]	マルチバイト文字列
	 * @return	tstring文字列
	 */
	inline tstring toTStr(const std::string& str)
	{
		return tstring(CA2T(str.c_str()));
	}
}

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
 * @brief ユーティリティ関数群
 * @note  現状は文字列関係のみ
 */
namespace util
{
	/**
	 * @brief   指定文字列が整数を表しているか否かを判定する
	 * @param	str  入力文字列
	 * @retval	整数である
	 * @retval	整数ではない
	 */
	bool isInteger(const std::string& str);
	bool isInteger(const CString& str);
	bool isInteger(const CComVariant& val);

	/**
	* @brief   指定文字列が浮動小数点を表しているか否かを判定する
	* @param	str  入力文字列
	* @retval	浮動小数点である
	* @retval	浮動小数点ではない
	*/
	bool isFloat(const std::string& str);
	bool isFloat(const CString& str);
	bool isFloat(const CComVariant& val);

	/**
	 * @brief   VARIANT型の変数タイプが数値であるか
	 * @param	val  判定対象
	 * @retval true  数値である
	 * @retval false 数値でない
	 */
	bool isNumericType(const CComVariant& val);

	/**
	 * @brief   VARIANT型の変数タイプが整数であるか
	 * @param	val  判定対象
	 * @retval true  数値である
	 * @retval false 数値でない
	 */
	bool isIntegerType(const CComVariant& val);
}

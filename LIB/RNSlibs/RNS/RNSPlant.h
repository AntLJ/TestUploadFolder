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

// RNSPlant.h: RNSPlant クラスのインターフェイス
//
// $Id: RNSPlant.h,v 1.5 2003/10/09 06:56:47 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RNSPLANT_H__FCED50F1_A521_455F_AA0D_50E565101206__INCLUDED_)
#define AFX_RNSPLANT_H__FCED50F1_A521_455F_AA0D_50E565101206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RNS.h"

#include <iostream>
#include <string>

struct RNSPlant  
{
public:

	/// ディレクトリ変更
	static bool changeDirectoryToFile(const char* cFileName);

	/// フルパスからファイル名の開始位置を取得する
	static const char* fileNameOf(const char* cFullPath);

	/// ファイル名と行番号を合成して、ソース位置特定識別文字列を作成する
	static std::string makeSourceLocation(const char* cFileName, int cLine);

	/// デバッグメッセージ出力用文字列を作成する
	static std::string stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg);

	/// 標準エラー出力にメッセージを出力する
	static void trace(std::ostream& cStream, const char* cMsg);

	/**
	 * @brief	無効値で例外を発生させる
	 *
	 * @param	cArg			[in]	検証対象
	 * @param	cFileName		[in]	ファイル名
	 * @param	cLine			[in]	行番号
	 * @param	cInvalidMessage	[in]	無効値エラーメッセージ
	 */
	template <typename T>
	static T valid(T cArg, const char* cFileName, int cLine, const char* cInvalidMessage = "invalid value")
	{
		if(! cArg)
			throw std::runtime_error(RNSPlant::stringForDebugMessage(cFileName, cLine, cInvalidMessage));

		return cArg;
	}

	/**
	 * @brief	0の場合例外を発生させる
	 *
	 * @param	cArg			[in]	検証対象
	 * @param	cFileName		[in]	ファイル名
	 * @param	cLine			[in]	行番号
	 * @param	cInvalidMessage	[in]	無効値エラーメッセージ
	 */
	template <typename T>
	static T nonZero(T cArg, const char* cFileName, int cLine, const char* cInvalidMessage = "invalid value")
	{
		if(cArg == 0)
			throw std::runtime_error(RNSPlant::stringForDebugMessage(cFileName, cLine, cInvalidMessage));

		return cArg;
	}

	/**
	 * @brief	NULLポインタで例外を発生させる
	 *
	 * @param	cPointer		[in]	検証対象ポインタ
	 * @param	cFileName		[in]	ファイル名
	 * @param	cLine			[in]	行番号
	 * @param	cInvalidMessage	[in]	NULLポインタだった場合のエラーメッセージ
	 */
	template <typename T>
	static T* valid_pointer(T* cPointer, const char* cFileName, int cLine, const char* cMessage = "NULL pointer found")
	{
		return nonZero(cPointer, cFileName, cLine, cMessage);
	}
};

/// RNSPlant.h の 仕様に置き換えるため、定義を解除する
#ifdef RNS_LOCATION
#undef RNS_LOCATION
#endif

/**
 * @brief	RNSPlant::makeSourceLocation() を使用してソース位置特定識別文字列を作成する
 *
 * @return	ソース位置特定識別文字列
 */
#define RNS_LOCATION_STRING	RNSPlant::makeSourceLocation(__FILE__, __LINE__)

/**
 * @brief RNS_LOCATION_STRING を使用してソース位置特定識別文字列を作成し、それをC文字列に変換する
 *
 * RNS.h で定義されている RNS_LOCATION に比べると、だいぶ遅い。
 * また文字列は即座にデストラクトされてしまうため、継続的に使用する場合はコピーする必要がある。
 *
 * @return ソース位置特定識別文字列
 */
#define RNS_LOCATION		RNS_LOCATION_STRING.c_str()

#define RNSTRACE(msg)	RNSPlant::trace(std::cerr, msg)
#define RNSTRACEEX(msg)	RNSPlant::trace(std::cerr, (RNS_LOCAION_STRING += msg).c_str())

#define RNS_DEBUG_STRING(msg)	RNSPlant::stringForDebugMessage(__FILE__, __LINE__, msg)
#define RNS_DEBUG_CSTR(msg)		RNSPlant::stringForDebugMessage(__FILE__, __LINE__, msg).c_str()

#define NONZERO(e)			RNSPlant::nonZero(e,  __FILE__, __LINE__)
#define VALID(e)			RNSPlant::valid(e, __FILE__, __LINE__)
#define VALID_POINTER(e)	RNSPlant::valid_pointer(e, __FILE__, __LINE__)

#endif // !defined(AFX_RNSPLANT_H__FCED50F1_A521_455F_AA0D_50E565101206__INCLUDED_)

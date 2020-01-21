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

// DRMADevPlant.cpp: DRMADevPlant クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <sstream>
#include "DRMADevPlant.h"

#ifdef _WIN32
const char drma::dev_plant::_pathChr = '\\';
#endif

//////////////////////////////////////////////////////////////////////
// 検索
//////////////////////////////////////////////////////////////////////

/**
 * @brief	フルパスからファイル名の開始位置を取得する
 *
 * @param	cFullPath	[in]	フルパス
 * @return	フルパスに存在する最後尾のセパレータから1byte後ろの文字へのポインタを返す。\n
 *			セパレータが存在しない場合は、cFullPathをそのまま返す。
 */
const char* drma::dev_plant::fileNameOf(const char* cFullPath)
{
	return strrchr(cFullPath, _pathChr) ? (strrchr(cFullPath, _pathChr) + 1) : cFullPath;
}

/**
 * @brief	ファイル名と行番号を合成して、ソース位置特定識別文字列を作成する
 *
 * @note	書式は"[ファイル名(行番号)]"
 *
 * @param	cFileName	[in]	ファイル名
 * @param	cLine		[in]	行番号
 * @return	ソース位置識別文字列
 */
std::string drma::dev_plant::makeSourceLocation(const char* cFileName, int cLine)
{
	std::stringstream	aStream;
	aStream << "[" << fileNameOf(cFileName) << "(" << cLine << ")]";

	return aStream.str();
}

/**
 * @brief	デバッグ用メッセージを出力する
 *
 * @note	書式は"[ファイル名(行番号)] メッセージ"
 *
 * @param	cFileName	[in]	ファイル名
 * @param	cLine		[in]	行番号
 * @param	cMsg		[in]	メッセージ
 * @return	ソース位置識別文字列+メッセージ
 */
std::string drma::dev_plant::stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg)
{
	std::string aResult(makeSourceLocation(cFileName, cLine));
	aResult += " ";
	aResult += cMsg;
	return aResult;
}

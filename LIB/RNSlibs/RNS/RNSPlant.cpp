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

// RNSPlant.cpp: RNSPlant クラスのインプリメンテーション
//
// $Id: RNSPlant.cpp,v 1.4 2003/05/16 09:37:38 f_kiku Exp $
//////////////////////////////////////////////////////////////////////

#include "RNS.h"
#include <direct.h>
#include <sstream>
#include "RNSPlant.h"

//////////////////////////////////////////////////////////////////////
// 走査
//////////////////////////////////////////////////////////////////////

/**
 * @brief	カレントディレクトリを変更する
 *
 * @param	cFileName	[in]	ファイル名
 * @retval	true	成功
 * @retval	false	失敗
 */
bool RNSPlant::changeDirectoryToFile(const char* cFileName)
{
	char* pPathEnd = 0;

	if((pPathEnd = strrchr((char*)cFileName, _pathChr)) != 0) {
		char	aPath[256];
		strncpy(aPath, cFileName, pPathEnd - cFileName);
		aPath[pPathEnd - cFileName] = 0;
		if(::chdir(aPath))
			return false;
	}

	return true;
}

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
const char* RNSPlant::fileNameOf(const char* cFullPath)
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
std::string RNSPlant::makeSourceLocation(const char* cFileName, int cLine)
{
	std::stringstream	aStream;
	aStream << "[" << fileNameOf(cFileName) << "(" << cLine << ")]";

	return aStream.str();
}

/**
 * @brief	デバッグ用メッセージを作成して返す
 *
 * @note	書式は"[<DM>ファイル名(行番号)] メッセージ"
 *
 * @param	cFileName	[in]	ファイル名
 * @param	cLine		[in]	行番号
 * @param	cMsg		[in]	メッセージ
 * @return	ソース位置識別文字列+メッセージ
 */
std::string RNSPlant::stringForDebugMessage(const char* cFileName, int cLine, const char* cMsg)
{
	std::string aResult(makeSourceLocation(cFileName, cLine));
	aResult += " ";
	aResult += cMsg;
	return aResult;
}

void RNSPlant::trace(std::ostream& cStream, const char* cMsg)
{
	cStream << cMsg << std::endl;
}

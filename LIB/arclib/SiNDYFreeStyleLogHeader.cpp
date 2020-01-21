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

#include "stdafx.h"
#include "SiNDYFreeStyleLogHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// FreeStyleLogHeader
//////////////////////////////////////////////////////////////////////

FreeStyleLogHeader::FreeStyleLogHeader()
{
}

FreeStyleLogHeader::FreeStyleLogHeader(std::istream& rStream)
{
	Builder()(*this, rStream);
}

FreeStyleLogHeader::FreeStyleLogHeader(std::wistream& rStream)
{
	Builder()(*this, rStream);
}
FreeStyleLogHeader::FreeStyleLogHeader(LPCTSTR lpszHeaderLine)
{
	Builder()(*this, lpszHeaderLine);
}

//////////////////////////////////////////////////////////////////////
// FreeStyleLogHeader::Builder
//////////////////////////////////////////////////////////////////////

/**
 * @brief ストリームを走査してヘッダ行を探し、自由形式ログヘッダを構築する
 *
 * @param rLogHeader [in] 構築対象自由形式ログヘッダ。
 * @param rStream [in] ログ入力ストリーム。
 * @retval true 構築成功。ストリームは、ヘッダ行の次の行の先頭を指す。
 * @retval false ヘッダ行が存在しない。ストリームはEOFを指す。
 */
bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, std::istream& rStream)
{
	char szLine[2048];
	while(rStream.getline(szLine, 2048)) {
		if(operator()(rLogHeader, szLine))
			return true;
	}

	return false;
}

bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, std::wistream& rStream)
{
	wchar_t szLine[2048];
	while(rStream.getline(szLine, 2048)) {
		if(operator()(rLogHeader, szLine))
			return true;
	}

	return false;
}

/**
 * @brief ヘッダ行文字列を指定し、自由形式ログヘッダを構築する（char版）
 *
 * @param rLogHeader [in] 構築対象自由形式ログヘッダ。
 * @param lpszHeaderLine [in] ヘッダ行文字列。
 * @retval true 構築成功。
 * @retval false ヘッダ行としては認められない。
 */
bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, LPCSTR lpszHeaderLine)
{
	// 空行、コメント行はヘッダに成り得ない
	if(! lpszHeaderLine || lpszHeaderLine[0] == 0 || lpszHeaderLine[0] == '#')
		return false;

	// それ以外はヘッダとして認める
	USES_CONVERSION;
	rLogHeader.setHeader(A2CT(lpszHeaderLine), FreeStyleLogHeader::seps());

	return true;
}

/**
 * @brief ヘッダ行文字列を指定し、自由形式ログヘッダを構築する（whar_t版）
 *
 * @param rLogHeader [in] 構築対象自由形式ログヘッダ。
 * @param lpszHeaderLine [in] ヘッダ行文字列。
 * @retval true 構築成功。
 * @retval false ヘッダ行としては認められない。
 */
bool FreeStyleLogHeader::Builder::operator()(SiNDYLogHeader& rLogHeader, LPCWSTR lpszHeaderLine)
{
	// 空行、コメント行はヘッダに成り得ない
	if(! lpszHeaderLine || lpszHeaderLine[0] == 0 || lpszHeaderLine[0] == '#')
		return false;

	// それ以外はヘッダとして認める
	USES_CONVERSION;
	rLogHeader.setHeader(W2CT(lpszHeaderLine), FreeStyleLogHeader::seps());

	return true;
}

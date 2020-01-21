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
#include "SiNDYStdLogHeader.h"
#include "SiNDYLogTypeSelector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

#if(0)
//////////////////////////////////////////////////////////////////////
// Reseeker
//////////////////////////////////////////////////////////////////////

/**
 * @brief オブジェクト破棄時にシーク位置を元に戻すクラス
 */
class Reseeker
{
public:
	Reseeker(std::istream& rSteram);
	~Reseeker();

private:
	std::istream& m_rStream;
	std::streampos m_SeekPos;
};

Reseeker::Reseeker(std::istream& rStream) :
m_rStream(rStream),
m_SeekPos(rStream.tellg())
{
}

Reseeker::~Reseeker()
{
	m_rStream.seekg(m_SeekPos, std::ios::beg);
}
#endif

//////////////////////////////////////////////////////////////////////
// SiNDYLogTypeSelector
//////////////////////////////////////////////////////////////////////

/**
 * @brief ログヘッダオブジェクトを生成する
 *
 * ストリームの現在位置から最大５行を読み込み、ヘッダタイプの記述を探す。<br>
 * bRetainSeekPosをfalseにした場合、生成されたログヘッダオブジェクトによって、シーク位置は変動する。
 * - 自由形式ログ : ヘッダ行の次の行の先頭。
 * - 標準形式ログ : ヘッダタイプが記述された行の次の行の先頭。
 * - オブジェクトが生成されなかった : シーク位置は変わらない。
 * 
 * @param rSteram [in] ログ入力ストリーム。
 * @param bRetainSeekPos [in] シーク位置を保持するか。
 * @return ログヘッダオブジェクト。ヘッダタイプが読み取れなかった場合はNULLを返す。
 */
SiNDYLogHeader* LogTypeSelector::newLogHeader(std::istream& rStream, bool bRetainSeekPos)
{
	std::streampos aSeekPos = rStream.tellg();

	USES_CONVERSION;
	LPCSTR lpszFreeStyleLogHeaderId = T2CA(FreeStyleLogHeader::id()); //< 自由形式ログヘッダID。
	LPCSTR lpszStdLogHeaderId = T2CA(StdLogHeader::id()); //< 標準形式ログヘッダID。

	SiNDYLogHeader* pResult = 0;

	char szLine[2048];
	for(int i = 0; i < 5; i++) {
		if(! rStream.getline(szLine, 2048))
			break;

		// トリミング。
		char* psz = szLine + strspn(szLine, " \t\r\n");

		// 空行読み飛ばし。
		if(*psz == 0)
			continue;

		// コメント行以外に辿り着いたら、もはやログ形式は特定できない。
		if(*psz != '#')
			break;

		if(strstr(psz, lpszFreeStyleLogHeaderId)) {
			// 自由形式ログヘッダを返す。
			pResult = new FreeStyleLogHeader(rStream);
			break;
		}
		else if(strstr(psz, lpszStdLogHeaderId)) {
			// 標準形式ログヘッダを返す。
			pResult = new StdLogHeader();
			break;
		}
	}

	if(bRetainSeekPos || ! pResult) {
		rStream.seekg(aSeekPos, std::ios::beg);
	}

	return pResult;
}

SiNDYLogHeader* LogTypeSelector::newLogHeader(std::wistream& rStream, bool bRetainSeekPos)
{
	std::streampos aSeekPos = rStream.tellg();

	USES_CONVERSION;
	LPCWSTR lpszFreeStyleLogHeaderId = T2CW(FreeStyleLogHeader::id()); //< 自由形式ログヘッダID。
	LPCWSTR lpszStdLogHeaderId = T2CW(StdLogHeader::id()); //< 標準形式ログヘッダID。

	SiNDYLogHeader* pResult = 0;

	wchar_t szLine[2048];
	for(int i = 0; i < 5; i++) {
		if(! rStream.getline(szLine, 2048))
			break;

		// トリミング。
		wchar_t* psz = szLine + ::wcsspn(szLine, L" \t\r\n");

		// 空行読み飛ばし。
		if(*psz == 0)
			continue;

		// コメント行以外に辿り着いたら、もはやログ形式は特定できない。
		if(*psz != '#')
			break;

		if(::wcsstr(psz, lpszFreeStyleLogHeaderId)) {
			// 自由形式ログヘッダを返す。
			pResult = new FreeStyleLogHeader(rStream);
			break;
		}
		else if(::wcsstr(psz, lpszStdLogHeaderId)) {
			// 標準形式ログヘッダを返す。
			pResult = new StdLogHeader();
			break;
		}
	}

	if(bRetainSeekPos || ! pResult) {
		rStream.seekg(aSeekPos, std::ios::beg);
	}

	return pResult;
}

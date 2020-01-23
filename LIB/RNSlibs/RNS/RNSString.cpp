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

/* ************************************************
	初稿 :
	改訂 :  
	追記 : 
 
	制作 :  Fukuta.K
************************************************* */

/* ----- インクルードファイル ----- */

#include <iostream>
#include <strstream>

#include "RNSString.h"

using namespace std;

/* ===== public メソッド ===== */

// ----- 初期化 ----- // 

/// コンストラクタ（const char*型文字列代入）
RNSString::RNSString(const char *iStr)
	: string(iStr)
{
}

/// コンストラクタ（string型文字列代入）
RNSString::RNSString(const std::string &iStr)
	: string(iStr)
{
}

/// コンストラクタ（数字指定）
RNSString::RNSString(int iValue)
{
	*this += iValue;
}

/// コンストラクタ（符号なし数字指定）
RNSString::RNSString(unsigned int iValue)
{
	*this += iValue;
}

/// 文字列追加
RNSString& RNSString::append(const RNSString &iStr)
{
	appendPreEffect();
	string::append(iStr);
	appendEpiEffect();
	return *this;
}

/// 指定した数だけ文字を追加する
RNSString& RNSString::append(size_type n, char &iChar)
{
	appendPreEffect(); 
	string::append(n, iChar); 
	appendEpiEffect(); 
	return *this; 
}

// ----- 編集 ----- // 

/// コメント消去（string型）
void RNSString::eraseComment(const std::string& iStr)
{
	int aCommentPos = find(iStr);

	if(aCommentPos != std::string::npos) {
		erase(aCommentPos);
	}
}

/// コメント消去（const char*型）
void RNSString::eraseComment(const char* iStr)
{
	eraseComment(string(iStr));
}

/// トリミング
RNSString RNSString::trimedString(const char* iTrimChars)
{
	int aHeaderNumber = find_first_not_of(iTrimChars);
	int aFooterNumber = find_last_not_of(iTrimChars);

	return RNSString(substr(aHeaderNumber, aFooterNumber));
}

// ----- 取得 ----- // 

/// 文字列分断（string型）
int RNSString::divide(std::vector< RNSRef<RNSString> >* oDivideStrList, const std::string& iDelim, const char* iTrimChars) const
{
	int aBeforePos = 0;
	int aSeparatePos = 0;

	aSeparatePos = find(iDelim, aBeforePos);
	for( ; aSeparatePos != std::string::npos; aSeparatePos = find(iDelim, aBeforePos)) {
		if(iTrimChars) {
			RNSString	aString(substr((aBeforePos, aSeparatePos - aBeforePos)));
			oDivideStrList->push_back(new RNSString(aString.trimedString(iTrimChars)));
		}
		else {
			oDivideStrList->push_back(new RNSString(substr(aBeforePos, aSeparatePos - aBeforePos)));
		}
		aBeforePos = aSeparatePos + 1;
	}

	if(! aBeforePos)
		return 0;

	oDivideStrList->push_back(new RNSString(substr(aBeforePos)));

	return oDivideStrList->size();
}

// ----- オペレータ ----- // 

/// 文字列を代入する
RNSString& RNSString::operator = (const std::string &iStr)
{
	*static_cast<string*>(this) = iStr; 
	return *this;
}

/// 文字列定数を代入する
RNSString& RNSString::operator = (const char *iStr)
{
	*static_cast<string*>(this) = iStr;
	return *this;
}

/// 数値を文字列として追加
RNSString& RNSString::operator += (int iValue)
{
	char		buffer[32];
	strstream	strm(buffer, 32, ios::out);

	strm << iValue << ends;
	append(buffer);

	return *this;
}

/// 符号なし数値を文字列として追加
RNSString& RNSString::operator += (unsigned int iValue)
{
	char		buffer[32];
	strstream	strm( buffer, 32, ios::out );

	strm << iValue << ends;
	append( buffer );

	return *this;
}

/// 文字列を追加する
RNSString& RNSString::operator += (const char* iStr)
{
	append(iStr); 
	return *this; 
}
 
/// 文字列を追加する
RNSString& RNSString::operator += (const RNSString &iStr)
{
	append(iStr); 
	return *this; 
}
 
/// 文字を追加する
RNSString& RNSString::operator += (char iChar )
{
	append(1, iChar); 
	return *this; 
} 

/// 文字列を追加する
RNSString& RNSString::operator << (const RNSString &iStr)
{
	*this += iStr;
	return *this;
}

/// 文字を追加する
RNSString& RNSString::operator << (char iChar)
{
	*this += iChar; 
	return *this;
}

/// 数字を文字として追加する
RNSString& RNSString::operator << (int iValue)
{
	*this += iValue;
	return *this;
}

// ----- その他の関数 ----- // 

/**
 * @brief	ワイド文字列を、対応するマルチバイト文字列に変換する
 *
 * @param	cInput	[in]	変換対象ワイド文字列
 * @return	対応するマルチバイト文字列
 */
const std::string narrow(const std::wstring& cInput)
{
#if _MSC_VER > 1200
	return std::string(CW2A(cInput.c_str()));
#else
	char*	pBuffer = new char[cInput.size() * MB_CUR_MAX + 1];
	wcstombs(pBuffer, cInput.c_str(), cInput.size() * MB_CUR_MAX);
	std::string aResult = pBuffer;
	delete [] pBuffer;
	return aResult;
#endif
}

/**
 * @brief	マルチバイト文字列を、対応するワイド文字列に変換する
 *
 * @param	cInput	[in]	変換対象マルチバイト文字列
 * @return	対応するワイド文字列
 */
const std::wstring widen(const std::string& cInput)
{
	wchar_t*	pBuffer = new wchar_t[cInput.size() + 1];
	mbstowcs(pBuffer, cInput.c_str(), cInput.size());
	std::wstring aResult = pBuffer;
	delete [] pBuffer;
	return aResult;
}

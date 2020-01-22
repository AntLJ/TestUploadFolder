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
#include "stdafx.h"
#include <boost/regex.hpp>

/**
 * @struct	dummy_predicate
 * @brief	regex_grepで使用するためのダミー
 */
struct dummy_predicate {
	bool operator ()(const boost::wsmatch& sm){
		return true;
	}
};

/**
 * @brief	正規表現でOKの文字列となっているか確認
 * @param	str   [in] 文字列	
 * @param	wreg   [in] 確認用正規表現
 * @retval	true  OK
 * @retval	false NG
 */
inline bool IsOKRegEx( const CString& str, const boost::wregex& wreg )
{
	boost::wsmatch	wsm;

	std::wstring strTmp = str;

	if(	boost::regex_search( strTmp, wsm, wreg ) ){
		return true;
	}
	else{
		return false;
	}
}

/**
 * @brief	正規表現で指定したものの個数カウント
 * @param	str  [in] 文字列
 * @param	wreg  [in] 確認用正規表現
 * @retval	long 検出個数
 */
inline long GetRegExCnt( const CString& str, const boost::wregex& wreg )
{
	std::wstring strTmp = str;

	return boost::regex_grep( dummy_predicate(), strTmp, wreg );
}

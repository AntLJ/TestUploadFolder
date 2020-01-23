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

#include <iostream>
#include <algorithm>
#include <filesystem>
#include "tstring.h"

/**
 * @brief パスを扱うUtillityクラス
 */
class PathUtillity
{
public:
	PathUtillity(){};
	~PathUtillity(){};

	/**
	 * @brief  ファイルの拡張子を除いた部分を返却
	 * @param  path  [in]	ファイルパス
	 * @retval
	 */
	static uh::tstring getStem(uh::tstring path);

private:

};

/**
 * @param フォルダかどうか判定する
 * @note FGDBフォルダはフォルダとみなしません
 * @param path [in] 判定パス
 * @return フォルダとみなされるならtrue
 */
bool isDirectory( const uh::tstring& path );

/// 文字列の大文字小文字を無視して比較する
bool eq( const uh::tstring& s1, const uh::tstring& s2 );

inline bool eq( const uh::tstring& s1, const uh::tstring& s2 )
{
	return 0 == _tcsicmp( s1.c_str(), s2.c_str() );
}

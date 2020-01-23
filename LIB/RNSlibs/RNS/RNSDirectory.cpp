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

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <imagehlp.h>
#else
#include <stdlib.h>
#endif



#include <iostream>

#include "RNSDirectory.h"

// ----- コンストラクタ ----- //

/// デフォルトコンストラクタ
RNSDirectory::RNSDirectory()
{
}

/// コンストラクタ（文字列代入）
RNSDirectory::RNSDirectory(const char *iStr)
		: RNSString(iStr)
{
	dirComplete();
}

/// コンストラクタ（文字列代入）
RNSDirectory::RNSDirectory(const std::string &iStr)
		: RNSString(iStr)
{
	dirComplete();
}

/// デストラクタ
RNSDirectory::~RNSDirectory()
{
}

// ----- 操作 ----- //

/// 文字列追加
RNSString& RNSDirectory::append(const RNSString &iStr)
{
	RNSString::append(iStr);
	dirComplete();
	return *this;
}

// ----- 判定 ----- //

/**
 * @brief	ディレクトリの存在確認
 *
 * @retval	true	存在する
 * @retval	false	存在しない
 */
bool RNSDirectory::exists() const
{
	if(empty())
		return false;

	std::string aString = substr(0, size()-1);
	struct stat	aStat;
	return (stat(aString.c_str(), &aStat) == 0 && (aStat.st_mode & _S_IFDIR));
}

/**
 * @brief	ディレクトリ作成
 *
 * @retval	true	作成成功
 * @retval	false	作成失敗
 */
bool RNSDirectory::mkdir() const
{
#ifdef _WIN32
	return (MakeSureDirectoryPathExists(c_str()) == TRUE);
#else
	RNSString	aString("mkdir -p ");
	aString += c_str();
	return (system(aString.c_str()) == 0);
#endif
}

// ----- オペレータ ----- //

/// 文字列を代入する
RNSString& RNSDirectory::operator = (const std::string &iStr)
{
	*static_cast<std::string*>(this) = iStr;
	dirComplete();
	return *this;
}

/// 文字列を代入する
RNSString& RNSDirectory::operator = (const char *iStr)
{
	*static_cast<std::string*>(this) = iStr;
	dirComplete();
	return *this;
}

/**
 * 追加、代入を行った後に行う処理
 *
 * 文字列をディレクトリ名として完成させる
 */
void RNSDirectory::appendEpiEffect()
{
	dirComplete();
}

///	末尾にディレクトリマークを付加
RNSDirectory& RNSDirectory::dirComplete(char iTrMark)
{
	if(empty())
		return *this;

	// ディレクトリ区切りマークじゃない
	if((*this)[(int)size()-1] != iTrMark){
		*this += iTrMark;//l1, iTrMark);
	}

	return *this;
}


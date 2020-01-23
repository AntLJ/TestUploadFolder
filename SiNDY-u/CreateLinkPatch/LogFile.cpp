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

/**
 *	@file LogFile.cpp
 *	@brief ログファイルクラス 実装部
 *	@author	F.Adachi
 *	@date	2005/06/29		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "LogFile.h"
#include <string>

LogFile* LogFile::mSelf = NULL;

/**
 *	@brief	コンストラクタ
 **/
LogFile::LogFile(void) : mLogStream(NULL)
{

}

/**
 *	@brief	デストラクタ
 **/
LogFile::~LogFile()
{
	delete mLogStream;
}


/**
 *	@brief	インスタンスの取得
 *	@note	LogFileインスタンスへのポインタを返す。インスタンスが生成されていなければ生成する。
 *	@return	LogFileインスタンスへのポインタ
 **/
LogFile* LogFile::Instance(void)
{
  if( mSelf == NULL ) {
	mSelf = new LogFile;
  }
  
  return mSelf;
}


/**
 *	@brief	ストリームの生成
 *	@note	与えられた出力ファイルをオープンし、ストリームオブジェクトを生成する
 *	@param	cFilePath	[in]	出力ファイルパス
 **/
void LogFile::setStream( const char* cFilePath )
{
	std::string aFunc = "LogFile::setStream()";

	mLogStream = new std::ofstream( cFilePath );
	if( !*mLogStream ) { throw std::runtime_error( "R1F:" + aFunc + "ログファイルオープンエラー" ); }
}

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
 *	@file LogFile.h
 *	@brief ログファイルクラス 定義部
 *	@note	Singletonパターン使用
 *	@author	F.Adachi
 *	@date	2005/06/29		新規作成
 *	$ID$
 */

#ifndef ___LOGFILE___
#define ___LOGFILE___

#include <fstream>
#include <boost/utility.hpp>

/// <B>ログファイルクラス</B>
class LogFile : boost::noncopyable
{
public:
	~LogFile();

	static LogFile* Instance(void);
	void setStream( const char* cFilePath );

	operator std::ostream&(void) { return ( mLogStream == NULL ) ? std::cerr : *mLogStream; }
	
private:
	static LogFile*	mSelf;				///< 自分自身へのポインタ
	std::ostream*		mLogStream;		///< ログ出力ストリーム
	LogFile(void);
};

#endif

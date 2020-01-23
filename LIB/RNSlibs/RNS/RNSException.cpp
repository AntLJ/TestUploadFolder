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
 
 	制作 :	Fukuta.K
 	
************************************************* */

#include <iostream>
#include <strstream>

#include "RNSException.h"

/*
 * *****
 * RNSException
 * *****
 */

/* --- 初期化 --- */
/**
 *  コンストラクタ
**/
RNSException::RNSException(const char *iMessage, const char *iLocation)
{
	mLocation = mMessage = 0;

	if(iLocation != NULL){
		mLocation = new RNSString(iLocation);
	}
	if(iMessage != NULL){
		mMessage = new RNSString(iMessage);
	}
}

RNSException::RNSException(const RNSException& iException)
{
	mLocation = new RNSString(iException.location());
	mMessage = new RNSString(iException.message());
}

RNSException::~RNSException()
{
	mLocation = mMessage = 0;
}

/* ===== パブリックメンバ ===== */
/* --- 入力 --- */
/**
 *  メッセージ
**/
RNSException& RNSException::setMessage(const char *iMessage)
{
	mMessage = 0;

	if(iMessage){
		mMessage = new RNSString(iMessage);
	}

	return *this;
}

RNSException& RNSException::setLocation(const char *iLocation)
{
	mLocation = 0;

	if(iLocation) {
		mLocation = new RNSString(iLocation);
	}

	return *this;
}

/* --- 出力 --- */

/**
 *  どこで？
**/
const char* RNSException::location() const
{
	if(mLocation == 0)
		return "";
	else
		return mLocation->c_str();
	
}

/**
 *  なにが？
**/
const char* RNSException::message() const
{
	if(mMessage == 0)
		return "";
	else
		return mMessage->c_str();
}

/**
 *	デフォルトメッセージ
**/
const char* RNSException::what() const
{
	static			char buffer[1024];
	std::strstream	strm(buffer, 1024, std::ios::out);

	strm << circumstance();
	
	if(mMessage) {
		strm << " : " << message();
	}

	if(mLocation) {
		strm << " [ " << location() << " ] ";
	}

	return buffer;
}

/*
 * *****
 * RNSExceptionCantOpenFile
 * *****
 */

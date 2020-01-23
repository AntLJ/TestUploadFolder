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

#ifndef __RNS_EXCEPTION__
#define __RNS_EXCEPTION__

/* ************************************************

 	初稿 :
 	改訂 :  
 	追記 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

#include <exception>

#include "RNS.h"
#include "RNSString.h"

class RNSException:public std::exception {
	RNSStringPtr	mLocation;
	RNSStringPtr 	mMessage;

 protected:
	virtual const char* circumstance() const{ return "exception occurred"; }

 public:
	RNSException(){mLocation = 0; mMessage = 0;}
	RNSException(const char *iMessage, const char *iLocation = NULL);
	RNSException(const RNSException& iExcept);
	virtual ~RNSException();

	virtual RNSException& setMessage(const char *iMessage);
	virtual RNSException& setLocation(const char *iLocation);

	virtual const char* location() const;			// 発生した場所
	virtual const char* origin() const{return "";}	// 発生させた物
	virtual const char* message() const;			// メッセージ
	virtual const char* what() const;				// 上三つをまとめて
};

/// 例外：無効な関数
class RNSExceptionInvalidFunction : public RNSException {
protected:
	virtual const char* circumstance() const { return "invalide function"; }

public:
	RNSExceptionInvalidFunction(const char* iFuncName)
	{ setMessage(iFuncName); }
	RNSExceptionInvalidFunction(const char* iFuncName, const char* iLocation)
	{ setMessage(iFuncName); setLocation(iLocation); }
};

/// 例外：ファイルが開けない
class RNSExceptionCantOpenFile : public RNSException {
protected:
	virtual const char* circumstance() const { return "cant open file"; }

public:
	RNSExceptionCantOpenFile(const char* iMessage)
	{ setMessage(iMessage); }
	RNSExceptionCantOpenFile(const char* iMessage, const char* iLocation)
	{ setMessage(iMessage); setLocation(iLocation); }
};


// 無限大になるっす
class RNSExceptionInfinity:public RNSException {
 protected:
	virtual const char* circumstance() const{ return "result is infinity"; };
 public:
	RNSExceptionInfinity(){};
	RNSExceptionInfinity(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionInfinity(){};
};

typedef RNSExceptionInfinity RNSExceptionDivideZero;

// NULL 参照時例外
class RNSExceptionNULL:public RNSException {
 protected:
	virtual const char* circumstance() const{ return "null read"; };
 public:
	RNSExceptionNULL(){};
	RNSExceptionNULL(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionNULL(){};
};

// 見つからなかったっす
class RNSExceptionNotFound:public RNSException{
 protected:
	virtual const char* circumstance() const{ return "not found"; };
 public:
	RNSExceptionNotFound(){};
	RNSExceptionNotFound(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionNotFound(){};
};

// 実行できないっす
class RNSExceptionCantExec:public RNSException{
 protected:
	virtual const char* circumstance() const{ return "can't execute"; };
 public:
	RNSExceptionCantExec(){};
	RNSExceptionCantExec(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionCantExec(){};
};

// dynamic_castできないっす
class RNSExceptionDynamicCast : public RNSException {
protected:
	virtual const char* circumstance() const { return "be cant dynamic_cast"; }
public:
	RNSExceptionDynamicCast(){}
	RNSExceptionDynamicCast(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionDynamicCast(){};
};
#endif

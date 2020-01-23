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

 	���e :
 	���� :  
 	�ǋL : 
 
 	���� :	Fukuta.K
 	
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

	virtual const char* location() const;			// ���������ꏊ
	virtual const char* origin() const{return "";}	// ������������
	virtual const char* message() const;			// ���b�Z�[�W
	virtual const char* what() const;				// ��O���܂Ƃ߂�
};

/// ��O�F�����Ȋ֐�
class RNSExceptionInvalidFunction : public RNSException {
protected:
	virtual const char* circumstance() const { return "invalide function"; }

public:
	RNSExceptionInvalidFunction(const char* iFuncName)
	{ setMessage(iFuncName); }
	RNSExceptionInvalidFunction(const char* iFuncName, const char* iLocation)
	{ setMessage(iFuncName); setLocation(iLocation); }
};

/// ��O�F�t�@�C�����J���Ȃ�
class RNSExceptionCantOpenFile : public RNSException {
protected:
	virtual const char* circumstance() const { return "cant open file"; }

public:
	RNSExceptionCantOpenFile(const char* iMessage)
	{ setMessage(iMessage); }
	RNSExceptionCantOpenFile(const char* iMessage, const char* iLocation)
	{ setMessage(iMessage); setLocation(iLocation); }
};


// ������ɂȂ����
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

// NULL �Q�Ǝ���O
class RNSExceptionNULL:public RNSException {
 protected:
	virtual const char* circumstance() const{ return "null read"; };
 public:
	RNSExceptionNULL(){};
	RNSExceptionNULL(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionNULL(){};
};

// ������Ȃ���������
class RNSExceptionNotFound:public RNSException{
 protected:
	virtual const char* circumstance() const{ return "not found"; };
 public:
	RNSExceptionNotFound(){};
	RNSExceptionNotFound(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionNotFound(){};
};

// ���s�ł��Ȃ�����
class RNSExceptionCantExec:public RNSException{
 protected:
	virtual const char* circumstance() const{ return "can't execute"; };
 public:
	RNSExceptionCantExec(){};
	RNSExceptionCantExec(const char *iMessage, const char *iLocation = NULL)
		: RNSException(iLocation, iMessage){};
	virtual ~RNSExceptionCantExec(){};
};

// dynamic_cast�ł��Ȃ�����
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

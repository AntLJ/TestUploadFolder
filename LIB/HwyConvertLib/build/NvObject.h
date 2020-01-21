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

/*
**	NvObject.h
**
**		navi98 network data core object class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			1997/11/26	navi97 §´§È∞˙÷πÅEˇ
*/

#ifndef	___NAVI98_CORE_OBJECT___
#define	___NAVI98_CORE_OBJECT___

#include "ZObject.h"
#include "ZMapInfo.h"
#include "ZIN.h"

class NvObject
{
	enum {
		kSign	= 0x4f424a20		// 'OBJ '
	};

	bool	mAlive;
	int		mUser;

public:
	NvObject(void) : mAlive(true), mUser(0) { }
	virtual ~NvObject() { }

protected:
	void	alive(void) { mAlive = true; }

public:
	bool	exist(void) const { return mAlive; }
	void	kill(void) { mAlive = false; }

	void	setUser(int cValue) { mUser = cValue; }
	int		getUser(void) const { return mUser; }

	bool	operator == (const NvObject& cObject) const { return compare(cObject) == 0; }
	bool	operator <  (const NvObject& cObject) const { return compare(cObject) <  0; }
	bool	operator >  (const NvObject& cObject) const { return compare(cObject) >  0; }

public:
	virtual int		getSign(void)            const { return kSign; }
	virtual int		compare(const NvObject&) const { return 0; }
};

#endif	// ___NAVI98_CORE_OBJECT___


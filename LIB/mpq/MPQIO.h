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
**	MPQIO
**
**		MPQ IO interface
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/8/6	çÏê¨
*/

#ifndef	___MPQ_IO___
#define	___MPQ_IO___

#include "MPQ.h"

struct MPQIO
{
	virtual ~MPQIO() { }

	virtual int		getVer(void) const	= 0;

	virtual bool	analyze(char* cBuffer, MPQ::Header* cHeader)		= 0;
	virtual bool	analyze(char* cBuffer, vector<MPQ::Link*>& cLink)	= 0;
	virtual bool	analyze(char* cBuffer, vector<MPQ::Attr*>& cAttr)	= 0;

	virtual bool	write(ostream& cStream, const MPQ::Header& cHeader)	= 0;
	virtual bool	write(ostream& cStream, const vector<MPQ::Link*>& cLink)	= 0;
	virtual bool	write(ostream& cStream, const vector<MPQ::Attr*>& cAttr)	= 0;
};

#endif	// ___MPQ_IO___


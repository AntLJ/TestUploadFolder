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
**	MPQVer1.h
**
**		MPQ IO for format ver.1 class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/8/6	çÏê¨
*/

#ifndef	___MPQ_VER1___
#define	___MPQ_VER1___

#include "MPQIO.h"

class MPQVer1 : public MPQIO
{
	map<int, MPQ::Attr*>	mAttr;
	map<int, int>			mObjID;

public:
	MPQVer1(void) { }
	virtual ~MPQVer1() { }

	virtual int		getVer(void) const { return 1; }

	virtual bool	analyze(char* cBuffer, MPQ::Header* cHeader);
	virtual bool	analyze(char* cBuffer, vector<MPQ::Link*>& cLink);
	virtual bool	analyze(char* cBuffer, vector<MPQ::Attr*>& cAttr);

	virtual bool	write(ostream& cStream, const MPQ::Header& cHeader);
	virtual bool	write(ostream& cStream, const vector<MPQ::Link*>& cLink);
	virtual bool	write(ostream& cStream, const vector<MPQ::Attr*>& cAttr);
	
	
	static int	SetAttr  (char* cPtr, MPQ::Attr* cAttr, int cFixed=-1);
	static int	WriteAttr(ostream& cStream, const MPQ::Attr& cAttr);
	static int	SizeAttr (const MPQ::Attr& cAttr);

	static void	r_write32B(ostream& cStream, int cValue);


};

#endif	// ___MPQ_VER1___


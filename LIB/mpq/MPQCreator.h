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
**	MPQCreator.h
**
**		MPQ skelton creator class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/8/5	çÏê¨
*/

#ifndef	___MPQ_CREATOR___
#define	___MPQ_CREATOR___

#include "MPQ.h"

class MPQCreator : public MPQ
{
	virtual Attr*	newAttr(void) { assert(0); return 0; }

public:
	MPQCreator(MPQIO* cIO) : MPQ(cIO) { }
	virtual ~MPQCreator() { }

	virtual void	addLink(Link* cLink);
};

#endif	// ___MPQ_CREATOR___


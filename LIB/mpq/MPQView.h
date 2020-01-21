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
**	MPQView.h
**
**		MPQ editable class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/8/6	çÏê¨
*/

#ifndef	___MPQ_VIEW___
#define	___MPQ_VIEW___

#include "MPQ.h"

class MPQView : public MPQ
{
	virtual void	addLink(Link* cLink) { assert(0); }
	virtual Attr*	newAttr(void) { assert(0); return 0; } 

public:
	MPQView(MPQIO* cIO) : MPQ(cIO) { }
	virtual ~MPQView() { }
};

#endif	// ___MPQ_VIEW___


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
**	MPQEdit.h
**
**		MPQ editable class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/8/5	çÏê¨
*/

#ifndef	___MPQ_EDIT___
#define	___MPQ_EDIT___

#include "MPQ.h"

class MPQEdit : public MPQ
{
	int		mNextID;

	virtual void	addLink(Link* cLink) { assert(0); }

public:
	MPQEdit(MPQIO* cIO) : MPQ(cIO), mNextID(-1) { }
	virtual ~MPQEdit() { }

	virtual Attr*	newAttr(void);
};

#endif	// ___MPQ_EDIT___


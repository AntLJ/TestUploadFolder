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
**	MPQPRO.h
**
**		MPQ class for PROFESSIONAL  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			2003/9/26	çÏê¨
*/

#ifndef	___MPQ_PRO___
#define	___MPQ_PRO___

#include "MPQEdit.h"

class MPQPro : public MPQEdit
{
public:
	MPQPro(MPQIO* cIO) : MPQEdit(cIO) { }
	virtual ~MPQPro() { }

	virtual void	addLink(Link* cLink);
};

#endif	// ___MPQ_PRO___


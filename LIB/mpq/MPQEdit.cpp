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
**	MPQEdit
**
**		MPQ editable class
**
**		Programmed by H.Miyahara
*/

#include "stdafx.h"
#include "MPQEdit.h"


/*
**	êVãKëÆê´ÇÃê∂ê¨
*/

MPQ::Attr* MPQEdit::newAttr(void)
{
	int	aID;
	if(0 == mAttr.size())
	{
		aID = 1;
	} else
	{
		if(-1 == mNextID)
		{
			for(vector<Attr*>::const_iterator i=mAttr.begin() ; i!=mAttr.end() ; ++i)
			{
				if(mNextID < (*i)->getID()) mNextID = (*i)->getID();
			}
			++mNextID;
		}
		aID = mNextID++;
	}

	Attr*	pAttr = new Attr(aID);
	mAttr.push_back(pAttr);

	return pAttr;
}


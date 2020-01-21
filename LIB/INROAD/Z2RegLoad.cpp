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

// Z2RegLoad.cpp: Z2RegLoad クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Z2RegLoad.h"

Z2RegLoad::Z2RegLoad() : ZRegLoad()
{

}

Z2RegLoad::~Z2RegLoad()
{

}

void Z2RegLoad::searchByID(long cID, std::vector<INReg *> *cRegList)
{
	Range aRange = find(cID);
	for (Range::first_type aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		INReg *reg = (INReg *)aIter->ePtr;
		cRegList->push_back(reg);
	}
	return;
}

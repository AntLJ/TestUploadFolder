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

// Z2RegLoad.h: Z2RegLoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Z2REGLOAD_H__AAB56191_DE8B_4AE8_822D_B73548A1A63C__INCLUDED_)
#define AFX_Z2REGLOAD_H__AAB56191_DE8B_4AE8_822D_B73548A1A63C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZRegLoad.h"

typedef INReg IN2Reg;

class Z2RegLoad : public ZRegLoad  
{
public:
	Z2RegLoad();
	virtual ~Z2RegLoad();

	virtual void searchByID(long cID, std::vector<INReg *> *cRegList);
};

#endif // !defined(AFX_Z2REGLOAD_H__AAB56191_DE8B_4AE8_822D_B73548A1A63C__INCLUDED_)

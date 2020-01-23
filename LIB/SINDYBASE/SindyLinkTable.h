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

// SindyLinkTable.h: SindyLinkTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYLINKTABLE_H__069D1AD9_ACB0_476E_9AFB_9E3381037FF9__INCLUDED_)
#define AFX_SINDYLINKTABLE_H__069D1AD9_ACB0_476E_9AFB_9E3381037FF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"

class SindyLinkTable : public SindyTable  
{
public:
	SindyLinkTable(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyLinkTable();

	virtual HRESULT LoadByMeshCode(long cMeshCode);
	virtual HRESULT LoadByLinkID(const std::set<long> &cList);
	virtual HRESULT SearchByLinkID(long cLinkID, GDBOBJRANGE *cRange);

protected:
	GDB2OBJLIST mLinkList;

protected:
	virtual HRESULT Store(const std::set<long> &cList);
	virtual HRESULT Store(_ICursorPtr ipCursor);
};

#endif // !defined(AFX_SINDYLINKTABLE_H__069D1AD9_ACB0_476E_9AFB_9E3381037FF9__INCLUDED_)

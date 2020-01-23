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

// SindyInfTable.h: SindyInfTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYINFTABLE_H__82A83BED_B852_463D_82EE_B5D3C19B6297__INCLUDED_)
#define AFX_SINDYINFTABLE_H__82A83BED_B852_463D_82EE_B5D3C19B6297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"
#include "SindyLqTable.h"
#include "SindyTools.h"

class SindyInfTable : public SindyTable  
{
public:
	SindyInfTable(SindyDBConnection *cDBConnection, const std::string &cInfName, const std::string &cLqName);
	virtual ~SindyInfTable();

	virtual HRESULT Load();
	virtual HRESULT LoadByMeshCode(long cMeshCode, bool cStart = true);
	virtual HRESULT LoadByObjectID(const std::set<long> &cList);

private:
	std::string mLqName;

protected:
	virtual HRESULT Store(_ICursorPtr ipCursor);
	virtual HRESULT Store(const std::set<long> &cBuff);
};

#endif // !defined(AFX_SINDYINFTABLE_H__82A83BED_B852_463D_82EE_B5D3C19B6297__INCLUDED_)

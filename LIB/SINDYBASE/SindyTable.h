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

// SindyTable.h: SindyTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYTABLE_H__115C6C23_AEFC_4CD0_9DE1_2A6C4B90A847__INCLUDED_)
#define AFX_SINDYTABLE_H__115C6C23_AEFC_4CD0_9DE1_2A6C4B90A847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyDataset.h"

class SindyTable : public SindyDataset  
{
public:
	SindyTable(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyTable();

	virtual HRESULT SearchByObjectID(long aID, _IRow **ipRow);
	virtual ITablePtr GetTable() { return (ITablePtr)ipDataset; }
};

#endif // !defined(AFX_SINDYTABLE_H__115C6C23_AEFC_4CD0_9DE1_2A6C4B90A847__INCLUDED_)

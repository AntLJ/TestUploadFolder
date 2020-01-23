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

// SindyRoadCodeList.h: SindyRoadCodeList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYROADCODELIST_H__D54B6CFE_FF0F_40DB_82BD_CAB1A81B74FC__INCLUDED_)
#define AFX_SINDYROADCODELIST_H__D54B6CFE_FF0F_40DB_82BD_CAB1A81B74FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"

class SindyRoadCodeList : public SindyTable  
{
public:
	SindyRoadCodeList(SindyDBConnection *cDBConnection);
	virtual ~SindyRoadCodeList();

	virtual HRESULT Load();
	virtual HRESULT SearchByRoadCode(long cRoadCode, _IRow **ipRow);

protected:
	GDBOBJLIST mRoadCodeList;
};

#endif // !defined(AFX_SINDYROADCODELIST_H__D54B6CFE_FF0F_40DB_82BD_CAB1A81B74FC__INCLUDED_)

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

// SindyAccessStore.h: SindyAccessStore クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYACCESSSTORE_H__33177EF6_8296_452C_BB1B_EF34D5D18295__INCLUDED_)
#define AFX_SINDYACCESSSTORE_H__33177EF6_8296_452C_BB1B_EF34D5D18295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"

class SindyAccessStore : public SindyTable  
{
public:
	SindyAccessStore(SindyDBConnection *cDBConnection);
	virtual ~SindyAccessStore();

	virtual HRESULT Load();
	virtual HRESULT SearchByAccessPointID(long cID, GDBOBJRANGE* cpRange);

protected:
	GDB2OBJLIST	mAccessStoreList;	// AccessPoint_IDをキーにしたリスト
};

#endif // !defined(AFX_SINDYACCESSSTORE_H__33177EF6_8296_452C_BB1B_EF34D5D18295__INCLUDED_)

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

// SindyEtcTollgate.h: SindyEtcTollgate クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyTable.h"

class SindyEtcTollgate : public SindyTable
{
public:
	SindyEtcTollgate( SindyDBConnection *cDBConnection );
	virtual ~SindyEtcTollgate();

	virtual HRESULT Load();
	virtual HRESULT SearchByTollgatetID( const CString& cTollgateID, _IRow **ipRow );

protected:
	GDBOBJLIST	mTollgateIDList;	// TOLLGATE_IDをキーにしたリスト
};


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

// SindyDataset.h: SindyDataset クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYDATASET_H__30FEF69E_8B8D_42D9_BD33_CE5917720FD9__INCLUDED_)
#define AFX_SINDYDATASET_H__30FEF69E_8B8D_42D9_BD33_CE5917720FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyDBConnection.h"

class SindyDataset : public GDBOBJLIST  
{
public:
	SindyDataset(SindyDBConnection *cDBConnection);
	virtual ~SindyDataset();

	virtual bool is_open() { return (ipDataset == NULL) ? false : true; }
	virtual HRESULT GetName(std::string *cName);
	virtual IDatasetPtr GetDataset() { return ipDataset; }

protected:
	SindyDBConnection *mpDBConnection;
	IDatasetPtr ipDataset;
};

#endif // !defined(AFX_SINDYDATASET_H__30FEF69E_8B8D_42D9_BD33_CE5917720FD9__INCLUDED_)

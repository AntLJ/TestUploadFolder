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

// SindyFeatureClass.h: SindyFeatureClass クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYFEATURECLASS_H__8007D838_0394_4185_9F81_1D348CB5713D__INCLUDED_)
#define AFX_SINDYFEATURECLASS_H__8007D838_0394_4185_9F81_1D348CB5713D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyDataset.h"

class SindyFeatureClass : public SindyDataset  
{
public:
	SindyFeatureClass(SindyDBConnection *cDBConnection, const std::string &cName);
	virtual ~SindyFeatureClass();

	virtual HRESULT SearchByObjectID(long aID, IFeature **ipFeature);
	virtual IFeatureClassPtr GetFeatureClass() { return (IFeatureClassPtr)ipDataset; }
};

#endif // !defined(AFX_SINDYFEATURECLASS_H__8007D838_0394_4185_9F81_1D348CB5713D__INCLUDED_)

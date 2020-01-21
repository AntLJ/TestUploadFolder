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

// INRoadInfoSet.h: INRoadInfoSet クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INROADINFOSET_H__BB8EB8D7_9AA2_4784_BDBD_E8B4DD13D222__INCLUDED_)
#define AFX_INROADINFOSET_H__BB8EB8D7_9AA2_4784_BDBD_E8B4DD13D222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "INROADINFO.h"

class INRoadInfoSet  
{
public:
	INRoadInfoSet();
	INRoadInfoSet(const char *cDirectory);
	virtual ~INRoadInfoSet();

	void SetDirectory(const char *cDirectory) { mDirectory = cDirectory; }
	bool Open(long cMesh, INROADINFO **cRoadInfo);
	void Close(long cMesh = 0);

private:
	std::string mDirectory;
	std::map<long, INROADINFO *> mInfoMap;
};

#endif // !defined(AFX_INROADINFOSET_H__BB8EB8D7_9AA2_4784_BDBD_E8B4DD13D222__INCLUDED_)

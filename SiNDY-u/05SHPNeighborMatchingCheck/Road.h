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

#pragma once
#include "GetPointInfo.h"

class CRoad
{
public:
	CRoad(long aMeshCode, std::wstring aLayerListPath, std::wstring aAllow);
	~CRoad(void);
	bool Main(std::wstring aTKYPath, std::wstring aJGDPath, std::wstring aDataSetName);
	//virtual void MakeDirPath(std::wstring aPath, std::wostringstream & aDirPath);

private:
	long m_SecondMeshCode;
	std::wstring m_LayerListPath;
	std::wstring m_Allow;

};

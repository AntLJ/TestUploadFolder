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

#include "StdAfx.h"
#include "ReadBMList.h"
#include <iostream>
#include <fstream>

CReadBMList::CReadBMList(std::wstring & rFilePath,std::wstring & rDataSetName) : CReadMeshList(rFilePath,rDataSetName)
{
}

CReadBMList::~CReadBMList(void)
{
}
bool CReadBMList::CheckMesh(long aMesh)
{
	if((aMesh<303640)||(aMesh>684827)){
		return false;
	}

	return true;
}

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

// INRoadInfoSet.cpp: INRoadInfoSet クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "INRoadInfoSet.h"
#include "WKHFile.h"

INRoadInfoSet::INRoadInfoSet() : mInfoMap(), mDirectory("")
{

}

INRoadInfoSet::INRoadInfoSet(const char *cDirectory) : mInfoMap(), mDirectory(cDirectory)
{

}

INRoadInfoSet::~INRoadInfoSet()
{
	Close();
}

void INRoadInfoSet::Close(long cMesh)
{
	if (! cMesh)
	{
		for (std::map<long, INROADINFO *>::iterator aIter = mInfoMap.begin(); aIter != mInfoMap.end(); aIter++)
			delete aIter->second;
	}
	else
	{
		std::map<long, INROADINFO *>::iterator aIter = mInfoMap.find(cMesh);
		if (aIter != mInfoMap.end())
		{
			delete aIter->second;
			mInfoMap.erase(aIter);
		}
	}
}

bool INRoadInfoSet::Open(long cMesh, INROADINFO **cRoadInfo)
{
	*cRoadInfo = NULL;

	std::map<long, INROADINFO *>::iterator aIter = mInfoMap.find(cMesh);
	if (aIter != mInfoMap.end())
	{
		*cRoadInfo = aIter->second;
		return true;
	}

	std::string aMntFile = CreateFileName(mDirectory, cMesh, "mnt", false);
	std::string aRdgFile = CreateFileName(mDirectory, cMesh, "rdg", false);
	if (_access(aMntFile.c_str(), 0) != 0)
	{
//		std::cerr << aMntFile << " not found." << std::endl;
		return false;
	}
	if (_access(aRdgFile.c_str(), 0) != 0)
	{
//		std::cerr << aRdgFile << " not found." << std::endl;
		return false;
	}

	INROADINFO *apRoadInfo = new INROADINFO();
	if (apRoadInfo == 0)
	{
		std::cerr << "memory allocation error." << std::endl;
		return false;
	}

	if (! apRoadInfo->Load(mDirectory.c_str(), cMesh))
	{
		std::cerr << cMesh << " file load error." << std::endl;
		return false;
	}

	mInfoMap.insert(std::pair<long, INROADINFO *>(cMesh, apRoadInfo));

	*cRoadInfo = apRoadInfo;

	return true;
}

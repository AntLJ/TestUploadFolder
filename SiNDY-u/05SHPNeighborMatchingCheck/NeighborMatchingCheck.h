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
#include <vector>
#include <set>

class CNeighborMatchingCheck
{
public:
	CNeighborMatchingCheck(void);
	~CNeighborMatchingCheck(void);
	int App(const Parameters & rParams); 
	bool ReadLayer(std::vector<std::string> *pLayerList,const std::string &rListName );
	bool GetTargetMesh(std::set<long> *TargetMesh,  const Parameters &rParams, int *iErrFlag);

private:
	bool MeshCheck(long aMeshCode,std::string aDataSet);

};

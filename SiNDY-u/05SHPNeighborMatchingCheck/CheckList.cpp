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
#include "CheckList.h"
#include <fstream>
#include <atl2/io.h>

CCheckList::CCheckList(void)
{
}

CCheckList::~CCheckList(void)
{
}

bool CCheckList::Check(std::set<CheckKey> &rTKYMergeSet, std::set<CheckKey> &rJGDMergeSet,std::wstring aLayerName)
{

	std::set<CheckKey>::iterator TKYitr = rTKYMergeSet.begin();
	
	bool aCheck=true;
	
	
	while( TKYitr != rTKYMergeSet.end() )
	{
		std::set<CheckKey>::iterator JGDitr = rJGDMergeSet.find(*TKYitr);
		if(JGDitr == rJGDMergeSet.end() ){
			std::cout << "ƒŒƒCƒ„–¼:" << aLayerName << '\t' << TKYitr->e_MMeshCode << '\t' << TKYitr->e_MOID << '\t' << TKYitr->e_MainPointNumber
				<< '\t' << TKYitr->e_NMeshCode << '\t' << TKYitr->e_NOID << '\t' << TKYitr->e_NeighborPointNumber << "\t‚ÌŠÖŒW‚ª¢ŠE‘ª’nŒn‚É‘¶Ý‚µ‚Ü‚¹‚ñ" << std::endl;
			aCheck = false;
		}

		TKYitr++;
	}

	return aCheck;	


}

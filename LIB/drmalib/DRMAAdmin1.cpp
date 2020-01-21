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

// DRMAAdmin1.cpp: DRMAAdmin1 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include <DRMAAdmin1.h>
#include "DRMACodeCommunity.h"

const int DRMAAdmin1::sRecordId = drma::kAdmin1Id;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMAAdmin1::DRMAAdmin1(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eMeshCode));
	VALID(SET_VALUE(eEditionNo));

	VALID(SET_VALUE(eScale));	
	VALID(SET_VALUE(eMeshName));	
	VALID(SET_VALUE(eRevisionYear));	
	VALID(SET_VALUE(eModYear));	

	VALID(SET_VALUE(eBias));	
	
	VALID(SET_VALUE(eBottomDistance));	
	VALID(SET_VALUE(eRightDistance));	
	VALID(SET_VALUE(eTopDistance));	
	VALID(SET_VALUE(eLeftDistance));

	VALID(SET_VALUE(eUpdateDate.eBaseRoadNode));
	VALID(SET_VALUE(eUpdateDate.eBaseRoadLink));
	VALID(SET_VALUE(eUpdateDate.eBaseRoadLinkAttr));
	VALID(SET_VALUE(eUpdateDate.eBeaconLocation));
	VALID(SET_VALUE(eUpdateDate.eAllRoadNode));
	VALID(SET_VALUE(eUpdateDate.eAllRoadLink));
	VALID(SET_VALUE(eUpdateDate.eSite));
	VALID(SET_VALUE(eUpdateDate.eAdminLocation));
	VALID(SET_VALUE(eUpdateDate.eRailwayLocation));
	VALID(SET_VALUE(eUpdateDate.eFacilLocation));
	VALID(SET_VALUE(eUpdateDate.eFacilShape));
	VALID(SET_VALUE(eUpdateDate.ePlaceNameLocation));
	VALID(SET_VALUE(eUpdateDate.eRoadLinkRelation));
	VALID(SET_VALUE(eUpdateDate.eAllRoadLinkAttr));

	VALID(SET_VALUE(eModDate.eBaseRoadNode));
	VALID(SET_VALUE(eModDate.eBaseRoadLink));
	VALID(SET_VALUE(eModDate.eBaseRoadLinkAttr));
	VALID(SET_VALUE(eModDate.eBeaconLocation));
	VALID(SET_VALUE(eModDate.eAllRoadNode));
	VALID(SET_VALUE(eModDate.eAllRoadLink));
	VALID(SET_VALUE(eModDate.eSite));
	VALID(SET_VALUE(eModDate.eAdminLocation));
	VALID(SET_VALUE(eModDate.eRailwayLocation));
	VALID(SET_VALUE(eModDate.eFacilLocation));
	VALID(SET_VALUE(eModDate.eFacilShape));
	VALID(SET_VALUE(eModDate.ePlaceNameLocation));
	VALID(SET_VALUE(eModDate.eRoadLinkRelation));
	VALID(SET_VALUE(eModDate.eAllRoadLinkAttr));

	VALID(SET_VALUE(eLastModDate));
}

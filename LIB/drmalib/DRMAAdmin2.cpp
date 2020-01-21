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

// DRMAAdmin2.cpp: DRMAAdmin2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMAAdmin2.h"
#include "DRMACodeCommunity.h"

const int DRMAAdmin2::sRecordId = drma::kAdmin2Id;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMAAdmin2::DRMAAdmin2(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(eRecordCount.eBaseRoadNode));
	VALID(SET_VALUE(eRecordCount.eBaseRoadLink));
	VALID(SET_VALUE(eRecordCount.eBaseRoadLinkAttr));
	VALID(SET_VALUE(eRecordCount.eBeaconLocation));
	VALID(SET_VALUE(eRecordCount.eAllRoadNode));
	VALID(SET_VALUE(eRecordCount.eAllRoadLink));
	VALID(SET_VALUE(eRecordCount.eSite));
	VALID(SET_VALUE(eRecordCount.eAdminLocation));
	VALID(SET_VALUE(eRecordCount.eRailwayLocation));
	VALID(SET_VALUE(eRecordCount.eFacilLocation));
	VALID(SET_VALUE(eRecordCount.eFacilShape));
	VALID(SET_VALUE(eRecordCount.ePlaceNameLocation));
	VALID(SET_VALUE(eRecordCount.eRoadLinkRelation));
	VALID(SET_VALUE(eRecordCount.eAllRoadLinkAttr));

	VALID(SET_VALUE(eItemCount.eBaseRoadNode));
	VALID(SET_VALUE(eItemCount.eBaseRoadLink));
	VALID(SET_VALUE(eItemCount.eBaseRoadLinkAttr));
	VALID(SET_VALUE(eItemCount.eBeaconLocation));
	VALID(SET_VALUE(eItemCount.eAllRoadNode));
	VALID(SET_VALUE(eItemCount.eAllRoadLink));
	VALID(SET_VALUE(eItemCount.eSite));
	VALID(SET_VALUE(eItemCount.eAdminLocation));
	VALID(SET_VALUE(eItemCount.eRailwayLocation));
	VALID(SET_VALUE(eItemCount.eFacilLocation));
	VALID(SET_VALUE(eItemCount.eFacilShape));
	VALID(SET_VALUE(eItemCount.ePlaceNameLocation));
	VALID(SET_VALUE(eItemCount.eRoadLinkRelation));
	VALID(SET_VALUE(eItemCount.eAllRoadLinkAttr));

	VALID(SET_VALUE(eCopyright));

	VALID(SET_VALUE(eMaxItemId.eBaseRoadNode));
	VALID(SET_VALUE(eMaxItemId.eAllRoadNode1));
	VALID(SET_VALUE(eMaxItemId.eAllRoadNode2));
	VALID(SET_VALUE(eMaxItemId.eSite));
	VALID(SET_VALUE(eMaxItemId.eAdminLocation));
	VALID(SET_VALUE(eMaxItemId.eRailwayLocation));
	VALID(SET_VALUE(eMaxItemId.eFacil));
	VALID(SET_VALUE(eMaxItemId.ePlaceNameLocation));
	VALID(SET_VALUE(eMaxItemId.eBeacon));
}


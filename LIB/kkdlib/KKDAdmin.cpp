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

// KKDAdmin.cpp: KKDAdmin クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDAdmin.h"
#include "KKDCodeCommunity.h"

const int KKDAdmin::sRecordId = kkd::kAdminId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

KKDAdmin::KKDAdmin(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eMeshCode));
	VALID(SET_VALUE(eEditionNo));

	VALID(SET_VALUE(eDrmaEditionNo));
	
	VALID(SET_VALUE(eUpdateDate.eRegulationRoadNode));
	VALID(SET_VALUE(eUpdateDate.eRegulationRoadLink));
	VALID(SET_VALUE(eUpdateDate.eLaneRegulationRoadNode));
	VALID(SET_VALUE(eUpdateDate.eLaneRegulationRoadLink));
	VALID(SET_VALUE(eUpdateDate.eLaneRegulationIntersectionNode));
	VALID(SET_VALUE(eModDate.eRegulationRoadNode));
	VALID(SET_VALUE(eModDate.eRegulationRoadLink));
	VALID(SET_VALUE(eModDate.eLaneRegulationRoadNode));
	VALID(SET_VALUE(eModDate.eLaneRegulationRoadLink));
	VALID(SET_VALUE(eModDate.eLaneRegulationIntersectionNode));
	VALID(SET_VALUE(eRecordCount.eRegulationRoadNode));
	VALID(SET_VALUE(eRecordCount.eRegulationRoadLink));
	VALID(SET_VALUE(eRecordCount.eLaneRegulationRoadNode));
	VALID(SET_VALUE(eRecordCount.eLaneRegulationRoadLink));
	VALID(SET_VALUE(eRecordCount.eLaneRegulationIntersectionNode));
	VALID(SET_VALUE(eItemCount.eRegulationRoadNode));
	VALID(SET_VALUE(eItemCount.eRegulationRoadLink));
	VALID(SET_VALUE(eItemCount.eLaneRegulationRoadNode));
	VALID(SET_VALUE(eItemCount.eLaneRegulationRoadLink));
	VALID(SET_VALUE(eItemCount.eLaneRegulationIntersectionNode));

	VALID(SET_VALUE(eCopyright));
	
	VALID(SET_VALUE(eMaxItemId.eBaseRoadNode));	
	VALID(SET_VALUE(eMaxItemId.eAllRoadNode1));	
	VALID(SET_VALUE(eMaxItemId.eAllRoadNode2));	
	VALID(SET_VALUE(eMaxItemId.eOtherNode1));	
	VALID(SET_VALUE(eMaxItemId.eOtherNode2));	
}

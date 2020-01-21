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

// DRMABeaconLocation.cpp: DRMABeaconLocation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMABeaconLocation.h"
#include "DRMACodeCommunity.h"

const int DRMABeaconLocation::sRecordId = drma::kBeaconLocationId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMABeaconLocation::DRMABeaconLocation(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(eItemNo));
	VALID(SET_VALUE(eId));
	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eApplication));

	VALID(SET_VALUE(ePoint.eX));
	VALID(SET_VALUE(ePoint.eY));

	VALID(SET_VALUE(eLinkId.eNode1Id));
	VALID(SET_VALUE(eLinkId.eNode2Id));
	VALID(SET_VALUE(eLinkDir));

	VALID(SET_VALUE(eName.eKanjiLength));
	VALID(SET_VALUE(eName.eKanji));
	VALID(SET_VALUE(eName.eKanaLength));
	VALID(SET_VALUE(eName.eKana));
}

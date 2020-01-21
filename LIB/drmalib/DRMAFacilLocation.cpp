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

// DRMAFacilLocation.cpp: DRMAFacilLocation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMAFacilLocation.h"
#include "DRMACodeCommunity.h"

const int DRMAFacilLocation::sRecordId = drma::kFacilLocationId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMAFacilLocation::DRMAFacilLocation(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId));
	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eAdminCode));
	VALID(SET_VALUE(eShapeCount));

	VALID(SET_VALUE(ePoint.eX));
	VALID(SET_VALUE(ePoint.eY));

	VALID(SET_VALUE(eShortName.eKanjiLength));
	VALID(SET_VALUE(eShortName.eKanji));
	VALID(SET_VALUE(eShortName.eKanaLength));
	VALID(SET_VALUE(eShortName.eKana));

	VALID(SET_VALUE(eLongName.eKanjiLength));
	VALID(SET_VALUE(eLongName.eKanji));
	VALID(SET_VALUE(eLongName.eKanaLength));
	VALID(SET_VALUE(eLongName.eKana));

	VALID(SET_VALUE(eAdjoinBaseNodeCount));
	VALID(SET_VALUE(eAdjoinBaseNodes[0].eMeshCode));
	VALID(SET_VALUE(eAdjoinBaseNodes[0].eId));
	VALID(SET_VALUE(eAdjoinBaseNodes[1].eMeshCode));
	VALID(SET_VALUE(eAdjoinBaseNodes[1].eId));

	VALID(SET_VALUE(eAdjoinBaseLinkCount));
	VALID(SET_VALUE(eAdjoinBaseLinks[0].eMeshCode));
	VALID(SET_VALUE(eAdjoinBaseLinks[0].eId.eNode1Id));
	VALID(SET_VALUE(eAdjoinBaseLinks[0].eId.eNode2Id));
	VALID(SET_VALUE(eAdjoinBaseLinks[0].eLinkDir));
	VALID(SET_VALUE(eAdjoinBaseLinks[1].eMeshCode));
	VALID(SET_VALUE(eAdjoinBaseLinks[1].eId.eNode1Id));
	VALID(SET_VALUE(eAdjoinBaseLinks[1].eId.eNode2Id));
	VALID(SET_VALUE(eAdjoinBaseLinks[1].eLinkDir));
}

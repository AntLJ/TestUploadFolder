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

// DRMAPlaceNameLocation.cpp: DRMAPlaceNameLocation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMAPlaceNameLocation.h"
#include "DRMACodeCommunity.h"

const int DRMAPlaceNameLocation::sRecordId = drma::kPlaceNameLocationId;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMAPlaceNameLocation::DRMAPlaceNameLocation(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId));
	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eAdminCode));
	VALID(SET_VALUE(eDispLevel));
	VALID(SET_VALUE(eAngle));

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
}

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

// DRMABackGround.cpp: DRMABackGround クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMABackGround.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DRMABackGround::DRMABackGround(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));

	VALID(SET_VALUE(eId));
	VALID(SET_VALUE(eSequence));
	VALID(SET_VALUE(eClass));

	VALID(SET_VALUE(ePointCount));
	appendPoints(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// 追加
//////////////////////////////////////////////////////////////////////

void DRMABackGround::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendPoints(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}


void DRMABackGround::appendPoints(const Format& cOriginalRecord)
{
	for(int i = 0; i < 21 && (int)ePoints.size() < ePointCount; i++) {
		ePoints.push_back(Point(cOriginalRecord.ePoints[i]));
	}
}


// DRMABackGround::Point クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

DRMABackGround::Point::Point(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eX));
	VALID(SET_VALUE(eY));
}

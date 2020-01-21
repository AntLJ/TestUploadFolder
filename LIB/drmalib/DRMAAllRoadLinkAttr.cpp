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

// DRMAAllRoadLinkAttr.cpp: DRMAAllRoadLinkAttr �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "DRMADevPlant.h"
#include "DRMAAllRoadLinkAttr.h"
#include "DRMACodeCommunity.h"

const int DRMAAllRoadLinkAttr::sRecordId = drma::kAllRoadLinkAttrId;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

DRMAAllRoadLinkAttr::DRMAAllRoadLinkAttr(const Format& cOriginalRecord)
{
	PREPARE_SET_VALUE(cOriginalRecord);

	VALID(SET_VALUE(eRecordId));
	VALID(SET_VALUE(eId.eNode1Id));
	VALID(SET_VALUE(eId.eNode2Id));
	VALID(SET_VALUE(eSequence));

	VALID(SET_VALUE(eAttrCount));
	appendAttrs(cOriginalRecord);

	VALID(SET_VALUE(eContinue));
}

//////////////////////////////////////////////////////////////////////
// �ǉ�
//////////////////////////////////////////////////////////////////////

void DRMAAllRoadLinkAttr::append(const Format& cOriginalRecord)
{
	assert(eContinue != 0);

	appendAttrs(cOriginalRecord);
	VALID(SET_VALUE_FROM(cOriginalRecord, eContinue));
}

void DRMAAllRoadLinkAttr::appendAttrs(const Format& cOriginalRecord)
{
	for(int i = 0; i < 3 && (int)eAttrs.size() < eAttrCount; i++) {
		eAttrs.push_back(Attr(cOriginalRecord.eAttrs[i]));
		eAttr2s.push_back(Attr2(cOriginalRecord.eAttr2s[i]));
	}
}

// DRMAAllRoadLinkAttr::Attr �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

DRMAAllRoadLinkAttr::Attr::Attr(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eClass));
	VALID(SET_VALUE(eDispLevel));
	VALID(SET_VALUE(eBeginPointNo));
	VALID(SET_VALUE(ePreBegin));
	VALID(SET_VALUE(eEndPointNo));
	VALID(SET_VALUE(eEpiEnd));
	VALID(SET_VALUE(eLength));
	
	// �������̕\���Q�l�ʒu���K�����W
	VALID(SET_VALUE(ePoint.eX));
	VALID(SET_VALUE(ePoint.eY));
	
	// ��������
	VALID(SET_VALUE(eName.eKanjiLength));
	VALID(SET_VALUE(eName.eKanji));
	VALID(SET_VALUE(eName.eKanaLength));
	VALID(SET_VALUE(eName.eKana));
}

// DRMAAllRoadLinkAttr::Attr2 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

DRMAAllRoadLinkAttr::Attr2::Attr2(const Format& cOriginal)
{
	PREPARE_SET_VALUE(cOriginal);

	VALID(SET_VALUE(eHeavyNoPassage));
	VALID(SET_VALUE(eFacilManager));
}

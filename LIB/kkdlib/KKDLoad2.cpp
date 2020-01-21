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

// KKDLoad2.cpp: KKDLoad2 �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDLoad2.h"

//////////////////////////////////////////////////////////////////////
// ���z�֐��̃I�[�o�[���C�h
//////////////////////////////////////////////////////////////////////

bool KKDLoad2::load(std::istream& cStream)
{
	if(! _KKDLoad2::load(cStream))
		return false;

	mRegulationRoadNodeLayer.setIndex();
	mRegulationRoadLinkLayer.setIndex();
	mLaneRegulationRoadNodeLayer.setIndex();
	mLaneRegulationRoadLinkLayer.setIndex();
	mLaneRegulationIntersectionNodeLayer.setIndex();

	return true;
}

void KKDLoad2::clear()
{
	_KKDLoad2::clear();

	// �C���f�b�N�X�j��
	mRegulationRoadNodeLayer.clearIndex();
	mRegulationRoadLinkLayer.clearIndex();
	mLaneRegulationRoadNodeLayer.clearIndex();
	mLaneRegulationRoadLinkLayer.clearIndex();
	mLaneRegulationIntersectionNodeLayer.clearIndex();
}

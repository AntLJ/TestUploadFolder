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

#pragma once
#include "CheckRoadWalkPassageCondition.h"

class CCheckRoadWalkOnewayCondition : public CCheckRoadWalkPassageCondition
{
public:
	CCheckRoadWalkOnewayCondition(void);
	~CCheckRoadWalkOnewayCondition(void);

	/**
	 * @brief	OWW�t�H�[�}�b�g�`�F�b�N
	 * @param	ipTable	[in]	�`�F�b�N�Ώۂ̃e�[�u��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	dbCtrl	[in,out]	��r�Ώۂ�DB���
	 * @return	bool
	 */
	virtual bool checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl);
};


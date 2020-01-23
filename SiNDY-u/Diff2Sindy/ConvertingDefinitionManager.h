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

#include <RowBase.h>
#include "Arguments.h"
#include "Conditions.h"

class AdditionalUpdate
{
public:
	AdditionalUpdate() : sindyHistory(false) {}
	bool sindyHistory; //!< SiNDY�^�C�v�̗���
	std::list<ConditionExpression> conditions;
};

/// ���[�v�^�C�v��`
enum LoopType : int
{
	LoopUnknown,
	LoopSRC,
	LoopDST,
};

class ConvertingDefinitionManager
{
public:
	/// �R���o�[�g���{��`������Ă��邩
	operator bool() const
	{
		return !m_listConditions.empty();
	}

	/**
	 * @brief  XML������͂���
	 * @param  path  [in]  XML�t�@�C���p�X
	 * @retval
	 */
	bool  parse(const uh::tstring& path);	

	/// DST���̏��������邩�ǂ���
	bool hasDstCondition() const;

	typedef std::pair<Condition,std::list<SPCondition>> Conditions;
	std::list<std::pair<LoopType,Conditions>> m_listConditions; //!< ���[�v�^�C�v���̊e�����
	AdditionalUpdate m_globalAdditionalUpdate; //!< �X�V���ǉ����i�S�̓K�p�j
	std::vector<std::pair<uh::tstring,IndexList>> m_listTarget; //!< �����Ώۃe�[�u�� first:�e�[�u���� second:�J�����Ή��\
};

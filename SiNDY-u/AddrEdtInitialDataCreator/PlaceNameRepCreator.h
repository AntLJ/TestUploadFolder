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
#include "BaseDataCreator.h"

/**
 * @brief   �n����\�|�C���g�f�[�^�����N���X�i�h���N���X�j
 */
class PlaceNameRepCreator : public BaseDataCreator
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   arg [in]  ����
	 */
	PlaceNameRepCreator(const Arguments& arg);

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~PlaceNameRepCreator() {}

	/**
	 * @brief   ������
	 */
	bool init();

	/**
	 * @brief   �f�[�^����
	 */
	bool create();

};


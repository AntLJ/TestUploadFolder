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

#include "SINDY2INHWYINFO.h"
#include "Customer.h"

/** 
 * @brief I/N�̎{�ݎ�ʃR�[�h�̎擾
 * @note	�^����ꂽ�I�u�W�F�N�g��������{�݂�Facil_Info_Point����SiNDY�̎{�ݎ�ʃR�[�h���擾���A�Ή�����I/N�̎{�ݎ�ʃR�[�h��Ԃ�
 * @param[in]	cpRow	�ΏۃI�u�W�F�N�g�iRoad_Code��Road_Seq�������Ă��邱�Ƃ��O��j
 * @param[in]	cpHwyInfo	Facil_Info_Point�擾�̂��߂�SINDY2INHWYINFO
 * @return	I/N�̎{�ݎ�ʃR�[�h
 */
long GetFacilClass(_IRow* cpRow, SINDY2INHWYINFO* cpHwyInfo);

/**
 * @brief �{�ݖ��̕ϊ����K�v�Ȏd�����悩�ۂ��𔻒肷��
 * @param[in]	cCustomer	�Ώێd������
 * @return	���茋��
 */
bool needFacilNameChange( Customer cCustomer );

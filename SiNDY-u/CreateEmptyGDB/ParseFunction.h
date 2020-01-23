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
#include "DataDef.h"

namespace parser
{

/**
 * @brief ���b�V�����X�g���~���Ƀ\�[�g�E���j�[�N�����ă�������ɓǍ���
 * @param filepath [in] ���b�V�����X�g�̃t�@�C���p�X�B
 * @param result   [out] �Ǎ��񂾌��ʂ̊i�[��B���g������ꍇ�A�㏑�������B
 * @retval true  �Ǎ��݂ɐ����B
 * @retval false �Ǎ��݂Ɏ��s�Bresult�͕K����ɂȂ�B
 */
bool meshListToList(const uh::tstring& filepath, std::vector<uh::tstring>& result);

/**
 * @brief �������ėǂ��^�O���ǂ�������
 * @param tag [in] ���肷��^�O
 * @retval true  �������Ă悢
 * @retval false ����`�̃^�O
 */
bool isIgnoreTag( const uh::tstring& tag );

/**
 * @brief XML�t�@�C������󃌃C�������p�̒�`��Ǎ���
 * @param filepath [in]  XML�t�@�C���̃p�X�B
 * @param retult   [out] �Ǎ��񂾌��ʂ̊i�[��B���g������ꍇ�A�㏑�������
 */
bool xmlToCreateDef(const uh::tstring& filepath, create_info::CreateInfo& result);

} // parser

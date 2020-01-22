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
 * @brief �������ėǂ��^�O���ǂ�������
 * @param tag [in] ���肷��^�O
 * @retval true  �������Ă悢
 * @retval false ����`�̃^�O
 */
bool isIgnoreTag( const uh::tstring& tag );

/**
 * @brief XML�t�@�C������C���f�b�N�X�����`��Ǎ���
 * @param filepath [in]  XML�t�@�C���̃p�X�B
 * @param retult   [out] �Ǎ��񂾌��ʂ̊i�[��B���g������ꍇ�A�㏑�������
 */
bool xmlToIndexDef(const uh::tstring& filepath, index_def::INDEXINFOMAP& result);

} // parser

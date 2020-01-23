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

/**
 * @brief �쐬����ێ�����N���X
 */
class CProductHolder
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param createInfoMap [in] ���C���E�h���C���E
	 * �t�B�[�`���f�[�^�Z�b�g�����̂��߂ɕK�v�ȏ��
	 */
	CProductHolder(const create_info::CREATEINFOMAP& createInfoMap):
		m_createInfoMap( createInfoMap )
	{}

	/**
	 * @brief �R���X�g���N�^�ŃZ�b�g�������ɏ]����
	 * ���C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g�𐶐�
	 * @retval true  �����ɐ���
	 * @retval false �����Ɏ��s
	 */
	bool create();

	/**
	 * @brief �����������C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g���擾
	 */
	const product::ProductDef& getResult() const { return m_productDef; }

protected:
	/// �������郌�C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g�̏��
	const create_info::CREATEINFOMAP& m_createInfoMap;
	/// ���������������郌�C���E�h���C���E�t�B�[�`���f�[�^�Z�b�g
	product::ProductDef m_productDef;
};

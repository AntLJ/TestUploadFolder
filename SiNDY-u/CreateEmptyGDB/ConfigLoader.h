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
 * @brief �ݒ�t�@�C���Ɠ���DB��Ǎ���
 */
class CConfigLoader
{
public:
	/// �R���X�g���N�^
	CConfigLoader(){}
	/// �f�X�g���N�^
	virtual ~CConfigLoader(){}

	/**
	 * @brief �ݒ�t�@�C����Ǎ���
	 * @detail filepath���󕶎���̏ꍇ�AinputDB���̑S���C���𐶐��ΏۂƂ���
	 * @param filepath [in] �ݒ�t�@�C���̃p�X
	 * @param inputStr [in] ����DB�̐ڑ�������
	 */
	bool loadConfig(const uh::tstring& filepath, const uh::tstring& inputStr);

	/**
	 * @brief �������郁�b�V�����C���̏����擾
	 * @param meshDBStr [in] �������郁�b�V�����C���̐ڑ�������
	 * @param layerName [in] �������郌�C����
	 */
	bool loadMesh(const uh::tstring& meshDBStr, const uh::tstring& layerName);

	/**
	 * @brief �Ǎ��񂾌��ʂ�Ԃ�
	 * @return ���C���E�f�[�^�Z�b�g�E�h���C�������̂��߂ɕK�v�ȏ��Q
	 */
	const create_info::CREATEINFOMAP& getResult() const { return m_createInfoMap; }

protected:
	create_info::CREATEINFOMAP m_createInfoMap; //!< ���C���E�f�[�^�Z�b�g�E�h���C�������̂��߂ɕK�v�ȏ��Q
};

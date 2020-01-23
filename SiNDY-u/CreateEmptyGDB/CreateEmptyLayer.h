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
 * @brief �Ώۂ�GDB�ɋ󃌃C�����쐬����
 */
class CCreateEmptyLayer
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param targetPathList [in] �������GDB�p�X�ꗗ
	 * @param meshlist       [in] ���b�V�����X�g�̓Ǎ��݌���
	 * @param productDef     [in] GDB�ɐ������郌�C���E
	 * �f�[�^�Z�b�g�E�h���C���̏��
	 */
	CCreateEmptyLayer(
		const std::vector<uh::tstring>& targetPathList
		, const std::vector<uh::tstring>& meshlist
		, const product::ProductDef& productDef
		):
		m_targetPathList(targetPathList)
		, m_meshlist(meshlist)
		, m_productDef(productDef)
	{}

	/**
	 * @brief �S�Ă̐�����ɋ󃌃C���E�f�[�^�Z�b�g�E�h���C�����쐬����
	 * @note �쐬��ɓ����̋󃌃C���E�f�[�^�Z�b�g�E�h���C�������łɑ��݂���ꍇ�A
	 * �쐬���Ȃ�
	 * @return �����̐���
	 */
	bool create();

protected:
	const std::vector<uh::tstring>& m_targetPathList; //!< �������GDB�p�X�ꗗ
	const std::vector<uh::tstring>& m_meshlist; //!< ���b�V�����X�g�̓Ǎ��݌���
	/// GDB�ɐ������郌�C���E�f�[�^�Z�b�g�E�h���C���̏��
	const product::ProductDef& m_productDef;

};

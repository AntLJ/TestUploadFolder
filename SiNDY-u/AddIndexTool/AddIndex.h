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
 * @brief �C���f�b�N�X��t�^����N���X
 */
class CAddIndex
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param db           [in] �Ώ�DB�̐ڑ�������
	 * @param indexInfoMap [in] �C���f�b�N�X�t�^�ɕK�v�ȏ��
	 */
	CAddIndex(
		const uh::tstring& db
		, const index_def::INDEXINFOMAP& indexInfoMap):
		m_db(db)
		, m_indexInfoMap(indexInfoMap)
	{}

	/**
	 * @brief �C���f�b�N�X�t�^���������s
	 * @retval true  �����ɐ���
	 * @retval false �����Ɏ��s
	 */
	bool execute();

protected:
	/**
	 * @brief �X�L�[�}�����b�N����
	 * @param layer     [in] ���b�N�Ώۃ��C��
	 * @param layerName [in] ���b�N�Ώۃ��C���̖��O
	 * @retval true  ���b�N�ɐ���
	 * @retval false ���b�N�Ɏ��s
	 */
	bool lockSchema(ITablePtr& layer, const uh::tstring& layerName);

	/**
	 * @brief �X�L�[�}�̃��b�N����������
	 * @note ���b�N�����Ɏ��s�����ꍇ�A�G���[���b�Z�[�W��f��
	 * @param layer     [in] ���b�N�Ώۃ��C��
	 * @param layerName [in] ���b�N�Ώۃ��C���̖��O
	 */
	void releaseSchema(ITablePtr& layer, const uh::tstring& layerName);

	/**
	 * @brief �Ώۃ��C���ɃC���f�b�N�X�t�^���������s
	 * @param layer         [in] �Ώۃ��C��
	 * @param layerName     [in] �Ώۃ��C���̖��O
	 * @param indexInfoList [in] �C���f�b�N�X����Ɋւ�����
	 */
	void addIndex(ITablePtr& layer
		, const uh::tstring& layerName
		, const std::vector<index_def::IndexInfo>& indexInfoList);

	/**
	 * @brief �Ώۃ��C���ɑ��݂���C���f�b�N�X���t�^���ꂽ�t�B�[���h�ꗗ���擾
	 * @param layer [in] �Ώۃ��C��
	 * @return �C���f�b�N�X���Ƃ��̃C���f�b�N�X���t�^���ꂽ�t�B�[���h�����X�g�̑Ή��\
	 */
	std::map<uh::tstring, std::vector<uh::tstring> > loadIndexes(const ITablePtr& layer);

protected:
	const uh::tstring m_db; //!< �Ώ�DB�̐ڑ�������
	/// �C���f�b�N�X�t�^�ɕK�v�ȏ��Q
	const index_def::INDEXINFOMAP& m_indexInfoMap;
};

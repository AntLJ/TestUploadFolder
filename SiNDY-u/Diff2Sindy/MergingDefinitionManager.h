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

#include <iostream>
#include <string>
#include <vector>
#include "Conditions.h"

/// �}�[�W���̑����R�s�[���@��`
enum AttributeSelector : int
{
	selectorAny, //!< �ǂ���ł��悢
	selectorNotNull, //!< NULL����Ȃ��ق�
	selectorBigger, //!< �傫����
	selectorSmaller, //!< ��������
};

/**
 * @brief �}�[�W���{��`�N���X
 */
class MergingDefinition
{
public:
	MergingDefinition();
	MergingDefinition(const MergingDefinition &o);
	~MergingDefinition();

	uh::tstring m_strTableName;					//!< �}�[�W����e�[�u������
	Condition m_cCondition;
	IndexList m_idxList; //!< �J�����Ή��\
};

//�}�[�W���{��`�Ǘ��N���X
class MergingDefinitionManager
{
public:
	MergingDefinitionManager() : m_setHistory(false) {}
	~MergingDefinitionManager(){}
	MergingDefinitionManager(const MergingDefinitionManager &o);

	/// �}�[�W���{��`������Ă��邩�ǂ���
	operator bool() const
	{
		return ! m_mergingDefinitions.empty();
	}

	/**
	 * @brief  �}�[�W���{��`�N���X�̐���Ԃ�
	 * @retval �}�[�W���{��`�N���X�̐�
	 */
	const unsigned size() const{ return m_mergingDefinitions.size(); }

	/**
	 * @brief  �C���f�b�N�X�ɑΉ�����}�[�W���{��`�N���X��Ԃ�(const��)
	 * @param  i  [in]  �C���f�b�N�X
	 * @retval �}�[�W���{��`�N���X
	 */
	const MergingDefinition& get (unsigned i) const
	{
		assert(i < m_mergingDefinitions.size());
		assert(i >= 0);
		return m_mergingDefinitions[i];
	}

	/**
	 * @brief  �C���f�b�N�X�ɑΉ�����}�[�W���{��`�N���X��Ԃ�
	 * @param  i  [in]  �C���f�b�N�X
	 * @retval �}�[�W���{��`�N���X
	 */
	const MergingDefinition& get(unsigned i)
	{
		assert(i < m_mergingDefinitions.size());
		assert(i >= 0);
		return m_mergingDefinitions[i];
	}

	/**
	 * @brief  XML������͂���
	 * @param  path  [in]  XML�t�@�C���p�X
	 * @retval
	 */
	bool  parse(const uh::tstring& path);

	/**
	* @brief �ۑ����ɗ�����t�^���邩�ǂ���
	* @retval false ������t�^���Ȃ�
	* @retval true ������t�^����
	*/
	bool isSetHistory() const { return m_setHistory; }
private:
	std::vector<MergingDefinition> m_mergingDefinitions;	//!< �}�[�W���{��`�N���X�̃��X�g
	bool m_setHistory; //!< ������t�^���邩�ǂ���
};

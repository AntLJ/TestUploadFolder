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

typedef uh::tstring ClippingDefinition;

/// �N���b�s���O����P��
enum ClipType : int
{
	clipTypeMesh,  //!< ���b�V�����E�ŃN���b�s���O
	clipTypeLot, //!< ���b�g���E�ŃN���b�s���O
};

//�N���b�s���O���{��`�Ǘ��N���X
class ClippingDefinitionManager
{
public:
	ClippingDefinitionManager() : m_setHistory(false), m_emClipType(clipTypeMesh) {}
	~ClippingDefinitionManager(){}
	ClippingDefinitionManager(const ClippingDefinitionManager &o);

	/// �N���b�s���O���{��`������Ă��邩�ǂ���
	operator bool() const
	{
		return ! m_ClippingDefinitions.empty();
	}

	/**
	 * @brief  �N���b�s���O���{��`�N���X�̐���Ԃ�
	 * @retval �N���b�s���O���{��`�N���X�̐�
	 */
	const unsigned size() const{ return m_ClippingDefinitions.size(); }

	/**
	 * @brief  �C���f�b�N�X�ɑΉ�����N���b�s���O���{��`�N���X��Ԃ�(const��)
	 * @param  i  [in]  �C���f�b�N�X
	 * @retval �N���b�s���O���{��`�N���X
	 */
	const ClippingDefinition& get (unsigned i) const
	{
		assert(i < m_ClippingDefinitions.size());
		assert(i >= 0);
		return m_ClippingDefinitions[i];
	}

	/**
	 * @brief  �C���f�b�N�X�ɑΉ�����N���b�s���O���{��`�N���X��Ԃ�
	 * @param  i  [in]  �C���f�b�N�X
	 * @retval �N���b�s���O���{��`�N���X
	 */
	const ClippingDefinition& get(unsigned i)
	{
		assert(i < m_ClippingDefinitions.size());
		assert(i >= 0);
		return m_ClippingDefinitions[i];
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

	/// �N���b�v�����̃^�C�v���擾����
	ClipType getClipType() const { return m_emClipType; }
private:
	std::vector<ClippingDefinition> m_ClippingDefinitions;	//!< �N���b�s���O���{��`�N���X�̃��X�g
	bool m_setHistory; //!< ������t�^���邩�ǂ���
	ClipType m_emClipType; //!< �N���b�s���O����P��
};

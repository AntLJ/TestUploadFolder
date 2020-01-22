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

namespace index_def
{

/**
 * @brief �C���f�b�N�X�̑���^�C�v
 */
enum TYPE
{
	NONE,//!< ��`�O
	ADD, //!< �C���f�b�N�X�t�^
	DEL, //!< �C���f�b�N�X�폜
};

/**
 * @brief �C���f�b�N�X����̂��߂ɕK�v�ȏ��̒�`
 */
class IndexInfo
{
public:
	/// �R���X�g���N�^
	IndexInfo():
		m_type(ADD)
		, m_order_asc(true)
		, m_unique(true)
	{}

	uh::tstring m_name;	//!< �C���f�b�N�X�𐶐�����t�B�[���h�̖���
	TYPE m_type;        //!< �C���f�b�N�X�̑���^�C�v
	bool m_order_asc;   //!< �C���f�b�N�X�̕��я�(true:����, false:�~��)
	bool m_unique;      //!< �C���f�b�N�X�����j�[�N���ۂ�
};


/**
 * @brief �C���f�b�N�X�t�^�̂��߂ɕK�v�ȏ���ݒ�t�@�C���̒�`���Ŋi�[
 * <���C����, �C���f�b�N�X���>
 */
class INDEXINFOMAP
{
protected:
	/// <���C����, �C���f�b�N�X���>
	typedef std::pair<uh::tstring,std::vector<IndexInfo>> VALUETYPE;

	std::vector<VALUETYPE> m_valueMap; // ��`��ێ�
public:
	/**
	 * @brief �w��̃��C�����̃C���f�b�N�X�����擾
	 * @detail �w��̃��C�����̏�񂪑��݂��Ȃ��ꍇ�A�쐬����
	 * @param name [in] ���C����
	 * @return ���C�����ɑΉ�����C���f�b�N�X���
	 */
	std::vector<IndexInfo>& getIndexInfo(const uh::tstring& name);

	/**
	 * @brief �w��̃��C�����̏�񂪑��݂��邩����
	 * @param name [in] ���C����
	 * @retval true  [in] ���݂���
	 * @retval false [in] ���݂��Ȃ�
	 */
	bool isExist(const uh::tstring& name) const;

	/**
	 * @brief �擪�̃C�e���[�^��Ԃ�
	 * @return �擪�̃C�e���[�^
	 */
	std::vector<VALUETYPE>::iterator begin()
	{
		return m_valueMap.begin();
	}
	/**
	 * @brief �����̃C�e���[�^��Ԃ�
	 * @return �����̃C�e���[�^
	 */
	std::vector<VALUETYPE>::iterator end()
	{
		return m_valueMap.end();
	}

	/**
	 * @brief �擪�̃C�e���[�^��Ԃ�
	 * @return �擪�̃C�e���[�^
	 */
	std::vector<VALUETYPE>::const_iterator begin() const
	{
		return m_valueMap.cbegin();
	}
	/**
	 * @brief �����̃C�e���[�^��Ԃ�
	 * @return �����̃C�e���[�^
	 */
	std::vector<VALUETYPE>::const_iterator end() const
	{
		return m_valueMap.cend();
	}
};

} // index_def

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
 * @brief �w��̃p�X�ɋ��GDB���쐬����
 */
class CCreateEmptyGDB
{
	/**
	 * @brief �t�H���_�̊K�w�\����`
	 */
	struct FolderTree
	{
		uh::tstring m_folderName;                //!< �t�H���_��
		std::vector<FolderTree> m_children;      //!< �t�H���_�̎q�v�f
		boost::optional<uh::tstring> m_fileName; //!< �t�H���_�����ɐ�������t�@�C����
	};

public:
	/// �R���X�g���N�^
	CCreateEmptyGDB(){}
	/// �f�X�g���N�^
	virtual ~CCreateEmptyGDB(){}

	/**
	 * @brief ��������DB�̃t�H�[�}�b�g��ݒ�
	 * @detail output�Ƀt�H���_���w�肵�Ă���ꍇ�A�s�v
	 * @param format [in] �t�H�[�}�b�g
	 */
	void setFormat(const uh::tstring& format);

	/**
	 * @brief DB�̐�����p�X��ݒ�
	 * @param path [in] ������̃p�X
	 * @param type [in] ��������DB�̃^�C�v
	 */
	void setOutput( const uh::tstring& path, OUTPUT_TYPE type);

	/**
	 * @brief ���b�V�����X�g��ݒ肷��
	 * @param mesh [in] ���b�V�����X�g
	 */
	void setMeshList( const std::vector<uh::tstring>& meshList );

	/**
	 * @brief ������̃t�H���_�E�t�@�C���𐶐�
	 * @retval true �v���I�Ȗ�肪�N���炸�Ƀt�@�C�����쐬�ł���
	 * @retval false �v���I�Ȗ�肪�N������
	 */
	bool create();

	/**
	 * @brief �����ΏۂɂȂ���DB�̃p�X���X�g�̎Q�Ƃ��擾
	 */
	const std::vector<uh::tstring>& getTargetPathList() const
	{
		return m_targetPathList;
	}

protected:
	/**
	 * @brief filepath�̉ӏ��̋��DB���쐬����
	 * @param filename [in] �쐬����t�@�C���̖��O(�g���q����)
	 * @param ext      [in] �쐬����t�@�C���̊g���q(.�t��)
	 * @param basePath [in] �쐬��̃t�H���_�p�X
	 * @return �����̐���
	 */
	bool createFile(const uh::tstring& filename, const uh::tstring& ext, const uh::tstring& basePath);

	/**
	 * @brief �K�w�I�Ƀt�H���_�E�t�@�C�����쐬����
	 * @param folderPath [in] �쐬��̃t�H���_�p�X
	 * @return �����̐���
	 */
	bool createLevelFolder(const uh::tstring& folderPath);

	/**
	 * @brief FolderTree�̏������Ƀt�H���_�E�t�@�C�����쐬����
	 * @note �t�@�C���E�t�H���_�����݂��Ȃ��ꍇ�̂݁A�쐬����
	 * @param node     [in] �쐬����t�H���_�E�t�@�C���̊K�w�\�������\����
	 * @param rootPath [in] �t�H���_�쐬�̃��[�g�ƂȂ�t�H���_
	 * @retval true  �t�@�C���E�t�H���_�̐����ɐ���
	 * @retval false �t�@�C���E�t�H���_�̐����Ɏ��s
	 */
	bool createFilesFromTree(const FolderTree& node, const boost::filesystem::path& rootPath);

private:
	uh::tstring m_format;     //!< ��������DB�̃t�H�[�}�b�g
	uh::tstring m_output;     //!< �o�͐�p�X
	OUTPUT_TYPE m_outputType; //!< �o�͐�^�C�v
	std::vector<uh::tstring> m_meshList; //!< ���b�V�����X�g

	/**
	 * @brief create()�̑ΏۂƂȂ���DB�̃p�X�ꗗ
	 * @detail �쐬����Ă��Ȃ��Ă��A�ΏۂɂȂ������_�ł��̃p�X�ɒǉ������
	 */
	std::vector<uh::tstring> m_targetPathList;
};

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

#include <string>
#include <vector>


/**
 * @brief IWorkspaceEdit �Ɏq�o�[�W�����쐬���Ă̕ҏW���T�|�[�g�����������̃��b�p�[�N���X
 */
class ChildVerEditWorkspace
{
public:
	/**
	 * @brief �f�t�H���g�R���X�g���N�^
	 */
	ChildVerEditWorkspace(void)
		: m_IsVersioned(false), m_ChildCreateMode(false), m_IsCreatedChild(false), m_IsEditFailed(false) {}

	/**
	 * @brief �R���X�g���N�^
	 * @param[in] ipWorkspace IWorkspace�I�u�W�F�N�g
	 */
	explicit ChildVerEditWorkspace(const IWorkspacePtr &ws)
		: m_Workspace( ws ), m_IsVersioned(false), m_ChildCreateMode(false), m_IsCreatedChild(false), m_IsEditFailed(false) {}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~ChildVerEditWorkspace(void){}

	/**
	 * @brief IWorkspace �̂��߂̃I�y���[�^
	 */
	operator IWorkspacePtr() const
	{
		return m_Workspace;
	}

	/**
	 * @brief IWorkspace �̗L�����̊m�F�����̃I�y���[�^
	 */
	operator bool() const
	{
		return m_Workspace;
	}

	/**
	 * @brief IWorkspace ����p�̃I�y���[�^
	 */
	ChildVerEditWorkspace& operator = (const IWorkspacePtr &ipWorkspace)
	{
		m_Workspace = ipWorkspace;
		return (*this);
	}

	/**
	 * @brief ���[�N�X�y�[�X�̃I�[�v��
	 * @param[in] connection DB�ڑ����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool open(const CString& connection);

	/**
	 * @brief �e�[�u���̃I�[�v��
	 * @param[in] table_name  �e�[�u����
	 * @return �I�[�v�����ꂽ�e�[�u��
	 */
	ITablePtr openTable(const CString& table_name);

	/**
	 * @param[in] �ҏW�J�n���Ɏq�o�[�W�������쐬���郂�[�h�ɐݒ肷��
	 * @detail   �@�w�肵���e�[�u�����S�ăo�[�W�����Ή��łȂ���΁A�G���[
	 * @param[in] ver_name     �q�o�[�W������
	 * @param[in] edit_targets �ҏW�Ώۂ̃e�[�u�������X�g(�o�[�W�����Ή��ł��邩�̔��f�Ɏg�p)
	 * @retval  true  �ݒ�ɐ���
	 * @retval  false �ݒ�Ɏ��s
	 */
	bool childCreateModeOn(const CString& ver_name, const std::vector<CString>& edit_targets);

	/**
	 * @brief �ҏW�J�n
	 * @retval true  �ҏW�J�n����
	 * @retval false �ҏW�J�n���s
	 */
	bool startEditing();

	/**
	 * @brief �ҏW�I��
	 * @param[in] �Z�[�u���邩�ۂ�
	 * @retval true  �ҏW�J�n����
	 * @retval false �ҏW�J�n���s
	 */
	bool stopEditing(bool bSave);

	/**
	 * @brief EditOperation �J�n
	 * @retval true  EditOperation �J�n����
	 * @retval false EditOperation �J�n���s
	 */
	bool startEditOperation();

	/**
	 * @brief EditOperation �I��
	 * @retval true  EditOperation �I������
	 * @retval false EditOperation �I�����s
	 */
	bool stopEditOperation();

	/**
	 * @brief EditOperation �� Abort ����
	 * @details �o�[�W�����Ή��ɂ̂ݎg�p��
	 * @retval true  EditOperation �I������
	 * @retval false EditOperation �I�����s
	 */
	bool abortEditOperation();

	/**
	 * @brief �ڑ��悪SDE�ł��邩
	 * @retval true  SDE
	 * @retval false FGDB or PGDB
	 */
	bool isSDE();

private:
	/**
	 * @brief �q�o�[�W�����̍쐬���A�Ǘ����郏�[�N�X�y�[�X�̑Ώۂ��q�o�[�W�����ɃX�C�b�`����
	 * @retval true  �q�o�[�W�����쐬����
	 * @retval false �q�o�[�W�����쐬���s
	 */
	bool createChildVer();

	/**
	 * @brief �q�o�[�W�������폜����
	 * @note  �q�o�[�W�������쐬���Ă��āA���ҏW�Ɏ��s���Ă���ꍇ�̂݁A�폜�������s��
	 * @retval true  �q�o�[�W�����폜����
	 * @retval false �q�o�[�W�����폜���s
	 */
	bool deleteChildVer();

private:
	IWorkspaceEditPtr m_Workspace;		//!< ���[�N�X�y�[�X

	bool m_IsVersioned;			//!< �ڑ����DB���o�[�W�����Ή����ۂ�
	bool m_ChildCreateMode;		//!< �ҏW�J�n����ƁA�쐬�����q�o�[�W�����ɑ΂��ĕҏW���J�n����B

	CString m_ChildVerName;		//!< �q�o�[�W������
	bool m_IsCreatedChild;		//!< �q�o�[�W�������쐬������

	bool m_IsEditFailed;		//!< �ҏW�Ɏ��s������
};

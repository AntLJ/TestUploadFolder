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

/**
 * @class	TableFinder
 * @brief	�e�[�u���Ǘ��N���X
 * @note	���̃N���X��ʂ��ăe�[�u�����擾����
 *			��x�I�[�v�������炻������̂܂܎g�p�ł���
 *			���[�N�X�y�[�X�E�t�B�[���h�C���f�b�N�X���Ǘ��ł���
 */
class TableFinder
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	TableFinder(void);

	 /**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~TableFinder(void);

	/**
	 * @brief	���[�N�X�y�[�X�ǉ�
	 * @param	[in] lpcszKey                ���[�N�X�y�[�X�L�[
	 * @param	[in] lpcszWorkspaceKey       ���[�N�X�y�[�X�L�[���[�h
	 * @retval	true:����  false:���s
	 */
	bool addWorkspace( const CString& lpcszKey, const CString& lpcszWorkspaceKey );

	/**
	 * @brief	���Z�b�g
	 * @note	���[�N�X�y�[�X���X�g�E�e�[�u�����X�g�E�t�B�[���h�}�b�v���X�g�S�č폜
	 */
	void reset()
	{
		m_mapWorkspace.clear();
		m_mapTable.clear();
	}

	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @note	�I�[�v������Ă��Ȃ��Ƃ��́A�I�[�v������
	 * @param	[in] lpcszKey		���[�N�X�y�[�X�L�[
	 * @return	IWorkspacePtr
	 */
	IWorkspacePtr getWorkspace( const CString& lpcszKey );

	/**
	 * @brief	�e�[�u������(���[�N�X�y�[�X�w���)
	 * @param	[in] lpcszWorkspaceKey       ���[�N�X�y�[�X�L�[(db_map��)
	 * @param	[in] lpcszTable              �e�[�u����(�L�[)
	 * @return	ITable
	 */
	ITablePtr findTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable );

private:

	/**
	 * @brief	�e�[�u���I�[�v��
	 * @param	[in] lpcszWorkspaceKey       ���[�N�X�y�[�X�L�[
	 * @param	[in] lpcszTable              �e�[�u����
	 * @return	ITable
	 */
	ITablePtr openTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable );	

private:

	typedef std::map< CString, CAdapt<IWorkspacePtr> >	WORKSPACEMAP;   //!< ���[�N�X�y�[�X�}�b�v
	typedef std::map< CString, CAdapt<ITablePtr> >		TABLEMAP;       //!< �e�[�u���}�b�v

	WORKSPACEMAP                m_mapWorkspace;                     //!< ���[�N�X�y�[�X���X�g
	TABLEMAP                    m_mapTable;                         //!< �e�[�u�����X�g

};

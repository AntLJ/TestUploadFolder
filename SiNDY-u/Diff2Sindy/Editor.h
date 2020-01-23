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
#include <workspace.h>

/// �o�[�W�����Ή���Ԏ��ʃR�[�h
enum ObjectType : int
{
	ObjectTypeUnknown, //!< �s��
	ObjectTypeVersioned, //!< �o�[�W�����Ή��̂�
	ObjectTypeNonVersioned, //!< �o�[�W������Ή��̂�
	ObjectTypeMix, //!< �o�[�W�����Ή��Ɣ�Ή�������
};

/**
 * @brief �ҏW�Z�b�V�����Ǘ��N���X
 *
 * �o�[�W�����Ή��Ɣ�Ή������݂��Ă��邩�̂����̃`�F�b�N�@�\��A
 * �R���t���N�g���Ƀ��R���T�C�����|�X�g������@�\���ۗL���܂��B
 */
class CEditor
{
public:
	CEditor() : m_undoredo(false){}
	CEditor( const sindy::CWorkspace& work ) : m_work(work) ,m_undoredo(false), m_versioned(false){}

	/**
	* @brief �w�肳�ꂽ�e�[�u���̃��X�g�̃o�[�W�����Ή��󋵂�Ԃ�
	* retval ObjectTypeVersioned �S�ăo�[�W�����Ή�����Ă���
	* retval ObjectTypeNonVersioned �S�ăo�[�W�����Ή�����ĂȂ�
	* retval ObjectTypeMix �o�[�W�����Ή��Ɣ�Ή������݂��Ă���
	*/
	ObjectType CheckObjectType( const std::list<uh::tstring>& listTableName ) const;

	/**
	* @brief �ҏW���J�n����
	* @param undoredo [in] UNDO/REDO��L���ɂ���ꍇ��true
	* @param versioned [in,optional] true�Ȃ�o�[�W�����Ή��Ƃ��ĕҏW���J�n����i�f�t�H���g�Ftrue�j
	* @retval true �ҏW�J�n����
	* @retval false �ҏW�J�n���s
	*/
	bool StartEditing(bool undoredo, bool versioned = true);

	/**
	* @brief �ҏW���I������
	* @note �ҏW�I�����A�R���t���N�g���Ă���ꍇ�ɂ͎����I�Ƀ��R���T�C�����|�X�g���������s���܂��B
	* @param save [in] true�Ȃ�ҏW��ۑ����ďI������Bfalse�Ȃ�j�����ďI������B
	* @retval true �ҏW�I������
	* @retval false �ҏW�I�����s
	*/
	bool StopEditing(bool save);

	/**
	* @brief �ҏW��ۑ�����
	*
	* �����I�ɂ́AStopEditing -> StartEditing���Ă��邾���ł��B
	* @retval true �ҏW�ۑ�����
	* @retval false �ҏW�ۑ����s
	*/
	bool SaveEditing();

	/**
	* @brief �ҏW�Z�b�V�������J�n����
	* @note StartEditing����Ă���K�v������܂�
	* @retval true �ҏW�Z�b�V�����J�n����
	* @retval false �ҏW�Z�b�V�����J�n���s
	*/
	bool StartOperation();

	/**
	* @brief �ҏW�Z�b�V�������I������
	* @retval true �ҏW�Z�b�V�����I������
	* @retval false �ҏW�Z�b�V�����I�����s
	*/
	bool StopOperation();

	/**
	* @brief �ҏW�Z�b�V������j������
	* @retval true �ҏW�Z�b�V�����j������
	* @retval false �ҏW�Z�b�V�����j�����s
	*/
	bool AbortOperation();

	/**
	* @brief �e�[�u���̃o�[�W�����Ή��󋵂ɉ����ĕҏW���J�n����
	* @note �o�[�W�����Ή��Ɣ�Ή������݂���ꍇ�͕ҏW���J�n���Ȃ�
	* @param listTableName [in] �����Ώۂ̃e�[�u���̃��X�g
	* @retval true �ҏW�J�n����
	* @retval false �ҏW�J�n���s
	*/
	bool StartEditingByObjectType( const std::list<uh::tstring>& listTableName );

	const sindy::CWorkspace& Workspace() const { return m_work; }
	sindy::CWorkspace& Workspace() { return m_work; }
private:
	sindy::CWorkspace m_work; //!< �ҏW�J�n�I���Ώۃ��[�N�X�y�[�X
	bool m_undoredo; //!< UNDO/REDO��L���ɂ��邩�ǂ����̊Ǘ��t���O
	bool m_versioned; //!< �o�[�W�����Ή��Ƃ��ĕҏW�J�n���邩�A�o�[�W������Ή��Ƃ��ĕҏW�J�n���邩�̊Ǘ��t���O
};


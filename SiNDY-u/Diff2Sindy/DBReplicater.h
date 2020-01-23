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

namespace sindy {
	class CWorkspace;
	class CRowBase;
	class CTableContainer;
} // sindy

/**
 * @brief �e�[�u���R�s�[�N���X
 */
class DBReplicater
{
public:
	DBReplicater();
	~DBReplicater();

	/**
	 * @brief �e�[�u�����R�s�[����
	 *
	 * �����X�L�[�}�Ńe�[�u�����R�s�[���܂��B<br>
	 * ���݂��郌�R�[�h�͑S���R�s�[����܂��B
	 *
	 * @param work [in] �R�s�[�Ώۂ̃e�[�u�������郏�[�N�X�y�[�X
	 * @parm name [in] �R�s�[���e�[�u����
	 * @param work_new [in] �R�s�[��̃e�[�u�������郏�[�N�X�y�[�X
	 * @param name_new [in] �y�[�X�g��e�[�u����
	 * @note �y�[�X�g��̃e�[�u�������݂���ꍇ�͏����Ɏ��s���܂�
	 */
	bool CopyTable2( const sindy::CWorkspace& work, const uh::tstring& name, const sindy::CWorkspace& work_new, const uh::tstring& name_new );

	/**
	 * @brief �e�[�u�����R�s�[����
	 * @param work [in] �R�s�[�Ώۂ̃e�[�u�������郏�[�N�X�y�[�X
	 * @parm name [in] �R�s�[���e�[�u����
	 * @param work_new [in] �R�s�[��̃e�[�u�������郏�[�N�X�y�[�X
	 * @param name_new [in] �y�[�X�g��e�[�u����
	 * @see CopyTable2( const CWorkspace& work, const uh::tstring& name, const uh::tstring& name_new )
	 * @note �y�[�X�g��̃e�[�u�������݂���ꍇ�͏����Ɏ��s���܂�
	 * @note ���S�̂��߁ASDE��ł͓��삳���Ȃ��悤�ɂȂ��Ă��܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool CopyTable( const sindy::CWorkspace& work, const uh::tstring& name, const sindy::CWorkspace& work_new, const uh::tstring& name_new );

	/**
	 * @brief �����ɍ��v������̂̂݃��R�[�h�ǉ�����
	 *
	 * �w��J�����������v����΁A���f�����قȂ�e�[�u���Ԃł��I�u�W�F�N�g�̃R�s�[���ł��܂��B
	 * �����t�B�[�`���N���X�̏ꍇ�́A����t�B�[�`���N���X�ł���K�v������܂��i�t�͂��̌���ł͂Ȃ��j�B
	 *
	 * @param filter [in] �N�G���t�B���^
	 * @param srcWorkspace [in] �R�s�[�����[�N�X�y�[�X
	 * @param srcTable [in] �R�s�[���e�[�u��
	 * @param destWorkspace [in] �y�[�X�g�惏�[�N�X�y�[�X
	 * @param destTable [in] �y�[�X�g��e�[�u����
	 * @param listColumns [in] �R�s�[�ΏۃJ�������X�g
	 * @param all_columns [in,optional] true�Ȃ炷�ׂẴJ�������R�s�[�ΏۂƂ���i�f�t�H���g�Ffalse�j
	 * @note �y�[�X�g��̃e�[�u�������݂��Ȃ��Ə����Ɏ��s���܂�
	 * @note �t�B�[�`���N���X�̏ꍇ�̓W�I���g���͎����I�ɃR�s�[�ΏۂƂȂ�܂�
	 * @retval true ����
	 * @retval false ���s
	 */
	bool InsertTable( IQueryFilterPtr filter, IWorkspacePtr srcWorkspace, const uh::tstring& srcTable, IWorkspacePtr destWorkspace, const uh::tstring& destTable, const std::list<uh::tstring>& listColumns, bool all_columns = false );

private:

};

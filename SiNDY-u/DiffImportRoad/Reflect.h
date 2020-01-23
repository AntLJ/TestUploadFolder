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
#include "LayerManage.h"

namespace Reflect{
	extern const long NO_CHG_MODE;	///< �e�X�g���[�h�i�X�V���Ȃ��j�r�b�g�t���O
	extern const long TDC_MODE;		///< TDC�D�惂�[�h �r�b�g�t���O
	extern bool g_IsContinue;		///< �����p���t���O�i���f�Ǘ��p�j
	extern long g_NowProccess;		///< ���ݏ������̃v���Z�X�i���o�[�i���Ԗځj
	extern long g_Mode;				///< ���샂�[�h
	extern long g_CurrentMesh;		///< �������̃��b�V���R�[�h
	extern MESH_SET g_MeshSet;		///< �����Ώۂ̃��b�V�����X�g�i�Z�b�g�j
	extern CString g_Operator;		///< �ŏI�X�V�҃Z�b�g�p�̃��[�U��
	extern CString g_FixUser;		///< FIX���[�U���i�폜�n�̃��O�o�͗p�j
}

/**
 * @brief �ҏW�������f�𑍍��I�ɊǗ��E���s����N���X
 */
class CReflect
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CReflect(void);

	/**
	 * @brief �f�X�g���N�^
	 */
	~CReflect(void);

	/**
	 * @brief �������E���O�����B�ŏ��Ɉ�񂾂��Ăׂ΂�������
	 *
	 * @param cConnectStr	[in]	�T�[�o�ڑ�������
	 * @param cOldPath		[in]	�ҏW�OPGDB�̃x�[�X�p�X
	 * @param cNewPath		[in]	�ҏW��PGDB�̃x�[�X�p�X
	 * @param cMeshList		[in]	���b�V�����X�g�t�@�C����
	 * @param cMode			[in]	���샂�[�h
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool init_once(const CString cConnectStr[2], LPCTSTR cOldPath, LPCTSTR cNewPath, LPCTSTR cMeshList, long cMode);

	/**
	 * @brief �������E���O�����B���f�������ɍŏ��������K�v�Ȃ��̂��܂�
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool CReflect::init(void);

	/**
	 * @brief �ҏW�������f��ƃ��C���֐�
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool execReflect(void);

private:
	CLayerManage m_LayerManage;	///< SDE�̃��[�N�X�y�[�X�E�t�B�[�`���N���X���̊Ǘ��N���X
	CString m_OldPath;		///< �ҏW�OPGDB�̃x�[�X�p�X
	CString m_NewPath;		///< �ҏW��PGDB�̃x�[�X�p�X
	CString m_ConnectStr[2];	///< �T�[�o�ڑ�������

	/**
	 * @brief ����̂ݕK�v�ȏ������E���O�����B�t�H���_�̗L�����ꕔ�͊O�Ŏ��s
	 *
	 * @param cMeshList	[in]	���b�V�����X�g�t�@�C����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnFirstOnlyInit(LPCTSTR cMeshList);

	/**
	 * @brief �R�}���h�t�����b�V�����X�g�̓ǂݍ��݂ƃ��b�V���|���S���擾
	 *
	 * @param cMeshList	[in]	���b�V�����X�g�t�@�C����
	 *
	 * @retval true		����
	 * @retval false	���s�A�G���[
	 */
	bool fnMeshLoad(LPCTSTR cMeshList);
};

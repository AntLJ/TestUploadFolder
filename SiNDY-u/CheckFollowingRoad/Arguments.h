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

#include "MeshType.h"

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments() : m_MeshType( meshtype::kUnknown ) {}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);
	
	/**
	 * @brief ���b�V�����X�g�̉��
	 * @note �������b�V���P�ʂ̎擾�A�����Ώۂ̃��b�V���R�[�h�ꗗ�̎擾�A���X�g���̕s���ȕ�����̎擾���s��
	 * @param	meshes [out] �����Ώۂ̃��b�V���R�[�h�̈ꗗ
	 * @param	mapInvalidRow [out]	���b�V�����X�g���̕s���ȕ�����ƍs�ԍ��̃y�A
	 * @retval	true	���b�V�����X�g�̓��e�ɖ��Ȃ�
	 * @retval	false	�s���ȕ����񂪑��݁A�܂���2��ވȏ�̃��b�V�������݂��Ă���
	 */
	bool parseMeshes( std::vector<long>& meshes, std::map<int, CString>& mapInvalidRow );

public:
	uh::tstring	m_BaseDB;	//!< ��r���o�[�W������DB
	uh::tstring m_CompDB;	//!< ��r��o�[�W������DB
	uh::tstring m_MeshList;	//!< ���b�V�����X�g
	CComVariant m_ModDate;	//!< �ŏI�X�V��
	uh::tstring m_Runlog;	//!< ���s���O
	uh::tstring m_Errlog;	//!< �G���[���O

	meshtype::MeshType m_MeshType;	//!< �������郁�b�V���P��
};

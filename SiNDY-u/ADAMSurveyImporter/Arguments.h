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
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments();

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  �f�[�^�쐬���̊m�F
	 * @param  optValue [in]  company�I�v�V�����l
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParamCompany(const uh::tstring& optValue);

	/**
	 * @brief  �f�[�^��̓��̊m�F
	 * @param  optValue [in]  company�I�v�V�����l
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParamDate(const uh::tstring& optValue);
public:
	//�f�[�^�쐬�� �Œ�l
	static const uh::tstring PS;	//!< �p�X�R
	static const uh::tstring AT;	//!< �A�C�T��
	static const uh::tstring KK;	//!< ���ۍq��
	static const uh::tstring IPC;	//!< iPC

	uh::tstring		survey_dir;		//!< [in]�C���|�[�g�t�@�C���f�B���N�g���p�X
	uh::tstring		import_db;		//!< �C���|�[�g��DB�ڑ����
	uh::tstring		company;		//!< �f�[�^�쐬��
	uh::tstring		date;			//!< �f�[�^��̓�
	uh::tstring		lot;			//!< ���b�g��
	uh::tstring		log_dir;		//!< ���O�o�͐�t�H���_�p�X
	uh::tstring		crd_param;		//!< ���W�ϊ��p�p�����[�^�t�@�C��

	bool			unuse_version;		//!< �o�[�W�������p���邩(true:���Ȃ�)
	bool			skip_delete_record;	//!< �X�V�R�[�h�u�폜�v�̃��R�[�h���X�L�b�v���邩
};

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
	 * @brief �p�����[�^�̒ǉ��`�F�b�N���[�h
	 */
	enum ChkMode
	{
		kNone,		//!< �Ȃ�
		kIsFile,	//!< �t�@�C���p�X�`�F�b�N
		kIsFolder	//!< �t�H���_�p�X�`�F�b�N
	};

	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments(){}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param[in]	argc  �R�}���h���C�������̐�
	 * @param[in]	argv  �R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);


private:

	/**
	 * @brief  �R�}���h���C�������̔ėp�`�F�b�N
	 * @param[in]  optName   �I�v�V������
	 * @param[in]  optValue  �I�v�V�����l
	 * @param[in]  bChkMode  �I�v�V�����̒ǉ��`�F�b�N���[�h�w�� 
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, ChkMode eChkMode = kNone ) const;

	/**
	 * @brief  latlon�I�v�V�����̃`�F�b�N
	 * @param[in]  strOptValue   �I�v�V�����l
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckLatLonParam( const uh::tstring& strOptValue ) const;

public:
	uh::tstring  m_input_db;		//!< DB�ڑ���
	uh::tstring  m_name_rule_ini;	//!< �C���X�g���������[�����t�@�C��
	uh::tstring  m_output_ini;		//!< �C���X�g�o�͏��ݒ�t�@�C�� 
	uh::tstring  m_output_dir;		//!< �o�̓t�H���_ 
	uh::tstring  m_runlog;			//!< �o�̓��O 
	uh::tstring  m_errlog;			//!< �G���[���O 
	uh::tstring  m_latlon;			//!< �ܓx�o�x�o�͐ݒ� 

	uh::tstring  m_road_db;			//!< DB�ڑ���(���H�m�[�h)

};

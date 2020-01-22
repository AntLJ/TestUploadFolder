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
	Arguments(){}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~Arguments(){}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param[in]	argc 	�R�}���h���C�������̐�
	 * @param[in]	argv 	�R�}���h���C�������̓��e
	 * @retval[in]	true	����
	 * @retval[in]	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @retval true    ����OK
	 * @retval false   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue);

public:
	uh::tstring  m_roadDB;    //!< ���HDB�ڑ����
	uh::tstring  m_adamDB;    //!< ADAM DB�ڑ����

	uh::tstring  m_meshListStr;     //!< ���b�V���R�[�h�A�܂��̓��b�V�����X�g�t�@�C���p�X
	std::vector<long> m_meshList;   //!< ���b�V�����X�g

	uh::tstring  m_logDir;    //!< ���O�o�̓t�H���_�p�X
};

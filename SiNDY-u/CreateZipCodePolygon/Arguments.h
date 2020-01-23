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
	Arguments() : m_executeMode(ExecuteMode::none)
	{
	}

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
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam(const char* const optName, const uh::tstring& optValue);

	/**
	 * @brief �R�}���h���C���I�v�V�����ꗗ�̎擾
	 * retval CString �S�I�v�V�����Ǝw�肵���l
	 */
	CString GetAllOptionInfo();

public:
	uh::tstring m_cityDb; //!< �s�s�n�}�s���E�|���S���ڑ���
	uh::tstring m_jusyoUser; //!< �Z�����[�U��(�p�X���[�h������������Ƃ���)
	uh::tstring m_jusyoDb; //!< �Z��DB��(delltcp�Aproteus)
	uh::tstring m_outputDir; //!< shp�t�@�C���o�͐�
	uh::tstring m_shapeName; //!< �X�֔ԍ��|���S���t�@�C����
	uh::tstring m_tableName; //!< �X�֔ԍ��e�[�u���t�@�C����
	uh::tstring m_runLog; //!< ���s���O�o�͐�p�X
	uh::tstring m_errLog; //!< �G���[���O�o�͐�p�X

	uh::tstring m_outputDb; //!< �o�͐�DB
	ExecuteMode m_executeMode; //!< ���s���[�h
};

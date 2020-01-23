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
#include "DataDef.h"

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
	 * @brief output�^�C�v���擾
	 * @return output�^�C�v
	 */
	OUTPUT_TYPE getOutputType() const { return m_outputType; }

protected:
	/**
	 * @brief  �K�{�ƂȂ�R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true    ����OK
	 * @retval false   ����NG
	 */
	bool checkParam( const char* const optName, const uh::tstring& optValue, bool bFile = false ) const;

	/**
	 * @brief �������t�@�C���ł��邱�Ƃ��m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @retval true  �t�@�C���ł���
	 * @retval false �t�@�C���ł͂Ȃ�
	 */
	bool checkFile( const char* const optName, const uh::tstring& optValue ) const;

	/**
	 * @brief �t�@�C���p�X���c�[���őΉ����Ă���g���q�������Ă��邩�m�F
	 * @param path [in] �m�F����t�@�C���p�X
	 */
	bool checkFormat( const uh::tstring& path ) const;

public:
	uh::tstring m_input;         //!< ���͏��
	uh::tstring m_output;        //!< �o�͏��
	uh::tstring m_meshList;      //!< ���b�V�����X�g
	uh::tstring m_meshDB;        //!< ���b�V���Q�Ɛ��DB
	uh::tstring m_meshLayerName; //!< ���b�V�����C����
	uh::tstring m_dbFormat;      //!< ��������DB�̃t�H�[�}�b�g
	uh::tstring m_config;        //!< ��`�t�@�C���p�X
	uh::tstring m_runLog;        //!< ���s���O�t�@�C���p�X
	uh::tstring m_errLog;        //!< �G���[���O�t�@�C���p�X

protected:
	OUTPUT_TYPE m_outputType;  //!< ��͂����o�͐�^�C�v���i�[
};

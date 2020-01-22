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
	Arguments(){ m_mode = _T("2"); }

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
	 * @param  bInt     [in]  ���l�ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false, bool bInt = false );

public:
	uh::tstring  m_mode;      //!< ���s���[�h(�f�t�H���g:2(�m�[�h���X�g+�C���X�g�`�F�b�N))
	uh::tstring  m_config;    //!< �R���t�B�O�t�@�C��
	uh::tstring  m_namerule;  //!< �C���X�g�t�@�C�����������[��
	uh::tstring  m_illustdb;  //!< �C���X�g�ڑ���
	uh::tstring  m_roaddb;    //!< ���H�ڑ���
	uh::tstring  m_runlog;    //!< ���s���O
	uh::tstring  m_errlog;    //!< �G���[���O
};

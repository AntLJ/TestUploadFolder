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

#include "IllustSettings.h"
#include "LogMgr.h"
#include "IllustNameTable.h"
#include <boost/program_options.hpp>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/illustlib/IllustLinkRow.h>

/**
 * @brief �C���X�g�t�@�C�����Ή��\�쐬�N���X
 */
class CreateIllustNameTable
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CreateIllustNameTable();

	/**
	 * @brief �f�X�g���N�^
	 */
	~CreateIllustNameTable();

	/**
	 * @brief ������
	 * @param argc [in] �����̐�
	 * @param argv [in] �������X�g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool init( int argc, _TCHAR* argv[] );

	/**
	 * @brief �����̎��s
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool execute();

private:
	/**
	 * @brief �����̃`�F�b�N
	 * @param argc [in] �����̐�
	 * @param argv [in] �������X�g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool checkArg( int argc, _TCHAR* argv[] );

	/**
	 * @brief �����p�����[�^�̎擾
	 * @param strName [in] ������
	 * @param strParam [out] �擾�����p�����[�^
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getArgParam( const std::string& strName, CString& strParam );

	/**
	 * @brief �t�@�C���̑��݂��m�F
	 * @param strFileName [in] �Ώۃt�@�C����
	 * @retval true  �t�@�C�������݂���
	 * @retval false �t�@�C�������݂��Ȃ�
	 */
	bool checkFileExistence( const CString& strFileName );

	/**
	 * @brief �G���[���O��W�����O�t�H�[�}�b�g�ŏo��
	 * @param illustLinkRow [in] �Ώۂ̃C���X�g�����N
	 * @param errLv [in] �G���[���x��
	 * @param strErrCode [in] �G���[�R�[�h
	 * @param strErrMsg [in] �G���[���b�Z�[�W
	 * @param strOption [in] �ǉ����
	 */
	void outputErrLog( const sindy::CSPIllustLinkRow& illustLinkRow, LogMgr::EErrLevel errLv, const CString& strErrCode, const CString& strErrMsg, const CString& strOption = _T("") );

private:
	boost::program_options::variables_map	m_varMap;	//!< �I�v�V�����i�[�}�b�v

	CString		m_strRunLog;	//!< ���s���O�t�@�C����
	CString		m_strErrLog;	//!< �G���[���O�t�@�C����

	CString		m_strDBConnect;	//!< DB�ڑ�������
	CString		m_strOutFile;	//!< �C���X�g�t�@�C�����Ή��\�t�@�C����
	CString		m_strIniFile;	//!< �������ݒ�t�@�C����

	int			m_nErrCounter;	//!< �G���[���̃J�E���^

	IllustSettings		m_cIllustSettings;	//!< �������ݒ�N���X

	sindy::CTableFinder	m_ipTableFinder;	//!< �e�[�u���t�@�C���_

	IllustNameTable		m_cIllustNameTable;	//!< �C���X�g�t�@�C�����Ή��\�N���X
};

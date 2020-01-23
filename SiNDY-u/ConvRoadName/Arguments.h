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
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/directory_util.h>


/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments()
	{
	}

	/**
	 * @brief �f�X�g���N�^
	 */
	~Arguments()
	{
	}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param argc [in] �R�}���h���C�������̐�
	 * @param argv [in] �R�}���h���C�������̓��e
	 * @retval true ����
	 * @retval false ���s
	 */
	bool parse(int argc, TCHAR* argv[]);

private:
	/**
	 * @brief �����`�F�b�N
	 * @retval true �`�F�b�NOK
	 * @retval false �`�F�b�NNG
	 */
	bool chkOptions();

	/*
	 * @brief �t�@�C�����݃`�F�b�N
	 * @param file [in] �`�F�b�N�Ώۃt�@�C��
	 * @retval true ���݂���
	 * @retval false ���݂��Ȃ�
	 */
	bool fileExistChk(const CString& file);

	/**
	 * @brief �f�B���N�g�����݃`�F�b�N
	 * @param dir [in] �`�F�b�N�Ώۃf�B���N�g��
	 * @retval true ���݂���
	 * @retval false ���݂��Ȃ�
	 */
	bool dirExistChk(const CString& dir);

	/**
	 * @brief �t�@�C���i�[�f�B���N�g�����݃`�F�b�N
	 * @param filePath [in] �`�F�b�N�Ώۃt�@�C��
	 * @retval true ���݂���
	 * @retval false ���݂��Ȃ�
	 */
	bool fileDirExistChk(const CString& filePath);

	/**
	 * @brief ���O�̏�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool initLog();

	/**
	* @brief �R�}���h���C�������o�͗p������擾
	* @return �R�}���h���C������������
	*/
	CString GetOption() const;

public:
	boost::program_options::variables_map m_vm;
	uh::tstring m_roadDB;
	uh::tstring m_roadOwner;
	uh::tstring m_nameDB;
	uh::tstring m_nameOwner;
	uh::tstring m_dicPath;
	uh::tstring m_outDir;
	uh::tstring m_runLog;
	uh::tstring m_errLog;
};


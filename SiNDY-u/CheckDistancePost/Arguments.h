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

namespace sindy
{
/**
 * @class CArguments
 * @brief �R�}���h���C�������N���X
 * @file  Arguments.h
 * @author �r�b�g�G�C�W�@�`
 * $Id$
*/
class CArguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CArguments()
	{
	}

	/**
	 * @brief �f�X�g���N�^
	 */
	~CArguments(){}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param argc [in] �R�}���h���C�������̐�
	 * @param argv [in] �R�}���h���C�������̓��e
	 * @return �����������ǂ���
	*/
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief �I�v�V���������ꗗ�쐬
	*/
	void GetOptionParam();

public:
	uh::tstring m_strDb;                    ///< ���̓p�����[�^ ����DB
	uh::tstring	m_strList;                  ///< ���̓p�����[�^ �ΏۘH���ꗗ�t�@�C���p�X
	uh::tstring	m_strRoadCost;              ///< ���̓p�����[�^ �Ώۓ��H��ʃR�X�g��`�t�@�C���p�X
	uh::tstring	m_strLinkCost;              ///< ���̓p�����[�^ �Ώۃ����N��ʃR�X�g��`�t�@�C���p�X
	uh::tstring m_strPitch;                 ///< ���̓p�����[�^ �s�b�`
	uh::tstring	m_strLogPath;               ///< ���̓p�����[�^ ��ƃ��O�o�̓p�X
	uh::tstring m_strBuf;                   ///< ���̓p�����[�^ �����N����o�b�t�@
	uh::tstring m_strRange;                 ///< ���̓p�����[�^ ���e�͈�
	uh::tstring m_strSeparate;              ///< ���̓p�����[�^ 2�𕪗����͈�

	CString	m_strOpParam;                   ///< �I�v�V���������ꗗ������
};

}

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

#include <TDC/illustlib/IllustOutputInfo.h>

/**
 * @brief �������ݒ背�R�[�h�N���X
 */
class IllustSettingsRec
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	IllustSettingsRec() {}

	/**
	 * @brief �f�X�g���N�^
	 */
	~IllustSettingsRec() {}

	/**
	 * @brief �C���X�g�t�@�C�������K���̐ݒ�
	 * @param strNamingRule [in] �����K��������i�J���}��؂�j
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool setNamingRule( const std::string& strNamingRule );

public:
	int						m_nIllustClass;		//!< �C���X�g��ʃR�[�h
	int						m_nCustomer;		//!< �d����R�[�h
	CString					m_strPrefix;		//!< �C���X�g�t�@�C���̃v���t�B�b�N�X
	CString					m_strExtension;		//!< �C���X�g�t�@�C���̊g���q
	std::vector<CString>	m_vecNamingRule;	//!< �C���X�g�t�@�C���̖����K��
	CString					m_strIllustDir;		//!< �C���X�g�t�@�C���̊i�[��f�B���N�g��
	CIllustOutputInfo::OutputInfo	m_cOutputInfo;	//!< �m�[�h���X�g�o�͏��t�@�C�����R�[�h�i�o�͏��O�����̂��߂Ɏg�p�j
	// ���{���� std::vector<CIllustOutputInfo::ExceptionCond> �݂̂𗘗p���������A�o�͏��O�����ɍ��v���邩�ǂ������肷��
	// CIllustOutputInfo::IsException() �̃p�����[�^�Ƃ��� CIllustOutputInfo::OutputInfo �����߂��邽�߁A���̂悤�ɂ��Ă���
};

/**
 * @brief �������ݒ�N���X
 * @note  �u�C���X�g��ʃR�[�h�v�u�d����R�[�h�v�̑g�ipair�j���L�[�Ƃ���map�Ń��R�[�h����ێ�����
 */
class IllustSettings : public std::map< std::pair< int, int >, IllustSettingsRec >
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	IllustSettings();

	/**
	 * @brief �f�X�g���N�^
	 */
	~IllustSettings();

	/**
	 * @brief �������ݒ�t�@�C����ǂݍ���
	 * @param strFileName [in] �������ݒ�t�@�C����
	 * @param ipIllustLinkTable [in] �C���X�g�����N�e�[�u��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool load( const CString& strFileName, const ITablePtr& ipIllustLinkTable );

private:
	/**
	 * @brief �C���X�g��ʃR�[�h�E�d����R�[�h���R�[�h�l�h���C���͈̔͂Ɋ܂܂�邩���`�F�b�N
	 * @param ipIllustLinkTable [in] �C���X�g�����N�e�[�u��
	 * @retval true  �R�[�h�l�h���C���͈̔͂Ɋ܂܂��
	 * @retval false �R�[�h�l�h���C���͈̔͂Ɋ܂܂�Ȃ�
	 */
	bool checkCodesRange( const ITablePtr& ipIllustLinkTable );

	/**
	 * @brief �C���X�g�t�@�C���̊i�[��f�B���N�g�������݂��邱�Ƃ��`�F�b�N
	 * @retval true  ���ׂẴf�B���N�g�������݂���
	 * @retval false ���݂��Ȃ��f�B���N�g��������
	 */
	bool checkDirExistence();

private:
	CIllustOutputInfo	m_cIllustOutputInfo;	//!< �m�[�h���X�g�o�͏��t�@�C���Ǘ��N���X�i�o�͏��O�����̏����̂��߂Ɏg�p�j
};

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
#include <boost/program_options.hpp>

/**
 * @brief �c�[���x�[�X�N���X
 */
class CToolBase
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CToolBase();

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CToolBase();

	/**
	 * @brief  ������
	 * @param  argc [in]  �R�}���h���C��������
	 * @param  argv [in]  �R�}���h���C������������
	 * @return bool ����ɏ������ł������ǂ���
	 */
	virtual bool Init( int argc, _TCHAR* argv[] ) = 0;

	/**
	 * @brief  �������s���z�֐�
	 * @return bool ����ɏ������I���������ǂ���
	 */
	virtual bool Execute() = 0;

protected:
	/**
	 * @brief  ���O�t�@�C�����I�[�v������֐�
	 * @note   �t�@�C���I�[�v���������ɁA�c�[�����ƈ������o�͂��邽�߂Ƀv���O�����I�v�V�����������ɕK�v
	 * @param  strLogFileName [in] ���O�t�@�C����
	 */
	bool OpenLogFile( const CString& strLogFileName );

	/**
	 * @brief  ���O�t�@�C���Ƀ��O���������ފ֐�
	 * @note   �W���o�͂���K�v�̂Ȃ����O�̏ꍇ�ɂ́A��2������false�ɂ��邱��
	 * @param  strLog  [in] �������ރ��O
	 * @param  bDisp   [in] �W���o�͂����邩�ǂ����i�f�t�H���g:true�j
	 */
	void WriteLog( const CString& strLog, bool bDisp = true );

	/**
	 * @brief  ���O�w�b�_�����O�ɏ������ފ֐�
	 * @note   �w�b�_���������ޏꍇ�̓T�u�N���X���ŃI�[�o�[���C�h
	 */
	virtual void WriteLogHeader();

	/**
	 * @brief  ���O�t�@�C�����N���[�Y����֐�
	 */
	void CloseLogFile();

private:
	/**
	 * @brief  ���O�t�@�C���Ƀv���O�����J�n�Ƃ��ăc�[�����A�I�v�V�����������o�͂���֐�
	 */
	void StartLogFile();

	/**
	 * @brief  ���O�t�@�C���Ƀv���O�����I�������Ə������ʂ��o�͂���֐�
	 */
	void EndLogFile();

	/**
	 * @brief  ���O�t�@�C���Ƀc�[�����ƃo�[�W�������o�͂���֐�
	 */
	void WriteFileVersion();

	/**
	 * @brief  ���O�t�@�C���ɃI�v�V�����������o�͂���֐�
	 */
	void WriteOptionString();

protected:
	boost::program_options::variables_map	m_variable_map;	//!< �v���O�����I�v�V�����i�[�}�b�v

	CString			m_strLogFileName;	//!< ���O�o�̓t�@�C����
	bool			m_bIsOK;			//!< �c�[��������I���������ǂ����i�p���c�[�����Ő���ɏI��������true���Z�b�g���Ă�����K�v������j
	std::ofstream	m_fStream;			//!< ���O�o�͗p�t�@�C���X�g���[��
};

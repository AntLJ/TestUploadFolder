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
 * @brief ���O�Ǘ��N���X�i�V���O���g���j
 */
class LogMgr
{
public:
	/**
	 * @brief �G���[���x��
	 */
	enum EErrLevel
	{
		kNone	= 0,	//!< �G���[���x���Ȃ�
		kInfo	= 1,	//!< ���
		kWarn	= 2,	//!< �x��
		kError	= 3,	//!< �G���[
		kFatal	= 4,	//!< �v���I�ȃG���[
	};

private:
	/**
	 * @brief �R���X�g���N�^
	 */
	LogMgr();

	/**
	 * @brief �R�s�[�R���X�g���N�^
	 * @param logMgr [in] �R�s�[���I�u�W�F�N�g
	 */
	LogMgr( const LogMgr& logMgr );

	/**
	 * @brief �f�X�g���N�^
	 */
	~LogMgr();

	/**
	 * @brief ������Z�q
	 * @param logMgr [in] �R�s�[���I�u�W�F�N�g
	 * @return �N���X�I�u�W�F�N�g���g
	 */
	LogMgr& operator = ( const LogMgr& logMgr );

	/**
	 * @brief �o�[�W�������o��
	 */
	void writeVersion();

	/**
	 * @brief �J�n�����o��
	 */
	void writeStartTime();

	/**
	 * @brief �I�������o��
	 */
	void writeEndTime();

public:
	/**
	 * @brief ���s���O�t�@�C����ݒ�
	 * @param strFileName [in] ���s���O�t�@�C����
	 * @param varMap [in] �I�v�V�����i�[�}�b�v�i�I�v�V�����j
	 * @return �����������ǂ���
	 */
	bool setRunLog( const CString& strFileName, const boost::program_options::variables_map* varMap = nullptr );

	/**
	 * @brief �G���[���O�t�@�C����ݒ�
	 * @param strFileName [in] �G���[���O�t�@�C����
	 * @return �����������ǂ���
	 */
	bool setErrLog( const CString& strFileName );

	/**
	 * @brief �I�v�V�������̏o��
	 * @param varMap [in] �I�v�V�����i�[�}�b�v
	 */
	void writeOptionInfos( const boost::program_options::variables_map& varMap );

	/**
	 * @brief �C���X�^���X�̎擾
	 * @return �C���X�^���X
	 */
	static LogMgr& getInstance()
	{
		static LogMgr logMgr;
		return logMgr;
	}

	/**
	 * @brief ���s���O�t�@�C���ւ̏o��
	 * @param strMessage [in] �o�̓��b�Z�[�W
	 */
	void writeRunLog( const CString& strMessage );

	/**
	 * @brief �G���[���O�t�@�C���ւ̏o��
	 * @param strMessage [in] �o�̓��b�Z�[�W
	 */
	void writeErrLog( const CString& strMessage );

	/**
	 * @brief �G���[���O�t�@�C���ւ̏o��
	 * @param errLv [in] �G���[���x��
	 * @param strMessage [in] �o�̓��b�Z�[�W
	 */
	void writeErrLog( EErrLevel errLv, const CString& strMessage );

	/**
	 * @brief �G���[���O���x���𕶎���ɕϊ�
	 * @param errLv [in] �G���[���x��
	 * @return �G���[���x����\�����镶����
	 */
	static CString getErrLvString( EErrLevel errLv );

	/**
	 * @brief �o�[�W������񕶎�����擾
	 * @note �u<���s�t�@�C����> F: x.x.x.x P: x.x.x.x�v�̌`���Ŏ擾
	 * @return �o�[�W������񕶎���
	 */
	static CString getVersionInfo();

private:
	std::ofstream	m_ofsRun;	//!< ���s���O�o�̓t�@�C���X�g���[��
	std::ofstream	m_ofsErr;	//!< �G���[���O�o�̓t�@�C���X�g���[��
};

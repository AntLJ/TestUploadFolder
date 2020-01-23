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

#include <fstream>
#include <boost/noncopyable.hpp>
#include <mutex>

#include "ErrDef.h"

#define RUNLOGGER CRunLogger::GetLogger()
#define ERRLOGGER CErrLogger::GetLogger()

/**
 * @brief  ���O�x�[�X�N���X
 */
class CLogger
{
public:

	/**
	 * @brief  �R���X�g���N�^
	 */
	CLogger(void){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CLogger(void){ Finalize(); }


public:

	/**
	 * @brief  �t�@�C���I�[�v��
	 */
	bool Initialize( const uh::tstring& dir_path );

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	virtual void Log( const uh::tstring& msg );

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << �I�y���[�^�̃I�[�o�[���[�h
	 */
	virtual CLogger& operator<<( const uh::tstring& msg );

private:

	/**
	 * @brief  �t�@�C�����Ԃ�
	 * @return �t�@�C����
	 */
	virtual uh::tstring getFileName() = 0;

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() = 0;

private:

	std::ofstream m_Log;		//!< ���O�t�@�C���X�g���[��

};


/**
 * @brief  ���s���O�N���X
 * @note   �V���O���g��
 */
class CRunLogger : public CLogger,
                   private boost::noncopyable
{
public:

	/**
	 * @brief  �ÓI�I�u�W�F�N�g�擾
	 */
	static std::mutex s_mutex;
	static CRunLogger& GetLogger()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		static CRunLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  �G���[���x���t���̃��b�Z�[�W�o��
	 * @param msg [in] ���b�Z�[�W
	 */
	void Fatal( const uh::tstring& msg );
	void Error( const uh::tstring& msg );
	void Warn( const uh::tstring& msg );
	void Info( const uh::tstring& msg );
	void Debug( const uh::tstring& msg );

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CRunLogger(void){};

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CRunLogger(void){};

	/**
	 * @brief  �t�@�C�����Ԃ�
	 * @return �t�@�C����
	 */
	virtual uh::tstring getFileName();

	/**
	 * @brief  �w�b�_�[�o��
	 */
	void writeHeader();

};


/**
 * @brief  �G���[���O�N���X
 * @note   �V���O���g��
 */
class CErrLogger : public CLogger,
                   private boost::noncopyable
{
public:

	/**
	 * @brief  �ÓI�I�u�W�F�N�g�擾
	 */
	static std::mutex s_mutex;
	static CErrLogger& GetLogger()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		static CErrLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  �G���[���x���t���̃��b�Z�[�W�o��
	 * @param layer [in] �G���[�Ώۃ��C��
	 * @param oid   [in] �G���[�ΏۃI�u�W�F�N�g��OID
	 * @param err   [in] �G���[���
	 */
	void Fatal( const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Error( const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Warn(  const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Info(  const uh::tstring& layer, long oid, const err_def::ErrPair& err );
	void Debug( const uh::tstring& layer, long oid, const err_def::ErrPair& err );

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CErrLogger(void){};

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CErrLogger(void){};

	/**
	 * @brief  �t�@�C�����Ԃ�
	 * @return �t�@�C����
	 */
	virtual uh::tstring getFileName();

	/**
	 * @brief  �w�b�_�[�o��
	 */
	void writeHeader();

};


namespace log_utility
{
	/**
	 * @brief  �t�@�C�����̃v���t�B�b�N�X���擾
	 * @return �v���t�B�b�N�X("�c�[����_YYYYMMDDHHMMSS_")��Ԃ�
	 */
	static uh::tstring GetLogfileNamePrefix();

	/**
	 * @brief  �����̏��𕶎���ŕԂ�
	 * @return ������� ( [YYYY-MM-DD HH:MM::SS] )
	 */
	static uh::tstring GetDateTimeInfo();
}

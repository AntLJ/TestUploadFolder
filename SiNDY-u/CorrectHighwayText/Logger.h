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
#include "Arguments.h"

#define RUNLOGGER CRunLogger::GetLogger()

/**
* @brief  ���O�x�[�X�N���X
*/
class CLogger
{
public:

	/**
	* @brief  �R���X�g���N�^
	*/
	CLogger(void) {}

	/**
	* @brief  �f�X�g���N�^
	*/
	virtual ~CLogger(void) { Finalize(); }


public:

	/**
	 * @brief  �t�@�C���I�[�v��
	 * @param args [in] �R�}���h���C������
	 */
	bool Initialize( const Arguments& args);

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	virtual void Log(const uh::tstring& strMessage);

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	virtual void Log(const TCHAR* format, ...);

	/**
	 * @brief  << �I�y���[�^�̃I�[�o�[���[�h
	 */
	virtual CLogger& operator<<(const uh::tstring& strMessage);

private:
	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader(const Arguments& args) = 0;

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
	 * @param strMessage [in] ���b�Z�[�W
	 */
	void Fatal(const uh::tstring& strMessage);
	void Error(const uh::tstring& strMessage);
	void Warn(const uh::tstring& strMessage);
	void Info(const uh::tstring& strMessage);
	void Debug(const uh::tstring& strMessage);

	/**
	 * @brief  �I�����O�o��
	 * @param ret [in] ��������/���s
	 */
	void writeExitLog(bool ret);

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CRunLogger(void) {}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CRunLogger(void) {}

	/**
	 * @brief  �w�b�_�[�o��
	 */
	void writeHeader(const Arguments& args) override;

};

namespace log_utility
{
	/**
	 * @brief  �����̏��𕶎���ŕԂ�
	 * @return ������� ( [YYYY-MM-DD HH:MM::SS] )
	 */
	static uh::tstring GetDateTimeInfo();
}

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
	bool Initialize( const uh::tstring& strDirPath );

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	virtual void Log( const uh::tstring& strMessage );

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << �I�y���[�^�̃I�[�o�[���[�h
	 */
	virtual CLogger& operator<<( const uh::tstring& strMessage );

private:

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() = 0;

private:

	std::ofstream m_Log;		//!< ���O�t�@�C���X�g���[��

};


/**
 * @brief  ���s���O�N���X
 */
class CRunLogger : public CLogger, 
                   private boost::noncopyable
{
public:

	/**
	 * @brief  �ÓI�I�u�W�F�N�g�擾
	 */
	static CRunLogger& GetLogger()
	{
		static CRunLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  ���O��񁕌��ݎ����o��
	 */
	void writeInfo( const CString& info );

	/**
	 * @brief  �R�}���h���C�������̓��e�o��
	 * @param arg [in] �R�}���h���C������
	 */
	void writeOption( const Arguments& arg );

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CRunLogger( void ) :CLogger(){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CRunLogger(void){}

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() override;

};


/**
 * @brief  �G���[���O�N���X
 */
class CErrLogger : public CLogger, 
                   private boost::noncopyable
{
public:

	/**
	 * @brief  �ÓI�I�u�W�F�N�g�擾
	 */
	static CErrLogger& GetLogger()
	{
		static CErrLogger m_inst;
		return m_inst;
	}

	/**
	 * @brief  ���b�Z�[�W�o�́iERROR�j
	 * @param layer		[in] ���C��
	 * @param oid		[in] �I�u�W�F�N�gID
	 * @param gid		[in] �O���[�o��ID
	 * @param errMsg	[in] ���b�Z�[�W
	 * @param meshCode	[in] ���b�V���R�[�h
	 * @param source	[in] ���H���E�������N��SOURCE
	 */
	void error( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source );

	/**
	 * @brief  ���b�Z�[�W�o�́iWARNING�j
	 * @param layer		[in] ���C��
	 * @param oid		[in] �I�u�W�F�N�gID
	 * @param gid		[in] �O���[�o��ID
	 * @param errMsg	[in] ���b�Z�[�W
	 * @param meshCode	[in] ���b�V���R�[�h
	 * @param source	[in] ���H���E�������N��SOURCE
	 */
	void warning( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source );

	/**
	 * @brief  ���b�Z�[�W�o�́iINFO�j
	 * @param layer		[in] ���C��
	 * @param oid		[in] �I�u�W�F�N�gID
	 * @param gid		[in] �O���[�o��ID
	 * @param errMsg	[in] ���b�Z�[�W
	 * @param meshCode	[in] ���b�V���R�[�h
	 * @param source	[in] ���H���E�������N��SOURCE
	 */
	void info( const CString& layer, const CString& oid, const CString& gid, const CString& errMsg, const CString& meshCode, const CString& source );

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CErrLogger(void) :CLogger(){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CErrLogger(void){}

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() override;

};


namespace log_utility
{
	/**
	 * @brief  �����̏��𕶎���ŕԂ�
	 * @return ������� ( [YYYY-MM-DD HH:MM::SS] )
	 */
	CString GetDateTimeInfo();
}

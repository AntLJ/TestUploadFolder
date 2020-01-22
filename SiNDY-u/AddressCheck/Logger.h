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
#include "ErrorDef.h"

#define RUNLOGGER RunLogger::getLogger()

/**
 * @brief  ���O�x�[�X�N���X
 */
class Logger
{
public:

	/**
	 * @brief  �R���X�g���N�^
	 */
	Logger(void){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~Logger(void){ finalize(); }

public:

	/**
	 * @brief  �t�@�C���I�[�v��
	 */
	bool initialize( const CString& strDirPath );

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void finalize() { m_log.close(); }
	
	/**
	 * @brief  �t�@�C���t���b�V��
	 */
	void flush() { m_log.flush(); }
	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	void log( const CString& strMessage );

	/**
	 * @brief  ���b�Z�[�W�o��
	 */
	void log( const TCHAR* format, ... );

	/**
	 * @brief  << �I�y���[�^�̃I�[�o�[���[�h
	 */
	Logger& operator<<( const CString& strMessage );

private:

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() = 0;

	std::ofstream m_log;		//!< ���O�t�@�C���X�g���[��

};


/**
 * @brief  ���s���O�N���X
 * @note   �V���O���g��
 */
class RunLogger : public Logger, 
                   private boost::noncopyable
{
public:

	/**
	 * @brief  �ÓI�I�u�W�F�N�g�擾
	 */
	static std::mutex s_mutex;
	static RunLogger& getLogger()
	{
		std::lock_guard<std::mutex> lock(s_mutex);

		static RunLogger m_inst;
		return m_inst;
	}

private:
	
	/**
	 * @brief  �R���X�g���N�^
	 */
	RunLogger( void ) :Logger(){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~RunLogger(void){}

	/**
	 * @brief  �w�b�_�[�o��
	 */
	void writeHeader() override;

};


/**
 * @brief  �G���[���O�N���X
 * @note   �V���O���g��
 */
class ErrLogger : public Logger, 
                   private boost::noncopyable
{
public:
		/**
	 * @brief  �R���X�g���N�^
	 */
	ErrLogger(void) :Logger(){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~ErrLogger(void){}

	/**
	 * @brief �o�͊֐�
	 * @param [in] emCode           �G���[�R�[�h
 	 * @param [in] lpcszLayer       ���C����
	 * @param [in] oID              �I�u�W�F�N�gID
	 * @param [in] ipGeometry       �W�I���g��(�G���[�ʒu)
	 * @param [in] lpcszFormat      ���2-n�̏o�̓t�H�[�}�b�g(�P�Ȃ炱���ɒl������)
	 * @param [in] ...              ���2-n
	 */
	void write( emCheck_ErrCode emCode, LPCTSTR lpcszLayer, long oID, const IGeometryPtr& ipGeometry, LPCTSTR lpcszFormat = NULL, ...);

private:
	
	/**
	 * @brief  �w�b�_�[�o��
	 */
	void writeHeader() override;

	/**
	 * @brief	�o�͊֐�����
	 * @param	[in] cErrInfo        �G���[���
	 * @param	[in] lpcszFormat     ���2-n�̏o�̓t�H�[�}�b�g
	 * @param	[in] vaInfo          ���2-n
	 */
	void writeCommon( const ErrInfo& cErrInfo, LPCTSTR lpcszFormat = NULL, va_list vaInfo = NULL );

		/**
	 * @brief	�G���[���x���擾
	 * @param	[in] emLevel		�G���[���x��
	 * @return	�G���[���x��
	 */
	LPCTSTR getErrorLevel( emErrLevel emLevel ) const;

	/**
	 * @brief	�G���[���b�Z�[�W�o��
	 * @param	[in] emCode		�G���[�R�[�h
	 * @return	�G���[���x��<tab>�G���[�R�[�h<tab>�G���[���b�Z�[�W
	 */
	CString getErrorString( emCheck_ErrCode emCode ) const;

	/**
	 * @brief	�o�x�E�ܓx�擾
	 * @param	[in]  ipGeometry    �W�I���g��
	 * @param	[out] point         WKSPoint
	 */
	void getLonLat( const IGeometryPtr& ipGeometry, WKSPoint& point )const;
};

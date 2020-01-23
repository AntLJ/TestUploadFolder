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
	CLogger(){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CLogger(){ Close(); }

public:

	/**
	 * @brief  �t�@�C���I�[�v��
	 * @param[in] filepath �t�@�C���p�X
	 * @retval true  �t�@�C���̃I�[�v���ɐ���
	 * @retval false �t�@�C���̃I�[�v���Ɏ��s
	 */
	bool Initialize( const uh::tstring& filepath );

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void Close() { m_Log.close(); }

	/**
	 * @brief  ���b�Z�[�W�o��
	 * @param[in] strMessage �o�͂��郁�b�Z�[�W
	 * @param[in] lineBreak ���s���邩�ۂ��i�f�t�H���g:true�j
	 */
	virtual void Log( const uh::tstring& strMessage, bool lineBreak = true );

	/**
	 * @brief 1�v�f��1�s�Ƃ��ă��b�Z�[�W�o��
	 * @param[in] strMessages �o�͂��郁�b�Z�[�W�Q
	 */
	virtual void Log( const std::vector<uh::tstring>& strMessages);

	/**
	 * @brief  ���b�Z�[�W�o��
	 * @detail ���s����Ȃ��B���s����ꍇ�̓t�H�[�}�b�g���ɉ��s����������B
	 * @param[in] format �o�͂��郁�b�Z�[�W�̃t�H�[�}�b�g
	 * (printf�̏����Ɠ���)
	 */
	virtual void FormatLog( const TCHAR* format, ... );

	/**
	 * @brief  << �I�y���[�^�̃I�[�o�[���[�h
	 */
	virtual CLogger& operator<<( const uh::tstring& strMessage );

private:
	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() = 0;

protected:

	std::wofstream m_Log;		//!< ���O�t�@�C���X�g���[��

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
	 * @brief FATAL�G���[�t���̃��b�Z�[�W�o��
	 * @param stringTableId [in] StringTable��ID
	 * @param dbPath        [in] �G���[��������DB�p�X
	 * @param layerName     [in] �G���[�����������C����
	 * @param columnName    [in] �G���[���������J������
	 * @param comment       [in] ���̑��o�͂��镶����
	 */
	void fatal( long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment = uh::tstring());

	/**
	 * @brief XML�Ǎ��ݗp�̃G���[�t���̃��b�Z�[�W�o��
	 * @param stringTableId [in] StringTable��ID
	 * @param tag       [in] �G���[�̂���^�O��
	 * @param attrName  [in] �G���[�̂��鑮����
	 * @param attrValue [in] �����Ɏw�肳�ꂽ�l
	 */
	void fatalXML( long stringTableId
		, const uh::tstring& tag
		, const uh::tstring& attrName
		, const uh::tstring& attrValue);

	/**
	 * @brief ERROR�G���[�t���̃��b�Z�[�W�o��
	 * @param stringTableId [in] StringTable��ID
	 * @param dbPath        [in] �G���[��������DB�p�X
	 * @param layerName     [in] �G���[�����������C����
	 * @param columnName    [in] �G���[���������J������
	 * @param comment       [in] ���̑��o�͂��镶����
	 */
	void error( long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment = uh::tstring());

	/**
	 * @brief Warning�G���[�t���̃��b�Z�[�W�o��
	 * @param stringTableId [in] StringTable��ID
	 * @param dbPath        [in] �G���[��������DB�p�X
	 * @param layerName     [in] �G���[�����������C����
	 * @param columnName    [in] �G���[���������J������
	 * @param comment       [in] ���̑��o�͂��镶����
	 */
	void warn( long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment = uh::tstring());

	/**
	 * @brief FATAL�G���[�t���̃��b�Z�[�W�o��
	 * @param[in] format �o�͂��郁�b�Z�[�W�̃t�H�[�}�b�g
	 * (printf�̏����Ɠ���)
	 */
	void fatal( const TCHAR* format, ... );
	/**
	 * @brief �G���[�t���̃��b�Z�[�W�o��
	 * @param[in] format �o�͂��郁�b�Z�[�W�̃t�H�[�}�b�g
	 * (printf�̏����Ɠ���)
	 */
	void error( const TCHAR* format, ... );
	/**
	 * @brief Warning�t���̃��b�Z�[�W�o��
	 * @param[in] format �o�͂��郁�b�Z�[�W�̃t�H�[�}�b�g
	 * (printf�̏����Ɠ���)
	 */
	void warn( const TCHAR* format, ... );
	/**
	 * @brief Info�t���̃��b�Z�[�W�o��
	 * @param[in] format �o�͂��郁�b�Z�[�W�̃t�H�[�}�b�g
	 * (printf�̏����Ɠ���)
	 */
	void info( const TCHAR* format, ... );
	/**
	 * @brief Debug�t���̃��b�Z�[�W�o��
	 * @param[in] format �o�͂��郁�b�Z�[�W�̃t�H�[�}�b�g
	 * (printf�̏����Ɠ���)
	 */
	void debug( const TCHAR* format, ... );

	/**
	 * @brief ���ݎ������o��
	 * @param[in] comment �����̑O�ɑ}������R�����g<br>
	 * "�J�n���� : "�Ȃ�
	 */
	void writeTime( const uh::tstring& comment = uh::tstring() );

	/**
	 * @brief �I�����Ƀ��O�Ɉُ�I���ƒʒm����
	 * @note ERROR�ȏ�ُ̈킪�������ꍇ�ɌĂяo���B
	 */
	void setFail();

protected:
	/**
	 * @brief ���O�ƕW���G���[�o�͂Ɏ��s���ʂ��o�͂��A�I�����������O�ɏo��
	 */
	void endLog();

	/**
	 * @brief �G���[���x���t���̃��b�Z�[�W�o�͂̋��ʕ����`
	 * @param stringTableId [in] StringTable��ID
	 * @param dbPath        [in] �G���[��������DB�p�X
	 * @param layerName     [in] �G���[�����������C����
	 * @param columnName    [in] �G���[���������J������
	 * @param comment       [in] ���̑��o�͂��镶����
	 */
	void writeError( const uh::tstring& strLevel, long stringTableId
		, const uh::tstring& dbPath
		, const uh::tstring& layerName
		, const uh::tstring& columnName
		, const uh::tstring& comment);

	/**
	 * @brief �G���[���x���t���̃��b�Z�[�W�o�͂̋��ʕ����`
	 * @param[in] strLevel �G���[���x��������
	 * @param[in] strMsg   �G���[���b�Z�[�W
	 */
	void writeError( const uh::tstring& strLevel, const uh::tstring& strMsg );

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CRunLogger():m_bFail(false){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CRunLogger(){ endLog(); }

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() override;

	bool m_bFail; //!< �I�����Ƀ��O�Ɉُ�I���Əo�͂��邩�ۂ�
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

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CErrLogger(){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CErrLogger(){}

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
	uh::tstring getDateTimeInfo();
}

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
	 * @brief  �t�@�C�����Ԃ�
	 * @return �t�@�C����
	 */
	virtual uh::tstring getFileName() = 0;

	/**
	 * @brief  �w�b�_�[�o��
	 */
	virtual void writeHeader() = 0;

protected:
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
private:
	/**
	* @brief  �G���[���O�o�͎��̃G���[���x���w��
	*/
	enum ErrLevel
	{
		kFatal,
		kErr,
		kWarn,
		kInfo,
		kDebug
	};

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
	* @brief  �G���[���x���w��̃��b�Z�[�W�o��
	*/
	void Fatal(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Error(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Warn(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Info(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
	void Debug(const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);

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

	/**
	* @brief  �G���[��������
	*         �������x�R�[�h(�ő�)�͎w�肪�Ȃ��Ȃ�΁A���O�֏o�͂��Ȃ�
	* @param[in] err_level    �������ރG���[���x��
	* @param[in] lane_link    �Ώۂ̎Ԑ������N
	* @param[in] road_link    �Ώۂ̓��H�����N
	* @param[in] basemesh     �x�[�X���b�V���ԍ�
	* @param[in] msg          �G���[���b�Z�[�W
	* @param[in] max_speed_c  �Ԑ������N�̍X�V���ꂽ�������x�R�[�h(�ő�)
	*/
	void writeErrRecord(CErrLogger::ErrLevel err_level, const IFeaturePtr& lane_link, const IFeaturePtr& road_link, long basemesh, const CString& msg, long max_speed_c = 0);
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

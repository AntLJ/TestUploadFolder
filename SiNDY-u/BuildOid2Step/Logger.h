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
	 * @param   strDirPath [in] �f�B���N�g���p�X
	 */
	bool Initialize( const uh::tstring& strDirPath );

	/**
	 * @brief  �t�@�C���N���[�Y
	 */
	void Finalize() { m_Log.close(); }

	/**
	 * @brief  ���b�Z�[�W�o��
	 * @param   strMessage [in] ���b�Z�[�W
	 */
	virtual void Log( const uh::tstring& strMessage );

	/**
	 * @brief  ���b�Z�[�W�o��
	 * @param   strMessage [in] ���b�Z�[�W
	 */
	virtual void Log( const TCHAR* format, ... );

	/**
	 * @brief  << �I�y���[�^�̃I�[�o�[���[�h
	 * @param   strMessage [in] ���b�Z�[�W
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
	CRunLogger(void){};

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CRunLogger(void){};

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
	 * @brief  ���b�Z�[�W�o�́iERROR�j
	 * @param layer		[in] ���C��
	 * @param oid		[in] �I�u�W�F�N�gID
	 * @param errMsg	[in] ���b�Z�[�W
	 * @param meshCode	[in] ���b�V���R�[�h
	 */
	void error( const CString& layer, const CString& oid, const CString& errMsg, const CString& meshCode );

	/**
	 * @brief  ���b�Z�[�W�o�́iWARNING�j
	 * @param layer		[in] ���C��
	 * @param oid		[in] �I�u�W�F�N�gID
	 * @param errMsg	[in] ���b�Z�[�W
	 * @param meshCode	[in] ���b�V���R�[�h
	 */
	void warning( const CString& layer, const CString& oid, const CString& errMsg, const CString& meshCode );

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
	 * @brief  �w�b�_�[�o��
	 */
	void writeHeader();

};


namespace log_utility
{
	/**
	 * @brief  �����̏��𕶎���ŕԂ�
	 * @return ������� ( [YYYY/MM/DD HH:MM::SS] )
	 */
	static CString GetDateTimeInfo();
}

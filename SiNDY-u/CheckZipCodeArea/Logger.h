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
	CLogger(void) {}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CLogger(void) { Finalize(); }


public:

	/**
	 * @brief  �t�@�C���I�[�v��
	 */
	bool Initialize(const uh::tstring& strDirPath);

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
	CRunLogger(void) {};

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CRunLogger(void) {};

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
	* @brief SiNDYStdLog�o��
	* @param layer [in] ���C����
	* @param oid [in] �I�u�W�F�N�gID
	* @param x [in] X���W(������)
	* @param y [in] Y���W(������)
	* @param errLevel [in] �G���[���x��
	* @param errCode [in] �G���[�R�[�h
	* @param errMsg [in] �G���[���b�Z�[�W
	* @param info [in] ���
	*/
	void writeStdLog(const CString& layer, const long oid, const CString& x, const CString& y,
		const CString& errLevel, const CString& errCode, const CString& errMsg, const CString& info);

private:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CErrLogger(void) {}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CErrLogger(void) {}

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

#pragma once
#include "ctime.h"

/**
* @brief ���O�Ǘ��N���X�i�V���O���g���j
*/
class LogSys
{

private:
	/**
	 * @brief �R���X�g���N�^
	 */
	LogSys(void);

	/**
	 * @brief �f�X�g���N�^
	 */
	~LogSys(void);

	/**
	 * @brief ���O�̏o�͐ݒ�
	 * @param ofsLog [in] ���O�t�@�C��
	 * @param bCerr [in] �W���o�̗͂L��
	 * @param bTime [in] ���ԏo�̗͂L��
	 * @param lpszMsg [in] �o�͂��镶����
	 */
	inline void WriteLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		std::string curr = GetTimeText();
		if (bCout) {
			if (bTime)
				std::cout << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cout << CT2CA(lpszMsg);
		}
		if (bCerr) {
			if (bTime)
				std::cerr << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cerr << CT2CA(lpszMsg);
		}
		if (ofsLog->is_open()){
			if (bTime)
				*ofsLog << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				*ofsLog << CT2CA(lpszMsg);
		}
	}

public:
	/**
	 * @brief �C���X�^���X�̎擾
	 * @return �C���X�^���X
	 */
	static LogSys& GetInstance(void){
		static LogSys kInstance;
		return kInstance;
	}

	/**
	 * @brief ���O�t�@�C�����쐬
	 * @param cFilePath [in] ���O�t�@�C���̃p�X
	 * @param ofsLogFile [in] ���O�o�̓t�@�C���X�g���[��
	 * @return �����������ǂ���
	 */
	bool SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile);

	/**
	 * @brief ���s���O�t�@�C����ݒ�
	 * @param cFilePath [in] ���s���O�t�@�C���̃p�X
	 * @return �����������ǂ���
	 */
	bool SetProcLog(const _TCHAR *cFilePath);

	/**
	 * @brief �G���[���O�t�@�C����ݒ�
	 * @param cFilePath [in] �G���[���O�t�@�C���̃p�X
	 * @return �����������ǂ���
	 */
	bool SetErrLog(const _TCHAR *cFilePath);

	/**
	 * @brief ���s���O�̏o�͐ݒ�
	 * @param bCout [in] �W���o�̗͂L��
	 * @param bTime [in] ���ԏo�̗͂L��
	 * @param lpszMsg [in] �o�͂��镶����
	 */
	inline void WriteProcLog(bool bCout, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsProcLog, bCout, false, bTime, lpszMsg);
	}

	/**
	 * @brief �G���[���O�̏o�͐ݒ�
	 * @param bCerr [in] �W���o�̗͂L��
	 * @param bTime [in] ���ԏo�̗͂L��
	 * @param lpszMsg [in] �o�͂��镶����
	 */
	inline void WriteErrLog(bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsErrLog, false, bCerr, bTime, lpszMsg);
	}


private:
	std::ofstream m_ofsProcLog;	// ���s���O�o�̓t�@�C���X�g���[��
	std::ofstream m_ofsErrLog;	// �G���[���O�o�̓t�@�C���X�g���[��
};


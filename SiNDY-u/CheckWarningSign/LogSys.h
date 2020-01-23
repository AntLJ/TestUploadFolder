#pragma once

#include "ctime.h"

namespace err_level {
	enum ELevel {
		warning,
		err,
		fatal,
	};
}

/**
* @brief ���O�Ǘ��N���X
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
	 * @param bCout [in] �W���o�̗͂L��
	 * @param bCerr [in] �W���o�̗͂L��
	 * @param bTime [in] ���ԏo�̗͂L��
	 * @param lpszMsg [in] �o�͂��镶����
	 */
	inline void writeLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		std::string curr = getTimeText();
		if (bCout) {
			if (bTime)
				std::cout << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cout << CT2CA(lpszMsg);
			std::cout << std::flush;
		}
		if (bCerr) {
			if (bTime)
				std::cerr << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cerr << CT2CA(lpszMsg);
			std::cerr << std::flush;
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
	static LogSys& getInstance(void){
		static LogSys kInstance;
		return kInstance;
	}

	/**
	 * @brief ���O�t�@�C�����쐬
	 * @param cFilePath [in] ���O�t�@�C���̃p�X
	 * @param ofsLogFile [in] ���O�o�̓t�@�C���X�g���[��
	 * @retval	true ����
	 * @retval	false ���s
	 */
	bool setLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile);

	/**
	 * @brief ���s���O�t�@�C����ݒ�
	 * @param cFilePath [in] ���s���O�t�@�C���̃p�X
	 * @retval	true ����
	 * @retval	false ���s
	 */
	bool setProcLog(const _TCHAR *cFilePath);

	/**
	 * @brief �G���[���O�t�@�C����ݒ�
	 * @param cFilePath [in] �G���[���O�t�@�C���̃p�X
	 * @retval	true ����
	 * @retval	false ���s
	 */
	bool setErrLog(const _TCHAR *cFilePath);


	/**
	 * @brief ���s���O�̏o�͐ݒ�
	 * @param bCout [in] �W���o�̗͂L��
	 * @param bTime [in] ���ԏo�̗͂L��
	 * @param lpszMsg [in] �o�͂��镶����
	 */
	inline void writeProcLog(bool bCout, bool bTime, LPCTSTR lpszMsg)
	{
		writeLog(&m_ofsProcLog, bCout, false, bTime, lpszMsg);
	}

	/**
	 * @brief �G���[���O�̏o�͐ݒ�
	 * @param bCerr [in] �W���o�̗͂L��
	 * @param bTime [in] ���ԏo�̗͂L��
	 * @param lpszMsg [in] �o�͂��镶����
	 */
	inline void writeErrLog(bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		writeLog(&m_ofsErrLog, false, bCerr, bTime, lpszMsg);
	}

	/**
	 * @brief �G���[���x���̐ݒ�
	 * @param bCerr [in] �W���o�̗͂L��
	 * @param eErrLevel [in] �G���[���x��
	 * @param lpszErrMsg [in] �o�͂��镶����
	 * @param lpszLayer [in] �o�͂��郌�C����
	 * @param lObjectId [in] �o�͂���I�u�W�F�N�gID
	 */
	inline void writeFreeStyleLog(bool bCerr, err_level::ELevel eErrLevel, LPCTSTR lpszErrMsg, LPCTSTR lpszLayer = _T("-"), long lObjectId = -1)
	{
		CString strErrLevel;
		switch(eErrLevel){
		case err_level::warning:
			strErrLevel = _T("WARNING");
			break;
		case err_level::err:
			strErrLevel = _T("ERR");
			break;
		case err_level::fatal:
			strErrLevel = _T("FATAL");
			break;
		default:
			strErrLevel = _T("unknown");
		}
		CString strMsg;
		strMsg.Format(_T("0\t%s\t%ld\t%s\t%s\n"), lpszLayer, lObjectId, strErrLevel, lpszErrMsg);

		writeErrLog(bCerr, false, strMsg);
	}

private:
	std::ofstream m_ofsProcLog;	// ���s���O�o�̓t�@�C���X�g���[��
	std::ofstream m_ofsErrLog;	// �G���[���O�o�̓t�@�C���X�g���[��

};


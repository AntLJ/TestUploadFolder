#pragma once

/**
 * @brief ���O�o�͗p�N���X
 */
class CLog
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CLog(){}

	/**
	 * @brief �f�X�g���N�^
	 */
	~CLog(){
		if(m_ofsRunLog)
			m_ofsRunLog.close();
		if(m_ofsErrLog)
			m_ofsErrLog.close();
	}

public:
	/**
	 * @brief ���O�o�̓N���X�̃C���X�^���X�擾
	 * @return �C���X�^���X
	 */
	static CLog& GetInstance(void){
		static CLog cInstance;
		return cInstance;
	}

	/**
	 * @brief ���O�o�͗p�̃t�@�C���X�g���[����ݒ肷��֐�
	 * @param strFilePath [in] ���O�t�@�C���p�X
	 * @param ofsLogFile [out] ���O�t�@�C���X�g���[��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile);

	/**
	 * @brief ���s���O�̃t�@�C���X�g���[����ݒ肷��֐�
	 * @param strFilePath [in] ���s���O�t�@�C���p�X
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool SetRunLog(const CString& strFilePath);

	/**
	 * @brief �G���[���O�̃t�@�C���X�g���[����ݒ肷��֐�
	 * @param strFilePath [in] �G���[���O�t�@�C���p�X
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool SetErrLog(const CString& strFilePath);

	/**
	 * @brief ���b�Z�[�W�o��
	 * @note  ���̊֐���cout,cerr�ɏo�͂����Ă���
	 * @param bCout      [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr      [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param bRun       [in] ���s���O�ɏo�͂��邩�ǂ���
	 * @param bErr       [in] �G���[���O�ɏo�͂��邩�ǂ���
	 * @param strMsg     [in] ���O�o�͗p���b�Z�[�W
	 * @param bLineBreak [in] ���s���邩
	 */
	void PrintLog(bool bCout, bool bCerr, bool bRun, bool bErr, const CString& strMsg, bool bLineBreak = true);

private:
	std::ofstream m_ofsRunLog;	//!< ���s���O�p�t�@�C���X�g���[��
	std::ofstream m_ofsErrLog;	//!< �G���[���O�p�t�@�C���X�g���[��
};

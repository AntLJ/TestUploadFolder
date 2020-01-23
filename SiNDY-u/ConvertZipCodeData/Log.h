#pragma once

// �G���[�^�C�v
namespace err_type {
	enum ECode {
		fatal,
		error,
		warning,
		info,
		debug
	};
}; // err_type

// �G���[�R�[�h
namespace err_code {
	enum ECode {
		// �S��
		NgAddrDBConnectString	= 10001,

		// �X�֔ԍ��ƏZ���R�t��
		NgDuplicateRelAddr		= 20001,
		NgAddrCode				= 20002,
		NgAddr11Dir				= 21001,
		NgAddr11File			= 21002,
		NgAddr11FileOpen		= 21003,
		NgAddr11Record			= 21004,
		NgAddr11Duplicate		= 21005,
		NgAddr11AccCode2		= 21006,

		// �X�֔ԍ��|���S��
		NgZipPriority			= 30001,
		NgOverlapZipValue		= 30002,
		NgSameZipCode			= 30003,

		// �d�S
		NgGetCentroid			= 40001,
		NgGetCentroid2			= 40002,

		// �s�撬����\�_
		NgDuplicateAddrRec		= 50001,
		NgAddrCode5Digit		= 50002,
		NgZipCodeUnder00		= 50003,

		// ArcObjects�֘A
		NgDBConnect				= 80001,
		NgOpenTable				= 80002,
		NgPutWhereClause		= 80003,
		NgGetCursor				= 80004,
		NgGetFields				= 80005,
		NgGetFieldCount			= 80006,
		NgGetField				= 80007,
		NgGetFieldName			= 80008,

		NgGetValue				= 81001,
		NgGetEnumDatasetName	= 81002,
		NgResetEnumDatasetName	= 81003,
		NgGetTableName			= 81004,
		NgDeleteTable			= 81005,
		NgGetShape				= 81006,
		NgGetOID				= 81007,
		NgQueryInterface		= 81008,

		NgPutName				= 82001,
		NgPutType				= 82002,
		NgPutEditable			= 82003,
		NgPutIsNullable			= 82004,
		NgPutLength				= 82005,
		NgPutPrecision			= 82006,
		NgPutSpRef				= 82007,
		NgPutGeoType			= 82008,
		NgPutGeoDef				= 82009,
		NgPutCoords				= 82010,
		NgAddField				= 82101,
		NgCreateTable			= 82201,

		NgInsertCursor			= 83001,
		NgCreateRowBuffer		= 83002,
		NgPutValue				= 83003,
		NgInsertRow				= 83004,
		NgFlush					= 83005,
		NgPutShape				= 83006,

		NgUnion					= 84001,

		// ���̑�
		NgOther					= 99999

	};
}; // err_code

/**
 * @brief ���O�o�͗p�N���X
 * @note  �W���o�́A�W���G���[�o�͂̂�
 */
class CLog {
public:
	CLog(){}
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
	 * @note  ArcObjects�ȊO�ł̃G���[�o�͗p
	 * @param bCout     [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr     [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param bRun      [in] ���s���O�ɏo�͂��邩�ǂ���
	 * @param bErr      [in] �G���[���O�ɏo�͂��邩�ǂ���
	 * @param errType   [in] �G���[�^�C�v
	 * @param errCode   [in] �G���[�R�[�h
	 * @param ...       [in] �ǉ�����
	 */
	void PrintLog1(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			err_code::ECode errCode,
			...);

	/**
	 * @brief ���b�Z�[�W�o��
	 * @note  ArcObjects�ł̃G���[�o�͗p
	 * @param bCout     [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr     [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param bRun      [in] ���s���O�ɏo�͂��邩�ǂ���
	 * @param bErr      [in] �G���[���O�ɏo�͂��邩�ǂ���
	 * @param errType   [in] �G���[�^�C�v
	 * @param errCode   [in] �G���[�R�[�h
	 * @param strTableName	[in] �e�[�u����
	 * @param strOID    [in] �L�[ID
	 * @param ...       [in] �ǉ�����
	 */
	void PrintLog2(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			err_code::ECode errCode,
			const CString& strTableName,
			const CString strOID,
			...);

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
	void PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			const CString& strMsg,
			bool bLineBreak = true);

private:
	/**
	 * @brief �G���[���b�Z�[�W������擾
	 * @param errCode [in] �G���[�R�[�h
	 * @return �G���[�R�[�h�ɑΉ�����G���[���b�Z�[�W������
	 */
	static CString GetErrCodeMsg(err_code::ECode errCode);

	/**
	 * @brief �G���[�^�C�v������擾
	 * @param errType [in] �G���[�^�C�v
	 * @return �G���[�^�C�v�ɑΉ����镶����
	 */
	static CString GetErrTypeMsg(err_type::ECode errType);

private:
	std::ofstream m_ofsRunLog; //!< ���s���O�p�t�@�C���X�g���[��
	std::ofstream m_ofsErrLog; //!< �G���[���O�p�t�@�C���X�g���[��
};

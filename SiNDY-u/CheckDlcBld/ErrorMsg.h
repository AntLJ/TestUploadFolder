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
		NgDlcBldWithin             = 10001, //!< Within���Ă��Ȃ�
		NgBldDlcContains           = 10002, //!< Contains���Ă��Ȃ�
		NgBldDlcTouches            = 10003, //!< Touches���Ă��Ȃ�
		NgBldDlcDisjoint           = 10004, //!< Disjoiont���Ă��Ȃ�
		NgBldDlcCross              = 10005, //!< Cross���Ă��Ȃ�
		DlcBldWithin               = 11001, //!< Within���Ă���
		BldDlcContains             = 11002, //!< Contains���Ă���
		BldDlcTouches              = 11003, //!< Touches���Ă���
		BldDlcDisjoint             = 11004, //!< Disjoiont���Ă���
		BldDlcCross                = 11005, //!< Cross���Ă���
		NgExistBld1                = 20001, //!< �Ώ�Bld����NG
		NgExistBld2                = 20002, //!< �Ώ�Bld����NG�A�א�Bld����NG
		NgExistBld3                = 20003, //!< �Ώ�Bld����NG�A�א�Bld����OK
		NgExistBld4                = 20004, //!< �Ώ�Bld����NG�A�א�Bld����OK
		NgExistBld5                = 20005, //!< �Ώ�Bld�{�א�Bld�`�����OK
		NgBldDlcContTouch          = 21001, //!< �Ώ�Bld����NG
		NgBldDlcContTouchNear      = 30001, //!< �א�Bld����NG
		NgBldDlcRelation           = 40001, //!< Dlc��Bld�̌`��֌W���s��

		NgOther         = 99999

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
	 * @param strFilePath [out] ���O�t�@�C���X�g���[��
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
	 * @note  ArcObjects�ł̃G���[�o�͗p
	 * @param bCout     [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr     [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param bRun      [in] ���s���O�ɏo�͂��邩�ǂ���
	 * @param bErr      [in] �G���[���O�ɏo�͂��邩�ǂ���
	 * @param errType   [in] �G���[�^�C�v
	 * @param strMsg1   [in] �o�̓��b�Z�[�W1
	 * @param strMsg2   [in] �o�̓��b�Z�[�W2
	 * @param strMsg3   [in] �o�̓��b�Z�[�W3
	 */
	void PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			const CString& strMsg1,
			const CString& strMsg2 = _T(""),
			const CString& strMsg3 = _T(""));

	/**
	 * @brief ���b�Z�[�W�o��
	 * @note  �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�����̊֐��Őݒ�
	 * @param bCout        [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr        [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param bRun         [in] ���s���O�ɏo�͂��邩�ǂ���
	 * @param bErr         [in] �G���[���O�ɏo�͂��邩�ǂ���
	 * @param errType      [in] �G���[�^�C�v
	 * @param errCode      [in] �G���[�R�[�h
	 * @param strTableName [in] �t�B�[�`���N���X��
	 * @param lOID         [in] �t�B�[�`���I�u�W�F�N�gID
	 * @param strOtherMsg1 [in] ���̑����b�Z�[�W1
	 * @param strOtherMsg2 [in] ���̑����b�Z�[�W2
	 * @param strOtherMsg3 [in] ���̑����b�Z�[�W3
	 */
	void PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			err_code::ECode errCode,
			const CString& strTableName,
			long lOID,
			const CString& strOtherMsg1 = _T(""),
			const CString& strOtherMsg2 = _T(""),
			const CString& strOtherMsg3 = _T(""));

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


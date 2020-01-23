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
};

// �G���[�R�[�h
namespace err_code {
	enum ECode {
		// ArcObjects�֘A
		NgDBConnect				= 10001,
		NgOpenTable				= 10002,
		NgGetIndex				= 10003,
		NgSetFilter				= 10004,
		NgGetRowCount			= 10005,
		NgGetCursor				= 10006,
		NgGetOID				= 10007,
		NgGetVal				= 10008,
		
		// �ϊ����f�[�^�֘A
		FailOpenInputFile		= 20001,
		NgDictionaryFieldNum	= 20002,
		NgDictionaryFieldVal	= 20003,
		NgDictionaryRecord		= 20004,
		DuplicateDictionary		= 20005,
		DuplicateData			= 20006,
		NoRelData				= 20007,
		NullStrData				= 20008,

		// �o�͐�֘A
		FailDelFile				= 30001,
		NoInited				= 30002,
		FailOpenOutputFile		= 30003,

		// �����񕪊������֘A
		NgSplitFunc				= 40001,
		NgParenConsis			= 40002,
		NgSpClsConsis			= 40003,
		DiffSplitCount			= 40004,
		UnsymmetricalParen		= 40005,
		UnsymmetricalWord		= 40006


	};
}; // err_code

class Log {
public:
	Log(){};
	~Log(){
		if (m_ofsRunLog)
			m_ofsRunLog.close();
		if (m_ofsErrLog)
			m_ofsErrLog.close();
	}

public:
	/**
	* @brief ���O�o�̓N���X�̃C���X�^���X�擾
	* @return �C���X�^���X
	*/
	static Log& GetInstance(void) {
		static Log cInstance;
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
		const long oid,
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
	std::ofstream m_ofsRunLog;
	std::ofstream m_ofsErrLog;
};


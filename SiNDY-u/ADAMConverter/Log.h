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
		// �I�v�V�����֘A

		// �t�@�C���֘A
		NgLoadCnvParam		= 10002,
		NgOpenOutputFile	= 10003,
		NgGetSubFields		= 10006,
		NgEncode			= 10007,
		NgExistField		= 10008,
		
		// �t�H�[�}�b�g�֘A
		NgOpEncode			= 11001,
		NgOpDelimiter		= 11002,
		NgOpNewline			= 11003,
		NgOpGeoref			= 11004,
		NgOpShapeType		= 11005,
		NgOpPolygonType		= 11006,

		// �ϊ��e�[�u���֘A
		NgLoadConvTable				= 12001,
		DuplicateConvTableGroup		= 12002,
		NgConTableTab				= 12003,
		DuplicateConvTableAttribute	= 12004,
		UnknownConvTableAttribute	= 12005,
		NoneConvTableGroup			= 12006,
		NgMatchGroup				= 12007,
		NoneConvTableAttribute		= 12008,
		NoneConvTableGroup2			= 12009,
		NgConvTableFieldName		= 12010,
		UnknownSearchType			= 12011,
		NgOidTableName				= 12012,

		// �ϊ��t�B�[���h�Ή��\�֘A
		NgLoadConvField				= 13001,
		DuplicateConvFieldGroup		= 13002,
		NoneConvFieldGroup			= 13003,
		NgConvFieldTab				= 13004,
		UnknownConvFieldAttribute	= 13005,
		NoneConvFieldSrcFieldComma	= 13006,
		NgConvFieldFieldType		= 13007,
		NgConvFieldLength			= 13008,
		NgConvFieldNull				= 13009,
		DuplicateConvFieldRoundValue= 13010,
		NgConvFieldOptionValue		= 13011,
		DuplicateRange				= 13012,
		NoneRequiredRange			= 13013,
		NgSelectRange				= 13014,
		NgConfigRange				= 13015,
		NoneConvFieldGroup2			= 13016,
		DuplicateDstField			= 13017,
		NgRangeValue				= 13018,
		NgNullValue					= 13019,

		// �o�̓f�[�^�쐬
		NgShapeType					= 30001,
		NgDataType					= 30002,
		NgGeomType					= 30003,
		NgTOKYOtoJGD2000_RP			= 30004,
		NgCoordIntegerDigitOver		= 30005,
		NgPointCoord				= 30006,

		// �o�̓f�[�^�`�F�b�N
		NgCheckValidValue			= 40001,
		NgCheckValidRange			= 40002,
		NgCheckErrValue				= 40003,
		NgCheckErrRange				= 40004,
		NgCheckNull					= 40005,
		NgCheckIntegerDigitOver		= 40006,
		NgCheckStringLength			= 40007,
		NgCheckFieldType			= 40008,

		// ArcObjects�֘A
		NgDBConnect				= 50001,
		NgOpenTable				= 50002,
		NgGetRowCount			= 50003,
		NgGetCursor				= 50004,
		NgQueryDef				= 50005,
		NgJoinTable				= 50006,
		NgSetFilter				= 50007,
		NgEvalute				= 50008,
		NgGetFields				= 50009,
		NgGetFieldCount			= 50010,
		NgGetField				= 50011,
		NgGetGeomDef			= 50012,
		NgGetHasZ				= 50013,
		NgGetHasM				= 50014,
		NgGetGeomType			= 50015,
		NgNextRowCount			= 50016,
		NgGetValueCount			= 50017,
		NgFindField				= 50018,
		NgSetSubFields			= 50019,
		NgSetPostfixClause		= 50020,
		NgSetPrefixClause		= 50021,

		NgGetValue				= 51001,
		NgQueryInterface		= 51002,
		NgGetPointCount			= 51003,
		NgGetPoint				= 51004,
		NgGetExteriorRingCount	= 51005,
		NgQueryExteriorRings	= 51006,
		NgGetInteriorRingCount	= 51007,
		NgQueryInteriorRings	= 51008,
		NgQueryCoord			= 51009,
		NgGetZ					= 51010,
		NgGetM					= 51011,

		// ���̑�
		NgChangeType			= 60001,

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

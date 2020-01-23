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
		NgRWLDuplicateData             = 10001, //!< �f�[�^���d�����Ă��遟
		NgRWLExistFromNodeInRWN        = 10002, //!< FROMNODE��RWN�ɑ��݂��Ȃ���
		NgRWLFromNodeAndRWNGeomSame    = 10003, //!< FROMNODEID��RWN�Ǝn�_�̌`�󂪕s��v��
		NgRWLExistToNodeInRWN          = 10004, //!< TONODEID��RWN�����݂��Ȃ���
		NgRWLToNodeAndRWNGeomSame      = 10005, //!< TONODEID��RWN�ƏI�_�̌`�󂪕s��v��
		NgRWLSmallSegment              = 10006, //!< �����Z�O�����g(%[�Z�O�����g��]�ȉ�)�����݂��遟
		NgRWLSameAttribute             = 10007, //!< ����OBJECTID�Q�̑������s��v��
		NgRWLSequenceDivID             = 10008, //!< ����OBJECTID�Q��DIVID���s�A����
		NgRWLDuplicateVertex           = 10009, //!< �\���_���d�����Ă��遟
		NgRWLDuplicateVertexAllData    = 10010, //!< �S�f�[�^���ō\���_���d�����Ă��遞
		NgRWLDuplicateSegment          = 10011, //!< �S�f�[�^���ŃZ�O�����g���d�����Ă��遟
		NgRWLFirstDivLinkFromDivIDNot0 = 10012, //!< �ŏ���DIVID���������N��FROMDIVID��0�łȂ���
		NgRWLLastDivLinkToDivIDNot0    = 10013, //!< �Ō��DIVID���������N��TODIVID��0�łȂ���
		NgRWLLoopLink                  = 10014, //!< ���[�v�����N�����݂��遟
		NgRWLGroupLink                 = 10015, //!< �O���[�v���ł��Ȃ���

		NgRWNDuplicateData             = 20001, //!< �f�[�^���d�����Ă��遟
		NgRWNDivOIDisMinimumRWLOID     = 20002, //!< DIVID�����m�[�hOBJECTID���ڑ������N�̍ŏ�OBJECTID�ɂȂ��Ă��Ȃ���
		NgRWNSequenceDivID             = 20003, //!< DIVID��������OBJECTID�̃m�[�h�������N�̌�����DIVID���A�����Ă��Ȃ���
		NgRWNDuplicateGeom             = 20004, //!< �`�󂪏d�����Ă��遟
		NgRWNExistInRWL                = 20005, //!< RWL��FROMTONODE�ɑ��݂��Ȃ���
		//NgRWNGeomSameRWL               = 20006, //!< RWL��FROMTONODE�Ɍ`���v���Ȃ���

		NgRSADuplicateData             = 30001, //!< �f�[�^���d�����Ă��遟

		NgRWLExistInBefore             = 11001, //!< �ϊ��O�f�[�^�ɑ��݂��Ȃ���
		NgRWLSameAttrAsBefore          = 11002, //!< �ϊ��O�����ƕs��v��
		NgRWLSameShapeAsBefore         = 11003, //!< �ϊ��O�`��ƕs��v��
		NgRWLSameAttrAsSplitData       = 11004, //!< ������f�[�^�̑������m���s��v��
		NgRWLSameRoadAttrAsSplitData   = 11005, //!< ROADATTR_C���ϊ��O�����ƕs��v�i�����N���������������j��
		NgRWLSameWalkableAsSplitData   = 11006, //!< WALKABLE_C���ϊ��O�����ƕs��v�i���s�҃m�[�h�����E���s�҃����N�}�[�W�����j��

		NgRWNExistInBefore             = 21001, //!< �ϊ��O�f�[�^�ɑ��݂��Ȃ���
		NgRWNSameAttrAsBefore          = 21002, //!< �ϊ��O�����ƕs��v��
		//NgRWNSameShapeAsBefore         = 21003, //!< �ϊ��O�`��ƕs��v��

		NgRWLExistInAfter              = 12001, //!< �ϊ���f�[�^�ɑ��݂��Ȃ���
		NgRWLMergeData                 = 12002, //!< �}�[�W�Ώۃ����N�łȂ���
		NgRWLMergeData2                = 12003, //!< �}�[�W�Ώۃ����N���l�ԋ��p�łȂ���

		NgRWNSameShape                 = 22001, //!< �ϊ���f�[�^�ɓ���`��̃f�[�^�����݂��Ȃ���

		NgHNDuplicateData              = 40001, //!< �f�[�^���d�����Ă��遟
		NgHNExistInRWL                 = 40002, //!< �Ή����郊���N�����݂��Ȃ���a
		NgHNSameHeight                 = 40003, //!< ����LINKIDID�̃����N�Őڑ��_�̍������s��v��a
		//NgHNFrom0Height                = 40004, //!< ����LINKIDID�̃����N�ŘA����Ԃ̊J�n�_�̍�����0�łȂ���a
		//NgHNTo0Height                  = 40005, //!< ����LINKIDID�̃����N�ŘA����Ԃ̏I���_�̍�����0�łȂ���a

		NgHNExistInBefore              = 41001, //!< �ϊ��O�f�[�^�ɑ��݂��Ȃ���a
		//NgHNSameAttrAsBefore           = 41002, //!< �ϊ��O�����ƕs��v��a
		//NgHNSameShapeAsBefore          = 41003, //!< �ϊ��O�`��ƕs��v��

		NgHNExistInAfter               = 42001, //!< �ϊ���f�[�^�ɑ��݂��Ȃ���
		NgHNSameLinkIdAsAfter          = 42002, //!< �ϊ���LINKID�ƕs��v��a
		NgHNSameAttrAsAfter            = 42003, //!< �ϊ��㑮���ƕs��v��a

		NgHNExistInAfterRWN            = 43001, //!< �ϊ���RWN�Ƀf�[�^�����݂��Ȃ���a
		NgHNReconstractHNData          = 43002, //!< �����m�[�h�f�[�^���č\�z�ł��Ȃ���a
		NgHNReconstractNGHNData        = 43003, //!< �����m�[�h�f�[�^���č\�z�����f�[�^���s����a

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
	 * @param lDivID       [in] �t�B�[�`���I�u�W�F�N�gDIVID
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
			long lDivID,
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


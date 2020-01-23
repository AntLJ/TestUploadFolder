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
		info
	};
}; // err_type

// �G���[�R�[�h
namespace err_code {
	enum ECode {
		NgOpenFile          = 1001, //!< �t�@�C���I�[�v�����s
		NgOpenWorkspace     = 1002, //!< ���[�N�X�y�[�X�ڑ����s
		NgOpenTable         = 1003, //!< �e�[�u���擾���s
		NgGetFields	        = 1004, //!< �t�B�[���h���X�g�擾���s
		NgGetField	        = 1005, //!< �t�B�[���h�擾���s
		NgGetRow            = 1006, //!< ���R�[�h�擾���s
		NgGetRowBuffer      = 1007, //!< ���R�[�h�o�b�t�@�擾���s
		NgGetShape          = 1008, //!< �`��擾���s
		//NgGetDomain         = 1009, //!< �h���C���擾���s
		//NgGetDomainField    = 1010, //!< �h���C���t�B�[���h�擾���s
		//NgGetFieldMap       = 1011, //!< �t�B�[���h�}�b�v�擾���s
		NgGetCursor         = 1012, //!< �J�[�\���擾���s
		NgGetAttr           = 1013, //!< �����擾���s
		//NgGetDomainValue    = 1014, //!< �h���C���l�擾���s
		//NgGetDataset        = 1015, //!< �f�[�^�Z�b�g�擾���s
		NgGetFieldType      = 1016, //!< �t�B�[���h�^�C�v�擾���s
		NgGetFieldIndex     = 1017, //!< �t�B�[���h�C���f�b�N�X�擾���s
		NgGetClipShape      = 1018, //!< �N���b�v�`��擾���s
		NgGetGeomType       = 1019, //!< �W�I���g���^�C�v�擾���s
		//NgGetPointCol       = 1020, //!< �|�C���g�R���N�V�����擾���s
		NgGetVertexCount    = 1021, //!< �\���_���擾���s
		//NgGetFieldLength    = 1022, //!< �t�B�[���h���擾���s
		NgGetFieldPrecision = 1023, //!< ���x�擾���s

		NgCreateDir         = 1101, //!< �f�B���N�g���쐬���s
		NgMeshCodeDigit     = 1102, //!< ���b�V���R�[�h�̌����������Ă��Ȃ�
		NgFieldType         = 1103, //!< �t�B�[���h�^�C�v���ϊ��ΏۊO
		NgCreateClone       = 1104, //!< �N���[���쐬�����s
		NgGeomType          = 1105, //!< �W�I���g���^�C�v�����Ή�
		NgIntersect         = 1106, //!< �N���b�s���O���s

		NgMoveFile          = 1201, //!< �t�@�C���ړ����s
		NgCreateTable       = 1301, //!< �e�[�u���쐬���s

		NgPutRowAttr        = 2001, //!< ���R�[�h�����t�^���s
		NgPutRowShape       = 2002, //!< ���R�[�h�`��t�^���s
		NgPutFieldAttr      = 2003, //!< �t�B�[���h�����t�^���s
		NgPutWhere          = 2004, //!< Where��w�莸�s
		//NgPutSubFields      = 2005, //!< SubFields�w�莸�s
		NgPutSpRel          = 2006, //!< �`�󌟍����@�ݒ莸�s
		NgPutRefGeom        = 2007, //!< �����`��ݒ莸�s

		//NgFGDB              = 3001, //!< FGDB�łȂ�
		//NgStartEdit         = 3002, //!< �ҏW�J�n���s
		//NgAbortEdit         = 3003, //!< �ҏW�j�����s
		//NgStopEdit          = 3004, //!< �ҏW�I�����s
		//NgStore             = 3005, //!< Store�������s
		NgFlush             = 3006, //!< Flush�������s

		NgAttrFormatEssField            = 4001, //!< �K�{�����i�ϊ����ƕϊ���j���w�肳��Ă��Ȃ�
		NgAttrFormatOrgFieldEmpty       = 4002, //!< �ϊ����t�B�[���h������
		NgAttrFormatDstFieldEmpty       = 4003, //!< �ϊ���t�B�[���h������
		NgAttrFormatOrgFieldExceptNewOp = 4004, //!< �ϊ����t�B�[���h���ɐV�K�쐬�t�B�[���h�I�v�V�����ȊO���w�肳��Ă���
		NgAttrFormatOrgFieldName        = 4005, //!< �ϊ����t�B�[���h�����݂��Ȃ�
		NgAttrFormatDstFieldSetOp       = 4006, //!< �ϊ���t�B�[���h�ɃI�v�V�������w�肳��Ă���
		NgAttrFormatOrgFieldDuplicate   = 4007, //!< �ϊ���t�B�[���h���d�����Ă���
		NgAttrFormatDstFieldDuplicate   = 4008, //!< �ϊ����t�B�[���h���d�����Ă���
		NgAttrFormatNewOp               = 4009, //!< �V�K�t�B�[���h�I�v�V�����͕ϊ����t�B�[���h���i��1�t�B�[���h�j�݂̂Ɏg�p�\
		NgAttrFormatNoOp                = 4010, //!< �I�v�V�����w��Ȃ��͕ϊ���t�B�[���h���i��2�t�B�[���h�j�̂݋��e
		NgAttrFormatUnknownOp           = 4011, //!< �ݒ�I�v�V�����s��
		NgAttrFormatValue               = 4012, //!< �ݒ�l�̃t�H�[�}�b�g�s��
		NgAttrFormatValueAps            = 4013, //!< �ݒ�l�̃t�H�[�}�b�g�s���i�u'�v���s���j
		NgAttrFormatFieldInfo           = 4101, //!< �t�B�[���h�ݒ�̃t�H�[�}�b�g�s��
		NgAttrFormatUnknownFieldInfo    = 4102, //!< �t�B�[���h�ݒ�̐ݒ荀�ڂ��s��
		NgAttrFormatFieldInfoDuplicate  = 4103, //!< �t�B�[���h�ݒ�̐ݒ荀�ڂ��d��
		NgAttrFormatRound               = 4201, //!< �ۂߍ��ݐݒ�̃t�H�[�}�b�g�s��
		NgAttrFormatRoundAps            = 4202, //!< �ۂߍ��ݐݒ�̃t�H�[�}�b�g�s���i�u'�v�s���j
		NgAttrFormatRoundDuplicate      = 4203, //!< �ۂߍ��ݐݒ�̐ݒ荀�ڂ��d��
		NgAttrFormatDefaultDupicate     = 4301, //!< �����l�ݒ�̐ݒ荀�ڂ��d��
		NgAttrFormatAttrAps             = 4401, //!< �����؂�o���ݒ�̃t�H�[�}�b�g�s���i�u'�v�s���j
		NgAttrFormatAttrDuplicate       = 4402, //!< �����؂�o���ݒ肪�d��
		NgAttrFormatNewDefault          = 4501, //!< �V�K�쐬�t�B�[���h�ɏ����l�ݒ肪����Ă��Ȃ�
		NgAttrFormatNewFieldValue       = 4502, //!< �V�K�쐬�t�B�[���h�ɑ����l�ݒ肪����Ă���
		NgAttrFormatNewRound            = 4503, //!< �V�K�쐬�t�B�[���h�Ɋۂߍ��ݐݒ肪����Ă���
		NgAttrFormatExistDefault        = 4504, //!< �����t�B�[���h�ɏ����l�ݒ肪����Ă���
		NgAttrFormatExistAttr           = 4505, //!< �����t�B�[���h�ɑ����؂�o���l�ݒ肪����Ă���
		NgAttrFormatNewFieldInfoType    = 4506, //!< �V�K�쐬�t�B�[���h�Ƀt�B�[���h�ݒ��TYPE���w�肪����Ă��Ȃ�
		NgAttrFormatExistFieldInfoType  = 4507, //!< �����t�B�[���h�Ƀt�B�[���h�ݒ��TYPE���w�肪����Ă���
		NgAttrFormatFieldInfoType       = 4601, //!< �t�B�[���h�ݒ��TYPE�ݒ�l���s��
		NgAttrFormatFieldInfoTypeValue  = 4602, //!< �t�B�[���h�ݒ�Ō^�ɂ������������w�肳��Ă��Ȃ�
		NgAttrFormatFieldInfoValue      = 4603, //!< �t�B�[���h�ݒ�ő����l���s��
		NgAttrFormatFieldInfoScale      = 4604, //!< �t�B�[���h�ݒ��SCALE�ݒ�l���s��
		NgAttrFormatFieldInfoPrecision  = 4605, //!< �t�B�[���h�ݒ��PRECISION�ݒ�l���s��
		NgAttrFormatFieldInfoLength     = 4606, //!< �t�B�[���h�ݒ��LENGTH�ݒ�l���s��

		NgAddRow            = 6001, //!< ���R�[�h�ǉ����s
		NgAddField          = 6002, //!< �t�B�[���h�ǉ����s
		//NgAddAttr           = 6003, //!< �����ǉ����s
		//NgAddDomain         = 6004, //!< �h���C���ǉ����s

		//NgUpdRow            = 6101, //!< ���R�[�h�X�V���s
		//NgUpdField          = 6102, //!< �t�B�[���h�X�V���s
		//NgUpdAttr           = 6103, //!< �����X�V���s
		//NgUpdDomain         = 6104, //!< �h���C���X�V���s

		//NgDelRow            = 6201, //!< ���R�[�h�폜���s
		//NgDelField          = 6202, //!< �t�B�[���h�폜���s
		//NgDelAttr           = 6203, //!< �����폜���s
		//NgDelDomain         = 6204, //!< �h���C���폜���s
		NgDelFile           = 6205, //!< �t�@�C���폜���s

		//GetManyRow          = 8001, //!< �������R�[�h�擾
		ExistField          = 8002, //!< �����t�B�[���h
		ChangePrecision     = 8201  //!< ���x�ݒ�ύX

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
	static CLog& GetInstance(void){
		static CLog cInstance;
		return cInstance;
	}
	bool SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile);
	bool SetRunLog(const CString& strFilePath);
	bool SetErrLog(const CString& strFilePath);


	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 IFeatureClassPtr ipFeatureClass);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 ITablePtr ipTable);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 IFeatureClassPtr ipFeatureClass,
						 long lOID);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 ITablePtr ipTableClass,
						 long lOID);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 IFeaturePtr ipFeature);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg,
						 _IRowPtr ipRow);

	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strOtherMsg);

	/**
	 * @brief ���b�Z�[�W�o��
	 * @note  �ŏI�I�ȃ��O�o�͕����̃t�H�[�}�b�g�����̊֐��Őݒ�
	 * @param bCout [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param errType [in] �G���[�^�C�v
	 * @param errCode [in] �G���[�R�[�h
	 * @param strTableName [in] �t�B�[�`���N���X��
	 * @param lOID [in] �t�B�[�`���I�u�W�F�N�gID
	 * @param strOtherMsg [in] ���̑����b�Z�[�W
	 */
	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 err_type::ECode errType,
						 err_code::ECode errCode,
						 const CString& strTableName,
						 long lOID,
						 const CString& strOtherMsg);

	/**
	 * @brief ���b�Z�[�W�o��
	 * @note  ���̊֐���cout,cerr�ɏo�͂����Ă���
	 * @param bCout [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 * @param strMsg [in] ���O�o�͗p���b�Z�[�W
	 */
	void PrintLog(bool bCout,
						 bool bCerr,
						 bool bRun,
						 bool bErr,
						 const CString& strMsg);

	/**
	 * @brief �c�[���o�[�W�����o��
	 * @param bCout [in] �W���o�͂ɏo�͂��邩�ǂ���
	 * @param bCerr [in] �W���G���[�o�͂ɏo�͂��邩�ǂ���
	 */
	static void PrintToolVersion(
						bool bCout,
						bool bCerr,
						bool bRun,
						bool bErr);

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


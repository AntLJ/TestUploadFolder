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
#include "Arguments.h"
#include "ConvertTable.h"
#include "OutputFormat.h"
#include "OutputString.h"

/**
 * @brief �f�[�^�ϊ��N���X
 */
class CADAMConverter
{
public:
	CADAMConverter(){}
	~CADAMConverter(){}

public:
	/**
	 * @brief  ������
	 * @param args [in] �R�}���h���C������
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool init(const Arguments& args);

	/**
	 * @brief  �������s
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool run();

private:
	/** 
	 * @brief �t�B�[���h�C���f�b�N�X�X�V
	 * @note �ϊ��e�[�u���ɐݒ肳��Ă���t�B�[���h�C���f�b�N�X���X�V����
	 * @param ipCursor [in] �ϊ����e�[�u���̃J�[�\��
	 * @param cConvertTableInfo [in,out] �ϊ��e�[�u�����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool updateFieldIndex(const _ICursorPtr& ipCursor, CConvTableInfo& cConvertTableInfo);

	/** 
	 * @brief �T�u�t�B�[���h�擾
	 * @note  �ϊ��t�B�[���h�Ή��\�����ɍ쐬����
	 * @param vecFieldDef [in] �ϊ��Ώۃe�[�u���̕ϊ��t�B�[���h�Ή��\
	 * @param oidField [in] �G���[�o�͗p�t�B�[���h��
	 * @param subFields [out] �T�u�t�B�[���h
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getSubFields(const std::vector<CFieldDefInfo>& vecFieldDef, const CString& oidField, CString& subFields);

	/** 
	 * @brief �J�[�\���擾�i�ʏ�e�[�u���Łj
	 * @param cConvTableInfo [in] �ϊ��Ώۂ̕ϊ��e�[�u�����
	 * @param strSubFields [in] �T�u�t�B�[���h
	 * @param ipCursor [out] �擾�J�[�\��
	 * @param size [out] ���R�[�h����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getCursorForNormal(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size);

	/** 
	 * @brief �J�[�\���擾�i�e�[�u�������Łj
	 * @param cConvTableInfo [in] �ϊ��Ώۂ̕ϊ��e�[�u�����
	 * @param strSubFields [in] �T�u�t�B�[���h
	 * @param ipCursor [out] �擾�J�[�\��
	 * @param size [out] ���R�[�h����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getCursorForQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size);

	/** 
	 * @brief �J�[�\���擾�iIQueryDef�g�p�j
	 * @param cConvTableInfo [in] �ϊ��Ώۂ̕ϊ��e�[�u�����
	 * @param strSubFields [in] �T�u�t�B�[���h
	 * @param ipCursor [out] �擾�J�[�\��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getCursorFromQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor);

	/** 
	 * @brief ���R�[�h�����擾�i�e�[�u�������Łj
	 * @param cConvTableInfo [in] �ϊ��Ώۂ̕ϊ��e�[�u�����
	 * @param size [out] ���R�[�h����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getRowCountForQueryDef(const CConvTableInfo& cConvTableInfo, long &size);

	/** 
	 * @brief ZM�l�g�p�t���O�{�`��^�C�v�擾
	 * @param strSrcTableName [in] �ϊ����e�[�u����
	 * @param ipCursor [in] �ϊ��Ώۂ̃J�[�\��
	 * @param isZ [out] Z�l�g�p�t���O
	 * @param isM [out] M�l�g�p�t���O
	 * @param geomType [out] �`��^�C�v
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getHasZM(const CString& strSrcTableName, const _ICursorPtr& ipCursor, bool& isZ, bool& isM, esriGeometryType& geomType);

	/**
	 * @brief �o�̓t�@�C���ɏ����o��
	 * @param ipCursor [in] �o�͕����̃G���R�[�h
	 * @param cOutputString [in] �����o�̓N���X
	 * @param cConvTableInfo [in] ���Ώۂ̕ϊ��e�[�u����
	 * @param ofs [in,out] �o�̓t�@�C���̃X�g���[��
	 * @param size [in] ���R�[�h���i�i�s�󋵏o�͗p�j
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool outputData(const _ICursorPtr& ipCursor, COutputString& cOutputString, const CConvTableInfo& cConvTableInfo, std::ofstream& ofs, long size);
	bool outputUniqData(const _ICursorPtr& ipCursor, COutputString& cOutputString, const CConvTableInfo& cConvTableInfo, std::ofstream& ofs, long size);

	/**
	 * @brief �o�̓t�@�C���ɏ����o��
	 * @param encode [in] �o�͕����̃G���R�[�h
	 * @param strOutputData [in] �o�͕�����
	 * @param ofs [in,out] �o�̓t�@�C���̃X�g���[��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool writeData(const ENCODE::ECode encode, const CString& strOutputData, std::ofstream& ofs);

private:
	CConvertTable m_cConvertTable;	//!< �ϊ����
	COutputFormat m_cOutputFormat;	//!< �o�̓t�H�[�}�b�g
	CString m_strOutputDir;			//!< �o�̓f�B���N�g��
	IWorkspacePtr m_ipSrcWork;		//!< �ϊ������[�N�X�y�[�X
	crd_cnv m_cnv;
};


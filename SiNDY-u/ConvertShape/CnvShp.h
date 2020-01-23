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

#include "global.h"

const long MAX_FILE_SIZE = (long)(1.5 * 1024 * 1024 * 1024);	// 1.5GB = 1,610,612,736 [byte]
const long FETCH_SIZE = 100000;

class CCnvShp
{
public:
	CCnvShp(void);
	CCnvShp(const ITablePtr& ipTable,
		    const std::list<PAIR_COL>& listFieldName,
			const std::map<ORG_COLNAME,std::set<CString>>& mapFieldValue,
			const std::map<ORG_COLNAME,TRANS_VALUE>& mapTransValue,
			const std::map<DEST_COLNAME,CComVariant>& mapDefaultValue,
			const std::map<DEST_COLNAME,std::map<CString,CString>>& mapFieldInfo,
			const std::map<DEST_COLNAME,std::map<CString,CString>>& mapAttrValue,
			const IGeometryPtr& ipGeom,
			const CString& strOut,
			bool bNoShape,
			const CString& strWhereQuery,
			const CString& strWhereLoop,
			const CONVERT_INFO& stCnvInfo
			);
	~CCnvShp(void);

	bool Convert();

	/**
	 * @brief �V�F�[�v�t�@�C�����ύX
	 * @return true  ��������
	 * @return false �������s
	 */
	bool ShapeNameChange(const long lDivideNum, const CString csOutFile);

	/**
	 * @brief �ϊ����擾
	 */
	void GetConvertInfo(CONVERT_INFO& stCnvInfo);

private:
	/**
	 * @brief �o�͗p�f�B���N�g���쐬
	 * @return true  ��������
	 * @return false �������s
	 */
	bool MakeDir();

	/**
	 * @brief �o�͗p�f�B���N�g���쐬
	 * @param ipBaseFeature [in] �N���b�v�Ώۃt�B�[�`��
	 * @param ipBaseGeom [in] �N���b�v�Ώۃt�B�[�`���̌`��
	 * @param ipClipGeom [in] �N���b�v�`��
	 * @param ipOutGeom [out] �N���b�v��̌`��
	 * @return true  ��������
	 * @return false �������s
	 */
	bool GetClipGeom(const IFeaturePtr& ipBaseFeature, const IGeometryPtr& ipBaseGeom, const IGeometryPtr& ipClipGeom, IGeometryPtr& ipOutGeom);

	/**
	 * @brief �V�F�[�v�t�@�C���ړ�
	 * @param strOrgFile [in] �ړ����t�@�C��
	 * @param strDestFile [in] �ړ���t�@�C��
	 * @return true  ��������
	 * @return false �������s
	 */
	bool MoveShape(const CString& strOrgFile, const CString& strDestFile);

	/**
	 * @brief �V�F�[�v�t�@�C���̑������R�[�h�T�C�Y�擾�i1���R�[�h���j
	 * @param listField [in] �t�B�[���h��񃊃X�g
	 * @return 1���R�[�h�̃T�C�Y
	 */
	long GetSizeOneRecord(const std::list<PAIR_FIELD>& listField);

	/**
	 * @brief �V�F�[�v�t�@�C���̌`��̃f�[�^�T�C�Y�擾�i1���R�[�h���j
	 * @param ipRow [in] �����Ώی`��̃t�B�[�`��
	 * @param ipGeom [in] �����Ώی`��
	 * @param lSize [out] �`��f�[�^�̃T�C�Y
	 * @return true  ��������
	 * @return false �������s
	 */
	bool GetSizeGeom(const _IRowPtr& ipRow, const IGeometryPtr& ipGeom, long& lSize);

	/**
	 * @brief �N���[���t�B�[���h�쐬
	 * @param ipTable [in] �N���[�����t�B�[���h�̃e�[�u��
	 * @param ipFields [in] �N���[�����̃t�B�[���h
	 * @param strOrgFieldName [in] �ϊ����̃t�B�[���h����
	 * @param strDstFieldName [in] �ϊ���̃t�B�[���h����
	 * @param ipDstField [out] �N���[���t�B�[���h
	 * @return true  ��������
	 * @return false �������s
	 */
	bool GetOrgCloneField(ITablePtr ipTable, IFieldsPtr ipFields, const CString& strOrgFieldName, const CString& strDstFieldName, IFieldPtr& ipDstField);

	/**
	 * @brief �t�B�[���h�����Z�b�g
	 * @param strOrgFieldName [in] �ϊ����̃t�B�[���h����
	 * @param strDstFieldName [in] �ϊ���̃t�B�[���h����
	 * @param ipDstField [in,out] �����Ώۃt�B�[���h
	 * @return true  ��������
	 * @return false �������s
	 */
	bool SetFieldInfos(const CString& strOrgFieldName, const CString& strDstFieldName, IFieldPtr& ipDstField);

	/**
	 * @brief �t�B�[���h�����Z�b�g
	 * @param strFieldName [in] �t�B�[���h����
	 * @param strFieldInfo [in] �ݒ���
	 * @param strFieldValue [in] �ݒ�l
	 * @param ipDstField [in,out] �����Ώۃt�B�[���h
	 * @return true  ��������
	 * @return false �������s
	 */
	bool SetFieldInfo(const CString& strFieldName, const CString& strFieldInfo, const CString& strFieldValue, IFieldPtr& ipDstField);

	/**
	 * @brief Where��擾
	 * @return Where��
	 */
	CString GetWhere();

private:
	ITablePtr m_ipTable;
	IGeometryPtr m_ipClipGeom;
	CString m_strOutFile;

	std::map<CString,long> m_mapOrgFieldNameIndex;
	
	std::list<PAIR_COL> m_listFieldName;                      //!< �ϊ����E��t�B�[���h�����X�g
	std::map<CString,CString> m_mapOrgDstFieldName;           //!< �ϊ����E��t�B�[���h���Ή��\
	std::map<CString,CString> m_mapDstOrgFieldName;           //!< �ϊ���E���t�B�[���h���Ή��\
	std::map<ORG_COLNAME,std::set<CString>> m_mapFieldValue;	        //!< �����w��pmap
	std::map<ORG_COLNAME,TRANS_VALUE> m_mapTransValue;                  //!< �ۂߍ��ݗpmap
	std::map<DEST_COLNAME,CComVariant> m_mapDefaultValue;               //!< �V�K�쐬�t�B�[���h�̃f�t�H���g�l
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapFieldInfo;    //!< �t�B�[���h�݌v���
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapAttrValue;    //!< �����擾�p

	CString m_strWhereQuery;          //!< �S�̂�Where��
	CString m_strWhereLoop;           //!< �t�B�[�`���擾���[�v���̃t�B���^
	bool m_bNoShape;                  //!< �`����o�͂��邩�ǂ����itrue:�o�͂��Ȃ�, false:�o�͂���j
	CONVERT_INFO m_stCnvInfo;		  //!< �ϊ����
};


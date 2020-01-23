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

/**
 * @brief �V�F�[�v�ASDE�Ȃǂ̃��R�[�h�f�[�^�m�ۗp�̃N���X
 */
class CRecodeBase {
public:
	CRecodeBase(){}
	~CRecodeBase(){}

public:
	/**
	 * @brief �����l�m�ۗp�֐��iIFeature�p�j
	 * @param strTableName         [in] �t�B�[�`���̃e�[�u����
	 * @param ipFeature            [in] �����擾�Ώۃt�B�[�`��
	 * @param mapFieldIndex2Name   [in] �t�B�[���h�C���f�b�N�X�ƃt�B�[���h���̂̃}�b�v
	 * @param mapFieldName2Index   [in] �t�B�[���h���̂ƃt�B�[���h�C���f�b�N�X�̃}�b�v
	 * @retval true  ��������
	 * @retval false �������s
	 */
	virtual bool SetData(const CString& strTableName, IFeaturePtr ipFeature, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index);

	/**
	 * @brief �����l�m�ۗp�֐��iIRow�p�j
	 * @param strTableName         [in] ���R�[�h�̃e�[�u����
	 * @param ipRow                [in] �����擾�Ώۃ��R�[�h
	 * @param mapFieldIndex2Name   [in] �t�B�[���h�C���f�b�N�X�ƃt�B�[���h���̂̃}�b�v
	 * @param mapFieldName2Index   [in] �t�B�[���h���̂ƃt�B�[���h�C���f�b�N�X�̃}�b�v
	 * @param strOIDFieldName      [in] OBJECTID�t�B�[���h�����݂��Ȃ����̑�p�t�B�[���h
	 * @param strDivIDFieldName    [in] OBJECTID�t�B�[���h�����݂��Ȃ����̑�p�t�B�[���h
	 * @retval true  ��������
	 * @retval false �������s
	 */
	virtual bool SetData(const CString& strTableName, _IRowPtr ipRow, const std::map<long,CString>& mapFieldIndex2Name, const std::map<CString,long>& mapFieldName2Index, const std::map<CString,long>& mapAttrName2Index, const CString& strOIDFieldName = _T(""), const CString& strDivIDFieldName = _T(""));

public:
	UNIQID eObjectID;                   //!< OBJECTID�{DIVID
	std::vector<CComVariant> eVecAttr;  //!< �`��ȊO�̑����l�m�ۗp
	std::vector<LONLAT> eVecLonLat;     //!< �ܓx�o�x�ɗ��Ƃ����񂾌`����
};


/**
 * @brief �V�F�[�v�ASDE�Ȃǂ̃e�[�u�����m�ۗp
 * @note ���R�[�h�����m�ۂ���
 */
class CTableBase {
public:
	CTableBase() : m_bBefore(false){}
	~CTableBase(){}

public:
	/**
	 * @brief �������p�֐�
	 * @param bBefore       [in] �ϊ��O���ϊ���e�[�u����
	 * @param ipWork        [in] ���[�N�X�y�[�X
	 * @param strTableName  [in] �e�[�u������
	 * @retval true  ��������
	 * @retval false �������s
	 */
	virtual bool Init(bool bBefore, IWorkspacePtr ipWork, const CString& strTableName);

	/**
	 * @brief �f�[�^�ǂݍ��ݗp�֐�
	 * @note �p����Œ�`������
	 */
	virtual bool LoadData() = 0;

protected:
	/**
	 * @brief �t�B�[���h���擾�֐�
	 * @retval true  ��������
	 * @retval false �������s
	 */
	virtual bool SetFieldInfo(const CString& strTableName, ITablePtr ipTable);

public:
	bool m_bBefore;                                 //!< true:�ϊ��O, false:�ϊ���
	CString m_strTableName;                         //!< �e�[�u�����́ibefore,after�t�j
	IFeatureClassPtr m_ipFeatureClass;              //!< �t�B�[�`���N���X
	ITablePtr m_ipTable;

	std::map<long,CString> m_mapFieldIndex2Name;    //!< �e�[�u���̃t�B�[���h�C���f�b�N�X�ƃt�B�[���h���̂̃}�b�v
	std::map<CString,long> m_mapFieldName2Index;    //!< �e�[�u���̃t�B�[���h���ƃt�B�[���h�C���f�b�N�X�̃}�b�v

	std::map<long,CString> m_mapAttrIndex2Name;     //!< �����m�ۗp�̃t�B�[���h�C���f�b�N�X�ƃt�B�[���h���̂̃}�b�v
	std::map<CString,long> m_mapAttrName2Index;     //!< �����m�ۗp�̃t�B�[���h�C���f�b�N�X�ƃt�B�[���h���̂̃}�b�v

};


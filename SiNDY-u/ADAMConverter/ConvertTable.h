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
#include "FieldDef.h"

/**
 * @brief �������@�^�C�v
 */
namespace SEARCHTYPE {
	enum ECode {
		UNKNOWN_TYPE = 0,	//!< ���ݒ�
		NORMAL,				//!< �ʏ�ł̌���
		QUERYDEF_LIMITED,	//!< QueryDef���g�p�����e�[�u�����������i�@�\����Łj
		QUERYDEF_FREE		//!< QueryDef���g�p�����e�[�u�����������i��������Łj
	};
}

// �ϊ��e�[�u���N���X
class CConvTableInfo {
public:
	CConvTableInfo() : m_searchType(SEARCHTYPE::UNKNOWN_TYPE){}
	~CConvTableInfo(){}

public:
	void clear(){
		m_GroupName.Empty();
		m_searchType = SEARCHTYPE::UNKNOWN_TYPE;
		m_srcTableName.Empty();
		m_dstFileName.Empty();
		m_subTableNames.Empty();
		m_joinKeys.Empty();
		m_oidTableName.Empty();
		m_oidField.Empty();
		m_postfix.Empty();
		m_prefix.Empty();
		m_vecFieldDef.clear();
		m_vecOrderbyField.clear();
	};

	/**
	 * @brief ORDER BY��Ɏw�肳�ꂽ�t�B�[���h���X�g���擾
	 */
	void CreateOrderByFieldList();

public:
	CString m_GroupName;		//!< �O���[�v��
	SEARCHTYPE::ECode m_searchType;	//!< �����^�C�v
	CString m_srcTableName; 	//!< �ϊ����e�[�u����
	CString m_dstFileName;		//!< �ϊ���t�@�C����
	CString m_subTableNames;	//!< �e�[�u�������p�e�[�u����
	CString m_joinKeys;			//!< �ϊ��������L�[
	CString m_oidTableName; 	//!< �G���[�o�͗p��ID����p�e�[�u����
	CString m_oidField;			//!< �G���[�o�͗p��ID����p�t�B�[���h��
	CString m_postfix;			//!< QueryDef->PostfixClause�p
	CString m_prefix;			//!< QueryDef->PrefixClause�p
	std::vector<CFieldDefInfo> m_vecFieldDef;	//!< �ϊ��t�B�[���h�Ή��\
	std::vector<CString> m_vecOrderbyField;	//!< postfix�Ŏw�肳�ꂽORDER BY��̃t�B�[���h���X�g
};

// �ϊ����N���X
class CConvertTable {
public:
	CConvertTable(){}
	~CConvertTable(){}

public:
	/**
	 * @brief ������
	 * @param convTalePath [in] �ϊ��e�[�u���̃p�X
	 * @param convFieldPath [in] �ϊ��t�B�[���h�Ή��\�̃p�X
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool init(const CString& convTablePath, const CString& convFieldPath);

	/**
	 * @brief �ϊ��e�[�u���ƕϊ��t�B�[���h�Ή��\�̃`�F�b�N
	 * @note ���f�[�^�Ƃ��đ��݂��邩�̃`�F�b�N���s��
	 * @param ���[�N�X�y�[�X [in] ipFWork
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool checkConvTableFeild(const IFeatureWorkspacePtr& ipFWork);

private:
	/**
	 * @brief �ϊ��e�[�u���ǂݍ���
	 * @param convTablePath [in] �ϊ��e�[�u���̃p�X
	 * @param mapConvTableInfo [out] �ϊ����Z�b�g
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool loadConvTable(const CString& convTablePath, std::map<CString,CConvTableInfo>& mapConvTableInfo);

	/**
	 * @brief �ϊ��t�B�[���h�Ή��\�ǂݍ���
	 * @param convFieldPath [in] �ϊ��t�B�[���h�Ή��\�̃p�X
	 * @param mapConvTableInfo [out] �ϊ����Z�b�g
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool loadConvField(const CString& convFieldPath, std::map<CString,CConvTableInfo>& mapConvTableInfo);

	/**
	 * @brief �ϊ����O���[�v�`�F�b�N
	 * @note loadTableField,loadConvField�Ŏ擾�����ϊ����Z�b�g�̃O���[�v�ɍ��ق��Ȃ���
	 * @param mapConvTableInfo1 [in] �ϊ����Z�b�g1
	 * @param mapConvTableInfo2 [in] �ϊ����Z�b�g2
	 * @retval true ���قȂ�
	 * @retval false ���ق���
	 */
	bool checkConvGroup(const std::map<CString,CConvTableInfo>& mapConvTableInfo1, const std::map<CString,CConvTableInfo>& mapConvTableInfo2);

	/**
	 * @brief �ϊ��e�[�u���`�F�b�N
	 * @param cConvTableInfo [in] �ϊ����Z�b�g
	 * @retval true �����s���Ȃ�
	 * @retval false �����s������
	 */
	bool checkConvTableInfo(const CConvTableInfo& cConvTableInfo);

	/**
	 * @brief �ϊ��t�B�[���h�Ή��\�`�F�b�N
	 * @param cConvTableInfo [in] �ϊ����Z�b�g
	 * @retval true �����s���Ȃ�
	 * @retval false �����s������
	 */
	bool checkConvFieldInfo(const CConvTableInfo& cConvTableInfo);

public:
	std::map<CString,CConvTableInfo> m_mapConvTableInfo; //!< �ϊ����
};

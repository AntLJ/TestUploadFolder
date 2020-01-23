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
#include "common.h"

/**
 * @brief ���̃f�[�^map�i�[�p�L�[
 */
class RoadNameKey {
public:
	RoadNameKey(const CString& name, const CString& yomi)
		:m_name(name), m_yomi(yomi) {}
	~RoadNameKey() {}
public:
	bool operator < (const RoadNameKey& roadNameKey) const
	{
		return (0 != this->m_name.Compare(roadNameKey.m_name)) ? this->m_name < roadNameKey.m_name :
			this->m_yomi < roadNameKey.m_yomi;
	}
	bool operator == (const RoadNameKey& roadNameKey) const
	{
		return (0 == this->m_name.Compare(roadNameKey.m_name)) &&
			(0 == this->m_yomi.Compare(roadNameKey.m_yomi));
	}
public:
	CString m_name;			//!< ����
	CString m_yomi;			//!< �ǂ�
};

// ������ʃR�[�h
namespace split_class {
	enum ECode {
		kBody = 1,			//!< Body(�ʏ�̖��̂Ɋ��蓖�Ă��鑮��)	
		kDirectional =2,	//!< Directional(���{�̃f�[�^�ł͎g�p����Ȃ�����)
		kParticle = 3,		//!< Particle(���{�̃f�[�^�ł͎g�p����Ȃ�����)
		kClassifier = 4,	//!< Classifier("�����ԓ�"�A"�L�����H"�A"���H"�Ɋ��蓖�Ă��鑮��)
		kConnector = 5,		//!< Connector("�`"�Ɋ��蓖�Ă��鑮��)
		kHonorific = 6,		//!< Honorific(���{�̃f�[�^�ł͎g�p����Ȃ�����)
	};
}

class BaseCnv
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param ipRoadLink [in] ���H�����N�t�B�[�`���N���X
	 * @param ipRoadNameList [in] ���H���̃e�[�u��
	 * @param ipSubNameTable [in] ���H���̈ȊO�ɕK�v�Ȗ��̃e�[�u��
	 */
	BaseCnv(const IFeatureClassPtr& ipRoadLink, const ITablePtr& ipRoadNameList, const ITablePtr& ipSubNameTable)
		:m_ipRoadLink(ipRoadLink), m_ipRoadNameList(ipRoadNameList), m_ipSubNameTable(ipSubNameTable), m_isErr(false) {}
	/*
	 * @brief �f�X�g���N�^
	 */
	~BaseCnv(){}

public:
	/**
	 * @brief �ϊ�����
	 * @note �e��out�ϐ��͏������ɏ����������Ȃ��B(�����N���X�ł̕ϊ����ʂ��܂Ƃ߂Ċi�[�����ꍇ�����邽��)
	 * @param rdNameRecs	[out] ���H���̃f�[�^���R�[�h�ϊ�����
	 * @param rlNameLnkRecs [out] ���H�����N�ɕR�Â����H���̃��R�[�h�ϊ�����
	 * @param spltRdNameRecs [out] ���H���̕����f�[�^���R�[�h�ϊ�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool cnv(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs);

	/**
 	 * @brief ����ɕ����������������Ă�ǂ�����Ԃ��֐�
	 * @retval true: ����I��
	 * @retval false: �����Ɏ��s�����P�[�X�����݂���B
	 */
	bool GetSplitResult() { return !m_isErr; }

protected:
	/**
	 * @brief ������
	 * @retval true ����
	 * @retval false ���s
 	 */
	bool init();

	/** 
	 * @brief ���H���̃��X�g�e�[�u���K�v�C���f�b�N�X�擾
	 * @retval true ����
	 * @retval false ���s
	 */
	bool setRoadNameFieldIndex();

	/**
	 * @brief ���H�����N���C���K�v�C���f�b�N�X�擾
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool setRoadLinkFieldIndex() = 0;

	/**
	 * @brief �T�u�e�[�u���K�v�C���f�b�N�X�擾
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool setSubTblFieldIndex();

	/**
	 * @brief �ϊ��Ώ�road_name_list�̎擾
	 * @retval true ����
	 * @retval false ���s
	 */
	bool loadRoadNameList();

	/**
	 * @brief �ϊ��ΏۃT�u�e�[�u���̎擾
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool loadSubTable();

	/**
	 * @brief �T�u�e�[�u���̊֘ARow�擾
	 * @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	 * @param ipLink [in] �ϊ��Ώۃ����N�t�B�[�`��
	 * @param subOID [out] �T�u�e�[�u���֘ARow��OBJECTID
	 * @param ipSubRow [out] �T�u�e�[�u���̊֘ARow
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow);

	/**
	 * @brief �����Ώۃ��R�[�h���`�F�b�N
	 * @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	 * @param ipLink [in] �ϊ��Ώۃ����N�t�B�[�`��
	 * @param subOID [in] �ϊ��ΏۃT�u���R�[�hOBJECTID
	 * @param ipSubRow [in] �ϊ��ΏۃT�u���R�[�h
	 * @param chkCnv [out] true:�ϊ��Ώ� false:�ϊ��ΏۊO
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv);

	/**
	 * @brief �����N�ɕR�Â����H���̃f�[�^�擾
	 * @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	 * @param ipLink [in] �ϊ��Ώۃ����N�t�B�[�`��
	 * @param subOID [in] �ϊ��ΏۃT�u���R�[�hOBJECTID
	 * @param ipSubRow [in] �ϊ��ΏۃT�u���R�[�h
	 * @param roadNameRecord [out] �ϊ��Ώۖ��̃f�[�^
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord) = 0;

	/**
	 * @brief ���H�����N�ɕR�Â����H���̃f�[�^�ϊ�
	 * @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	 * @param roadNameRecord [in] ���H���̃f�[�^
	 * @param rlNameLnkRecs [out] ���H�����N�ɕR�Â����H���̃��R�[�h�ϊ�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool cnvRelNameLink(const long& linkOID, const RoadNameRecord& roadNameRecord, std::set<RelNameLinkRecord>& rlNameLnkRecs);

	/**
	 * @brief ���H���́E���H���̕����f�[�^�ϊ��ς݃`�F�b�N
	 * @param roadNameRecord [in] ���H���̃f�[�^
	 * @param rdNameRecs [in] ���H���̃f�[�^�ϊ�����
	 * @return ture:�ϊ��ς� false:���ϊ�
	 */
	bool isConverted(const RoadNameRecord& roadNameRecord, const std::set<RoadNameRecord>& rdNameRecs);

	/**
	 * @brief ���H���̃f�[�^�ϊ�
	 * @param roadNameRecord [in] ���H���̃f�[�^
	 * @param rdNameRecs [out] ���H���̃f�[�^�ϊ�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool cnvRoadName(const RoadNameRecord& roadNameRecord, std::set<RoadNameRecord>& rdNameRecs);

	/**
	 * @brief ���H���̕����f�[�^�ϊ�
	 * @param linkOID [in] �ϊ��Ώۃ����NOBJECTID
	 * @param ipLink [in] �ϊ��Ώۃ����N�t�B�[�`��
	 * @param subOID [in] �ϊ��ΏۃT�u���R�[�hOBJECTID
	 * @param ipSubRow [in] �ϊ��ΏۃT�u���R�[�h
	 * @param roadNameRecord [in] ���H���̃f�[�^
	 * @param spltRdNameRecs [out] ���H���̕����f�[�^���R�[�h�ϊ�����
	 */
	virtual bool cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs) = 0;

protected:
	IFeatureClassPtr m_ipRoadLink;		//!< ���H�����N���C��
	ITablePtr m_ipRoadNameList;			//!< ���H���̃��X�g�e�[�u��
	ITablePtr m_ipSubNameTable;			//!< ���H�����N���C���E���H���̃e�[�u���ȊO�ɎQ�Ƃ��閼�̃e�[�u��
	CString m_subNametableName;			//!< ���H�����N���C���E���H���̃e�[�u���ȊO�ɎQ�Ƃ��閼�̃e�[�u���̃e�[�u����
	CString m_roadLinkWhere;			//!< ���H�����N��������

	long m_nameListClsIndex;			//!< ���H���̃��X�g�e�[�u�����̎�ʃC���f�b�N�X
	long m_nameListNameIndex;			//!< ���H���̃��X�g�e�[�u�����̃C���f�b�N�X
	long m_nameListYomiIndex;			//!< ���H���̃��X�g�e�[�u���ǂ݃C���f�b�N�X

	long m_tgtNameClass;				//!< �o�͖��̎��
	
	std::map<RoadNameKey, RoadNameRecord> m_tgtRdNameRecs;		//!< �ϊ��Ώۓ��H���̃��R�[�h

	bool m_isErr;									//!< ����������ɍs��ꂽ�����f����ϐ�(true:����������ɍs���Ă��Ȃ��P�[�X����.false:�S�Đ���ɕ�������Ă���)
};

